//=============================================================================
//
// 衝撃波処理 [impactwave.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _IMPACTWAVE_H_		//このマクロ定義がされていなかったら
#define _IMPACTWAVE_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_IMPACTWAVE	(256)

//列挙型定義
typedef enum
{
	INPACTWAVE_TYPE_BLACK = 0,
	INPACTWAVE_TYPE_BLACK2,
	INPACTWAVE_TYPE_ORANGE,
	INPACTWAVE_TYPE_ORANGE2,
	INPACTWAVE_TYPE_BLUE,
	INPACTWAVE_TYPE_PURPLE,
	INPACTWAVE_TYPE_GREEN,
	INPACTWAVE_TYPE_PURPLE2,
	INPACTWAVE_TYPE_PURPLE3,
	INPACTWAVE_TYPE_PINK,
	INPACTWAVE_TYPE_MAX
}INPACTWAVE_TYPE;

//メッシュフィールド構造体
typedef struct
{
	D3DXVECTOR3 posOrigin;		//起点の位置
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 nor[MAX_IMPACTWAVE];	//法線の向き
	D3DXCOLOR col;			//色
	D3DXCOLOR colOrigin;	//元の色
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float fOutWidth;		//横幅(外)
	float fInWidth;			//幅(内)
	float fRotWidth;		//1分割数あたりの角度割合
	float fHeight;
	float fMove;			//広がる速度
	int nLife;				//寿命
	int nMaxLife;			//最大寿命
	int nTexType;			//テクスチャ種類
	bool bUse;				//使用しているか
}IMPACTWAVE;

//プロトタイプ宣言
void InitImpactWave(void);
void UninitImpactWave(void);
void UpdateImpactWave(void);
void DrawImpactWave(void);
int SetImpactWave(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fMove, int nTexType);
void SetPositionImpactWave(int nIdxWave, D3DXVECTOR3 pos);

#endif