//=============================================================================
//
// 軌跡処理 [meshorbit.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _ORBIT_H_		//このマクロ定義がされていなかったら
#define _ORBIT_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_ORBIT	(1)	//壁の最大数

//マクロ定義
#define POS_ORBIT_HEIGHT	(250.0f)
#define POS_ORBIT_WIDTH	(0.0f)
#define OFFSET_POINT		(2)
#define ALL_POINT		(OFFSET_POINT * 15)

//メッシュフィールド構造体
typedef struct
{
	D3DXMATRIX *pMtxparent;				//親のマトリックスのポインタ
	D3DXVECTOR3 aOffSet[OFFSET_POINT];	//両端のオフセット
	D3DXCOLOR aCol[OFFSET_POINT];		//両端の基準の色
	D3DXMATRIX amtxWorldPoint[OFFSET_POINT];	//両端のワールドマトリックス
	D3DXVECTOR3 aPosPoint[ALL_POINT];	//計算後の各頂点座標
	D3DXCOLOR aColPoint[ALL_POINT];		//各頂点の色
}ORBIT;

//プロトタイプ宣言
void InitMeshOrbit(void);
void UninitMeshOrbit(void);
void UpdateMeshOrbit(void);
void DrawMeshOrbit(void);
void SetMeshOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height);
ORBIT *GetMeshOrbit(void);
LPDIRECT3DVERTEXBUFFER9 GetBuffMeshOrbit(void);

#endif