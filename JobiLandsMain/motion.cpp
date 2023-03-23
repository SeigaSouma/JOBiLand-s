//=============================================================================
//
// ���[�V�������� [motion.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "motion.h"
#include "model.h"
#include "calculation.h"

//==================================================================================
//���[�V�����̐ݒ菈��
//==================================================================================
void SetMotion(Motion *aMotion, int nMotionType)
{

	if (aMotion->nNowMotionNum != nMotionType)
	{//�Z�b�g���郂�[�V�����ƌ��݂̃��[�V��������������Ȃ�������

		//���݂̃��[�V�����ԍ��ݒ�
		aMotion->nNowMotionNum = nMotionType;

		//���݂̃L�[�ԍ����Z�b�g
		aMotion->nPatternKey = 0;

		//�t���[���J�E���g���Z�b�g
		aMotion->nCntFrame = 0;

		//���t���[�������Z�b�g
		aMotion->nCntAllFrame = 0;
	}
}

//==================================================================================
//���[�V�����X�V����
//==================================================================================
void UpdateMotion(MotionData *aMotionData, Motion *aMotion, Model *aParts, D3DXVECTOR3 *pPos)
{
	for (int nCntParts = 0; nCntParts < aMotion->nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

		//���̃L�[
		int nNextKey = (aMotion->nPatternKey + 1) % aMotionData[aMotion->nNowMotionNum].nNumKey;

		//�O��Ǝ��̃L�[�̍����擾
		float rotDiffX = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].rot.z;

		//�p�[�c�̌�����ݒ�
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

		//�p�x�̐��K��
		RotNormalize(&aParts[aMotion->nNowMotionNum].rot.x);
		RotNormalize(&aParts[aMotion->nNowMotionNum].rot.y);
		RotNormalize(&aParts[aMotion->nNowMotionNum].rot.z);


		//�p�[�c�̈ʒu��ݒ�
		if (nCntParts == 0)
		{
			//�ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = aMotionData[aMotion->nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].aParts[nCntParts].pos.z;

			//�ʒu�␳
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

	//�t���[���̃J�E���g�����Z
	aMotion->nCntFrame++;
	aMotion->nCntAllFrame++;

	if (aMotion->nCntFrame >= aMotionData[aMotion->nNowMotionNum].aKey[aMotion->nPatternKey].nFrame)
	{//�t���[���̃J�E���g���t���[�����ɒB������

		//�t���[���̃J�E���g���[���ɖ߂�
		aMotion->nCntFrame = 0;

		//�p�^�[��NO.�X�V
		aMotion->nPatternKey = (aMotion->nPatternKey + 1) % aMotionData[aMotion->nNowMotionNum].nNumKey;

		if (aMotion->nPatternKey == 0)
		{//�L�[���[���̎�

			//���t���[�������Z�b�g
			aMotion->nCntAllFrame = 0;

			//���[�V�����̐ݒ�
			if (aMotionData[aMotion->nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{//���[�v���[�V��������Ȃ���
				
				//�j���[�g�������[�V������ݒ�
				SetMotion(aMotion, 0);
			}
			else
			{//���[�v���[�V�����̎�

				//���݂Ɠ������[�V������ݒ�
				SetMotion(aMotion, aMotion->nNowMotionNum);
			}
		}
	}
}