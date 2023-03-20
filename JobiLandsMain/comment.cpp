//=============================================================================
//
// コメント処理 [comment.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "ranking.h"
#include "comment.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
#include "ranking_comment.h"

//マクロ定義
#define MAX_TOP			(4)		//頂点数
#define TEX_V		(0.1f)		//V座標
#define SPAWNCOUNTER		(40)
#define MOVE		(5.0f)
#define WIDTH		(400.0f)
#define HEIGHT		(30.0f)

//プロトタイプ宣言
void UpdateManySpawnComment(void);
void UpdateRankingSpawnComment(void);

//グローバル変数宣言
const char *c_apFilenameCommnet[] =		//ファイル読み込み
{
	"data\\TEXTURE\\comment_01.png",
	"data\\TEXTURE\\comment_02.png",
	"data\\TEXTURE\\comment_03.png",
	"data\\TEXTURE\\comment_04.png",
	"data\\TEXTURE\\comment_05.png",
	"data\\TEXTURE\\comment_06.png",
	"data\\TEXTURE\\comment_07.png",
	"data\\TEXTURE\\comment_08.png",
	"data\\TEXTURE\\comment_09.png",
	"data\\TEXTURE\\comment_10.png",
};
LPDIRECT3DTEXTURE9 g_pTextureCommnet[(sizeof c_apFilenameCommnet) / sizeof(*c_apFilenameCommnet)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCommnet = NULL;	//頂点バッファへのポインタ
Commnet g_aCommnet;						//コメントの情報

//==================================================================================
//コメントの初期化処理
//==================================================================================
void InitCommnet(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCommnet) / sizeof(*c_apFilenameCommnet); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCommnet[nCntTex],
			&g_pTextureCommnet[nCntTex]);
	}

	//コメントの情報の初期化
	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		g_aCommnet.aInfo[nCntCommnet].pos = D3DXVECTOR3(640.0f, 500.0f, 0.0f);
		g_aCommnet.aInfo[nCntCommnet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
		g_aCommnet.aInfo[nCntCommnet].nType = 0;	//種類
		g_aCommnet.aInfo[nCntCommnet].nTexV = 0;	//テクスチャの何分割目か
		g_aCommnet.aInfo[nCntCommnet].bUse = false;	//使用しているか
	}

	//コメントの初期化
	g_aCommnet.nCntSpawn = 0;
	g_aCommnet.nNumSpawn = 0;
	g_aCommnet.bUse = false;    //使用しているか

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * MAX_NICOCOME,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffCommnet,
								NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCommnet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCommnet->Unlock();
}

//==================================================================================
//コメントの終了処理
//==================================================================================
void UninitCommnet(void)
{

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCommnet) / sizeof(*c_apFilenameCommnet); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureCommnet[nCntTex] != NULL)
		{
			g_pTextureCommnet[nCntTex]->Release();
			g_pTextureCommnet[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCommnet != NULL)
	{
		g_pVtxBuffCommnet->Release();
		g_pVtxBuffCommnet = NULL;
	}
}

//==================================================================================
//コメントの更新処理
//==================================================================================
void UpdateCommnet(void)
{

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCommnet->Lock(0, 0, (void**)&pVtx, 0);

	if (GetMode() == MODE_GAME)
	{
		//出現までの更新処理
		UpdateManySpawnComment();
	}
	else
	{
		UpdateRankingSpawnComment();
	}

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aCommnet.aInfo[nCntCommnet].bUse == true)
		{//使用されていたら

			//移動
			g_aCommnet.aInfo[nCntCommnet].pos.x -= g_aCommnet.aInfo[nCntCommnet].move.x;

			if (g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH <= 0.0f)
			{//画面外に出たら

				//使用していない状態にする
				g_aCommnet.aInfo[nCntCommnet].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
		}

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCommnet->Unlock();
}

//==================================================================================
//大量発生のコメント更新処理
//==================================================================================
void UpdateManySpawnComment(void)
{
	if (g_aCommnet.bUse == true)
	{//使用されていたら

		//大量発生のコメントカウント加算
		g_aCommnet.nCntSpawn++;

		//出現するまでのカウンター設定
		int nSpawnCounter = SPAWNCOUNTER - (g_aCommnet.nNumSpawn * 10);

		if (nSpawnCounter <= 4)
		{
			nSpawnCounter = 4;
		}

		if (g_aCommnet.nCntSpawn >= nSpawnCounter)
		{//出現までのカウントに達したら

			//大量発生のコメントカウントリセット
			g_aCommnet.nCntSpawn = 0;

			//回数加算
			g_aCommnet.nNumSpawn++;

			//コメントの設定
			SetComment();
		}
	}
}

//==================================================================================
//ランキングのコメント更新処理
//==================================================================================
void UpdateRankingSpawnComment(void)
{

	//ランキングの情報取得
	Ranking *pRanking = GetRanking();

	if (g_aCommnet.bUse == true)
	{//使用されていたら

		//出現するまでのカウンター設定
		int nSpawnCounter = SPAWNCOUNTER;

		if (pRanking->nOldMode == MODE_TITLE)
		{//ランキングの遷移前がリザルトの時

			nSpawnCounter = 80;
		}

		//大量発生のコメントカウント加算
		g_aCommnet.nCntSpawn++;

		if (g_aCommnet.nCntSpawn >= nSpawnCounter)
		{//出現までのカウントに達したら

			//大量発生のコメントカウントリセット
			g_aCommnet.nCntSpawn = 0;

			//回数加算
			g_aCommnet.nNumSpawn++;

			//コメントの設定
			SetComment();
		}
	}
}

//==================================================================================
//コメントの描画処理
//==================================================================================
void DrawCommnet(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCommnet, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テキストの描画
	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aCommnet.aInfo[nCntCommnet].bUse == true)
		{//使用されていたら

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureCommnet[g_aCommnet.aInfo[nCntCommnet].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCommnet * 4, 2);
		}
	}

}

//==================================================================================
//コメント情報取得
//==================================================================================
void SetComment(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//ランキングの情報取得
	Ranking *pRanking = GetRanking();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCommnet->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aCommnet.bUse == false)
	{//使用されていなかったら

		//大量発生のコメントを使用状態にする
		g_aCommnet.bUse = true;
	}

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aCommnet.aInfo[nCntCommnet].bUse == false)
		{//使用していなかったら

			//出現横ランダム
			g_aCommnet.aInfo[nCntCommnet].pos.x = SCREEN_WIDTH + WIDTH;
			g_aCommnet.aInfo[nCntCommnet].pos.x += (rand() % 6) * 10.0f;

			if (GetMode() == MODE_RANKING)
			{
				g_aCommnet.aInfo[nCntCommnet].pos.x -= (SCREEN_WIDTH * 0.3f);
			}

			//出現高さランダム
			g_aCommnet.aInfo[nCntCommnet].pos.y = SCREEN_HEIGHT * 0.5f;
			g_aCommnet.aInfo[nCntCommnet].pos.y += (rand() % 7 - 3) * 80.0f;
			g_aCommnet.aInfo[nCntCommnet].pos.y += (rand() % 7 - 3) * 20.0f;

			//移動量ランダム
			g_aCommnet.aInfo[nCntCommnet].move.x = (float)(rand() % 80) * 0.1f + MOVE;

			//種類ランダム
			if (GetMode() != MODE_RANKING)
			{//ランキング以外

				g_aCommnet.aInfo[nCntCommnet].nType = rand() % 4;
			}
			else
			{//ランキングのとき

				if (pRanking->nOldMode == MODE_TITLE)
				{//タイトルからランキングいったとき

					g_aCommnet.aInfo[nCntCommnet].nType = rand() % 2 + 8;
				}
				else
				{//リザルトからランキングにいったとき

					g_aCommnet.aInfo[nCntCommnet].nType = rand() % 2 + 4;

					if (pRanking->bNewRecord == true && rand() % 2 == 0)
					{//ニューレコードが入った時

						g_aCommnet.aInfo[nCntCommnet].nType = 6;
					}
					else if (pRanking->bNewRecord == false && rand() % 2 == 0)
					{//ランキング外

						g_aCommnet.aInfo[nCntCommnet].nType = 7;
					}
				}
			}

			//テクスチャの何分割目か
			g_aCommnet.aInfo[nCntCommnet].nTexV = rand() % 10;

			//使用している状態にする
			g_aCommnet.aInfo[nCntCommnet].bUse = true;

			if (GetMode() == MODE_RANKING)
			{
				SetRankingComment(g_aCommnet.aInfo[nCntCommnet].nTexV, g_aCommnet.aInfo[nCntCommnet].nType);
			}


			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V);
			pVtx[1].tex = D3DXVECTOR2(1.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
			pVtx[3].tex = D3DXVECTOR2(1.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCommnet->Unlock();
}

//==================================================================================
//コメント情報取得
//==================================================================================
Commnet *GetCommnet(void)
{
	return &g_aCommnet;
}