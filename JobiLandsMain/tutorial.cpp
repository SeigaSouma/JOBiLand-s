//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "tutorial.h"
#include "sound.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "meshcylinder.h"
#include "impactwave.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshdome.h"
#include "model.h"
#include "player.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "debugproc.h"
#include "shadow.h"
#include "particle.h"
#include "2D_effect.h"
#include "effect.h"
#include "pause.h"

//�}�N����`
#define FEVERHYPNOSIS_NUM	(10)

//�O���[�o���ϐ�
Tutorial g_aTutorial;	//�`���[�g���A���̏��
bool g_bSkip;

//==============================================================
//�`���[�g���A����ʂ̏���������
//==============================================================
void InitTutorial(void)
{
	//�e��ϐ�������
	g_aTutorial.nStep = 0;	//�X�e�b�v�̏�����
	g_aTutorial.aControl.bUPMove = false;		//��ړ�
	g_aTutorial.aControl.bLMove = false;		//���ړ�
	g_aTutorial.aControl.bRMove = false;		//�E�ړ�
	g_aTutorial.aControl.bDWMove = false;		//���ړ�

	g_aTutorial.aControl.bSetMob = false;		//���u�z�u������
	g_aTutorial.aControl.bDispatchR = false;	//�E�h��
	g_aTutorial.aControl.bDispatchL = false;	//���h��
	g_aTutorial.aControl.bReturnL = false;		//���A��
	g_aTutorial.aControl.bReturnR = false;		//�E�A��
	
	g_aTutorial.aControl.bHypnosis = false;		//�Ö��xMAX
	g_aTutorial.aControl.bCutePillow = false;	//�������J�o�[�킹��

	g_aTutorial.aControl.bFeverUse = false;		//�t�B�[�o�[��Ԃɂ�����
	g_aTutorial.aControl.bSetMobFever = false;	//�t�B�[�o�[���Ƀ��u�z�u������
	g_aTutorial.aControl.nCntHypnosisFever = 0;	//�t�B�[�o�[���Ƀ��u�߂܂�����

	g_aTutorial.aControl.bSetEnemy = false;		//�G�z�u������
	g_aTutorial.aControl.bReleaseCrowd = false;	//���]����

	g_bSkip = false;		//�X�L�b�v�m�F��ʕ`�悵�Ȃ�

	//�e�̏���������
	InitShadow();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//2D�G�t�F�N�g�̏���������
	InitEffect_2D();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//�Ռ��g�̏���������
	InitImpactWave();

	//���b�V���̏��̏���������
	InitMeshField();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//���f���̏���������
	InitModel();

	//�v���C���[�̏���������
	InitPlayer();

	//�|�[�Y���j���[�̏���������
	InitPause();

	//�J��������������
	InitCamera();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM_TUTORIAL);
}

//==============================================================
//�`���[�g���A����ʂ̏I������
//==============================================================
void UninitTutorial(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�|�[�Y���j���[�̏I������
	UninitPause();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//2D�G�t�F�N�g�̏I������
	UninitEffect_2D();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�e�̏I������
	UninitShadow();

	//�v���C���[�̏I������
	UninitPlayer();

	//���f���̏I������
	UninitModel();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���h�[���̏I������
	UninitMeshDome();

	//�Ռ��g�̏I������
	UninitImpactWave();

	//���b�V���̏��̏I������
	UninitMeshField();
}

//==============================================================
//�`���[�g���A����ʂ̍X�V����
//==============================================================
void UpdateTutorial(void)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	if (g_bSkip == false)
	{
		//���f���̍X�V����
		UpdateModel();

		//2D�G�t�F�N�g�̍X�V����
		UpdateEffect_2D();

		//if (pEdit->bUse == false)
		{
			//�G�t�F�N�g�̍X�V����
			UpdateEffect();

			//�p�[�e�B�N���̍X�V����
			UpdateParticle();

			//���b�V���̏��̍X�V����
			UpdateMeshField();

			//���b�V���V�����_�[�̍X�V����
			UpdateMeshCylinder();

			//���b�V���h�[���̍X�V����
			UpdateMeshDome();

			//�e�̍X�V����
			UpdateShadow();

			//�Ռ��g�̍X�V����
			UpdateImpactWave();
		}
	}

	PrintDebugProc("�`���[�g���A���̃X�e�b�v�F%d", g_aTutorial.nStep);

}

//==============================================================
//�`���[�g���A����ʂ̕`�揈��
//==============================================================
void DrawTutorial(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{

		//���f���̕`�揈��
		DrawModel(0);

		//���b�V���̏��̕`�揈��
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���h�[���̕`�揈��
		DrawMeshDome();

		//�e�̕`�揈��
		DrawShadow();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();

		//�Ռ��g�̕`�揈��
		DrawImpactWave();

		//���f���̕`�揈��
		DrawModel(1);
	}

	if (nType == DRAWTYPE_UI)
	{
		//2D�G�t�F�N�g�̕`�揈��
		DrawEffect_2D();
	}
}

//==============================================================
//�`���[�g���A����ʂ̏��擾
//==============================================================
Tutorial *GetTutorial(void)
{
	return &g_aTutorial;
}

//==============================================================
//�X�L�b�v�m�F�̐ݒ菈��
//==============================================================
void SetEnableSkip(bool bSkip)
{
	g_bSkip = bSkip;
}
