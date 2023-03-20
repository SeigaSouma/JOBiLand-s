//=============================================================================
//
// ポーズ処理 [pause.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "player.h"

//マクロ定義
#define MAX_TOP			(4)				//頂点数
#define POS_XSELECT		(270.0f)		//選択肢のX
#define POS_YSELECT		(100.0f)		//選択肢のY
#define DIS_POSY		(180.0f)		//選択肢の間隔

//グローバル変数宣言
const char *c_apFilenamePause[] =					//ファイル読み込み
{
	"data\\TEXTURE\\black_fade.png",
	"data\\TEXTURE\\pause_menu.png",
	"data\\TEXTURE\\pause_game.png",
	"data\\TEXTURE\\pause_retry.png",
	"data\\TEXTURE\\pause_title.png",
};

LPDIRECT3DTEXTURE9 g_pTexturePause[(sizeof c_apFilenamePause) / sizeof(*c_apFilenamePause)] = {};//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//頂点バッファへのポインタ
D3DXVECTOR3	g_posWindow;						//ポーズの位置
D3DXVECTOR3	g_posSelect;						//ポーズの位置
D3DXCOLOR g_colPause;							//ポーズのカラー
PAUSEMENU g_PauseMenu;							//ポーズメニューの選択肢
int g_nPauseSelect;								//ポーズの現在の選択肢
int g_FadeCount;                                //フェードするカウント

//==================================================================================
//ポーズの初期化処理
//==================================================================================
void InitPause(void)
{
	g_posWindow = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//ウィンドウ位置
	g_posSelect = D3DXVECTOR3(640.0f, 240.0f, 0.0f);	//選択肢位置
	g_PauseMenu = PAUSEMENU_CONTINUE;					//メニューの初期化
	g_nPauseSelect = PAUSEMENU_CONTINUE;				//メニューの初期化
	g_colPause = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//ポーズのカラー
	g_FadeCount = 0;

														//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenamePause) / sizeof(*c_apFilenamePause); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenamePause[nCntTex],
			&g_pTexturePause[nCntTex]);

	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * PAUSEVTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[PAUSEVTX_FADE * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[PAUSEVTX_FADE * 4 + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[PAUSEVTX_FADE * 4 + 2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[PAUSEVTX_FADE * 4 + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[PAUSEVTX_FADE * 4 + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[PAUSEVTX_FADE * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[PAUSEVTX_FADE * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[PAUSEVTX_FADE * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	//メニューウィンドウ
	pVtx[PAUSEVTX_WINDOW * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[PAUSEVTX_WINDOW * 4 + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//ゲームに戻る選択肢
	pVtx[PAUSEVTX_CONTINUE * 4 + 0].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_CONTINUE * 4 + 1].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_CONTINUE * 4 + 2].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_CONTINUE * 4 + 3].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	g_posSelect.y += DIS_POSY;

	//リトライの選択肢
	pVtx[PAUSEVTX_RETRY * 4 + 0].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_RETRY * 4 + 1].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_RETRY * 4 + 2].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_RETRY * 4 + 3].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	g_posSelect.y += DIS_POSY;

	//タイトルに戻るの選択肢
	pVtx[PAUSEVTX_QUIT * 4 + 0].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_QUIT * 4 + 1].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_QUIT * 4 + 2].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_QUIT * 4 + 3].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);

	//pVtx += 4 * PAUSEVTX_CONTINUE;

	for (int nCount = 0; nCount < PAUSEVTX_MAX; nCount++)
	{
		if (nCount >= PAUSEVTX_CONTINUE)
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
	g_pVtxBuffPause->Unlock();
}

//==================================================================================
//ポーズの終了処理
//==================================================================================
void UninitPause(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenamePause) / sizeof(*c_apFilenamePause); nCount++)
	{
		//テクスチャの破棄
		if (g_pTexturePause[nCount] != NULL)
		{
			g_pTexturePause[nCount]->Release();
			g_pTexturePause[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//==================================================================================
//ポーズの更新処理
//==================================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	int nFade = GetFade();

	g_FadeCount++;

	if (nFade == FADE_NONE)
	{//フェードしていない状態

	 //選択肢の更新処理
		UpdateSelect(g_nPauseSelect);

		if (GetKeyboardTrigger(DIK_W) == true || GetGamepadTrigger(BUTTON_UP, 0))
		{//上系が押された

		 //パターンNo.を更新
			g_nPauseSelect = (g_nPauseSelect + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}
		else if (GetKeyboardTrigger(DIK_S) == true || GetGamepadTrigger(BUTTON_DOWN, 0))
		{//下系が押された

		 //パターンNo.を更新
			g_nPauseSelect = (g_nPauseSelect + 1) % PAUSEMENU_MAX;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}


		if (GetStickSelect() == false && YGetStickPressL(BUTTON_LY, 0) > 0)
		{//上に倒された, スティックの判定がOFFの場合

		 //左スティックの判定を渡す
			SetEnableStickSelect(true);

			//パターンNo.を更新
			g_nPauseSelect = (g_nPauseSelect + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}
		else if (GetStickSelect() == false && YGetStickPressL(BUTTON_LY, 0) < 0)
		{//下に倒された, スティックの判定がOFFの場合

		 //左スティックの判定を渡す
			SetEnableStickSelect(true);

			//パターンNo.を更新
			g_nPauseSelect = (g_nPauseSelect + 1) % PAUSEMENU_MAX;

			//サウンド再生
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}


		if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0))
		{//決定が押された

			switch (g_nPauseSelect)
			{
			case PAUSEMENU_CONTINUE:
				SetPause();
				SetEnablePause(false);		//ポーズを解除する
				break;

			case PAUSEMENU_RETRY:

				//サウンド再生
				//PlaySound(SOUND_LABEL_SE_AMURO);

				//ゲームをリトライする
				SetVoiceFade(MODE_GAME);

				
				break;

			case PAUSEMENU_LEVELSELECT:

				//サウンド再生
				//PlaySound(SOUND_LABEL_SE_MODORU);

				//ゲームをリトライする
				SetVoiceFade(MODE_TITLE);
					
				break;

			}
		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();

}

//==================================================================================
//ポーズ中の選択肢更新
//==================================================================================
void UpdateSelect(int PauseSelect)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//選択肢までポインタ移動
	pVtx += 4 * PAUSEVTX_CONTINUE;

	static float fMoveAlpha = 0.008f;

	if (g_colPause.a > 1.0f)
	{//不透明度100%
		g_colPause.a = 1.0f;
		fMoveAlpha *= -1;
	}
	else if (g_colPause.a < 0.4f)
	{//不透明度20%
		g_colPause.a = 0.4f;
		fMoveAlpha *= -1;
	}

	//不透明度の更新
	g_colPause.a += fMoveAlpha;

	//選択肢から始める
	for (int nCount = PAUSEVTX_CONTINUE; nCount < PAUSEVTX_MAX; nCount++)
	{
		if (nCount != PauseSelect + PAUSEVTX_CONTINUE)
		{//今の選択肢じゃないとき
		 //頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			pVtx[0].col = g_colPause;
			pVtx[1].col = g_colPause;
			pVtx[2].col = g_colPause;
			pVtx[3].col = g_colPause;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPause->Unlock();
}

//==================================================================================
//ポーズの描画処理
//==================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスへのポインタ

										//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < PAUSEVTX_MAX; nCount++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTexturePause[nCount]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);
	}

}

//==================================================================================
//ポーズの設定処理
//==================================================================================
void SetPause(void)
{
	g_PauseMenu = PAUSEMENU_CONTINUE;		//メニューの初期化
	g_nPauseSelect = PAUSEMENU_CONTINUE;	//選択肢の初期化
}
