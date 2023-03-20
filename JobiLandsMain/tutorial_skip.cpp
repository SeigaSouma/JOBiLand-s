//=============================================================================
//
// スキップ確認処理 [pause.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "tutorial_skip.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "dispatch.h"
#include "buddy.h"
#include "player.h"

//マクロ定義
#define MAX_TOP			(4)				//頂点数
#define POS_XSELECT		(270.0f)		//選択肢のX
#define POS_YSELECT		(100.0f)		//選択肢のY
#define DIS_POSY		(430.0f)		//選択肢の間隔

//グローバル変数宣言
const char *c_apFilenameSkip[] =					//ファイル読み込み
{
	"data\\TEXTURE\\black_fade.png",
	"data\\TEXTURE\\tutorial_skip00.png",		//背景
	"data\\TEXTURE\\tutorial_skip01.png",		//はい
	"data\\TEXTURE\\tutorial_skip02.png",		//いいえ

};

LPDIRECT3DTEXTURE9 g_pTextureSkip[(sizeof c_apFilenameSkip) / sizeof(*c_apFilenameSkip)] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkip = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3	g_posSkipWindow;					//スキップ確認の位置
D3DXVECTOR3	g_posSkipSelect;					//スキップ確認の位置
D3DXCOLOR g_colSkip;							//スキップ確認のカラー
SKIPMENU g_SkipMenu;							//スキップ確認メニューの選択肢
int g_nSkipSelect;								//スキップ確認の現在の選択肢
int g_FadeSkipCount;                            //フェードするカウント

//==================================================================================
//スキップ確認の初期化処理
//==================================================================================
void InitTutorialSkip(void)
{
	g_posSkipWindow = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//ウィンドウ位置
	g_posSkipSelect = D3DXVECTOR3(420.0f, 440.0f, 0.0f);	//選択肢位置
	g_SkipMenu = SKIPMENU_SKIP;					//メニューの初期化
	g_nSkipSelect = SKIPMENU_SKIP;				//メニューの初期化
	g_colSkip = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//スキップ確認のカラー
	g_FadeSkipCount = 0;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSkip) / sizeof(*c_apFilenameSkip); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameSkip[nCntTex],
			&g_pTextureSkip[nCntTex]);

	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * SKIPVTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkip,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSkip->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[SKIPVTX_FADE * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[SKIPVTX_FADE * 4 + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[SKIPVTX_FADE * 4 + 2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[SKIPVTX_FADE * 4 + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[SKIPVTX_FADE * 4 + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[SKIPVTX_FADE * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[SKIPVTX_FADE * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[SKIPVTX_FADE * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	//メニューウィンドウ
	pVtx[SKIPVTX_WINDOW * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[SKIPVTX_WINDOW * 4 + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//スキップする
	pVtx[SKIPVTX_SKIP * 4 + 0].pos = D3DXVECTOR3(g_posSkipSelect.x - POS_XSELECT, g_posSkipSelect.y - POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_SKIP * 4 + 1].pos = D3DXVECTOR3(g_posSkipSelect.x + POS_XSELECT, g_posSkipSelect.y - POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_SKIP * 4 + 2].pos = D3DXVECTOR3(g_posSkipSelect.x - POS_XSELECT, g_posSkipSelect.y + POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_SKIP * 4 + 3].pos = D3DXVECTOR3(g_posSkipSelect.x + POS_XSELECT, g_posSkipSelect.y + POS_YSELECT, 0.0f);
	g_posSkipSelect.x += DIS_POSY;

	//チュートリアル続ける
	pVtx[SKIPVTX_TUTORIAL * 4 + 0].pos = D3DXVECTOR3(g_posSkipSelect.x - POS_XSELECT, g_posSkipSelect.y - POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_TUTORIAL * 4 + 1].pos = D3DXVECTOR3(g_posSkipSelect.x + POS_XSELECT, g_posSkipSelect.y - POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_TUTORIAL * 4 + 2].pos = D3DXVECTOR3(g_posSkipSelect.x - POS_XSELECT, g_posSkipSelect.y + POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_TUTORIAL * 4 + 3].pos = D3DXVECTOR3(g_posSkipSelect.x + POS_XSELECT, g_posSkipSelect.y + POS_YSELECT, 0.0f);

	//pVtx += 4 * SKIPVTX_CONTINUE;

	for (int nCount = 0; nCount < SKIPVTX_MAX; nCount++)
	{
		if (nCount >= SKIPVTX_SKIP)
		{//選択肢まで来たら

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);


		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffSkip->Unlock();
}

//==================================================================================
//スキップ確認の終了処理
//==================================================================================
void UninitTutorialSkip(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameSkip) / sizeof(*c_apFilenameSkip); nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureSkip[nCount] != NULL)
		{
			g_pTextureSkip[nCount]->Release();
			g_pTextureSkip[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSkip != NULL)
	{
		g_pVtxBuffSkip->Release();
		g_pVtxBuffSkip = NULL;
	}
}

//==================================================================================
//スキップ確認の更新処理
//==================================================================================
void UpdateTutorialSkip(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSkip->Lock(0, 0, (void**)&pVtx, 0);

	int nFade = GetFade();

	g_FadeSkipCount++;

	if (nFade == FADE_NONE)
	{//フェードしていない状態

		//選択肢の更新処理
		UpdateTutorialSelect(g_nSkipSelect);

		if (GetKeyboardTrigger(DIK_A) == true || GetGamepadTrigger(BUTTON_LEFT, 0))
		{//左系が押された

			//パターンNo.を更新
			g_nSkipSelect = (g_nSkipSelect + (SKIPMENU_MAX - 1)) % SKIPMENU_MAX;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
		}
		else if (GetKeyboardTrigger(DIK_D) == true || GetGamepadTrigger(BUTTON_RIGHT, 0))
		{//右系が押された

			//パターンNo.を更新
			g_nSkipSelect = (g_nSkipSelect + 1) % SKIPMENU_MAX;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
		}


		if (GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0)
		{//左に倒された, スティックの判定がOFFの場合

			//左スティックの判定を渡す
			SetEnableStickSelect(true);

			//パターンNo.を更新
			g_nSkipSelect = (g_nSkipSelect + (SKIPMENU_MAX - 1)) % SKIPMENU_MAX;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
		}
		else if (GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0)
		{//右に倒された, スティックの判定がOFFの場合

			//左スティックの判定を渡す
			SetEnableStickSelect(true);

			//パターンNo.を更新
			g_nSkipSelect = (g_nSkipSelect + 1) % SKIPMENU_MAX;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
		}


		if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0))
		{//決定が押された

			switch (g_nSkipSelect)
			{
			case SKIPMENU_SKIP:		//スキップする

				//サウンド再生
				PlaySound(SOUND_LABEL_SE_AMURO);

				//ゲーム
				SetVoiceFade(MODE_GAME);

				break;

			case SKIPMENU_TUTORIAL:

				SetTutorialSkip();
				SetEnableSkip(false);		//ポーズを解除する

				break;
			}
		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffSkip->Unlock();



	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//派遣カーソルの情報取得
	DISPATCH *pDispatch = GetDispatch();

	if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
	{//Lが押された && Aボタンが離された

	 //攻撃状態解除
		pBuddy[BUDDYTYPE_HYOROGARI].bATK = false;
		pBuddy[BUDDYTYPE_DEPPA].bATK = false;
		pPlayer->bATK = false;

		//モーションの設定
		SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_DEF);
		SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_DEF);
		SetMotisonPlayer(PLAYERMOTION_DEF);
	}

	if (pDispatch->bUse == true)
	{
		if (GetGamepadRelease(BUTTON_LB + pDispatch->nType, 0) || GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
		{//派遣の使用状況入れ替え

			if (pDispatch->nType == 0)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DISPATCHL);

				//セリフをセット
				//pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });
			}
			else if (pDispatch->nType == 1)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DISPATCHR);

				//セリフをセット
				//pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });
			}

			//派遣する
			SetBuddyDispatch(pDispatch->pos, pDispatch->nType);
			pDispatch->bUse = false;
			pDispatch->nType = -1;
		}
	}

}

//==================================================================================
//スキップ確認中の選択肢更新
//==================================================================================
void UpdateTutorialSelect(int SkipSelect)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSkip->Lock(0, 0, (void**)&pVtx, 0);

	//選択肢までポインタ移動
	pVtx += 4 * SKIPVTX_SKIP;

	static float fMoveAlpha = 0.008f;

	if (g_colSkip.a > 1.0f)
	{//不透明度100%
		g_colSkip.a = 1.0f;
		fMoveAlpha *= -1;
	}
	else if (g_colSkip.a < 0.4f)
	{//不透明度20%
		g_colSkip.a = 0.4f;
		fMoveAlpha *= -1;
	}

	//不透明度の更新
	g_colSkip.a += fMoveAlpha;

	//選択肢から始める
	for (int nCount = SKIPVTX_SKIP; nCount < SKIPVTX_MAX; nCount++)
	{
		if (nCount != SkipSelect + SKIPVTX_SKIP)
		{//今の選択肢じゃないとき
		 //頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			pVtx[0].col = g_colSkip;
			pVtx[1].col = g_colSkip;
			pVtx[2].col = g_colSkip;
			pVtx[3].col = g_colSkip;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSkip->Unlock();
}

//==================================================================================
//スキップ確認の描画処理
//==================================================================================
void DrawTutorialSkip(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ

										//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffSkip, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < SKIPVTX_MAX; nCount++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureSkip[nCount]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);
	}

}

//==================================================================================
//スキップ確認の設定処理
//==================================================================================
void SetTutorialSkip(void)
{
	g_SkipMenu = SKIPMENU_SKIP;		//メニューの初期化
	g_nSkipSelect = SKIPMENU_SKIP;	//選択肢の初期化
}
