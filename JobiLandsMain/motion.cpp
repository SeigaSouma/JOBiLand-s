//=============================================================================
//
// モーション処理 [motion.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "motion.h"


//プロトタイプ宣言

//グローバル変数宣言

//==================================================================================
//モーション更新処理
//==================================================================================
void UpdateMotion(MotionData *aMotionData, Motion *aMotion)
{
	for (int nCntParts = 0; nCntParts < aMotion->nPartsNum; nCntParts++)
	{//パーツ分繰り返す

		//次のキー
		int nNextKey = (g_aPlayer.nPatternKey + 1) % g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey;
		int nnn;

		if (g_aPlayer.nPatternKey == g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
		{//ループしないとき

		 //nNextKey = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		if (g_aPlayer.nPatternKey == g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_ON)
		{//ループの時

			nnn = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		//前回と次のキーの差分取得
		float rotDiffX = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z;

		//パーツの向きを設定
		g_aPlayer.aModel[nCntParts].rot.x =
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		g_aPlayer.aModel[nCntParts].rot.y =
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		g_aPlayer.aModel[nCntParts].rot.z =
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		//角度の正規化
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x);
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y);
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z);


		//パーツの位置を設定
		if (nCntParts == 0)
		{
			//目標の位置との差分を求める
			float posDiffX = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z;

			//位置補正
			g_aPlayer.pos.x +=
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x +
				posDiffX *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);

			g_aPlayer.pos.y =
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y +
				posDiffY *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);

			g_aPlayer.pos.z +=
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z +
				posDiffZ *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);
		}
	}

	//フレームのカウントを加算
	g_aPlayer.nCntFrame++;
	g_aPlayer.nCntAllFrame++;

	if (g_aPlayer.nCntFrame >= g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame)
	{//フレームのカウントがフレーム数に達したら

	 //フレームのカウントをゼロに戻す
		g_aPlayer.nCntFrame = 0;

		//パターンNO.更新
		g_aPlayer.nPatternKey = (g_aPlayer.nPatternKey + 1) % g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

		if (g_aPlayer.nPatternKey == 0)
		{//キーがゼロの時

		 //攻撃状態解除
			g_aPlayer.nCntAllFrame = 0;	//総フレーム数リセット

			if (g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
			else
			{
				//モーションの設定
				SetMotisonPlayer(g_aPlayer.nNowMotionNum);
			}
		}
	}
}