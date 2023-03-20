//<====================================================
//
//イベントウィンドウ処理(EventWindow.h)
//Author kazuki watanabe
//
//<====================================================
#ifndef  _EVENTWINDOW_H_
#define _EVENTWINDOW_H_

#include "main.h"

#define NUM_E_WINDOW (1)	//イベントウィンドウの数

//イベントウィンドウの列挙型
typedef enum
{
	EVENTWINDOW_VTX_RED = 0,	//赤幕
	EVENTWINDOW_VTX_FRAM,		//警告の枠
	EVENTWINDOW_VTX_TEXT,		//警告のテキスト
	EVENTWINDOW_VTX_LINEUP,		//警告のライン上
	EVENTWINDOW_VTX_LINEDW,		//警告のライン下
	EVENTWINDOW_VTX_MAX
}EVENTWINDOW_VTX;

typedef enum
{
	EVENTWINDOW_STEP_RED = 0,	//赤幕ウィンウィン
	EVENTWINDOW_STEP_FRAMMOVE,	//枠移動
	EVENTWINDOW_STEP_TEXTLINE,	//テキスト&ライン移動
	EVENTWINDOW_STEP_OUT,		//退場
}EVENTWINDOW_STEP;


//イベントウィンドウの構造体
typedef struct
{
	bool bUse;		//使用しているか
	int nStep;		//ステップ
	UI_2D aUI[EVENTWINDOW_VTX_MAX];	//UI情報
}EVENTWINDOW;

//プロトタイプ宣言
void InitEventWindow(void);
void UninitEventWindow(void);
void UpdateEventWindow(void);
void DrawEventWindow(void);
void SetEventWindow(void);
EVENTWINDOW *GetEventWindow(void);



#endif
