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
#include "enemy.h"
#include "player.h"
#include "edit.h"
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
#include "serif.h"
#include "numberPeople.h"
#include "ground.h"
#include "sound.h"
#include "fevergauge.h"
#include "dispatchpoint.h"
#include "updown_fram.h"
#include "start_talk.h"
#include "cutin.h"
#include "comment.h"
#include "realtime_ranking.h"
#include "nofade.h"
#include "iventradar.h"
#include "mapshape.h"
#include "minimapframe.h"
#include "endmessage.h"
#include "resultmessage.h"
#include "flag.h"
#include "endcountdown.h"
#include "animation.h"
#include "fenish.h"
#include "feverpush.h"
#include "dispatch_texture.h"

//�v���g�^�C�v�錾
void DebugEdit(void);

//�O���[�o���ϐ�
GAMESTATE g_gameState = GAMESTATE_START;		//�Q�[���̏��
TARGETNOP g_nTargetNoP;                     //�ڕW�̐l���̏��
int g_nCounterGameState = 0;				//��ԊǗ��J�E���^�[
bool g_bPause = false;						//�|�[�Y��Ԃ�ON/OFF
bool g_bStart = true;						//�X�^�[�g���g�p���Ă��邩

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

	//���̏���������
	InitFlag();

	//���b�V���̏��̏���������
	InitMeshField();

	//���b�V���̕ǂ̏���������
	//InitMeshWall();

	//���b�V���h�[���̏���������
	InitMeshDome();

	////�O�Ղ̏���������
	//InitMeshOrbit();

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

	//�G�f�B�b�g�̏�����
	InitEdit();

	//�C�x���g���̏���������
	InitIventSign();

	//�^�C���T�C�N���̏���������
	InitTimeCycle();

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

	//�U�Q�[�W
	//InitNiseGauge();

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

	//�l���\���̏���������
	InitPeople();

	//�t�B�[�o�[�Q�[�W�̏���������
	InitFeverGauge();

	//�l���\���̔w�i�̏�����
	InitGround();

	//�㉺�̘g�̏���������
	InitUpDownFram();

	//�ŏ��̉�b�̏���������
	InitStartTalk();

	//�Q�[���̃J��������������
	InitGameCamera();

	//�J�b�g�C���̏���������
	InitCutin();

	//�R�����g�̏���������
	InitCommnet();

	//���A���^�C�������L���O�̏���������
	InitRealTimeRanking();

	//���[�_�[�̏���������
	InitIventRadar();

	//�~�j�}�b�v�t���[���̏���������
	InitMinimapframe();

	//�Q�[���I���O���b�Z�[�W�̏���������
	InitEndmessage();

	//���ʂ̃��b�Z�[�W�̏���������
	InitResultMessage();

	//�}�b�v�̌`����������
	//InitMapShape();

	//�Q�[���I��5�b�O�J�E���g�̏���������
	InitEndCountDown();

	//�A�j���[�V�����̏���������
	InitAnimation();

	//�I���̏���������
	InitFinish();

	//�t�B�[�o�[�v�b�V���̏���������
	InitFeverPush();

	//�h���e�N�X�`���̏���������
	InitDispatchTex();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM_GAME);

	g_gameState = GAMESTATE_START;		//�ʏ��Ԃɐݒ�
	g_nTargetNoP.nCntPeople = TARGETHITO;      //���̖ڕW�̐l��
	g_nCounterGameState = 0;
	g_bStart = true;
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

	//���b�V���̕ǂ̏I������
	//UninitMeshWall();

	//���b�V���̏��̏I������
	UninitMeshField();

	////�O�Ղ̏I������
	//UninitMeshOrbit();

	//�^�C���T�C�N���̏I������
	UninitTimeCycle();

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

	//�U�Q�[�W
	//UninitNiseGauge();

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

	//�Z���t�̏I������
	UninitSerif();

	//��̌`�̏I������
	UnInitSearchFan();

	//�l���\���̏I������
	UninitPeople();

	//�t�B�[�o�[�Q�[�W�̏I������
	UninitFeverGauge();

	//�l���\���̔w�i�̏I������
	UninitGround();

	//�㉺�̘g�̏I������
	UninitUpDownFram();

	//�ŏ��̉�b�̏I������
	UninitStartTalk();

	//�J�b�g�C���̏I������
	UninitCutin();

	//�R�����g�̏I������
	UninitCommnet();

	//���A���^�C�������L���O�̏I������
	UninitRealTimeRanking();

	//���[�_�[�̏I������
	UninitIventRadar();

	//�~�j�}�b�v�t���[���̏I������
	UninitMinimapframe();

	//�Q�[���I���O���b�Z�[�W�̏I������
	UninitEndmessage();

	//���ʂ̃��b�Z�[�W�̏I������
	UninitResultMessage();

	//�}�b�v�̌`�I������
	//UninitMapShape();

	//���̏I������
	UnInitFlag();

	//�Q�[���I��5�b�O�J�E���g�̏I������
	UninitEndCountDown();

	//�A�j���[�V�����̏I������
	UninitAnimation();

	//�I���̏I������
	UninitFinish();

	//�t�B�[�o�[�v�b�V���̏I������
	UninitFeverPush();

	//�h���e�N�X�`���̏I������
	UninitDispatchTex();

}

//==============================================================
//�Q�[����ʂ̍X�V����
//==============================================================
void UpdateGame(void)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�G�f�B�b�g�̏��擾
	Edit *pEdit = GetEdit();

	//�J�b�g�C�����擾
	CUTIN *pCutin = GetCutin();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

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
			if (pCamera->nState != CAMERASTATE_MANYSPAWN && pCutin->bUse == false)
			{//��ʔ����̃J��������Ȃ������� && �J�b�g�C��������Ȃ�������

				//�G�t�F�N�g�̍X�V����
				UpdateEffect();

				//�p�[�e�B�N���̍X�V����
				UpdateParticle();

				//���b�V���̏��̍X�V����
				UpdateMeshField();

				//�A�j���[�V�����̍X�V����
				UpdateAnimation();

				//���b�V���̕ǂ̍X�V����
				//UpdateMeshWall();

				//���b�V���V�����_�[�̍X�V����
				UpdateMeshCylinder();

				//���b�V���h�[���̍X�V����
				UpdateMeshDome();

				//�v���C���[�̍X�V����
				UpdatePlayer();

				//���_�̍X�V����
				UpdateBuddy();

				//�G�̍X�V����
				UpdateEnemy();
				
				//��̌`�̍X�V����
				UpdateSearchFan();

				//�^�C���T�C�N���̍X�V����
				UpdateTimeCycle();

				//���u�̍X�V����
				UpdateMob();

				//�Q�O�̍X�V����
				UpdateCrowd();

				//���邮��̍X�V����
				UpdateRotate();

				//�Ö��x�Q�[�W�̍X�V����
				UpdateHypnosisGauge();

				//�h���^�[�Q�b�g�̍X�V����
				UpdateDisPatch();

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

				////�O�Ղ̍X�V����
				//UpdateMeshOrbit();

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

				//�l���\���̍X�V����
				UpdatePeople();

				//�l���\���̔w�i�̍X�V����
				UpdateGround();

				//�U�Q�[�W
				//UpdateNiseGauge();

				//���S�C�̒e�̍X�V����
				UpdateWatergun();

				//�Z���t�̍X�V����
				UpdateSerif();

				//���̍X�V����
				UpdateFlag();

				//�Q�[���I��5�b�O�J�E���g�̍X�V����
				UpdateEndCountDown();
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

			//�㉺�̘g�̍X�V����
			UpdateUpDownFram();

			//�ŏ��̉�b�̍X�V����
			UpdateStartTalk();

			//�J�b�g�C���̍X�V����
			UpdateCutin();

			//�R�����g�̍X�V����
			UpdateCommnet();

			//���A���^�C�������L���O�̍X�V����
			UpdateRealTimeRanking();

			//���[�_�[�̍X�V����
			UpdateIventRadar();

			//�~�j�}�b�v�t���[���̍X�V����
			UpdateMinimapframe();

			//�Q�[���I���O���b�Z�[�W�̍X�V����
			UpdateEndmessage();

			//���ʂ̃��b�Z�[�W�̍X�V����
			UpdateResultMessage();

			//�h���e�N�X�`���̍X�V����
			UpdateDispatchTex();

			//�I���̍X�V����
			UpdateFinish();

			//�}�b�v�̌`�X�V����
			//UpdateMapShape();

		}
	}
	else if (g_bPause == true)
	{//�|�[�Y�{�^����������Ă���Ƃ�

		UpdatePause();		//�|�[�Y���j���[��\������
	}

	if (GetGameState() != GAMESTATE_START && GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_AFTEREND)
	{
		if ((GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(BUTTON_START, 0)) &&
			GetFade() == FADE_NONE)
		{//P�L�[�������ꂽ��

			if (g_bPause == false)
			{
				//�T�E���h�Đ�
				PlaySound(SOUND_LABEL_SE_THEWORLD);
			}

			g_bPause = (g_bPause == false) ? true : false;		//�|�[�Y���j���[���g���Ă����Ԃɂ���
		}
	}

	//�Q�O���擾
	CrowdInfo *pCrowdInfo = GetCrowdInfo();
	
	switch (g_gameState)
	{
	case GAMESTATE_BEFOREEND:

		if (g_nCounterGameState <= 0)
		{
			//�ʒu�ݒ�
			SetGameEnd();
		}

		g_nCounterGameState++;

		if (g_nCounterGameState >= 60 * 3)
		{//3�b�ȍ~

			//�J�����Ǐ]OFF
			pCamera->bFollow = false;

			//���ʂ̃��b�Z�[�W�ݒ�
			if (pPlayer->pos.z <= pCamera->posV.z * 0.5f &&
				GetResultMessage()->bUse == false)
			{
				if (TARGETHITO <= pCrowdInfo->nCrowdValue)
				{//�ړI�̐���������������

					//����
					SetResultMessage(RESULTTYPE_CLEAR);
				}
				else
				{
					//���s
					SetResultMessage(RESULTTYPE_FAILED);
				}
			}

			if (pPlayer->pos.z <= pCamera->posV.z)
			{
				SetGameState(GAMESTATE_END, 0);
			}
		}

		break;

	case GAMESTATE_END:							//�I�����
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0)
		{

			//���[�h�ݒ�(���U���g��ʂɈڍs)
			SetFade(MODE_RESULT);				//�t�F�[�h�A�E�g
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

		//���b�V���̏��̕`�揈��
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//�A�j���[�V�����̕`�揈��
		DrawAnimation();

		//���b�V���̕ǂ̕`�揈��
		//DrawMeshWall();

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���h�[���̕`�揈��
		DrawMeshDome();

		//�O�Ղ̕`�揈��
		//DrawMeshOrbit();

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

		//���̕`�揈��
		DrawFlag();

		//�G�f�B�b�g�̕`�揈��
		DrawEdit();

		//��ʔ����n�_�̕`�揈��
		DrawManyPoint();

		//�h���V�����_�[�̕`�揈��
		DrawDispatchPoint();

		//�T���͈͂̕`�揈��
		DrawSearchCircle();

		//�������J�o�[�̕`�揈��
		DrawCutepillow();

		////�X�R�A�̕`�揈��
		//DrawScore();

		//�G�f�B�b�g�̕`�揈��
		DebugEdit();

		//�Ռ��g�̕`�揈��
		DrawImpactWave();

		//���f���̕`�揈��
		DrawModel(1);

	}

	if (nType == DRAWTYPE_UI)
	{

		//�~�j�}�b�v�t���[���̕`�揈��
		DrawMinimapframe();

		if (g_gameState != GAMESTATE_START && g_gameState != GAMESTATE_BEFOREEND && g_gameState != GAMESTATE_END)
		{
			//�^�C���T�C�N���̕`�揈��
			DrawTimeCycle();

			//���̓Q�[�W�̕`�揈��
			DrawPowerGage();

			//�t�B�[�o�[�Q�[�W�̕`�揈��
			DrawFeverGauge();

			//2D�G�t�F�N�g�̕`�揈��
			DrawEffect_2D();

			//�t�B�[�o�[�v�b�V���̕`�揈��
			DrawFeverPush();

			//�h���e�N�X�`���̕`�揈��
			DrawDispatchTex();

			//�R�����g�̕`�揈��
			DrawCommnet();

			//�l���\���̔w�i�̕`�揈��
			DrawGround();

			//�l���\���̕`�揈��
			DrawPeople();

			//�C�x���g�֘A�̕`�揈��
			DrawEventWindow();

			//�C�x���g�X�^���v�̕`�揈��
			DrawEventStamp();

			//���A���^�C�������L���O�̕`�揈��
			DrawRealTimeRanking();

			//��ʔ�����UI�`�揈��
			DrawManyUI();

			//�x��������UI�`�揈��
			DrawManyMORS();

			//�Q�[���I���O���b�Z�[�W�̕`�揈��
			DrawEndmessage();

			//�Q�[���I��5�b�O�J�E���g�̕`�揈��
			DrawEndCountDown();

			//�I���̕`�揈��
			DrawFinish();

		}

		//�J�b�g�C���̕`�揈��
		DrawCutin();

		//�㉺�̘g�̕`�揈��
		DrawUpDownFram();

		//���ʂ̃��b�Z�[�W�̕`�揈��
		DrawResultMessage();

		//�ŏ��̉�b�̕`�揈��
		DrawStartTalk();

		if (g_bPause == true)
		{//�|�[�Y���j���[���g���Ă���Ƃ�

			//�|�[�Y��ʂ�\������
			DrawPause();
		}
	}

	if (nType == DRAWTYPE_MAP)
	{
		//�t�B�[���h�̕`�揈��
		DrawMeshField(DRAWFIELD_TYPE_MAP);

		//�v���C���[�̕`�揈��
		DrawMiniIcon();

		//�~�j�}�b�v�̕`�揈��
		DrawMinimap();

		////�}�b�v�̌`�`�揈��
		//DrawMapShape();
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
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

TARGETNOP *GetnTargetNop(void)
{
	return &g_nTargetNoP;
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
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();

	//�G�̏��擾
	Enemy *pEnemy = GetEnemy();

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//��̓����蔻��̏����擾
	SearchFan *pSearchFan = GetSearchFan();

	//�I��UI�̏��擾
	FENISH *pFenish = GetFinish();

	//�J�����̈ʒu�ݒ�
	pCamera->rot = D3DXVECTOR3(0.0f, 0.0f, -0.35f);
	pCamera->fDistance = 800;	//����

	//�v���C���[�̈ʒu�ݒ�
	pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetMotisonPlayer(PLAYERMOTION_DEF);

	pFenish->bUse = false;

	//���_�̈ʒu�ݒ�
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++, pBuddy++)
	{
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:
			pBuddy->pos = D3DXVECTOR3(pPlayer->pos.x + sinf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			pBuddy->nState = BUDDYSTATE_NONE;
			break;

		case BUDDYTYPE_DEPPA:
			pBuddy->pos = D3DXVECTOR3(pPlayer->pos.x + sinf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			pBuddy->nState = BUDDYSTATE_NONE;
			break;
		}

		SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
	}

	pBuddy -= MAX_BUDDY;

	int nCntSet = 0;

	//�G�̈ʒu�ݒ�
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bMoreSpawn == true && nCntSet < 3)
		{//�g�p���Ă�����

			if (nCntSet == 0)
			{
				pEnemy->pos = D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z + 300.0f);	//�ʒu�ݒ�
				pSearchFan[pEnemy->nIdxSearchFan].bUse = false;
			}
			else if (nCntSet == 1)
			{
				pEnemy->pos = D3DXVECTOR3(pBuddy[BUDDYTYPE_HYOROGARI].pos.x, 0.0f, pBuddy[BUDDYTYPE_HYOROGARI].pos.z + 300.0f);		//�ʒu�ݒ�
				pSearchFan[pEnemy->nIdxSearchFan].bUse = false;
			}
			else if (nCntSet == 2)
			{
				pEnemy->pos = D3DXVECTOR3(pBuddy[BUDDYTYPE_DEPPA].pos.x, 0.0f, pBuddy[BUDDYTYPE_DEPPA].pos.z + 300.0f);		//�ʒu�ݒ�
				pSearchFan[pEnemy->nIdxSearchFan].bUse = false;
			}

			nCntSet++;

		}
	}

	//�g�ړ�
	SetUpDownFram();

}