//=============================================================================
//
// 終了 [finish.h]
// Author : 大原怜将
//
//=============================================================================
#ifndef _FENISH_H_
#define _FENISH_H_

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
} FENISH;

//プロトタイプ宣言
void InitFinish(void);
void UninitFinish(void);
void UpdateFinish(void);
void DrawFinish(void);
void SetFinish(int nType);
FENISH *GetFinish(void);



#endif
