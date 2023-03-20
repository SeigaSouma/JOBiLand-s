//=============================================================================
//
// ポリゴン処理 [minimap.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "minimap.h"
#include "mapshape.h"

//マクロ定義
#define POS_POLYGON	(1000.0f)
#define MAX_TOP		(4)
#define MAX_BILDING	(256)

//グローバル変数宣言
const char *c_apFilenameMiniMap[] =					//ファイル読み込み
{
	"data\\TEXTURE\\window1.png",
};
LPDIRECT3DTEXTURE9 g_apTextureMiniMap[(sizeof c_apFilenameMiniMap) / sizeof(*c_apFilenameMiniMap)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniMap = NULL;			//頂点バッファへのポインタ
MINIMAP g_aMinimap[MAX_BILDING];			//ミニマップの情報

//==================================================================================
//ミニマップの初期化処理
//==================================================================================
void InitMinimap(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMiniMap) / sizeof(*c_apFilenameMiniMap); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMiniMap[nCntTex],
			&g_apTextureMiniMap[nCntTex]);
	}

	//各要素初期化
	for (int nCnt = 0; nCnt < MAX_BILDING; nCnt++)
	{
		g_aMinimap[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aMinimap[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aMinimap[nCnt].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//最大サイズ
		g_aMinimap[nCnt].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//最小サイズ

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMinimap[nCnt].mtxWorld);
		g_aMinimap[nCnt].bUse = false;				//使用されているかどうか
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_BILDING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiniMap,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILDING; nCnt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMin.x, 0.0f, g_aMinimap[nCnt].vtxMax.z);
		pVtx[1].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMax.x, 0.0f, g_aMinimap[nCnt].vtxMax.z);
		pVtx[2].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMin.x, 0.0f, g_aMinimap[nCnt].vtxMin.z);
		pVtx[3].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMax.x, 0.0f, g_aMinimap[nCnt].vtxMin.z);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		/*pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =*/
			pVtx[3].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);

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
	g_pVtxBuffMiniMap->Unlock();

	//		SetMiniMap(D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f));
			/*SetMiniMap(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f), -D3DXVECTOR3(200.0f, 0.0f, 200.0f));*/

}

//==================================================================================
//ミニマップの終了処理
//==================================================================================
void UninitMinimap(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMiniMap) / sizeof(*c_apFilenameMiniMap); nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureMiniMap[nCntTex] != NULL)
		{
			g_apTextureMiniMap[nCntTex]->Release();
			g_apTextureMiniMap[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMiniMap != NULL)
	{
		g_pVtxBuffMiniMap->Release();
		g_pVtxBuffMiniMap = NULL;
	}
}

//==================================================================================
//ミニマップの更新処理
//==================================================================================
void UpdateMinimap(void)
{

}

//==================================================================================
//ミニマップの描画処理
//==================================================================================
void DrawMinimap(void)
{

	//マップの形描画処理
	//DrawMapShape();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	//ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

#if 1
	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
#else
	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
#endif

	for (int nCnt = 0; nCnt < MAX_BILDING; nCnt++)
	{
		if (g_aMinimap[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMinimap[nCnt].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMinimap[nCnt].rot.y, g_aMinimap[nCnt].rot.x, g_aMinimap[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aMinimap[nCnt].mtxWorld, &g_aMinimap[nCnt].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aMinimap[nCnt].pos.x, g_aMinimap[nCnt].pos.y, g_aMinimap[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aMinimap[nCnt].mtxWorld, &g_aMinimap[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMinimap[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMiniMap, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMiniMap[0]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==================================================================================
//ミニマップの配置処理
//==================================================================================
void SetMiniMap(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILDING; nCnt++)
	{
		if (g_aMinimap[nCnt].bUse == false)
		{//使用されていない状態にする

			g_aMinimap[nCnt].pos = pos;
			g_aMinimap[nCnt].vtxMax = vtxMax;
			g_aMinimap[nCnt].vtxMin = vtxMin;
			g_aMinimap[nCnt].bUse = true;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMin.x, 0.0f, g_aMinimap[nCnt].vtxMax.z);
			pVtx[1].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMax.x, 0.0f, g_aMinimap[nCnt].vtxMax.z);
			pVtx[2].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMin.x, 0.0f, g_aMinimap[nCnt].vtxMin.z);
			pVtx[3].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMax.x, 0.0f, g_aMinimap[nCnt].vtxMin.z);

		/*	pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);*/

			break;
		}
		
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMiniMap->Unlock();
}
