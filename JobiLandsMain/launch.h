//=============================================================================
//
// 発射物処理 [launch.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _LAUNCH_H_		//このマクロ定義がされていなかったら
#define _LAUNCH_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_LAUNCH		(128)			// 発射物の最大数

#include "main.h"
#include "model.h"

//モデルの種類
typedef enum
{
	LAUNCHTYPE_GOOD = 0,		// 良い奴
	LAUNCHTYPE_EVIL,			// 悪い奴
	LAUNCHTYPE_MAX				// この列挙型の総数
}LAUNCHTYPE;

//モデルの種類
typedef enum
{
	LAUNCHSTATE_FLY = 0,			// 飛ぶ状態
	LAUNCHSTATE_RETURN_POSSIBLE,	// 跳ね返し可能状態
	LAUNCHSTATE_RETURN,				// 跳ね返し状態
	LAUNCHSTATE_MAX					// この列挙型の総数
}LAUNCHSTATE;

//プレイヤーの構造体
typedef struct
{
	Model modelData;	// モデル情報
	float fGravity;		// 重力
	int nScore;			// スコアを
}Launch;

//プロトタイプ宣言
void InitLaunch(void);
void UninitLaunch(void);
void UpdateLaunch(void);
void DrawLaunch(void);
void SetLaunch(void);
Launch *GetLaunch(void);

#endif