//==============================================================
//
//表示の背景[ground.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _GROUND_H_				//このマクロ定義がされていなかったら
#define _GROUND_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//人数の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	float fHeight;			//縦幅
	float fWidth;			//横幅
	bool bUse;				//使用しているか
}Ground;

//プロトタイプ宣言
void InitGround(void);
void UninitGround(void);
void UpdateGround(void);
void DrawGround(void);

#endif
