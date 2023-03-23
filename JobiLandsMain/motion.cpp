//=============================================================================
//
// モーション処理 [motion.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "motion.h"
#include "model.h"
#include "calculation.h"

//==================================================================================
//モーションの設定処理
//==================================================================================
void SetMotion(Motion *aMotion, int nMotionType)
{

	if (aMotion->nNowMotionNum != nMotionType)
	{//セットするモーションと現在のモーションが同じじゃなかったら

		//現在のモーション番号設定
		aMotion->nNowMotionNum = nMotionType;

		//現在のキー番号リセット
		aMotion->nPatternKey = 0;

		//フレームカウントリセット
		aMotion->nCntFrame = 0;

		//総フレーム数リセット
		aMotion->nCntAllFrame = 0;
	}
}

//==================================================================================
//モーション更新処理
//==================================================================================
void UpdateMotion(MotionData *aMotionData, Motion *aMotion, Model *aParts, D3DXVECTOR3 *pPos)
{
	for (int nCntParts = 0; nCntParts < aMotion->nPartsNum; nCntParts++)
	{//パーツ分繰り返す

		//次のキー
		int nNextKey = (aMotion->nPatternKey + 1) % aMotionData[aMotion->nNowMotionNum].nNumKey;

		//前回と次のキーの差分取得
		float rotDiffX = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.z;

		//パーツの向きを設定
		aParts[nCntParts].rot.x =
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)aMotion->nCntFrame /
			(float)aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].nFrame
			);

		aParts[nCntParts].rot.y =
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)aMotion->nCntFrame /
			(float)aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].nFrame
			);

		aParts[nCntParts].rot.z =
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)aMotion->nCntFrame /
			(float)aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].nFrame
			);

		//角度の正規化
		RotNormalize(&aParts[aMotion->nNowMotionNum].rot.x);
		RotNormalize(&aParts[aMotion->nNowMotionNum].rot.y);
		RotNormalize(&aParts[aMotion->nNowMotionNum].rot.z);


		//パーツの位置を設定
		if (nCntParts == 0)
		{
			//目標の位置との差分を求める
			float posDiffX = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.z;

			//位置補正
			pPos->x +=
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.x +
				posDiffX *
				(
				(float)aMotion->nCntFrame /
				(float)aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].nFrame);

			pPos->y =
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.y +
				posDiffY *
				(
				(float)aMotion->nCntFrame /
				(float)aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].nFrame);

			pPos->z +=
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.z +
				posDiffZ *
				(
				(float)aMotion->nCntFrame /
				(float)aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].nFrame);
		}
	}

	//フレームのカウントを加算
	aMotion->nCntFrame++;
	aMotion->nCntAllFrame++;

	if (aMotion->nCntFrame >= aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].nFrame)
	{//フレームのカウントがフレーム数に達したら

		//フレームのカウントをゼロに戻す
		aMotion->nCntFrame = 0;

		//パターンNO.更新
		aMotion->nPatternKey = (aMotion->nPatternKey + 1) % aMotionData[aMotion->nNowMotionNum].nNumKey;

		if (aMotion->nPatternKey == 0)
		{//キーがゼロの時

			//総フレーム数リセット
			aMotion->nCntAllFrame = 0;

			//モーションの設定
			if (aMotionData[aMotion->nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{//ループモーションじゃない時
				
				//ニュートラルモーションを設定
				SetMotion(aMotion, 0);
			}
			else
			{//ループモーションの時

				//現在と同じモーションを設定
				SetMotion(aMotion, aMotion->nNowMotionNum);
			}
		}
	}
}