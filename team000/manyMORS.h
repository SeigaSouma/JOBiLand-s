//=============================================================================
//
// 大量発生のUI処理 [manyMORS.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MANY_MORS_H_
#define _MANY_MORS_H_

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
}MANY_MORS;

//プロトタイプ宣言
void InitManyMORS(void);
void UninitManyMORS(void);
void UpdateManyMORS(void);
void DrawManyMORS(void);
void SetManyMORS(int nType);
MANY_MORS *GetManyMORS(void);



#endif
