//=============================================================================
//
// ポーズ処理 [pause.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _PAUSE_H_	//このマクロ定義がされていなかったら
#define _PAUSE_H_	//二重インクルード防止のマクロを定義する

//列挙型定義
//ポーズメニュー
typedef enum
{
	PAUSEMENU_CONTINUE = 0,		//ポーズの解除
	PAUSEMENU_RETRY,			//リトライ
	PAUSEMENU_LEVELSELECT,		//レベルセレクトに戻る	
	PAUSEMENU_MAX
}PAUSEMENU;

typedef enum
{
	PAUSEVTX_FADE = 0,		//黒い幕
	PAUSEVTX_WINDOW,		//メニューウィンドウ
	PAUSEVTX_CONTINUE,		//ゲームに戻る
	PAUSEVTX_RETRY,			//リトライ
	PAUSEVTX_QUIT,			//タイトルに戻る	
	PAUSEVTX_MAX
}PAUSEVTX;

//ポーズの状態
typedef enum
{
	PAUSESTATE_SELECT = 0,	//選べる状態
	PAUSESTATE_FADE,		//フェード中
	PAUSESTATE_MAX
}PAUSESTATE;

//プロトタイプ宣言
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void UpdateSelect(int PauseSelect);
void SetPause(void);

#endif