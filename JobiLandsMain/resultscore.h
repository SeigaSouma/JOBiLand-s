//==============================================================
//
//DirectX[resultscore.h]
//Author:日野澤匠泉
//
//==============================================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include"main.h"

//リザルトスコア構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動
	D3DXCOLOR col;			//カラー

	float fHeight;		//高さ
	float fWidth;		//幅

	bool bUse;			//使用されているかどうか
}ResultScore;

//プロトタイプ宣言
void InitResultScore(void);
void UninitResultScore(void);
void UpdateResultScore(void);
void DrawResultScore(void);
void SetResultScore(int nScore);
void AddResultScore(int nValue);
int GetResultScore(void);

#endif
