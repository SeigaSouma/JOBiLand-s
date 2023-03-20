//=============================================================================
//
// ランキング処理 [ranking.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "light.h"
#include "camera.h"
#include "shadow.h"
#include "edit.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "particle.h"
#include "effect.h"
#include "2D_effect.h"
#include "player.h"
#include "sound.h"
#include "sound.h"

//マクロ定義
#define DEFAULT_SCORE	(100)
#define MIN_RANKING		(5)			//ランキング数の最小値
#define NUM_PLACE		(4)			//スコアの桁数
#define SET_TIME		(80)
#define CHANGE_TIME		(60 * 25)
#define START_X			(580.0f)
#define START_TEXT_X	(640.0f)
#define WIDTH			(60.0f * 1.5f)
#define HEIGHT			(40.0f * 1.5f)
#define DIS_X			(-42.0f * 1.9f)
#define DIS_Y			(130.0f)
//#define TEXT_WIDTH		(64.0f * 5.0f)
#define TEXT_WIDTH		(640.0f)
#define TEXT_HEIGHT		(360.0f)
#define FILE_RANK		"data\\TEXT\\realtime_ranking.bin"		//ランキングファイル

//プロトタイプ宣言
void SetRanking(int nCntRank);
void SaveDataRanking(void);
void LoadDataRanking(void);
void CalRanking(void);
void SetProvisionRanking(void);
void UpdateNewRecordRanking(void);

void CreatRankingBuff(void);
void CreatRankingText(void);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;		//頂点バッファのポインタ

const char *c_apFilenameRankingText[] =		//ファイル読み込み
{
	"data\\TEXTURE\\ranking_02.png",
};
LPDIRECT3DTEXTURE9 g_pTextureRankingText[(sizeof c_apFilenameRankingText) / sizeof(*c_apFilenameRankingText)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingText = NULL;		//頂点バッファのポインタ

Ranking g_Ranking;	//ランキングの情報
RankingText g_aRankingText;		//テキストの情報

//==============================================================
//ランキングの初期化処理
//==============================================================
void InitRanking(int nOldMode)
{

	//各種変数初期化
	for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
	{
		g_Ranking.aEachRealRanking[nCntRank].pos = D3DXVECTOR3(START_X, 100.0f + DIS_Y * nCntRank, 0.0f);
		g_Ranking.aEachRealRanking[nCntRank].posDest = D3DXVECTOR3(START_X, 100.0f + DIS_Y * nCntRank, 0.0f);	//目標の位置
		g_Ranking.aEachRealRanking[nCntRank].nScore = 0;		//それぞれのスコア
		g_Ranking.aEachRealRanking[nCntRank].nDigit = 0;		//桁数
		g_Ranking.aEachRealRanking[nCntRank].fWidth = WIDTH;
		g_Ranking.aEachRealRanking[nCntRank].fHeight = HEIGHT;
		g_Ranking.aEachRealRanking[nCntRank].bUse = false;		//使用しているか

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos =
				D3DXVECTOR3(
					g_Ranking.aEachRealRanking[nCntRank].pos.x + (nCntScore * DIS_X),
					g_Ranking.aEachRealRanking[nCntRank].pos.y,
					0.0f);
		}
	}

	for (int nCntAllRank = 0; nCntAllRank < 512; nCntAllRank++)
	{//全てのランキングデータ初期化

		g_Ranking.nScore[nCntAllRank] = 0;
	}
	g_Ranking.nNumRanking = 0;	//ランキングの数
	g_Ranking.nChangeTime = 0;	//自動遷移のカウンター
	g_Ranking.nCntSetCounter = 0;	//セットするまでのカウンター
	g_Ranking.nCntUse = 0;		//使用カウント

	//今回のスコア取得
	g_Ranking.nNewRecordNum = -1;	//ニューレコードの番号
	g_Ranking.bNewRecord = false;	//ニューレコードの判定
	g_Ranking.nOldMode = nOldMode;		//遷移前のモード

	//ロード処理
	LoadDataRanking();

	if (g_Ranking.nOldMode == MODE_RESULT)
	{//自動遷移では更新しない

		//計算処理
		CalRanking();

		//セーブ
		SaveDataRanking();
	}

	//頂点バッファ生成
	CreatRankingBuff();
	CreatRankingText();

	for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
	{
		SetRanking(nCntRank);
	}

	//影の初期化処理
	InitShadow();

	//モデルの初期化処理
	InitModel();

	//エディットの初期化
	InitEdit();

	//プレイヤーの初期化処理
	InitPlayer();

	//メッシュフィールドの初期化処理
	InitMeshField();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	//メッシュドームの初期化処理
	InitMeshDome();

	//エフェクトの初期化処理
	InitEffect();

	//2Dエフェクトの初期化処理
	InitEffect_2D();

	//パーティクルの初期化処理
	InitParticle();

	//カメラ初期化処理
	InitCamera();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM_RANKING);
}

//==============================================================
//ランキングの終了処理
//==============================================================
void UninitRanking(void)
{

	//サウンドの停止
	StopSound();

	//影の終了処理
	UninitShadow();

	//モデルの終了処理
	UninitModel();

	//エディットの初期化
	UninitEdit();

	//プレイヤーの終了処理
	UninitPlayer();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	//メッシュドームの終了処理
	UninitMeshDome();

	//エフェクトの終了処理
	UninitEffect();

	//2Dエフェクトの終了処理
	UninitEffect_2D();

	//パーティクルの終了処理
	UninitParticle();

	//テクスチャの破棄
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRankingText) / sizeof(*c_apFilenameRankingText); nCntTex++)
	{
		if (g_pTextureRankingText[nCntTex] != NULL)
		{
			g_pTextureRankingText[nCntTex]->Release();
			g_pTextureRankingText[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRankingText != NULL)
	{
		g_pVtxBuffRankingText->Release();
		g_pVtxBuffRankingText = NULL;
	}

}

//==============================================================
//ランキングの更新処理
//==============================================================
void UpdateRanking(void)
{
	//セットするまでのカウンター加算
	g_Ranking.nCntSetCounter++;

	if (SET_TIME <= g_Ranking.nCntSetCounter)
	{//規定値を超えたら

		//ランキングを下から設定
		SetRankingPos();
	}

	if (g_Ranking.nOldMode == MODE_RESULT && g_Ranking.bNewRecord == true &&
		SET_TIME < g_Ranking.nCntSetCounter && g_Ranking.nCntUse >= MIN_RANKING)
	{//遷移前がリザルト && ニューレコードの時

		//ニューレコード時の更新処理
		UpdateNewRecordRanking();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0) || CHANGE_TIME <= g_Ranking.nChangeTime)
	{//Enterが押された

		if (g_Ranking.nCntUse >= MIN_RANKING)
		{//全部出ていたら

			SetFade(MODE_TITLE);
		}
		else
		{
			SetProvisionRanking();
		}
	}

	for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
	{
		//数字のテクスチャ設定
		SetRanking(nCntRank);
	}
}

//==============================================================
//ランキングの規定値に設定処理
//==============================================================
void SetProvisionRanking(void)
{
	for (int nCntRank = 0; nCntRank < MIN_RANKING - g_Ranking.nChangeTime; nCntRank++)
	{
		//設定処理
		SetRankingPos();

		//位置補正
		g_Ranking.aEachRealRanking[nCntRank].pos.y = g_Ranking.aEachRealRanking[nCntRank].posDest.y;

		//長さ補正
		g_Ranking.aEachRealRanking[nCntRank].fWidth = WIDTH;
		g_Ranking.aEachRealRanking[nCntRank].fHeight = HEIGHT;
	}
}

//==============================================================
//ランキングのニューレコード時更新処理
//==============================================================
void UpdateNewRecordRanking(void)
{

	static int nMove = 1;	//移動方向

	//ニューレコードのランキングを拡縮する
	g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth += (WIDTH * 0.02f) * nMove;
	g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fHeight += (HEIGHT * 0.02f) * nMove;

	if (WIDTH * 1.5f < g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth)
	{//規定値を超えたら

		g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth = WIDTH * 1.5f;
		g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fHeight = HEIGHT * 1.5f;
		nMove *= -1;
	}
	else if (WIDTH > g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth)
	{//規定値を下回ったら

		g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth = WIDTH;
		g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fHeight = HEIGHT;
		nMove *= -1;
	}

}

//==============================================================
//ランキングの描画処理
//==============================================================
void DrawRanking(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{
		//モデルの描画処理
		DrawModel(0);

		//エディットの初期化
		DrawEdit();

		//プレイヤーの描画処理
		DrawPlayer();

		//メッシュフィールドの描画処理
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュドームの描画処理
		DrawMeshDome();

		//影の描画処理
		DrawShadow();

		//エフェクトの描画処理
		DrawEffect();

		//パーティクルの描画処理
		DrawParticle();
	}
	else if (nType == DRAWTYPE_UI)
	{
		//2Dエフェクトの描画処理
		DrawEffect_2D();

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffRankingText, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureRankingText[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
		{
			if (g_Ranking.aEachRealRanking[nCntRank].bUse == true)
			{//使用されていたら

				for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
				{
					if (g_Ranking.aEachRealRanking[nCntRank].nDigit > nCntScore)
					{//ケタ分表示

						//テクスチャの設定
						pDevice->SetTexture(0, g_pTextureRanking);

						//ポリゴンの描画
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntScore * 4) + (nCntRank * NUM_PLACE * 4), 2);
					}
				}
			}
		}
	}
}

//==============================================================
//ランキングのテクスチャ設定処理
//==============================================================
void SetRanking(int nCntRank)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntRank * NUM_PLACE;

	int nCount = 0, nValue = g_Ranking.nScore[nCntRank];
	do
	{//0になるまでカウント進める

		nValue /= 10;
		nCount++;

	} while (nValue > 0);

	//ケタ割り出し
	g_Ranking.aEachRealRanking[nCntRank].nDigit = nCount;

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

		aTexU[nCntTex] = g_Ranking.nScore[nCntRank] % nDigit / (nDigit / 10);
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
	g_pVtxBuffRanking->Unlock();
}

//==============================================================
//ランキングの設定処理
//==============================================================
void SetRankingPos(void)
{
	for (int nCntRank = MIN_RANKING - 1; nCntRank >= 0; nCntRank--)
	{
		if (g_Ranking.aEachRealRanking[nCntRank].bUse == false)
		{//使用されていなかったら

			g_Ranking.aEachRealRanking[nCntRank].pos = D3DXVECTOR3(START_X, 100.0f + DIS_Y * nCntRank, 0.0f);
			g_Ranking.aEachRealRanking[nCntRank].fWidth = WIDTH * 10.0f;
			g_Ranking.aEachRealRanking[nCntRank].fHeight = HEIGHT * 10.0f;
			g_Ranking.aEachRealRanking[nCntRank].bUse = true;

			//セットする間隔リセット
			g_Ranking.nCntSetCounter = 0;

			//使用してるカウント加算
			g_Ranking.nCntUse++;

			for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
			{//桁数分位置設定

				g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos =
					D3DXVECTOR3(
						g_Ranking.aEachRealRanking[nCntRank].pos.x + (nCntScore * DIS_X),
						g_Ranking.aEachRealRanking[nCntRank].pos.y,
						0.0f);
			}

			//SEをセット
			PlaySound(SOUND_LABEL_SE_IDOU);

			break;
		}
	}
}

//==============================================================
//ロード処理
//==============================================================
void LoadDataRanking(void)
{
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(FILE_RANK, "rb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//ファイルに数値を読み込む
		fread(&g_Ranking.nNumRanking, sizeof(int), 1, pFile);
		fread(&g_Ranking.nScore[0], sizeof(int), g_Ranking.nNumRanking, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合

		g_Ranking.nNumRanking = MIN_RANKING;
		for (int nCount = 0; nCount < g_Ranking.nNumRanking; nCount++)
		{//全部ゼロ

			g_Ranking.nScore[nCount] = 200 - (nCount * 10);
		}

		SaveDataRanking();

	}
}

//==============================================================
//計算処理
//==============================================================
void CalRanking(void)
{

	//ランキングの数加算
	g_Ranking.nNumRanking++;

	//降順処理
	for (int nCntData = 0; nCntData < g_Ranking.nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < g_Ranking.nNumRanking; nCntSort++)
		{
			if (g_Ranking.nScore[nCntData] < g_Ranking.nScore[nCntSort])
			{//要素1より要素2が大きかったら

				int nTemp = g_Ranking.nScore[nCntData];
				g_Ranking.nScore[nCntData] = g_Ranking.nScore[nCntSort];
				g_Ranking.nScore[nCntSort] = nTemp;

			}
		}
	}

	//今回の数を取得
	int NowData = 0;

	//ランキング最大値よりも今回のが小さかったら
	if (g_Ranking.nScore[g_Ranking.nNumRanking - 1] < NowData)
	{//入れ替え

		g_Ranking.nScore[g_Ranking.nNumRanking - 1] = NowData;	//最小値と今回のデータ入れ替え
	}

	if (g_Ranking.nScore[MIN_RANKING - 1] < NowData)
	{//入れ替え

		g_Ranking.nNewRecordNum = MIN_RANKING - 1;			//ニューレコードの要素番号
		g_Ranking.bNewRecord = true;	//フラグON
	}

	//降順処理
	for (int nCntData = 0; nCntData < g_Ranking.nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < g_Ranking.nNumRanking; nCntSort++)
		{
			if (g_Ranking.nScore[nCntData] < g_Ranking.nScore[nCntSort])
			{//要素1より要素2が大きかったら

				int nTemp = g_Ranking.nScore[nCntData];
				g_Ranking.nScore[nCntData] = g_Ranking.nScore[nCntSort];
				g_Ranking.nScore[nCntSort] = nTemp;
			}
		}
	}

	if (g_Ranking.bNewRecord == true)
	{//ニューレコードの時

		for (int nCount = MIN_RANKING; nCount > 0; nCount--)
		{//要素番号記憶して抜ける

			if (g_Ranking.nScore[nCount - 1] == g_Ranking.nThisScore)
			{
				g_Ranking.nNewRecordNum = nCount - 1;
				break;
			}
		}
	}

}

//==============================================================
//セーブ処理
//==============================================================
void SaveDataRanking(void)
{
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(FILE_RANK, "wb");

	if (pFile != NULL)
	{//ファイルが開けた場合

		//ファイルに数値を書き出す
		fwrite(&g_Ranking.nNumRanking, sizeof(int), 1, pFile);
		fwrite(&g_Ranking.nScore[0], sizeof(int), g_Ranking.nNumRanking, pFile);

		//ファイルを閉じる
		fclose(pFile);
	}
}

//==============================================================
//ランキングの頂点バッファ生成
//==============================================================
void CreatRankingBuff(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_05.png",
		&g_pTextureRanking);


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE * MIN_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	int aTexU[NUM_PLACE];

	for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
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

			aTexU[nCntTex] = g_Ranking.aEachRealRanking[nCntRank].nScore % nDigit / (nDigit / 10);
			nDigit /= 10;
		}

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x - g_Ranking.aEachRealRanking[nCntRank].fWidth, g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y - g_Ranking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x + g_Ranking.aEachRealRanking[nCntRank].fWidth, g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y - g_Ranking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x - g_Ranking.aEachRealRanking[nCntRank].fWidth, g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y + g_Ranking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x + g_Ranking.aEachRealRanking[nCntRank].fWidth, g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y + g_Ranking.aEachRealRanking[nCntRank].fHeight, 0.0f);

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
	g_pVtxBuffRanking->Unlock();
}

//==============================================================
//ランキング(テキスト)の頂点バッファ生成
//==============================================================
void CreatRankingText(void)
{
	//デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRankingText) / sizeof(*c_apFilenameRankingText); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameRankingText[nCntTex],
			&g_pTextureRankingText[nCntTex]);
	}

	//各種変数初期化
	ZeroMemory(&g_aRankingText, sizeof(RankingText));
	ZeroMemory(&g_aRankingText.aUI, sizeof(UI_2D));
	g_aRankingText.aUI.pos = D3DXVECTOR3(START_TEXT_X, 360.0f, 0.0f);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingText,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingText->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aRankingText.aUI.pos.x - TEXT_WIDTH, g_aRankingText.aUI.pos.y - TEXT_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aRankingText.aUI.pos.x + TEXT_WIDTH, g_aRankingText.aUI.pos.y - TEXT_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aRankingText.aUI.pos.x - TEXT_WIDTH, g_aRankingText.aUI.pos.y + TEXT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aRankingText.aUI.pos.x + TEXT_WIDTH, g_aRankingText.aUI.pos.y + TEXT_HEIGHT, 0.0f);

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
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffRankingText->Unlock();
}

//==============================================================
//ランキングの情報取得
//==============================================================
Ranking *GetRanking(void)
{
	return &g_Ranking;
}