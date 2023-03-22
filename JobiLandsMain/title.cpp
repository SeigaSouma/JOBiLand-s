//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "title.h"
#include "edit.h"
#include "fade.h"
#include "game.h"
#include "light.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "particle.h"
#include "effect.h"
#include "2D_effect.h"
#include "player.h"
#include "sound.h"
#include "titleselect.h"
#include "titlelogo.h"

//�}�N����`
#define CHANGE_TIME	(60 * 25)	//�؂�ւ��^�C�}�[

//�O���[�o���ϐ��錾
bool g_bEndFrag;									//�I���̃t���O
Title g_Title;

//==================================================================================
//�^�C�g���̏���������
//==================================================================================
void InitTitle(void)
{
	g_Title.OldSelect = TITLESELECT_START;	//�O��̑I����
	g_Title.nSelect = TITLESELECT_START;	//����̑I����
	g_Title.nCntChange = 0;
	g_Title.nState = TITLESTATE_NONE;	//���
	g_bEndFrag = false;

	//�e�̏���������
	InitShadow();

	//���f���̏���������
	InitModel();

	//�G�f�B�b�g�̏�����
	InitEdit();

	//�^�C�g���̑I��������������
	InitTitleSelect();

	// �^�C�g���̃��S�̏���������
	InitTitleLogo();

	//�v���C���[�̏���������
	InitPlayer();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//2D�G�t�F�N�g�̏���������
	InitEffect_2D();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�^�C�g���̃J��������������
	InitCamera();

	//BGM���Z�b�g
	PlaySound(SOUND_LABEL_BGM_TITLE);
}

//==================================================================================
//�^�C�g���̏I������
//==================================================================================
void UninitTitle(void)
{
	StopSound();

	//���f���̏I������
	UninitModel();

	//�G�f�B�b�g�̏I������
	UninitEdit();

	//�^�C�g���̑I�����I������
	UninitTitleSelect();

	// �^�C�g���̃��S�̏I������
	UninitTitleLogo();

	//�v���C���[�̏I������
	UninitPlayer();

	//�e�̏I������
	UninitShadow();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���E�H�[���̏I������
	UninitMeshDome();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//2D�G�t�F�N�g�̏I������
	UninitEffect_2D();

	//�p�[�e�B�N���̏I������
	UninitParticle();
}

//==================================================================================
//�^�C�g���̍X�V����
//==================================================================================
void UpdateTitle(void)
{

	if (GetFade() == FADE_NONE && g_Title.nState == TITLESTATE_NONE)
	{//�������Ă��Ȃ��Ƃ�

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange++;

		//�I������
		UpdateSelectTitle();

	}
	else if (g_Title.nState == TITLESTATE_TKTK)
	{//�`�J�`�J��Ԃ̂Ƃ�

		//�I�����̃`�J�`�J����
		//UpdateTitleTKTK();
	}
	else if (g_Title.nState == TITLESTATE_FADE)
	{//�J�ڏ�Ԃ̂Ƃ�

		switch (g_Title.nSelect)
		{

		case TITLESELECT_START:
			SetFade(MODE_TUTORIAL);
			break;

		case TITLESELECT_END:
			g_bEndFrag = true;	//�I���̃t���OON
			break;

		}

	}

	//�J�����̍X�V����
	UpdateCamera();

	//���f���̍X�V����
	UpdateModel();

	//�G�f�B�b�g�̍X�V����
	UpdateEdit();

	//�^�C�g���̑I�����X�V����
	UpdateTitleSelect();

	// �^�C�g���̃��S�̍X�V����
	UpdateTitleLogo();

	//�e�̍X�V����
	UpdateShadow();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//2D�G�t�F�N�g�̍X�V����
	UpdateEffect_2D();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	if (g_Title.nCntChange >= CHANGE_TIME)
	{//�����J�ڂ̃J�E���^�[���K��l�ɒB������

		//�����L���O�ɑJ��
		SetFade(MODE_RANKING);
	}
}

//==================================================================================
//�^�C�g���̕`�揈��
//==================================================================================
void DrawTitle(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{
		//���b�V���t�B�[���h�̕`�揈��
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���E�H�[���̕`�揈��
		DrawMeshDome();

		//�e�̕`�揈��
		DrawShadow();

		//���f���̕`�揈��
		DrawModel(0);

		//�G�f�B�b�g�̕`�揈��
		DrawEdit();

		// �^�C�g���̃��S�̕`�揈��
		DrawTitleLogo();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//�I�����̕`��
		DrawTitleSelect();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();
	}

	if (nType == DRAWTYPE_UI)
	{
		//2D�G�t�F�N�g�̕`�揈��
		DrawEffect_2D();
	}
}

//==================================================================================
//�I�����ʂ̍X�V����
//==================================================================================
void UpdateSelectTitle(void)
{

	if (GetKeyboardTrigger(DIK_D) == true || GetGamepadTrigger(BUTTON_RIGHT, 0) ||
		(GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0))
	{//�E�n�������ꂽ

	 //���X�e�B�b�N�̔����n��
		SetEnableStickSelect(true);

		//�p�^�[��No.���X�V
		g_Title.nSelect = (g_Title.nSelect + 1) % TITLESELECT_MAX;

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange = 0;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_CURSOR);
	}
	else if (GetKeyboardTrigger(DIK_A) == true || GetGamepadTrigger(BUTTON_LEFT, 0) ||
		(GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0))
	{//���n�������ꂽ

	 //���X�e�B�b�N�̔����n��
		SetEnableStickSelect(true);

		//�p�^�[��No.���X�V
		g_Title.nSelect = (g_Title.nSelect + (TITLESELECT_MAX - 1)) % TITLESELECT_MAX;

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange = 0;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_CURSOR);
	}


	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0))
	{//Enter�������ꂽ

		if (g_Title.nSelect == TITLESELECT_START)
		{//�X�^�[�g�̎������`�J�`�J

			//�`�J�`�J��ԂɈڍs
			g_Title.nState = TITLESTATE_FADE;
		}
		else
		{//���͂����J��

			//�t�F�[�h��ԂɈڍs
			g_Title.nState = TITLESTATE_FADE;
		}

		//�T�E���h�Đ�
		//PlaySound(SOUND_LABEL_SE_SYUTUGEKI);
	}

	//�I�����̐F�X�V
	//UpdateSelectTitleCol(g_Title.nSelect);

}

//==================================================================================
//�I���̃t���O���擾
//==================================================================================
bool GetEndFrag(void)
{
	return g_bEndFrag;
}

//==================================================================================
//�^�C�g���̏��擾
//==================================================================================
Title *GetTitle(void)
{
	return &g_Title;
}