//=============================================================================
//
// モーション処理 [motion.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _MOTION_H_	//このマクロ定義がされていなかったら
#define _MOTION_H_	//二重インクルード防止のマクロを定義する

#include "main.h"
#include "model.h"

//マクロ定義
#define MAX_MOTION	(16)	//モーションの最大数
#define MAX_KEY		(24)	//キーの最大数
#define MAX_PARTS	(32)	//パーツの最大数

//列挙型定義
typedef enum
{
	MOTION_LOOP_OFF = 0,	//ループ無し
	MOTION_LOOP_ON,		//ループする
	MOTION_LOOP_MAX
}MOTION_LOOP;

//パーツ毎の構造体
typedef struct
{
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotDest;	//目標の向き
	D3DXVECTOR3 rotDiff;	//向きの差分
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posDest;	//目標の位置
	D3DXVECTOR3 posDiff;	//位置の差分
}Parts;

//キー毎の構造体
typedef struct
{
	int nFrame;			//再生フレーム
	Parts aParts[MAX_PARTS];
}Key;

//モーション毎の構造体
typedef struct
{
	int nNumKey;		//キーの数
	int nLoop;			//ループ判定
	bool bUse;			//使用しているか
	Key aKey[MAX_KEY];
}MotionData;

//モーション全体の構造体
typedef struct
{
	int nNowMotionNum;		//現在のモーション番号
	int nCntAllFrame;		//総フレームカウント
	int nCntFrame;			//フレームのカウント
	int nPatternKey;		//何個目のキーか
	int nPartsNum;			//パーツ数
}Motion;

//プロトタイプ宣言
void UpdateMotion(MotionData *aMotionData, Motion *aMotion, Model *aParts, D3DXVECTOR3 *pPos);
void SetMotison(Motion *aMotion, int nMotionType);

#endif