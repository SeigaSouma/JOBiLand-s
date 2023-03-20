//=============================================================================
//
// 最初の会話処理 [start_talk.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _START_TALK_H_		//このマクロ定義がされていなかったら
#define _START_TALK_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//列挙型定義
typedef enum
{
	START_TALK_VTX_FADE = 0,
	START_TALK_VTX_TEXT1,
	START_TALK_VTX_TEXT2,
	START_TALK_VTX_TEXT3,
	START_TALK_VTX_TEXT4,
	START_TALK_VTX_TEXT5,
	START_TALK_VTX_SKIP,
	START_TALK_VTX_MAX,
}START_TALK_VTX;

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	bool bUse;	//使用しているか
}StartTalk;

//プロトタイプ宣言
void InitStartTalk(void);
void UninitStartTalk(void);
void UpdateStartTalk(void);
void DrawStartTalk(void);
void SetStartTalk(void);
void SetTartgetWindow(void);
StartTalk *GetStartTalk(void);

#endif