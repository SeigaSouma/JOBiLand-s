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
	LAUNCHTYPE_MAX				// ���̗񋓌^�̑���
}LAUNCHTYPE;

//���f���̎��
typedef enum
{
	LAUNCHSTATE_FLY = 0,			// ��ԏ��
	LAUNCHSTATE_RETURN_POSSIBLE,	// ���˕Ԃ��\���
	LAUNCHSTATE_RETURN,				// ���˕Ԃ����
	LAUNCHSTATE_MAX					// ���̗񋓌^�̑���
}LAUNCHSTATE;

//�v���C���[�̍\����
typedef struct
{
	Model modelData;	// ���f�����
	float fGravity;		// �d��
	int nScore;			// �X�R�A
	float fSpeed;		// ���x
	int nLevel;			// ���x��
}Launch;

// �\���̒�`
typedef struct
{
	int nLevel;			// ���x��
	float fFrame;		// �t���[����
	float fGravity;		// �d��
}Launch_Info;

//�v���g�^�C�v�錾
void InitLaunch(void);				// ���˕��̏���������
void UninitLaunch(void);			// ���˕��̏I������
void UpdateLaunch(void);			// ���˕��̍X�V����
void DrawLaunch(void);				// ���˕��̕`�揈��
void SetLaunch(int nLevel);			// ���˕��̐ݒ菈��
Launch *GetLaunch(void);			// ���˕��̎擾����
Launch_Info *GetLaundhLevel(void);	// ���˕��̃��x���̎擾����

#endif