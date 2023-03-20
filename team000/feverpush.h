//=============================================================================
//
// フィーバープッシュ処理 [feverpush.h]
// Author : 佐藤根詩音
//
//=============================================================================
#ifndef _FEVERPUSH_H_				//このマクロ定義がされていなかったら
#define _FEVERPUSH_H_				//2重インクルード防止のマクロを定義する

#include "main.h"

//フィーバープッシュの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	float fWidth;			//横幅
	float fHeight;			//縦幅
	float fWidthMove;		//横幅
	float fHeightMove;		//縦幅
	bool bUse;				//使用してるか
} FeverPush;

//プロトタイプ宣言
void InitFeverPush(void);
void UninitFeverPush(void);
void UpdateFeverPush(void);
void DrawFeverPush(void);
FeverPush *GetFeverPush(void);

#endif
