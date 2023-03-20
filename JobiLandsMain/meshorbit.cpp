//=============================================================================
//
// 軌跡処理 [meshorbit.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "meshorbit.h"
#include "input.h"
#include "player.h"

//マクロ定義
#define WIDTH	(10)
#define HEIGHT	(1)
#define ORBIT_COUNT	(1)

//グローバル変数宣言
const char *c_apFilenameMeshOrbit[] =	//ファイル読み込み
{
	"data\\TEXTURE\\gradation002.jpg",
};
LPDIRECT3DTEXTURE9 g_pTextureMeshOrbit[(sizeof c_apFilenameMeshOrbit) / sizeof(*c_apFilenameMeshOrbit)] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshOrbit = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshOrbit = NULL;	//インデックスバッファへのポインタ
ORBIT g_aMeshOrbit[MAX_ORBIT];						//軌跡の情報
int g_nNumIndexaMeshOrbit;			//インデックス数
int g_nNumVertexaMeshOrbit;			//頂点数
int g_nCntOrbit = 0;

//==================================================================================
//軌跡の初期化処理
//==================================================================================
void InitMeshOrbit(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMeshOrbit) / sizeof(*c_apFilenameMeshOrbit); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMeshOrbit[nCntTex],
			&g_pTextureMeshOrbit[nCntTex]);
	}

	//各要素初期化
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aMeshOrbit[nCntOrbit].pMtxparent = NULL;				//親のマトリックスのポインタ

		for (int nCntOffset = 0; nCntOffset < OFFSET_POINT; nCntOffset++)
		{
			g_aMeshOrbit[nCntOrbit].aOffSet[nCntOffset] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//両端のオフセット
			g_aMeshOrbit[nCntOrbit].aCol[nCntOffset] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//両端の基準の色

			//両端のワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[nCntOffset]);
		}

		for (int nCntPoint = 0; nCntPoint < ALL_POINT; nCntPoint++)
		{
			g_aMeshOrbit[nCntOrbit].aPosPoint[nCntPoint] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//計算後の各頂点座標
			g_aMeshOrbit[nCntOrbit].aColPoint[nCntPoint] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//各頂点の色
		}
	}
	g_nNumIndexaMeshOrbit = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1)) * MAX_ORBIT;
	g_nNumVertexaMeshOrbit = ((WIDTH + 1) * (HEIGHT + 1)) * MAX_ORBIT;
	g_nCntOrbit = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexaMeshOrbit,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshOrbit,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 textextex[100];

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		//頂点情報の設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//頂点座標の設定
				pVtx[0].pos = g_aMeshOrbit[nCntOrbit].aPosPoint[(nCntHeight * (WIDTH + 1)) + nCntWidth];

				//法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				//頂点カラーの設定
				if (((nCntHeight * (WIDTH + 1)) + nCntWidth) < WIDTH + 1)
				{
					pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.3f);
				}
				else
				{

					pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
				}

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2
				(
					((float)nCntWidth / (1.0f / (float)(WIDTH + 1))) * (1.0f / (float)(WIDTH + 1)),
					((float)nCntHeight / (1.0f / (float)(HEIGHT + 1))) * (1.0f / (float)(HEIGHT + 1))
				);

				textextex[(nCntHeight * (WIDTH + 1)) + nCntWidth] = D3DXVECTOR2
				(
					((float)nCntWidth / (1.0f / (float)(WIDTH + 1))) * (1.0f / (float)(WIDTH + 1)),
					((float)nCntHeight / (1.0f / (float)(HEIGHT + 1))) * (1.0f / (float)(HEIGHT + 1))
				);

				pVtx += 1;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshOrbit->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexaMeshOrbit,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshOrbit,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshOrbit->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;
	int nVtxPoint = 0;

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{

		//頂点番号データの設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//高さの分割数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				pIdx[nCntIdx + 0] = (nCntWidth + (WIDTH + 1) * (nCntHeight + 1)) + nVtxPoint;
				pIdx[nCntIdx + 1] = (nCntWidth + ((WIDTH + 1) * nCntHeight)) + nVtxPoint;
				nCntIdx += 2;	//2つずつ見てるから
			}

			if (nCntHeight + 1 < HEIGHT)
			{//最後のちょんは打たない

				//空打ち2つ分
				pIdx[nCntIdx + 0] = (((WIDTH + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
				pIdx[nCntIdx + 1] = ((WIDTH + 1) * (nCntHeight + 2)) + nVtxPoint;
				nCntIdx += 2;	//2つずつ見てるから
			}
		}

		nVtxPoint += (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//今回のインデックス数を加算

	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshOrbit->Unlock();

}

//==================================================================================
//軌跡の終了処理
//==================================================================================
void UninitMeshOrbit(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMeshOrbit) / sizeof(*c_apFilenameMeshOrbit); nCntTex++)
	{
		if (g_pTextureMeshOrbit[nCntTex] != NULL)
		{
			g_pTextureMeshOrbit[nCntTex]->Release();
			g_pTextureMeshOrbit[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshOrbit != NULL)
	{
		g_pVtxBuffMeshOrbit->Release();
		g_pVtxBuffMeshOrbit = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffMeshOrbit != NULL)
	{
		g_pIdxBuffMeshOrbit->Release();
		g_pIdxBuffMeshOrbit = NULL;
	}

}

//==================================================================================
//軌跡の更新処理
//==================================================================================
void UpdateMeshOrbit(void)
{
	
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	D3DXVECTOR3 pos[ALL_POINT];

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		int nCntVtx = 0;

		//頂点情報の設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0, nCntVtx = 0; nCntWidth < WIDTH + 1; nCntWidth++, nCntVtx++)
			{//横の頂点数分繰り返す

				//頂点座標の設定
				pVtx[nCntVtx              ].pos = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth];
				pVtx[nCntVtx + (WIDTH + 1)].pos = g_aMeshOrbit[nCntOrbit].aPosPoint[(WIDTH + 1) + nCntWidth];

				pos[nCntVtx              ] = g_aMeshOrbit[nCntOrbit].aPosPoint[(WIDTH + 1) + nCntWidth];
				pos[nCntVtx + (WIDTH + 1)] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth];
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshOrbit->Unlock();
}

//==================================================================================
//軌跡の描画処理
//==================================================================================
void DrawMeshOrbit(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス宣言
	int nIdxPoint = 0;

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 背面のカリングはしません。
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (GetKeyboardPress(DIK_F3) == true)
	{//F9が押された
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//ワーイヤーフレーム
	}

	//加算合成の設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//減算合成の設定
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{

		//========================================================
		//オフセット0のマトリックス作成
		//========================================================
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]);
		D3DXMatrixIdentity(g_aMeshOrbit[nCntOrbit].pMtxparent);
		D3DXMatrixIdentity(&mtxTrans);

		//オフセット分を反映する
		D3DXMatrixTranslation(&mtxTrans, g_aMeshOrbit[nCntOrbit].aOffSet[0].x, g_aMeshOrbit[nCntOrbit].aOffSet[0].y, g_aMeshOrbit[nCntOrbit].aOffSet[0].z);
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0], &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0], &mtxTrans);

		*g_aMeshOrbit[nCntOrbit].pMtxparent = pPlayer->aModel[1].mtxWorld;
		//親のマトリックスをかける
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0], &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0], g_aMeshOrbit[nCntOrbit].pMtxparent);

		//========================================================
		//オフセット0のマトリックス作成
		//========================================================
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1]);
		D3DXMatrixIdentity(&mtxTrans);

		//オフセット分を反映する
		D3DXMatrixTranslation(&mtxTrans, g_aMeshOrbit[nCntOrbit].aOffSet[1].x, g_aMeshOrbit[nCntOrbit].aOffSet[1].y + 100.0f, g_aMeshOrbit[nCntOrbit].aOffSet[1].z);
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1], &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1], &mtxTrans);

		//親のマトリックスをかける
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1], &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1], g_aMeshOrbit[nCntOrbit].pMtxparent);

		if (g_nCntOrbit == ORBIT_COUNT)
		{
			//保存してある頂点座標と頂点カラーをずらす(上書き保存)
			/*for (int nCntWidth = ALL_POINT - 1; nCntWidth >= 0; nCntWidth--)
			{
				g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth + 2] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth + 0];
				g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth + 3] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth + 1];
			}*/

			for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					//g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) + nCntWidth] = g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 2) + nCntWidth];
					//g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1 + (WIDTH + 1)) + nCntWidth] = g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit + (WIDTH + 1) - 2) + nCntWidth];

					/*g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - nCntWidth] = 
						g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - nCntWidth - 1];

					g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - (WIDTH + 1) - nCntWidth] = 
						g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - (WIDTH + 1) - nCntWidth - 1];*/

					g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - nCntWidth] =
						g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - nCntWidth - 1];

					g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - (WIDTH + 1) - nCntWidth] =
						g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - (WIDTH + 1) - nCntWidth - 1];

					/*g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 0] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 2];
					g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 1] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 3];*/
				}
			}

			/*for (int nCntWidth = g_nNumVertexaMeshOrbit - 1; nCntWidth >= 0;)
			{
				g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 0] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 2];
				g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 1] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 3];
				nCntWidth -= 2;
			}*/

			//最新の頂点座標と頂点カラーを代入する
			g_aMeshOrbit[nCntOrbit].aPosPoint[WIDTH + 1] = D3DXVECTOR3(
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]._41,
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]._42,
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]._43);

			g_aMeshOrbit[nCntOrbit].aPosPoint[0] = D3DXVECTOR3(
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1]._41,
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1]._42,
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1]._43);
		}

		//ワールドマトリックスの設定
		//pDevice->SetTransform(D3DTS_WORLD, &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffMeshOrbit, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffMeshOrbit);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureMeshOrbit[0]);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			((WIDTH + 1) * (HEIGHT + 1)),
			nIdxPoint,
			(HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1)) - 2);

		nIdxPoint += (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//今回のインデックス数を加算

	}

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 三角形の頂点が反時計回りならば裏向きと見なし描画しません。（デフォルト）
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//埋めるモード

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	g_nCntOrbit++;
	if (g_nCntOrbit > ORBIT_COUNT)
	{
		g_nCntOrbit = 0;
	}
}

//==================================================================================
//軌跡の情報取得
//==================================================================================
ORBIT *GetMeshOrbit(void)
{
	return &g_aMeshOrbit[0];
}

//==================================================================================
//軌跡の頂点情報取得
//==================================================================================
LPDIRECT3DVERTEXBUFFER9 GetBuffMeshOrbit(void)
{
	return g_pVtxBuffMeshOrbit;
}