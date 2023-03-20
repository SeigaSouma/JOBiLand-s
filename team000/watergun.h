//=============================================================================
//
// エフェクト処理 [watergun.h]
// Author : 大原怜将
//
//=============================================================================

#ifndef _WATERGUN_H_		//このマクロ定義がされていなかったら
#define _WATERGUN_H_		//二重インクルード防止のマクロを定義する

//インクルード
#include "main.h"

//マクロ定義
#define WATERBULLET_LIFE		(30)

//列挙型定義
typedef enum
{
	WATERBULLETTYPE_NORMAL = 0,	//通常エフェクト
	WATERBULLETTYPE_SMOKE,		//煙エフェクト
	WATERBULLETTYPE_SMOKEBLACK,	//黒煙
	WATERBULLETTYPE_BLACK,		//黒エフェクト
	WATERBULLETTYPE_HART,        //ハートエフェクト
	WATERBULLETTYPE_MAX
}WATERBULLETTYPE;

//エフェクト構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXCOLOR col;		//色
	float fRadius;		//半径
	float fMaxRadius;	//最大半径
	float fHeight;      //高さ
	float fWidth;       //幅
	int nLife;			//寿命
	int nMaxLife;		//最大寿命(固定)
	int moveType;		//移動の種類
	int nType;			//種類
	bool bUse;			//使用しているかどうか
	bool bAddAlpha;		//加算合成の判定
}Watergun;

//プロトタイプ宣言
void InitWatergun(void);
void UninitWatergun(void);
void UpdateWatergun(void);
void DrawWatergun(void);
void SetWatergun(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType);
int GetWatergunNum(void);
bool GetWatergunUse(void);

#endif
