//=============================================================================
//
// リアルタイムランキング処理 [realtime_ranking.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include"realtime_ranking.h"
#include"input.h"
#include"fade.h"
#include"crowd.h"
#include"game.h"

//マクロ定義
#define DEFAULT_SCORE	(100)
#define MAX_PATTERN_LIVE	(2)
#define MIN_RANKING		(5)			//ランキング数の最小値
#define NUM_PLACE		(4)			//スコアの桁数
#define WIDTH			(60.0f * 0.45f)
#define HEIGHT			(40.0f * 0.45f)
#define DIS_X			(-30.0f)
#define TEXT_WIDTH			(350.0f * 0.5f)
#define TEXT_HEIGHT			(35.0f * 0.5f)
#define FILE_RANK		"data\\TEXT\\realtime_ranking.bin"		//ランキングファイル

//プロトタイプ宣言
void SetRealTimeRanking(int nCntRank);
void SaveDataRealTimeRanking(void);
void LoadDataRealTimeRanking(void);
void CalRealTimeRanking(void);
void CalNowRealTimeRanking(void);
void UpdateRealTimeRankingTop(void);
void UpdateRealTimeRankingLive(void);

void CreatRealTimeRankingBuff(void);
void CreatRealTimeText(void);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureRealTimeRanking = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRealTimeRanking = NULL;		//頂点バッファのポインタ

const char *c_apFilenameRealTimeText[] =		//ファイル読み込み
{
	"data\\TEXTURE\\neet_vision1.png",
	"data\\TEXTURE\\nextranknum_01.png",
	"data\\TEXTURE\\nowranking_01.png",
	"data\\TEXTURE\\ranktop_01.png",
	"data\\TEXTURE\\neet_vision2.png",
};
LPDIRECT3DTEXTURE9 g_pTextureRealTimeText[(sizeof c_apFilenameRealTimeText) / sizeof(*c_apFilenameRealTimeText)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRealTimeText = NULL;		//頂点バッファのポインタ

RealTimeRanking g_RealTimeRanking;	//ランキングの情報
RealTimeText g_aRealTimeText[REALRANKINGTEXT_VTX_MAX];		//テキストの情報

//==============================================================
//リアルタイムランキングの初期化処理
//==============================================================
void InitRealTimeRanking(void)
{

	//各種変数初期化
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].pos = D3DXVECTOR3(200.0f, 50.0f, 0.0f);
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].pos = D3DXVECTOR3(200.0f, 100.0f, 0.0f);

	for (int nCntRank = 0; nCntRank < REALRANKING_TYPE_MAX; nCntRank++)
	{
		g_RealTimeRanking.aEachRealRanking[nCntRank].nScore = 3;		//それぞれのスコア
		g_RealTimeRanking.aEachRealRanking[nCntRank].nDigit = 0;		//桁数
		g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth = WIDTH;
		g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight = HEIGHT;

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			if (nCntRank == REALRANKING_TYPE_RANKING)
			{
				g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos =
					D3DXVECTOR3(
						-(2 * DIS_X * 0.9f) + g_RealTimeRanking.aEachRealRanking[nCntRank].pos.x + (nCntScore * DIS_X * 0.9f),
						g_RealTimeRanking.aEachRealRanking[nCntRank].pos.y,
						0.0f);
			}
			else
			{
				g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos =
					D3DXVECTOR3(
						-(2 * DIS_X * 0.9f) + g_RealTimeRanking.aEachRealRanking[nCntRank].pos.x + (nCntScore * DIS_X * 0.9f),
						g_RealTimeRanking.aEachRealRanking[nCntRank].pos.y,
						0.0f);
			}

		}
	}

	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].fWidth = WIDTH * 0.95f;
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].fHeight = HEIGHT * 0.95f;
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].fWidth = WIDTH * 0.95f;
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].fHeight = HEIGHT * 0.95f;

	for (int nCntAllRank = 0; nCntAllRank < 512; nCntAllRank++)
	{//全てのランキングデータ初期化

		g_RealTimeRanking.nScore[nCntAllRank] = 0;
	}
	g_RealTimeRanking.nNumRanking = 0;	//ランキングの数
	g_RealTimeRanking.nNowScore = 0;		//現在のスコア
	g_RealTimeRanking.bTop = false;			//1位かどうか

	//ロード処理
	LoadDataRealTimeRanking();

	//計算処理
	CalRealTimeRanking();

	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].nScore = g_RealTimeRanking.nScore[g_RealTimeRanking.nNumRanking - 1];		//それぞれのスコア

	g_RealTimeRanking.nNowRank = g_RealTimeRanking.nNumRanking + 1;	//現在の順位


	CreatRealTimeRankingBuff();
	CreatRealTimeText();

}

//==============================================================
//リアルタイムランキングの終了処理
//==============================================================
void UninitRealTimeRanking(void)
{

	//テクスチャの破棄
	if (g_pTextureRealTimeRanking != NULL)
	{
		g_pTextureRealTimeRanking->Release();
		g_pTextureRealTimeRanking = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRealTimeRanking != NULL)
	{
		g_pVtxBuffRealTimeRanking->Release();
		g_pVtxBuffRealTimeRanking = NULL;
	}

	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRealTimeText) / sizeof(*c_apFilenameRealTimeText); nCntTex++)
	{
		if (g_pTextureRealTimeText[nCntTex] != NULL)
		{
			g_pTextureRealTimeText[nCntTex]->Release();
			g_pTextureRealTimeText[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRealTimeText != NULL)
	{
		g_pVtxBuffRealTimeText->Release();
		g_pVtxBuffRealTimeText = NULL;
	}

}

//==============================================================
//リアルタイムランキングの更新処理
//==============================================================
void UpdateRealTimeRanking(void)
{

	//群衆の人数取得
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//現在のスコア
	g_RealTimeRanking.nNowScore = 3 + pCrowdInfo->nCrowdValue;

	//現在の順位計算処理
	CalNowRealTimeRanking();

	//記録更新中の処理
	UpdateRealTimeRankingTop();

	//ライブアニメーション
	UpdateRealTimeRankingLive();

	for (int nCntRank = 0; nCntRank < REALRANKING_TYPE_MAX; nCntRank++)
	{
		//数字のテクスチャ設定
		SetRealTimeRanking(nCntRank);
	}
}

//==============================================================
//リアルタイムランキングの記録更新中処理
//==============================================================
void UpdateRealTimeRankingTop(void)
{

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRealTimeText->Lock(0, 0, (void**)&pVtx, 0);

	static int nMove = 1;

	if (g_RealTimeRanking.bTop == true)
	{//記録更新中だったら

		g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth += (TEXT_WIDTH * 0.025f) * nMove;
		g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight += (TEXT_HEIGHT * 0.025f) * nMove;

		if (g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth >= TEXT_WIDTH * 1.5f)
		{
			g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth = TEXT_WIDTH * 1.5f;
			g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight = TEXT_HEIGHT * 1.5f;
			nMove *= -1;
		}
		else if (g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth <= TEXT_WIDTH)
		{
			g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth = TEXT_WIDTH;
			g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight = TEXT_HEIGHT;
			nMove *= -1;
		}
	}
	else
	{
		g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth = TEXT_WIDTH;
		g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight = TEXT_HEIGHT;
	}

	//頂点座標の設定
	pVtx[REALRANKINGTEXT_VTX_TOP * 4 + 0].pos = D3DXVECTOR3(g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.x - g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth, g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.y - g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_TOP * 4 + 1].pos = D3DXVECTOR3(g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.x + g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth, g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.y - g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_TOP * 4 + 2].pos = D3DXVECTOR3(g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.x - g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth, g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.y + g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_TOP * 4 + 3].pos = D3DXVECTOR3(g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.x + g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth, g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.y + g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRealTimeText->Unlock();
}

//==============================================================
//リアルタイムランキングのライブ点滅処理
//==============================================================
void UpdateRealTimeRankingLive(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRealTimeText->Lock(0, 0, (void**)&pVtx, 0);

	g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nCntAnim++;	//カウンターを加算

	if ((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nCntAnim % 60) == 0)
	{//規定カウント進んだら

		//パターンNo.を更新
		g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim = (g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim + 1) % MAX_PATTERN_LIVE;
		g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nCntAnim = 0;
	}

	//頂点座標の設定
	pVtx[REALRANKINGTEXT_VTX_FADE * 4 + 0].tex = D3DXVECTOR2((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim % MAX_PATTERN_LIVE) * 0.5f, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_FADE * 4 + 1].tex = D3DXVECTOR2((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim % MAX_PATTERN_LIVE + 1) * 0.5f, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_FADE * 4 + 2].tex = D3DXVECTOR2((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim % MAX_PATTERN_LIVE) * 0.5f, 1.0f);
	pVtx[REALRANKINGTEXT_VTX_FADE * 4 + 3].tex = D3DXVECTOR2((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim % MAX_PATTERN_LIVE + 1) * 0.5f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRealTimeText->Unlock();
}

//==============================================================
//リアルタイムランキングの描画処理
//==============================================================
void DrawRealTimeRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRealTimeText, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRank = 0; nCntRank < REALRANKINGTEXT_VTX_GIZA; nCntRank++)
	{
		if (nCntRank == REALRANKINGTEXT_VTX_TOP && g_RealTimeRanking.bTop == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRealTimeText[nCntRank]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
		}
		else if (nCntRank != REALRANKINGTEXT_VTX_TOP)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRealTimeText[nCntRank]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
		}
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRealTimeRanking, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRank = 0; nCntRank < g_RealTimeRanking.nNumRanking; nCntRank++)
	{
		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			if (g_RealTimeRanking.aEachRealRanking[nCntRank].nDigit > nCntScore)
			{//ケタ分表示

				if (nCntRank == REALRANKING_TYPE_RANKING && g_RealTimeRanking.bTop == false)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_pTextureRealTimeRanking);

					//ポリゴンの描画
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntScore * 4) + (nCntRank * NUM_PLACE * 4), 2);
				}
				else if (nCntRank != REALRANKING_TYPE_RANKING)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_pTextureRealTimeRanking);

					//ポリゴンの描画
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntScore * 4) + (nCntRank * NUM_PLACE * 4), 2);
				}
			}
		}
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRealTimeText, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_RealTimeRanking.bTop == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRealTimeText[REALRANKINGTEXT_VTX_TOP]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, REALRANKINGTEXT_VTX_TOP * 4, 2);
	}

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRealTimeText, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureRealTimeText[REALRANKINGTEXT_VTX_GIZA]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, REALRANKINGTEXT_VTX_GIZA * 4, 2);
	

}

//==============================================================
//リアルタイムランキングの設定処理
//==============================================================
void SetRealTimeRanking(int nCntRank)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRealTimeRanking->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntRank * NUM_PLACE;

	int nCount = 0, nValue = g_RealTimeRanking.aEachRealRanking[nCntRank].nScore;
	do
	{//0になるまでカウント進める

		nValue /= 10;
		nCount++;

	} while (nValue > 0);

	//ケタ割り出し
	g_RealTimeRanking.aEachRealRanking[nCntRank].nDigit = nCount;

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

		aTexU[nCntTex] = g_RealTimeRanking.aEachRealRanking[nCntRank].nScore % nDigit / (nDigit / 10);
		nDigit /= 10;
	}

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRealTimeRanking->Unlock();
}

//==============================================================
//リアルタイムランキングの現在の順位計算処理
//==============================================================
void CalNowRealTimeRanking(void)
{
	int nCntAllRank = 0;
	for (nCntAllRank = 0; nCntAllRank < g_RealTimeRanking.nNumRanking; nCntAllRank++)
	{//全てのランキングデータ

		if (g_RealTimeRanking.nScore[nCntAllRank] <= g_RealTimeRanking.nNowScore)
		{//現在のランキングの方が大きかったら

			//順位を設定
			g_RealTimeRanking.nNowRank = nCntAllRank + 1;

			if (nCntAllRank <= 0)
			{//0以下の時
				
				//1位かどうかの判定
				g_RealTimeRanking.bTop = true;
			}
			else
			{
				g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].nScore = g_RealTimeRanking.nScore[nCntAllRank - 1];

				//1位かどうかの判定
				g_RealTimeRanking.bTop = false;
			}
			break;
		}
	}
	

	if (g_RealTimeRanking.nScore[nCntAllRank - 1] > g_RealTimeRanking.nNowScore)
	{//現在のランキングの1つ下よりも現在のスコアが低い

		//ランキングを1つ下げる
		g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].nScore = g_RealTimeRanking.nScore[nCntAllRank - 1];
		g_RealTimeRanking.nNowRank = nCntAllRank + 1;
	}

	//現在の順位
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].nScore = g_RealTimeRanking.nNowRank;
}

//==============================================================
//ロード処理
//==============================================================
void LoadDataRealTimeRanking(void)
{
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(FILE_RANK, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//ファイルに数値を読み込む
		fread(&g_RealTimeRanking.nNumRanking, sizeof(int), 1, pFile);
		fread(&g_RealTimeRanking.nScore[0], sizeof(int), g_RealTimeRanking.nNumRanking, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合

		g_RealTimeRanking.nNumRanking = MIN_RANKING;
		for (int nCount = 0; nCount < g_RealTimeRanking.nNumRanking; nCount++)
		{//全部ゼロ

			g_RealTimeRanking.nScore[nCount] = 200 - (nCount * 10);
		}

		SaveDataRealTimeRanking();

	}
}

//==============================================================
//計算処理
//==============================================================
void CalRealTimeRanking(void)
{

	//降順処理
	for (int nCntData = 0; nCntData < g_RealTimeRanking.nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < g_RealTimeRanking.nNumRanking; nCntSort++)
		{
			if (g_RealTimeRanking.nScore[nCntData] < g_RealTimeRanking.nScore[nCntSort])
			{//要素1より要素2が大きかったら

				int nTemp = g_RealTimeRanking.nScore[nCntData];
				g_RealTimeRanking.nScore[nCntData] = g_RealTimeRanking.nScore[nCntSort];
				g_RealTimeRanking.nScore[nCntSort] = nTemp;
			}
		}
	}
}

//==============================================================
//セーブ処理
//==============================================================
void SaveDataRealTimeRanking(void)
{
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(FILE_RANK, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//ファイルに数値を書き出す
		fwrite(&g_RealTimeRanking.nNumRanking, sizeof(int), 1, pFile);
		fwrite(&g_RealTimeRanking.nScore[0], sizeof(int), g_RealTimeRanking.nNumRanking, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
}

//==============================================================
//リアルタイムランキングの頂点バッファ生成
//==============================================================
void CreatRealTimeRankingBuff(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_04.png",
		&g_pTextureRealTimeRanking);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE * REALRANKING_TYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRealTimeRanking,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRealTimeRanking->Lock(0, 0, (void**)&pVtx, 0);

	int aTexU[NUM_PLACE];

	for (int nCntRank = 0; nCntRank < REALRANKING_TYPE_MAX; nCntRank++)
	{
		int nDigit = 1;		// aTexU計算用

							//計算用割り出し
		for (int nCntDigit = 0; nCntDigit < NUM_PLACE; nCntDigit++)
		{
			nDigit *= 10;
		}

		//テクスチャ座標に代入する
		for (int nCntTex = 0; nCntTex < NUM_PLACE; nCntTex++)
		{//桁数分設定

			aTexU[nCntTex] = g_RealTimeRanking.aEachRealRanking[nCntRank].nScore % nDigit / (nDigit / 10);
			nDigit /= 10;
		}

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x - g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth, g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y - g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x + g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth, g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y - g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x - g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth, g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y + g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x + g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth, g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y + g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight, 0.0f);

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
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRealTimeRanking->Unlock();
}

//==============================================================
//リアルタイムランキング(テキスト)の頂点バッファ生成
//==============================================================
void CreatRealTimeText(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRealTimeText) / sizeof(*c_apFilenameRealTimeText); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameRealTimeText[nCntTex],
			&g_pTextureRealTimeText[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * REALRANKINGTEXT_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRealTimeText,
		NULL);

	for (int nCntRank = 0; nCntRank < REALRANKINGTEXT_VTX_MAX; nCntRank++)
	{
		//UI情報の初期化
		ZeroMemory(&g_aRealTimeText[nCntRank].aUI, sizeof(UI_2D));
	}

	for (int nCntRank = 0; nCntRank < REALRANKINGTEXT_VTX_MAX; nCntRank++)
	{
		g_aRealTimeText[nCntRank].aUI.fWidth = TEXT_WIDTH;
		g_aRealTimeText[nCntRank].aUI.fHeight = TEXT_HEIGHT;
	}

	//フェード幕
	g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.pos = D3DXVECTOR3(
		165.0f,
		75.0f,
		0.0f);

	//次の順位の人数
	g_aRealTimeText[REALRANKINGTEXT_VTX_NEXTNUM].aUI.pos = D3DXVECTOR3(
		195.0f,
		g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].pos.y,
		0.0f);

	//現在の順位&現在の人数
	g_aRealTimeText[REALRANKINGTEXT_VTX_NOW].aUI.pos = D3DXVECTOR3(
		255.0f,
		g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].pos.y,
		0.0f);

	g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos = D3DXVECTOR3(
		200.0f,
		g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].pos.y,
		0.0f);

	//ギザギザノイズ
	g_aRealTimeText[REALRANKINGTEXT_VTX_GIZA].aUI.pos = g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.pos;

	g_aRealTimeText[REALRANKINGTEXT_VTX_NOW].aUI.fWidth = TEXT_WIDTH;
	g_aRealTimeText[REALRANKINGTEXT_VTX_NOW].aUI.fHeight = TEXT_HEIGHT;

	g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.fWidth = 165.0f;
	g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.fHeight = TEXT_HEIGHT * 6.0f;

	g_aRealTimeText[REALRANKINGTEXT_VTX_GIZA].aUI.fWidth = g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.fWidth;
	g_aRealTimeText[REALRANKINGTEXT_VTX_GIZA].aUI.fHeight = g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.fHeight;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRealTimeText->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < REALRANKINGTEXT_VTX_MAX; nCntRank++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aRealTimeText[nCntRank].aUI.pos.x - g_aRealTimeText[nCntRank].aUI.fWidth, g_aRealTimeText[nCntRank].aUI.pos.y - g_aRealTimeText[nCntRank].aUI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRealTimeText[nCntRank].aUI.pos.x + g_aRealTimeText[nCntRank].aUI.fWidth, g_aRealTimeText[nCntRank].aUI.pos.y - g_aRealTimeText[nCntRank].aUI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRealTimeText[nCntRank].aUI.pos.x - g_aRealTimeText[nCntRank].aUI.fWidth, g_aRealTimeText[nCntRank].aUI.pos.y + g_aRealTimeText[nCntRank].aUI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRealTimeText[nCntRank].aUI.pos.x + g_aRealTimeText[nCntRank].aUI.fWidth, g_aRealTimeText[nCntRank].aUI.pos.y + g_aRealTimeText[nCntRank].aUI.fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		if (nCntRank != REALRANKINGTEXT_VTX_GIZA)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		}

		//テクスチャ座標の設定
		if (nCntRank == REALRANKINGTEXT_VTX_FADE)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRealTimeText->Unlock();
}