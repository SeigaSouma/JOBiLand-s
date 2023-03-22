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

//列挙型定義
typedef enum
{
	MOTION_LOOP_OFF = 0,	//ループ無し
	MOTION_LOOP_ON,		//ループする
	MOTION_LOOP_MAX
}MOTION_LOOP;

//プロトタイプ宣言
void UpdateMotion(MotionData *aMotionData, Motion *aMotion, Model *aParts, D3DXVECTOR3 *pPos);
void SetMotison(Motion *aMotion, int nMotionType);

#endif