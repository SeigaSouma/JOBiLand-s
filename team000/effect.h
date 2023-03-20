//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _EFFECT_H_		//このマクロ定義がされていなかったら
#define _EFFECT_H_		//二重インクルード防止のマクロを定義する

//インクルード
#include "main.h"

//マクロ定義
#define EFFECT_LIFE		(30)

//列挙型定義
typedef enum
{
	MOVEEFFECT_ADD = 0,	//加算
	MOVEEFFECT_SUB,		//減算
	MOVEEFFECT_SUPERSUB,	//超減算
	MOVEEFFECT_NONE,	//変化なし
	MOVEEFFECT_MAX
}MOVEEFFECT;

typedef enum
{
	EFFECTTYPE_NORMAL = 0,	//通常エフェクト
	EFFECTTYPE_SMOKE,		//煙エフェクト
	EFFECTTYPE_SMOKEBLACK,	//黒煙
	EFFECTTYPE_BLACK,		//黒エフェクト
	EFFECTTYPE_HART,        //ハートエフェクト
	EFFECTTYPE_JUJI,		//十字エフェクト
	EFFECTTYPE_THUNDER,		//雷エフェクト
	EFFECTTYPE_THUNDER2,	//雷エフェクト
	EFFECTTYPE_MAX
}EFFECTTYPE;

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
	float fAngle;		//角度
	float fLength;		//対角線の長さ

	bool bUse;			//使用しているかどうか
	bool bAddAlpha;		//加算合成の判定
}Effect;

//プロトタイプ宣言
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType);
int GetEffectNum(void);
bool GetEffectUse(void);

#endif