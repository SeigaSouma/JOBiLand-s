//=============================================================================
//
// 衝撃波処理 [scorecircle.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "scorecircle.h"
#include "impactwave.h"
#include "player.h"
#include "input.h"

//マクロ定義
#define POS_SCORECIRCLE_Y	(0.0f)
#define WIDTH				(32)	//分割数
#define HEIGHT				(1)		//高さ分割

//グローバル変数宣言
const char *c_apFilenameScoreCircle[] =	//ファイル読み込み
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
LPDIRECT3DTEXTURE9 g_apTextureScoreCircle[(sizeof c_apFilenameScoreCircle) / sizeof(*c_apFilenameScoreCircle)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreCircle = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffScoreCircle = NULL;	//インデックスバッファへのポインタ
SCORECIRCLE g_aScoreCircle[MAX_SCORECIRCLE];		//衝撃波の情報
int g_nNumVertxScoreCircle;
int g_nNumIndexScoreCircle;

//==================================================================================
//衝撃波の初期化処理
//==================================================================================
void InitScoreCircle(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameScoreCircle) / sizeof(*c_apFilenameScoreCircle); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameScoreCircle[nCntTex],
			&g_apTextureScoreCircle[nCntTex]);
	}

	//各要素初期化
	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		g_aScoreCircle[nCntWave].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//起点の位置
		g_aScoreCircle[nCntWave].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aScoreCircle[nCntWave].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動
		g_aScoreCircle[nCntWave].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

		for (int nCntNor = 0; nCntNor < 256; nCntNor++)
		{
			g_aScoreCircle[nCntWave].nor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//法線の向き
		}
		g_aScoreCircle[nCntWave].col = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);			//色
		g_aScoreCircle[nCntWave].colOrigin = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);	//元の色

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aScoreCircle[nCntWave].mtxWorld);
		g_aScoreCircle[nCntWave].fOutWidth = 100.0f;		//横幅(外)
		g_aScoreCircle[nCntWave].fInWidth = 0.0f;			//幅(内)
		g_aScoreCircle[nCntWave].fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);		//1分割数あたりの角度割合
		g_aScoreCircle[nCntWave].fHeight = 0.0f;
		g_aScoreCircle[nCntWave].fMove = 0.0f;			//広がる速度
		g_aScoreCircle[nCntWave].nLife = 0;				//寿命
		g_aScoreCircle[nCntWave].nMaxLife = 0;			//最大寿命
		g_aScoreCircle[nCntWave].nTexType = 0;			//テクスチャ種類
		g_aScoreCircle[nCntWave].bUse = false;				//使用しているか
	}
	g_nNumIndexScoreCircle = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//インデックス数
	g_nNumVertxScoreCircle = (HEIGHT + 1) * (WIDTH + 1);	//頂点数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_nNumVertxScoreCircle * MAX_SCORECIRCLE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreCircle,
		NULL);

	VERTEX_3D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreCircle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[512];		//計算用の座標

	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//上
				pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * g_aScoreCircle[nCntWave].fOutWidth,
					POS_SCORECIRCLE_Y,
					cosf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * g_aScoreCircle[nCntWave].fOutWidth
				);

				//下
				pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * g_aScoreCircle[nCntWave].fInWidth,
					POS_SCORECIRCLE_Y,
					cosf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * g_aScoreCircle[nCntWave].fInWidth
				);

				//各頂点から原点を引く
				g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aScoreCircle[nCntWave].pos;
				g_aScoreCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aScoreCircle[nCntWave].pos;

				//出た向きの値を正規化する
				D3DXVec3Normalize(&g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
				D3DXVec3Normalize(&g_aScoreCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))], &g_aScoreCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

			}
		}
	}

	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		//頂点情報の設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//頂点座標の設定
				pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//法線ベクトルの設定
				pVtx[0].nor = g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//頂点カラーの設定
				pVtx[0].col = g_aScoreCircle[nCntWave].col;

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
	g_pVtxBuffScoreCircle->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (g_nNumIndexScoreCircle * MAX_SCORECIRCLE),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffScoreCircle,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffScoreCircle->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0, nVtxPoint = 0, nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
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

		nVtxPoint += g_nNumVertxScoreCircle;	//頂点数分足す
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffScoreCircle->Unlock();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//スコアの円設定
	SetScoreCircle(D3DXVECTOR3(-200.0f, pPlayer->pos.y + 70.0f, pPlayer->pos.z - 2.02f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * 0.5f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 100.0f, 0.0f, 0);
	SetScoreCircle(D3DXVECTOR3(-200.0f, pPlayer->pos.y + 70.0f, pPlayer->pos.z - 1.01f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * 0.5f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 200.0f, 0.0f, 0);
	SetScoreCircle(D3DXVECTOR3(-200.0f, pPlayer->pos.y + 70.0f, pPlayer->pos.z), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * 0.5f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 300.0f, 0.0f, 0);
}

//==================================================================================
//衝撃波の終了処理
//==================================================================================
void UninitScoreCircle(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameScoreCircle) / sizeof(*c_apFilenameScoreCircle); nCntTex++)
	{
		if (g_apTextureScoreCircle[nCntTex] != NULL)
		{
			g_apTextureScoreCircle[nCntTex]->Release();
			g_apTextureScoreCircle[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScoreCircle != NULL)
	{
		g_pVtxBuffScoreCircle->Release();
		g_pVtxBuffScoreCircle = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffScoreCircle != NULL)
	{
		g_pIdxBuffScoreCircle->Release();
		g_pIdxBuffScoreCircle = NULL;
	}

}

//==================================================================================
//衝撃波の更新処理
//==================================================================================
void UpdateScoreCircle(void)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	int nCheck = 0;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreCircle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		if (g_aScoreCircle[nCntWave].bUse == true)
		{//使用されていたら

			//今回の位置まで進める
			pVtx += g_nNumVertxScoreCircle * nCntWave;
			nCheck += g_nNumVertxScoreCircle * nCntWave;

			g_aScoreCircle[nCntWave].pos += g_aScoreCircle[nCntWave].move;

			//広げていく
			g_aScoreCircle[nCntWave].fOutWidth += g_aScoreCircle[nCntWave].fMove;

			//頂点情報の更新
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * (g_aScoreCircle[nCntWave].fOutWidth - nCntHeight * g_aScoreCircle[nCntWave].fInWidth),
						POS_SCORECIRCLE_Y + nCntHeight * g_aScoreCircle[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * (g_aScoreCircle[nCntWave].fOutWidth - nCntHeight * g_aScoreCircle[nCntWave].fInWidth)
					);

					//頂点カラーの設定
					pVtx[0].col = g_aScoreCircle[nCntWave].col;

					pVtx += 1;
					nCheck += 1;
				}
			}

			//進めた分戻す
			pVtx -= (g_nNumVertxScoreCircle * nCntWave + g_nNumVertxScoreCircle);
			nCheck -= (g_nNumVertxScoreCircle * nCntWave + g_nNumVertxScoreCircle);

			//寿命更新
			//g_aScoreCircle[nCntWave].nLife--;

			//不透明度更新
			//g_aScoreCircle[nCntWave].col.a = g_aScoreCircle[nCntWave].colOrigin.a * (float)g_aScoreCircle[nCntWave].nLife / (float)g_aScoreCircle[nCntWave].nMaxLife;

			//if (g_aScoreCircle[nCntWave].nLife <= 0)
			//{//寿命が尽きた

			//	g_aScoreCircle[nCntWave].bUse = false;
			//}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScoreCircle->Unlock();
}

//==================================================================================
//衝撃波の位置設定処理
//==================================================================================
void SetPositionScoreCircle(int nIdxWave, D3DXVECTOR3 move)
{
	/*if (g_aScoreCircle[nIdxWave].bUse == true)
	{
		g_aScoreCircle[nIdxWave].pos.x += move.x;
		g_aScoreCircle[nIdxWave].pos.z += move.z;
	}*/
}

//==================================================================================
//衝撃波の描画処理
//==================================================================================
void DrawScoreCircle(void)
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

	for (int nCntWave = 0, nIdxPoint = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		if (g_aScoreCircle[nCntWave].bUse == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aScoreCircle[nCntWave].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aScoreCircle[nCntWave].rot.y, g_aScoreCircle[nCntWave].rot.x, g_aScoreCircle[nCntWave].rot.z);
			D3DXMatrixMultiply(&g_aScoreCircle[nCntWave].mtxWorld, &g_aScoreCircle[nCntWave].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aScoreCircle[nCntWave].pos.x, g_aScoreCircle[nCntWave].pos.y, g_aScoreCircle[nCntWave].pos.z);
			D3DXMatrixMultiply(&g_aScoreCircle[nCntWave].mtxWorld, &g_aScoreCircle[nCntWave].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aScoreCircle[nCntWave].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffScoreCircle, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffScoreCircle);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureScoreCircle[g_aScoreCircle[nCntWave].nTexType]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_nNumVertxScoreCircle/* * nCntWave*/,
				nIdxPoint,
				(g_nNumIndexScoreCircle - 2));

		}

		//今回のインデックス数を加算
		nIdxPoint += g_nNumIndexScoreCircle;
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
void SetScoreCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nTexType)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScoreCircle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posMath[512];		//計算用の座標

	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		if (g_aScoreCircle[nCntWave].bUse == false)
		{//使用していなかったら

			//今回の位置まで進める
			pVtx += g_nNumVertxScoreCircle * nCntWave;

			//各要素設定
			g_aScoreCircle[nCntWave].pos = pos;
			g_aScoreCircle[nCntWave].rot = rot;
			g_aScoreCircle[nCntWave].col = col;
			g_aScoreCircle[nCntWave].colOrigin = col;
			g_aScoreCircle[nCntWave].fOutWidth = fWidth;
			g_aScoreCircle[nCntWave].fInWidth = g_aScoreCircle[nCntWave].fOutWidth;
			g_aScoreCircle[nCntWave].fHeight = fHeight;
			g_aScoreCircle[nCntWave].nTexType = nTexType;
			g_aScoreCircle[nCntWave].bUse = true;

			//頂点情報の設定
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * (g_aScoreCircle[nCntWave].fOutWidth - nCntHeight * g_aScoreCircle[nCntWave].fInWidth),
						POS_SCORECIRCLE_Y + nCntHeight * g_aScoreCircle[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * (g_aScoreCircle[nCntWave].fOutWidth - nCntHeight * g_aScoreCircle[nCntWave].fInWidth)
					);

					//頂点座標の設定
					pVtx[0].pos = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))];

					//各頂点から原点を引く
					g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aScoreCircle[nCntWave].pos;

					//出た向きの値を正規化する
					D3DXVec3Normalize(&g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);

					//法線ベクトルの設定
					pVtx[0].nor = g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

					pVtx += 1;
				}
			}

			int nCntIdx = g_nNumIndexScoreCircle * nCntWave;
			int nVtxPoint = g_nNumVertxScoreCircle * nCntWave;	//頂点数分足す

			break;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScoreCircle->Unlock();
}