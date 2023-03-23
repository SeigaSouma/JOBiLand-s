//==============================================================
//
//DirectX[tutorial.h]
//Author:����������
//
//==============================================================
#ifndef _TUTORIAL_H_				//���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�^�C�g���̏��
typedef enum
{
	TUTORIALSTATE_APPEAR = 0,			//�o�����(�_��)
	TUTORIALSTATE_NORMAL,				//�ʏ���
	TUTORIALSTATE_WAIT,				//�o���҂����(�\���Ȃ�)
	TUTORIALSTATE_MAX
} TUTORIALSTATE;

typedef enum
{
	TUTORIAL_STEP_START = 0,
	TUTORIAL_STEP_MOVE,
	TUTORIAL_STEP_DISPATCH,
	TUTORIAL_STEP_RETURN,
	TUTORIAL_STEP_HYPNOSIS,
	TUTORIAL_STEP_SENNOU,
	TUTORIAL_STEP_USEFEVER,
	TUTORIAL_STEP_HYPNOSISFEVER,
	TUTORIAL_STEP_MORS,
	TUTORIAL_STEP_END,
	TUTORIAL_STEP_MAX
}TUTORIAL_STEP;

//�`���[�g���A���\���̂̒�`
typedef struct
{
	bool bUPMove;		//��ړ�
	bool bLMove;		//���ړ�
	bool bRMove;		//�E�ړ�
	bool bDWMove;		//���ړ�
	bool bSetMob;		//���u�z�u������
	bool bDispatchR;	//�E�h��
	bool bDispatchL;	//���h��
	bool bReturnL;		//���A��
	bool bReturnR;		//�E�A��
	bool bHypnosis;		//�Ö��xMAX
	bool bCutePillow;	//�������J�o�[�킹��
	bool bFeverUse;		//�t�B�[�o�[��Ԃɂ�����
	int nCntHypnosisFever;	//�t�B�[�o�[���Ƀ��u�߂܂�����
	bool bSetMobFever;		//�t�B�[�o�[���Ƀ��u�z�u������

	bool bSetEnemy;		//�G�z�u������
	bool bReleaseCrowd;	//���]����

}TutorialControl;

typedef struct
{
	int nStep;				//�X�e�b�v
	TutorialControl aControl;	//�`���[�g���A���̑����
} Tutorial;

//�v���g�^�C�v�錾
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

#endif
