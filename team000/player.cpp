//=============================================================================
//
// �v���C���[���� [player.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "enemy.h"
#include "meshwall.h"
#include "game.h"
#include "edit.h"
#include "tutorial.h"
#include "impactwave.h"
#include "calculation.h"
#include "dispatch.h"
#include "cutepillow.h"
#include "shadow.h"
#include "meshorbit.h"
#include "mob.h"
#include "minimap_icon.h"
#include "particle.h"
#include "debugproc.h"
#include "serif.h"
#include "sound.h"
#include "updown_fram.h"
#include "start_talk.h"
#include "manypoint.h"
#include "flag.h"


//�}�N����`
#define DMG_TIME	(10)	//�_���[�W��Ԃ̎���
#define ULT_WAIT	(100)	//�҂����
#define FLAG_POS	(150.0f)	//���̈ʒu
#define FLEN            (800.0f)
#define PLAYER_TXT	"data\\TEXT\\motion_set_player.txt"

//�v���g�^�C�v�錾
void UpdateStartPlayer(void);
void UpdateGamePlayer(void);
void ControllEndPlayer(void);
void ControllPlayer(void);
void ReadSetPlayer(void);
void ReadSetMotionPlayer(void);
void UpdateMotionPlayer(void);
void LimitPosPlayer(void);
void CollisionCharPlayer(void);
void CollisionWallPlayer(void);
void UpdateStatePlayer(void);
void UpdateATKGun(void);
void UpdateATKPenLight(void);
void CollisionATKPlayer(float fDistance, float fAngle, int nValue);
void CalFormationPosPlayer(void);
int SetHypnosisWavePlayer(float fRot, float fWidthMove);

void ControllTutorialPlayer(void);

//�O���[�o���ϐ��錾
Player g_aPlayer;		//�v���C���[�̏��
MotionData g_aMotionData[MAX_MOTION];		//���[�V�����p�̏��
int g_nGameTimePlayer;

//==================================================================================
//�v���C���[�̏���������
//==================================================================================
void InitPlayer(void)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�v�f������
	if (GetMode() == MODE_GAME)
	{
		g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, -800.0f);
		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);
		g_aPlayer.bUse = true;										//�g�p���Ă��邩
	}
	else if (GetMode() == MODE_TITLE)
	{
		g_aPlayer.pos = D3DXVECTOR3(-150.0f, 0.0f, 0.0f);
		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);
		g_aPlayer.bUse = false;										//�g�p���Ă��邩
	}
	else if (GetMode() == MODE_RANKING)
	{
		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);

		//���S����D3DX_PI * -0.5f�̕�����-150.0f
		g_aPlayer.pos = D3DXVECTOR3(sinf(D3DX_PI * -0.5f + g_aPlayer.rot.y) * -150.0f, 0.0f, cosf(D3DX_PI * -0.5f + g_aPlayer.rot.y) * -150.0f);
		g_aPlayer.bUse = true;										//�g�p���Ă��邩
	}
	else if (GetMode() == MODE_RESULT)
	{
		g_aPlayer.pos = D3DXVECTOR3(-25.0f, 143.0f, 300.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, pCamera->rot.y, 0.0f);
	}
	else
	{
		g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.bUse = true;										//�g�p���Ă��邩
	}
	g_aPlayer.vecMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��x�N�g��
	g_aPlayer.vecLine[4] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f) };	//���E���x�N�g��
	g_aPlayer.vecToPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�x�N�g���̒���
	g_aPlayer.fRotDiff = 0.0f;									//�����̍���
	g_aPlayer.fRotDest = g_aPlayer.rot.y;						//�����̍���
	g_aPlayer.fRot[4] = { 0.0f };								//�x�N�g���̌���

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);
	g_aPlayer.nIdxShadow = -1;									//�e�̃C���f�b�N�X�ԍ�
	g_aPlayer.nIdxHPGauge = -1;									//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�

	for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
	{
		g_aPlayer.nIdxWave[nCntWave] = -1;						//�Ռ��g�̃C���f�b�N�X�ԍ�
	}

	g_aPlayer.nIdxIcon = -1;									//�~�j�}�b�v�A�C�R���̃C���f�b�N�X�ԍ�
	g_aPlayer.nIdxSerif = -1;									//�Z���t�̃C���f�b�N�X�ԍ�
	g_aPlayer.nIdxFlag = -1;									//���̃C���f�b�N�X�ԍ�
	g_aPlayer.nState = PLAYERSTATE_NONE;						//���
	g_aPlayer.nCntState = 0;									//��ԃJ�E���g
	g_aPlayer.bDisp = true;										//�`�悵�Ă��邩
	g_aPlayer.bFever = false;									//�t�B�[�o�[��Ԃ�
	g_aPlayer.bSetTalk = false;			//��b�̃Z�b�g������


	//���[�V�����n������
	g_aPlayer.nNowMotionNum = -1;		//���݂̃��[�V�����ԍ�
	g_aPlayer.nCntAllFrame = 0;			//���t���[���J�E���g
	g_aPlayer.nCntFrame = 0;			//�t���[���̃J�E���g
	g_aPlayer.nPatternKey = 0;			//���ڂ̃L�[��
	g_aPlayer.nPartsNum = 0;			//�p�[�c��
	g_aPlayer.bMove = false;			//�ړ����Ă��邩
	g_aPlayer.bJump = false;			//�W�����v���Ă��邩
	g_aPlayer.bATK = false;				//�U�����Ă邩

	g_aPlayer.nCntWalk = 0;				//�ړ��̃J�E���g
	g_aPlayer.nCntPenlight = 0;			//�Ö��̃J�E���g
	g_aPlayer.nCntDis = 0;				//�h���̃J�E���g
	g_aPlayer.nCntReturn = 0;			//�A�҂̃J�E���g
	g_aPlayer.fRadius = 50.0f;			//���a

	for (int nCount = 0; nCount < MAX_MODEL; nCount++)
	{
		g_aPlayer.aModel[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���݂̈ʒu
		g_aPlayer.aModel[nCount].posOld = g_aPlayer.aModel[nCount].pos;			//�O��̈ʒu
		g_aPlayer.aModel[nCount].posOrigin = g_aPlayer.aModel[nCount].pos;		//���̈ʒu
		g_aPlayer.aModel[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
		g_aPlayer.aModel[nCount].rotOrigin = g_aPlayer.aModel[nCount].rot;		//���̌���
		g_aPlayer.aModel[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ���

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].OldmtxWorld);

		g_aPlayer.aModel[nCount].nIdxShadow = -1;		//�e�̃C���f�b�N�X�ԍ�
		g_aPlayer.aModel[nCount].nIdxRotate = -1;		//���邮��̃C���f�b�N�X�ԍ�
		g_aPlayer.aModel[nCount].nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
		g_aPlayer.aModel[nCount].nShadow = -1;			//�e���g�����ǂ���
		g_aPlayer.aModel[nCount].nFever = -1;				//�e���g�����ǂ���

		g_aPlayer.aModel[nCount].nNumVtx = 0;			//���_��
		g_aPlayer.aModel[nCount].dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aPlayer.aModel[nCount].pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aPlayer.aModel[nCount].vtxMin = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);		//���f���̍ŏ��l
		g_aPlayer.aModel[nCount].vtxMax = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);			//���f���̍ő�l

		g_aPlayer.aModel[nCount].nType = -1;		//�ǂݍ��ރ��f���̎��
		g_aPlayer.aModel[nCount].nParent = -1;		//�e�̔ԍ�
		g_aPlayer.aModel[nCount].bUse = false;		//�g�p���Ă��邩
		g_aPlayer.aModel[nCount].nState = 0;		//���

		g_aPlayer.aModel[nCount].pMesh = NULL;				//���b�V��(���_���)�ւ̃|�C���^
		g_aPlayer.aModel[nCount].pBuffMat = NULL;			//�}�e���A���ւ̃|�C���^

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aPlayer.aModel[nCount].pMatData[nCntMat].MatD3D = {};
			g_aPlayer.aModel[nCount].pMatData[nCntMat].pTextureFilename = {};	//�}�e���A���̃f�[�^
			g_aPlayer.aModel[nCount].acFilename[nCntMat] = {};		//�t�@�C����
		}
		
		g_aPlayer.aModel[nCount].dwNumMat = NULL;			//�}�e���A���̐�

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aPlayer.aModel[nCount].pTexture[nCntTex] = {};		//�e�N�X�`���ւ̃|�C���^
		}

	}

	//������̏�����
	for (int nCntPos = 0; nCntPos < MAX_CROWD; nCntPos++)
	{
		g_aPlayer.aFormation[nCntPos].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.aFormation[nCntPos].nIndex = -1;
		g_aPlayer.aFormation[nCntPos].bUse = false;
	}

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//���[�V�����̐��J��Ԃ�

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{//�L�[�̐��J��Ԃ�

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{//�p�[�c�̐��J��Ԃ�

				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̌���
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����̍���
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̈ʒu
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu�̍���

			}

			g_aMotionData[nCntMotion].aKey[nCntKey].nFrame = 0;	//�Đ��t���[��
		}

		g_aMotionData[nCntMotion].nNumKey = 0;				//�L�[�̐�
		g_aMotionData[nCntMotion].nLoop = MOTION_LOOP_ON;	//���[�v����
		g_aMotionData[nCntMotion].bUse = false;				//�g�p���Ă��邩
	}

	g_nGameTimePlayer = 0;

	//�O���t�@�C���ǂݍ���
	ReadSetPlayer();
	ReadSetMotionPlayer();

	//���[�V�����̐ݒ�
	SetMotisonPlayer(PLAYERMOTION_DEF);
}

//==================================================================================
//�v���C���[�̏I������
//==================================================================================
void UninitPlayer(void)
{

	for (int nCount = 0; nCount < MAX_MODEL; nCount++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aPlayer.aModel[nCount].pTexture[nCntTex] != NULL)
			{
				g_aPlayer.aModel[nCount].pTexture[nCntTex]->Release();
				g_aPlayer.aModel[nCount].pTexture[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_aPlayer.aModel[nCount].pMesh != NULL)
		{
			g_aPlayer.aModel[nCount].pMesh->Release();
			g_aPlayer.aModel[nCount].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aPlayer.aModel[nCount].pBuffMat != NULL)
		{
			g_aPlayer.aModel[nCount].pBuffMat->Release();
			g_aPlayer.aModel[nCount].pBuffMat = NULL;
		}
	}

}

//==================================================================================
//�v���C���[�̍X�V����
//==================================================================================
void UpdatePlayer(void)
{

	if (GetGameState() == GAMESTATE_START)
	{
		//�X�^�[�g���̍X�V����
		UpdateStartPlayer();
	}
	else
	{
		//�Q�[�����̍X�V����
		UpdateGamePlayer();
	}
	
}

//==================================================================================
//�X�^�[�g���̍X�V����
//==================================================================================
void UpdateStartPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�㉺�̘g���擾
	UPDOWN_FRAM *pFram = GetUpDownFram();

	if (g_aPlayer.bUse == true)
	{//�g�p���Ă�����

		if (GetUpDownFram()->bUse == false && GetUpDownFram()->bEnd == false)
		{
			//�g�ړ�
			SetUpDownFram();
		}

		//�ʒu�͌Œ�
		g_aPlayer.rot.y = D3DX_PI + pCamera->rot.y;
		g_aPlayer.fRotDiff = 0.0f;									//�����̍���
		g_aPlayer.fRotDest = g_aPlayer.rot.y;						//�����̍���

		if (pFram->bEnd == true)
		{//�g��������

			if (g_aPlayer.pos.z <= 0.0f)
			{//�����l�ɒ����Ă��Ȃ�������

				g_aPlayer.move.x += sinf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.move.z += cosf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.bMove = true;
			}
			else
			{
				if (g_aPlayer.bSetTalk == false)
				{//�܂��Z�b�g���Ă��Ȃ�������

					g_aPlayer.bSetTalk = true;

					//�ŏ��̉�b�ݒ�
					GetStartTalk()->bUse = true;
				}
			}
		}

		if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f &&
			g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
		{
			g_aPlayer.bMove = false;
		}

		//�ʒu�X�V
		g_aPlayer.pos += g_aPlayer.move;

		//��������
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;


		if (g_aPlayer.bMove == true)
		{//�ړ����Ă�Ƃ�

			g_aPlayer.nCntWalk++;	//�J�E���g���Z

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//���s�p�[�e�B�N���̐ݒ�
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				PlaySound(SOUND_LABEL_SE_ASIOTO);
			}


			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}

		}
		else
		{//�j���[�g�������[�V����

			if (g_aPlayer.bATK == false && g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR)
			{
				//�U����ԉ���
				g_aPlayer.bATK = false;

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}


		//���[�V�����̍X�V
		UpdateMotionPlayer();

		//���W����
		LimitPosPlayer();

		//�e�̈ʒu�X�V
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		//�A�C�R���̈ʒu�X�V
		UpdatePositionIcon(g_aPlayer.nIdxIcon, g_aPlayer.pos);

		//���̈ʒu�X�V
		SetPositionFlag(g_aPlayer.nIdxFlag, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + FLAG_POS, g_aPlayer.pos.z));
	}
}

//==================================================================================
//�Q�[�����̍X�V����
//==================================================================================
void UpdateGamePlayer(void)
{
	if (g_aPlayer.bUse == true)
	{//�g�p���Ă�����

		//�ߋ��̈ʒu�ۑ�
		g_aPlayer.posOld = g_aPlayer.pos;

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//�p�[�c���J��Ԃ�

		 //�p�[�c���Ƃ̃}�g���b�N�X�擾
			D3DXMATRIX mtxParts = GetParentMatrix(nCntParts);

			g_aPlayer.aModel[nCntParts].OldmtxWorld = mtxParts;
		}


		if (GetGameState() == GAMESTATE_BEFOREEND || GetGameState() == GAMESTATE_END)
		{
			//�Q�[���I�����̃v���C���[�̑���
			ControllEndPlayer();
		}
		else
		{
			//�v���C���[�̑���
			ControllPlayer();
		}

		//����̈ʒu�X�V
		CalFormationPosPlayer();

		//�U������
		UpdateATKGun();
		UpdateATKPenLight();

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//�ړ����Ă�Ƃ�

			g_aPlayer.nCntWalk++;	//�J�E���g���Z

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//���s�p�[�e�B�N���̐ݒ�
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				PlaySound(SOUND_LABEL_SE_ASIOTO);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.bFever == false)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERWALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION && g_aPlayer.bFever == true)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_FEVERWALK);
			}
		}
		else
		{//�j���[�g�������[�V����

			if (g_aPlayer.bATK == false && g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
			{
				//�U����ԉ���
				g_aPlayer.bATK = false;

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION || 
			g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION || 
			g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION || 
			g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
		{//�Ö��A�N�V������

			g_aPlayer.nCntPenlight++;

			if (g_aPlayer.nCntPenlight == 20 || g_aPlayer.nCntPenlight == 40)
			{
				if (g_aPlayer.nCntPenlight >= 40)
				{
					g_aPlayer.nCntPenlight = 0;
				}

				//�Ö��̃z�����z�����ݒ�
				if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION)
				{//�~�܂��čÖ�

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
				{//�����čÖ�
					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION)
				{//�����čÖ�

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
				{//�����čÖ�

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
				}
			
				if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
				{//�t�B�[�o�[���Ȃ�

				 //�Ö��̃z�����z�����ݒ�
					if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION)
					{//�~�܂��čÖ�

						g_aPlayer.nIdxWave[1] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[2] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[3] = SetHypnosisWavePlayer(g_aPlayer.rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
					}
					else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
					{//�����čÖ�

						g_aPlayer.nIdxWave[1] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[2] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[3] = SetHypnosisWavePlayer(g_aPlayer.rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
					}
				}
			}
		}
		else if (
			g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
		{//�Ö��A�N�V�������ĂȂ��Ƃ�

			//�Ö��̉�������
			StopSound(SOUND_LABEL_BGM_HYPNOSIS);
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVECURSOR)
		{
			DISPATCH *pDispatch = GetDispatch();

			//�ڕW�̊p�x�����߂�
			g_aPlayer.fRotDest = atan2f((g_aPlayer.pos.x - pDispatch->pos.x), (g_aPlayer.pos.z - pDispatch->pos.z));
			g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

			//�p�x�̐��K��
			g_aPlayer.fRotDiff = RotNormalize(g_aPlayer.fRotDiff);

			//�p�x�̕␳������
			g_aPlayer.rot.y += g_aPlayer.fRotDiff;

			//�p�x�̐��K��
			g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);
		}


		//�O�Ղ̏��擾
		ORBIT *pOrbit = GetMeshOrbit();

		//�p�[�c���Ƃ̃}�g���b�N�X�擾
		D3DXMATRIX mtxParts = GetParentMatrix(10);

		pOrbit->pMtxparent = &mtxParts;

		//���[�V�����̍X�V
		UpdateMotionPlayer();

		//�L�����N�^�[�Ƃ̓����蔻��
		CollisionCharPlayer();

		//�ǂƂ̓����蔻��
		//CollisionWall(&g_aPlayer.pos, g_aPlayer.posOld);

		//���W����
		LimitPosPlayer();

		//�e�̈ʒu�X�V
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		//�A�C�R���̈ʒu�X�V
		UpdatePositionIcon(g_aPlayer.nIdxIcon, g_aPlayer.pos);

		//�Z���t�̈ʒu�X�V
		SetPositionSerif(g_aPlayer.nIdxSerif, { g_aPlayer.pos.x, g_aPlayer.pos.y + 150.0f, g_aPlayer.pos.z });

		//�Ռ��g�̈ʒu�X�V
		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
		{//�ړ��Ö��A�N�V������

			for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
			{
				SetPositionImpactWave(g_aPlayer.nIdxWave[nCntWave], g_aPlayer.move);
			}
		}

		//���̈ʒu�X�V
		SetPositionFlag(g_aPlayer.nIdxFlag, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + FLAG_POS, g_aPlayer.pos.z));

	}

	//��ԍX�V
	UpdateStatePlayer();
}

//==================================================================================
//�Q�[���I�����̍X�V����
//==================================================================================
void ControllEndPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	g_aPlayer.bMove = true;

	g_aPlayer.rot.y = 0.0f;

	g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
	g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
	g_aPlayer.fRotDest = pCamera->rot.y;

	//�d�͏���
	//g_aPlayer.move.y -= 1.0f;

	//�ʒu�X�V
	g_aPlayer.pos += g_aPlayer.move;

	//����
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;
}

//==================================================================================
//���f���̑���
//==================================================================================
void ControllPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�G�f�B�b�g�̏��擾
	Edit  *pEdit = GetEdit();

	//�^�[�Q�b�g�̏��擾
	DISPATCH *pDispatch = GetDispatch();

	if (GetGameState() == GAMESTATE_NONE && pDispatch->bUse == false && pEdit->bUse == false)
	{//�^�[�Q�b�g���ȊO

		if (g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR)
		{//�ړ��s���[�V�������͋����L�����Z��

			if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
			{//���L�[�������ꂽ,���ړ�

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//A+W,����ړ�

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LRUP + pCamera->rot.y;
					pCamera->bSPush = false;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//A+S,�����ړ�

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LRDW + pCamera->rot.y;

					pCamera->bSPush = true;
				}
				else
				{//A,���ړ�

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LR + pCamera->rot.y;
					pCamera->bSPush = false;
				}

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
				{//�Ö����Ă��Ȃ�������

					g_aPlayer.bMove = true;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
			{//D�L�[�������ꂽ,�E�ړ�

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//D+W,�E��ړ�

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRUP + pCamera->rot.y;
					pCamera->bSPush = false;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//D+S,�E���ړ�

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRDW + pCamera->rot.y;

					pCamera->bSPush = true;
				}
				else
				{//D,�E�ړ�

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LR + pCamera->rot.y;
					pCamera->bSPush = false;
				}

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
				{//�Ö����Ă��Ȃ�������

					g_aPlayer.bMove = true;
				}
			}
			else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//W�������ꂽ�A���ړ�

				g_aPlayer.move.x += sinf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.move.z += cosf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.fRotDest = D3DX_PI + pCamera->rot.y;
				pCamera->bSPush = false;

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
				{//�Ö����Ă��Ȃ�������

					g_aPlayer.bMove = true;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//S�������ꂽ�A��O�ړ�

				g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.fRotDest = pCamera->rot.y;

				pCamera->bSPush = true;

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
				{//�Ö����Ă��Ȃ�������

					g_aPlayer.bMove = true;
				}
			}
			else
			{//�Ȃ�̃L�[��������Ă��Ȃ�

				if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f ||
					g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
				{
					g_aPlayer.bMove = false;
					pCamera->bSPush = false;

					if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
					{//�ړ��Ö����̎�

						//���[�V�����̐ݒ�
						SetMotisonPlayer(PLAYERMOTION_ACTION);
					}

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION && g_aPlayer.bFever == true && g_aPlayer.bATK == true)
					{//�ړ��Ö����̎�

						//���[�V�����̐ݒ�
						SetMotisonPlayer(PLAYERMOTION_FEVERACTION);
					}
				}
			}
		}
		

		if (g_aPlayer.bATK == false && g_aPlayer.bJump == false && GetKeyboardTrigger(DIK_RETURN) == true ||
			g_aPlayer.bATK == false && g_aPlayer.bJump == false && GetGamepadTrigger(BUTTON_B, 0))
		{//ENTER�������ꂽ,,�e����

			//�������̐ݒ菈��
			SetCutepillow(g_aPlayer.pos, g_aPlayer.rot, PILLOWPARENT_PLAYER);

			if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
			{//�t�B�[�o�[���Ȃ�

				//�S���ʕ�����
				SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + D3DX_PI * 0.5f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
				SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + D3DX_PI * 1.0f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
				SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + -D3DX_PI * 0.5f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
			}

			//SE�Z�b�g
			PlaySound(SOUND_LABEL_SE_DAKIMAKURA);
		}

		if (GetKeyboardPress(DIK_L) == true || 
			GetGamepadPress(BUTTON_A, 0))
		{//L�������ꂽ && A�{�^���������ꂽ

			//g_aPlayer.bATK = true;

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
			{//�Ö����Ă��Ȃ�������

				//�Ö�SE
				PlaySound(SOUND_LABEL_BGM_HYPNOSIS);
			}

			if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f &&
				g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
			{//�~�܂��Ă���Ƃ��͋���

			 g_aPlayer.bMove = false;
			 g_aPlayer.move.x = 0.0f;
			 g_aPlayer.move.z = 0.0f;

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.bFever == false)
				{//�t�B�[�o�[������Ȃ�������

					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_ACTION);
				}

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION && g_aPlayer.bFever == true)
				{//�t�B�[�o�[����������

					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_FEVERACTION);
				}
			}
			else
			{//�����Ă�Ƃ��͎�

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.bFever == false)
				{//f�t�B�[�o�[������Ȃ�������
					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_MOVEACTION);
				}

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION && g_aPlayer.bFever == true/* && g_aPlayer.bATK == false*/)
				{//�t�B�[�o�[����������

					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_FEVERMOVEACTION);
				}
			}
		}

		if (GetKeyboardRelease(DIK_L) == true || 
			GetGamepadRelease(BUTTON_A, 0))
		{//L�������ꂽ && A�{�^���������ꂽ

			//�U����ԉ���
			g_aPlayer.bATK = false;

			//���[�V�����̐ݒ�
			SetMotisonPlayer(PLAYERMOTION_DEF);
		}
	}

	if (GetGamepadTrigger(BUTTON_RB, 0) || GetKeyboardTrigger(DIK_RSHIFT) == true)
		{//�h���̎g�p�󋵓���ւ�

			SetRightDisPatch();
		}
		if (GetGamepadTrigger(BUTTON_LB, 0) || GetKeyboardTrigger(DIK_LSHIFT) == true)
		{//�h���̎g�p�󋵓���ւ�

			SetLeftDisPatch();
		}

	//���݂ƖڕW�̍��������߂�
	g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

	//�p�x�̐��K��
	g_aPlayer.fRotDiff = RotNormalize(g_aPlayer.fRotDiff);

	//�p�x�̕␳������
	g_aPlayer.rot.y += g_aPlayer.fRotDiff * 0.1f;

	//�p�x�̐��K��
	g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

	//�d�͏���
	//g_aPlayer.move.y -= 1.0f;

	if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
	{//�t�B�[�o�[���Ȃ�

		//����
		g_aPlayer.move.x *= 1.1f;
		g_aPlayer.move.z *= 1.1f;
	}

	//�ʒu�X�V
	g_aPlayer.pos += g_aPlayer.move;

	//��������
	if (g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION)
	{
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;
	}
	else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
	{
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR_HYPNOSIS;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR_HYPNOSIS;

		if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
		{//�t�B�[�o�[���Ȃ�

			//����
			g_aPlayer.move.x *= 1.1f;
			g_aPlayer.move.z *= 1.1f;
		}
	}

	PrintDebugProc(
		"\n------�v���C���[�̑���------\n"
		"<�ړ�> W/A/S/D\n"
		"<�Ö�> L\n"
		"<�h��> LT/RT\n"
		"<������> ENTER\n\n"
		"<�ړ���> [%f, %f, %f]\n", g_aPlayer.move.x, g_aPlayer.move.y, g_aPlayer.move.z);

}

//==================================================================================
//�v���C���[�̍��W����
//==================================================================================
void LimitPosPlayer(void)
{
	if (g_aPlayer.pos.y < 0.0f)
	{//�n�ʂ�艺�ɍs������

		//g_aPlayer.pos.y = 0.0f;

		//�d�͏���
		g_aPlayer.move.y = 0.0f;

		g_aPlayer.bJump = false;
	}


	//�ړ�����
	if (GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_END)
	{
		if (g_aPlayer.pos.x - g_aPlayer.fRadius <= -LIMIT_POS && g_aPlayer.posOld.x - g_aPlayer.fRadius >= -LIMIT_POS)
		{//����-1000�ȉ�, �O��-1000�ȏ�

			g_aPlayer.pos.x = -LIMIT_POS + g_aPlayer.fRadius;
		}
		if (g_aPlayer.pos.x + g_aPlayer.fRadius >= LIMIT_POS && g_aPlayer.posOld.x + g_aPlayer.fRadius <= LIMIT_POS)
		{//����1000�ȏ�, �O��1000�ȉ�

			g_aPlayer.pos.x = LIMIT_POS - g_aPlayer.fRadius;
		}
		if (g_aPlayer.pos.z + g_aPlayer.fRadius >= LIMIT_POS && g_aPlayer.posOld.z + g_aPlayer.fRadius <= LIMIT_POS)
		{//����1000�ȏ�, �O��1000�ȉ�

			g_aPlayer.pos.z = LIMIT_POS - g_aPlayer.fRadius;
		}
		if (g_aPlayer.pos.z - g_aPlayer.fRadius <= -LIMIT_POS && g_aPlayer.posOld.z - g_aPlayer.fRadius >= -LIMIT_POS)
		{//����-1000�ȉ�, �O��-1000�ȏ�

			g_aPlayer.pos.z = -LIMIT_POS + g_aPlayer.fRadius;
		}
	}
}

//==================================================================================
//�v���C���[�̑���v�Z����
//==================================================================================
void CalFormationPosPlayer(void)
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

				g_aPlayer.aFormation[nCntCrowd + nCntWidth].pos =
					D3DXVECTOR3
					(
						g_aPlayer.pos.x + sinf(-D3DX_PI * MOVE_LR + fRot + g_aPlayer.rot.y) * (FORMATION_DISTANCE * (nCntformation + 1)),
						0.0f,
						g_aPlayer.pos.z + cosf(-D3DX_PI * MOVE_LR + fRot + g_aPlayer.rot.y) * (FORMATION_DISTANCE * (nCntformation + 1)));
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
//�v���C���[�̑���v�Z����
//==================================================================================
int SetFormationPosPlayer(void)
{
	int nCntIdx = 0;

	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aPlayer.aFormation[nCntCrowd].bUse == false)
		{//�g�p���Ă��Ȃ�������

			//�C���f�b�N�X�ԍ�����
			nCntIdx = nCntCrowd;

			g_aPlayer.aFormation[nCntCrowd].bUse = true;
			break;
		}
	}

	return nCntIdx;
}

//==================================================================================
//�Ö����̃z�����z�����ݒ菈��
//==================================================================================
int SetHypnosisWavePlayer(float fRot, float fWidthMove)
{
	//�Ö��̃z�����z����
	int nIdxWave = SetImpactWave
	(
		D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 50.0f, g_aPlayer.pos.z),
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
//�K���̍U���X�V����
//==================================================================================
void UpdateATKGun(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	static int nCnt = 0;

	if (g_aPlayer.bATK == true)
	{//�K��������

		nCnt++;

		if (nCnt % 5 == 0 && nCnt <= 15)
		{
		}

		if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
		{//���L�[�������ꂽ,���ړ�

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//A+W,����ړ�

				g_aPlayer.fRotDest = D3DX_PI * MOVE_LRUP + pCamera->rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//A+S,�����ړ�

				g_aPlayer.fRotDest = D3DX_PI * MOVE_LRDW + pCamera->rot.y;
			}
			else
			{//A,���ړ�

				g_aPlayer.fRotDest = D3DX_PI * MOVE_LR + pCamera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
		{//D�L�[�������ꂽ,�E�ړ�

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//D+W,�E��ړ�

				g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRUP + pCamera->rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//D+S,�E���ړ�

				g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRDW + pCamera->rot.y;
			}
			else
			{//D,�E�ړ�

				g_aPlayer.fRotDest = -D3DX_PI * MOVE_LR + pCamera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
		{//W�������ꂽ�A���ړ�

			g_aPlayer.fRotDest = D3DX_PI + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
		{//S�������ꂽ�A��O�ړ�

			g_aPlayer.fRotDest = pCamera->rot.y;
		}
	}
	else
	{
		nCnt = 0;
	}
}

//==================================================================================
//�U������
//==================================================================================
void UpdateATKPenLight(void)
{
	switch (g_aPlayer.nNowMotionNum)
	{
	case PLAYERMOTION_ACTION:

		//�U���̓����蔻��
		CollisionATKPlayer(PENLIGHT_RADIUS, PENLIGHT_ANGLE, STOP_HYPNOSIS);
		break;


	case PLAYERMOTION_MOVEACTION:

		//�U���̓����蔻��
		CollisionATKPlayer(PENLIGHT_RADIUS_MOVE, PENLIGHT_ANGLE_MOVE, MOVE_HYPNOSIS);
		break;

	case PLAYERMOTION_FEVERACTION:

		//�U���̓����蔻��
		CollisionATKPlayer(PENLIGHT_RADIUS, PENLIGHT_ANGLE, STOP_FEVER_HYPNOSIS);
		break;

	case PLAYERMOTION_FEVERMOVEACTION:

		//�U���̓����蔻��
		CollisionATKPlayer(PENLIGHT_RADIUS_MOVE, PENLIGHT_ANGLE_MOVE, MOVE_FEVER_HYPNOSIS);
		break;
	}
}

//==================================================================================
//�v���C���[�̍U��
//==================================================================================
void CollisionATKPlayer(float fDistance, float fAngle, int nValue)
{
	//���u�̏��擾
	Mob *pMob = GetMob();

	//�y�����C�g�̍U��
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState != MOBSTATE_APPIEAR)
		{//���u���g�p����Ă�����

			bool bPenLight;

			D3DXVECTOR3 pos = D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y) * -fDistance);

			//�y�����C�g�̓����蔻��
			bPenLight = CollisionFan(g_aPlayer.pos,
				D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y) * -fDistance),
				D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y) * -fDistance),
				pMob->aModel.pos, pMob->aModel.posOld);

			if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
			{//�t�B�[�o�[���Ȃ�

				if (bPenLight == false)
				{//�܂��������Ă��Ȃ�������

					bPenLight = CollisionFan(g_aPlayer.pos,
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y + D3DX_PI * 0.5f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y + D3DX_PI * 0.5f) * -fDistance),
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y + D3DX_PI * 0.5f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y + D3DX_PI * 0.5f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}

				if (bPenLight == false)
				{//�܂��������Ă��Ȃ�������

					bPenLight = CollisionFan(g_aPlayer.pos,
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y + D3DX_PI * 1.0f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y + D3DX_PI * 1.0f) * -fDistance),
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y + D3DX_PI * 1.0f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y + D3DX_PI * 1.0f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}

				if (bPenLight == false)
				{//�܂��������Ă��Ȃ�������

					bPenLight = CollisionFan(g_aPlayer.pos,
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y + -D3DX_PI * 0.5f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y + -D3DX_PI * 0.5f) * -fDistance),
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y + -D3DX_PI * 0.5f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y + -D3DX_PI * 0.5f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}
			}

			if (bPenLight == true)
			{//��͈̔͂ɓ�������

				//�Ö��x����������܂ł̃J�E���^�[��������
				pMob->nSubHypCounter = HYP_CNTTIME;

				//���u�̃q�b�g����
				HitMob(nCntMob, nValue);
			}
		}
	}
}

//==================================================================================
//�ǂƂ̓����蔻��
//==================================================================================
void CollisionWallPlayer(void)
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
				g_aPlayer.pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * -fLen);

			//�E�̒��_���W
			D3DXVECTOR3 pos1 = D3DXVECTOR3(
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen,
				g_aPlayer.pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen);

			//���E���̃x�N�g��
			D3DXVECTOR3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			//�v���C���[�̋��E���̃x�N�g��
			D3DXVECTOR3 vecLinePlayer;
			vecLinePlayer.x = g_aPlayer.pos.x - g_aPlayer.posOld.x;
			vecLinePlayer.z = g_aPlayer.pos.z - g_aPlayer.posOld.z;

			//�v���C���[�ƕǂ̃x�N�g��
			D3DXVECTOR3 vecToPosPlayer;
			vecToPosPlayer.x = pos1.x - g_aPlayer.posOld.x;
			vecToPosPlayer.z = pos1.z - g_aPlayer.posOld.z;

			//�ʐς̍ő�l
			float fMaxAreaPlayer = (vecLinePlayer.z * vecLine.x) - (vecLinePlayer.x * vecLine.z);

			//����̖ʐ�
			float fNowAreaPlayer = (vecToPosPlayer.z * vecLine.x) - (vecToPosPlayer.x * vecLine.z);

			//����
			float fRatePlayer = fNowAreaPlayer / fMaxAreaPlayer;

			if (fRatePlayer >= 0.0f && fRatePlayer <= 1.0f)
			{//�ʐς͈͓̔��ɂ����画��

				if ((vecLinePlayer.z * vecToPosPlayer.x) - (vecLinePlayer.x * vecToPosPlayer.z) > 0)
				{//�ǂɓ���������

					//��_����͂ݏo����
					D3DXVECTOR3 CollisionPointPlayer = D3DXVECTOR3(0.0f, g_aPlayer.pos.y, 0.0f);
					CollisionPointPlayer.x = (g_aPlayer.pos.x + (vecLinePlayer.x * (fRatePlayer - 1.0f)));
					CollisionPointPlayer.z = (g_aPlayer.pos.z + (vecLinePlayer.z * (fRatePlayer - 1.0f)));

					//�@���x�N�g��(���E���x�N�g����X��Z���])
					D3DXVECTOR3 vecNor = D3DXVECTOR3(vecLine.z, 0.0f, -vecLine.x);

					//�x�N�g���̐��K��
					D3DXVec3Normalize(&vecNor, &vecNor);

					//�v���C���[�̋t�ړ���
					D3DXVECTOR3 PlayerInverceMove;
					PlayerInverceMove.z = vecLinePlayer.z * (fRatePlayer - 1.0f);
					PlayerInverceMove.x = vecLinePlayer.x * (fRatePlayer - 1.0f);

					//����(�ǂ̖@���ƃv���C���[�̋t�ړ���)
					float fDot = (PlayerInverceMove.x * vecNor.x) + (PlayerInverceMove.z * vecNor.z);

					//�ǂ���ړ���
					D3DXVECTOR3 MoveWall = vecNor * fDot;

					D3DXVECTOR3 ppp = (CollisionPointPlayer - g_aPlayer.pos);
					D3DXVECTOR3 pppp = ppp + MoveWall;

					//�Ԃ������_�ɕ␳
					g_aPlayer.pos += MoveWall + (vecNor * 0.1f);

				}

			}

		}

	}
}

//==================================================================================
//�e�̃q�b�g����
//==================================================================================
void HitPlayer(D3DXVECTOR3 move, int nDMG)
{

	////�G�f�B�b�g�̏��擾
	//Edit *pEdit = GetEdit();

	//if (g_aPlayer.bUlt == false && g_aPlayer.bUse == true && g_aPlayer.nState == PLAYERSTATE_NONE && pEdit->bUse == false)
	//{//�E���g��Ԃ���Ȃ��Ƃ�

	//	g_aPlayer.move = move;	//�ړ��ʉ��Z
	//	g_aPlayer.nLife -= nDMG;	//�̗͌��炷

	//	if (g_aPlayer.nLife <= 0)
	//	{//�̗͂��Ȃ��Ȃ���

	//		g_aPlayer.nLife = 0;
	//		//g_aPlayer.bUse = false;
	//		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//		//�T�E���h��~
	//		SetGameState(GAMESTATE_END, 120);
	//	}
	//	else
	//	{//�܂��̗͂��c���Ă�

	//		g_aPlayer.nCntState = DMG_TIME;	//�_���[�W��Ԃ�ۂ��Ԃ�^����

	//		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	//		{//�p�[�c���J��Ԃ�

	//			D3DXMATERIAL *pMat;	//�}�e���A���f�[�^�ւ̃|�C���^

	//								//�}�e���A���f�[�^�ւ̃|�C���^���擾
	//			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

	//			//���_�����J��Ԃ�
	//			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
	//			{//�}�e���A����Ԃ�����

	//				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//			}
	//		}
	//	}
	//}
}


//==================================================================================
//�v���C���[�̏�ԍX�V
//==================================================================================
void UpdateStatePlayer(void)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();



	D3DXMATERIAL *pMat;	//�}�e���A���f�[�^�ւ̃|�C���^

	switch (g_aPlayer.nState)
	{
	case PLAYERSTATE_NONE:

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//�p�[�c���J��Ԃ�

		 //�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
			{
				//���̐F�ɖ߂�
				pMat[nCntMat].MatD3D.Diffuse = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Ambient = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Ambient;
			}
		}
		break;
	}
}

//==================================================================================
//�L�����N�^�[�Ƃ̓����蔻��
//==================================================================================
void CollisionCharPlayer(void)
{
	//�G�̏��擾
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bMoreSpawn == true)
		{//�G���g�p����Ă�����

		 //�L�����N�^�[���m�̓����蔻��
			CollisionCharacter(
				&g_aPlayer.pos, &g_aPlayer.posOld, D3DXVECTOR3(g_aPlayer.fRadius, 0.0f, g_aPlayer.fRadius), D3DXVECTOR3(-g_aPlayer.fRadius, 0.0f, -g_aPlayer.fRadius),
				&pEnemy->pos, D3DXVECTOR3(pEnemy->fRadius, 0.0f, pEnemy->fRadius), D3DXVECTOR3(-pEnemy->fRadius, 0.0f, -pEnemy->fRadius));
		}
	}


	//���f���̏��擾
	ModelModel *pModel = GetModel();

	//��ʔ����̏��擾	
	ManySpawn *pManySpawn = GetManySpawn();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true && (pModel->nType < SMALL_NUM || pModel->nType >= CONE_NUM))
		{//���f�����g�p����Ă�����

			//�L�����N�^�[���m�̓����蔻��
			CollisionCharacter(
				&g_aPlayer.pos, &g_aPlayer.posOld, D3DXVECTOR3(g_aPlayer.fRadius, 0.0f, g_aPlayer.fRadius), D3DXVECTOR3(-g_aPlayer.fRadius, 0.0f, -g_aPlayer.fRadius),
				&pModel->posOrigin, pModel->vtxMax, pModel->vtxMin);

			float fLength = g_aPlayer.fRadius + FLEN;

			if (CircleRange(pManySpawn->aSpawn[pModel->nType].pos, g_aPlayer.pos) <= (fLength * fLength) &&
				pManySpawn->aSpawn[pModel->nType].nMany == 1 &&
				pModel->nType < 40 && 
				pManySpawn->aSpawn[pModel->nType].bUse == false)
			{
				pManySpawn->aSpawn[pModel->nType].bUse = true;
			}
		}
	}
}

//==================================================================================
//�v���C���[�̃��[�V��������
//==================================================================================
void SetMotisonPlayer(int nMotionType)
{

	//if (g_aPlayer.nNowMotionNum != nMotionType)
	//{

	//	for (int nCntKey = 0; nCntKey < g_aMotionData[nMotionType].nNumKey; nCntKey++)
	//	{//�L�[�����J��Ԃ�

	//		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	//		{//�p�[�c���J��Ԃ�

	//			g_aPlayer.aModel[nCntParts].rot = g_aMotionData[nMotionType].aKey[nCntKey].aParts[nCntParts].rot;
	//			g_aPlayer.aModel[nCntParts].pos = g_aPlayer.aModel[nCntParts].posOrigin;
	//		}

	//		g_aPlayer.nCntFrame = 0;	//�t���[���J�E���g���Z�b�g
	//	}

	//	//���̃��[�V�����̍X�V
	//	g_aPlayer.nNowMotionNum = nMotionType;

	//	g_aPlayer.nPatternKey = 0;	//���݂̃L�[�ԍ����Z�b�g
	//	g_aPlayer.nCntAllFrame = 0;	//���t���[�������Z�b�g
	//}


	//for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	//{//�p�[�c���J��Ԃ�

	//	//���̌����ƌ��݂̌����ŖڕW�̌��������߂�
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDest = g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot;
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDest = g_aPlayer.aModel[nCntParts].posOrigin + g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos;

	//	//���݂ƖڕW�̍��������߂�
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff = g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDest - g_aPlayer.aModel[nCntParts].rot;
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDiff = g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDest - g_aPlayer.aModel[nCntParts].pos;

	//	//�p�x�̐��K��
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.x = RotNormalize(g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.x);
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.y = RotNormalize(g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.y);
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.z = RotNormalize(g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.z);

	//}

	//���̃��[�V�����̍X�V
	g_aPlayer.nNowMotionNum = nMotionType;
	g_aPlayer.nPatternKey = 0;	//���݂̃L�[�ԍ����Z�b�g
	g_aPlayer.nCntFrame = 0;	//�t���[���J�E���g���Z�b�g
	g_aPlayer.nCntAllFrame = 0;	//���t���[�������Z�b�g

}

//==================================================================================
//�v���C���[�̃��[�V�����X�V����
//==================================================================================
void UpdateMotionPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	//{//�p�[�c���J��Ԃ�

	//	//���������߂ċK��t���[���ŏI���悤�ɕ␳
	//	g_aPlayer.aModel[nCntParts].rot +=
	//		g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff /	//�ړI�ƌ��݂̍���
	//		(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame;					//�Đ��t���[���Ŋ���

	//	//�ʒu
	//	g_aPlayer.aModel[nCntParts].pos +=
	//		g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDiff /	//�ړI�ƌ��݂̍���
	//		(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame;					//�Đ��t���[���Ŋ���

	//	if (g_aPlayer.aModel[nCntParts].nParent == -1)
	//	{//�e�̏ꍇ

	//		//�}�g���b�N�X�̎擾
	//		D3DXMATRIX mtxwood = GetParentMatrix(nCntParts);

	//		g_aPlayer.pos.x += 
	//			sinf(g_aPlayer.rot.y) * (g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDiff.z /	//�ړI�ƌ��݂̍���
	//			(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);			//�Đ��t���[���Ŋ���

	//		g_aPlayer.pos.z += 
	//			cosf(g_aPlayer.rot.y) * (g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDiff.z /	//�ړI�ƌ��݂̍���
	//			(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);			//�Đ��t���[���Ŋ���
	//	}

	//	//�p�x�̐��K��
	//	g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x);
	//	g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y);
	//	g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z);

	//}

	////�t���[���̃J�E���g�����Z
	//g_aPlayer.nCntFrame++;
	//g_aPlayer.nCntAllFrame++;

	//if (g_aPlayer.nCntFrame >= g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame)
	//{//�t���[���̃J�E���g���t���[�����ɒB������

	//	//�t���[���̃J�E���g���[���ɖ߂�
	//	g_aPlayer.nCntFrame = 0;

	//	//�p�^�[��NO.�X�V
	//	g_aPlayer.nPatternKey = (g_aPlayer.nPatternKey + 1) % g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

	//	if (g_aPlayer.nPatternKey == 0)
	//	{//�L�[���[���̎�

	//		//�U����ԉ���
	//		g_aPlayer.bATK = false;
	//		g_aPlayer.nCntAllFrame = 0;	//���t���[�������Z�b�g

	//		if (g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
	//		{

	//			//���[�V�����̐ݒ�
	//			SetMotisonPlayer(PLAYERMOTION_DEF);
	//		}
	//		else
	//		{
	//			//���[�V�����̐ݒ�
	//			SetMotisonPlayer(g_aPlayer.nNowMotionNum);
	//		}

	//	}
	//	else
	//	{
	//		//���[�V�����̐ݒ�
	//		SetMotisonPlayer(g_aPlayer.nNowMotionNum);
	//	}

	//}

	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

	 //���̃L�[
		int nNextKey = (g_aPlayer.nPatternKey + 1) % g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey;
		int nnn;

		if (g_aPlayer.nPatternKey == g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
		{//���[�v���Ȃ��Ƃ�

		 //nNextKey = g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		if (g_aPlayer.nPatternKey == g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_ON)
		{//���[�v�̎�

			nnn = g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		//�O��Ǝ��̃L�[�̍����擾
		float rotDiffX = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z;

		//�p�[�c�̌�����ݒ�
		g_aPlayer.aModel[nCntParts].rot.x =
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		g_aPlayer.aModel[nCntParts].rot.y =
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		g_aPlayer.aModel[nCntParts].rot.z =
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		//�p�x�̐��K��
		g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x);
		g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y);
		g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z);


		//�p�[�c�̈ʒu��ݒ�
		if (nCntParts == 0 && GetMode() != MODE_RANKING && (GetMode() == MODE_GAME && GetGameState() != GAMESTATE_BEFOREEND))
		{
			//�ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z;

			//�ʒu�␳
			g_aPlayer.pos.x +=
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x +
				posDiffX *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);

			g_aPlayer.pos.y =
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y +
				posDiffY *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);

			g_aPlayer.pos.z +=
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z +
				posDiffZ *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);
		}
	}

	//�t���[���̃J�E���g�����Z
	g_aPlayer.nCntFrame++;
	g_aPlayer.nCntAllFrame++;

	if (g_aPlayer.nCntFrame >= g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame)
	{//�t���[���̃J�E���g���t���[�����ɒB������

	 //�t���[���̃J�E���g���[���ɖ߂�
		g_aPlayer.nCntFrame = 0;

		//�p�^�[��NO.�X�V
		g_aPlayer.nPatternKey = (g_aPlayer.nPatternKey + 1) % g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

		if (g_aPlayer.nPatternKey == 0)
		{//�L�[���[���̎�

			//�U����ԉ���
			g_aPlayer.bATK = false;
			g_aPlayer.nCntAllFrame = 0;	//���t���[�������Z�b�g

			if (g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
			else
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(g_aPlayer.nNowMotionNum);
			}
		}
	}
}

//==================================================================================
//�v���C���[�̕`�揈��
//==================================================================================
void DrawPlayer(void)
{


	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxnParent;			//�e�̌v�Z�p�}�g���b�N�X

	if (g_aPlayer.bDisp == true && g_aPlayer.bUse == true)
	{//�g�p���Ă�����

	 //���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//�����𔽉f����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//�ʒu�𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int nCount = 0; nCount < g_aPlayer.nPartsNum; nCount++)
		{

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCount].rot.y, g_aPlayer.aModel[nCount].rot.x, g_aPlayer.aModel[nCount].rot.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCount].pos.x, g_aPlayer.aModel[nCount].pos.y, g_aPlayer.aModel[nCount].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxTrans);


			if (g_aPlayer.aModel[nCount].nParent == -1)
			{//NULL�̏ꍇ,�e

			 //�v���C���[�̌����𔽉f����
				mtxnParent = g_aPlayer.mtxWorld;
			}
			else
			{//�p�[�c

			 //�e�̌����𔽉f����
				mtxnParent = g_aPlayer.aModel[g_aPlayer.aModel[nCount].nParent].mtxWorld;
			}

			//�����̂�ɐe�̃��[���h�}�g���b�N�X���|���Z
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxnParent);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCount].mtxWorld);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCount].pBuffMat->GetBufferPointer();

			if (g_aPlayer.aModel[nCount].bUse == true)
			{//�p�[�c���g�p����Ă�����

			 //���_�����J��Ԃ�
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCount].dwNumMat; nCntMat++)
				{

					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aPlayer.aModel[nCount].pTexture[nCntMat]);

					//�v���C���[(�p�[�c)�̕`��
					g_aPlayer.aModel[nCount].pMesh->DrawSubset(nCntMat);

				}
			}
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}

}

//==================================================================================
//�v���C���[�̏��擾
//==================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}

//==================================================================================
//�v���C���[�̐ݒ菈��
//==================================================================================
void SetPlayer(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

	 //X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aPlayer.aModel[nCntParts].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aPlayer.aModel[nCntParts].pBuffMat,
			NULL,
			&g_aPlayer.aModel[nCntParts].dwNumMat,
			&g_aPlayer.aModel[nCntParts].pMesh);

		D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

		//���_�����J��Ԃ�
		for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
		{
			//�}�e���A���̃f�[�^�擾
			g_aPlayer.aModel[nCntParts].pMatData[nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�t�@�C�������݂���

			 //�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aPlayer.aModel[nCntParts].pTexture[nCntMat]);
			}
		}

		if (g_aPlayer.aModel[nCntParts].nFever != 0)
		{
			//�p�[�c���g�p���
			g_aPlayer.aModel[nCntParts].bUse = true;
		}
		else
		{
			g_aPlayer.aModel[nCntParts].bUse = false;
		}	
		

		//���f���̑S���_�`�F�b�N
		CheckVtx(g_aPlayer.aModel[nCntParts].rot.y, &g_aPlayer.aModel[nCntParts].vtxMax, &g_aPlayer.aModel[nCntParts].vtxMin, g_aPlayer.aModel[nCntParts].pMesh, g_aPlayer.aModel[nCntParts].pVtxBuff);
	}

	//�e�̐ݒ�
	if (GetMode() != MODE_TITLE)
	{
		g_aPlayer.nIdxShadow = SetShadow(g_aPlayer.fRadius * 0.5f, g_aPlayer.fRadius * 0.5f);
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
	}

	//�A�C�R���̐ݒ�
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		g_aPlayer.nIdxIcon = SetIcon(MINIMAP_ICON_PLAYER, 100.0f);
		UpdatePositionIcon(g_aPlayer.nIdxIcon, g_aPlayer.pos);
	}

	//���̐ݒ�
	g_aPlayer.nIdxFlag = SetFlag();
	SetPositionFlag(g_aPlayer.nIdxFlag, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + FLAG_POS, g_aPlayer.pos.z));
}

//==================================================================================
//��΃}�g���b�N�X�̎擾
//==================================================================================
D3DXMATRIX GetParentMatrix(int nIdxParts)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxnParent;			//�e�̌v�Z�p�}�g���b�N�X

									//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

	//�����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

	//�ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

	for (int nCount = 0; nCount < g_aPlayer.nPartsNum; nCount++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);

		//�����𔽉f����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCount].rot.y, g_aPlayer.aModel[nCount].rot.x, g_aPlayer.aModel[nCount].rot.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxRot);

		//�ʒu�𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCount].pos.x, g_aPlayer.aModel[nCount].pos.y, g_aPlayer.aModel[nCount].pos.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxTrans);


		if (g_aPlayer.aModel[nCount].nParent == -1)
		{//NULL�̏ꍇ,�e

		 //�v���C���[�̌����𔽉f����
			mtxnParent = g_aPlayer.mtxWorld;
		}
		else
		{//�p�[�c

		 //�e�̌����𔽉f����
			mtxnParent = g_aPlayer.aModel[g_aPlayer.aModel[nCount].nParent].mtxWorld;
		}

		//�����̂�ɐe�̃��[���h�}�g���b�N�X���|���Z
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxnParent);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCount].mtxWorld);
	}

	return g_aPlayer.aModel[nIdxParts].mtxWorld;
}

//==================================================================================
//�v���C���[�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetPlayer(void)
{
	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

						//�t�@�C�����J��
	pFile = fopen(PLAYER_TXT, "r");

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
				fscanf(pFile, "%d", &g_aPlayer.nPartsNum);	//���f����
			}

			while (nCntFileName != g_aPlayer.nPartsNum)
			{//���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			 //������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);

				//���f�����̐ݒ�
				if (strcmp(aComment, "MODEL_FILENAME") == 0)
				{//NUM_MODEL��������

					fscanf(pFile, "%s", &aComment[0]);	//=�̕�
					fscanf(pFile, "%s", &g_aPlayer.aModel[nCntFileName].acFilename[0]);	//�t�@�C����
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
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nType);	//���f����ނ̗�

							}

							if (strcmp(aComment, "PARENT") == 0)
							{//PARENT��������e�̔ԍ��ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nParent);	//�e�̔ԍ�

							}

							if (strcmp(aComment, "POS") == 0)
							{//POS��������ʒu�ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.x);	//X���W
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.y);	//Y���W
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.z);	//Z���W

																							//���̈ʒu���蓖��
								g_aPlayer.aModel[nCntSetParts].posOrigin = g_aPlayer.aModel[nCntSetParts].pos;

							}

							if (strcmp(aComment, "ROT") == 0)
							{//ROT������������ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.x);	//X�̊p�x
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.y);	//Y�̊p�x
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.z);	//Z�̊p�x

							}

							if (strcmp(aComment, "WEAPON") == 0)
							{//WEAPON�������畐�킩�ǂ����ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�

							}

							if (strcmp(aComment, "FEVER") == 0)
							{
								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nFever);
								
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

	//�v���C���[�̔z�u
	SetPlayer();

}

//==================================================================================
//�v���C���[�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetMotionPlayer(void)
{

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	char aComment[MAX_COMMENT];	//�R�����g�p
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0;

	//�t�@�C�����J��
	pFile = fopen("data\\TEXT\\motion_set_player.txt", "r");

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
						fscanf(pFile, "%d", &g_aMotionData[nCntMotion].nLoop);	//���[�v0��1

					}

					if (strcmp(aComment, "NUM_KEY") == 0)
					{//NUM_KEY��������L�[���ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aMotionData[nCntMotion].nNumKey);	//�L�[��

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
								fscanf(pFile, "%d", &g_aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//�Đ��t���[��

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
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X���W
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y���W
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z���W
									}

									if (strcmp(aComment, "ROT") == 0)
									{//ROT��������p�x�ǂݍ���

										fscanf(pFile, "%s", &aComment[0]);		//=�̕�
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X�p�x
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y�p�x
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z�p�x
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
	}
}

//==================================================================================
//�`���[�g���A���̍X�V����
//==================================================================================
void UpdateTutorialPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	if (g_aPlayer.bUse == true)
	{//�g�p���Ă�����

		//�ߋ��̈ʒu�ۑ�
		g_aPlayer.posOld = g_aPlayer.pos;

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//�p�[�c���J��Ԃ�

			//�p�[�c���Ƃ̃}�g���b�N�X�擾
			D3DXMATRIX mtxParts = GetParentMatrix(nCntParts);

			g_aPlayer.aModel[nCntParts].OldmtxWorld = mtxParts;
		}


		//�v���C���[�̑���
		ControllTutorialPlayer();

		//����̈ʒu�X�V
		CalFormationPosPlayer();

		//�U������
		UpdateATKGun();
		UpdateATKPenLight();

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//�ړ����Ă�Ƃ�

			g_aPlayer.nCntWalk++;	//�J�E���g���Z

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//���s�p�[�e�B�N���̐ݒ�
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				PlaySound(SOUND_LABEL_SE_ASIOTO);
			}


			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.bFever == false)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERWALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION && g_aPlayer.bFever == true)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_FEVERWALK);
			}

		}
		else
		{//�j���[�g�������[�V����

			if (g_aPlayer.bATK == false && g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
			{
				//�U����ԉ���
				g_aPlayer.bATK = false;

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION ||
			g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION ||
			g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION ||
			g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
		{//�Ö��A�N�V������

			g_aPlayer.nCntPenlight++;

			if (g_aPlayer.nCntPenlight == 20 || g_aPlayer.nCntPenlight == 40)
			{
				if (g_aPlayer.nCntPenlight >= 40)
				{
					g_aPlayer.nCntPenlight = 0;
				}

				//�Ö��̃z�����z�����ݒ�
				if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION)
				{//�~�܂��čÖ�

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
				{//�����čÖ�
					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION)
				{//�����čÖ�

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
				{//�����čÖ�

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
				}

				if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
				{//�t�B�[�o�[���Ȃ�

					//�Ö��̃z�����z�����ݒ�
					if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION)
					{//�~�܂��čÖ�

						g_aPlayer.nIdxWave[1] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[2] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[3] = SetHypnosisWavePlayer(g_aPlayer.rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
					}
					else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
					{//�����čÖ�

						g_aPlayer.nIdxWave[1] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[2] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[3] = SetHypnosisWavePlayer(g_aPlayer.rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
					}
				}
			}
		}
		else if (
			g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
		{//�Ö��A�N�V�������ĂȂ��Ƃ�

		 //�Ö��̉�������
			StopSound(SOUND_LABEL_BGM_HYPNOSIS);
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVECURSOR)
		{
			DISPATCH *pDispatch = GetDispatch();

			//�ڕW�̊p�x�����߂�
			g_aPlayer.fRotDest = atan2f((g_aPlayer.pos.x - pDispatch->pos.x), (g_aPlayer.pos.z - pDispatch->pos.z));
			g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

			//�p�x�̐��K��
			g_aPlayer.fRotDiff = RotNormalize(g_aPlayer.fRotDiff);

			//�p�x�̕␳������
			g_aPlayer.rot.y += g_aPlayer.fRotDiff;

			//�p�x�̐��K��
			g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);
		}

		//�O�Ղ̏��擾
		ORBIT *pOrbit = GetMeshOrbit();

		//�p�[�c���Ƃ̃}�g���b�N�X�擾
		D3DXMATRIX mtxParts = GetParentMatrix(10);

		pOrbit->pMtxparent = &mtxParts;

		//���[�V�����̍X�V
		UpdateMotionPlayer();

		//�L�����N�^�[�Ƃ̓����蔻��
		CollisionCharPlayer();

		//���W����
		LimitPosPlayer();

		//�e�̈ʒu�X�V
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		//�A�C�R���̈ʒu�X�V
		UpdatePositionIcon(g_aPlayer.nIdxIcon, g_aPlayer.pos);

		//�Z���t�̈ʒu�X�V
		SetPositionSerif(g_aPlayer.nIdxSerif, { g_aPlayer.pos.x, g_aPlayer.pos.y + 150.0f, g_aPlayer.pos.z });

		//�Ռ��g�̈ʒu�X�V
		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
		{//�ړ��Ö��A�N�V������

			for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
			{
				SetPositionImpactWave(g_aPlayer.nIdxWave[nCntWave], g_aPlayer.move);
			}
		}

		//�Z���t�̈ʒu�X�V
		SetPositionSerif(g_aPlayer.nIdxSerif, { g_aPlayer.pos.x, g_aPlayer.pos.y + 150.0f, g_aPlayer.pos.z });
	}

	//��ԍX�V
	UpdateStatePlayer();

}

//==================================================================================
//�`���[�g���A���̑���
//==================================================================================
void ControllTutorialPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�G�f�B�b�g�̏��擾
	Edit  *pEdit = GetEdit();

	//�^�[�Q�b�g�̏��擾
	DISPATCH *pDispatch = GetDispatch();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	if (/*GetGameState() == GAMESTATE_NONE && */pDispatch->bUse == false)
	{//�^�[�Q�b�g���ȊO

		//========================================================================
		//�ړ��̃X�e�b�v�ȏ�ő���\
		//========================================================================
		if (pTutorial->nStep >= TUTORIAL_STEP_MOVE)
		{
			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR)
			{//�ړ��s���[�V�������͋����L�����Z��

				if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
				{//���L�[�������ꂽ,���ړ�

					if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
					{//A+W,����ړ�

						g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = D3DX_PI * MOVE_LRUP + pCamera->rot.y;
					}
					else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
					{//A+S,�����ړ�

						g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = D3DX_PI * MOVE_LRDW + pCamera->rot.y;
					}
					else
					{//A,���ړ�

						g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = D3DX_PI * MOVE_LR + pCamera->rot.y;
					}

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
					{//�Ö����Ă��Ȃ�������

						g_aPlayer.bMove = true;
					}

					//���ړ�����
					pTutorial->aControl.bLMove = true;
				}
				else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
				{//D�L�[�������ꂽ,�E�ړ�

					if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
					{//D+W,�E��ړ�

						g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRUP + pCamera->rot.y;
					}
					else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
					{//D+S,�E���ړ�

						g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRDW + pCamera->rot.y;
					}
					else
					{//D,�E�ړ�

						g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = -D3DX_PI * MOVE_LR + pCamera->rot.y;
					}

					//�E�ړ�����
					pTutorial->aControl.bRMove = true;

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
					{//�Ö����Ă��Ȃ�������

						g_aPlayer.bMove = true;
					}
				}
				else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//W�������ꂽ�A���ړ�

					g_aPlayer.move.x += sinf(pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI + pCamera->rot.y;

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
					{//�Ö����Ă��Ȃ�������

						g_aPlayer.bMove = true;
					}

					//���ړ�����
					pTutorial->aControl.bUPMove = true;

				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//S�������ꂽ�A��O�ړ�

					g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = pCamera->rot.y;

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
					{//�Ö����Ă��Ȃ�������

						g_aPlayer.bMove = true;
					}

					//��O�ړ�����
					pTutorial->aControl.bDWMove = true;
				}
				else
				{//�Ȃ�̃L�[��������Ă��Ȃ�

					if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f ||
						g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
					{
						g_aPlayer.bMove = false;

						if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
						{//�ړ��Ö����̎�

							//���[�V�����̐ݒ�
							SetMotisonPlayer(PLAYERMOTION_ACTION);
						}

						if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION && g_aPlayer.bFever == true && g_aPlayer.bATK == true)
						{//�ړ��Ö����̎�

							//���[�V�����̐ݒ�
							SetMotisonPlayer(PLAYERMOTION_FEVERACTION);
						}
					}
				}
			}
		}

		//========================================================================
		//�h���̃X�e�b�v�ȏ�ő���\
		//========================================================================
		if (pTutorial->nStep >= TUTORIAL_STEP_DISPATCH && pTutorial->nStep != TUTORIAL_STEP_MORS)
		{
			if (GetGamepadTrigger(BUTTON_RB, 0) || GetKeyboardTrigger(DIK_RSHIFT) == true)
			{//�h���̎g�p�󋵓���ւ�

				SetRightDisPatch();
			}
			if (GetGamepadTrigger(BUTTON_LB, 0) || GetKeyboardTrigger(DIK_LSHIFT) == true)
			{//�h���̎g�p�󋵓���ւ�

				SetLeftDisPatch();
			}
		}

		//========================================================================
		//�Ö��̃X�e�b�v�ȏ�ő���\
		//========================================================================
		if (pTutorial->nStep >= TUTORIAL_STEP_HYPNOSIS && pTutorial->nStep != TUTORIAL_STEP_MORS)
		{
			if (GetKeyboardPress(DIK_L) == true || GetGamepadPress(BUTTON_A, 0))
			{//L�������ꂽ && A�{�^���������ꂽ

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
					g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
					g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
					g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
				{//�Ö����Ă��Ȃ�������

				 //�Ö�SE
					PlaySound(SOUND_LABEL_BGM_HYPNOSIS);
				}

				if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f &&
					g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
				{//�~�܂��Ă���Ƃ��͋���

					g_aPlayer.bMove = false;
					g_aPlayer.move.x = 0.0f;
					g_aPlayer.move.z = 0.0f;

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.bFever == false)
					{//�t�B�[�o�[������Ȃ�������

						//���[�V�����̐ݒ�
						SetMotisonPlayer(PLAYERMOTION_ACTION);
					}

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION && g_aPlayer.bFever == true)
					{//�t�B�[�o�[����������

						//���[�V�����̐ݒ�
						SetMotisonPlayer(PLAYERMOTION_FEVERACTION);
					}
				}
				else
				{//�����Ă�Ƃ��͎�

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.bFever == false)
					{//f�t�B�[�o�[������Ȃ�������

						//���[�V�����̐ݒ�
						SetMotisonPlayer(PLAYERMOTION_MOVEACTION);
					}

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION && g_aPlayer.bFever == true)
					{//�t�B�[�o�[����������

						//���[�V�����̐ݒ�
						SetMotisonPlayer(PLAYERMOTION_FEVERMOVEACTION);
					}
				}
			}

			if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
			{//L�������ꂽ && A�{�^���������ꂽ

				//�U����ԉ���
				g_aPlayer.bATK = false;

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		//========================================================================
		//���]�̃X�e�b�v�ȏ�ő���\
		//========================================================================
		if (pTutorial->nStep >= TUTORIAL_STEP_SENNOU && pTutorial->nStep != TUTORIAL_STEP_MORS)
		{
			if (g_aPlayer.bATK == false && g_aPlayer.bJump == false && GetKeyboardTrigger(DIK_RETURN) == true ||
				g_aPlayer.bATK == false && g_aPlayer.bJump == false && GetGamepadTrigger(BUTTON_B, 0))
			{//ENTER�������ꂽ,,�e����

				//�������̐ݒ菈��
				SetCutepillow(g_aPlayer.pos, g_aPlayer.rot, PILLOWPARENT_PLAYER);

				if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
				{//�t�B�[�o�[���Ȃ�

					//�S���ʕ�����
					SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + D3DX_PI * 0.5f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
					SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + D3DX_PI * 1.0f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
					SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + -D3DX_PI * 0.5f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
				}

				//SE�Z�b�g
				PlaySound(SOUND_LABEL_SE_DAKIMAKURA);
			}
		}
	}

	//���݂ƖڕW�̍��������߂�
	g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

	//�p�x�̐��K��
	g_aPlayer.fRotDiff = RotNormalize(g_aPlayer.fRotDiff);

	//�p�x�̕␳������
	g_aPlayer.rot.y += g_aPlayer.fRotDiff * 0.1f;

	//�p�x�̐��K��
	g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

	//�d�͏���
	//g_aPlayer.move.y -= 1.0f;

	if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
	{//�t�B�[�o�[���Ȃ�

		//����
		g_aPlayer.move.x *= 1.1f;
		g_aPlayer.move.z *= 1.1f;
	}

	if (pTutorial->nStep == TUTORIAL_STEP_MORS)
	{
		g_aPlayer.move.x *= 0.8f;
		g_aPlayer.move.z *= 0.8f;
	}

	//�ʒu�X�V
	g_aPlayer.pos += g_aPlayer.move;

	//��������
	if (g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION)
	{
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;
	}
	else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
	{
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR_HYPNOSIS;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR_HYPNOSIS;

		if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
		{//�t�B�[�o�[���Ȃ�

		 //����
			g_aPlayer.move.x *= 1.1f;
			g_aPlayer.move.z *= 1.1f;
		}
	}
}

//==================================================================================
//�^�C�g���̍X�V����
//==================================================================================
void UpdateTitlePlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	if (g_aPlayer.bUse == true)
	{//�g�p���Ă�����

		//�ߋ��̈ʒu�ۑ�
		g_aPlayer.posOld = g_aPlayer.pos;

		//�����Œ�
		g_aPlayer.rot.y += 0.025f;

		g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

		g_aPlayer.fRotDiff = 0.0f;									//�����̍���
		g_aPlayer.fRotDest = g_aPlayer.rot.y;						//�����̍���

		g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y + g_aPlayer.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y + g_aPlayer.rot.y) * 1.0f;
		g_aPlayer.bMove = true;

		//�ʒu�X�V
		g_aPlayer.pos += g_aPlayer.move;

		//��������
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//�ړ����Ă�Ƃ�

			g_aPlayer.nCntWalk++;	//�J�E���g���Z

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//���s�p�[�e�B�N���̐ݒ�
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}

		}
		else
		{//�j���[�g�������[�V����

			if (g_aPlayer.bATK == false && g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR)
			{
				//�U����ԉ���
				g_aPlayer.bATK = false;

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION)
		{//�Ö��A�N�V������

			g_aPlayer.nCntPenlight++;

			
		}

		//���[�V�����̍X�V
		UpdateMotionPlayer();

		//���W����
		LimitPosPlayer();

		//�e�̈ʒu�X�V
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
	}

	//��ԍX�V
	UpdateStatePlayer();
}

//==================================================================================
//�����L���O�̍X�V����
//==================================================================================
void UpdateRankingPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	if (g_aPlayer.bUse == true)
	{//�g�p���Ă�����

		//�ߋ��̈ʒu�ۑ�
		g_aPlayer.posOld = g_aPlayer.pos;

		//�����Œ�
		g_aPlayer.rot.y += 0.025f;

		g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

		g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y + g_aPlayer.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y + g_aPlayer.rot.y) * 1.0f;
		g_aPlayer.bMove = true;

		//�ʒu�X�V
		g_aPlayer.pos += g_aPlayer.move;

		//��������
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//�ړ����Ă�Ƃ�

			g_aPlayer.nCntWalk++;	//�J�E���g���Z

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//���s�p�[�e�B�N���̐ݒ�
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_RANKING)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_RANKING);
			}

		}

		//���[�V�����̍X�V
		UpdateMotionPlayer();

		//���W����
		LimitPosPlayer();

		//�e�̈ʒu�X�V
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
	}

	//��ԍX�V
	UpdateStatePlayer();
}

//==================================================================================
//���U���g�̍X�V����
//==================================================================================
void UpdateResultPlayer(void)
{
	if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION)
	{//�ړ��Ö����̎�

		//���[�V�����̐ݒ�
		SetMotisonPlayer(PLAYERMOTION_ACTION);
	}

	//���[�V�����̍X�V
	UpdateMotionPlayer();

	PrintDebugProc(
		"---------------- �v���C���[��� ----------------\n"
		"�y���_�z[X�F%f Y�F%f Z�F%f]\n",
		g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
}