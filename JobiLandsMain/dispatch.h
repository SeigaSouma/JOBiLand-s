//=============================================================================
//
// 派遣処理 [dispatch.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _DISPATCH_H_		//このマクロ定義がされていなかったら
#define _DISPATCH_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_DISPACTH	(1)	//派遣の最大数

//派遣の列挙型定義
typedef enum
{
	DISPATCH_TYPE_LEFT = 0,
	DISPATCH_TYPE_RIGHT,
	DISPATCH_TYPE_MAX
}DISPATCH_TYPE;

//派遣構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているか
	int nType;				//種類
}DISPATCH;

//プロトタイプ宣言
void InitDisPatch(void);
void UninitDisPatch(void);
void UpdateDisPatch(void);
void UpdateTutorialDisPatch(void);
void DrawDisPatch(void);
void SetRightDisPatch(void);
void SetLeftDisPatch(void);
DISPATCH *GetDispatch(void);

#endif