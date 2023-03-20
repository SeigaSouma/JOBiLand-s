//=============================================================================
//
// エフェクト処理 [2D_effect.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _EFFECT_2D_H_		//このマクロ定義がされていなかったら
#define _EFFECT_2D_H_		//二重インクルード防止のマクロを定義する

//インクルード
#include "main.h"

//マクロ定義
#define EFFECT_2D_LIFE		(30)

//エフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXCOLOR col;		//色
	float fRadius;		//半径
	float fMaxRadius;	//最大半径
	int nLife;			//寿命
	int nMaxLife;		//最大寿命(固定)
	int moveType;		//移動の種類
	int nType;			//種類
	bool bUse;			//使用しているかどうか
	bool bAddAlpha;		//加算合成の判定
}Effect_2D;

//プロトタイプ宣言
void InitEffect_2D(void);
void UninitEffect_2D(void);
void UpdateEffect_2D(void);
void DrawEffect_2D(void);
void SetEffect_2D(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType);
int GetEffect_2DNum(void);
bool GetEffect_2DUse(void);

#endif