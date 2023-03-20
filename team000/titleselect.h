//=============================================================================
//
// モブ処理 [titleselect.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _TITLESELECT_H_		//このマクロ定義がされていなかったら
#define _TITLESELECT_H_		//二重インクルード防止のマクロを定義する

#include "main.h"
#include "model.h"

//マクロ定義
#define MAX_TITLESELECT	(2)

//群衆の構造体
typedef struct
{
	int nFloating;	//浮遊の向き
	int nTurnCounter;	//回転した回数カウント
	float fPosDiffX;	//目的の位置との差分
	float fPosDiffY;	//目的の位置との差分
	Model aModel;					//モデルの構造体
}TitleSelect;

//プロトタイプ宣言
void InitTitleSelect(void);
void UninitTitleSelect(void);
void UpdateTitleSelect(void);
void UpdateTutorialTitleSelect(void);
void DrawTitleSelect(void);
TitleSelect *GetTitleSelect(void);
void SetTitleSelect(D3DXVECTOR3 pos, int nModelType);

#endif