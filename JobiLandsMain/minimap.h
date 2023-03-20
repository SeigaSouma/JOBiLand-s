//=============================================================================
//
// ポリゴン処理 [minimap.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MINIMAP_H_		//このマクロ定義がされていなかったら
#define _MINIMAP_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//ポリゴン構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 vtxMax;		//最大サイズ
	D3DXVECTOR3 vtxMin;		//最小サイズ
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用されているかどうか

}MINIMAP;

//プロトタイプ宣言
void InitMinimap(void);
void UninitMinimap(void);
void UpdateMinimap(void);
void DrawMinimap(void);
void SetMiniMap(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

#endif