//==============================================================
//
//DirectX[fade.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _FADE_H_				//このマクロ定義がされていなかったら
#define _FADE_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//フェードの状態
typedef enum
{
	FADE_NONE = 0,		//何もしていない状態
	FADE_IN,			//フェードイン状態
	FADE_OUT,			//フェードアウト状態
	FADE_VOICEOUT,      //ポーズのフェードアウト
	FADE_MAX
} FADE;

typedef enum
{
	FADE_VTX_FADE = 0,
	FADE_VTX_MAX
} FADE_VTX;

typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXCOLOR col;		//色
	FADE nState;		//状態
	float fWidth[FADE_VTX_MAX];		//幅
	float fHeight[FADE_VTX_MAX];		//高さ
	int nCntZoom;		//大きくなる時間
	int nCntVoice;		//ボイスのカウンター
	int nTexType;		//テクスチャの種類
}Fade;

//プロトタイプ宣言
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE modeNext);
void SetVoiceFade(MODE modeNext);
FADE GetFade(void);

#endif
