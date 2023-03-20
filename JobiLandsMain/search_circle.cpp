//=============================================================================
//
// 探索範囲処理 [search_circle.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "search_circle.h"
#include "input.h"
#include "calculation.h"
#include "crowd.h"

//マクロ定義
#define POS_SEARCH_CIRCLE_Y	(0.0f)
#define WIDTH				(32)	//分割数
#define HEIGHT				(1)		//高さ分割

//プロトタイプ宣言
void CollisionSearchCircle(void);

//グローバル変数宣言
const char *c_apFilenameSearchCircle[] =	//ファイル読み込み
{
	"data\\TEXTURE\\gradation001.jpg",
	"data\\TEXTURE\\gradation000.jpg",
	"data\\TEXTURE\\gradation003.jpg",
	"data\\TEXTURE\\gradation004.jpg",
	"data\\TEXTURE\\gradation005.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureSearchCircle[(sizeof c_apFilenameSearchCircle) / sizeof(*c_apFilenameSearchCircle)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSearchCircle = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSearchCircle = NULL;	//インデックスバッファへのポインタ
SEARCH_CIRCLE g_aSearchCircle[MAX_SEARCH_CIRCLE];		//探索範囲の情報
int g_nNumVertxSearchCircle;
int g_nNumIndexSearchCircle;

//==================================================================================
//探索範囲の初期化処理
//==================================================================================
void InitSearchCircle(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSearchCircle) / sizeof(*c_apFilenameSearchCircle); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameSearchCircle[nCntTex],
			&g_apTextureSearchCircle[nCntTex]);
	}
	
	//各要素初期化
	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		g_aSearchCircle[nCntWave].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSearchCircle[nCntWave].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSearchCircle[nCntWave].nor[nCntWave] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//法線の向き
		g_aSearchCircle[nCntWave].col = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);
		g_aSearchCircle[nCntWave].colOrigin = g_aSearchCircle[nCntWave].col;

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aSearchCircle[nCntWave].mtxWorld);

		g_aSearchCircle[nCntWave].fOutWidth = 100.0f;
		g_aSearchCircle[nCntWave].fInWidth = 0.0f;
		g_aSearchCircle[nCntWave].fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);		//1つごとの角度を求める
		g_aSearchCircle[nCntWave].fHeight = 0.0f;
		g_aSearchCircle[nCntWave].fMove = 0.0f;
		g_aSearchCircle[nCntWave].nLife = 0;		//寿命
		g_aSearchCircle[nCntWave].nMaxLife = 0;		//最大寿命
		g_aSearchCircle[nCntWave].nTexType = 0;		//テクスチャ種類
		g_aSearchCircle[nCntWave].bUse = false;
	}
	g_nNumIndexSearchCircle = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//インデックス数
	g_nNumVertxSearchCircle = (HEIGHT + 1) * (WIDTH + 1);		//頂点数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_nNumVertxSearchCircle * MAX_SEARCH_CIRCLE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSearchCircle,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSearchCircle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[MAX_SEARCH_CIRCLE];		//計算用の座標

	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//上
				pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * g_aSearchCircle[nCntWave].fOutWidth,
					POS_SEARCH_CIRCLE_Y,
					cosf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * g_aSearchCircle[nCntWave].fOutWidth
				);

				//下
				pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * g_aSearchCircle[nCntWave].fInWidth,
					POS_SEARCH_CIRCLE_Y,
					cosf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * g_aSearchCircle[nCntWave].fInWidth
				);

				//各頂点から原点を引く
				g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aSearchCircle[nCntWave].pos;
				g_aSearchCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aSearchCircle[nCntWave].pos;

				//出た向きの値を正規化する
				D3DXVec3Normalize(&g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
				D3DXVec3Normalize(&g_aSearchCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))], &g_aSearchCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

			}
		}
	}

	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		//頂点情報の設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//頂点座標の設定
				pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//法線ベクトルの設定
				pVtx[0].nor = g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//頂点カラーの設定
				pVtx[0].col = g_aSearchCircle[nCntWave].col;

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2
				(
					nCntWidth * (1.0f / (float)(WIDTH / WIDTH)),
					nCntHeight * (1.0f / (float)(HEIGHT))
				);

				pVtx += 1;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSearchCircle->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (g_nNumIndexSearchCircle * MAX_SEARCH_CIRCLE),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSearchCircle,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffSearchCircle->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0, nVtxPoint = 0, nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
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

				pIdx[nCntIdx + 0] = (((WIDTH + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
				pIdx[nCntIdx + 1] = ((WIDTH + 1) * (nCntHeight + 2)) + nVtxPoint;

				nCntIdx += 2;	//2つずつ見てるから
			}
		}

		nVtxPoint += g_nNumVertxSearchCircle;	//頂点数分足す
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffSearchCircle->Unlock();

}

//==================================================================================
//探索範囲の終了処理
//==================================================================================
void UninitSearchCircle(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSearchCircle) / sizeof(*c_apFilenameSearchCircle); nCntTex++)
	{
		if (g_apTextureSearchCircle[nCntTex] != NULL)
		{
			g_apTextureSearchCircle[nCntTex]->Release();
			g_apTextureSearchCircle[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSearchCircle != NULL)
	{
		g_pVtxBuffSearchCircle->Release();
		g_pVtxBuffSearchCircle = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffSearchCircle != NULL)
	{
		g_pIdxBuffSearchCircle->Release();
		g_pIdxBuffSearchCircle = NULL;
	}

}

//==================================================================================
//探索範囲の更新処理
//==================================================================================
void UpdateSearchCircle(void)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSearchCircle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		if (g_aSearchCircle[nCntWave].bUse == true)
		{//使用されていたら

			//今回の位置まで進める
			pVtx += g_nNumVertxSearchCircle * nCntWave;

			//広げていく
			g_aSearchCircle[nCntWave].fOutWidth += g_aSearchCircle[nCntWave].fMove;

			//頂点情報の更新
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * (g_aSearchCircle[nCntWave].fOutWidth - nCntHeight * g_aSearchCircle[nCntWave].fInWidth),
						POS_SEARCH_CIRCLE_Y + nCntHeight * g_aSearchCircle[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * (g_aSearchCircle[nCntWave].fOutWidth - nCntHeight * g_aSearchCircle[nCntWave].fInWidth)
					);

					//頂点カラーの設定
					pVtx[0].col = g_aSearchCircle[nCntWave].col;

					pVtx += 1;
				}
			}

			//進めた分戻す
			pVtx -= (g_nNumVertxSearchCircle * nCntWave + g_nNumVertxSearchCircle);

			//寿命更新
			g_aSearchCircle[nCntWave].nLife--;

			//不透明度更新
			g_aSearchCircle[nCntWave].col.a = g_aSearchCircle[nCntWave].colOrigin.a * (float)g_aSearchCircle[nCntWave].nLife / (float)g_aSearchCircle[nCntWave].nMaxLife;

			if (g_aSearchCircle[nCntWave].nLife <= 0)
			{//寿命が尽きた

				g_aSearchCircle[nCntWave].bUse = false;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSearchCircle->Unlock();
}

//==================================================================================
//影の位置設定処理
//==================================================================================
void SetPositionSearchCircle(int nIdxSearchRadius, D3DXVECTOR3 pos)
{
	if (g_aSearchCircle[nIdxSearchRadius].bUse == true)
	{
		g_aSearchCircle[nIdxSearchRadius].pos = D3DXVECTOR3(pos.x, 1.2f, pos.z);
	}
}

//==================================================================================
//影の位置設定処理
//==================================================================================
void CollisionSearchCircle(void)
{

	////群衆の情報取得
	//Crowd *pCrowd = GetCrowd();

	//for (int nCount = 0; nCount < MAX_CROWD; nCount++, pCrowd++)
	//{
	//	if (pCrowd->aModel.bUse == true)
	//	{//群衆が使用されていたら

	//	 //キャラクター同士の当たり判定
	//	 /*CollisionCharacter(
	//	 &g_aCrowd[nCntCrowd].aModel.pos, &g_aCrowd[nCntCrowd].aModel.posOld, g_aCrowd[nCntCrowd].aModel.vtxMax, g_aCrowd[nCntCrowd].aModel.vtxMin,
	//	 &pCrowd->aModel.pos, pCrowd->aModel.vtxMax, pCrowd->aModel.vtxMin);*/
	//	}
	//}

	////敵とプレイヤーの距離
	//float fLength = pCrowd->aModel.vtxMax.x + fRadius;

	//if (SphereRange(D3DXVECTOR3(), pCrowd->aModel.pos) <= (fLength * fLength))
	//{//球の中に入ったら


	//}
}

//==================================================================================
//探索範囲の描画処理
//==================================================================================
void DrawSearchCircle(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// 背面のカリングはしません。
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntWave = 0, nIdxPoint = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		if (g_aSearchCircle[nCntWave].bUse == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSearchCircle[nCntWave].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSearchCircle[nCntWave].rot.y, g_aSearchCircle[nCntWave].rot.x, g_aSearchCircle[nCntWave].rot.z);
			D3DXMatrixMultiply(&g_aSearchCircle[nCntWave].mtxWorld, &g_aSearchCircle[nCntWave].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aSearchCircle[nCntWave].pos.x, g_aSearchCircle[nCntWave].pos.y, g_aSearchCircle[nCntWave].pos.z);
			D3DXMatrixMultiply(&g_aSearchCircle[nCntWave].mtxWorld, &g_aSearchCircle[nCntWave].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSearchCircle[nCntWave].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSearchCircle, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffSearchCircle);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureSearchCircle[g_aSearchCircle[nCntWave].nTexType]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_nNumVertxSearchCircle/* * nCntWave*/,
				nIdxPoint,
				(g_nNumIndexSearchCircle - 2));

		}

		//今回のインデックス数を加算
		nIdxPoint += g_nNumIndexSearchCircle;
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// 三角形の頂点が反時計回りならば裏向きと見なし描画しません。（デフォルト）
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

//==================================================================================
//探索範囲の設定処理
//==================================================================================
int SetSearchCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fMove, int nTexType)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSearchCircle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posMath[MAX_SEARCH_CIRCLE];		//計算用の座標
	int nCntShadow = -1;

	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		if (g_aSearchCircle[nCntWave].bUse == false)
		{//使用していなかったら

			nCntShadow = nCntWave;			//インデックス番号取得

			//今回の位置まで進める
			pVtx += g_nNumVertxSearchCircle * nCntWave;

			//各要素設定
			g_aSearchCircle[nCntWave].pos = pos;
			g_aSearchCircle[nCntWave].pos.y = 1.2f;
			g_aSearchCircle[nCntWave].rot = rot;
			g_aSearchCircle[nCntWave].col = col;
			g_aSearchCircle[nCntWave].colOrigin = col;
			g_aSearchCircle[nCntWave].nLife = nLife;
			g_aSearchCircle[nCntWave].nMaxLife = nLife;
			g_aSearchCircle[nCntWave].fOutWidth = fWidth;
			g_aSearchCircle[nCntWave].fInWidth = g_aSearchCircle[nCntWave].fOutWidth;
			g_aSearchCircle[nCntWave].fHeight = fHeight;
			g_aSearchCircle[nCntWave].fMove = fMove;
			g_aSearchCircle[nCntWave].nTexType = nTexType;
			g_aSearchCircle[nCntWave].bUse = true;

			//頂点情報の設定
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//横の頂点数分繰り返す

				 //posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				 //(
				 //	sinf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * (g_aSearchCircle[nCntWave].fOutWidth - nCntHeight * g_aSearchCircle[nCntWave].fInWidth),
				 //	POS_IMPACTWAVE_Y + nCntHeight * 1000.0f,
				 //	cosf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * (g_aSearchCircle[nCntWave].fOutWidth - nCntHeight * g_aSearchCircle[nCntWave].fInWidth)
				 //);

				 ////頂点座標の設定
				 //pVtx[0].pos = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))];

					//各頂点から原点を引く
					g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aSearchCircle[nCntWave].pos;

					//出た向きの値を正規化する
					D3DXVec3Normalize(&g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);

					//法線ベクトルの設定
					pVtx[0].nor = g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

					pVtx += 1;
				}
			}

			int nCntIdx = g_nNumIndexSearchCircle * nCntWave;
			int nVtxPoint = g_nNumVertxSearchCircle * nCntWave;	//頂点数分足す

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSearchCircle->Unlock();

	return nCntShadow;

}