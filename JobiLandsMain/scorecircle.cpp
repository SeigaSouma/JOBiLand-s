//=============================================================================
//
// 衝撃波処理 [impactwave.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "impactwave.h"
#include "input.h"

//マクロ定義
#define POS_IMPACTWAVE_Y	(0.0f)
#define WIDTH				(32)	//分割数
#define HEIGHT				(1)		//高さ分割

//グローバル変数宣言
const char *c_apFilenameImpactWave[] =	//ファイル読み込み
{
	"data\\TEXTURE\\gradation001.jpg",
	"data\\TEXTURE\\gradation000.jpg",
	"data\\TEXTURE\\gradation003.jpg",
	"data\\TEXTURE\\gradation004.jpg",
	"data\\TEXTURE\\gradation005.jpg",
	"data\\TEXTURE\\gradation006.jpg",
	"data\\TEXTURE\\gradation007.jpg",
	"data\\TEXTURE\\gradation008.jpg",
	"data\\TEXTURE\\gradation009.jpg",
	"data\\TEXTURE\\gradation011.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureImpactWave[(sizeof c_apFilenameImpactWave) / sizeof(*c_apFilenameImpactWave)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffImpactWave = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffImpactWave = NULL;	//インデックスバッファへのポインタ
IMPACTWAVE g_aImpactWave[MAX_IMPACTWAVE];		//衝撃波の情報
int g_nNumVertx;
int g_nNumIndex;

//==================================================================================
//衝撃波の初期化処理
//==================================================================================
void InitImpactWave(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameImpactWave) / sizeof(*c_apFilenameImpactWave); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameImpactWave[nCntTex],
			&g_apTextureImpactWave[nCntTex]);
	}

	//各要素初期化
	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		g_aImpactWave[nCntWave].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//起点の位置
		g_aImpactWave[nCntWave].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aImpactWave[nCntWave].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動
		g_aImpactWave[nCntWave].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

		for (int nCntNor = 0; nCntNor < MAX_IMPACTWAVE; nCntNor++)
		{
			g_aImpactWave[nCntWave].nor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//法線の向き
		}
		g_aImpactWave[nCntWave].col = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);			//色
		g_aImpactWave[nCntWave].colOrigin = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);	//元の色

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aImpactWave[nCntWave].mtxWorld);
		g_aImpactWave[nCntWave].fOutWidth = 100.0f;		//横幅(外)
		g_aImpactWave[nCntWave].fInWidth = 0.0f;			//幅(内)
		g_aImpactWave[nCntWave].fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);		//1分割数あたりの角度割合
		g_aImpactWave[nCntWave].fHeight = 0.0f;
		g_aImpactWave[nCntWave].fMove = 0.0f;			//広がる速度
		g_aImpactWave[nCntWave].nLife = 0;				//寿命
		g_aImpactWave[nCntWave].nMaxLife = 0;			//最大寿命
		g_aImpactWave[nCntWave].nTexType = 0;			//テクスチャ種類
		g_aImpactWave[nCntWave].bUse = false;				//使用しているか
	}
	g_nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//インデックス数
	g_nNumVertx = (HEIGHT + 1) * (WIDTH + 1);	//頂点数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_nNumVertx * MAX_IMPACTWAVE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffImpactWave,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffImpactWave->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[MAX_IMPACTWAVE];		//計算用の座標

	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//上
				pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * g_aImpactWave[nCntWave].fOutWidth,
					POS_IMPACTWAVE_Y,
					cosf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * g_aImpactWave[nCntWave].fOutWidth
				);

				//下
				pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * g_aImpactWave[nCntWave].fInWidth,
					POS_IMPACTWAVE_Y,
					cosf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * g_aImpactWave[nCntWave].fInWidth
				);

				//各頂点から原点を引く
				g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aImpactWave[nCntWave].pos;
				g_aImpactWave[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aImpactWave[nCntWave].pos;

				//出た向きの値を正規化する
				D3DXVec3Normalize(&g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
				D3DXVec3Normalize(&g_aImpactWave[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))], &g_aImpactWave[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

			}
		}
	}

	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		//頂点情報の設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//頂点座標の設定
				pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//法線ベクトルの設定
				pVtx[0].nor = g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//頂点カラーの設定
				pVtx[0].col = g_aImpactWave[nCntWave].col;

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
	g_pVtxBuffImpactWave->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (g_nNumIndex * MAX_IMPACTWAVE),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffImpactWave,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffImpactWave->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0, nVtxPoint = 0, nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
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

		nVtxPoint += g_nNumVertx;	//頂点数分足す
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffImpactWave->Unlock();

}

//==================================================================================
//衝撃波の終了処理
//==================================================================================
void UninitImpactWave(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameImpactWave) / sizeof(*c_apFilenameImpactWave); nCntTex++)
	{
		if (g_apTextureImpactWave[nCntTex] != NULL)
		{
			g_apTextureImpactWave[nCntTex]->Release();
			g_apTextureImpactWave[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffImpactWave != NULL)
	{
		g_pVtxBuffImpactWave->Release();
		g_pVtxBuffImpactWave = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffImpactWave != NULL)
	{
		g_pIdxBuffImpactWave->Release();
		g_pIdxBuffImpactWave = NULL;
	}

}

//==================================================================================
//衝撃波の更新処理
//==================================================================================
void UpdateImpactWave(void)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	int nCheck = 0;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffImpactWave->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		if (g_aImpactWave[nCntWave].bUse == true)
		{//使用されていたら

			//今回の位置まで進める
			pVtx += g_nNumVertx * nCntWave;
			nCheck += g_nNumVertx * nCntWave;

			g_aImpactWave[nCntWave].pos += g_aImpactWave[nCntWave].move;

			//広げていく
			g_aImpactWave[nCntWave].fOutWidth += g_aImpactWave[nCntWave].fMove;

			//頂点情報の更新
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * (g_aImpactWave[nCntWave].fOutWidth - nCntHeight * g_aImpactWave[nCntWave].fInWidth),
						POS_IMPACTWAVE_Y + nCntHeight * g_aImpactWave[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * (g_aImpactWave[nCntWave].fOutWidth - nCntHeight * g_aImpactWave[nCntWave].fInWidth)
					);

					//頂点カラーの設定
					pVtx[0].col = g_aImpactWave[nCntWave].col;

					pVtx += 1;
					nCheck += 1;
				}
			}

			//進めた分戻す
			pVtx -= (g_nNumVertx * nCntWave + g_nNumVertx);
			nCheck -= (g_nNumVertx * nCntWave + g_nNumVertx);

			//寿命更新
			g_aImpactWave[nCntWave].nLife--;

			//不透明度更新
			g_aImpactWave[nCntWave].col.a = g_aImpactWave[nCntWave].colOrigin.a * (float)g_aImpactWave[nCntWave].nLife / (float)g_aImpactWave[nCntWave].nMaxLife;

			if (g_aImpactWave[nCntWave].nLife <= 0)
			{//寿命が尽きた

				g_aImpactWave[nCntWave].bUse = false;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffImpactWave->Unlock();
}

//==================================================================================
//衝撃波の位置設定処理
//==================================================================================
void SetPositionImpactWave(int nIdxWave, D3DXVECTOR3 move)
{
	if (g_aImpactWave[nIdxWave].bUse == true)
	{
		g_aImpactWave[nIdxWave].pos.x += move.x;
		g_aImpactWave[nIdxWave].pos.z += move.z;
	}
}

//==================================================================================
//衝撃波の描画処理
//==================================================================================
void DrawImpactWave(void)
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

	for (int nCntWave = 0, nIdxPoint = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		if (g_aImpactWave[nCntWave].bUse == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aImpactWave[nCntWave].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aImpactWave[nCntWave].rot.y, g_aImpactWave[nCntWave].rot.x, g_aImpactWave[nCntWave].rot.z);
			D3DXMatrixMultiply(&g_aImpactWave[nCntWave].mtxWorld, &g_aImpactWave[nCntWave].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aImpactWave[nCntWave].pos.x, g_aImpactWave[nCntWave].pos.y, g_aImpactWave[nCntWave].pos.z);
			D3DXMatrixMultiply(&g_aImpactWave[nCntWave].mtxWorld, &g_aImpactWave[nCntWave].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aImpactWave[nCntWave].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffImpactWave, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffImpactWave);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureImpactWave[g_aImpactWave[nCntWave].nTexType]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_nNumVertx/* * nCntWave*/,
				nIdxPoint,
				(g_nNumIndex - 2));

		}

		//今回のインデックス数を加算
		nIdxPoint += g_nNumIndex;
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
//衝撃波の設定処理
//==================================================================================
int SetImpactWave(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fMove, int nTexType)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffImpactWave->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posMath[MAX_IMPACTWAVE];		//計算用の座標

	int nCnt = -1;
	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		if (g_aImpactWave[nCntWave].bUse == false)
		{//使用していなかったら

			//今回の位置まで進める
			pVtx += g_nNumVertx * nCntWave;

			//各要素設定
			g_aImpactWave[nCntWave].pos = pos;
			g_aImpactWave[nCntWave].move.x = sinf(D3DX_PI + rot.y) * move.x;
			g_aImpactWave[nCntWave].move.z = cosf(D3DX_PI + rot.y) * move.z;
			g_aImpactWave[nCntWave].rot = rot;
			g_aImpactWave[nCntWave].col = col;
			g_aImpactWave[nCntWave].colOrigin = col;
			g_aImpactWave[nCntWave].nLife = nLife;
			g_aImpactWave[nCntWave].nMaxLife = nLife;
			g_aImpactWave[nCntWave].fOutWidth = fWidth;
			g_aImpactWave[nCntWave].fInWidth = g_aImpactWave[nCntWave].fOutWidth;
			g_aImpactWave[nCntWave].fHeight = fHeight;
			g_aImpactWave[nCntWave].fMove = fMove;
			g_aImpactWave[nCntWave].nTexType = nTexType;
			g_aImpactWave[nCntWave].bUse = true;

			//頂点情報の設定
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * (g_aImpactWave[nCntWave].fOutWidth - nCntHeight * g_aImpactWave[nCntWave].fInWidth),
						POS_IMPACTWAVE_Y + nCntHeight * g_aImpactWave[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * (g_aImpactWave[nCntWave].fOutWidth - nCntHeight * g_aImpactWave[nCntWave].fInWidth)
					);

					//頂点座標の設定
					pVtx[0].pos = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))];

					//各頂点から原点を引く
					g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aImpactWave[nCntWave].pos;

					//出た向きの値を正規化する
					D3DXVec3Normalize(&g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);

					//法線ベクトルの設定
					pVtx[0].nor = g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

					pVtx += 1;
				}
			}

			int nCntIdx = g_nNumIndex * nCntWave;
			int nVtxPoint = g_nNumVertx * nCntWave;	//頂点数分足す
			nCnt = nCntWave;

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffImpactWave->Unlock();

	return nCnt;
}