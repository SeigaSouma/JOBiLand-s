//=============================================================================
//
// 終了5秒前カウント処理 [endcountdown.h]
// Author : 大原怜将
//
//=============================================================================
#ifndef _ENDCOUNTDOWN_H_
#define _ENDCOUNTDOWN_H_

#include "main.h"

//大量発生のUI構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	int nType;			//種類
	float fWidth;		//幅
	float fHeight;		//高さ
	int nCntZoom;		//大きくなる時間
	bool bUse;			//使用しているか
} ENDCOUNTDOWN;

//プロトタイプ宣言
void InitEndCountDown(void);
void UninitEndCountDown(void);
void UpdateEndCountDown(void);
void DrawEndCountDown(void);
void SetEndCountDown(int nType);
ENDCOUNTDOWN *GetEndCountDown(void);



#endif
