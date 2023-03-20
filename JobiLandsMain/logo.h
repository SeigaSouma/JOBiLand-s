//=============================================================================
//
// タイトル画面処理 [logo.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _LOGO_H_		//このマクロ定義がされていなかったら
#define _LOGO_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//列挙型定義
typedef enum
{
	LOGOSTATE_NONE = 0,	//なにもしてない状態
	LOGOSTATE_MOVELOG,		//ロゴ動き状態
	LOGOSTATE_TKTK,		//チカチカ状態
	LOGOSTATE_FADE,		//フェード状態
	LOGOSTATE_MAX

}LOGOSTATE;

typedef enum
{
	LOGOVTX_WHITE = 0,	//幕
	LOGOVTX_LOG,		//ロゴ
	LOGOVTX_MAX
}LOGOVTX;

//タイトル構造体
typedef struct
{
	int nCntChange;	//切り替わる時間
	D3DXCOLOR col;	//色
	D3DXVECTOR3 pos;	//位置
}Logo;

//プロトタイプ宣言
void InitLogo(void);
void UninitLogo(void);
void UpdateLogo(void);
void DrawLogo(int nType);
Logo *GetLogo(void);

#endif