//=============================================================================
//
// チュートリアル吹き出し処理 [tutorial_window.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "tutorial_window.h"
#include "tutorial.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"

//マクロ定義
#define MAX_TOP			(4)		//頂点数
#define WIDTH		(640.0f)
#define HEIGHT		(110.0f)
#define SKIP_WIDTH	(125.0f)
#define SKIP_HEIGHT	(25.0f)
#define ALPHA_MOVE	(0.02f)

//グローバル変数宣言
const char *c_apFilenameTutorialWindow[] =		//ファイル読み込み
{
	"data\\TEXTURE\\tutorial_window.png",
	"data\\TEXTURE\\tutorial_step00.png",
	"data\\TEXTURE\\tutorial_step01.png",
	"data\\TEXTURE\\tutorial_step02.png",
	"data\\TEXTURE\\tutorial_step03.png",
	"data\\TEXTURE\\tutorial_step04.png",
	"data\\TEXTURE\\tutorial_step05.png",
	"data\\TEXTURE\\tutorial_step06.png",
	"data\\TEXTURE\\tutorial_step07.png",
	"data\\TEXTURE\\tutorial_step08.png",
	"data\\TEXTURE\\tutorial_step09.png",
	"data\\TEXTURE\\tutorial_skip.png",
};
LPDIRECT3DTEXTURE9 g_pTextureTutorialWindow[(sizeof c_apFilenameTutorialWindow) / sizeof(*c_apFilenameTutorialWindow)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialWindow = NULL;	//頂点バッファへのポインタ
TutorialWindow g_aTutorialWindow;						//チュートリアル吹き出しの情報

//==================================================================================
//チュートリアル吹き出しの初期化処理
//==================================================================================
void InitTutorialWindow(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialWindow) / sizeof(*c_apFilenameTutorialWindow); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTutorialWindow[nCntTex],
			&g_pTextureTutorialWindow[nCntTex]);
	}

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//チュートリアル吹き出しの情報の初期化
	g_aTutorialWindow.pos = D3DXVECTOR3(640.0f, 500.0f, 0.0f);
	g_aTutorialWindow.nStep = pTutorial->nStep;
	g_aTutorialWindow.nOldStep = -1;
	g_aTutorialWindow.fAlpha = 1.0f;
	g_aTutorialWindow.nWaitTime = 0;
	g_aTutorialWindow.nIdxMoveLog = SetMoveLog();	//動く枠のインデックス取得
	g_aTutorialWindow.bMove = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * TUTORIAL_WINDOW_VTX_MAX,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffTutorialWindow,
								NULL);

	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorialWindow = 0; nCntTutorialWindow < TUTORIAL_WINDOW_VTX_MAX; nCntTutorialWindow++)
	{
		if (nCntTutorialWindow == TUTORIAL_WINDOW_VTX_SKIP)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(450.0f + g_aTutorialWindow.pos.x - SKIP_WIDTH, 110.0f + g_aTutorialWindow.pos.y - SKIP_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(450.0f + g_aTutorialWindow.pos.x + SKIP_WIDTH, 110.0f + g_aTutorialWindow.pos.y - SKIP_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450.0f + g_aTutorialWindow.pos.x - SKIP_WIDTH, 110.0f + g_aTutorialWindow.pos.y + SKIP_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(450.0f + g_aTutorialWindow.pos.x + SKIP_WIDTH, 110.0f + g_aTutorialWindow.pos.y + SKIP_HEIGHT, 0.0f);
		}
		else
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - WIDTH, g_aTutorialWindow.pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + WIDTH, g_aTutorialWindow.pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - WIDTH, g_aTutorialWindow.pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + WIDTH, g_aTutorialWindow.pos.y + HEIGHT, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorialWindow->Unlock();
}

//==================================================================================
//チュートリアル吹き出しの終了処理
//==================================================================================
void UninitTutorialWindow(void)
{

	//サウンドの停止
	//StopSound();

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialWindow) / sizeof(*c_apFilenameTutorialWindow); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureTutorialWindow[nCntTex] != NULL)
		{
			g_pTextureTutorialWindow[nCntTex]->Release();
			g_pTextureTutorialWindow[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorialWindow != NULL)
	{
		g_pVtxBuffTutorialWindow->Release();
		g_pVtxBuffTutorialWindow = NULL;
	}
}

//==================================================================================
//チュートリアル吹き出しの更新処理
//==================================================================================
void UpdateTutorialWindow(void)
{

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//プレイヤー情報取得
	Player *pPlayer = GetPlayer();

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorialWindow->Lock(0, 0, (void**)&pVtx, 0);

	//チュートリアルの進行度取得
	g_aTutorialWindow.nStep = pTutorial->nStep;

	if (g_aTutorialWindow.nOldStep != g_aTutorialWindow.nStep)
	{//前回の進行度と違っていたら

		g_aTutorialWindow.nWaitTime = 20;	//待ち時間固定
		g_aTutorialWindow.fAlpha = 1.0f;

		//ロゴの長さリセット
		ResetMoveLog(g_aTutorialWindow.nIdxMoveLog);
		g_aTutorialWindow.nOldStep = g_aTutorialWindow.nStep;

		//サウンド再生
		//PlaySound(SOUND_LABEL_SE_MSGWINDOW);
	}

	if (pPlayer->nNowMotionNum != PLAYERMOTION_DEF)
	{//ニュートラルモーション以外で消していく

		g_aTutorialWindow.nWaitTime--;	//待ち時間加算

		if (g_aTutorialWindow.nWaitTime <= 10)
		{
			g_aTutorialWindow.nWaitTime = 10;	//待ち時間固定
			g_aTutorialWindow.fAlpha -= ALPHA_MOVE;	//不透明度減少

			if (g_aTutorialWindow.fAlpha <= 0.4f)
			{//規定値で固定
				g_aTutorialWindow.fAlpha = 0.4f;
			}
		}
	}
	else
	{//他は出現

		g_aTutorialWindow.nWaitTime++;	//待ち時間加算

		if (g_aTutorialWindow.nWaitTime >= 20)
		{
			g_aTutorialWindow.nWaitTime = 20;	//待ち時間固定
			g_aTutorialWindow.fAlpha += ALPHA_MOVE;	//不透明度増加

			if (g_aTutorialWindow.fAlpha >= 1.0f)
			{//規定値で固定
				g_aTutorialWindow.fAlpha = 1.0f;
			}
		}
	}
	
	//大きくなる→縮む→大きくなって規定値
	float fWidth = MoveLog(g_aTutorialWindow.nIdxMoveLog, WIDTH, 15);
	float fHeight = MoveLog(g_aTutorialWindow.nIdxMoveLog, HEIGHT, 15);

	//頂点座標の設定
	pVtx[TUTORIAL_WINDOW_VTX_WINDOW * 4 + 0].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - fWidth, g_aTutorialWindow.pos.y - fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_WINDOW * 4 + 1].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + fWidth, g_aTutorialWindow.pos.y - fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_WINDOW * 4 + 2].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - fWidth, g_aTutorialWindow.pos.y + fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_WINDOW * 4 + 3].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + fWidth, g_aTutorialWindow.pos.y + fHeight, 0.0f);

	pVtx[TUTORIAL_WINDOW_VTX_TEXT * 4 + 0].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - fWidth, g_aTutorialWindow.pos.y - fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_TEXT * 4 + 1].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + fWidth, g_aTutorialWindow.pos.y - fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_TEXT * 4 + 2].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - fWidth, g_aTutorialWindow.pos.y + fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_TEXT * 4 + 3].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + fWidth, g_aTutorialWindow.pos.y + fHeight, 0.0f);


	for (int nCntTutorialWindow = 0; nCntTutorialWindow < TUTORIAL_WINDOW_VTX_MAX; nCntTutorialWindow++)
	{
		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aTutorialWindow.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aTutorialWindow.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aTutorialWindow.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aTutorialWindow.fAlpha);

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorialWindow->Unlock();

}

//==================================================================================
//チュートリアル吹き出しの描画処理
//==================================================================================
void DrawTutorialWindow(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialWindow, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorialWindow[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//テキストの描画
	for (int nCntTutorialWindow = TUTORIAL_WINDOW_VTX_TEXT; nCntTutorialWindow < TUTORIAL_WINDOW_VTX_SKIP; nCntTutorialWindow++)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTutorialWindow[g_aTutorialWindow.nStep + TUTORIAL_WINDOW_VTX_TEXT]);
	
		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorialWindow * 4, 2);
	}

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureTutorialWindow[((sizeof c_apFilenameTutorialWindow) / sizeof(*c_apFilenameTutorialWindow)) - 1]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, TUTORIAL_WINDOW_VTX_SKIP * 4, 2);
}

//==================================================================================
//チュートリアル吹き出し情報取得
//==================================================================================
TutorialWindow *GetTutorialWindow(void)
{
	return &g_aTutorialWindow;
}