//=============================================================================
//
// 大量発生のUI処理 [manyMORS.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input.h"
#include "light.h"
#include "manyMORS.h"
#include "2D_effect.h"
#include "particle.h"
#include "camera.h"
#include "eventstamp.h"
#include "sound.h"

#define NUM_PLACE	(4)
#define MOVE_TIME	(20)
#define TEXT_X	(100.0f)
#define TEXT_Y	(100.0f)

//グローバル変数宣言
const char *c_apFilenameManyMORS[] =	//ファイル読み込み
{
	"data\\TEXTURE\\security_01.png",	//警
	"data\\TEXTURE\\security_02.png",	//備
	"data\\TEXTURE\\security_03.png",	//強
	"data\\TEXTURE\\security_04.png",	//化
};

LPDIRECT3DTEXTURE9 g_pTextureManyMORS[(sizeof c_apFilenameManyMORS) / sizeof(*c_apFilenameManyMORS)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManyMORS = NULL;		//頂点バッファへのポインタ
MANY_MORS g_aManyMORS[NUM_PLACE];					//文章
int g_nCntAfterManyMORS;

//=============================================================================
//大量発生のUI初期化処理
//=============================================================================					   
void InitManyMORS(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameManyMORS) / sizeof(*c_apFilenameManyMORS); CntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameManyMORS[CntTex],
			&g_pTextureManyMORS[CntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffManyMORS,
		NULL);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		g_aManyMORS[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);	//位置
		g_aManyMORS[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色
		g_aManyMORS[nCntTxt].nType = 0;			//種類
		g_aManyMORS[nCntTxt].fWidth = 0.0f;		//幅
		g_aManyMORS[nCntTxt].fHeight = 0.0f;		//高さ
		g_aManyMORS[nCntTxt].nCntZoom = 0;		//大きくなる時間
		g_aManyMORS[nCntTxt].bUse = false;			//使用しているか
	}
	g_nCntAfterManyMORS = 0;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffManyMORS->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - TEXT_X, g_aManyMORS[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + TEXT_X, g_aManyMORS[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - TEXT_X, g_aManyMORS[nCntTxt].pos.y + TEXT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + TEXT_X, g_aManyMORS[nCntTxt].pos.y + TEXT_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffManyMORS->Unlock();
}

//=============================================================================
//大量発生のUI終了処理
//=============================================================================
void UninitManyMORS(void)
{

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameManyMORS) / sizeof(*c_apFilenameManyMORS); CntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureManyMORS[CntTex] != NULL)
		{
			g_pTextureManyMORS[CntTex]->Release();
			g_pTextureManyMORS[CntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffManyMORS != NULL)
	{
		g_pVtxBuffManyMORS->Release();
		g_pVtxBuffManyMORS = NULL;
	}
}

//=============================================================================
//大量発生のUI更新処理
//=============================================================================
void UpdateManyMORS(void)
{

	//イベントスタンプの情報取得
	EVENTSTAMP *pEventStamp = GetEventStamp();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffManyMORS->Lock(0, 0, (void**)&pVtx, 0);

	int nCntUse = 0;
	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyMORS[nCntTxt].bUse == true)
		{//使用されていたら

			//使用カウントを加算
			nCntUse++;

			//拡大の時間加算
			g_aManyMORS[nCntTxt].nCntZoom++;

			if (g_aManyMORS[nCntTxt].nCntZoom <= MOVE_TIME)
			{
				g_aManyMORS[nCntTxt].fWidth += (TEXT_X - g_aManyMORS[nCntTxt].fWidth) * 0.25f;
				g_aManyMORS[nCntTxt].fHeight += (TEXT_Y - g_aManyMORS[nCntTxt].fHeight) * 0.25f;
				g_aManyMORS[nCntTxt].pos.y += (200.0f - g_aManyMORS[nCntTxt].pos.y) * 0.25f;
			}

			if (g_aManyMORS[nCntTxt].nCntZoom == 10)
			{//着きそうなときにパーティクル

				SetParticle(g_aManyMORS[nCntTxt].pos, PARTICLE_TYPE_MANYTXT_MORS);

				if (nCntTxt == 0)
				{
					PlaySound(SOUND_LABEL_SE_KYUWAWA);
				}
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y - g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y - g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y + g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y + g_aManyMORS[nCntTxt].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	if (nCntUse >= NUM_PLACE - 1)
	{//全部使用されていたら
		
		//余韻の時間加算
		g_nCntAfterManyMORS++;

		for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
		{
			if (g_nCntAfterManyMORS >= 150)
			{//余韻で消す

				g_aManyMORS[nCntTxt].fWidth += (10.0f - g_aManyMORS[nCntTxt].fWidth) * 0.25f;
				g_aManyMORS[nCntTxt].fHeight += (10.0f - g_aManyMORS[nCntTxt].fHeight) * 0.25f;
				//g_aManyMORS[nCntTxt].pos.y += (100.0f - g_aManyMORS[nCntTxt].pos.y) * 0.25f;
				g_aManyMORS[nCntTxt].pos.x += (1100.0f - g_aManyMORS[nCntTxt].pos.x) * 0.25f;

				if (g_aManyMORS[nCntTxt].fWidth < 12.0f)
				{
					g_aManyMORS[nCntTxt].bUse = false;

					if (pEventStamp[EVENTTYPE_MORS].bUse == false)
					{
						SetEventStamp(EVENTTYPE_MORS);
					}
				}
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffManyMORS->Unlock();

}
//=============================================================================
//大量発生のUI描画処理
//=============================================================================
void DrawManyMORS(void)
{

	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffManyMORS, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyMORS[nCntTxt].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureManyMORS[nCntTxt]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTxt * 4, 2);
		}
	}
}

//=============================================================================
//大量発生のUI設定処理
//=============================================================================
void SetManyMORS(int nType)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffManyMORS->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyMORS[nCntTxt].bUse == false)
		{//使用されていなかったら

			g_aManyMORS[nCntTxt].bUse = true;
			g_aManyMORS[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);
			g_aManyMORS[nCntTxt].fWidth = 1500.0f;
			g_aManyMORS[nCntTxt].fHeight = 1500.0f;
			g_aManyMORS[nCntTxt].nCntZoom = 0;
			g_nCntAfterManyMORS = 0;
			g_aManyMORS[nCntTxt].nType = nType;	//イベントの種類

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y - g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y - g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y + g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y + g_aManyMORS[nCntTxt].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffManyMORS->Unlock();
}

//=============================================================================
//大量発生のUI情報取得
//=============================================================================
MANY_MORS *GetManyMORS(void)
{
	return &g_aManyMORS[0];
}