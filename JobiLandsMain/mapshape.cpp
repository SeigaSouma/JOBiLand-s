//=============================================================================
//
// マップの形処理 [mapshape.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "mapshape.h"
#include "input.h"
#include "debugproc.h"
#include "crowd.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "buddy.h"
#include "debugproc.h"
#include "sound.h"
#include "cutin.h"

//マクロ定義
#define POS_X		(250.0f * 0.5f)	//ゲージの長さ
#define POS_Y		(250.0f * 0.5f)	//ゲージの長さ
#define DOWN_MOVE	(0.05f)	//減る速度
#define UP_POS		(4.0f)	//増える大きさ

//プロタイプ宣言
void UpdateGameMapShape(void);

//グローバル変数
const char *c_apFilenameMapShape[] =		//ファイル読み込み
{
	"data\\TEXTURE\\mapimage_01.png",
	NULL,
};
LPDIRECT3DTEXTURE9 g_apTextureMapShape[(sizeof c_apFilenameMapShape) / sizeof(*c_apFilenameMapShape)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMapShape = NULL;
MapShape g_aMapShape;	//マップの形の情報

//==============================================================
//マップの形の初期化処理
//==============================================================
void InitMapShape(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMapShape) / sizeof(*c_apFilenameMapShape); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMapShape[nCntTex],
			&g_apTextureMapShape[nCntTex]);
	}

	//各種変数初期化
	for (int nCntMapShape = 0; nCntMapShape < MAPSHAPE_VTX_MAX; nCntMapShape++)
	{
		g_aMapShape.aUI[nCntMapShape].pos = D3DXVECTOR3(POS_X, SCREEN_HEIGHT - POS_Y, 0.0f);	//頂点座標
		g_aMapShape.aUI[nCntMapShape].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//頂点カラー
		g_aMapShape.aUI[nCntMapShape].fWidth = POS_X;		//幅
		g_aMapShape.aUI[nCntMapShape].fHeight = POS_Y;		//高さ
		g_aMapShape.aUI[nCntMapShape].nPatternAnim = 0;	//アニメーションパターンNo
		g_aMapShape.aUI[nCntMapShape].nCntAnim = 0;		//アニメーションカウンター
		g_aMapShape.aUI[nCntMapShape].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_aMapShape.aUI[nCntMapShape].fAngle = 0.0f;		//角度
		g_aMapShape.aUI[nCntMapShape].fLength = 0.0f;		//対角線の長さ
		g_aMapShape.aUI[nCntMapShape].nCount = 0;			//雑カウント
		g_aMapShape.aUI[nCntMapShape].bUse = false;			//使用しているか
		g_aMapShape.aUI[nCntMapShape].aTexU = 0.0f;		//テクスチャ座標U[スクロール用]
		g_aMapShape.aUI[nCntMapShape].aTexV = 0.0f;		//テクスチャ座標V[スクロール用]
	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAPSHAPE_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMapShape,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMapShape->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntMapShape = 0; nCntMapShape < MAPSHAPE_VTX_MAX; nCntMapShape++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aMapShape.aUI[nCntMapShape].pos.x - POS_X, g_aMapShape.aUI[nCntMapShape].pos.y - POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aMapShape.aUI[nCntMapShape].pos.x + POS_X, g_aMapShape.aUI[nCntMapShape].pos.y - POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aMapShape.aUI[nCntMapShape].pos.x - POS_X, g_aMapShape.aUI[nCntMapShape].pos.y + POS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aMapShape.aUI[nCntMapShape].pos.x + POS_X, g_aMapShape.aUI[nCntMapShape].pos.y + POS_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = g_aMapShape.aUI[nCntMapShape].col;
		pVtx[1].col = g_aMapShape.aUI[nCntMapShape].col;
		pVtx[2].col = g_aMapShape.aUI[nCntMapShape].col;
		pVtx[3].col = g_aMapShape.aUI[nCntMapShape].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMapShape->Unlock();
}

//==============================================================
//マップの形の画面の終了処理
//==============================================================
void UninitMapShape(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMapShape) / sizeof(*c_apFilenameMapShape); nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureMapShape[nCntTex] != NULL)
		{
			g_apTextureMapShape[nCntTex]->Release();
			g_apTextureMapShape[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMapShape != NULL)
	{
		g_pVtxBuffMapShape->Release();
		g_pVtxBuffMapShape = NULL;
	}

}

//==============================================================
//マップの形の画面の更新処理
//==============================================================
void UpdateMapShape(void)
{
	switch (GetMode())
	{
	case MODE_GAME:
		UpdateGameMapShape();
		break;

	case MODE_TUTORIAL:
		//UpdateTutorialMapShape();
		break;
	}
}

//==============================================================
//マップの形のゲーム時更新処理
//==============================================================
void UpdateGameMapShape(void)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//群衆の情報取得
	Crowd *pCrowd = GetCrowd();

	//群衆の数の情報取得
	CrowdNum *pCrowdNum = GetCrowdNum();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffMapShape->Lock(0, 0, (void**)&pVtx, 0);


	//頂点バッファをアンロックする
	g_pVtxBuffMapShape->Unlock();
}

//==============================================================
//マップの形の画面の描画処理
//==============================================================
void DrawMapShape(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffMapShape, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntMapShape = 0; nCntMapShape < MAPSHAPE_VTX_MAX; nCntMapShape++)
	{
		switch (nCntMapShape)
		{
		case MAPSHAPE_VTX_FRAM:

			//アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 250);

			//αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		//case MAPSHAPE_VTX_TEX:

		//	//Zテストを無効にする
		//	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
		//	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
		//	break;
		}

		//テクスチャセット
		pDevice->SetTexture(0, g_apTextureMapShape[nCntMapShape]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntMapShape * 4, 2);	//プリミティブの種類

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		//Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//==============================================================
//マップの形の情報
//==============================================================
MapShape *GetMapShape(void)
{
	return &g_aMapShape;
}