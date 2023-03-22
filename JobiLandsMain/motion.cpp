//=============================================================================
//
// ���[�V�������� [motion.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "motion.h"


//�v���g�^�C�v�錾

//�O���[�o���ϐ��錾

//==================================================================================
//���[�V�����X�V����
//==================================================================================
void UpdateMotion(MotionData *aMotionData, Motion *aMotion)
{
	for (int nCntParts = 0; nCntParts < aMotion->nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

		//���̃L�[
		int nNextKey = (g_aPlayer.nPatternKey + 1) % g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey;
		int nnn;

		if (g_aPlayer.nPatternKey == g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
		{//���[�v���Ȃ��Ƃ�

		 //nNextKey = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		if (g_aPlayer.nPatternKey == g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_ON)
		{//���[�v�̎�

			nnn = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		//�O��Ǝ��̃L�[�̍����擾
		float rotDiffX = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z;

		//�p�[�c�̌�����ݒ�
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

		//�p�x�̐��K��
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x);
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y);
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z);


		//�p�[�c�̈ʒu��ݒ�
		if (nCntParts == 0)
		{
			//�ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z;

			//�ʒu�␳
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

	//�t���[���̃J�E���g�����Z
	g_aPlayer.nCntFrame++;
	g_aPlayer.nCntAllFrame++;

	if (g_aPlayer.nCntFrame >= g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame)
	{//�t���[���̃J�E���g���t���[�����ɒB������

	 //�t���[���̃J�E���g���[���ɖ߂�
		g_aPlayer.nCntFrame = 0;

		//�p�^�[��NO.�X�V
		g_aPlayer.nPatternKey = (g_aPlayer.nPatternKey + 1) % g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

		if (g_aPlayer.nPatternKey == 0)
		{//�L�[���[���̎�

		 //�U����ԉ���
			g_aPlayer.nCntAllFrame = 0;	//���t���[�������Z�b�g

			if (g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
			else
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(g_aPlayer.nNowMotionNum);
			}
		}
	}
}