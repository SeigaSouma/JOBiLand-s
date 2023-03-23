//=============================================================================
//
// ���˕����� [launch.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _LAUNCH_H_		//���̃}�N����`������Ă��Ȃ�������
#define _LAUNCH_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_LAUNCH		(128)			// ���˕��̍ő吔

#include "main.h"
#include "model.h"

//���f���̎��
typedef enum
{
	LAUNCHTYPE_GOOD = 0,		// �ǂ��z
	LAUNCHTYPE_EVIL,			// �����z
	LAUNCHTYPE_MAX
}LAUNCHTYPE;

//�v���C���[�̍\����
typedef struct
{
	Model		modelData;	// ���f�����
}Launch;

//�v���g�^�C�v�錾
void InitLaunch(void);
void UninitLaunch(void);
void UpdateLaunch(void);
void DrawLaunch(void);
void SetLaunch(void);
Launch *GetLaunch(void);

#endif