//=============================================================================
//
// リアルタイムランキング処理 [realtime_ranking.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _REALTIME_RANKING_H_				//このマクロ定義がされていなかったら
#define _REALTIME_RANKING_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//ランキング構造体

typedef enum
{
	REALRANKING_TYPE_RANKNUM = 0,	//現在の順位
	REALRANKING_TYPE_RANKING,	//次のランキングスコア
	REALRANKING_TYPE_MAX
}REALRANKING_TYPE;

typedef enum
{
	REALRANKINGTEXT_VTX_FADE = 0,	//下地
	REALRANKINGTEXT_VTX_NEXTNUM,	//次の順位の人数
	REALRANKINGTEXT_VTX_NOW,		//現在の順位&現在の人数
	REALRANKINGTEXT_VTX_TOP,		//記録更新中
	REALRANKINGTEXT_VTX_GIZA,		//ギザギザ
	REALRANKINGTEXT_VTX_MAX
}REALRANKINGTEXT_VTX;

//数字1つ1つ
typedef struct
{
	D3DXVECTOR3 pos;	//位置
}EachScoreNumber;

//ランキング1つ1つ
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nScore;		//現在のスコア
	int nDigit;		//桁数
	float fWidth;
	float fHeight;

	EachScoreNumber aScoreNum[4];
}EachRealRanking;

typedef struct
{
	int nNumRanking;	//ランキングの数
	int nScore[512];	//全てのランキングデータ
	int nNowRank;		//現在の順位
	int nNowScore;		//現在のスコア
	bool bTop;			//1位かどうか

	EachRealRanking aEachRealRanking[REALRANKING_TYPE_MAX];	//それぞれのスコア
} RealTimeRanking;


typedef struct
{
	UI_2D aUI;	//UI情報
} RealTimeText;

//プロトタイプ宣言
void InitRealTimeRanking(void);
void UninitRealTimeRanking(void);
void UpdateRealTimeRanking(void);
void DrawRealTimeRanking(void);

#endif