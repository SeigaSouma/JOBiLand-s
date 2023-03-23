//==============================================================
//
//�Q�[������[game.cpp]
//Author:����������
//
//==============================================================
#include "game.h"
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
#include "edit.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "debugproc.h"
#include "shadow.h"
#include "particle.h"
#include "2D_effect.h"
#include "effect.h"
#include "pause.h"
#include "sound.h"
#include "nofade.h"
#include "launch.h"
#include "scorecircle.h"
#include "controllpanel.h"

//�v���g�^�C�v�錾
void DebugEdit(void);

//�O���[�o���ϐ�
GAMESTATE g_gameState;		//�Q�[���̏��
int g_nCounterGameState;	//��ԊǗ��J�E���^�[
bool g_bPause = false;		//�|�[�Y��Ԃ�ON/OFF

//==============================================================
//�Q�[����ʂ̏���������
//==============================================================
void InitGame(void)
{
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

	//���b�V���̕ǂ̏���������
	//InitMeshWall();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//���f���̏���������
	InitModel();

	//�v���C���[�̏���������
	InitPlayer();

	//�G�f�B�b�g�̏�����
	InitEdit();

	//�|�[�Y���j���[�̏���������
	InitPause();

	//�X�R�A�̏���������
	InitScore();

	//�Q�[���̃J��������������
	InitCamera();

	// ���˕��̏���������
	InitLaunch();

	// �X�R�A�T�[�N���̏���������
	InitScoreCircle();

	// ����p�l���̏���������
	InitControllPanel();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM_GAME);

	//�e��ϐ�������
	g_gameState = GAMESTATE_NONE;
	g_nCounterGameState = 0;
	g_bPause = false;
}

//==============================================================
//�Q�[����ʂ̏I������
//==============================================================
void UninitGame(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�|�[�Y���j���[�̏I������
	UninitPause();
	SetEnablePause(false);		//�|�[�Y����������

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//2D�G�t�F�N�g�̏I������
	UninitEffect_2D();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�e�̏I������
	UninitShadow();

	//�G�f�B�b�g�̏I������
	UninitEdit();

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

	//���b�V���̕ǂ̏I������
	//UninitMeshWall();

	//���b�V���̏��̏I������
	UninitMeshField();

	// ���˕��̏I��
	UninitLaunch();

	// �X�R�A�T�[�N���̏I������
	UninitScoreCircle();

	// ����p�l���̏I������
	UninitControllPanel();

	//�X�R�A�̏I������
	UninitScore();
}

//==============================================================
//�Q�[����ʂ̍X�V����
//==============================================================
void UpdateGame(void)
{
	//�G�f�B�b�g�̏��擾
	Edit *pEdit = GetEdit();

	if (GetFade() == FADE_NONE)
	{//�������Ă��Ȃ��Ƃ�
		if (GetKeyboardTrigger(DIK_F2) == true)
		{
			SetFade(MODE_RESULT);
		}
	}

	if (g_bPause == false)
	{
		//�G�f�B�b�g�̍X�V����
		UpdateEdit();

		//���f���̍X�V����
		UpdateModel();

		//2D�G�t�F�N�g�̍X�V����
		UpdateEffect_2D();

		if (pEdit->bUse == false)
		{
			//if (pCamera->nState != CAMERASTATE_MANYSPAWN && pCutin->bUse == false)
			{//��ʔ����̃J��������Ȃ������� && �J�b�g�C��������Ȃ�������

				//�G�t�F�N�g�̍X�V����
				UpdateEffect();

				//�p�[�e�B�N���̍X�V����
				UpdateParticle();

				//���b�V���̏��̍X�V����
				UpdateMeshField();

				//���b�V���̕ǂ̍X�V����
				//UpdateMeshWall();

				//���b�V���V�����_�[�̍X�V����
				UpdateMeshCylinder();

				//���b�V���h�[���̍X�V����
				UpdateMeshDome();

				// ���˕��̍X�V
				UpdateLaunch();

				//�v���C���[�̍X�V����
				UpdatePlayer();

				//�e�̍X�V����
				UpdateShadow();

				//�Ռ��g�̍X�V����
				UpdateImpactWave();

				////�O�Ղ̍X�V����
				//UpdateMeshOrbit();

				//�X�R�A�̍X�V����
				UpdateScore();

				// �X�R�A�T�[�N���̍X�V����
				UpdateScoreCircle();

				// ����p�l���̍X�V����
				UpdateControllPanel();

			}
		}
	}
	else if (g_bPause == true)
	{//�|�[�Y�{�^����������Ă���Ƃ�

		UpdatePause();		//�|�[�Y���j���[��\������
	}

	if (*GetGameState() != GAMESTATE_START)
	{
		if ((GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(BUTTON_START, 0)) &&
			GetFade() == FADE_NONE)
		{//P�L�[�������ꂽ��

			if (g_bPause == false)
			{
				//�T�E���h�Đ�
				//PlaySound(SOUND_LABEL_SE_THEWORLD);
			}

			g_bPause = (g_bPause == false) ? true : false;		//�|�[�Y���j���[���g���Ă����Ԃɂ���
		}
	}

	
	switch (g_gameState)
	{
	case GAMESTATE_SHOT:		// �ł��o�����

		// ���˂��鏈��
		ShotLaunchChunk();

		break;

	case GAMESTATE_END:

		// �J�ڃJ�E���^�[���Z
		g_nCounterGameState--;

		if (g_nCounterGameState <= 0)
		{
			// ���[�h�ݒ�(���U���g��ʂɈڍs)
			SetFade(MODE_RESULT);		// �t �F �[ �h �A �E �g
		}
		break;
	}
}

//==============================================================
//�Q�[����ʂ̕`�揈��
//==============================================================
void DrawGame(int nType)
{

	if (nType == DRAWTYPE_MAIN)
	{

		//���f���̕`�揈��
		DrawModel(0);

		// ���˕��̕`�揈��
		DrawLaunch();

		//���b�V���̏��̕`�揈��
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//���b�V���̕ǂ̕`�揈��
		//DrawMeshWall();

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���h�[���̕`�揈��
		DrawMeshDome();

		//�O�Ղ̕`�揈��
		//DrawMeshOrbit();

		//�e�̕`�揈��
		DrawShadow();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();

		//�G�f�B�b�g�̕`�揈��
		DrawEdit();

		////�X�R�A�̕`�揈��
		//DrawScore();

		//�G�f�B�b�g�̕`�揈��
		DebugEdit();

		//�Ռ��g�̕`�揈��
		DrawImpactWave();

		// �X�R�A�T�[�N���̕`�揈��
		DrawScoreCircle();

		//���f���̕`�揈��
		DrawModel(1);

	}

	if (nType == DRAWTYPE_UI)
	{

		if (g_gameState != GAMESTATE_START)
		{
			//2D�G�t�F�N�g�̕`�揈��
			DrawEffect_2D();
		}

		//�X�R�A�̕`�揈��
		DrawScore();

		// ����p�l���̕`�揈��
		DrawControllPanel();

		if (g_bPause == true)
		{//�|�[�Y���j���[���g���Ă���Ƃ�

			//�|�[�Y��ʂ�\������
			DrawPause();
		}
	}
}

//==============================================================
//�Q�[����Ԃ̐ݒ�
//==============================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//==============================================================
//�Q�[����Ԃ̎擾
//==============================================================
GAMESTATE *GetGameState(void)
{
	return &g_gameState;
}

//==============================================================
//�|�[�Y��ʂ̐ݒ菈��
//==============================================================
void SetEnablePause(bool bPouse)
{
	g_bPause = bPouse;
}

//==============================================================
//�|�[�Y�̏��擾
//==============================================================
bool GetPause(void)
{
	return g_bPause;
}

//==============================================================
//�G�f�B�b�g�̑������
//==============================================================
void DebugEdit(void)
{
	//�G�f�B�b�g�̏��擾
	Edit *pEdit = GetEdit();

	PrintDebugProc("[F3]�F���C���[�t���[��\n");
	PrintDebugProc("[F6]�F�w�ʒǏ]�؂�ւ�\n");
	PrintDebugProc("[F7]�F�J�����Ǐ]�؂�ւ�\n");
	PrintDebugProc("[F8]�F�G�f�B�b�g���[�h\n");

	if (pEdit->bUse == true)
	{//�G�f�B�b�g���[�h����Ȃ���

		//������ɑ��
		PrintDebugProc(
			"<���f���z�u>   [ENTER]\n"
			"<�ړ�>         �����F[��,��,��,��]�@�ᑬ�F[W,A,S,D]\n"
			"<��]>         [LSHIFT, RSHIFT]\n"
			"<�㏸,���~>    [I,K]\n"
			"<��ޕύX>     [1, 2][%d]\n"
			"<�݈͂ړ�>     [SPACE]\n"
			"<�폜>         [DELETE]\n"
			"<�ʒu>         [X�F%f Y�F%f Z�F%f]\n"
			"<����>         [X�F%f Y�F%f Z�F%f]\n", pEdit->nNowModelType, pEdit->pos.x, pEdit->pos.y, pEdit->pos.z, pEdit->rot.x, pEdit->rot.y, pEdit->rot.z);

	}
}

//==============================================================
//�Q�[���I�����̍X�V
//==============================================================
void SetGameEnd(void)
{
	////�v���C���[�̏��擾
	//Player *pPlayer = GetPlayer();

	////�J�����̏��擾
	//Camera *pCamera = GetCamera();

	////�J�����̈ʒu�ݒ�
	//pCamera->rot = D3DXVECTOR3(0.0f, 0.0f, -0.35f);
	//pCamera->fDistance = 800;	//����

	////�v���C���[�̈ʒu�ݒ�
	//pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	////�j���[�g�������[�V�����ݒ�
	//SetMotionPlayer(PLAYERMOTION_DEF);
}