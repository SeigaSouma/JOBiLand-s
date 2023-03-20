//=============================================================================
//
// 終了UI [finish.cpp]
// Author : 大原怜将
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
#include "fenish.h"

#define NUM_PLACE	(4)
#define MOVE_TIME	(20)
#define TEXT_X	(250.0f)
#define TEXT_Y	(125.0f)

////グローバル変数宣言
//const char *c_apFilenameFinish[] =	//ファイル読み込み
//{
//	"data\\TEXTURE\\tairyo_01.png",	//大
//};

LPDIRECT3DTEXTURE9 g_pTextureFinish = NULL;	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFinish = NULL;		//頂点バッファへのポインタ
FENISH g_aFinish;					//文章
int g_nCntAfterFinish;

//=============================================================================
//終了UIの初期化処理
//=============================================================================					   
void InitFinish(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\endcount_finish.png",
		&g_pTextureFinish);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFinish,
		NULL);

	
	g_aFinish.pos = D3DXVECTOR3(1280.0f, 340.0f, 0.0f);	//位置
	g_aFinish.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色
	g_aFinish.nType = 0;			//種類
	g_aFinish.fWidth = 0.0f;		//幅
	g_aFinish.fHeight = 0.0f;		//高さ
	g_aFinish.nCntZoom = 0;		//大きくなる時間
	g_aFinish.bUse = false;		//使用しているか
	
	g_nCntAfterFinish = 0;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFinish->Lock(0, 0, (void**)&pVtx, 0);

	
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aFinish.pos.x - TEXT_X, g_aFinish.pos.y - TEXT_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aFinish.pos.x + TEXT_X, g_aFinish.pos.y - TEXT_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aFinish.pos.x - TEXT_X, g_aFinish.pos.y + TEXT_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aFinish.pos.x + TEXT_X, g_aFinish.pos.y + TEXT_Y, 0.0f);

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
	

	//頂点バッファをアンロックする
	g_pVtxBuffFinish->Unlock();
}

//=============================================================================
//終了UIの終了処理
//=============================================================================
void UninitFinish(void)
{

	//サウンド終了
	StopSound();
	
	//テクスチャの破棄
	if (g_pTextureFinish != NULL)
	{
		g_pTextureFinish->Release();
		g_pTextureFinish = NULL;
	}
	

	//頂点バッファの破棄
	if (g_pVtxBuffFinish != NULL)
	{
		g_pVtxBuffFinish->Release();
		g_pVtxBuffFinish = NULL;
	}
}

//=============================================================================
//終了UIの更新処理
//=============================================================================
void UpdateFinish(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	VERTEX_2D *pVtx;

	//イベントスタンプの情報取得
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFinish->Lock(0, 0, (void**)&pVtx, 0);

	int nCntUse = 0;

	if (GetKeyboardTrigger(DIK_5) == true)
	{//↓キーが押された

		SetFinish(0);
	}

	if (g_aFinish.bUse == true)
	{//使用されていたら

		//使用カウントを加算
		nCntUse++;

		//拡大の時間加算
		g_aFinish.nCntZoom++;

		if (g_aFinish.nCntZoom <= MOVE_TIME)
		{
			g_aFinish.fWidth += (TEXT_X - g_aFinish.fWidth) * 0.25f;
			g_aFinish.fHeight += (TEXT_Y - g_aFinish.fHeight) * 0.25f;
			g_aFinish.pos.y += (360.0f - g_aFinish.pos.y) * 0.25f;
		}
		
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aFinish.pos.x - g_aFinish.fWidth, g_aFinish.pos.y - g_aFinish.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFinish.pos.x + g_aFinish.fWidth, g_aFinish.pos.y - g_aFinish.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFinish.pos.x - g_aFinish.fWidth, g_aFinish.pos.y + g_aFinish.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFinish.pos.x + g_aFinish.fWidth, g_aFinish.pos.y + g_aFinish.fHeight, 0.0f);

		pVtx += 4;
	}

	
	

	//if (nCntUse >= NUM_PLACE - 1)
	//{//全部使用されていたら

	//	if (g_nCntAfterFinish == 60)
	//	{
	//		//激アツキター！
	//		PlaySound(SOUND_LABEL_SE_FINISH);
	//	}

	//	//余韻の時間加算
	//	g_nCntAfterFinish++;

	//	
	//	if (g_nCntAfterFinish >= 150)
	//	{//余韻で消す

	//		g_aFinish.fWidth += (10.0f - g_aFinish.fWidth) * 0.25f;
	//		g_aFinish.fHeight += (10.0f - g_aFinish.fHeight) * 0.25f;
	//		//g_aFinish.pos.y += (100.0f - g_aFinish.pos.y) * 0.25f;
	//		g_aFinish.pos.x += (1100.0f - g_aFinish.pos.x) * 0.25f;

	//		if (g_aFinish.fWidth < 12.0f)
	//		{
	//			//目標の地点設定
	//			pCamera->nMoveStep = CAMERAMOVE_UP;
	//			g_aFinish.bUse = false;

	//			if (pEventStamp[EVENTTYPE_MANYMOB].bUse == false)
	//			{
	//				SetEventStamp(EVENTTYPE_MANYMOB);
	//			}
	//		}
	//	}
		
	//}

	//頂点バッファをアンロックする
	g_pVtxBuffFinish->Unlock();

}
//=============================================================================
//終了UIの描画処理
//=============================================================================
void DrawFinish(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFinish, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	if (g_aFinish.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureFinish);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	
}

//=============================================================================
//終了UIの設定処理
//=============================================================================
void SetFinish(int nType)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFinish->Lock(0, 0, (void**)&pVtx, 0);

	
	if (g_aFinish.bUse == false)
	{//使用されていなかったら

		g_aFinish.bUse = true;
		g_aFinish.pos = D3DXVECTOR3(1280.0f * 0.5f, 340.0f, 0.0f);	//位置
		g_aFinish.fWidth = 3000.0f;
		g_aFinish.fHeight = 1500.0f;
		g_aFinish.nCntZoom = 0;
		g_nCntAfterFinish = 0;
		g_aFinish.nType = nType;	//イベントの種類

		//激アツキター！
		PlaySound(SOUND_LABEL_SE_FINISH);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aFinish.pos.x - g_aFinish.fWidth, g_aFinish.pos.y - g_aFinish.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFinish.pos.x + g_aFinish.fWidth, g_aFinish.pos.y - g_aFinish.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFinish.pos.x - g_aFinish.fWidth, g_aFinish.pos.y + g_aFinish.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFinish.pos.x + g_aFinish.fWidth, g_aFinish.pos.y + g_aFinish.fHeight, 0.0f);

		pVtx += 4;
	}

	
	

	//頂点バッファをアンロックする
	g_pVtxBuffFinish->Unlock();
}

//=============================================================================
//終了UIの情報取得
//=============================================================================
FENISH *GetFinish(void)
{
	return &g_aFinish;
}