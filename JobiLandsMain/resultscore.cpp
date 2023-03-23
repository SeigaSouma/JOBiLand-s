//==============================================================
//
//DirectX[resultscore.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include"main.h"
#include"resultscore.h"
#include"fade.h"
#include "sound.h"

//マクロ定義
#define NUM_PLACE		(2)			//スコアの桁数
#define WIDTH	(100.0f)
#define HEIGHT	(100.0f)

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;		//頂点バッファのポインタ
int g_nResultScore = 0;										//スコアの値

ResultScore g_aResultScore[NUM_PLACE];		//スコア情報

//==============================================================
//リザルトスコアの初期化処理
//==============================================================
void InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int aTexU[NUM_PLACE];

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_05.png",
		&g_pTextureResultScore);

	for (int nCntRS = 0; nCntRS < NUM_PLACE; nCntRS++)
	{
		g_aResultScore[nCntRS].pos = {480.0f + nCntRS * 100.0f,300.0f,0.0f};		//位置を初期化する
		g_aResultScore[nCntRS].move = { 0.0f,0.0f,0.0f };	//移動量を初期化する
		g_aResultScore[nCntRS].fHeight = HEIGHT * 10.0f;			//高さ
		g_aResultScore[nCntRS].fWidth = WIDTH * 10.0f;				//幅
		g_nResultScore = 0;									//値を初期化する
		g_aResultScore[nCntRS].bUse = false;				//使用されていない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	aTexU[0] = g_nResultScore % 100 / 10;
	aTexU[1] = g_nResultScore % 10 / 1;

	for (int nCntRS = 0; nCntRS < NUM_PLACE; nCntRS++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x - g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y - g_aResultScore[nCntRS].fHeight,0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x + g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y - g_aResultScore[nCntRS].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x - g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y + g_aResultScore[nCntRS].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x + g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y + g_aResultScore[nCntRS].fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;											//頂点データのポインタを４つ分集める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//==============================================================
//リザルトスコアの終了処理
//==============================================================
void UninitResultScore(void)
{

	StopSound();

	//テクスチャの破棄
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

//==============================================================
//リザルトスコアの更新処理
//==============================================================
void UpdateResultScore(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	if (GetFade() == FADE_NONE)
	{
		for (int nCntRS = 0; nCntRS < NUM_PLACE; nCntRS++)
		{
			if (g_aResultScore[nCntRS].bUse == false)
			{
				//SEをセット
				//PlaySound(SOUND_LABEL_SE_IDOU);
			}

			g_aResultScore[nCntRS].bUse = true;		//使用されている

			//目的の位置に向かわせる処理
			g_aResultScore[nCntRS].pos.y += ((SCREEN_HEIGHT / 2 - 200) - g_aResultScore[nCntRS].pos.y) * 0.25f;
			g_aResultScore[nCntRS].fHeight += (HEIGHT - g_aResultScore[nCntRS].fHeight) * 0.25f;
			g_aResultScore[nCntRS].fWidth += (WIDTH - g_aResultScore[nCntRS].fWidth) * 0.25f;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x - g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y - g_aResultScore[nCntRS].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x + g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y - g_aResultScore[nCntRS].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x - g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y + g_aResultScore[nCntRS].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x + g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y + g_aResultScore[nCntRS].fHeight, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//==============================================================
//リザルトスコアの描画処理
//==============================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureResultScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (g_aResultScore[nCntScore].bUse == true)
		{//使用されているとき

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);

		}
	}
}

//==============================================================
//リザルトスコアの設定処理
//==============================================================
void SetResultScore(int nScore)
{
	int aTexU[NUM_PLACE];		//各桁の数字を格納
	int nCntScore;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	g_nResultScore = nScore;

	aTexU[0] = g_nResultScore % 100 / 10;
	aTexU[1] = g_nResultScore % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;											//頂点データのポインタを４つ分集める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//==============================================================
//リザルトスコアの加算処理
//==============================================================
void AddResultScore(int nValue)
{
	int aTexU[NUM_PLACE];		//各桁の数字を格納
	int nCntScore;
	VERTEX_2D *pVtx;			//頂点情報へのポインタ

	g_nResultScore += nValue;

	aTexU[0] = g_nResultScore % 100 / 10;
	aTexU[1] = g_nResultScore % 10 / 1;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;											//頂点データのポインタを４つ分集める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultScore->Unlock();
}

//==============================================================
//リザルトスコアの取得
//==============================================================
int GetResultScore(void)
{
	return g_nResultScore;
}