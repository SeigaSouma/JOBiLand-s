//=============================================================================
//
// チュートリアルの会話処理 [tutorial_talk.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "tutorial_talk.h"
#include "tutorial.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
#include "player.h"
#include "buddy.h"
#include "dispatch.h"

//マクロ定義
#define MAX_TOP			(4)		//頂点数
#define WIDTH			(640.0f)
#define HEIGHT			(360.0f)
#define OK_WIDTH		(250.0f)
#define OK_HEIGHT		(80.0f)
#define SET_TIME		(80)
#define ENTER_COOLTIME	(20)

//プロトタイプ宣言
void CreatVtxTutorialTalk(void);
void CreatVtxTutorialOK(void);
void UpdateTutorialOK(void);

//グローバル変数宣言
const char *c_apFilenameTutorialTalk[] =		//ファイル読み込み
{
	NULL,
	"data\\TEXTURE\\tutorial_talk01.png",
	"data\\TEXTURE\\tutorial_talk02.png",
	"data\\TEXTURE\\tutorial_talk03.png",
	"data\\TEXTURE\\tutorial_talk04.png",
	"data\\TEXTURE\\tutorial_talk05.png",
	"data\\TEXTURE\\tutorial_talk06.png",
	"data\\TEXTURE\\tutorial_talk07.png",
	"data\\TEXTURE\\tutorial_talk08.png",
	"data\\TEXTURE\\tutorial_talk09.png",
};

const char *c_apFilenameTutorialOK[] =		//ファイル読み込み
{
	"data\\TEXTURE\\tutorial_OK.png",
};
LPDIRECT3DTEXTURE9 g_pTextureTutorialTalk[(sizeof c_apFilenameTutorialTalk) / sizeof(*c_apFilenameTutorialTalk)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialTalk = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTutorialOK[(sizeof c_apFilenameTutorialOK) / sizeof(*c_apFilenameTutorialOK)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialOK = NULL;	//頂点バッファへのポインタ
TutorialTalk g_aTutorialTalk;						//チュートリアルの会話の情報
TutorialOK g_aTutorialOK;						//チュートリアルの完了の情報

//==================================================================================
//チュートリアルの会話の初期化処理
//==================================================================================
void InitTutorialTalk(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialTalk) / sizeof(*c_apFilenameTutorialTalk); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTutorialTalk[nCntTex],
			&g_pTextureTutorialTalk[nCntTex]);
	}

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialOK) / sizeof(*c_apFilenameTutorialOK); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTutorialOK[nCntTex],
			&g_pTextureTutorialOK[nCntTex]);
	}

	CreatVtxTutorialTalk();
	CreatVtxTutorialOK();
}

//==================================================================================
//チュートリアルの会話の終了処理
//==================================================================================
void UninitTutorialTalk(void)
{

	//サウンドの停止
	//StopSound();

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialTalk) / sizeof(*c_apFilenameTutorialTalk); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureTutorialTalk[nCntTex] != NULL)
		{
			g_pTextureTutorialTalk[nCntTex]->Release();
			g_pTextureTutorialTalk[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorialTalk != NULL)
	{
		g_pVtxBuffTutorialTalk->Release();
		g_pVtxBuffTutorialTalk = NULL;
	}

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialOK) / sizeof(*c_apFilenameTutorialOK); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureTutorialOK[nCntTex] != NULL)
		{
			g_pTextureTutorialOK[nCntTex]->Release();
			g_pTextureTutorialOK[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorialOK != NULL)
	{
		g_pVtxBuffTutorialOK->Release();
		g_pVtxBuffTutorialOK = NULL;
	}
}

//==================================================================================
//チュートリアルの会話の更新処理
//==================================================================================
void UpdateTutorialTalk(void)
{
	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//派遣カーソルの情報取得
	DISPATCH *pDispatch = GetDispatch();

	//ステップ同期
	g_aTutorialTalk.nStep = pTutorial->nStep;

	if (g_aTutorialTalk.bSet == true && g_aTutorialTalk.bUse == false)
	{
		//設定時間減算
		g_aTutorialTalk.nCntSet--;

		if (g_aTutorialTalk.nCntSet <= 0)
		{//時間経過

			g_aTutorialTalk.bSet = false;
			g_aTutorialTalk.bUse = true;
			g_aTutorialOK.bUse = false;

			PlaySound(SOUND_LABEL_SE_TUTORIALWINDOW);
		}
	}

	if (g_aTutorialTalk.bUse == true)
	{//使用していたら

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
				}
				else if (pDispatch->nType == 1)
				{
					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_DISPATCHR);
				}

				//派遣する
				SetBuddyDispatch(pDispatch->pos, pDispatch->nType);
				pDispatch->bUse = false;
				pDispatch->nType = -1;
			}
		}

		//クールタイム加算
		g_aTutorialTalk.nCntEnter++;

		if ((GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0)) &&
			g_aTutorialTalk.nCntEnter >= ENTER_COOLTIME)
		{//エンターキーが押された時

			pTutorial->nStep += 1;
			g_aTutorialTalk.bUse = false;
			g_aTutorialTalk.nCntEnter = 0;
		}
	}

	//OKの更新処理
	UpdateTutorialOK();

}

//==================================================================================
//チュートリアルの完了の更新処理
//==================================================================================
void UpdateTutorialOK(void)
{

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialOK->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aTutorialOK.bUse == true)
	{//使用されていたら

		//チュートリアルの会話の情報の初期化
		g_aTutorialOK.pos.y += (360.0f - g_aTutorialOK.pos.y) * 0.15f;
		g_aTutorialOK.col.a += (1.0f - g_aTutorialOK.col.a) * 0.15f;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aTutorialOK.col;
		pVtx[1].col = g_aTutorialOK.col;
		pVtx[2].col = g_aTutorialOK.col;
		pVtx[3].col = g_aTutorialOK.col;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorialOK->Unlock();
}

//==================================================================================
//チュートリアルの会話の描画処理
//==================================================================================
void DrawTutorialTalk(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialOK, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aTutorialOK.bUse == true)
	{//使用していたら

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorialOK[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}



	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialTalk, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	if (g_aTutorialTalk.bUse == true)
	{//使用していたら

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorialTalk[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//テキストの描画
		for (int nCntTutorialTalk = TUTORIAL_TALK_VTX_TEXT; nCntTutorialTalk < TUTORIAL_TALK_VTX_MAX; nCntTutorialTalk++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTutorialTalk[g_aTutorialTalk.nStep + TUTORIAL_TALK_VTX_TEXT]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorialTalk * 4, 2);
		}
	}
}

//==================================================================================
//チュートリアルの会話の設定処理
//==================================================================================
void SetTutorialTalk(void)
{
	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	if (g_aTutorialTalk.bUse == false && g_aTutorialTalk.bSet == false)
	{//設定していなかったら

		g_aTutorialTalk.bSet = true;
		g_aTutorialTalk.nCntSet = SET_TIME;
		g_aTutorialTalk.nCntEnter = 0;

		if (pTutorial->nStep >= TUTORIAL_STEP_MOVE)
		{
			SetTutorialOK();
			PlaySound(SOUND_LABEL_SE_TUTORIALCLEAR);
		}
	}
}

//==================================================================================
//チュートリアルの完了の設定処理
//==================================================================================
void SetTutorialOK(void)
{

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialOK->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aTutorialOK.bUse == false)
	{
		//チュートリアルの会話の情報の初期化
		g_aTutorialOK.pos = D3DXVECTOR3(640.0f, 650.0f, 0.0f);
		g_aTutorialOK.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_aTutorialOK.bUse = true;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aTutorialOK.col;
		pVtx[1].col = g_aTutorialOK.col;
		pVtx[2].col = g_aTutorialOK.col;
		pVtx[3].col = g_aTutorialOK.col;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorialOK->Unlock();
}

//==================================================================================
//チュートリアルの会話情報取得
//==================================================================================
void CreatVtxTutorialTalk(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//チュートリアルの会話の情報の初期化
	g_aTutorialTalk.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	g_aTutorialTalk.nStep = pTutorial->nStep;
	g_aTutorialTalk.nCntSet = 0;	//設定のカウント
	g_aTutorialTalk.nCntEnter = 0;	//Enter押すまでのクールタイム
	g_aTutorialTalk.bSet = false;
	g_aTutorialTalk.bUse = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * TUTORIAL_TALK_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialTalk,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialTalk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorialTalk = 0; nCntTutorialTalk < TUTORIAL_TALK_VTX_MAX; nCntTutorialTalk++)
	{
		if (nCntTutorialTalk == TUTORIAL_TALK_VTX_FADE)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x - 640.0f, g_aTutorialTalk.pos.y - 360.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x + 640.0f, g_aTutorialTalk.pos.y - 360.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x - 640.0f, g_aTutorialTalk.pos.y + 360.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x + 640.0f, g_aTutorialTalk.pos.y + 360.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
		}
		else
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x - WIDTH, g_aTutorialTalk.pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x + WIDTH, g_aTutorialTalk.pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x - WIDTH, g_aTutorialTalk.pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x + WIDTH, g_aTutorialTalk.pos.y + HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
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

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorialTalk->Unlock();
}

//==================================================================================
//チュートリアルの会話情報取得
//==================================================================================
void CreatVtxTutorialOK(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//チュートリアルの会話の情報の初期化
	g_aTutorialOK.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	g_aTutorialOK.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorialOK.bUse = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialOK,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialOK->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);

	//頂点カラーの設定
	pVtx[0].col = g_aTutorialOK.col;
	pVtx[1].col = g_aTutorialOK.col;
	pVtx[2].col = g_aTutorialOK.col;
	pVtx[3].col = g_aTutorialOK.col;

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

	//頂点バッファをアンロックする
	g_pVtxBuffTutorialOK->Unlock();
}

//==================================================================================
//チュートリアルの会話情報取得
//==================================================================================
TutorialTalk *GetTutorialTalk(void)
{
	return &g_aTutorialTalk;
}