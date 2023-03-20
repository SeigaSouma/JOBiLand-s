//==============================================================
//
//DirectX[light.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _LIGHT_H_		//このマクロ定義がされていなかったら
#define _LIGHT_H_		//2重インクルード防止のマクロを定義する

#include"main.h"

//列挙型定義
typedef enum
{
	LIGHTCOL_TYPE_MIDNIGHT = 0,	//夜中
	LIGHTCOL_TYPE_DAWN,			//夜明け
	LIGHTCOL_TYPE_EARLY,		//早朝
	LIGHTCOL_TYPE_DAYTIME,		//昼間
	LIGHTCOL_TYPE_MAX
}LIGHTCOL_TYPE;

typedef enum
{
	LIGHTTYPE_MAIN0 = 0,
	LIGHTTYPE_MAIN1,
	LIGHTTYPE_MAIN2,
	LIGHTTYPE_POINT0,
}LIGHTTYPE;

//構造体
typedef struct
{
	D3DLIGHT9 light;		//元のライト構造体
	D3DXVECTOR3 vecDir;		//設定用方向ベクトル
	D3DXVECTOR3 vecDirDest[LIGHTCOL_TYPE_MAX];	//目標の方向ベクトル
	D3DXVECTOR3 vecDirDiff[LIGHTCOL_TYPE_MAX];	//方向ベクトルの差分
	D3DXCOLOR DiffuseDest[LIGHTCOL_TYPE_MAX];	//目標の拡散光
	D3DXCOLOR DiffuseDiff[LIGHTCOL_TYPE_MAX];	//拡散光の差分
}LIGHT;

//プロトタイプ宣言
void InitLight(void);
void UnInitLight(void);
void UpdateLight(void);
void ResetLight(void);

#endif
