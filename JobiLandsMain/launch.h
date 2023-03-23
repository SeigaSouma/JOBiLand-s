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
	int nScore;			// スコア
	float fSpeed;		// 速度
	int nLevel;			// レベル
}Launch;

// 構造体定義
typedef struct
{
	int nLevel;			// レベル
	float fFrame;		// フレーム数
	float fGravity;		// 重力
}Launch_Info;

//プロトタイプ宣言
void InitLaunch(void);				// 発射物の初期化処理
void UninitLaunch(void);			// 発射物の終了処理
void UpdateLaunch(void);			// 発射物の更新処理
void DrawLaunch(void);				// 発射物の描画処理
void SetLaunch(int nLevel);			// 発射物の設定処理
Launch *GetLaunch(void);			// 発射物の取得処理
Launch_Info *GetLaundhLevel(void);	// 発射物のレベルの取得処理

#endif