//=============================================================================
//
// ミニマップアイコン [minimap_icon.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MINIMAP_ICON_H_		//このマクロ定義がされていなかったら
#define _MINIMAP_ICON_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//アイコンの列挙型
typedef enum
{
	MINIMAP_ICON_PLAYER,
	MINIMAP_ICON_CHIBI,
	MINIMAP_ICON_KODEBU,
	MINIMAP_ICON_ENEMY,
	MINIMAP_ICON_MOB,
	MINIMAP_ICON_MAX
}MINIMAP_ICON;

//スポーン地点の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nType;				//アイコンのタイプ
	bool bUse;				//使用されているかどうか
}MINIICON;


//プロトタイプ宣言
void InitMiniIcon(void);
void UninitMiniIcon(void);
void UpdateMiniIcon(void);
void DrawMiniIcon(void);
void UpdatePositionIcon(int nIdxIcon, D3DXVECTOR3 pos);
int SetIcon(int nType, float size);
MINIICON *GetMiniIcon(void);

#endif