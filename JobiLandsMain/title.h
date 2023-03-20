//=============================================================================
//
// タイトル画面処理 [title.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _TITLE_H_		//このマクロ定義がされていなかったら
#define _TITLE_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//列挙型定義
typedef enum
{
	TITLESTATE_NONE = 0,	//なにもしてない状態
	TITLESTATE_TKTK,		//チカチカ状態
	TITLESTATE_FADE,		//フェード状態
	TITLESTATE_MAX

}TITLESTATE;

typedef enum
{
	TITLELOG_BLACK = 0,	//黒幕
	TITLELOG_LOG,		//ロゴ
	TITLELOG_LINE,		//線
	TITLELOG_LIBELLION,	//英語
	TITLELOG_HONKI,		//本気出す
	TITLELOG_TITLELOGO,	//タイトルロゴ全部
	TITLELOG_MAX
}TITLELOG;

typedef enum
{
	TITLESELECT_START = 0,	//スタート
	TITLESELECT_END,		//終了
	TITLESELECT_MAX
}TITLESELECT;

//タイトル構造体
typedef struct
{
	int OldSelect;	//前回の選択肢
	int nSelect;	//現在の選択肢
	int nCntChange;	//切り替わる時間
	int nState;	//状態
}Title;

//プロトタイプ宣言
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(int nType);
void UpdateSelectTitle(void);
void UpdateTitleTKTK(void);
void UpdateSelectTitleCol(int Select);
void UpdateOptionTitle(int nType);
bool GetEndFrag(void);
Title *GetTitle(void);

#endif