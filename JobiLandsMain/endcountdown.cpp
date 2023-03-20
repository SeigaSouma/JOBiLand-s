//=============================================================================
//
// 大量発生のUI処理 [manyUI.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input.h"
#include "light.h"
#include "endcountdown.h"
#include "2D_effect.h"
#include "particle.h"
#include "camera.h"
#include "sound.h"
#include "eventstamp.h"

#define NUM_PLACE	(5)
#define MOVE_TIME	(60)
#define TEXT_X	(125.0f)
#define TEXT_Y	(125.0f)

//グローバル変数宣言
const char *c_apFilenameEndCountDown[] =	//ファイル読み込み
{
	"data\\TEXTURE\\endcount_5.png",	//5
	"data\\TEXTURE\\endcount_4.png",	//4
	"data\\TEXTURE\\endcount_3.png",	//3
	"data\\TEXTURE\\endcount_2.png",	//2
	"data\\TEXTURE\\endcount_1.png",	//1
};

LPDIRECT3DTEXTURE9 g_pTextureEndCountDown[(sizeof c_apFilenameEndCountDown) / sizeof(*c_apFilenameEndCountDown)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEndCountDown = NULL;		//頂点バッファへのポインタ
ENDCOUNTDOWN g_aEndCountDown[NUM_PLACE];					//文章
int g_nCntAfterEndCountDown;

//=============================================================================
//大量発生のUI初期化処理
//=============================================================================					   
void InitEndCountDown(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameEndCountDown) / sizeof(*c_apFilenameEndCountDown); CntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEndCountDown[CntTex],
			&g_pTextureEndCountDown[CntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEndCountDown,
		NULL);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//g_aEndCountDown[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);	//位置
		g_aEndCountDown[nCntTxt].pos = D3DXVECTOR3(1280.0f * 0.5f, 500.0f, 0.0f);	//位置
		g_aEndCountDown[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色
		g_aEndCountDown[nCntTxt].nType = 0;			//種類
		g_aEndCountDown[nCntTxt].fWidth = 0.0f;		//幅
		g_aEndCountDown[nCntTxt].fHeight = 0.0f;		//高さ
		g_aEndCountDown[nCntTxt].nCntZoom = 0;		//大きくなる時間
		g_aEndCountDown[nCntTxt].bUse = false;			//使用しているか
	}
	g_nCntAfterEndCountDown = 0;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEndCountDown->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - TEXT_X, g_aEndCountDown[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + TEXT_X, g_aEndCountDown[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - TEXT_X, g_aEndCountDown[nCntTxt].pos.y + TEXT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + TEXT_X, g_aEndCountDown[nCntTxt].pos.y + TEXT_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEndCountDown->Unlock();
}

//=============================================================================
//大量発生のUI終了処理
//=============================================================================
void UninitEndCountDown(void)
{

	//SEを終了
	StopSound();

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameEndCountDown) / sizeof(*c_apFilenameEndCountDown); CntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureEndCountDown[CntTex] != NULL)
		{
			g_pTextureEndCountDown[CntTex]->Release();
			g_pTextureEndCountDown[CntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEndCountDown != NULL)
	{
		g_pVtxBuffEndCountDown->Release();
		g_pVtxBuffEndCountDown = NULL;
	}
}

//=============================================================================
//大量発生のUI更新処理
//=============================================================================
void UpdateEndCountDown(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	VERTEX_2D *pVtx;

	//イベントスタンプの情報取得
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEndCountDown->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_4) == true)
	{//↓キーが押された

		SetEndCountDown(0);
	}

	int nCntUse = 0;
	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aEndCountDown[nCntTxt].bUse == true)
		{//使用されていたら

			//使用カウントを加算
			nCntUse++;

			//拡大の時間加算
			g_aEndCountDown[nCntTxt].nCntZoom++;

			g_aEndCountDown[nCntTxt].col.a -= 0.02f;

			if (g_aEndCountDown[nCntTxt].nCntZoom <= MOVE_TIME)
			{
				g_aEndCountDown[nCntTxt].fHeight += TEXT_Y * 0.2f;
				g_aEndCountDown[nCntTxt].fWidth += TEXT_X * 0.2f;

				//g_aEndCountDown[nCntTxt].fWidth += (TEXT_X - g_aEndCountDown[nCntTxt].fWidth) * 0.25f;
				//g_aEndCountDown[nCntTxt].fHeight += (TEXT_Y - g_aEndCountDown[nCntTxt].fHeight) * 0.25f;
				//[nCntTxt].pos.y += (360.0f - g_aEndCountDown[nCntTxt].pos.y) * 0.25f;
			}
			else if (g_aEndCountDown[nCntTxt].nCntZoom == MOVE_TIME + 1)
			{//移動が終わったら

			    //次の文字の設定
				SetEndCountDown(0);
			}

			if (g_aEndCountDown[nCntTxt].nCntZoom == 3)
			{
				PlaySound(SOUND_LABEL_SE_ENDCOUNTDOWN);
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y - g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y - g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y + g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y + g_aEndCountDown[nCntTxt].fHeight, 0.0f);

			//頂点カラー
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndCountDown[nCntTxt].col.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndCountDown[nCntTxt].col.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndCountDown[nCntTxt].col.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndCountDown[nCntTxt].col.a);
		}

		pVtx += 4;
	}

	if (g_aEndCountDown[4].col.a <= 0.0f)
	{
		for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
		{
			g_aEndCountDown[nCntTxt].bUse = false;

			g_aEndCountDown[nCntTxt].col.a = 0.6f;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEndCountDown->Unlock();

}
//=============================================================================
//大量発生のUI描画処理
//=============================================================================
void DrawEndCountDown(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEndCountDown, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aEndCountDown[nCntTxt].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEndCountDown[nCntTxt]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTxt * 4, 2);
		}
	}
}

//=============================================================================
//大量発生のUI設定処理
//=============================================================================
void SetEndCountDown(int nType)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEndCountDown->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aEndCountDown[nCntTxt].bUse == false)
		{//使用されていなかったら

			g_aEndCountDown[nCntTxt].bUse = true;
			//g_aEndCountDown[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);	//位置
			g_aEndCountDown[nCntTxt].pos = D3DXVECTOR3(1280.0f * 0.5f, 350.0f, 0.0f);
			g_aEndCountDown[nCntTxt].fWidth = 10.0f;
			g_aEndCountDown[nCntTxt].fHeight = 10.0f;
			g_aEndCountDown[nCntTxt].nCntZoom = 0;
			g_nCntAfterEndCountDown = 0;
			g_aEndCountDown[nCntTxt].nType = nType;	//イベントの種類

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y - g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y - g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y + g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y + g_aEndCountDown[nCntTxt].fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEndCountDown->Unlock();
}

//=============================================================================
//大量発生のUI情報取得
//=============================================================================
ENDCOUNTDOWN *GetEndCountDown(void)
{
	return &g_aEndCountDown[0];
}