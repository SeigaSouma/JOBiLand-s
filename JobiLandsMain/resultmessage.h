//=============================================================================
//
// 結果のメッセージ処理 [resultmessage.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _RESULT_MESSAGE_H_
#define _RESULT_MESSAGE_H_

#include "main.h"

//結果のメッセージの列挙型定義
typedef enum
{
	RESULTTYPE_CLEAR = 0,
	RESULTTYPE_FAILED,
	RESULTTYPE_MAX
}RESULTTYPE;

//結果のメッセージ構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 posOrigin;	//位置
	D3DXCOLOR col;		//色
	int nType;			//種類
	float fWidth;		//幅
	float fHeight;		//高さ
	int nCntZoom;		//大きくなる時間
	bool bUse;			//使用しているか
}RESULT_MESSAGE;

//プロトタイプ宣言
void InitResultMessage(void);
void UninitResultMessage(void);
void UpdateResultMessage(void);
void DrawResultMessage(void);
void SetResultMessage(int nType);
RESULT_MESSAGE *GetResultMessage(void);



#endif
