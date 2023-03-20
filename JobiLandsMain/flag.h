//==============================================================
//
//旗[flag.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _FLAG_H_		//このマクロ定義がされていなかったら
#define _FLAG_H_		//2重インクルード防止のマクロを定義する

#include"main.h"

//旗の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;			//位置
	D3DXMATRIX mtxWorld;		//ワールドマトリックス

	int nCntAnim;				//アニメーション変更カウンター
	int nPatternAnim;			//アニメーションNo.
	bool bUse;					//使用しているか
} Flag;

//プロトタイプ宣言
void InitFlag(void);
void UnInitFlag(void);
void UpdateFlag(void);
void DrawFlag(void);
void SetPositionFlag(int nIdxFlag, D3DXVECTOR3 pos);
int SetFlag(void);
Flag *GetFlag(void);

#endif
