//=============================================================================
//
// ���[�V�������� [motion.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _MOTION_H_	//���̃}�N����`������Ă��Ȃ�������
#define _MOTION_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "model.h"

//�񋓌^��`
typedef enum
{
	MOTION_LOOP_OFF = 0,	//���[�v����
	MOTION_LOOP_ON,		//���[�v����
	MOTION_LOOP_MAX
}MOTION_LOOP;

//�v���g�^�C�v�錾
void UpdateMotion(MotionData *aMotionData, Motion *aMotion, Model *aParts, D3DXVECTOR3 *pPos);
void SetMotison(Motion *aMotion, int nMotionType);

#endif