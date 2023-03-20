//=============================================================================
//
// 派遣テクスチャ処理 [dispatch_texture.h]
// Author : 佐藤根詩音
//
//=============================================================================
#ifndef _DISPATCHTEXTURE_H_				//このマクロ定義がされていなかったら
#define _DISPATCHTEXTURE_H_				//2重インクルード防止のマクロを定義する

#include "main.h"

//派遣テクスチャの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	int nCntDispatch;		//派遣
	bool bUse;				//使用してるか
} DispatchTex;

//プロトタイプ宣言
void InitDispatchTex(void);
void UninitDispatchTex(void);
void UpdateDispatchTex(void);
void DrawDispatchTex(void);
DispatchTex *GetDispatchTex(void);

#endif
