//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,	// タイトル
	SOUND_LABEL_BGM_GAME,		// ゲーム
	SOUND_LABEL_BGM_RESULT,		// リザルト
	SOUND_LABEL_BGM_RANKING,	// ランキング
	SOUND_LABEL_BGM_TUTORIAL,	// チュートリアル
	SOUND_LABEL_SE_CURSOR,		// カーソル移動
	SOUND_LABEL_SE_ARMMOVE,		//ロボットの腕が動く音
	SOUND_LABEL_SE_HIT,			//ロボットの腕が当たる音
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void VolumeChange(float fVolume);
int GetVolume(void);

#endif
