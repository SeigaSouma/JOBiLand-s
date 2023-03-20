//=============================================================================
//
// メッシュシリンダー処理 [manypoint.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MANYPOINT_H_		//このマクロ定義がされていなかったら
#define _MANYPOINT_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define POS_MANYPOINT	(200.0f)

//メッシュシリンダー構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nNumIndex;			//インデックス数
	int nNumVertex;			//頂点数
	bool bUse;				//使用しているか
}MANYPOINT;

//プロトタイプ宣言
void InitManyPoint(void);
void UninitManyPoint(void);
void UpdateManyPoint(void);
void DrawManyPoint(void);
MANYPOINT *GetManyPoint(void);
#endif