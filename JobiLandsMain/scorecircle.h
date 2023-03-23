//=============================================================================
//
// 衝撃波処理 [scorecircle.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _SCORECIRCLE_H_		//このマクロ定義がされていなかったら
#define _SCORECIRCLE_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_SCORECIRCLE	(3)

//メッシュフィールド構造体
typedef struct
{
	D3DXVECTOR3 posOrigin;		//起点の位置
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 nor[256];	//法線の向き
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
}SCORECIRCLE;

//プロトタイプ宣言
void InitScoreCircle(void);
void UninitScoreCircle(void);
void UpdateScoreCircle(void);
void DrawScoreCircle(void);
void SetScoreCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nTexType);
void SetPositionScoreCircle(int nIdxWave, D3DXVECTOR3 pos);

#endif