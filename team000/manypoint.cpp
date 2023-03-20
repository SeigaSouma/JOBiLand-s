//=============================================================================
//
// 大量発生地点処理 [manypoint.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "manypoint.h"
#include "input.h"
#include "mob.h"

//マクロ定義
#define POS_MANYPOINT_Y	(1000.0f)
#define MOVE		(0.0f)
#define WIDTH		(16)
#define HEIGHT		(1)

//グローバル変数宣言
const char *c_apFilenameManyPoint[] =	//ファイル読み込み
{
	"data\\TEXTURE\\gradation007.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureManyPoint[(sizeof c_apFilenameManyPoint) / sizeof(*c_apFilenameManyPoint)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManyPoint = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffManyPoint = NULL;	//インデックスバッファへのポインタ
MANYPOINT g_aManyPoint;									//大量発生地点の情報

//==================================================================================
//大量発生地点の初期化処理
//==================================================================================
void InitManyPoint(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameManyPoint) / sizeof(*c_apFilenameManyPoint); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameManyPoint[nCntTex],
			&g_apTextureManyPoint[nCntTex]);
	}

	//各要素初期化
	g_aManyPoint.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	g_aManyPoint.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aManyPoint.mtxWorld);
	g_aManyPoint.nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));			//インデックス数
	g_aManyPoint.nNumVertex = (HEIGHT + 1) * (WIDTH + 1);			//頂点数
	g_aManyPoint.bUse = false;				//使用しているか

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aManyPoint.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffManyPoint,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffManyPoint->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[128];		//計算用の座標
	float fRot = (D3DX_PI * 2) / (float)(WIDTH);	//1つごとの角度を求める

	D3DXVECTOR3 posTest = D3DXVECTOR3(1000.0f, 500.0f, 2000.0f);
	D3DXVECTOR3 NormalizeNor[128];

	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * POS_MANYPOINT,
				(POS_MANYPOINT_Y * HEIGHT) - ((POS_MANYPOINT_Y * (nCntHeight + 1))),
				cosf(nCntWidth % WIDTH * fRot) * POS_MANYPOINT
			);

			pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * POS_MANYPOINT,
				(POS_MANYPOINT_Y * HEIGHT) - ((POS_MANYPOINT_Y * nCntHeight)),
				cosf(nCntWidth % WIDTH * fRot) * POS_MANYPOINT
			);

			//各頂点から原点を引く
			NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aManyPoint.pos;
			NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aManyPoint.pos;

			//出た向きの値を正規化する
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))],				&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))],	&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

		}
	}

	//頂点情報の設定
	for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
	{//縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			//頂点座標の設定
			pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

			//法線ベクトルの設定
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))];

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(WIDTH)),
				nCntHeight * (1.0f / (float)(HEIGHT))
			);

			pVtx += 1;
		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffManyPoint->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aManyPoint.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffManyPoint,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffManyPoint->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;

	//頂点番号データの設定
	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//高さの分割数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			pIdx[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
			pIdx[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);
			
			nCntIdx += 2;	//2つずつ見てるから
		}

		if (nCntHeight + 1 < HEIGHT)
		{//最後のちょんは打たない

			pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);

			nCntIdx += 2;	//2つずつ見てるから
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffManyPoint->Unlock();
}

//==================================================================================
//大量発生地点の終了処理
//==================================================================================
void UninitManyPoint(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameManyPoint) / sizeof(*c_apFilenameManyPoint); nCntTex++)
	{
		if (g_apTextureManyPoint[nCntTex] != NULL)
		{
			g_apTextureManyPoint[nCntTex]->Release();
			g_apTextureManyPoint[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffManyPoint != NULL)
	{
		g_pVtxBuffManyPoint->Release();
		g_pVtxBuffManyPoint = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffManyPoint != NULL)
	{
		g_pIdxBuffManyPoint->Release();
		g_pIdxBuffManyPoint = NULL;
	}

}

//==================================================================================
//大量発生地点の更新処理
//==================================================================================
void UpdateManyPoint(void)
{
	ManySpawn *pManySpawn = GetManySpawn();
	static float fAlpha = 0.2f;
	static int fMove = 1;

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffManyPoint->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aManyPoint.bUse == true)
	{//使用されていたら

		//不透明度加算
		fAlpha += 0.01f * fMove;

		if (fAlpha > 0.6f)
		{
			//不透明度加算
			fAlpha = 0.6f;
			fMove *= -1;
		}
		else if (fAlpha < 0.2f)
		{
			//不透明度加算
			fAlpha = 0.2f;
			fMove *= -1;
		}
		

		//大量発生地点に移動
		g_aManyPoint.pos = D3DXVECTOR3(
			pManySpawn->aSpawn[pManySpawn->nBillNumber].pos.x,
			0.0f,
			pManySpawn->aSpawn[pManySpawn->nBillNumber].pos.z);

		//頂点情報の設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

				pVtx += 1;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffManyPoint->Unlock();
}

//==================================================================================
//大量発生地点の描画処理
//==================================================================================
void DrawManyPoint(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// 背面のカリングはしません。
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	if (g_aManyPoint.bUse == true)
	{//使用されていたら

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aManyPoint.mtxWorld);

		//向きを反映する
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aManyPoint.rot.y, g_aManyPoint.rot.x, g_aManyPoint.rot.z);
		D3DXMatrixMultiply(&g_aManyPoint.mtxWorld, &g_aManyPoint.mtxWorld, &mtxRot);

		//位置を反映する
		D3DXMatrixTranslation(&mtxTrans, g_aManyPoint.pos.x, g_aManyPoint.pos.y, g_aManyPoint.pos.z);
		D3DXMatrixMultiply(&g_aManyPoint.mtxWorld, &g_aManyPoint.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aManyPoint.mtxWorld);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffManyPoint, 0, sizeof(VERTEX_3D));

		//インデックスバッファをデータストリームに設定
		pDevice->SetIndices(g_pIdxBuffManyPoint);

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_apTextureManyPoint[0]);

		//ポリゴンの描画
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			g_aManyPoint.nNumVertex,
			0,
			g_aManyPoint.nNumIndex - 2);
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//デフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

//==================================================================================
//大量発生地点の情報取得
//==================================================================================
MANYPOINT *GetManyPoint(void)
{
	return &g_aManyPoint;
}
