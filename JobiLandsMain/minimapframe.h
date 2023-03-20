//==============================================================
//
//ミニマップのフレーム[minimapframe.h]
//Author:大原怜将
//
//==============================================================
#ifndef _MINIMAPFRAME_H_				//このマクロ定義がされていなかったら
#define _MINIMAPFRAME_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//人
//typedef enum
//{
//	RADARTYPE_MANYPEOPLE = 0,	//大量発生中
//	RADARTYPE_FEWPEOPLE,		//大量発生なし
//	RADARTYPE_MAX
//}RADARTYPE;

//ミニマップフレームの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	float fWidth;			//横幅
	float fHeight;			//縦幅
	bool bUse;              //使用しているかどうか
}Minimapframe;

//プロトタイプ宣言
void InitMinimapframe(void);
void UninitMinimapframe(void);
void UpdateMinimapframe(void);
void DrawMinimapframe(void);
Minimapframe *GetMinimapframe(void);

#endif
