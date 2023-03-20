//=============================================================================
//
// 上下の枠処理 [updown_fram.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _UPDOWN_FRAM_H_
#define _UPDOWN_FRAM_H_

#include "main.h"

//列挙型定義
typedef enum
{
	FRAMTYPE_UP = 0,
	FRAMTYPE_DOWN,
	FRAMTYPE_MAX
}FRAMTYPE;

typedef enum
{
	UPDOWN_FRAM_STATE_IN = 0,
	UPDOWN_FRAM_STATE_OUT,
	UPDOWN_FRAM_STATE_MAX
}UPDOWN_FRAM_STATE;

//上下の枠構造体
typedef struct
{
	D3DXVECTOR3 pos[2];	//位置
	D3DXCOLOR col;		//色
	int nState;			//状態
	bool bUse;			//使用しているか
	bool bEnd;			//到着しているか
}UPDOWN_FRAM;

//プロトタイプ宣言
void InitUpDownFram(void);
void UninitUpDownFram(void);
void UpdateUpDownFram(void);
void DrawUpDownFram(void);
void SetUpDownFram(void);
UPDOWN_FRAM *GetUpDownFram(void);



#endif
