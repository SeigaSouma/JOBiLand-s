//==============================================================
//
//DirectX[score.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include"main.h"

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void SetScore(int nScore);
void AddScore(int nValue);
int GetScore(void);

#endif