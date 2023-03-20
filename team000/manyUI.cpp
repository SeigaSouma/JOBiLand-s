//=============================================================================
//
// 大量発生のUI処理 [manyUI.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input.h"
#include "light.h"
#include "manyUI.h"
#include "2D_effect.h"
#include "particle.h"
#include "camera.h"
#include "sound.h"
#include "eventstamp.h"

#define NUM_PLACE	(4)
#define MOVE_TIME	(20)
#define TEXT_X	(125.0f)
#define TEXT_Y	(125.0f)

//グローバル変数宣言
const char *c_apFilenameManyUI[] =	//ファイル読み込み
{
	"data\\TEXTURE\\tairyo_01.png",	//大
	"data\\TEXTURE\\tairyo_02.png",	//量
	"data\\TEXTURE\\tairyo_03.png",	//発
	"data\\TEXTURE\\tairyo_04.png",	//生
};

LPDIRECT3DTEXTURE9 g_pTextureManyUI[(sizeof c_apFilenameManyUI) / sizeof(*c_apFilenameManyUI)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManyUI = NULL;		//頂点バッファへのポインタ
MANY_UI g_aManyUI[NUM_PLACE];					//文章
int g_nCntAfterManyUI;

//=============================================================================
//大量発生のUI初期化処理
//=============================================================================					   
void InitManyUI(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameManyUI) / sizeof(*c_apFilenameManyUI); CntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameManyUI[CntTex],
			&g_pTextureManyUI[CntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffManyUI,
		NULL);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		g_aManyUI[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);	//位置
		g_aManyUI[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色
		g_aManyUI[nCntTxt].nType = 0;			//種類
		g_aManyUI[nCntTxt].fWidth = 0.0f;		//幅
		g_aManyUI[nCntTxt].fHeight = 0.0f;		//高さ
		g_aManyUI[nCntTxt].nCntZoom = 0;		//大きくなる時間
		g_aManyUI[nCntTxt].bUse = false;			//使用しているか
	}
	g_nCntAfterManyUI = 0;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffManyUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - TEXT_X, g_aManyUI[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + TEXT_X, g_aManyUI[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - TEXT_X, g_aManyUI[nCntTxt].pos.y + TEXT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + TEXT_X, g_aManyUI[nCntTxt].pos.y + TEXT_Y, 0.0f);

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
	g_pVtxBuffManyUI->Unlock();
}

//=============================================================================
//大量発生のUI終了処理
//=============================================================================
void UninitManyUI(void)
{

	//SEを終了
	StopSound();

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameManyUI) / sizeof(*c_apFilenameManyUI); CntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureManyUI[CntTex] != NULL)
		{
			g_pTextureManyUI[CntTex]->Release();
			g_pTextureManyUI[CntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffManyUI != NULL)
	{
		g_pVtxBuffManyUI->Release();
		g_pVtxBuffManyUI = NULL;
	}
}

//=============================================================================
//大量発生のUI更新処理
//=============================================================================
void UpdateManyUI(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	VERTEX_2D *pVtx;

	//イベントスタンプの情報取得
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffManyUI->Lock(0, 0, (void**)&pVtx, 0);

	int nCntUse = 0;
	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyUI[nCntTxt].bUse == true)
		{//使用されていたら

			//使用カウントを加算
			nCntUse++;

			//拡大の時間加算
			g_aManyUI[nCntTxt].nCntZoom++;

			if (g_aManyUI[nCntTxt].nCntZoom <= MOVE_TIME)
			{
				g_aManyUI[nCntTxt].fWidth += (TEXT_X - g_aManyUI[nCntTxt].fWidth) * 0.25f;
				g_aManyUI[nCntTxt].fHeight += (TEXT_Y - g_aManyUI[nCntTxt].fHeight) * 0.25f;
				g_aManyUI[nCntTxt].pos.y += (360.0f - g_aManyUI[nCntTxt].pos.y) * 0.25f;
			}
			else if(g_aManyUI[nCntTxt].nCntZoom == MOVE_TIME + 1)
			{//移動が終わったら

				//次の文字の設定
				SetManyUI(0);
			}

			if (g_aManyUI[nCntTxt].nCntZoom == 10)
			{//着きそうなときにパーティクル

				SetParticle(g_aManyUI[nCntTxt].pos, PARTICLE_TYPE_MANYTXT);

				PlaySound(SOUND_LABEL_SE_EVENTTEXT);
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y - g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y - g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y + g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y + g_aManyUI[nCntTxt].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	if (nCntUse >= NUM_PLACE - 1)
	{//全部使用されていたら
		
		if (g_nCntAfterManyUI == 60)
		{
			//激アツキター！
			PlaySound(SOUND_LABEL_SE_GEKIATSU);
		}

		//余韻の時間加算
		g_nCntAfterManyUI++;

		for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
		{
			if (g_nCntAfterManyUI >= 150)
			{//余韻で消す

				g_aManyUI[nCntTxt].fWidth += (10.0f - g_aManyUI[nCntTxt].fWidth) * 0.25f;
				g_aManyUI[nCntTxt].fHeight += (10.0f - g_aManyUI[nCntTxt].fHeight) * 0.25f;
				//g_aManyUI[nCntTxt].pos.y += (100.0f - g_aManyUI[nCntTxt].pos.y) * 0.25f;
				g_aManyUI[nCntTxt].pos.x += (1100.0f - g_aManyUI[nCntTxt].pos.x) * 0.25f;

				if (g_aManyUI[nCntTxt].fWidth < 12.0f)
				{
					//目標の地点設定
					pCamera->nMoveStep = CAMERAMOVE_UP;
					g_aManyUI[nCntTxt].bUse = false;

					if (pEventStamp[EVENTTYPE_MANYMOB].bUse == false)
					{
						SetEventStamp(EVENTTYPE_MANYMOB);
					}
				}
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffManyUI->Unlock();

}
//=============================================================================
//大量発生のUI描画処理
//=============================================================================
void DrawManyUI(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffManyUI, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyUI[nCntTxt].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureManyUI[nCntTxt]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTxt * 4, 2);
		}
	}
}

//=============================================================================
//大量発生のUI設定処理
//=============================================================================
void SetManyUI(int nType)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffManyUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyUI[nCntTxt].bUse == false)
		{//使用されていなかったら

			g_aManyUI[nCntTxt].bUse = true;
			g_aManyUI[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);
			g_aManyUI[nCntTxt].fWidth = 1500.0f;
			g_aManyUI[nCntTxt].fHeight = 1500.0f;
			g_aManyUI[nCntTxt].nCntZoom = 0;
			g_nCntAfterManyUI = 0;
			g_aManyUI[nCntTxt].nType = nType;	//イベントの種類

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y - g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y - g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y + g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y + g_aManyUI[nCntTxt].fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffManyUI->Unlock();
}

//=============================================================================
//大量発生のUI情報取得
//=============================================================================
MANY_UI *GetManyUI(void)
{
	return &g_aManyUI[0];
}