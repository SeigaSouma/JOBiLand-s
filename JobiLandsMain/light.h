//==============================================================
//
//DirectX[light.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _LIGHT_H_		//このマクロ定義がされていなかったら
#define _LIGHT_H_		//2重インクルード防止のマクロを定義する

#include"main.h"

typedef enum
{
	LIGHTTYPE_MAIN0 = 0,
	LIGHTTYPE_MAIN1,
	LIGHTTYPE_MAIN2,
	LIGHTTYPE_MAX
}LIGHTTYPE;

//構造体
typedef struct
{
	D3DLIGHT9 light;		//元のライト構造体
	D3DXVECTOR3 vecDir;		//設定用方向ベクトル
	D3DXVECTOR3 vecDirDest;	//目標の方向ベクトル
	D3DXVECTOR3 vecDirDiff;	//方向ベクトルの差分
	D3DXCOLOR DiffuseDest;	//目標の拡散光
	D3DXCOLOR DiffuseDiff;	//拡散光の差分
}LIGHT;

//プロトタイプ宣言
void InitLight(void);
void UnInitLight(void);
void UpdateLight(void);

#endif
