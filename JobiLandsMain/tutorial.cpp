//=============================================================================
//
// �`���[�g���A������ [tutorial.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "tutorial.h"
#include "tutorial_window.h"
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
#include "enemy.h"
#include "player.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "TimeCycle.h"
#include "debugproc.h"
#include "crowd.h"
#include "mob.h"
#include "buddy.h"
#include "rotate.h"
#include "hypnosis_gauge.h"
#include "dispatch.h"
#include "search_circle.h"
#include "iventsign.h"
#include "cutepillow.h"
#include "shadow.h"
#include "meshorbit.h"
#include "minimap.h"
#include "minimap_icon.h"
#include "particle.h"
#include "2D_effect.h"
#include "effect.h"
#include "pause.h"
#include "powergage.h"
#include "EventWindow.h"
#include "watergun.h"
#include "search_fan.h"
#include "manyUI.h"
#include "manypoint.h"
#include "manyMORS.h"
#include "eventstamp.h"
#include "dispatchpoint.h"
#include "serif.h"
#include "fevergauge.h"
#include "cutin.h"
#include "minimapframe.h"
#include "tutorial_talk.h"
#include "numberPeople.h"
#include "ground.h"
#include "tutorial_skip.h"
#include "feverpush.h"
#include "dispatch_texture.h"

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

	//���C�g�̃��Z�b�g����
	ResetLight();

	//�e�̏���������
	InitShadow();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//2D�G�t�F�N�g�̏���������
	InitEffect_2D();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//���邮��̏���������
	InitRotate();

	//�Ö��x�Q�[�W�̏���������
	InitHypnosisGauge();

	//��̌`�̏�����
	InitSearchFan();

	//�h���^�[�Q�b�g�̏���������
	InitDisPatch();

	//�h���V�����_�[�̏���������
	InitDispatchPoint();

	//��ʔ����n�_�̏���������
	InitManyPoint();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//�Ռ��g�̏���������
	InitImpactWave();

	//�T���͈͂̏���������
	InitSearchCircle();

	//���b�V���̏��̏���������
	InitMeshField();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//�~�j�}�b�v�̏�����
	InitMinimap();

	//�A�C�R���̏���������
	InitMiniIcon();

	//���f���̏���������
	InitModel();

	//�v���C���[�̏���������
	InitPlayer();

	//���_�̏���������
	InitBuddy();

	//�G�̏���������
	InitEnemy();

	//�C�x���g���̏���������
	InitIventSign();

	//�^�C���T�C�N���̏���������
	InitTimeCycle();

	//�t�B�[�o�[�Q�[�W�̏���������
	InitFeverGauge();

	//�Q�O�̏���������
	InitCrowd();

	//���u�̏���������
	InitMob();

	//�������J�o�[�̏���������
	InitCutepillow();

	//�|�[�Y���j���[�̏���������
	InitPause();

	//���̓Q�[�W�̏���������
	InitPowerGage();

	//�C�x���g�֘A�̏���������
	InitEventWindow();

	//��ʔ�����UI
	InitManyUI();

	//�x��������UI����������
	InitManyMORS();

	//�C�x���g�X�^���v�̏���������
	InitEventStamp();

	//���S�C�̒e�̏���������
	InitWatergun();

	//�Z���t�̏���������
	InitSerif();

	//�`���[�g���A���E�B���h�E�̏���������
	InitTutorialWindow();

	//�`���[�g���A���g�[�N�̏���������
	InitTutorialTalk();

	//�Q�[���̃J��������������
	InitGameCamera();

	//�J�b�g�C���̏���������
	InitCutin();

	//�~�j�}�b�v�t���[���̏���������
	InitMinimapframe();

	//�l���\���̔w�i�̏���������
	InitGround();

	//�l���\���̏���������
	InitPeople();

	//�X�L�b�v�m�F�̏���������
	InitTutorialSkip();

	//�t�B�[�o�[�v�b�V���̏���������
	InitFeverPush();

	//�h���e�N�X�`���̏���������
	InitDispatchTex();

	//�T�E���h�̍Đ�
	//PlaySound();

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

	//�C�x���g���̏I������
	UninitIventSign();

	//�~�j�}�b�v�̏I������
	UninitMinimap();

	//�A�C�R���̏I������
	UninitMiniIcon();

	//�G�̏I������
	UninitEnemy();

	//�v���C���[�̏I������
	UninitPlayer();

	//���_�̏I������
	UninitBuddy();

	//���f���̏I������
	UninitModel();

	//�h���^�[�Q�b�g�̏I������
	UninitDisPatch();

	//�h���V�����_�[�̏I������
	UninitDispatchPoint();

	//��ʔ����n�_�̏I������
	UninitManyPoint();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���h�[���̏I������
	UninitMeshDome();

	//�Ռ��g�̏I������
	UninitImpactWave();

	//�T���͈͂̏I������
	UninitSearchCircle();

	//���b�V���̏��̏I������
	UninitMeshField();

	//�^�C���T�C�N���̏I������
	UninitTimeCycle();

	//�t�B�[�o�[�Q�[�W�̏I������
	UninitFeverGauge();

	//�Q�O�̏I������
	UninitCrowd();

	//���u�̏I������
	UninitMob();

	//���邮��̏I������
	UnInitRotate();

	//�Ö��x�Q�[�W�̏I������
	UninitHypnosisGauge();

	//�������J�o�[�̏I������
	UninitCutepillow();

	//���̓Q�[�W�̏I������
	UninitPowerGage();

	//�C�x���g�֘A�̏I������
	UninitEventWindow();

	//��ʔ�����UI�I������
	UninitManyUI();

	//�x��������UI�I������
	UninitManyMORS();

	//�C�x���g�X�^���v�̏I������
	UninitEventStamp();

	//���S�C�̒e�̏I������
	UninitWatergun();

	//��̌`�̏I������
	UnInitSearchFan();

	//�Z���t�̏I������
	UninitSerif();

	//�`���[�g���A���E�B���h�E�̏I������
	UninitTutorialWindow();

	//�`���[�g���A���g�[�N�̏I������
	UninitTutorialTalk();

	//�J�b�g�C���̏I������
	UninitCutin();

	//�~�j�}�b�v�t���[���̏I������
	UninitMinimapframe();

	//�l���\���̔w�i�̏I������
	UninitGround();

	//�l���\���̏I������
	UninitPeople();

	//�X�L�b�v�m�F�̏I������
	UninitTutorialSkip();

	//�t�B�[�o�[�v�b�V���̏I������
	UninitFeverPush();

	//�h���e�N�X�`���̏I������
	UninitDispatchTex();
}

//==============================================================
//�`���[�g���A����ʂ̍X�V����
//==============================================================
void UpdateTutorial(void)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�J�b�g�C�����擾
	CUTIN *pCutin = GetCutin();

	//�`���[�g���A���g�[�N�̏��擾
	TutorialTalk *pTutorialTalk = GetTutorialTalk();

	if (g_bSkip == false)
	{
		//���f���̍X�V����
		UpdateModel();

		//2D�G�t�F�N�g�̍X�V����
		UpdateEffect_2D();

		//if (pEdit->bUse == false)
		{
			if (pCamera->nState != CAMERASTATE_MANYSPAWN && pCutin->bUse == false && pTutorialTalk->bUse == false)
			{//��ʔ����̃J��������Ȃ������� && �J�b�g�C��������Ȃ�������

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

				//�v���C���[�̍X�V����
				UpdateTutorialPlayer();

				//���_�̍X�V����
				UpdateBuddy();

				//�G�̍X�V����
				UpdateTutorialEnemy();

				//��̌`�̍X�V����
				UpdateSearchFan();

				//�^�C���T�C�N���̍X�V����
				UpdateTimeCycle();

				//���u�̍X�V����
				UpdateTutorialMob();

				//�Q�O�̍X�V����
				UpdateCrowd();

				//���邮��̍X�V����
				UpdateRotate();

				//�Ö��x�Q�[�W�̍X�V����
				UpdateHypnosisGauge();

				//�h���^�[�Q�b�g�̍X�V����
				UpdateTutorialDisPatch();

				//�h���V�����_�[�̍X�V����
				UpdateDispatchPoint();

				//�������J�o�[�̍X�V����
				UpdateCutepillow();

				//�e�̍X�V����
				UpdateShadow();

				//�Ռ��g�̍X�V����
				UpdateImpactWave();

				//�T���͈͂̍X�V����
				UpdateSearchCircle();

				//�~�j�}�b�v�̍X�V����
				UpdateMinimap();

				//�A�C�R���̍X�V����
				UpdateMiniIcon();

				//���̓Q�[�W�̍X�V����
				UpdatePowerGage();

				//�t�B�[�o�[�Q�[�W�̍X�V����
				UpdateFeverGauge();

				//�t�B�[�o�[�v�b�V���̍X�V����
				UpdateFeverPush();

				//���S�C�̒e�̍X�V����
				UpdateWatergun();

				//�Z���t�̍X�V����
				UpdateSerif();

				//�l���\���̍X�V����
				UpdateGround();

				//�l���\���̍X�V����
				UpdatePeople();
			}

			//�C�x���g�֘A�̍X�V����
			UpdateEventWindow();

			//��ʔ�����UI�X�V����
			UpdateManyUI();

			//�x��������UI�X�V����
			UpdateManyMORS();

			//�C�x���g�X�^���v�̍X�V����
			UpdateEventStamp();

			//��ʔ����n�_�̍X�V����
			UpdateManyPoint();

			//�C�x���g���̍X�V����
			UpdateIventSign();

			//�J�b�g�C���̍X�V����
			UpdateCutin();

			//�~�j�}�b�v�t���[���̍X�V����
			UpdateMinimapframe();

			//�`���[�g���A���E�B���h�E�̍X�V����
			UpdateTutorialWindow();

			//�`���[�g���A���g�[�N�̍X�V����
			UpdateTutorialTalk();

			//�h���e�N�X�`���̍X�V����
			UpdateDispatchTex();
		}
	}
	else if (g_bSkip == true)
	{
		//�X�L�b�v�m�F�̍X�V����
		UpdateTutorialSkip();
	}

	if ((GetKeyboardTrigger(DIK_BACKSPACE) == true || GetGamepadTrigger(BUTTON_START, 0)) &&
		GetFade() == FADE_NONE && g_aTutorial.nStep < TUTORIAL_STEP_END)
	{//P�L�[�������ꂽ��

		if (g_bSkip == false)
		{
			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_THEWORLD);
		}

		g_bSkip = (g_bSkip == false) ? true : false;		//�|�[�Y���j���[���g���Ă����Ԃɂ���

		//SetFade(MODE_GAME);
	}

	if (pCamera->nState != CAMERASTATE_MANYSPAWN && pCutin->bUse == false)
	{//��ʔ����̃J��������Ȃ������� && �J�b�g�C��������Ȃ�������

		switch (g_aTutorial.nStep)
		{
		case TUTORIAL_STEP_START:

			if (GetFade() == FADE_NONE)
			{//�S���ʈړ��I�������

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_MOVE:

			if (g_aTutorial.aControl.bUPMove == true &&
				g_aTutorial.aControl.bLMove == true &&
				g_aTutorial.aControl.bRMove == true &&
				g_aTutorial.aControl.bDWMove == true)
			{//�S���ʈړ��I�������

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_DISPATCH:

			if (g_aTutorial.aControl.bDispatchR == true &&
				g_aTutorial.aControl.bDispatchL == true)
			{//�����h���ł�����

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_RETURN:

			if (g_aTutorial.aControl.bReturnL == true &&
				g_aTutorial.aControl.bReturnR == true)
			{//�����A�ҏo������

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_HYPNOSIS:

			if (g_aTutorial.aControl.bSetMob == true &&
				g_aTutorial.aControl.bHypnosis == true)
			{//���u�ݒu�ƍÖ����ł�����

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_SENNOU:

			if (g_aTutorial.aControl.bCutePillow == true)
			{//���]�ł�����

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_USEFEVER:

			if (g_aTutorial.aControl.bFeverUse == true)
			{//�t�B�[�o�[��Ԃɓ�������

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_HYPNOSISFEVER:

			if (g_aTutorial.aControl.nCntHypnosisFever >= FEVERHYPNOSIS_NUM)
			{//�t�B�[�o�[�Ő��]�ł�����

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_MORS:

			if (g_aTutorial.aControl.bSetEnemy == true &&
				g_aTutorial.aControl.bReleaseCrowd == true)
			{//�G�ݒu�ƌQ�O�̐��]�����ł�����

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_END:

			if ((GetKeyboardTrigger(DIK_BACKSPACE) == true || GetGamepadTrigger(BUTTON_START, 0)) &&
				GetFade() == FADE_NONE)
			{//P�L�[�������ꂽ��

				SetFade(MODE_GAME);
			}
			break;
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

		//�h���^�[�Q�b�g�̕`�揈��
		DrawDisPatch();

		//�e�̕`�揈��
		DrawShadow();

		//��̌`�̕`�揈��
		DrawSearchFan();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//���_�̕`�揈��
		DrawBuddy();

		//�G�̕`�揈��
		DrawEnemy();

		//�Q�O�̕`�揈��
		DrawCrowd();

		//���u�̕`�揈��
		DrawMob();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();

		//���S�C�̒e�̕`�揈��
		DrawWatergun();

		//���邮��̕`�揈��
		DrawRotate();

		//�Ö��x�Q�[�W�̕`�揈��
		DrawHypnosisGauge();

		//�Z���t�̕`�揈��
		DrawSerif();

		//��ʔ����n�_�̕`�揈��
		DrawManyPoint();

		//�h���V�����_�[�̕`�揈��
		DrawDispatchPoint();

		//�T���͈͂̕`�揈��
		DrawSearchCircle();

		//�������J�o�[�̕`�揈��
		DrawCutepillow();

		//�Ռ��g�̕`�揈��
		DrawImpactWave();

		//���f���̕`�揈��
		DrawModel(1);

		//�~�j�}�b�v�t���[���̕`�揈��
		DrawMinimapframe();
	}

	if (nType == DRAWTYPE_MAP)
	{
		//�t�B�[���h�̕`��
		DrawMeshField(DRAWFIELD_TYPE_MAP);

		//�v���C���[�̕`��
		DrawMiniIcon();

		//�~�j�}�b�v�̕`��
		DrawMinimap();
	}

	if (nType == DRAWTYPE_UI)
	{
		//�^�C���T�C�N���̕`�揈��
		DrawTimeCycle();

		//���̓Q�[�W�̕`�揈��
		DrawPowerGage();

		//�t�B�[�o�[�Q�[�W�̕`�揈��
		DrawFeverGauge();

		//2D�G�t�F�N�g�̕`�揈��
		DrawEffect_2D();

		//�l���\���̔w�i�̕`�揈��
		DrawGround();

		//�l���\���̕`�揈��
		DrawPeople();

		//�t�B�[�o�[�v�b�V���̕`�揈��
		DrawFeverPush();

		//�h���e�N�X�`���̕`�揈��
		DrawDispatchTex();

		//�C�x���g�֘A�̕`�揈��
		DrawEventWindow();

		//��ʔ�����UI�`�揈��
		DrawManyUI();

		//�x��������UI�`�揈��
		DrawManyMORS();

		//�C�x���g�X�^���v�̕`�揈��
		DrawEventStamp();

		//�`���[�g���A���E�B���h�E�̕`�揈��
		DrawTutorialWindow();

		//�J�b�g�C���̕`�揈��
		DrawCutin();

		//�`���[�g���A���g�[�N�̕`�揈��
		DrawTutorialTalk();

		if (g_bSkip == true)
		{
			//�X�L�b�v�m�F�̕`�揈��
			DrawTutorialSkip();
		}

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
