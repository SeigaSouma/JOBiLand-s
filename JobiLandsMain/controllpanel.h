//=============================================================================
//
// コントロールパネル処理 [controllpanel.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CONTROLLPANEL_H_				//このマクロ定義がされていなかったら
#define _CONTROLLPANEL_H_				//2重インクルード防止のマクロを定義する

#include "main.h"

//コントロールパネルの列挙型
typedef enum
{
	CONTROLLPANEL_VTX_FRAM = 0,
	CONTROLLPANEL_VTX_UPBUTTON,
	CONTROLLPANEL_VTX_DWBUTTON,
	CONTROLLPANEL_VTX_MAX
}CONTROLLPANEL_VTX;

//コントロールパネルの構造体
typedef struct
{
	UI_2D aUI[CONTROLLPANEL_VTX_MAX];	//UI情報
}ControllPanel;

//プロトタイプ宣言
void InitControllPanel(void);
void UninitControllPanel(void);
void UpdateControllPanel(void);
void DrawControllPanel(void);
ControllPanel *GetControllPanel(void);

#endif
