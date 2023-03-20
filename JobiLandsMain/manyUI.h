//=============================================================================
//
// 大量発生のUI処理 [manyUI.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MANY_UI_H_
#define _MANY_UI_H_

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
}MANY_UI;

//プロトタイプ宣言
void InitManyUI(void);
void UninitManyUI(void);
void UpdateManyUI(void);
void DrawManyUI(void);
void SetManyUI(int nType);
MANY_UI *GetManyUI(void);



#endif
