//=============================================================================
//
// ���_���� [buddy.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "buddy.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "meshwall.h"
#include "game.h"
#include "edit.h"
#include "impactwave.h"
#include "calculation.h"
#include "hypnosis_gauge.h"
#include "rotate.h"
#include "crowd.h"
#include "shadow.h"
#include "cutepillow.h"
#include "minimap_icon.h"
#include "particle.h"
#include "tutorial.h"
#include "title.h"
#include "sound.h"

//�}�N����`
#define RETURN_MOVE		(2.5f)
#define THROW_COOLTIME	(90)	//�������Ƃ��̃N�[���^�C��
#define MOVE_DEF	(70)		//�ړ��ύX�J�E���^�[�̏����l
#define MOVE_MAX	(100)		//�ړ��ύX�J�E���^�[�ɑ����ő�l
#define MOVE_MIN	(50)		//�ړ��ύX�J�E���^�[�ɑ����ŏ��l
#define ROT_DEF		(100)		//�����J�E���^�[�̏����l
#define ROT_MAX		(100)		//�����J�E���^�[�ɑ����ő�l
#define ROT_MIN		(50)		//�����J�E���^�[�ɑ����ŏ��l
#define STOP_CNT	(200)		//�~�܂�J�E���^�[���o���Ԋu
#define MOVE_DISTANCE	(1000.0f)	//�ړ��͈�
#define RANGE_DISTANCE	(280.0f)	//�h���̃��u�����͈�
#define ADD_DISTANCE	(2.2f)		//�~�܂��Ă�Ƃ��̔����͈�
#define CHASE_TIME		(60 * 5)	//�ǂ������鎞��
#define NOTCHASE_TIME	(60 * 2)	//�ǂ������Ȃ�����
#define ESCAPE_DISTANCE	(SEARCH_RADIUS + 400.0f)		//��������͈̔�
#define ESCAPE_TIME		(60 * 1)	//�����鎞��
#define BUDDY1_TXT	"data\\TEXT\\motion_set_buddy1.txt"
#define BUDDY2_TXT	"data\\TEXT\\motion_set_buddy2.txt"

//�v���g�^�C�v�錾
void ReadSetBuddy(void);
void ReadSetMotionBuddy(void);
void UpdateMotionBuddy(int nCntBuddy);
void LimitPosBuddy(int nCntBuddy);
void ControllBuddy(int nCntBuddy);
void ControllTutorialBuddy(int nCntBuddy);
void CollisionCharBuddy(int nCntBuddy);
void CollisionWallBuddy(int nCntBuddy);
void CollisionATKBuddy(int nCntBuddy, float fDistance, float fRadius, float fAngle, int nValue);
void ThrowPillowBuddy(int nCntBuddy);
void UpdateStateBuddy(int nCntBuddy);
void RotCorrect(int nCntBuddy);
void UpdateStateMove(int nCntBuddy);
void MoveCount(int nCntBuddy);
void CollisionChaseBuddy(int nCntBuddy);
void UpdateMoveBuddy(int nCntBuddy);
void CalFormationPosBuddy(int nCntBuddy);
void CollisionEscapeBuddy(int nCntBuddy);
int SetHypnosisWaveBuddy(int nCntBuddy, float fRot, float fWidthMove);
void UpdateGameEndBuddy(int nCntBuddy);

//�O���[�o���ϐ��錾
Buddy g_aBuddy[MAX_BUDDY];		//���_�̏��

//==================================================================================
//���_�̏���������
//==================================================================================
void InitBuddy(void)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�v�f������
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;

		case BUDDYTYPE_DEPPA:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;
		}

		if (GetMode() == MODE_GAME)
		{
			g_aBuddy[nCntBuddy].rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);		//����
		}
		else if (GetMode() == MODE_RANKING)
		{
			g_aBuddy[nCntBuddy].rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);		//����
		}
		else
		{
			g_aBuddy[nCntBuddy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		}

		//�����ʒu�ݒ�
		if (GetMode() != MODE_TITLE && GetMode() != MODE_RESULT)
		{//�^�C�g���ƃ��U���g�ȊO

			g_aBuddy[nCntBuddy].pos = g_aBuddy[nCntBuddy].formationPos;		//���݂̈ʒu
			g_aBuddy[nCntBuddy].posOld = g_aBuddy[nCntBuddy].formationPos;		//�O��̈ʒu
			g_aBuddy[nCntBuddy].bUse = true;	//�g�p���Ă���󋵂ɂ���
		}
		else if (GetMode() == MODE_TITLE)
		{
			switch (nCntBuddy)
			{
			case BUDDYTYPE_HYOROGARI:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
				break;

			case BUDDYTYPE_DEPPA:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
				break;
			}
			g_aBuddy[nCntBuddy].bUse = false;	//�g�p���Ă���󋵂ɂ���
		}
		else if (GetMode() == MODE_RESULT)
		{
			switch (nCntBuddy)
			{
			case BUDDYTYPE_HYOROGARI:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(-160.0f, 76.0f, -30.0f);
				break;

			case BUDDYTYPE_DEPPA:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(140.0f, 76.0f, -30.0f);
				break;
			}
		}
		if (GetMode() == MODE_RANKING)
		{
			switch (nCntBuddy)
			{
			case BUDDYTYPE_HYOROGARI:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);

				//���S����D3DX_PI * -0.5f�̕�����-150.0f
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(sinf(D3DX_PI * -0.25f + g_aBuddy[nCntBuddy].rot.y) * -150.0f, 0.0f, cosf(D3DX_PI * -0.25f + g_aBuddy[nCntBuddy].rot.y) * -150.0f);
				break;

			case BUDDYTYPE_DEPPA:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);

				//���S����D3DX_PI * -0.5f�̕�����-150.0f
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(sinf(D3DX_PI * -0.75f + g_aBuddy[nCntBuddy].rot.y) * -150.0f, 0.0f, cosf(D3DX_PI * -0.75f + g_aBuddy[nCntBuddy].rot.y) * -150.0f);
				break;
			}
		}

		g_aBuddy[nCntBuddy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		g_aBuddy[nCntBuddy].mtxWorld = {};	//���[���h�}�g���b�N�X
		g_aBuddy[nCntBuddy].bFever = false;	//�t�B�[�o�[			//�t�B�[�o�[���
		g_aBuddy[nCntBuddy].fRotDiff = 0.0f;			//�����̍���
		g_aBuddy[nCntBuddy].fRotDest = g_aBuddy[nCntBuddy].rot.y;			//�����̍���
		g_aBuddy[nCntBuddy].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aBuddy[nCntBuddy].nIdxIcon = -1;			//�~�j�}�b�v�A�C�R���̃C���f�b�N�X�ԍ�

		for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
		{
			g_aBuddy[nCntBuddy].nIdxWave[nCntWave] = -1;			//�Ռ��g�̃C���f�b�N�X�ԍ�
		}
		
		g_aBuddy[nCntBuddy].nState = 0;				//���
		g_aBuddy[nCntBuddy].nCntState = 0;			//��ԃJ�E���g
		g_aBuddy[nCntBuddy].nCntChase = 0;			//�ǂ��|���J�E���g
		g_aBuddy[nCntBuddy].bCollisionModel = true;	//���f���Ƃ̔������邩�ǂ���

		//���[�V�����n
		g_aBuddy[nCntBuddy]. nNowMotionNum = 0;		//���݂̃��[�V�����ԍ�
		g_aBuddy[nCntBuddy]. nCntFrame = 0;			//�t���[���̃J�E���g
		g_aBuddy[nCntBuddy]. nPatternKey = 0;		//���ڂ̃L�[��
		g_aBuddy[nCntBuddy]. nPartsNum = 0;			//�p�[�c��
		g_aBuddy[nCntBuddy]. bMove = false;				//�ړ����Ă��邩
		g_aBuddy[nCntBuddy]. bATK = false;				//�U�����Ă邩
		g_aBuddy[nCntBuddy].nCntAllFrame = 0;		//���t���[���J�E���g

		g_aBuddy[nCntBuddy].nCntWalk = 0;			//�ړ��̃J�E���g
		g_aBuddy[nCntBuddy].nCntPenlight = 0;		//�Ö��̃J�E���g
		g_aBuddy[nCntBuddy].fRadius = 50.0f;	//���a

		g_aBuddy[nCntBuddy].fThrowRadius = 240.0f;	//������͈�
		g_aBuddy[nCntBuddy].nThrowCnt = 0;			//������J�E���g

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			g_aBuddy[nCntBuddy].aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
			g_aBuddy[nCntBuddy].aModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
			g_aBuddy[nCntBuddy].aModel[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
			g_aBuddy[nCntBuddy].aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
			g_aBuddy[nCntBuddy].aModel[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
			g_aBuddy[nCntBuddy].aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
			g_aBuddy[nCntBuddy].aModel[nCntModel].mtxWorld = {};	//���[���h�}�g���b�N�X
			g_aBuddy[nCntBuddy].aModel[nCntModel].OldmtxWorld = {};	//���[���h�}�g���b�N�X


			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBuddy[nCntBuddy].aModel[nCntModel].mtxWorld);
			D3DXMatrixIdentity(&g_aBuddy[nCntBuddy].aModel[nCntModel].OldmtxWorld);

			g_aBuddy[nCntBuddy].aModel[nCntModel].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
			g_aBuddy[nCntBuddy].aModel[nCntModel].nIdxRotate = -1;	//���邮��̃C���f�b�N�X�ԍ�
			g_aBuddy[nCntBuddy].aModel[nCntModel].nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
			g_aBuddy[nCntBuddy].aModel[nCntModel].nShadow = 1;			//�e���g�����ǂ���
			g_aBuddy[nCntBuddy].aModel[nCntModel].nFever = -1;

			g_aBuddy[nCntBuddy].aModel[nCntModel].nNumVtx = 0;			//���_��
			g_aBuddy[nCntBuddy].aModel[nCntModel].dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
			g_aBuddy[nCntBuddy].aModel[nCntModel].pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
			g_aBuddy[nCntBuddy].aModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
			g_aBuddy[nCntBuddy].aModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

			g_aBuddy[nCntBuddy].aModel[nCntModel].nType = 0;			//�ǂݍ��ރ��f���̎��
			g_aBuddy[nCntBuddy].aModel[nCntModel].nParent = -1;		//�e�̔ԍ�
			g_aBuddy[nCntBuddy].aModel[nCntModel].bUse = false;			//�g�p���Ă��邩
			g_aBuddy[nCntBuddy].aModel[nCntModel].nState = 0;			//���

			g_aBuddy[nCntBuddy].aModel[nCntModel].pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
			g_aBuddy[nCntBuddy].aModel[nCntModel].pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^

			for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
			{
				g_aBuddy[nCntBuddy].aModel[nCntModel].pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
				g_aBuddy[nCntBuddy].aModel[nCntModel].pMatData[nCntMat].pTextureFilename = {};	//�}�e���A���̃f�[�^
				g_aBuddy[nCntBuddy].aModel[nCntModel].acFilename[nCntMat] = {};	//�t�@�C����
			}
			g_aBuddy[nCntBuddy].aModel[nCntModel].dwNumMat = NULL;			//�}�e���A���̐�

			for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
			{
				g_aBuddy[nCntBuddy].aModel[nCntModel].pTexture[nCntTex] = NULL;	//�e�N�X�`���ւ̃|�C���^
			}

		}

		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
		{//���[�V�����̐��J��Ԃ�

			g_aBuddy[nCntBuddy].aMotionData[nCntMotion].bUse = false;	//�g�p���Ă��邩
			g_aBuddy[nCntBuddy].aMotionData[nCntMotion].nLoop = MOTION_LOOP_ON;	//���[�v����
			g_aBuddy[nCntBuddy].aMotionData[nCntMotion].nNumKey = 0;				//�L�[�̐�

			for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
			{//�L�[�̐��J��Ԃ�

				g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].nFrame = 0;	//�Đ��t���[��

				for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
				{//�p�[�c�̐��J��Ԃ�

					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̈ʒu
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu�̍���
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̌���
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����̍���
				}
			}
		}

		g_aBuddy[nCntBuddy].aMoveAI.nRotCounter = 0;			//�����ύX�J�E���^�[
		g_aBuddy[nCntBuddy].aMoveAI.nMoveCounter = MOVE_DEF + MOVE_MAX;		//�ړ��ύX�J�E���^�[
		g_aBuddy[nCntBuddy].aMoveAI.nRot = 1;					//������+-
		g_aBuddy[nCntBuddy].aMoveAI.nMoveState = MOVEAI_NONE;	//�ړ����
		g_aBuddy[nCntBuddy].aMoveAI.nCntStop = 0;				//�~�܂��Ă����Ԃ��o���₷������J�E���^�[
		g_aBuddy[nCntBuddy].aMoveAI.nChaseCounter = 0;			//���b�ǂ������Ă邩
		g_aBuddy[nCntBuddy].aMoveAI.nNotChaseCounter = NOTCHASE_TIME;		//�ǂ�������܂ł̎���
		g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter = 0;			//�����鎞��

		g_aBuddy[nCntBuddy].aMoveAI.fDistance = 0.0f;			//����

		g_aBuddy[nCntBuddy].aMoveAI.bPosDest = false;				//�ړI�n�ɂ�����
		g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;				//���u�𔭌��������ǂ���
		g_aBuddy[nCntBuddy].aMoveAI.bChaseTime = false;			//�ǂ�������܂ł̎��Ԃ𑫂���
		g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;				//�y�����C�g���g���Ă邩
		g_aBuddy[nCntBuddy].aMoveAI.bEscape = false;				//�����邩

		for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
		{
			g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] = false;	//���Ԗڂ̃��u����������
			g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = false;	//�y�����C�g�͈̔͂ɓ����Ă��邩
		}

		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			g_aBuddy[nCntBuddy].aMoveAI.bEscapeNum[nCntEnemy] = false;	//���Ԗڂ̓G��
		}

		for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
		{
			g_aBuddy[nCntBuddy].aFormation[nCntCrowd].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����̈ʒu
			g_aBuddy[nCntBuddy].aFormation[nCntCrowd].nIndex = -1;			//����̃C���f�b�N�X�ԍ�
			g_aBuddy[nCntBuddy].aFormation[nCntCrowd].bUse = false;			//������g�p���Ă��邩
		}
	}


	//�O���t�@�C���ǂݍ���
	ReadSetBuddy();
	ReadSetMotionBuddy();
}

//==================================================================================
//���_�̏I������
//==================================================================================
void UninitBuddy(void)
{
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		for (int nCount = 0; nCount < MAX_MODEL; nCount++)
		{
			for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
			{
				//�e�N�X�`���̔j��
				if (g_aBuddy[nCntBuddy].aModel[nCount].pTexture[nCntTex] != NULL)
				{
					g_aBuddy[nCntBuddy].aModel[nCount].pTexture[nCntTex]->Release();
					g_aBuddy[nCntBuddy].aModel[nCount].pTexture[nCntTex] = NULL;
				}
			}

			//���b�V���̔j��
			if (g_aBuddy[nCntBuddy].aModel[nCount].pMesh != NULL)
			{
				g_aBuddy[nCntBuddy].aModel[nCount].pMesh->Release();
				g_aBuddy[nCntBuddy].aModel[nCount].pMesh = NULL;
			}

			//�}�e���A���̔j��
			if (g_aBuddy[nCntBuddy].aModel[nCount].pBuffMat != NULL)
			{
				g_aBuddy[nCntBuddy].aModel[nCount].pBuffMat->Release();
				g_aBuddy[nCntBuddy].aModel[nCount].pBuffMat = NULL;
			}
		}
	}
}

//==================================================================================
//���_�̍X�V����
//==================================================================================
void UpdateBuddy(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//���u�̏��擾
	Mob *pMob = GetMob();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++, pMob++)
	{
		if (g_aBuddy[nCntBuddy].bUse == true)
		{//�g�p���Ă�����

			//�ߋ��̈ʒu�ۑ�
			g_aBuddy[nCntBuddy].posOld = g_aBuddy[nCntBuddy].pos;

			//���_�̑���
			if (GetMode() == MODE_GAME && GetGameState() != GAMESTATE_BEFOREEND)
			{
				ControllBuddy(nCntBuddy);
			}
			else if (GetMode() == MODE_GAME && GetGameState() == GAMESTATE_BEFOREEND)
			{
				UpdateGameEndBuddy(nCntBuddy);
			}
			else if (GetMode() == MODE_TUTORIAL)
			{
				ControllTutorialBuddy(nCntBuddy);
			}

			//����̈ʒu�v�Z
			CalFormationPosBuddy(nCntBuddy);

			//�������J�o�[��������
			if (GetMode() == MODE_GAME || (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_END))
			{
				ThrowPillowBuddy(nCntBuddy);
			}

			if (g_aBuddy[nCntBuddy].bMove == true)
			{//�ړ����Ă�Ƃ�

				g_aBuddy[nCntBuddy].nCntWalk++;	//�J�E���g���Z

				if (g_aBuddy[nCntBuddy].nCntWalk > 15)
				{
					g_aBuddy[nCntBuddy].nCntWalk = 0;

					//���s�p�[�e�B�N���̐ݒ�
					SetParticle(g_aBuddy[nCntBuddy].pos, PARTICLE_TYPE_WALK);
				}


				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_WALK &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION &&
					g_aBuddy[nCntBuddy].bFever == false)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_WALK);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERWALK &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION && g_aBuddy[nCntBuddy].bFever == true)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERWALK);
				}
			}
			else if (g_aBuddy[nCntBuddy].bATK == true)
			{//�U�����Ă�Ƃ�

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION && g_aBuddy[nCntBuddy].bFever == false)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_ACTION);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION && g_aBuddy[nCntBuddy].bFever == true)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERACTION);
				}
			}
			else
			{//�j���[�g�������[�V����

				if (g_aBuddy[nCntBuddy].bATK == false &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_DEF && 
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_KYOROKYORO &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION &&
					g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH ||
					(g_aBuddy[nCntBuddy].aMoveAI.bPenUse == false && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH && g_aBuddy[nCntBuddy].aMoveAI.nMoveState != MOVEAI_STOP) 
					|| g_aBuddy[nCntBuddy].aMoveAI.nMoveState == MOVEAI_ESCAPE)
				{
					//�U����ԉ���
					g_aBuddy[nCntBuddy].bATK = false;

					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
				}
			}

			if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_ACTION || 
				g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION || 
				g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERACTION ||
				g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERMOVEACTION)
			{//�Ö��A�N�V������

				if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION &&
					pPlayer->nNowMotionNum != PLAYERMOTION_MOVEACTION ||
					g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERMOVEACTION &&
					pPlayer->nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
				{//�ړ��Ö��� && �v���C���[���ړ��Ö�����߂Ă�����

					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
				}

				g_aBuddy[nCntBuddy].nCntPenlight++;

				if (g_aBuddy[nCntBuddy].nCntPenlight == 20 || g_aBuddy[nCntBuddy].nCntPenlight == 40)
				{
					if (g_aBuddy[nCntBuddy].nCntPenlight >= 40)
					{
						g_aBuddy[nCntBuddy].nCntPenlight = 0;
					}

					//�Ö��̃z�����z�����ݒ�
					if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_ACTION)
					{//�~�܂��čÖ�

						if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
						{
							g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						}
						else
						{
							g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * (-0.25f + (0.5f * nCntBuddy)), (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						}
					}
					else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION)
					{//�����čÖ�

						if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
						{

							g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
						}
						else
						{

							g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * (-0.25f + (0.5f * nCntBuddy)), (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
						}
					}
					else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERACTION)
					{//�t�B�[�o�[���̍Ö�

						g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
					}
					else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERMOVEACTION)
					{//�t�B�[�o�[���̍Ö�

						g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
					}

					if (g_aBuddy[nCntBuddy].bFever == true)
					{//�t�B�[�o�[���Ȃ�

						//�Ö��̃z�����z�����ݒ�
						if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERACTION)
						{//�~�܂��čÖ�

							g_aBuddy[nCntBuddy].nIdxWave[1] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
							g_aBuddy[nCntBuddy].nIdxWave[2] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
							g_aBuddy[nCntBuddy].nIdxWave[3] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						}
						else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERMOVEACTION)
						{//�����čÖ�

							g_aBuddy[nCntBuddy].nIdxWave[1] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
							g_aBuddy[nCntBuddy].nIdxWave[2] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
							g_aBuddy[nCntBuddy].nIdxWave[3] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						}
					}
				}
			}
			else if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION &&
				g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION &&
				g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION &&
				g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION)
			{

				//�U����ԉ���
				g_aBuddy[nCntBuddy].bATK = false;
			}

			//���[�V�����̍X�V
			UpdateMotionBuddy(nCntBuddy);

			//�L�����N�^�[�Ƃ̓����蔻��
			CollisionCharBuddy(nCntBuddy);

			//�ǂƂ̓����蔻��
			//CollisionWall(&g_aBuddy[nCntBuddy].pos, g_aBuddy[nCntBuddy].posOld);

			//���W����
			LimitPosBuddy(nCntBuddy);

			//��ԍX�V
			UpdateStateBuddy(nCntBuddy);

			switch (g_aBuddy[nCntBuddy].nNowMotionNum)
			{
			case BUDDYMOTION_ACTION:

				CollisionATKBuddy(nCntBuddy, PENLIGHT_RADIUS, PENLIGHT_RADIUS, PENLIGHT_ANGLE, STOP_HYPNOSIS);
				break;


			case BUDDYMOTION_MOVEACTION:

				CollisionATKBuddy(nCntBuddy, PENLIGHT_RADIUS_MOVE, PENLIGHT_RADIUS, PENLIGHT_ANGLE_MOVE, MOVE_HYPNOSIS);
				break;

			case BUDDYMOTION_FEVERACTION:

				CollisionATKBuddy(nCntBuddy, PENLIGHT_RADIUS, PENLIGHT_RADIUS, PENLIGHT_ANGLE, STOP_FEVER_HYPNOSIS);
				break;

			case BUDDYMOTION_FEVERMOVEACTION:

				CollisionATKBuddy(nCntBuddy, PENLIGHT_RADIUS_MOVE, PENLIGHT_RADIUS, PENLIGHT_ANGLE_MOVE, MOVE_FEVER_HYPNOSIS);
				break;
			}

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aBuddy[nCntBuddy].nIdxShadow, g_aBuddy[nCntBuddy].pos);

			//�A�C�R���̈ʒu�X�V
			UpdatePositionIcon(g_aBuddy[nCntBuddy].nIdxIcon, g_aBuddy[nCntBuddy].pos);

			//�Ռ��g�̈ʒu�X�V
			if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION)
			{//�ړ��Ö��A�N�V������

				for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
				{
					SetPositionImpactWave(g_aBuddy[nCntBuddy].nIdxWave[nCntWave], g_aBuddy[nCntBuddy].move);
				}
			}
		}
	}
}

//==================================================================================
//���f���̑���
//==================================================================================
void ControllBuddy(int nCntBuddy)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//if (GetGameState() == GAMESTATE_NONE)
	{//�������Ă��Ȃ����

		if (pPlayer->nNowMotionNum != PLAYERMOTION_ACTION && g_aBuddy[nCntBuddy].aMoveAI.bPosDest == false)
		{//�A�N�V�������Ă��Ȃ��Ƃ� && �ړI�n�ɂ��ĂȂ��Ƃ�

			if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (BUDDY_DISTANCE * BUDDY_DISTANCE))
			{//�܂��K��l�ɒ����Ă��Ȃ�������

				//�ړ��ʉ��Z
				if (g_aBuddy[nCntBuddy].nState != BUDDYSTATE_RETURN)
				{//�A�ҏ�Ԃ���Ȃ��Ƃ�

					g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
					g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
				}
				else if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_RETURN)
				{//�A�ҏ�Ԃ̂Ƃ�

					g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * RETURN_MOVE;
					g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * RETURN_MOVE;
				}
			}
		}

		if ((GetKeyboardPress(DIK_L) == true || GetGamepadPress(BUTTON_A, 0)) &&
			g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH && GetGameState() == GAMESTATE_NONE)
		{//L�������ꂽ || �y�����C�g���g��Ăꂢ��Ƃ�

			//�U�����Ă��Ԃɂ���
			g_aBuddy[nCntBuddy].bATK = true;

			if (g_aBuddy[nCntBuddy].move.x <= 1.0f && g_aBuddy[nCntBuddy].move.x >= -1.0f &&
				g_aBuddy[nCntBuddy].move.z <= 1.0f && g_aBuddy[nCntBuddy].move.z >= -1.0f)
			{//�~�܂��Ă���Ƃ��͋���

				g_aBuddy[nCntBuddy].bMove = false;
				g_aBuddy[nCntBuddy].move.x = 0.0f;
				g_aBuddy[nCntBuddy].move.z = 0.0f;

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION && g_aBuddy[nCntBuddy].bFever == false)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_ACTION);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION && g_aBuddy[nCntBuddy].bFever == true)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERACTION);
				}
			}
			else
			{//�����Ă�Ƃ��͎�

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION && g_aBuddy[nCntBuddy].bFever == false)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_MOVEACTION);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION && g_aBuddy[nCntBuddy].bFever == true/* && g_aBuddy[nCntBuddy].bATK == false*/)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERMOVEACTION);
				}
			}
		}

		if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
		{//L�������ꂽ && A�{�^���������ꂽ

			//�U����ԉ���
			g_aBuddy[nCntBuddy].bATK = false;

			//���[�V�����̐ݒ�
			SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
		}

		if (g_aBuddy[nCntBuddy].aMoveAI.bPenUse == true)
		{//�y�����C�g���g��Ăꂢ��Ƃ�

			//�U�����Ă��Ԃɂ���
			g_aBuddy[nCntBuddy].bATK = true;
			g_aBuddy[nCntBuddy].move.x = 0.0f;
			g_aBuddy[nCntBuddy].move.z = 0.0f;
		}
	}

	if (g_aBuddy[nCntBuddy].move.x >= 1.0f || g_aBuddy[nCntBuddy].move.x <= -1.0f || g_aBuddy[nCntBuddy].move.z >= 1.0f || g_aBuddy[nCntBuddy].move.z <= -1.0f)
	{//�ړ����Ă�Ƃ�

		g_aBuddy[nCntBuddy].bMove = true;
	}
	else
	{//�ړ����ĂȂ��Ƃ�

		g_aBuddy[nCntBuddy].bMove = false;

		//if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION || g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_ACTION)
		//{//�ړ��Ö����̎�

		//	//�U����ԉ���
		//	g_aBuddy[nCntBuddy].bATK = false;
		//}
	}

	//�d�͏���
	g_aBuddy[nCntBuddy].move.y -= 1.0f;
	g_aBuddy[nCntBuddy].move.y = 0.0f;

	if (g_aBuddy[nCntBuddy].bFever == true || (g_aBuddy[nCntBuddy].bCollisionModel == false && g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH))
	{//�t�B�[�o�[���Ȃ�

		//����
		g_aBuddy[nCntBuddy].move.x *= 1.1f;
		g_aBuddy[nCntBuddy].move.z *= 1.1f;
	}

	//�ʒu�X�V
	g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

	//��������
	if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION)
	{
		g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
		g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;
	}
	else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION)
	{
		g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR_HYPNOSIS;
		g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR_HYPNOSIS;
	}

}

//==================================================================================
//���_�̍��W����
//==================================================================================
void LimitPosBuddy(int nCntBuddy)
{
	if (g_aBuddy[nCntBuddy].pos.y < 0.0f)
	{//�n�ʂ�艺�ɍs������

		g_aBuddy[nCntBuddy].pos.y = 0.0f;

		//�d�͏���
		g_aBuddy[nCntBuddy].move.y = 0.0f;
	}

	//�ړ�����
	if (GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_END)
	{
		if (g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].fRadius <= -LIMIT_POS && g_aBuddy[nCntBuddy].posOld.x - g_aBuddy[nCntBuddy].fRadius >= -LIMIT_POS)
		{//����-1000�ȉ�, �O��-1000�ȏ�

			g_aBuddy[nCntBuddy].pos.x = -LIMIT_POS + g_aBuddy[nCntBuddy].fRadius;
		}
		if (g_aBuddy[nCntBuddy].pos.x + g_aBuddy[nCntBuddy].fRadius >= LIMIT_POS && g_aBuddy[nCntBuddy].posOld.x + g_aBuddy[nCntBuddy].fRadius <= LIMIT_POS)
		{//����1000�ȏ�, �O��1000�ȉ�

			g_aBuddy[nCntBuddy].pos.x = LIMIT_POS - g_aBuddy[nCntBuddy].fRadius;
		}
		if (g_aBuddy[nCntBuddy].pos.z + g_aBuddy[nCntBuddy].fRadius >= LIMIT_POS && g_aBuddy[nCntBuddy].posOld.z + g_aBuddy[nCntBuddy].fRadius <= LIMIT_POS)
		{//����1000�ȏ�, �O��1000�ȉ�

			g_aBuddy[nCntBuddy].pos.z = LIMIT_POS - g_aBuddy[nCntBuddy].fRadius;
		}
		if (g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].fRadius <= -LIMIT_POS && g_aBuddy[nCntBuddy].posOld.z - g_aBuddy[nCntBuddy].fRadius >= -LIMIT_POS)
		{//����-1000�ȉ�, �O��-1000�ȏ�

			g_aBuddy[nCntBuddy].pos.z = -LIMIT_POS + g_aBuddy[nCntBuddy].fRadius;
		}
	}

}

//==================================================================================
//���_�̑���v�Z����
//==================================================================================
void CalFormationPosBuddy(int nCntBuddy)
{
	//�Q�O�̏����擾
	Crowd *pCrowd = GetCrowd();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	for (int nCntCrowd = 0, nCntformation = 0; nCntCrowd < MAX_CROWD; nCntformation++)
	{
		//���̐l��
		int nWidth = 2 + nCntCrowd;

		if (nWidth >= 50)
		{
			nWidth = 50;
		}

		//���̐l�����J��Ԃ�
		for (int nCntWidth = 0; nCntWidth < nWidth; nCntWidth++)
		{
			//�����̐ݒ�
			float fRotWidth = 2.0f / (float)(nWidth - 1);
			float fRot = fRotWidth * nCntWidth + (D3DX_PI * MOVE_LRDW);

			if (nCntCrowd + nCntWidth < MAX_CROWD)
			{//�ő�l�ȉ�

				g_aBuddy[nCntBuddy].aFormation[nCntCrowd + nCntWidth].pos =
					D3DXVECTOR3
					(
						g_aBuddy[nCntBuddy].pos.x + sinf(-D3DX_PI * MOVE_LR + fRot + g_aBuddy[nCntBuddy].rot.y) * (FORMATION_DISTANCE * (nCntformation + 1)),
						0.0f,
						g_aBuddy[nCntBuddy].pos.z + cosf(-D3DX_PI * MOVE_LR + fRot + g_aBuddy[nCntBuddy].rot.y) * (FORMATION_DISTANCE * (nCntformation + 1)));
			}
			else
			{
				break;
			}
		}

		nCntCrowd += (nWidth - 1);
	}
}

//==================================================================================
//���_�̑���v�Z����
//==================================================================================
int SetFormationPosBuddy(int nCntBuddy)
{
	int nCntIdx = 0;

	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aBuddy[nCntBuddy].aFormation[nCntCrowd].bUse == false)
		{//�g�p���Ă��Ȃ�������

			//�C���f�b�N�X�ԍ�����
			nCntIdx = nCntCrowd;

			g_aBuddy[nCntBuddy].aFormation[nCntCrowd].bUse = true;
			break;
		}
	}

	return nCntIdx;
}

//==================================================================================
//�Ö����̃z�����z�����ݒ菈��
//==================================================================================
int SetHypnosisWaveBuddy(int nCntBuddy, float fRot, float fWidthMove)
{
	//�Ö��̃z�����z����
	int nIdxWave = SetImpactWave
	(
		D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x, g_aBuddy[nCntBuddy].pos.y + 50.0f, g_aBuddy[nCntBuddy].pos.z),
		D3DXVECTOR3(PENLIGHT_RADIUS / (float)40, 0.0f, PENLIGHT_RADIUS / (float)40),
		D3DXVECTOR3(D3DX_PI * 0.5f, fRot, 0.0f),
		D3DXCOLOR(1.0f, 0.3f, 1.0f, 0.8f),
		40.0f,	//��
		15.0f,	//����
		40,		//����
		fWidthMove,	//���̈ړ���
		INPACTWAVE_TYPE_PINK		//�e�N�X�`���^�C�v
	);

	return nIdxWave;
}

//==================================================================================
//���_�̕������J�o�[��������
//==================================================================================
void ThrowPillowBuddy(int nCntBuddy)
{
	//���u�̏��擾
	Mob *pMob = GetMob();

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if ((pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS && g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_NONE) || 
			(pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH))
		{//�g�p����Ă��� && �N�[���^�C�����I����Ă� && �y����������Ȃ� && �������ĂȂ� ||
		 //�g�p����Ă��� && �N�[���^�C�����I����Ă� && �h�����

			if (CircleRange(g_aBuddy[nCntBuddy].pos, pMob->aModel.pos) <= (g_aBuddy[nCntBuddy].fThrowRadius * g_aBuddy[nCntBuddy].fThrowRadius))
			{//�T���T�[�N���̒��ɓ�������

				if (g_aBuddy[nCntBuddy].nThrowCnt >= 1)
				{//�N�[���^�C������������

					g_aBuddy[nCntBuddy].nThrowCnt = (g_aBuddy[nCntBuddy].nThrowCnt + 1) % THROW_COOLTIME;
				}

				if (g_aBuddy[nCntBuddy].nThrowCnt <= 0)
				{
					//�������J�o�[�̌��������߂�
					float fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - pMob->aModel.pos.x), (g_aBuddy[nCntBuddy].pos.z - pMob->aModel.pos.z));

					//�������̐ݒ菈��
					if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
					{//�h�����͎����B���e

						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(0.0f, fRotDest, 0.0f), 1 + nCntBuddy);
					}
					else
					{//���čs���Ă�Ƃ��̓v���C���[���e

						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(0.0f, fRotDest, 0.0f), PARENT_PLAYER);
					}

					if (g_aBuddy[nCntBuddy].bFever == true)
					{//�t�B�[�o�[���Ȃ�

						//�S���ʕ�����
						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(g_aBuddy[nCntBuddy].rot.x, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f, g_aBuddy[nCntBuddy].rot.z), PILLOWPARENT_PLAYER);
						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(g_aBuddy[nCntBuddy].rot.x, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f, g_aBuddy[nCntBuddy].rot.z), PILLOWPARENT_PLAYER);
						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(g_aBuddy[nCntBuddy].rot.x, g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f, g_aBuddy[nCntBuddy].rot.z), PILLOWPARENT_PLAYER);
					}

					//�N�[���^�C���ݒ�
					g_aBuddy[nCntBuddy].nThrowCnt = 1;
				}
				break;
			}
		}
	}
}

//==================================================================================
//���_�̍U��
//==================================================================================
void CollisionATKBuddy(int nCntBuddy, float fDistance, float fRadius, float fAngle, int nValue)
{
	//���u�̏��擾
	Mob *pMob = GetMob();

	bool bPenLight = false, bSearch = false;

	//�y�����C�g�̍U��
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState != MOBSTATE_APPIEAR)
		{//���u���g�p����Ă�����

			//�y�����C�g�̓����蔻��
			bSearch = CollisionFan(g_aBuddy[nCntBuddy].pos,
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f), g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f)),
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f), g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f)),
				pMob->aModel.pos, pMob->aModel.posOld);


			//�y�����C�g�̓����蔻��
			bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(fAngle + g_aBuddy[nCntBuddy].rot.y) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(fAngle + g_aBuddy[nCntBuddy].rot.y) * -fDistance),
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-fAngle + g_aBuddy[nCntBuddy].rot.y) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-fAngle + g_aBuddy[nCntBuddy].rot.y) * -fDistance),
				pMob->aModel.pos, pMob->aModel.posOld);

			if (g_aBuddy[nCntBuddy].bFever == true)
			{//�t�B�[�o�[���Ȃ�

				if (bPenLight == false)
				{//�܂��������Ă��Ȃ�������

					bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f) * -fDistance),
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}

				if (bPenLight == false)
				{//�܂��������Ă��Ȃ�������

					bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f) * -fDistance),
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}

				if (bPenLight == false)
				{//�܂��������Ă��Ȃ�������

					bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(fAngle + g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(fAngle + g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f) * -fDistance),
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-fAngle + g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-fAngle + g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}
			}


			if (bPenLight == true)
			{//��͈̔͂ɓ�������

				if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
				{//�h�����

					//���u�𔭌����Ă��Ȃ���Ԃɂ���
					g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

					//�y�����C�g���g�p���Ă����Ԃɂ���
					g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = true;
					g_aBuddy[nCntBuddy].aMoveAI.bPenUse = true;

				}

				//�Ö��x����������܂ł̃J�E���^�[��������
				pMob->nSubHypCounter = HYP_CNTTIME;

				//���u�̃q�b�g����
				HitMob(nCntMob, nValue);
			}

			if (bPenLight == false && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
			{//��͈̔͂ɓ����ĂȂ� && �y�����C�g���g���Ă��� && �h�����

				 //�y�����C�g���g�p���Ă��Ȃ���Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = false;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;
			}
		}
		else if (pMob->aModel.bUse == false && pMob->aModel.nState != MOBSTATE_APPIEAR)
		{
			if (bPenLight == false && g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] == true && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
			{//��͈̔͂ɓ����ĂȂ� && �y�����C�g���g���Ă��� && �h�����

			 //�y�����C�g���g�p���Ă��Ȃ���Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = false;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;
			}
		}
	}

	if (bSearch == false && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH/* && g_aBuddy[nCntBuddy].bATK == true*/)
	{//��͈̔͂ɓ����ĂȂ� && �y�����C�g���g���Ă��� && �h�����

	 //�y�����C�g���g�p���Ă��Ȃ���Ԃɂ���
		g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;
	}
}

//==================================================================================
//�ǂƂ̓����蔻��
//==================================================================================
void CollisionWallBuddy(int nCntBuddy)
{
	//�ǂ̏��擾
	MESHWALL *pMeshWall = GetMeshWall();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pMeshWall++)
	{//�ǂ̐����J��Ԃ�

		if (pMeshWall->bUse == true)
		{//�ǂ��g�p����Ă���Ƃ�

			//�ǂ̕�
			float fLen = (pMeshWall->nWidth * POS_MESHWALL) * 0.5f;

			//���̒��_���W
			D3DXVECTOR3 pos0 = D3DXVECTOR3(
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * -fLen,
				g_aBuddy[nCntBuddy].pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * -fLen);

			//�E�̒��_���W
			D3DXVECTOR3 pos1 = D3DXVECTOR3(
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen,
				g_aBuddy[nCntBuddy].pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen);

			//���E���̃x�N�g��
			D3DXVECTOR3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			//���_�̋��E���̃x�N�g��
			D3DXVECTOR3 vecLineBuddy;
			vecLineBuddy.x = g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].posOld.x;
			vecLineBuddy.z = g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].posOld.z;

			//���_�ƕǂ̃x�N�g��
			D3DXVECTOR3 vecToPosBuddy;
			vecToPosBuddy.x = pos1.x - g_aBuddy[nCntBuddy].posOld.x;
			vecToPosBuddy.z = pos1.z - g_aBuddy[nCntBuddy].posOld.z;

			//�ʐς̍ő�l
			float fMaxAreaBuddy = (vecLineBuddy.z * vecLine.x) - (vecLineBuddy.x * vecLine.z);

			//����̖ʐ�
			float fNowAreaBuddy = (vecToPosBuddy.z * vecLine.x) - (vecToPosBuddy.x * vecLine.z);

			//����
			float fRateBuddy = fNowAreaBuddy / fMaxAreaBuddy;

			if (fRateBuddy >= 0.0f && fRateBuddy <= 1.0f)
			{//�ʐς͈͓̔��ɂ����画��

				if ((vecLineBuddy.z * vecToPosBuddy.x) - (vecLineBuddy.x * vecToPosBuddy.z) > 0)
				{//�ǂɓ���������

					//��_����͂ݏo����
					D3DXVECTOR3 CollisionPointBuddy = D3DXVECTOR3(0.0f, g_aBuddy[nCntBuddy].pos.y, 0.0f);
					CollisionPointBuddy.x = (g_aBuddy[nCntBuddy].pos.x + (vecLineBuddy.x * (fRateBuddy - 1.0f)));
					CollisionPointBuddy.z = (g_aBuddy[nCntBuddy].pos.z + (vecLineBuddy.z * (fRateBuddy - 1.0f)));

					//�@���x�N�g��(���E���x�N�g����X��Z���])
					D3DXVECTOR3 vecNor = D3DXVECTOR3(vecLine.z, 0.0f, -vecLine.x);

					//�x�N�g���̐��K��
					D3DXVec3Normalize(&vecNor, &vecNor);

					//���_�̋t�ړ���
					D3DXVECTOR3 BuddyInverceMove;
					BuddyInverceMove.z = vecLineBuddy.z * (fRateBuddy - 1.0f);
					BuddyInverceMove.x = vecLineBuddy.x * (fRateBuddy - 1.0f);

					//����(�ǂ̖@���Ƒ��_�̋t�ړ���)
					float fDot = (BuddyInverceMove.x * vecNor.x) + (BuddyInverceMove.z * vecNor.z);

					//�ǂ���ړ���
					D3DXVECTOR3 MoveWall = vecNor * fDot;

					D3DXVECTOR3 ppp = (CollisionPointBuddy - g_aBuddy[nCntBuddy].pos);
					D3DXVECTOR3 pppp = ppp + MoveWall;

					//�Ԃ������_�ɕ␳
					g_aBuddy[nCntBuddy].pos += MoveWall + (vecNor * 0.1f);

				}
			}
		}
	}
}

//==================================================================================
//���_�̏�ԍX�V
//==================================================================================
void UpdateStateBuddy(int nCntBuddy)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	bool fRange = false;		//���u���͈͂ɓ��������̔���

	switch (g_aBuddy[nCntBuddy].nState)
	{
	case BUDDYSTATE_NONE:

		//�ړI�n�ɂ��ĂȂ���Ԃɂ���
		g_aBuddy[nCntBuddy].aMoveAI.bPosDest = false;

		//���u�𔭌����ĂȂ���Ԃɂ���
		g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

		//�y�����C�g���g�p���Ă��Ȃ���Ԃɂ���
		g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;

		//����̈ʒu�ݒ�
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;

		case BUDDYTYPE_DEPPA:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;
		}

		if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) < (BUDDY_DISTANCE * BUDDY_DISTANCE))
		{//�ړI�n�ɒ�������

			g_aBuddy[nCntBuddy].bCollisionModel = true;
			g_aBuddy[nCntBuddy].nCntChase = 0;
		}

		break;

	case BUDDYSTATE_DISPATCH:

		if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) < (BUDDY_DISTANCE * BUDDY_DISTANCE))
		{//�ړI�n�ɂ����Ƃ�

			//�ړI�n�ɂ�����Ԃɂ���
			g_aBuddy[nCntBuddy].aMoveAI.bPosDest = true;
		}

		if (g_aBuddy[nCntBuddy].aMoveAI.bPosDest == true)
		{//�ړI�n�ɂ�����Ԃ̂Ƃ�

			//MORS���瓦���锻��
			CollisionEscapeBuddy(nCntBuddy);

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION)
			{//�U�����[�V��������Ȃ��Ƃ�

				if (g_aBuddy[nCntBuddy].aMoveAI.bMobDisc == false && g_aBuddy[nCntBuddy].aMoveAI.bEscape == false)
				{//���u�𔭌����ĂȂ��Ƃ� && �����ĂȂ��Ƃ�

					//�ړ��J�E���^�[�X�V
					MoveCount(nCntBuddy);
				}

				if (g_aBuddy[nCntBuddy].aMoveAI.nMoveState != MOVEAI_STOP)
				{//�~�܂��Ԃ���Ȃ��Ƃ�

					//�ړ��̍X�V
					UpdateMoveBuddy(nCntBuddy);
				}
				else if(g_aBuddy[nCntBuddy].aMoveAI.nMoveState == MOVEAI_STOP)
				{//����낫��낵�Ă鎞

					if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_KYOROKYORO)
					{
						//���[�V�����̐ݒ�
						SetMotisonBuddy(nCntBuddy, BUDDYMOTION_KYOROKYORO);
					}
				}

				if (g_aBuddy[nCntBuddy].aMoveAI.bChaseTime == true)
				{//�ǂ�������܂ł̃J�E���g����Ƃ�

					//�ǂ�������܂ł̎���
					g_aBuddy[nCntBuddy].aMoveAI.nNotChaseCounter++;
				}

				if (g_aBuddy[nCntBuddy].aMoveAI.bEscape == false)
				{
					//���u�ǂ���������
					CollisionChaseBuddy(nCntBuddy);

				}

				//�ړ��̏�ԍX�V
				UpdateStateMove(nCntBuddy);

			}

		}

		break;

	case BUDDYSTATE_RETURN:

		//�ړI�n�ɂ��ĂȂ���Ԃɂ���
		g_aBuddy[nCntBuddy].aMoveAI.bPosDest = false;

		//���u�𔭌����ĂȂ���Ԃɂ���
		g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

		//�y�����C�g���g�p���Ă��Ȃ���Ԃɂ���
		g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;

		//����̈ʒu�ݒ�
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;

		case BUDDYTYPE_DEPPA:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;
		}

		if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) <= (BUDDY_DISTANCE * BUDDY_DISTANCE))
		{//�v���C���[�̂Ƃ���ɒ����Ă�����

			g_aBuddy[nCntBuddy].nState = BUDDYSTATE_NONE;

			if (nCntBuddy == BUDDYTYPE_HYOROGARI)
			{
				//SE���Z�b�g
				PlaySound(SOUND_LABEL_SE_ONMINO);
			}

			if (nCntBuddy == BUDDYTYPE_DEPPA)
			{
				//SE���Z�b�g
				PlaySound(SOUND_LABEL_SE_ORANGEKUN);
			}
			
		}

		break;
	}

	if (((GetMode() == MODE_GAME && GetGameState() != GAMESTATE_BEFOREEND) || GetMode() == MODE_TUTORIAL) &&
		CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (BUDDY_DISTANCE * BUDDY_DISTANCE) && g_aBuddy[nCntBuddy].aMoveAI.bPosDest == false)
	{//�܂��ړI�n�ɂ��ĂȂ��Ƃ�

		//�ڕW�̊p�x�����߂�
		g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].formationPos.x), (g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].formationPos.z));
		
		//�p�x�̕␳
		RotCorrect(nCntBuddy);
	}
}

//==================================================================================
//�ړ��̃J�E���^�[�X�V����
//==================================================================================
void MoveCount(int nCntBuddy)
{
	//�ړ��ύX�J�E���^�[�𑫂�
	g_aBuddy[nCntBuddy].aMoveAI.nMoveCounter++;
	g_aBuddy[nCntBuddy].aMoveAI.nCntStop++;


	if (g_aBuddy[nCntBuddy].aMoveAI.nMoveCounter >= MOVE_DEF + rand() % MOVE_MAX + MOVE_MIN)
	{//��莞�Ԍo������

		if (g_aBuddy[nCntBuddy].aMoveAI.nCntStop >= STOP_CNT)
		{
			//�~�܂��Ă����Ԃɂ���
			g_aBuddy[nCntBuddy].aMoveAI.nMoveState = MOVEAI_STOP;

			g_aBuddy[nCntBuddy].aMoveAI.nCntStop = 0;
		}
		else
		{
			//�ړ���Ԃ�ύX����
			g_aBuddy[nCntBuddy].aMoveAI.nMoveState = rand() % MOVEAI_STOP + 1;
		}

		g_aBuddy[nCntBuddy].aMoveAI.nMoveCounter = 0;

	}


	//������+-�ύX�J�E���^�[
	g_aBuddy[nCntBuddy].aMoveAI.nRotCounter++;

	if (g_aBuddy[nCntBuddy].aMoveAI.nRotCounter >= ROT_DEF + rand() % ROT_MAX + ROT_MIN)
	{//��莞�Ԍo������

		//������^�t�ɂ���
		g_aBuddy[nCntBuddy].aMoveAI.nRot *= -1;

		//����+-�ύX�J�E���^�[��������
		g_aBuddy[nCntBuddy].aMoveAI.nRotCounter = 0;
	}
}

//==================================================================================
//�ړ��̏�ԍX�V����
//==================================================================================
void UpdateStateMove(int nCntBuddy)
{

	//���u�̏��擾
	Mob *pMob = GetMob();

	bool bSearch = false;

	//�y�����C�g�̍U��
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState != MOBSTATE_APPIEAR && g_aBuddy[nCntBuddy].aMoveAI.bEscape == false)
		{//���u���g�p����Ă�����

			//�y�����C�g�̓����蔻��
			bSearch = CollisionFan(g_aBuddy[nCntBuddy].pos,
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f), g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f)),
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f), g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f)),
				pMob->aModel.pos, pMob->aModel.posOld);

			//�y�����C�g�̓����蔻��
			bool bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -RANGE_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -RANGE_DISTANCE),
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -RANGE_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -RANGE_DISTANCE),
				pMob->aModel.pos, pMob->aModel.posOld);

			if (bSearch == true)
			{//��͈̔͂ɓ�������

				//���u�𔭌����Ă��Ȃ���Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

				//�y�����C�g���g�p���Ă����Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = true;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = true;
			}

			
			if (bPenLight == false && g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] == true && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
			{//��͈̔͂ɓ����ĂȂ� && �y�����C�g���g���Ă��� && �h�����

				//�y�����C�g���g�p���Ă��Ȃ���Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = false;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;
			}
			
		}
	}

	switch (g_aBuddy[nCntBuddy].aMoveAI.nMoveState)
	{
	case MOVEAI_WIDTH:			//���ړ�

		g_aBuddy[nCntBuddy].fRotDest = 0.5f * D3DX_PI * g_aBuddy[nCntBuddy].aMoveAI.nRot;

		break;

	case MOVEAI_HEIGHT:			//�c�ړ�

		if (g_aBuddy[nCntBuddy].aMoveAI.nRot == -1)
		{
			//��
			g_aBuddy[nCntBuddy].fRotDest = 1.0f * D3DX_PI;
		}
		else if (g_aBuddy[nCntBuddy].aMoveAI.nRot == 1)
		{
			//��O
			g_aBuddy[nCntBuddy].fRotDest = 0.0f * D3DX_PI;
		}

		break;

	case MOVEAI_DIAGONAL_FORM:		//�΂߈ړ�(��)

		g_aBuddy[nCntBuddy].fRotDest = 0.25f * D3DX_PI * g_aBuddy[nCntBuddy].aMoveAI.nRot;

		break;

	case MOVEAI_DIAGONAL_BACK:		//�΂߈ړ�(��O)

		g_aBuddy[nCntBuddy].fRotDest = 0.75f * D3DX_PI * g_aBuddy[nCntBuddy].aMoveAI.nRot;

		break;

	case MOVEAI_STOP:				//���������i�ړ������j

		//rot�ɒ��ڑ��
		g_aBuddy[nCntBuddy].rot.y += 0.01f * D3DX_PI * g_aBuddy[nCntBuddy].aMoveAI.nRot;

		break;

	case MOVEAI_CHASE:		//�ǂ��������

		if (g_aBuddy[nCntBuddy].aMoveAI.bMobDisc == true)
		{//���u�𔭌����Ă�Ƃ�

			//�ǂ������J�E���^�[
			g_aBuddy[nCntBuddy].aMoveAI.nChaseCounter++;
		}

		if (g_aBuddy[nCntBuddy].aMoveAI.nChaseCounter >= CHASE_TIME)
		{
			//���u�𔭌����ĂȂ���Ԃɂ���
			g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

			//�J�E���^�[������
			g_aBuddy[nCntBuddy].aMoveAI.nChaseCounter = 0;
		}
		break;

	case MOVEAI_ESCAPE:				//�������

		if (g_aBuddy[nCntBuddy].aMoveAI.bEscape == true)
		{
			g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter++;
		}

		break;
	}

}

//==================================================================================
//�ړ��̍X�V����
//==================================================================================
void UpdateMoveBuddy(int nCntBuddy)
{
	if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (MOVE_DISTANCE * MOVE_DISTANCE))
	{//�ړ��͈͊O�̎�

		//�ڕW�̊p�x�����߂�
		g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].formationPos.x), (g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].formationPos.z));
	}

	//�p�x�̕␳
	RotCorrect(nCntBuddy);

	//�j�[�g�����ړ�
	g_aBuddy[nCntBuddy].move.x += sinf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI) * CHARA_MOVE;
	g_aBuddy[nCntBuddy].move.z += cosf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI) * CHARA_MOVE;
}

//==================================================================================
//�p�x�̕␳
//==================================================================================
void RotCorrect(int nCntBuddy)
{
	//�ڕW�̊p�x�����߂�
	g_aBuddy[nCntBuddy].fRotDiff = g_aBuddy[nCntBuddy].fRotDest - g_aBuddy[nCntBuddy].rot.y;

	//�p�x�̐��K��
	RotNormalize(&g_aBuddy[nCntBuddy].fRotDiff);

	//�p�x�̕␳������
	g_aBuddy[nCntBuddy].rot.y += g_aBuddy[nCntBuddy].fRotDiff * 0.2f;

	//�p�x�̐��K��
	RotNormalize(&g_aBuddy[nCntBuddy].rot.y);
}

//==================================================================================
//�h���̃��u�ǂ���������
//==================================================================================
void CollisionChaseBuddy(int nCntBuddy)
{
	//���u�̏��擾
	Mob *pMob = GetMob();

	bool fRange = false;

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState != MOBSTATE_APPIEAR)
		{//���u���g�p����Ă�����

			if (g_aBuddy[nCntBuddy].aMoveAI.nMoveState == MOVEAI_STOP)
			{//�~�܂��Ă�Ƃ�

				//��̓����蔻��
				fRange = CollisionFan(g_aBuddy[nCntBuddy].pos,
					D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.25f) * -RANGE_DISTANCE * ADD_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.25f) * -RANGE_DISTANCE * ADD_DISTANCE),
					D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.25f) * -RANGE_DISTANCE * ADD_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.25f) * -RANGE_DISTANCE * ADD_DISTANCE),
					pMob->aModel.pos, pMob->aModel.posOld);

			}
			else if (g_aBuddy[nCntBuddy].aMoveAI.nMoveState != MOVEAI_STOP)
			{//�ړ����Ă�Ƃ�

				//��̓����蔻��
				fRange = CollisionFan(g_aBuddy[nCntBuddy].pos,
					D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.25f) * -RANGE_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.25f) * -RANGE_DISTANCE),
					D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.25f) * -RANGE_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.25f) * -RANGE_DISTANCE),
					pMob->aModel.pos, pMob->aModel.posOld);

			}


			if (g_aBuddy[nCntBuddy].aMoveAI.bMobDisc == false && g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] == true)
			{//���u�𔭌����ĂȂ� && ���̔ԍ��̃��u�𔭌����Ă���

				//���̃��u�𔭌����ĂȂ���Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] = false;

				//�ǂ�������܂ł̃J�E���g����
				g_aBuddy[nCntBuddy].aMoveAI.bChaseTime = true;

			}
			else if (fRange == true && g_aBuddy[nCntBuddy].aMoveAI.bMobDisc == false && pMob->aModel.nState != MOBSTATE_APPIEAR && pMob->nHypnosis < 10 && g_aBuddy[nCntBuddy].aMoveAI.nNotChaseCounter >= NOTCHASE_TIME)
			{//��͈̔͂ɓ����Ă���Ƃ� && ���u�𔭌����ĂȂ��Ƃ� && ���u���o����Ԃ���Ȃ��Ƃ� && ��莞�Ԍo�����Ƃ�

				//���u�𔭌�������Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = true;

				//���̔ԍ��̃��u�𔭌�������Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] = true;

				//�ǂ�������Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.nMoveState = MOVEAI_CHASE;

				//�ǂ�������܂ł̃J�E���g���Ȃ�
				g_aBuddy[nCntBuddy].aMoveAI.bChaseTime = false;

				//�J�E���g������
				g_aBuddy[nCntBuddy].aMoveAI.nNotChaseCounter = 0;

			}

			if (g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] == true)
			{//�����������u�̏ꍇ

				//�ڕW�̊p�x�����߂�
				g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - pMob->aModel.pos.x), (g_aBuddy[nCntBuddy].pos.z - pMob->aModel.pos.z));

			}
		}
	}
}

//==================================================================================
//�h����MORS���瓦���锻��
//==================================================================================
void CollisionEscapeBuddy(int nCntBuddy)
{
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bMoreSpawn == true)
		{//�g�p���Ă�����

			if (CircleRange(g_aBuddy[nCntBuddy].pos, pEnemy->pos) < (ESCAPE_DISTANCE * ESCAPE_DISTANCE))
			{//�͈͓���MORS��������

				//������Ԃɂ���
				g_aBuddy[nCntBuddy].aMoveAI.nMoveState = MOVEAI_ESCAPE;

				//������
				g_aBuddy[nCntBuddy].aMoveAI.bEscape = true;
				g_aBuddy[nCntBuddy].aMoveAI.bEscapeNum[nCntEnemy] = true;
				g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter = 0;

				//�y�����C�g���g�p���Ȃ���Ԃɂ���
				g_aBuddy[nCntBuddy].bATK = false;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;

				//MORS�Ɛ^���΂ɖڕW�n�_�ݒ�
				g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - pEnemy->pos.x), (g_aBuddy[nCntBuddy].pos.z - pEnemy->pos.z)) + D3DX_PI;
			}
			else if (g_aBuddy[nCntBuddy].aMoveAI.bEscapeNum[nCntEnemy] == true && g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter >= ESCAPE_TIME)
			{

				//������
				g_aBuddy[nCntBuddy].aMoveAI.bEscape = false;
				g_aBuddy[nCntBuddy].aMoveAI.bEscapeNum[nCntEnemy] = false;

				g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter = 0;

			}
		}
	}

}

//==================================================================================
//�L�����N�^�[�Ƃ̓����蔻��
//==================================================================================
void CollisionCharBuddy(int nCntBuddy)
{
	//�G�̏��擾
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bMoreSpawn == true)
		{//�G���g�p����Ă�����

			//�L�����N�^�[���m�̓����蔻��
			CollisionCharacter(
				&g_aBuddy[nCntBuddy].pos, &g_aBuddy[nCntBuddy].posOld, D3DXVECTOR3(g_aBuddy[nCntBuddy].fRadius, 0.0f, g_aBuddy[nCntBuddy].fRadius), D3DXVECTOR3(-g_aBuddy[nCntBuddy].fRadius, 0.0f, -g_aBuddy[nCntBuddy].fRadius),
				&pEnemy->pos, D3DXVECTOR3(pEnemy->fRadius, 0.0f, pEnemy->fRadius), D3DXVECTOR3(-pEnemy->fRadius, 0.0f, -pEnemy->fRadius));
		}
	}

	
	//���f���̏��擾
	ModelModel *pModel = GetModel();

	//�����������̔���
	bool bHit = false;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true && pModel->nType < SMALL_NUM && g_aBuddy[nCntBuddy].nState != BUDDYSTATE_RETURN && g_aBuddy[nCntBuddy].bCollisionModel == true)
		{//���f�����g�p����Ă����� && �A�ҏ�Ԃ���Ȃ��Ƃ� && �������鎞

			//�������Ă�����
			if (bHitCharacter(
				&g_aBuddy[nCntBuddy].pos, &g_aBuddy[nCntBuddy].posOld, D3DXVECTOR3(g_aBuddy[nCntBuddy].fRadius, 0.0f, g_aBuddy[nCntBuddy].fRadius), D3DXVECTOR3(-g_aBuddy[nCntBuddy].fRadius, 0.0f, -g_aBuddy[nCntBuddy].fRadius),
				&pModel->posOrigin, pModel->vtxMax, pModel->vtxMin) == true)
			{
				bHit = true;
			}

			//�L�����N�^�[���m�̓����蔻��
			CollisionCharacter(
				&g_aBuddy[nCntBuddy].pos, &g_aBuddy[nCntBuddy].posOld, D3DXVECTOR3(g_aBuddy[nCntBuddy].fRadius, 0.0f, g_aBuddy[nCntBuddy].fRadius), D3DXVECTOR3(-g_aBuddy[nCntBuddy].fRadius, 0.0f, -g_aBuddy[nCntBuddy].fRadius),
				&pModel->posOrigin, pModel->vtxMax, pModel->vtxMin);
		}
	}

	//�������Ă�����
	if (bHit == true)
	{
		//�Ǐ]�J�E���g�����Z
		g_aBuddy[nCntBuddy].nCntChase++;
	}
	else
	{
		//�Ǐ]�J�E���g�����Z
		g_aBuddy[nCntBuddy].nCntChase--;

		if (g_aBuddy[nCntBuddy].nCntChase <= 0)
		{
			g_aBuddy[nCntBuddy].nCntChase = 0;
		}
	}

	//�Ǐ]�J�E���g�ɒB���Ă�����
	if (g_aBuddy[nCntBuddy].nCntChase > 60)
	{
		g_aBuddy[nCntBuddy].nCntChase = 60;
		g_aBuddy[nCntBuddy].bCollisionModel = false;
	}
}

//==================================================================================
//���_�̃��[�V��������
//==================================================================================
void SetMotisonBuddy(int nCntBuddy, int nMotionType)
{

	//if (g_aBuddy[nCntBuddy].nNowMotionNum != nMotionType)
	//{

	//	for (int nCntKey = 0; nCntKey < g_aBuddy[nCntBuddy].aMotionData[nMotionType].nNumKey; nCntKey++)
	//	{//�L�[�����J��Ԃ�

	//		for (int nCntParts = 0; nCntParts < g_aBuddy[nCntBuddy].nPartsNum; nCntParts++)
	//		{//�p�[�c���J��Ԃ�

	//			g_aBuddy[nCntBuddy].aModel[nCntParts].rot = g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[nCntKey].aParts[nCntParts].rot;
	//			g_aBuddy[nCntBuddy].aModel[nCntParts].pos = g_aBuddy[nCntBuddy].aModel[nCntParts].posOrigin;
	//		}

	//		g_aBuddy[nCntBuddy].nCntFrame = 0;	//�t���[���J�E���g���Z�b�g
	//	}

	//	//���̃��[�V�����̍X�V
	//	g_aBuddy[nCntBuddy].nNowMotionNum = nMotionType;

	//	g_aBuddy[nCntBuddy].nPatternKey = 0;	//���݂̃L�[�ԍ����Z�b�g
	//	g_aBuddy[nCntBuddy].nCntAllFrame = 0;	//���t���[�������Z�b�g
	//}


	//for (int nCntParts = 0; nCntParts < g_aBuddy[nCntBuddy].nPartsNum; nCntParts++)
	//{//�p�[�c���J��Ԃ�

	//	//���̌����ƌ��݂̌����ŖڕW�̌��������߂�
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDest = g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot;
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].posDest = g_aBuddy[nCntBuddy].aModel[nCntParts].posOrigin + g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos;

	//	//���݂ƖڕW�̍��������߂�
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff = g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDest - g_aBuddy[nCntBuddy].aModel[nCntParts].rot;
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].posDiff = g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].posDest - g_aBuddy[nCntBuddy].aModel[nCntParts].pos;

	//	//�p�x�̐��K��
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.x = RotNormalize(g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.x);
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.y = RotNormalize(g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.y);
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.z = RotNormalize(g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.z);

	//}

	//���̃��[�V�����̍X�V
	g_aBuddy[nCntBuddy].nNowMotionNum = nMotionType;
	g_aBuddy[nCntBuddy].nPatternKey = 0;	//���݂̃L�[�ԍ����Z�b�g
	g_aBuddy[nCntBuddy].nCntFrame = 0;	//�t���[���J�E���g���Z�b�g
	g_aBuddy[nCntBuddy].nCntAllFrame = 0;	//���t���[�������Z�b�g

}

//==================================================================================
//���_�̃��[�V�����X�V����
//==================================================================================
void UpdateMotionBuddy(int nCntBuddy)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	for (int nCntParts = 0; nCntParts < g_aBuddy[nCntBuddy].nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

		//���̃L�[
		int nNextKey = (g_aBuddy[nCntBuddy].nPatternKey + 1) % g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nNumKey;

		if (g_aBuddy[nCntBuddy].nPatternKey == g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nNumKey - 1 && 
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nLoop == MOTION_LOOP_OFF)
		{//���[�v���Ȃ��Ƃ�

			nNextKey = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nNumKey - 1;
		}

		//�O��Ǝ��̃L�[�̍����擾
		float rotDiffX = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.z;


		//�p�[�c�̌�����ݒ�
		g_aBuddy[nCntBuddy].aModel[nCntParts].rot.x =
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)g_aBuddy[nCntBuddy].nCntFrame /
			(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame
			);

		g_aBuddy[nCntBuddy].aModel[nCntParts].rot.y =
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)g_aBuddy[nCntBuddy].nCntFrame /
			(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame
			);

		g_aBuddy[nCntBuddy].aModel[nCntParts].rot.z =
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)g_aBuddy[nCntBuddy].nCntFrame /
			(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame
			);

		//�p�x�̐��K��
		RotNormalize(&g_aBuddy[nCntBuddy].aModel[g_aBuddy[nCntBuddy].nNowMotionNum].rot.x);
		RotNormalize(&g_aBuddy[nCntBuddy].aModel[g_aBuddy[nCntBuddy].nNowMotionNum].rot.y);
		RotNormalize(&g_aBuddy[nCntBuddy].aModel[g_aBuddy[nCntBuddy].nNowMotionNum].rot.z);


		//�p�[�c�̈ʒu��ݒ�
		if (nCntParts == 0 && 
			(GetMode() == MODE_TITLE || GetMode() == MODE_TUTORIAL || (GetMode() == MODE_GAME && GetGameState() != GAMESTATE_BEFOREEND)))
		{
			//�ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.z;

			//�ʒu�␳
			g_aBuddy[nCntBuddy].pos.x +=
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.x +
				posDiffX *
				(
				(float)g_aBuddy[nCntBuddy].nCntFrame /
				(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame);

			g_aBuddy[nCntBuddy].pos.y =
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.y +
				posDiffY *
				(
				(float)g_aBuddy[nCntBuddy].nCntFrame /
				(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame);

			g_aBuddy[nCntBuddy].pos.z +=
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.z +
				posDiffZ *
				(
				(float)g_aBuddy[nCntBuddy].nCntFrame /
				(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame);
		}
	}

	//�t���[���̃J�E���g�����Z
	g_aBuddy[nCntBuddy].nCntFrame++;
	g_aBuddy[nCntBuddy].nCntAllFrame++;

	if (g_aBuddy[nCntBuddy].nCntFrame >= g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame)
	{//�t���[���̃J�E���g���t���[�����ɒB������

		//�t���[���̃J�E���g���[���ɖ߂�
		g_aBuddy[nCntBuddy].nCntFrame = 0;

		//�p�^�[��NO.�X�V
		g_aBuddy[nCntBuddy].nPatternKey = (g_aBuddy[nCntBuddy].nPatternKey + 1) % g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nNumKey;

		if (g_aBuddy[nCntBuddy].nPatternKey == 0)
		{//�L�[���[���̎�

			//�U����ԉ���
			g_aBuddy[nCntBuddy].bATK = false;
			g_aBuddy[nCntBuddy].nCntAllFrame = 0;	//���t���[�������Z�b�g

			if (g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//���[�V�����̐ݒ�
				SetMotisonBuddy(nCntBuddy, PLAYERMOTION_DEF);
			}
			else
			{
				//���[�V�����̐ݒ�
				SetMotisonBuddy(nCntBuddy, g_aBuddy[nCntBuddy].nNowMotionNum);
			}
		}
	}

}

//==================================================================================
//���_�̕`�揈��
//==================================================================================
void DrawBuddy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxnParent;			//�e�̌v�Z�p�}�g���b�N�X

	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		if (g_aBuddy[nCntBuddy].bUse == true)
		{//�g�p���Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aBuddy[nCntBuddy].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBuddy[nCntBuddy].rot.y, g_aBuddy[nCntBuddy].rot.x, g_aBuddy[nCntBuddy].rot.z);
			D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].mtxWorld, &g_aBuddy[nCntBuddy].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aBuddy[nCntBuddy].pos.x, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z);
			D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].mtxWorld, &g_aBuddy[nCntBuddy].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aBuddy[nCntBuddy].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			for (int nCount = 0; nCount < g_aBuddy[nCntBuddy].nPartsNum; nCount++)
			{
				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld);

				//�����𔽉f����
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBuddy[nCntBuddy].aModel[nCount].rot.y, g_aBuddy[nCntBuddy].aModel[nCount].rot.x, g_aBuddy[nCntBuddy].aModel[nCount].rot.z);
				D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &mtxRot);

				//�ʒu�𔽉f����
				D3DXMatrixTranslation(&mtxTrans, g_aBuddy[nCntBuddy].aModel[nCount].pos.x, g_aBuddy[nCntBuddy].aModel[nCount].pos.y, g_aBuddy[nCntBuddy].aModel[nCount].pos.z);
				D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &mtxTrans);

				if (g_aBuddy[nCntBuddy].aModel[nCount].nParent == -1)
				{//NULL�̏ꍇ,�e

					//���_�̌����𔽉f����
					mtxnParent = g_aBuddy[nCntBuddy].mtxWorld;
				}
				else
				{//�p�[�c

					//�e�̌����𔽉f����
					mtxnParent = g_aBuddy[nCntBuddy].aModel[g_aBuddy[nCntBuddy].aModel[nCount].nParent].mtxWorld;
				}

				//�����̂�ɐe�̃��[���h�}�g���b�N�X���|���Z
				D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &mtxnParent);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aBuddy[nCntBuddy].aModel[nCount].pBuffMat->GetBufferPointer();

				if (g_aBuddy[nCntBuddy].aModel[nCount].bUse == true)
				{//�p�[�c���g�p����Ă�����

					//���_�����J��Ԃ�
					for (int nCntMat = 0; nCntMat < (int)g_aBuddy[nCntBuddy].aModel[nCount].dwNumMat; nCntMat++)
					{
						//�}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//�e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_aBuddy[nCntBuddy].aModel[nCount].pTexture[nCntMat]);

						//���_(�p�[�c)�̕`��
						g_aBuddy[nCntBuddy].aModel[nCount].pMesh->DrawSubset(nCntMat);
					}
				}
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//���_�̏��擾
//==================================================================================
Buddy *GetBuddy(void)
{
	return &g_aBuddy[0];
}

//==================================================================================
//���_�̐ݒ菈��
//==================================================================================
void SetBuddy(int nCntBuddy)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParts = 0; nCntParts < g_aBuddy[nCntBuddy].nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

		//X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aBuddy[nCntBuddy].aModel[nCntParts].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aBuddy[nCntBuddy].aModel[nCntParts].pBuffMat,
			NULL,
			&g_aBuddy[nCntBuddy].aModel[nCntParts].dwNumMat,
			&g_aBuddy[nCntBuddy].aModel[nCntParts].pMesh);

		D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aBuddy[nCntBuddy].aModel[nCntParts].pBuffMat->GetBufferPointer();

		//���_�����J��Ԃ�
		for (int nCntMat = 0; nCntMat < (int)g_aBuddy[nCntBuddy].aModel[nCntParts].dwNumMat; nCntMat++)
		{
			g_aBuddy[nCntBuddy].aModel[nCntParts].pTexture[nCntMat] = NULL;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�t�@�C�������݂���

				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aBuddy[nCntBuddy].aModel[nCntParts].pTexture[nCntMat]);
			}
		}

		if (g_aBuddy[nCntBuddy].aModel[nCntParts].nFever != 0)
		{
			//�p�[�c���g�p���
			g_aBuddy[nCntBuddy].aModel[nCntParts].bUse = true;
		}
		else
		{
			g_aBuddy[nCntBuddy].aModel[nCntParts].bUse = false;
		}

		//���f���̑S���_�`�F�b�N
		CheckVtx(g_aBuddy[nCntBuddy].aModel[nCntParts].rot.y, &g_aBuddy[nCntBuddy].aModel[nCntParts].vtxMax, &g_aBuddy[nCntBuddy].aModel[nCntParts].vtxMin, g_aBuddy[nCntBuddy].aModel[nCntParts].pMesh, g_aBuddy[nCntBuddy].aModel[nCntParts].pVtxBuff);
	}

	//�e�̐ݒ菈��
	if (GetMode() != MODE_TITLE)
	{
		g_aBuddy[nCntBuddy].nIdxShadow = SetShadow(g_aBuddy[nCntBuddy].fRadius * 0.5f, g_aBuddy[nCntBuddy].fRadius * 0.5f);
		SetPositionShadow(g_aBuddy[nCntBuddy].nIdxShadow, g_aBuddy[nCntBuddy].pos);
	}

	//�A�C�R���̐ݒ菈��
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:

			//�A�C�R���̐ݒ菈��
			g_aBuddy[nCntBuddy].nIdxIcon = SetIcon(MINIMAP_ICON_CHIBI, 100.0f);
			break;

		case BUDDYTYPE_DEPPA:

			//�A�C�R���̐ݒ菈��
			g_aBuddy[nCntBuddy].nIdxIcon = SetIcon(MINIMAP_ICON_KODEBU, 100.0f);
			break;
		}
		UpdatePositionIcon(g_aBuddy[nCntBuddy].nIdxIcon, g_aBuddy[nCntBuddy].pos);
	}
}

//==================================================================================
//���_�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetBuddy(void)
{

	//���_�����J��Ԃ�
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

		//�t�@�C�����J��
		switch (nCntBuddy)
		{
		case 0:
			pFile = fopen(BUDDY1_TXT, "r");
			break;

		case 1:
			pFile = fopen(BUDDY2_TXT, "r");
			break;
		}


		if (pFile != NULL)
		{//�t�@�C�����J�����ꍇ

			char aComment[MAX_COMMENT];	//�R�����g�A�m�F��

			int nCntSetParts = 0;
			int nCntFileName = 0;

			while (1)
			{//END_SCRIPT������܂ŌJ��Ԃ�

				//������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);

				//���f�����̐ݒ�
				if (strcmp(aComment, "NUM_MODEL") == 0)
				{//NUM_MODEL��������

					fscanf(pFile, "%s", &aComment[0]);	//=�̕�
					fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].nPartsNum);	//���f����
				}

				while (nCntFileName != g_aBuddy[nCntBuddy].nPartsNum)
				{//���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

					//������̓ǂݍ���
					fscanf(pFile, "%s", &aComment[0]);

					//���f�����̐ݒ�
					if (strcmp(aComment, "MODEL_FILENAME") == 0)
					{//NUM_MODEL��������

						fscanf(pFile, "%s", &aComment[0]);	//=�̕�
						fscanf(pFile, "%s", &g_aBuddy[nCntBuddy].aModel[nCntFileName].acFilename[0]);	//�t�@�C����
						nCntFileName++;	//�t�@�C�������Z
					}
				}

				//�e�p�[�c�̐ݒ�
				if (strcmp(aComment, "CHARACTERSET") == 0)
				{//�L�����N�^�[���̓ǂݍ��݂��J�n

					while (strcmp(aComment, "END_CHARACTERSET") != 0)
					{//END_CHARACTERSET������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	//�m�F����

						if (strcmp(aComment, "PARTSSET") == 0)
						{//PARTSSET�Ńp�[�c���̓ǂݍ��݊J�n

							while (strcmp(aComment, "END_PARTSSET") != 0)
							{//END_PARTSSET������܂ŌJ��Ԃ�

								fscanf(pFile, "%s", &aComment[0]);	//�m�F����

								if (strcmp(aComment, "INDEX") == 0)
								{//INDEX�������烂�f���ԍ��ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		//=�̕�
									fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].nType);	//���f����ނ̗�

								}

								if (strcmp(aComment, "PARENT") == 0)
								{//PARENT��������e�̔ԍ��ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		//=�̕�
									fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].nParent);	//�e�̔ԍ�

								}

								if (strcmp(aComment, "POS") == 0)
								{//POS��������ʒu�ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		//=�̕�
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].pos.x);	//X���W
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].pos.y);	//Y���W
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].pos.z);	//Z���W

									//���̈ʒu���蓖��
									g_aBuddy[nCntBuddy].aModel[nCntSetParts].posOrigin = g_aBuddy[nCntBuddy].aModel[nCntSetParts].pos;

								}

								if (strcmp(aComment, "ROT") == 0)
								{//ROT������������ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		//=�̕�
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].rot.x);	//X�̊p�x
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].rot.y);	//Y�̊p�x
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].rot.z);	//Z�̊p�x

								}

								if (strcmp(aComment, "WEAPON") == 0)
								{//WEAPON�������畐�킩�ǂ����ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		//=�̕�

								}

								if (strcmp(aComment, "FEVER") == 0)
								{//WEAPON�������畐�킩�ǂ����ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		//=�̕�
									fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].nFever);		//��ɏo�����t�B�[�o�[���ɏo����������
									
								}

							}//END_PARTSSET�̂�����

							nCntSetParts++;	//�p�[�c�̃J�E���g�����Z
						}

					}//END_CHARACTERSET�̂�����
				}

				if (strcmp(aComment, "END_SCRIPT") == 0)
				{//�I�������Ń��[�v�𔲂���

					break;
				}
			}

			//�t�@�C�������
			fclose(pFile);
		}

		//���_�̔z�u
		SetBuddy(nCntBuddy);
	}
}

//==================================================================================
//���_�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetMotionBuddy(void)
{

	//���_�����J��Ԃ�
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

		char aComment[MAX_COMMENT];	//�R�����g�p

		int nCntMotion = 0, nCntKey = 0, nCntParts = 0;

		//�t�@�C�����J��
		switch (nCntBuddy)
		{
		case 0:
			pFile = fopen(BUDDY1_TXT, "r");
			break;

		case 1:
			pFile = fopen(BUDDY2_TXT, "r");
			break;
		}

		if (pFile != NULL)
		{//�t�@�C�����J�����ꍇ

			while (1)
			{//END_SCRIPT������܂ŌJ��Ԃ�

				//������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);

				//==================================================
				//�e���[�V�����̐ݒ�
				//==================================================
				if (strcmp(aComment, "MOTIONSET") == 0)
				{//���[�V�������̓ǂݍ��݂��J�n

					while (strcmp(aComment, "END_MOTIONSET") != 0)
					{//END_MOTIONSET������܂ŌJ��Ԃ�

						fscanf(pFile, "%s", &aComment[0]);	//�m�F����

						if (strcmp(aComment, "LOOP") == 0)
						{//LOOP�������烋�[�vON/OFF�ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		//=�̕�
							fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].nLoop);	//���[�v0��1
						}

						if (strcmp(aComment, "NUM_KEY") == 0)
						{//NUM_KEY��������L�[���ǂݍ���

							fscanf(pFile, "%s", &aComment[0]);		//=�̕�
							fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].nNumKey);	//�L�[��
						}

						//==================================================
						//�e�L�[�̐ݒ�
						//==================================================
						if (strcmp(aComment, "KEYSET") == 0)
						{//KEYSET�ŃL�[���̓ǂݍ��݊J�n

							while (strcmp(aComment, "END_KEYSET") != 0)
							{//END_KEYSET������܂ŌJ��Ԃ�

								fscanf(pFile, "%s", &aComment[0]);	//�m�F����

								if (strcmp(aComment, "FRAME") == 0)
								{//FRAME��������Đ��t���[�����ǂݍ���

									fscanf(pFile, "%s", &aComment[0]);		//=�̕�
									fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//�Đ��t���[��
								}

								//==================================================
								//�p�[�c�̐ݒ�
								//==================================================
								if (strcmp(aComment, "PARTS") == 0)
								{//PARTS�Ńp�[�c���̓ǂݍ��݊J�n

									while (strcmp(aComment, "END_PARTS") != 0)
									{//END_PARTS������܂ŌJ��Ԃ�

										fscanf(pFile, "%s", &aComment[0]);	//�m�F����

										if (strcmp(aComment, "POS") == 0)
										{//POS��������ʒu�ǂݍ���

											fscanf(pFile, "%s", &aComment[0]);		//=�̕�
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X���W
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y���W
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z���W
										}

										if (strcmp(aComment, "ROT") == 0)
										{//ROT��������p�x�ǂݍ���

											fscanf(pFile, "%s", &aComment[0]);		//=�̕�
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X�p�x
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y�p�x
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z�p�x
										}

										if (strcmp(aComment, "FEVER") == 0)
										{//ROT��������p�x�ǂݍ���

											fscanf(pFile, "%s", &aComment[0]);		//=�̕�
									
										}

									}//END_PARTS�̂�����

									nCntParts++;	//�p�[�c�̃J�E���g���Z
								}


							}//END_KEYSET�̂�����

							nCntParts = 0;	//�p�[�c�̃J�E���g���Z�b�g
							nCntKey++;	//�L�[�̃J�E���g���Z
						}

					}//END_MOTIONSET�̂�����

					nCntKey = 0;	//�L�[�̃J�E���g���Z�b�g
					nCntMotion++;	//���[�V�����̃J�E���g���Z
				}

				if (strcmp(aComment, "END_SCRIPT") == 0)
				{//�I�������Ń��[�v�𔲂���

					break;
				}
			}

			//�t�@�C�������
			fclose(pFile);

			//���[�V�����̐ݒ�
			SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
		}

	}
}

//==================================================================================
//���_�̓ǂݍ��ݏ���
//==================================================================================
void SetBuddyDispatch(D3DXVECTOR3 pos, int nType)
{

	//�Q�O�̏����擾
	Crowd *pCrowd = GetCrowd();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	int nCntUse = 0;
	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; pCrowd++, nCntCrowd++)
	{
		if (pCrowd->aModel.bUse == true)
		{
			if (g_aBuddy[nType].nState != BUDDYSTATE_DISPATCH && g_aBuddy[(nType + 1) % BUDDYTYPE_MAX].nState != BUDDYSTATE_DISPATCH)
			{//�ǂ������h������Ă��Ȃ��Ƃ�

				if (nCntUse > (pCrowdInfo->nCrowdValue / 3) * 2 && pCrowd->nParent == PARENT_PLAYER)
				{//3���̂Q��������e���j�[�g�ɂ���

					//�����ύX����
					pPlayer->aFormation[pCrowd->aFormation.nIndex].bUse = false;

					//����̐ݒ�
					pCrowd->aFormation.nIndex = SetFormationPos(nType + 1);

					//�e�̎�ސݒ�
					pCrowd->nParent = nType + 1;
				}
			}
			else if (g_aBuddy[nType].nState != BUDDYSTATE_DISPATCH && g_aBuddy[(nType + 1) % BUDDYTYPE_MAX].nState == BUDDYSTATE_DISPATCH)
			{//����̂�����h������Ă��Ȃ��Ƃ�

				if (nCntUse > (pCrowdInfo->nCrowdValue / 3) * 1 && nCntUse <= (pCrowdInfo->nCrowdValue / 3) * 2 && pCrowd->nParent == PARENT_PLAYER)
				{//3����1�`3����2��e�j�[�g�ɂ���

					//�����ύX����
					pPlayer->aFormation[pCrowd->aFormation.nIndex].bUse = false;

					//����̐ݒ�
					pCrowd->aFormation.nIndex = SetFormationPos(nType + 1);

					//�e�̎�ސݒ�
					pCrowd->nParent = nType + 1;
				}
			}

			nCntUse++;
		}
	}

	g_aBuddy[nType].formationPos = pos;
	g_aBuddy[nType].nState = BUDDYSTATE_DISPATCH;
}

//==================================================================================
//�Q�[���I�����̍X�V����
//==================================================================================
void UpdateGameEndBuddy(int nCntBuddy)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�����Œ�
	g_aBuddy[nCntBuddy].rot.y = 0.0f;
	g_aBuddy[nCntBuddy].fRotDiff = 0.0f;		//�����̍���
	g_aBuddy[nCntBuddy].fRotDest = g_aBuddy[nCntBuddy].rot.y;		//�����̍���

	//���Ɍ������đ���
	g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
	g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
	g_aBuddy[nCntBuddy].bMove = true;

	//�ʒu�X�V
	g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

	//��������
	g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
	g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;

}

//==================================================================================
//���_�̑���(�`���[�g���A��)
//==================================================================================
void ControllTutorialBuddy(int nCntBuddy)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	if (pPlayer->nNowMotionNum != PLAYERMOTION_ACTION && g_aBuddy[nCntBuddy].aMoveAI.bPosDest == false)
	{//�A�N�V�������Ă��Ȃ��Ƃ� && �ړI�n�ɂ��ĂȂ��Ƃ�

		if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (BUDDY_DISTANCE * BUDDY_DISTANCE))
		{//�܂��K��l�ɒ����Ă��Ȃ�������

			//�ړ��ʉ��Z
			if (g_aBuddy[nCntBuddy].nState != BUDDYSTATE_RETURN)
			{//�A�ҏ�Ԃ���Ȃ��Ƃ�

				g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
				g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
			}
			else if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_RETURN)
			{//�A�ҏ�Ԃ̂Ƃ�

				g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * RETURN_MOVE;
				g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * RETURN_MOVE;
			}
		}
	}

	if ((GetKeyboardPress(DIK_L) == true || GetGamepadPress(BUTTON_A, 0)) &&
		g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH &&
		pTutorial->nStep >= TUTORIAL_STEP_HYPNOSIS)
	{//L�������ꂽ || �y�����C�g���g��Ăꂢ��Ƃ�

		//�U�����Ă��Ԃɂ���
		g_aBuddy[nCntBuddy].bATK = true;

		if (g_aBuddy[nCntBuddy].move.x <= 1.0f && g_aBuddy[nCntBuddy].move.x >= -1.0f &&
			g_aBuddy[nCntBuddy].move.z <= 1.0f && g_aBuddy[nCntBuddy].move.z >= -1.0f)
		{//�~�܂��Ă���Ƃ��͋���

			g_aBuddy[nCntBuddy].bMove = false;
			g_aBuddy[nCntBuddy].move.x = 0.0f;
			g_aBuddy[nCntBuddy].move.z = 0.0f;

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION && g_aBuddy[nCntBuddy].bFever == false)
			{
				//���[�V�����̐ݒ�
				SetMotisonBuddy(nCntBuddy, BUDDYMOTION_ACTION);
			}

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION && g_aBuddy[nCntBuddy].bFever == true)
			{
				//���[�V�����̐ݒ�
				SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERACTION);
			}
		}
		else
		{//�����Ă�Ƃ��͎�

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION && g_aBuddy[nCntBuddy].bFever == false)
			{
				//���[�V�����̐ݒ�
				SetMotisonBuddy(nCntBuddy, BUDDYMOTION_MOVEACTION);
			}

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION && g_aBuddy[nCntBuddy].bFever == true)
			{
				//���[�V�����̐ݒ�
				SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERMOVEACTION);
			}
		}
	}

	if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
	{//L�������ꂽ && A�{�^���������ꂽ

		//�U����ԉ���
		g_aBuddy[nCntBuddy].bATK = false;

		//���[�V�����̐ݒ�
		SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
	}

	if (g_aBuddy[nCntBuddy].aMoveAI.bPenUse == true)
	{//�y�����C�g���g��Ăꂢ��Ƃ�

		//�U�����Ă��Ԃɂ���
		g_aBuddy[nCntBuddy].bATK = true;
		g_aBuddy[nCntBuddy].move.x = 0.0f;
		g_aBuddy[nCntBuddy].move.z = 0.0f;
	}

	if (g_aBuddy[nCntBuddy].move.x >= 1.0f || g_aBuddy[nCntBuddy].move.x <= -1.0f || g_aBuddy[nCntBuddy].move.z >= 1.0f || g_aBuddy[nCntBuddy].move.z <= -1.0f)
	{//�ړ����Ă�Ƃ�

		g_aBuddy[nCntBuddy].bMove = true;
	}
	else
	{//�ړ����ĂȂ��Ƃ�

		g_aBuddy[nCntBuddy].bMove = false;
	}

	//�d�͏���
	g_aBuddy[nCntBuddy].move.y -= 1.0f;
	g_aBuddy[nCntBuddy].move.y = 0.0f;

	if (g_aBuddy[nCntBuddy].bFever == true || (g_aBuddy[nCntBuddy].bCollisionModel == false && g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH))
	{//�t�B�[�o�[���Ȃ�

		//����
		g_aBuddy[nCntBuddy].move.x *= 1.1f;
		g_aBuddy[nCntBuddy].move.z *= 1.1f;
	}

	//�ʒu�X�V
	g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

	//��������
	if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION)
	{
		g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
		g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;
	}
	else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION)
	{
		g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR_HYPNOSIS;
		g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR_HYPNOSIS;
	}
}

//==================================================================================
//���_�̍X�V����
//==================================================================================
void UpdateTitleBuddy(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�^�C�g���̏��擾
	Title *pTitle = GetTitle();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		if (g_aBuddy[nCntBuddy].bUse == true)
		{//�g�p���Ă�����

			//�ߋ��̈ʒu�ۑ�
			g_aBuddy[nCntBuddy].posOld = g_aBuddy[nCntBuddy].pos;

			if (pTitle->nState != TITLESTATE_TKTK && pTitle->nState != TITLESTATE_FADE)
			{
				if (pTitle->nSelect == TITLESELECT_START)
				{
					g_aBuddy[BUDDYTYPE_HYOROGARI].formationPos.z = -300.0f;
					g_aBuddy[BUDDYTYPE_HYOROGARI].formationPos.x = -100.0f;
					g_aBuddy[BUDDYTYPE_DEPPA].formationPos.z = 0.0f;
					g_aBuddy[BUDDYTYPE_DEPPA].formationPos.x = 100.0f;
				}
				else
				{
					g_aBuddy[BUDDYTYPE_HYOROGARI].formationPos.z = 0.0f;
					g_aBuddy[BUDDYTYPE_HYOROGARI].formationPos.x = -100.0f;
					g_aBuddy[BUDDYTYPE_DEPPA].formationPos.z = -300.0f;
					g_aBuddy[BUDDYTYPE_DEPPA].formationPos.x = 100.0f;
				}
			}

			if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (BUDDY_DISTANCE * BUDDY_DISTANCE) && g_aBuddy[nCntBuddy].aMoveAI.bPosDest == false)
			{//�܂��ړI�n�ɂ��ĂȂ��Ƃ�

				//�ڕW�̊p�x�����߂�
				g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].formationPos.x), (g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].formationPos.z));

				//�p�x�̐��K��
				RotNormalize(&g_aBuddy[nCntBuddy].fRotDest);

				//�ړI�n��������
				RotCorrect(nCntBuddy);

				//�j�[�g�����ړ�
				g_aBuddy[nCntBuddy].move.x += sinf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI) * CHARA_MOVE;
				g_aBuddy[nCntBuddy].move.z += cosf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI) * CHARA_MOVE;
			}
			else
			{
				g_aBuddy[nCntBuddy].fRotDest = 0.0f;

				//�p�x�̐��K��
				RotNormalize(&g_aBuddy[nCntBuddy].fRotDest);

				RotCorrect(nCntBuddy);
				g_aBuddy[nCntBuddy].move.x = 0.0f;
				g_aBuddy[nCntBuddy].move.z = 0.0f;

				if (pTitle->nState != TITLESTATE_TKTK)
				{//�`�J�`�J����Ȃ��Ƃ�

					if (pTitle->nSelect == TITLESELECT_START)
					{//�I�������X�^�[�g�̎�

						if (g_aBuddy[BUDDYTYPE_DEPPA].nNowMotionNum != BUDDYMOTION_DEF)
						{
							//���[�V�����̐ݒ�
							SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_DEF);
						}

						if (g_aBuddy[BUDDYTYPE_HYOROGARI].nNowMotionNum != BUDDYMOTION_ACTION)
						{
							//���[�V�����̐ݒ�
							SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_ACTION);
						}
					}
					else
					{//�I���̎�

						if (g_aBuddy[BUDDYTYPE_HYOROGARI].nNowMotionNum != BUDDYMOTION_DEF)
						{
							//���[�V�����̐ݒ�
							SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_DEF);
						}

						if (g_aBuddy[BUDDYTYPE_DEPPA].nNowMotionNum != BUDDYMOTION_ACTION)
						{
							//���[�V�����̐ݒ�
							SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_ACTION);
						}
					}
				}
				else
				{
					if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_DEF)
					{
						//���[�V�����̐ݒ�
						SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
					}
				}
			}

			//�ʒu�X�V
			g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

			//��������
			g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
			g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;
			g_aBuddy[nCntBuddy].bMove = true;

			if (g_aBuddy[nCntBuddy].move.x <= 1.0f && g_aBuddy[nCntBuddy].move.x >= -1.0f &&
				g_aBuddy[nCntBuddy].move.z <= 1.0f && g_aBuddy[nCntBuddy].move.z >= -1.0f)
			{//�~�܂��Ă���Ƃ��͋���

				g_aBuddy[nCntBuddy].bMove = false;
			}

			if (g_aBuddy[nCntBuddy].bMove == true)
			{//�ړ����Ă�Ƃ�

				g_aBuddy[nCntBuddy].nCntWalk++;	//�J�E���g���Z

				if (g_aBuddy[nCntBuddy].nCntWalk > 15)
				{
					g_aBuddy[nCntBuddy].nCntWalk = 0;

					//���s�p�[�e�B�N���̐ݒ�
					SetParticle(g_aBuddy[nCntBuddy].pos, PARTICLE_TYPE_WALK);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_WALK)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_WALK);
				}
			}

			//���[�V�����̍X�V
			UpdateMotionBuddy(nCntBuddy);

			//�L�����N�^�[�Ƃ̓����蔻��
			CollisionCharBuddy(nCntBuddy);

			//���W����
			LimitPosBuddy(nCntBuddy);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aBuddy[nCntBuddy].nIdxShadow, g_aBuddy[nCntBuddy].pos);
		}
	}
}

//==================================================================================
//���_�̍X�V����
//==================================================================================
void UpdateResultBuddy(void)
{
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION)
		{

			//���[�V�����̐ݒ�
			SetMotisonBuddy(nCntBuddy, BUDDYMOTION_ACTION);
		}

		//���[�V�����̍X�V
		UpdateMotionBuddy(nCntBuddy);

		g_aBuddy[nCntBuddy].pos.y = 76.0f;
	}
}

//==================================================================================
//�����L���O�̍X�V����
//==================================================================================
void UpdateRankingBuddy(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		if (g_aBuddy[nCntBuddy].bUse == true)
		{//�g�p���Ă�����

			//�ߋ��̈ʒu�ۑ�
			g_aBuddy[nCntBuddy].posOld = g_aBuddy[nCntBuddy].pos;

			//�����Œ�
			g_aBuddy[nCntBuddy].rot.y += 0.025f;

			RotNormalize(&g_aBuddy[nCntBuddy].rot.y);

			g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * 1.0f;
			g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * 1.0f;

			g_aBuddy[nCntBuddy].bMove = true;

			//�ʒu�X�V
			g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

			//��������
			g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
			g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;

			if (g_aBuddy[nCntBuddy].bMove == true)
			{//�ړ����Ă�Ƃ�

				g_aBuddy[nCntBuddy].nCntWalk++;	//�J�E���g���Z

				if (g_aBuddy[nCntBuddy].nCntWalk > 15)
				{
					g_aBuddy[nCntBuddy].nCntWalk = 0;

					//���s�p�[�e�B�N���̐ݒ�
					SetParticle(g_aBuddy[nCntBuddy].pos, PARTICLE_TYPE_WALK);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_RANKING)
				{
					//���[�V�����̐ݒ�
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_RANKING);
				}
			}

			//���[�V�����̍X�V
			UpdateMotionBuddy(nCntBuddy);

			//���W����
			LimitPosBuddy(nCntBuddy);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aBuddy[nCntBuddy].nIdxShadow, g_aBuddy[nCntBuddy].pos);
		}
	}
}
