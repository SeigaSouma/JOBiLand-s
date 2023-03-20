//=============================================================================
//
// マップの形処理 [mapshape.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MAPSHAPE_H_				//このマクロ定義がされていなかったら
#define _MAPSHAPE_H_				//2重インクルード防止のマクロを定義する

#include "main.h"

//マップの形の列挙型
typedef enum
{
	MAPSHAPE_VTX_FRAM = 0,
	MAPSHAPE_VTX_MAX
}MAPSHAPE_VTX;

//マップの形の構造体
typedef struct
{
	UI_2D aUI[MAPSHAPE_VTX_MAX];	//UI情報
}MapShape;

//プロトタイプ宣言
void InitMapShape(void);
void UninitMapShape(void);
void UpdateMapShape(void);
void DrawMapShape(void);
MapShape *GetMapShape(void);

#endif
