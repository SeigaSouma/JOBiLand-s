//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _TUTORIALSKIP_H_	//このマクロ定義がされていなかったら
#define _TUTORIALSKIP_H_	//二重インクルード防止のマクロを定義する

//列挙型定義
//ポーズメニュー
typedef enum
{
	SKIPMENU_SKIP = 0,		//スキップする
	SKIPMENU_TUTORIAL,		//チュートリアルに戻る	
	SKIPMENU_MAX
}SKIPMENU;

typedef enum
{
	SKIPVTX_FADE = 0,		//黒い幕
	SKIPVTX_WINDOW,			//メニューウィンドウ
	SKIPVTX_SKIP,			//スキップする
	SKIPVTX_TUTORIAL,		//チュートリアル続ける	
	SKIPVTX_MAX
}SKIPVTX;

//ポーズの状態
typedef enum
{
	SKIPSTATE_SELECT = 0,	//選べる状態
	SKIPSTATE_FADE,		//フェード中
	SKIPSTATE_MAX
}SKIPSTATE;

//プロトタイプ宣言
void InitTutorialSkip(void);
void UninitTutorialSkip(void);
void UpdateTutorialSkip(void);
void DrawTutorialSkip(void);
void UpdateTutorialSelect(int TutorialSkipSelect);
void SetTutorialSkip(void);

#endif