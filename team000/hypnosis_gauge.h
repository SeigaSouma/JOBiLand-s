//=============================================================================
//
// 催眠ゲージ処理 [hypnosis_gauge.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _HYPNISISGAUGE_H_		//このマクロ定義がされていなかったら
#define _HYPNISISGAUGE_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_HYPNOSISGAUGE	(256)	//催眠ゲージの最大数

//列挙型定義
typedef enum
{
	HYPNISISGAUGE_VTX_BLACK = 0,	//黒ゲージ
	HYPNISISGAUGE_VTX_PINK,		//ピンクゲージ
	HYPNISISGAUGE_VTX_FRAM,		//枠
	HYPNISISGAUGE_VTX_MAX
}HYPNISISGAUGE_VTX;


//催眠度ゲージ構造体
typedef struct
{
	D3DXCOLOR col;		//色
	float fWidth;		//横幅
	float fMaxWidth;	//横最大幅
	float fHeight;		//縦幅
	float fWidthDest;	//目標の幅
	float fWidthDiff;	//幅の差分

}HypnosisGauge_Frame;

typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているか
	bool bDisp;				//描画するか
	int nMaxHyp;			//最大値
	int nHypnosis;			//現在地

	HypnosisGauge_Frame aFrame[HYPNISISGAUGE_VTX_MAX];	//枠
}HypnosisGauge;

//プロトタイプ宣言
void InitHypnosisGauge(void);
void UninitHypnosisGauge(void);
void UpdateHypnosisGauge(void);
void DrawHypnosisGauge(void);
int SetHypnosisGauge(float fWidth, float fHeight, int nLife);
void SetPositionHypnosisGauge(int nIdxGauge, D3DXVECTOR3 pos, int nMaxLife);
HypnosisGauge *GetHypnosisGauge(void);
void GetHypnosisUse(int nIdxGauge, bool bUse);

#endif