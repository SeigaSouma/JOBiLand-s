//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MESHFIELD_H_		//このマクロ定義がされていなかったら
#define _MESHFIELD_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_FIELD	(256)	//地面の最大数

//メッシュフィールドのタイプ
typedef enum
{
	DRAWFIELD_TYPE_MAIN = 0,
	DRAWFIELD_TYPE_MAP,
	DRAWFIELD_TYPE_MAX
}DRAWFIELD_TYPE;

//メッシュフィールド構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nNumIndex;			//インデックス数
	int nNumVertex;			//頂点数
	int nWidth;				//横分割数
	int nHeight;			//縦分割数
	float fWidthLen;		//横の長さ
	float fHeightLen;		//縦の長さ
	bool bUse;				//使用しているか
	int nType;				//種類
}MESHFIELD;

//プロトタイプ宣言
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(DRAWFIELD_TYPE nType);
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height, float fWidthLen, float fHeightLen, int nType);

#endif