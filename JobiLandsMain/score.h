//=============================================================================
//
// リアルタイムランキング処理 [realtime_ranking.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _SCORE_H_				//このマクロ定義がされていなかったら
#define _SCORE_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//ランキング構造体
//数字1つ1つ
typedef struct
{
	D3DXVECTOR3 pos;	//位置
}EachScore;

typedef struct
{
	int nScore;		//現在のスコア
	int nDigit;		//桁数
	D3DXVECTOR3 pos;	//位置
	float fWidth;
	float fHeight;

	EachScore aScoreNum[4];
} Score;

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);

#endif