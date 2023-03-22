//=============================================================================
//
// �J�������� [camera.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "debugproc.h"
#include "main.h"
#include "camera.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "tutorial.h"
#include "player.h"
#include "calculation.h"
#include "sound.h"

//�}�N��
#define MOVE			(2.5f)		//�ړ���
#define MAX_LENGTH		(20000.0f)	//�ő勗��
#define MIN_LENGTH		(10.0f)		//�ŏ�����
#define ROT_MOVE_MOUSE	(0.01f)		//��]�ړ���
#define ROT_MOVE_STICK	(0.0015f)		//��]�ړ���
#define ROT_MOVE		(0.025f)		//��]�ړ���
#define MIN_ROT			(-D3DX_PI * 0.49f)	//�J�����Œ�p
#define MAX_ROT			(D3DX_PI * 0.49f)	//�J�����Œ�p
#define BACKFOLLOW_TIME	(20)		//�w�ʕ␳�܂ł̎���
#define DIS_Y			(50.0f)
#define MANY_RADIUS	(POS_MANYPOINT + 430.0f)	//��ʔ���SE�̔��a

//�v���g�^�C�v�錾
void MoveCameraInput(int nCntCamera);
void MoveCameraStick(int nCntCamera);
void MoveCameraMouse(int nCntCamera);
void MoveCameraVR(int nCntCamera);
void MoveCameraV(int nCntCamera);
void MoveCameraR(int nCntCamera);
void MoveCameraDistance(int nCntCamera);
void SetCameraV(int nCntCamera);
void SetCameraR(int nCntCamera);
void SetCameraRot(int nCntCamera);
void MoveCameraFollow(int nCntCamera);
void UpdateManySpawnCamera(int nCntCamera);
//void LimitPosCamera(void);
//void CollisionModelCamera(void);
//void UpdateResultCamera(void);

//�O���[�o���ϐ��錾
Camera g_aCamera[CAMERATYPE_MAX];	//�J�����̏��

//==================================================================================
//�J�����̏���������
//==================================================================================
void InitCamera(void)
{
	
	switch (GetMode())
	{
	case MODE_LOGO:
		InitTitleCamera();
		break;

	case MODE_TITLE:
		InitTitleCamera();
		break;

	case MODE_GAME:
		InitGameCamera();
		break;

	case MODE_RESULT:
		InitResultCamera();
		break;

	case MODE_RANKING:
		InitRankingCamera();
		break;

	}

}

//==================================================================================
//�^�C�g���̃J��������������
//==================================================================================
void InitTitleCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < CAMERATYPE_MAX; nCntCamera++)
	{
		if (nCntCamera == CAMERATYPE_MAIN)
		{
			g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 200.0f, -0.0f);		//�����_
			g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 300.0f, g_aCamera[nCntCamera].posR.z - 200.0f);	//���_
			g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g��
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//���
			g_aCamera[nCntCamera].fDistance = 500.0f;	//����
			g_aCamera[nCntCamera].nMoveStep = 0;			//�i�K
			g_aCamera[nCntCamera].nCntStep = 0;			//�i�K�̃J�E���g

			g_aCamera[nCntCamera].bFollow = false;	//�Ǐ]ON
			g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����

			g_aCamera[nCntCamera].bBackFollow = false;		//�w�ʒǏ]
			g_aCamera[nCntCamera].nCntBackFollow = 0;		//�Ǐ]�J�E���g

			g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//�ڕW�̎��_
			g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//�ڕW�̒����_

			g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//�ڕW�̌���
			g_aCamera[nCntCamera].rotVDiff = 0.0f;			//�ڕW�̎��_�̍���

			g_aCamera[nCntCamera].viewport.X = 0;	//�`�悷���ʂ̍���X���W
			g_aCamera[nCntCamera].viewport.Y = 0;	//�`�悷���ʂ̍���Y���W
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//�`�悷���ʂ̕�
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//�`�悷���ʂ̍���

			//���_�̑������
			SetCameraV(nCntCamera);
		}
		
		if (nCntCamera == CAMERATYPE_UI)
		{
			g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 200.0f, 0.0f);		//�����_
			g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 300.0f, g_aCamera[nCntCamera].posR.z + -400.0f);	//���_
			g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g��
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//���
			g_aCamera[nCntCamera].fDistance = 1500.0f;	//����
			g_aCamera[nCntCamera].nMoveStep = 0;			//�i�K
			g_aCamera[nCntCamera].nCntStep = 0;			//�i�K�̃J�E���g

			g_aCamera[nCntCamera].bFollow = false;	//�Ǐ]ON
			g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����

			g_aCamera[nCntCamera].bBackFollow = false;		//�w�ʒǏ]
			g_aCamera[nCntCamera].nCntBackFollow = 0;		//�Ǐ]�J�E���g

			g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//�ڕW�̎��_
			g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//�ڕW�̒����_

			g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//�ڕW�̌���
			g_aCamera[nCntCamera].rotVDiff = 0.0f;			//�ڕW�̎��_�̍���

			g_aCamera[nCntCamera].viewport.X = 0;	//�`�悷���ʂ̍���X���W
			g_aCamera[nCntCamera].viewport.Y = 0;	//�`�悷���ʂ̍���Y���W
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//�`�悷���ʂ̕�
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//�`�悷���ʂ̍���

																	//���_�̑������
			SetCameraV(nCntCamera);
		}

		g_aCamera[nCntCamera].viewport.MinZ = 0.0f;
		g_aCamera[nCntCamera].viewport.MaxZ = 1.0f;
	}
}

//==================================================================================
//�����L���O�̃J��������������
//==================================================================================
void InitRankingCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < CAMERATYPE_MAX; nCntCamera++)
	{
		if (nCntCamera == CAMERATYPE_MAIN)
		{
			g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�����_
			g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 300.0f, g_aCamera[nCntCamera].posR.z + -800.0f);	//���_
			g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g��
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//���
			g_aCamera[nCntCamera].fDistance = 1500.0f;	//����
			g_aCamera[nCntCamera].nMoveStep = 0;			//�i�K
			g_aCamera[nCntCamera].nCntStep = 0;			//�i�K�̃J�E���g

			g_aCamera[nCntCamera].bFollow = false;	//�Ǐ]ON
			g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����

			g_aCamera[nCntCamera].bBackFollow = false;		//�w�ʒǏ]
			g_aCamera[nCntCamera].nCntBackFollow = 0;		//�Ǐ]�J�E���g

			g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//�ڕW�̎��_
			g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//�ڕW�̒����_

			g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//�ڕW�̌���
			g_aCamera[nCntCamera].rotVDiff = 0.0f;			//�ڕW�̎��_�̍���

			g_aCamera[nCntCamera].viewport.X = 0;	//�`�悷���ʂ̍���X���W
			g_aCamera[nCntCamera].viewport.Y = 0;	//�`�悷���ʂ̍���Y���W
			g_aCamera[nCntCamera].viewport.Width = (DWORD)(SCREEN_WIDTH * 0.7f);	//�`�悷���ʂ̕�
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//�`�悷���ʂ̍���
		}
		if (nCntCamera == CAMERATYPE_UI)
		{
			g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);		//�����_
			g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 300.0f, g_aCamera[nCntCamera].posR.z + -500.0f);	//���_
			g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g��
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//���
			g_aCamera[nCntCamera].fDistance = 1500.0f;	//����
			g_aCamera[nCntCamera].nMoveStep = 0;			//�i�K
			g_aCamera[nCntCamera].nCntStep = 0;			//�i�K�̃J�E���g

			g_aCamera[nCntCamera].bFollow = false;	//�Ǐ]ON
			g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����

			g_aCamera[nCntCamera].bBackFollow = false;		//�w�ʒǏ]
			g_aCamera[nCntCamera].nCntBackFollow = 0;		//�Ǐ]�J�E���g

			g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//�ڕW�̎��_
			g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//�ڕW�̒����_

			g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//�ڕW�̌���
			g_aCamera[nCntCamera].rotVDiff = 0.0f;			//�ڕW�̎��_�̍���

			g_aCamera[nCntCamera].viewport.X = 0;	//�`�悷���ʂ̍���X���W
			g_aCamera[nCntCamera].viewport.Y = 0;	//�`�悷���ʂ̍���Y���W
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//�`�悷���ʂ̕�
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//�`�悷���ʂ̍���
		}

		g_aCamera[nCntCamera].viewport.MinZ = 0.0f;
		g_aCamera[nCntCamera].viewport.MaxZ = 1.0f;
	}
}

//==================================================================================
//�Q�[���̃J��������������
//==================================================================================
void InitGameCamera(void)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	for (int nCntCamera = 0; nCntCamera < CAMERATYPE_MAX; nCntCamera++)
	{
		g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 50.0f, -500.0f);		//�����_
		g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 200.0f, g_aCamera[nCntCamera].posR.z + -400.0f);	//���_
		g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//������x�N�g��
		g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//���
		g_aCamera[nCntCamera].nMoveStep = 0;			//�i�K
		g_aCamera[nCntCamera].nCntStep = 0;			//�i�K�̃J�E���g

		g_aCamera[nCntCamera].fDistance = 800;	//����
		g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, -0.35f);	//����

		g_aCamera[nCntCamera].bBackFollow = false;		//�w�ʒǏ]
		g_aCamera[nCntCamera].nCntBackFollow = 0;		//�Ǐ]�J�E���g

		g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//�ڕW�̎��_
		g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//�ڕW�̒����_

		g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//�ڕW�̌���
		g_aCamera[nCntCamera].rotVDiff = 0.0f;			//�ڕW�̎��_�̍���
		g_aCamera[nCntCamera].bFollow = true;	//�Ǐ]ON

		//�J�������Ƃ̐ݒ�
		switch (nCntCamera)
		{
		case CAMERATYPE_MAIN:
			g_aCamera[nCntCamera].viewport.X = 0;	//�`�悷���ʂ̍���X���W
			g_aCamera[nCntCamera].viewport.Y = 0;	//�`�悷���ʂ̍���Y���W
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//�`�悷���ʂ̕�
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//�`�悷���ʂ̍���
			
			g_aCamera[nCntCamera].posV.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posV.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posV.y = DIS_Y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

			g_aCamera[nCntCamera].posV.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posV.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posV.y = DIS_Y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

			//�����_�̑������
			g_aCamera[nCntCamera].posR.x = g_aCamera[nCntCamera].posV.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posR.z = g_aCamera[nCntCamera].posV.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posR.y = g_aCamera[nCntCamera].posV.y + sinf(g_aCamera[nCntCamera].rot.z) * g_aCamera[nCntCamera].fDistance;

			//�����_�̈ʒu�X�V
			SetCameraR(nCntCamera);
		break;

		case CAMERATYPE_UI:
			g_aCamera[nCntCamera].viewport.X = 0;	//�`�悷���ʂ̍���X���W
			g_aCamera[nCntCamera].viewport.Y = 0;	//�`�悷���ʂ̍���Y���W
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//�`�悷���ʂ̕�
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//�`�悷���ʂ̍���
			break;
		}

		g_aCamera[nCntCamera].viewport.MinZ = 0.0f;
		g_aCamera[nCntCamera].viewport.MaxZ = 1.0f;
	}
}

//==================================================================================
//���U���g�J�����̏���������
//==================================================================================
void InitResultCamera(void)
{
	g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(0.0f, 214.0f, 408.0f);		//�����_
	g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(-4.5f, 161.0f, -569.0f);		//���_
	g_aCamera[CAMERATYPE_MAIN].viewport.X = 0;	//�`�悷���ʂ̍���X���W
	g_aCamera[CAMERATYPE_MAIN].viewport.Y = 0;	//�`�悷���ʂ̍���Y���W
	g_aCamera[CAMERATYPE_MAIN].viewport.Width = SCREEN_WIDTH;	//�`�悷���ʂ̕�
	g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	//�`�悷���ʂ̍���
	g_aCamera[CAMERATYPE_MAIN].viewport.MinZ = 0.0f;
	g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ = 1.0f;

}

//==================================================================================
//�J�����̏I������
//==================================================================================
void UninitCamera(void)
{

}

//==================================================================================
//�J�����̍X�V����
//==================================================================================
void UpdateCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < CAMERATYPE_MAX; nCntCamera++)
	{
		if (nCntCamera == CAMERATYPE_MAIN)
		{//���C���J�����̂�

			switch (GetMode())
			{
			case MODE_GAME:
				UpdateGameCamera(nCntCamera);
				break;

			case MODE_TUTORIAL:
				UpdateTutorialCamera(nCntCamera);
				break;

			case MODE_TITLE:
				//UpdateTitleCamera(nCntCamera);
				break;

			case MODE_RESULT:
				//�e�L�X�g�̕`��
				PrintDebugProc(
					"---------------- �J������� ----------------\n"
					"�y�����z[X�F%f Y�F%f Z�F%f]\n"
					"�y�����z[%f]\n"
					"�y���_�z[X�F%f Y�F%f Z�F%f]\n"
					"�y�����_�z[X�F%f Y�F%f Z�F%f]\n",
					g_aCamera[nCntCamera].rot.x, g_aCamera[nCntCamera].rot.y, g_aCamera[nCntCamera].rot.z,
					g_aCamera[nCntCamera].fDistance,
					g_aCamera[nCntCamera].posV.x, g_aCamera[nCntCamera].posV.y, g_aCamera[nCntCamera].posV.z,
					g_aCamera[nCntCamera].posR.x, g_aCamera[nCntCamera].posR.y, g_aCamera[nCntCamera].posR.z);

				//UpdateGameCamera(nCntCamera);
				break;

			case MODE_RANKING:
				//UpdateGameCamera(nCntCamera);
				break;
			}
		}
	}
}

//==================================================================================
//�`���[�g���A�����̍X�V����
//==================================================================================
void UpdateTutorialCamera(int nCntCamera)
{
	//if (pCutin->bUse == false && pTutorialTalk->bUse == false)
	{//��ʔ����̃J��������Ȃ������� && �J�b�g�C��������Ȃ�������

		UpdateGameCamera(nCntCamera);
	}
}

//==================================================================================
//�Q�[�����̍X�V
//==================================================================================
void UpdateGameCamera(int nCntCamera)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	if (g_aCamera[nCntCamera].nState == CAMERASTATE_NONE)
	{
		//���_�E�����_�ړ�
		MoveCameraFollow(nCntCamera);
		MoveCameraInput(nCntCamera);
		MoveCameraVR(nCntCamera);
		MoveCameraR(nCntCamera);
		MoveCameraV(nCntCamera);
		MoveCameraDistance(nCntCamera);
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F6) == true)
	{//F6�������ꂽ,�w�ʒǏ]�؂�ւ�

		g_aCamera[nCntCamera].bBackFollow = g_aCamera[nCntCamera].bBackFollow ? false : true;
	}
	if (GetKeyboardTrigger(DIK_F7) == true)
	{//F7�������ꂽ,�Ǐ]�؂�ւ�

		g_aCamera[nCntCamera].bFollow = g_aCamera[nCntCamera].bFollow ? false : true;
	}
#endif

	//�e�L�X�g�̕`��
	PrintDebugProc(
	"---------------- �J������� ----------------\n"
	"�y�����z[X�F%f Y�F%f Z�F%f]\n"
	"�y�����z[%f]\n"
	"�y���_�z[X�F%f Y�F%f Z�F%f]\n"
	"�y�����_�z[X�F%f Y�F%f Z�F%f]\n", 
		g_aCamera[nCntCamera].rot.x, g_aCamera[nCntCamera].rot.y, g_aCamera[nCntCamera].rot.z,
		g_aCamera[nCntCamera].fDistance, 
		g_aCamera[nCntCamera].posV.x, g_aCamera[nCntCamera].posV.y, g_aCamera[nCntCamera].posV.z,
		g_aCamera[nCntCamera].posR.x, g_aCamera[nCntCamera].posR.y, g_aCamera[nCntCamera].posR.z);
}

//==================================================================================
//�^�C�g�����̍X�V
//==================================================================================
void UpdateTitleCamera(int nCntCamera)
{

	if (g_aCamera[nCntCamera].nMoveStep == 0)
	{
		//��Ɍ�����
		g_aCamera[nCntCamera].rot.z += 0.04f;

		if (g_aCamera[nCntCamera].rot.z >= D3DX_PI * 0.4f)
		{//���������

			g_aCamera[nCntCamera].rot.z = D3DX_PI * 0.4f;
		}
	}

	if (g_aCamera[nCntCamera].nMoveStep == 1)
	{
		g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 80.0f, -200.0f);		//�����_
		g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 100.0f, g_aCamera[nCntCamera].posR.z + -400.0f);	//���_
		g_aCamera[nCntCamera].fDistance = 800;	//����

		//��Ɍ�����
		g_aCamera[nCntCamera].rot.z -= 0.04f;

		if (g_aCamera[nCntCamera].rot.z <= -0.1f)
		{//���������

			g_aCamera[nCntCamera].rot.z = -0.1f;
		}
	}

	//��]�̕␳
	SetCameraRot(nCntCamera);

	//�����_�̈ʒu�X�V
	SetCameraR(nCntCamera);
	SetCameraV(nCntCamera);
}

//==================================================================================
//���͋@��̃J�����ړ�
//==================================================================================
void MoveCameraInput(int nCntCamera)
{
	if ((GetMode() == MODE_GAME && GetGameState() == GAMESTATE_NONE && g_aCamera[nCntCamera].nState == CAMERASTATE_NONE) ||
		GetMode() == MODE_TUTORIAL)
	{
		//�}�E�X�ł̈ړ�����
		MoveCameraMouse(nCntCamera);

		//�X�e�B�b�N�ł̈ړ�����
		MoveCameraStick(nCntCamera);
	}
}

//==================================================================================
//�R���g���[���[�ł̈ړ�����
//==================================================================================
void MoveCameraStick(int nCntCamera)
{
	g_aCamera[nCntCamera].rot.y += GetStickMoveR(0).x * ROT_MOVE_STICK;
#if _DEBUG
	g_aCamera[nCntCamera].rot.z += GetStickMoveR(0).y * ROT_MOVE_STICK;
#endif

	//��]�̕␳
	SetCameraRot(nCntCamera);

	//���_�̑������
	SetCameraV(nCntCamera);
}

//==================================================================================
//�}�E�X�ł̈ړ�����
//==================================================================================
void MoveCameraMouse(int nCntCamera)
{

	#if _DEBUG

	#else NDEBUG

	#endif

	if (GetMousePress(MOUSE_BUTTON_LEFT) == true &&
		GetMousePress(MOUSE_BUTTON_RIGHT) == true)
	{//���E��������

#if _DEBUG
		g_aCamera[nCntCamera].move.x += (GetMouseMove().x * sinf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE) -
			(GetMouseMove().y * cosf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE);

		g_aCamera[nCntCamera].move.z += (GetMouseMove().x * cosf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE) +
			(GetMouseMove().y * sinf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE);
#endif

		//�ړ��ʕ␳
		MoveCameraVR(nCntCamera);

		//��]�̕␳
		SetCameraRot(nCntCamera);

		//�����_�ݒ�
		SetCameraR(nCntCamera);
	}
	else if (GetMousePress(MOUSE_BUTTON_LEFT) == true)
	{//���N���b�N���Ă�Ƃ�,���_��]

		g_aCamera[nCntCamera].rot.y += GetMouseMove().x * ROT_MOVE_MOUSE;

#if _DEBUG

		g_aCamera[nCntCamera].rot.z += GetMouseMove().y * ROT_MOVE_MOUSE;	
#endif

		//��]�̕␳
		SetCameraRot(nCntCamera);

		//���_�̑������
		SetCameraV(nCntCamera);

	}
	else if (GetMousePress(MOUSE_BUTTON_RIGHT) == true)
	{//�E�N���b�N���Ă�Ƃ�,�����_��]

		g_aCamera[nCntCamera].rot.y += GetMouseMove().x * ROT_MOVE_MOUSE;

#if _DEBUG
		g_aCamera[nCntCamera].rot.z += GetMouseMove().y * ROT_MOVE_MOUSE;
#endif

		//��]�̕␳
		SetCameraRot(nCntCamera);

		//�����_�̈ʒu�X�V
		SetCameraR(nCntCamera);
	}

	//�}�E�X�z�C�[���ŋ�������
	g_aCamera[nCntCamera].fDistance += GetMouseMove().z * (MOVE * 0.3f);
	MoveCameraDistance(nCntCamera);

	//���_�̑������
	SetCameraV(nCntCamera);
}

//==================================================================================
//�J�����̎��_�ړ�
//==================================================================================
void MoveCameraV(int nCntCamera)
{

	if ((GetMode() == MODE_GAME && GetGameState() == GAMESTATE_NONE && g_aCamera[nCntCamera].nState == CAMERASTATE_NONE) ||
		GetMode() == MODE_TUTORIAL)
	{

#ifdef _DEBUG
		//���_�ړ�
		if (GetKeyboardPress(DIK_Y) == true)
		{//Y�L�[�������ꂽ,���_��ړ�

			g_aCamera[nCntCamera].rot.z -= ROT_MOVE;
		}
		else if (GetKeyboardPress(DIK_N) == true)
		{//N�L�[�������ꂽ,���_���ړ�

			g_aCamera[nCntCamera].rot.z += ROT_MOVE;
		}
#endif

		if (GetKeyboardPress(DIK_Z) == true/* || GetGamepadPress(BUTTON_RB, 0)*/)
		{//Z�L�[�������ꂽ

			g_aCamera[nCntCamera].rot.y += ROT_MOVE;
			//pPlayer->aTutorial.bLTurn = true;
		}
		if (GetKeyboardPress(DIK_C) == true/* || GetGamepadPress(BUTTON_LB, 0)*/)
		{//C�L�[�������ꂽ

			g_aCamera[nCntCamera].rot.y -= ROT_MOVE;
			//pPlayer->aTutorial.bRTurn = true;

		}
	}

	//��]�̕␳
	SetCameraRot(nCntCamera);

	//���_�̑������
	SetCameraV(nCntCamera);

}

//==================================================================================
//�J�����̒����_�ړ�
//==================================================================================
void MoveCameraR(int nCntCamera)
{
	if ((GetMode() == MODE_GAME && GetGameState() == GAMESTATE_NONE && g_aCamera[nCntCamera].nState == CAMERASTATE_NONE) ||
		GetMode() == MODE_TUTORIAL)
	{
		//����
		if (GetKeyboardPress(DIK_Q) == true)
		{//Q�L�[�������ꂽ,������


			g_aCamera[nCntCamera].rot.y -= ROT_MOVE;

		}
		else if (GetKeyboardPress(DIK_E) == true)
		{//E�L�[�������ꂽ,������

			g_aCamera[nCntCamera].rot.y += ROT_MOVE;
		}

#ifdef _DEBUG
		//�㏸���~
		if (GetKeyboardPress(DIK_T) == true)
		{//T�L�[�������ꂽ

			g_aCamera[nCntCamera].rot.z += ROT_MOVE;

		}
		else if (GetKeyboardPress(DIK_B) == true)
		{//B�L�[�������ꂽ

			g_aCamera[nCntCamera].rot.z -= ROT_MOVE;

		}
#endif
	}

	//��]�̕␳
	SetCameraRot(nCntCamera);

	//�����_�̈ʒu�X�V
	SetCameraR(nCntCamera);
}

//==================================================================================
//�J�����̎��_�E�����_�ړ�
//==================================================================================
void MoveCameraVR(int nCntCamera)
{

	//if (g_aCamera[nCntCamera].bFollow == false)
	//{//�Ǐ]��OFF�̎�����

	//	if (GetKeyboardPress(DIK_A) == true || XGetStickPressR(BUTTON_LX, 0) < 0)
	//	{//A�L�[�������ꂽ,���_���ړ�

	//		if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
	//		{//A+W,����ړ�

	//			g_aCamera[nCntCamera].move.x += sinf(-D3DX_PI * MOVE_LRDW + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(-D3DX_PI * MOVE_LRDW + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}
	//		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
	//		{//A+S,�����ړ�

	//			g_aCamera[nCntCamera].move.x += sinf(-D3DX_PI * MOVE_LRUP + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(-D3DX_PI * MOVE_LRUP + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}
	//		else
	//		{//A,���ړ�

	//			g_aCamera[nCntCamera].move.x += sinf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}

	//	}
	//	else if (GetKeyboardPress(DIK_D) == true || XGetStickPressR(BUTTON_LX, 0) > 0)
	//	{//D�L�[�������ꂽ,���_�E�ړ�

	//		if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
	//		{//D+W,�E��ړ�

	//			g_aCamera[nCntCamera].move.x += sinf(D3DX_PI * MOVE_LRDW + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(D3DX_PI * MOVE_LRDW + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}
	//		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
	//		{//D+S,�E���ړ�

	//			g_aCamera[nCntCamera].move.x += sinf(D3DX_PI * MOVE_LRUP + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(D3DX_PI * MOVE_LRUP + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}
	//		else
	//		{//D,�E�ړ�

	//			g_aCamera[nCntCamera].move.x += sinf(D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}

	//	}
	//	else if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
	//	{//W�������ꂽ�A���ړ�

	//		g_aCamera[nCntCamera].move.x += sinf(g_aCamera[nCntCamera].rot.y) * MOVE;
	//		g_aCamera[nCntCamera].move.z += cosf(g_aCamera[nCntCamera].rot.y) * MOVE;
	//	}
	//	else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
	//	{//S�������ꂽ�A��O�ړ�

	//		g_aCamera[nCntCamera].move.x += sinf(D3DX_PI + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		g_aCamera[nCntCamera].move.z += cosf(D3DX_PI + g_aCamera[nCntCamera].rot.y) * MOVE;
	//	}

	//}

	//�ړ��ʕ������Z
	g_aCamera[nCntCamera].posV.x += g_aCamera[nCntCamera].move.x;
	g_aCamera[nCntCamera].posV.z += g_aCamera[nCntCamera].move.z;

	//�ړ��ʂ����Z�b�g
	g_aCamera[nCntCamera].move.x = 0.0f;
	g_aCamera[nCntCamera].move.z = 0.0f;

	//�����_�̈ʒu�X�V
	SetCameraR(nCntCamera);
}

//==================================================================================
// ���_�E�����_�ԋ���
//==================================================================================
void MoveCameraDistance(int nCntCamera)
{

	//���E�l�␳
	if (g_aCamera[nCntCamera].fDistance >= MAX_LENGTH)
	{
		g_aCamera[nCntCamera].fDistance = MAX_LENGTH;
	}
	else if (g_aCamera[nCntCamera].fDistance <= MIN_LENGTH)
	{
		g_aCamera[nCntCamera].fDistance = MIN_LENGTH;
	}

}

//==================================================================================
//�J�����̎��_�������
//==================================================================================
void SetCameraV(int nCntCamera)
{

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	if (g_aCamera[nCntCamera].bFollow == false)
	{//�Ǐ]���Ȃ��Ƃ�

		//���_�̑������
		g_aCamera[nCntCamera].posV.x = g_aCamera[nCntCamera].posR.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.z = g_aCamera[nCntCamera].posR.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.y = g_aCamera[nCntCamera].posR.y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;
	}
	else if(g_aCamera[nCntCamera].bFollow == true)
	{//�Ǐ]ON

		float fYcamera = 0.0f;

		//���_�̑������
		g_aCamera[nCntCamera].posVDest.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posVDest.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posVDest.y = fYcamera + DIS_Y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

		//�␳����
		g_aCamera[nCntCamera].posV += (g_aCamera[nCntCamera].posVDest - g_aCamera[nCntCamera].posV) * 0.12f;
	}

}

//==================================================================================
//�J�����̒����_�������
//==================================================================================
void SetCameraR(int nCntCamera)
{

	if (g_aCamera[nCntCamera].bFollow == false)
	{//�Ǐ]���Ȃ��Ƃ�

		//�����_�̑������
		g_aCamera[nCntCamera].posR.x = g_aCamera[nCntCamera].posV.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posR.z = g_aCamera[nCntCamera].posV.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posR.y = g_aCamera[nCntCamera].posV.y + sinf(g_aCamera[nCntCamera].rot.z) * g_aCamera[nCntCamera].fDistance;
	}
	else
	{//�Ǐ]ON

		//�����_�̑������
		g_aCamera[nCntCamera].posRDest.x = g_aCamera[nCntCamera].posV.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posRDest.z = g_aCamera[nCntCamera].posV.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posRDest.y = g_aCamera[nCntCamera].posV.y + sinf(g_aCamera[nCntCamera].rot.z) * g_aCamera[nCntCamera].fDistance;

		//�␳����
		g_aCamera[nCntCamera].posR += (g_aCamera[nCntCamera].posRDest - g_aCamera[nCntCamera].posR) * 0.08f;

	}

}

//==================================================================================
//�J�����̉�]�␳
//==================================================================================
void SetCameraRot(int nCntCamera)
{
	//Y���̕␳
	if (g_aCamera[nCntCamera].rot.y > D3DX_PI)
	{
		g_aCamera[nCntCamera].rot.y += -D3DX_PI * 2.0f;
	}
	else if (g_aCamera[nCntCamera].rot.y < -D3DX_PI)
	{
		g_aCamera[nCntCamera].rot.y += D3DX_PI * 2.0f;
	}

	//Z���̕␳
	if (g_aCamera[nCntCamera].rot.z > D3DX_PI)
	{
		g_aCamera[nCntCamera].rot.z += -D3DX_PI * 2.0f;
	}
	else if (g_aCamera[nCntCamera].rot.z < -D3DX_PI)
	{
		g_aCamera[nCntCamera].rot.z += D3DX_PI * 2.0f;
	}

	//Z���̌Œ�
	if (g_aCamera[nCntCamera].rot.z > MAX_ROT)
	{
		g_aCamera[nCntCamera].rot.z = MAX_ROT;
	}
	else if (g_aCamera[nCntCamera].rot.z < MIN_ROT)
	{
		g_aCamera[nCntCamera].rot.z = MIN_ROT;
	}

}

//==================================================================================
//�J�����̔w�ʎ����Ǐ]
//==================================================================================
void MoveCameraFollow(int nCntCamera)
{

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	if (g_aCamera[nCntCamera].bBackFollow == true)
	{//�Ǐ]����Ƃ�

		g_aCamera[nCntCamera].rotVDest.y = (D3DX_PI + pPlayer->rot.y);

		//if (pPlayer->move.x > 1.0f || pPlayer->move.x < -1.0f || pPlayer->move.z > 1.0f || pPlayer->move.z < -1.0f)
		//{//�ړ����Ă�Ƃ�

		//	g_aCamera[nCntCamera].nCntBackFollow = 0;	//�J�E���g���Z�b�g
		//}
		//else
		//{
		//	g_aCamera[nCntCamera].nCntBackFollow++;	//�J�E���g���Z
		//}

		if (g_aCamera[nCntCamera].nCntBackFollow >= BACKFOLLOW_TIME)
		{//�w�ʕ␳�̃J�E���g���K��l��������

			g_aCamera[nCntCamera].nCntBackFollow = 0;

			//�␳����
			g_aCamera[nCntCamera].rotVDiff = (g_aCamera[nCntCamera].rotVDest.y - g_aCamera[nCntCamera].rot.y);

			if (g_aCamera[nCntCamera].rotVDiff > D3DX_PI)
			{
				g_aCamera[nCntCamera].rotVDiff += (-D3DX_PI * 2.0f);
			}
			else if (g_aCamera[nCntCamera].rotVDiff < -D3DX_PI)
			{
				g_aCamera[nCntCamera].rotVDiff += (D3DX_PI * 2.0f);
			}

			//�����ŕ␳����
			g_aCamera[nCntCamera].rot.y += g_aCamera[nCntCamera].rotVDiff * 0.015f;

			if (g_aCamera[nCntCamera].rot.y > D3DX_PI)
			{
				g_aCamera[nCntCamera].rot.y += (-D3DX_PI * 2.0f);
			}
			else if (g_aCamera[nCntCamera].rot.y < -D3DX_PI)
			{
				g_aCamera[nCntCamera].rot.y += (D3DX_PI * 2.0f);
			}

			//��]�̕␳
			SetCameraRot(nCntCamera);

			////�����_�̈ʒu�X�V
			//SetCameraR(nCntCamera);

			////���_�̑������
			//SetCameraV(nCntCamera);

			////���_�̑������
			//g_aCamera[nCntCamera].posV.x = g_aCamera[nCntCamera].posR.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			//g_aCamera[nCntCamera].posV.z = g_aCamera[nCntCamera].posR.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			//g_aCamera[nCntCamera].posV.y = g_aCamera[nCntCamera].posR.y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

		}
	}
}

//==================================================================================
//�J�����̐ݒ菈��
//==================================================================================
void SetCamera(int nIdx)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�r���[�|�[�g�̐ݒ�
	pDevice->SetViewport(&g_aCamera[nIdx].viewport);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera[nIdx].mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&g_aCamera[nIdx].mtxProjection,
								D3DXToRadian(45.0f),	//����p
								(float)g_aCamera[nIdx].viewport.Width / (float)g_aCamera[nIdx].viewport.Height,	//�A�X�y�N�g��
								10.0f,		//���s���̐���
								20000.0f);	//���s���̐���

	//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION, &g_aCamera[nIdx].mtxProjection);

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aCamera[nIdx].mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&g_aCamera[nIdx].mtxView,
		&g_aCamera[nIdx].posV,	//���_
		&g_aCamera[nIdx].posR,	//�����_
		&g_aCamera[nIdx].vecU);//������x�N�g��

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &g_aCamera[nIdx].mtxView);

}

//==================================================================================
//�J�����̏��擾
//==================================================================================
Camera *GetCamera(void)
{
	return &g_aCamera[0];
}