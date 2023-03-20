//==========================================================================
//
//イベントサイン処理
//Author:日野澤匠泉	[iventsign.h]
//
//==========================================================================
#ifndef _IVENTSIGN_H_
#define _IVENTSIGN_H_

#include"main.h"

//イベントサインの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 Iventpos;	//イベント位置
	D3DXVECTOR3 rot;	//向き
	D3DXVECTOR3 Dest;	//目標の移動方向
	bool bUse;			//使用しているかどうか
}IventSign;

//プロトタイプ宣言
void InitIventSign(void);
void UninitIventSign(void);
void UpdateIventSign(void);
void DrawIventSign(void);
IventSign *GetIventSign(void);
void SetIventSign(D3DXVECTOR3 pos);
#endif

