//==============================================================
//
//DirectX[nofade.h]
//Author:大原怜将
//
//==============================================================
#ifndef _NOFADE_H_				//このマクロ定義がされていなかったら
#define _NOFADE_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//フェードの状態
typedef enum
{
	NOFADE_OUT = 0,			//フェードアウト状態
	NOFADE_IN,			//フェードイン状態
	NOFADE_MAX
} NOFADESTATE;

//フェードしないフェードの構造体
typedef struct
{
	NOFADESTATE nState;
	D3DXCOLOR colorNoFade;		//ポリゴン（フェード）の色
	int nCounter;               //フェードに入るまでの時間
	bool bUse;
} NOFADE;

//プロトタイプ宣言
void InitNoFade(void);
void UninitNoFade(void);
void UpdateNoFade(void);
void DrawNoFade(void);
void SetNoFade(int nCounter);
NOFADE GetNoFade(void);

#endif
