//===========================================================================
//
//アニメーション[animation.h]
//Author:日野澤匠泉	
//
//===========================================================================

#ifndef _ANIMATION_H_	//この定義がされていなかったら
#define _ANIMATION_H_	//二重インクルード防止のマクロ定義をする

#include "main.h"

//マクロ定義
#define MAX_TEX	(2)
#define NUM_TEX	(128)			

//壁の構造体定義
typedef struct
{
	D3DXVECTOR3 pos;			//現在の位置
	D3DXVECTOR3 posOld;			//過去の位置
	D3DXVECTOR3 rot;			//向き
	D3DXMATRIX g_mtxWorld;		//ワールドマトリックス
	bool bUse;					//使用しているかどうか
	int nType;					//壁の種類
	int nCountAnim;				//
	int nPatternAnim;			//
	float fWidth;                 //幅
	float fHeight;                //高さ
}Animation;

typedef enum
{
	ANIMATION_000,
	ANIMATION_001,
	ANIMATION_MAX
}ANIMATION_TYPE;

//プロトタイプ宣言
void InitAnimation(void);
void UninitAnimation(void);
void UpdateAnimation(void);
void DrawAnimation(void);
void SetAnimation(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight);
Animation *GetAnimation(void);

#endif
