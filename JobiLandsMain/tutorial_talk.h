//=============================================================================
//
// チュートリアル吹き出し処理 [tutorial_talk.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _TUTORIAL_TALK_H_		//このマクロ定義がされていなかったら
#define _TUTORIAL_TALK_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//列挙型定義
typedef enum
{
	TUTORIAL_TALK_VTX_FADE = 0,
	TUTORIAL_TALK_VTX_TEXT,
	TUTORIAL_TALK_VTX_MAX,
}TUTORIAL_TALK_VTX;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	int nStep;		//進行度
	int nCntSet;	//設定のカウント
	int nCntEnter;	//Enter押すまでのクールタイム
	bool bUse;		//使用しているか
	bool bSet;		//設定しているか
}TutorialTalk;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	bool bUse;			//使用しているか
}TutorialOK;

//プロトタイプ宣言
void InitTutorialTalk(void);
void UninitTutorialTalk(void);
void UpdateTutorialTalk(void);
void DrawTutorialTalk(void);
void SetTutorialTalk(void);
void SetTutorialOK(void);
TutorialTalk *GetTutorialTalk(void);

#endif