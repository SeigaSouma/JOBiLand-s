//=============================================================================
//
// タイトルロゴ処理 [titlelogo.h]
// Author : 小原立暉
//
//=============================================================================
#ifndef _TITLELOGO_H_		//このマクロ定義がされていなかったら
#define _TITLELOGO_H_		//二重インクルード防止のマクロを定義する

#include "main.h"
#include "model.h"

//=======================================
// マクロ定義
//=======================================

//=======================================
// 列挙型定義(TITLE_LOGO_STATE)
//=======================================
typedef enum
{
	TITLE_LOGO_STATE_APPEAR = 0,		// 出現状態
	TITLE_LOGO_STATE_NONE,				// 通常状態
	TITLE_LOGO_STATE_MAX				// この列挙型の総数
}TITLE_LOGO_STATE;

//=======================================
// 構造体定義(TitleLogo)
//=======================================
typedef struct
{
	Model		aModel;		// モデルの構造体
}TitleLogo;

//=======================================
//プロトタイプ宣言
//=======================================
void InitTitleLogo(void);
void UninitTitleLogo(void);
void UpdateTitleLogo(void);
void DrawTitleLogo(void);

#endif