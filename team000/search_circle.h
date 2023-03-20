//=============================================================================
//
// 探索範囲処理 [search_circle.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _SEARCH_CIRCLE_H_		//このマクロ定義がされていなかったら
#define _SEARCH_CIRCLE_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_SEARCH_CIRCLE	(256)

//列挙型定義
typedef enum
{
	SEARCH_CIRCLE_TYPE_BLACK = 0,
	SEARCH_CIRCLE_TYPE_BLACK2,
	SEARCH_CIRCLE_TYPE_ORANGE,
	SEARCH_CIRCLE_TYPE_ORANGE2,
	SEARCH_CIRCLE_TYPE_BLUE,
	SEARCH_CIRCLE_TYPE_MAX
}SEARCH_CIRCLE_TYPE;

//メッシュフィールド構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 nor[MAX_SEARCH_CIRCLE];	//法線の向き
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
}SEARCH_CIRCLE;

//プロトタイプ宣言
void InitSearchCircle(void);
void UninitSearchCircle(void);
void UpdateSearchCircle(void);
void DrawSearchCircle(void);
int SetSearchCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fMove, int nTexType);
void SetPositionSearchCircle(int nIdxSearchRadius, D3DXVECTOR3 pos);

#endif