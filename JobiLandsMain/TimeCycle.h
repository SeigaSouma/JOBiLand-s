//<====================================================
//
//タイムサイクル処理(TimeCycle.h)
//Author kazuki watanabe
//
//<====================================================
#ifndef  _TIMECYCLE_H_
#define _TIMECYCLE_H_

#include "main.h"

//タイムサイクルの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	float fAngle;		//角度
	float fLength;		//対角線の長さ

}TIMECYCLE;

//プロトタイプ宣言
void InitTimeCycle(void);
void UninitTimeCycle(void);
void UpdateTimeCycle(void);
void DrawTimeCycle(void);
#endif

