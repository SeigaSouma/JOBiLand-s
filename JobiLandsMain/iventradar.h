//==============================================================
//
//イベントサインの下地[iventradar.h]
//Author:大原怜将
//
//==============================================================
#ifndef _IVENTRADAR_H_				//このマクロ定義がされていなかったら
#define _IVENTRADAR_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//人
typedef enum
{
	RADARTYPE_MANYPEOPLE = 0,	//大量発生中
	RADARTYPE_FEWPEOPLE,		//大量発生なし
	RADARTYPE_MAX
}RADARTYPE;

//人数の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	RADARTYPE type;			//種類
	D3DXCOLOR col;          //色
	float fWidth;			//横幅
	float fHeight;			//縦幅
	bool bUse;				//使用しているか
}IventRadar;

//プロトタイプ宣言
void InitIventRadar(void);
void UninitIventRadar(void);
void UpdateIventRadar(void);
void DrawIventRadar(void);
void SetIventRadar(void);
IventRadar *GetIventRadar(void);

#endif
