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

//�}�N����`
#define MAX_MOTION	(16)	//���[�V�����̍ő吔
#define MAX_KEY		(24)	//�L�[�̍ő吔
#define MAX_PARTS	(32)	//�p�[�c�̍ő吔

//�񋓌^��`
typedef enum
{
	MOTION_LOOP_OFF = 0,	//���[�v����
	MOTION_LOOP_ON,		//���[�v����
	MOTION_LOOP_MAX
}MOTION_LOOP;

//�p�[�c���̍\����
typedef struct
{
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 rotDest;	//�ڕW�̌���
	D3DXVECTOR3 rotDiff;	//�����̍���
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posDest;	//�ڕW�̈ʒu
	D3DXVECTOR3 posDiff;	//�ʒu�̍���
}Parts;

//�L�[���̍\����
typedef struct
{
	int nFrame;			//�Đ��t���[��
	Parts aParts[MAX_PARTS];
}Key;

//���[�V�������̍\����
typedef struct
{
	int nNumKey;		//�L�[�̐�
	int nLoop;			//���[�v����
	bool bUse;			//�g�p���Ă��邩
	Key aKey[MAX_KEY];
}MotionData;

//���[�V�����S�̂̍\����
typedef struct
{
	int nNowMotionNum;		//���݂̃��[�V�����ԍ�
	int nCntAllFrame;		//���t���[���J�E���g
	int nCntFrame;			//�t���[���̃J�E���g
	int nPatternKey;		//���ڂ̃L�[��
	int nPartsNum;			//�p�[�c��
}Motion;

//�v���g�^�C�v�錾
void UpdateMotion(MotionData *aMotionData, Motion *aMotion, Model *aParts, D3DXVECTOR3 *pPos);
void SetMotison(Motion *aMotion, int nMotionType);

#endif