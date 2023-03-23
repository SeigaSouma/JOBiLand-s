//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include"score.h"
#include"input.h"
#include"fade.h"
#include"game.h"

//マクロ定義
#define NUM_PLACE		(4)			//スコアの桁数
#define WIDTH			(60.0f)
#define HEIGHT			(40.0f)
#define DIS_X			(-70.0f)

//プロトタイプ宣言
void CreatScoreBuff(void);
void SetScore(void);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//頂点バッファのポインタ

Score g_Score;	//ランキングの情報

//==============================================================
//スコアの初期化処理
//==============================================================
void InitScore(void)
{

	//各種変数初期化
	g_Score.pos = D3DXVECTOR3(640.0f, 50.0f, 0.0f);
	g_Score.nScore = 0;		//それぞれのスコア
	g_Score.nDigit = 0;		//桁数
	g_Score.fWidth = WIDTH;
	g_Score.fHeight = HEIGHT;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		g_Score.aScoreNum[nCntScore].pos =
			D3DXVECTOR3(
				(g_Score.pos.x + (nCntScore * DIS_X)) - ((DIS_X * NUM_PLACE) * 0.5f),
				g_Score.pos.y,
				0.0f);
	}

	//スコアの頂点バッファ生成
	CreatScoreBuff();

}

//==============================================================
//スコアの終了処理
//==============================================================
void UninitScore(void)
{

	//テクスチャの破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}


}

//==============================================================
//スコアの更新処理
//==============================================================
void UpdateScore(void)
{
	//現在のスコア
	g_Score.nScore = 333;

	//数字のテクスチャ設定
	SetScore();
}

//==============================================================
//スコアの描画処理
//==============================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (g_Score.nDigit > nCntScore)
		{//ケタ分表示

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureScore);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
		}
	}

}

//==============================================================
//スコアの設定処理
//==============================================================
void SetScore(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int nCount = 0, nValue = g_Score.nScore;
	do
	{//0になるまでカウント進める

		nValue /= 10;
		nCount++;

	} while (nValue > 0);

	//ケタ割り出し
	g_Score.nDigit = nCount;

	int aTexU[NUM_PLACE];
	int nDigit = 1;		// aTexU計算用

	//計算用割り出し
	for (int nCntDigit = 0; nCntDigit < NUM_PLACE; nCntDigit++)
	{
		nDigit *= 10;
	}
	
	//テクスチャ座標に代入する
	for (int nCntTex = NUM_PLACE - 1; nCntTex >= 0; nCntTex--)
	{//桁数分設定

		aTexU[nCntTex] = g_Score.nScore % nDigit / (nDigit / 10);
		nDigit /= 10;
	}

	for (int nCntScore = 0; nCntScore < g_Score.nDigit; nCntScore++)
	{
		g_Score.aScoreNum[nCntScore].pos =
			D3DXVECTOR3(
			(g_Score.pos.x + (nCntScore * DIS_X)) - ((DIS_X * g_Score.nDigit) * 0.5f),
				g_Score.pos.y,
				0.0f);
	}

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x - g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y - g_Score.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x + g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y - g_Score.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x - g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y + g_Score.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x + g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y + g_Score.fHeight, 0.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}

//==============================================================
//スコアの頂点バッファ生成
//==============================================================
void CreatScoreBuff(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_04.png",
		&g_pTextureScore);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int aTexU[NUM_PLACE];
	int nDigit = 1;		// aTexU計算用

	//計算用割り出し
	for (int nCntDigit = 0; nCntDigit < NUM_PLACE; nCntDigit++)
	{
		nDigit *= 10;
	}

	//テクスチャ座標に代入する
	for (int nCntTex = 0; nCntTex < NUM_PLACE; nCntTex++)
	{//桁数分設定

		aTexU[nCntTex] = g_Score.nScore % nDigit / (nDigit / 10);
		nDigit /= 10;
	}

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x - g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y - g_Score.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x + g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y - g_Score.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x - g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y + g_Score.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x + g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y + g_Score.fHeight, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffScore->Unlock();
}
