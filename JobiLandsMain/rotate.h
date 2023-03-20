//==============================================================
//
//DirectX[rotate.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _ROTATE_H_		//このマクロ定義がされていなかったら
#define _ROTATE_H_		//2重インクルード防止のマクロを定義する

#include"main.h"

//マクロ定義
#define NUM_ROTATE			(3)			//ぐるぐるの個数

//ぐるぐる構造体の定義
typedef struct
{
	D3DXVECTOR3 pos[NUM_ROTATE];	//位置
	D3DXVECTOR3 rot[NUM_ROTATE];	//向き
	D3DXMATRIX mtxWorld;			//ワールドマトリックス

	bool bUse;						//ぐるぐるを使用しているか
	bool bDisp;						//ぐるぐるを描画するか
} Rotate;

//プロトタイプ宣言
void InitRotate(void);
void UnInitRotate(void);
void UpdateRotate(void);
void DrawRotate(void);
void SetPositionRotate(int nIdxRotate, D3DXVECTOR3 pos);
int SetRotate(void);
Rotate *GetRotate(void);

#endif
