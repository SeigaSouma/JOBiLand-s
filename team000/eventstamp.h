//=============================================================================
//
// イベントスタンプ処理 [eventstamp.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _EVENTSTAMP_H_
#define _EVENTSTAMP_H_

#include "main.h"

//マクロ定義
#define MAX_EVENTSTAMP	(2)	//イベントスタンプの最大数

//列挙型定義
typedef enum
{
	EVENTTYPE_MANYMOB = 0,
	EVENTTYPE_MORS,
	EVENTTYPE_MAX
}EVENTTYPE;

//イベントウィンドウの構造体
typedef struct
{
	bool bUse;		//使用しているか
	int nType;		//種類
	UI_2D aUI;	//UI情報
}EVENTSTAMP;

//プロトタイプ宣言
void InitEventStamp(void);
void UninitEventStamp(void);
void UpdateEventStamp(void);
void DrawEventStamp(void);
void SetEventStamp(int nType);
EVENTSTAMP *GetEventStamp(void);



#endif
