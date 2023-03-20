//=============================================================================
//
// ロゴ処理 [logo.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "logo.h"
#include "fade.h"

#define MIN_ALPHA	(0.4f)		//不透明度最小値
#define MAX_ALPHA	(1.0f)		//不透明度最大値
#define CHANGE_TIME	(60 * 3)	//切り替えタイマー
#define LOG_WIDTH	(640.0f)
#define LOG_HEIGHT	(LOG_WIDTH * 0.2f)
#define LOG_MINI_WIDTH	(150.0f * 0.5f)
#define LOG_MINI_HEIGHT	(200.0f * 0.5f)

//グローバル変数宣言
const char *c_apFilenameLogo[] =					//ファイル読み込み
{
	NULL,
	"data\\TEXTURE\\teamLog_01.png",
};
LPDIRECT3DTEXTURE9 g_pTextureLogo[(sizeof c_apFilenameLogo) / sizeof(*c_apFilenameLogo)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo = NULL;		//頂点バッファへのポインタ
Logo g_Logo[LOGOVTX_MAX];

//==================================================================================
//ロゴの初期化処理
//==================================================================================
void InitLogo(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameLogo) / sizeof(*c_apFilenameLogo); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameLogo[nCntTex],
			&g_pTextureLogo[nCntTex]);
	}

	//各種変数初期化
	for (int nCntLogo = 0; nCntLogo < LOGOVTX_MAX; nCntLogo++)
	{
		g_Logo[nCntLogo].nCntChange = 0;	//切り替わる時間
		g_Logo[nCntLogo].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
		g_Logo[nCntLogo].pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//位置
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOGOVTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntLogo = 0; nCntLogo < LOGOVTX_MAX; nCntLogo++)
	{
		//頂点座標の設定
		switch (nCntLogo)
		{
		case LOGOVTX_WHITE:
			pVtx[0].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x - 640.0f, g_Logo[nCntLogo].pos.y - 360.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x + 640.0f, g_Logo[nCntLogo].pos.y - 360.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x - 640.0f, g_Logo[nCntLogo].pos.y + 360.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x + 640.0f, g_Logo[nCntLogo].pos.y + 360.0f, 0.0f);
			break;

		case LOGOVTX_LOG:
			pVtx[0].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x - LOG_WIDTH, g_Logo[nCntLogo].pos.y - LOG_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x + LOG_WIDTH, g_Logo[nCntLogo].pos.y - LOG_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x - LOG_WIDTH, g_Logo[nCntLogo].pos.y + LOG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x + LOG_WIDTH, g_Logo[nCntLogo].pos.y + LOG_HEIGHT, 0.0f);
			break;
		}

		//頂点カラーの設定
		pVtx[0].col = g_Logo[nCntLogo].col;
		pVtx[1].col = g_Logo[nCntLogo].col;
		pVtx[2].col = g_Logo[nCntLogo].col;
		pVtx[3].col = g_Logo[nCntLogo].col;

		//rhwの設定
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffLogo->Unlock();
}

//==================================================================================
//ロゴの終了処理
//==================================================================================
void UninitLogo(void)
{

	for (int nCntLogo = 0; nCntLogo < (sizeof c_apFilenameLogo) / sizeof(*c_apFilenameLogo); nCntLogo++)
	{
		if (g_pTextureLogo[nCntLogo] != NULL)
		{
			g_pTextureLogo[nCntLogo]->Release();
			g_pTextureLogo[nCntLogo] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffLogo != NULL)
	{
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}

}

//==================================================================================
//ロゴの更新処理
//==================================================================================
void UpdateLogo(void)
{
	if (GetFade() == FADE_NONE)
	{//何もしていないとき

		//自動遷移のカウンター加算
		g_Logo[0].nCntChange++;

		if (GetKeyboardTrigger(DIK_RETURN) == true ||
			GetGamepadTrigger(BUTTON_A, 0) ||
			GetGamepadTrigger(BUTTON_START, 0) || g_Logo[0].nCntChange >= CHANGE_TIME)
		{//Enterが押された

			SetFade(MODE_TITLE);
		}
	}
}

//==================================================================================
//ロゴの描画処理
//==================================================================================
void DrawLogo(int nType)
{
	if (nType == DRAWTYPE_UI)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntLogo = 0; nCntLogo < LOGOVTX_MAX; nCntLogo++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureLogo[nCntLogo]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLogo * 4, 2);
		}
	}
}

//==================================================================================
//ロゴの情報取得
//==================================================================================
Logo *GetLogo(void)
{
	return &g_Logo[0];
}