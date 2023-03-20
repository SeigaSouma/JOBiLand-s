//=============================================================================
//
// フィーバーゲージ処理 [fevergauge.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _FEVERGAUGE_H_				//このマクロ定義がされていなかったら
#define _FEVERGAUGE_H_				//2重インクルード防止のマクロを定義する

#include "main.h"

//勢力ゲージの列挙型
typedef enum
{
	FEVERGAUGE_VTX_FRAM = 0,
	FEVERGAUGE_VTX_TEX,
	FEVERGAUGE_VTX_GAUGE,
	FEVERGAUGE_VTX_MAX
}FEVERGAUGE_VTX;

//勢力ゲージの構造体
typedef struct
{
	bool bFever;		//フィーバーかどうか
	bool bUseButton;	//ボタンを押したか
	UI_2D aUI[FEVERGAUGE_VTX_MAX];	//UI情報
}FeverGauge;

//プロトタイプ宣言
void InitFeverGauge(void);
void UninitFeverGauge(void);
void UpdateFeverGauge(void);
void DrawFeverGauge(void);
FeverGauge *GetFeverGauge(void);

#endif
