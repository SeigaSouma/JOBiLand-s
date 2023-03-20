//=============================================================================
//
// リザルト処理 [player.h]
// Author : ひのさわ
//
//=============================================================================
#ifndef _RESULT_H_				//このマクロ定義がされていなかったら
#define _RESULT_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//プロトタイプ宣言
void InitResult(void);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(int nType);

#endif
