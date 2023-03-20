//=============================================================================
//
// チュートリアル吹き出し処理 [tutorial_window.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _TUTORIAL_WINDOW_H_		//このマクロ定義がされていなかったら
#define _TUTORIAL_WINDOW_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//列挙型定義
typedef enum
{
	TUTORIAL_WINDOW_VTX_WINDOW = 0,
	TUTORIAL_WINDOW_VTX_TEXT,
	TUTORIAL_WINDOW_VTX_SKIP,
	TUTORIAL_WINDOW_VTX_MAX,
}TUTORIAL_WINDOW_VTX;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	//D3DXVECTOR3 posSkip;	//位置
	int nStep;		//進行度
	int nOldStep;	//進行度
	float fAlpha;	//不透明度
	int nWaitTime;	//待ち時間
	int nIdxMoveLog;	//動くロゴのインデックス番号
	bool bMove;		//動いたかの判定
}TutorialWindow;

//プロトタイプ宣言
void InitTutorialWindow(void);
void UninitTutorialWindow(void);
void UpdateTutorialWindow(void);
void DrawTutorialWindow(void);
TutorialWindow *GetTutorialWindow(void);

#endif