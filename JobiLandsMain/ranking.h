//==============================================================
//
//DirectX[ranking.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _RANKING_H_				//このマクロ定義がされていなかったら
#define _RANKING_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//数字1つ1つ
typedef struct
{
	D3DXVECTOR3 pos;	//位置
}EachRankingNumber;

//ランキング1つ1つ
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posDest;	//目標の位置
	int nScore;		//現在のスコア
	int nDigit;		//桁数
	float fWidth;
	float fHeight;
	bool bUse;		//使用しているか

	EachRankingNumber aScoreNum[4];
}EachRanking;

typedef struct
{
	int nNumRanking;	//ランキングの数
	int nScore[512];	//全てのランキングデータ
	int nChangeTime;	//自動遷移のカウンター
	int nCntSetCounter;	//セットするまでのカウンター
	int nCntUse;		//使用カウント
	int nThisScore;		//今回のスコア
	int nNewRecordNum;	//ニューレコードの番号
	bool bNewRecord;	//ニューレコードの判定
	int nOldMode;		//遷移前のモード

	EachRanking aEachRealRanking[5];	//それぞれのスコア
} Ranking;


typedef struct
{
	UI_2D aUI;	//UI情報
} RankingText;

//プロトタイプ宣言
void InitRanking(int nOldMode);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(int nType);
void SetRanking(int nScore);
void SetRankingPos(void);
Ranking *GetRanking(void);

#endif