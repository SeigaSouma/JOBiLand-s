//==============================================================
//
//表示する人数[numberPeople.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _NUMBERPEOPLE_H_				//このマクロ定義がされていなかったら
#define _NUMBERPEOPLE_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//人
typedef enum
{
	PEOPLESTATE_NEET = 0,	//ニート
	PEOPLESTATE_ENEMY,		//敵
	PEOPLESTATE_MAX
}PEOPLESTATE;

//人数の構造体
typedef struct
{
	D3DXVECTOR3 pos[4];		//位置
	float aDestTexV[4];		//目標のV座標
	float aTexV[4];			//現在のV座標
	int nPeople;			//人数
	bool bUse;				//使用しているか
}People;

//プロトタイプ宣言
void InitPeople(void);
void UninitPeople(void);
void UpdatePeople(void);
void DrawPeople(void);

#endif
