//=============================================================================
//
// メッシュシリンダー処理 [dispatchpoint.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _DISPATCHPOINT_H_		//このマクロ定義がされていなかったら
#define _DISPATCHPOINT_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//メッシュシリンダー構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nNumIndex;			//インデックス数
	int nNumVertex;			//頂点数
	bool bUse;				//使用しているか
}DISPATCHPOINT;

//プロトタイプ宣言
void InitDispatchPoint(void);
void UninitDispatchPoint(void);
void UpdateDispatchPoint(void);
void DrawDispatchPoint(void);
DISPATCHPOINT *GetDispatchPoint(void);
#endif