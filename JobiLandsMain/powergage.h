//==============================================================
//
//DirectX[powergage.h]
//Author:日野澤匠泉
//
//==============================================================
#ifndef _POWERGAGE_H_				//このマクロ定義がされていなかったら
#define _POWERGAGE_H_				//2重インクルード防止のマクロを定義する

#include "main.h"

//勢力ゲージの列挙型
typedef enum
{
	POWERGAGE_VTX_FRAM = 0,
	POWERGAGE_VTX_NEET,
	POWERGAGE_VTX_ENEMY,
	POWERGAGE_VTX_TEX,
	POWERGAGE_VTX_MAX
}POWERGAGE_VTX;

//勢力ゲージの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き
	int population;		//人口
	float result;		//比率計算結果
	bool bUse;			//使用されているかどうか
}PowerGage;

//プロトタイプ宣言
void InitPowerGage(void);
void UninitPowerGage(void);
void UpdatePowerGage(void);
void DrawPowerGage(void);
PowerGage *GetPowerGage(void);

#endif
