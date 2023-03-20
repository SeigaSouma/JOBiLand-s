//=============================================================================
//
// ���u���� [mob.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "mob.h"
#include "crowd.h"
#include "input.h"
#include "game.h"
#include "player.h"
#include "calculation.h"
#include "debugproc.h"
#include "rotate.h"
#include "hypnosis_gauge.h"
#include "calculation.h"
#include "shadow.h"
#include "buddy.h"
#include "minimap_icon.h"
#include "debugproc.h"
#include "iventsign.h"
#include "EventWindow.h"
#include "camera.h"
#include "manyUI.h"
#include "manypoint.h"
#include "eventstamp.h"
#include "tutorial.h"
#include "sound.h"
#include "comment.h"
#include "iventradar.h"
#include "endmessage.h"
#include "endcountdown.h"
#include "player.h"
#include "fenish.h"


//�}�N����`
#define MOVE		(0.9f)
#define DISTANCE	(60.0f)	//�Ԋu
#define DISTANCE_LENGTH	(200.0f)	//�Ԋu
#define MOB_NUMBER		(33)		//���u�̃��f���̔ԍ�
#define HYPSUB_INTER	(80)		//�Ö��x�Q�[�W�����Ԋu
#define HYPADD_INTER	(40)		//�Ö��x�Q�[�W�����Ԋu
#define ROT_CNT			(50)		//rot�̐؂�ւ�����
#define ROT_ROUND		(0.0025f)	//�X����p�x
#define NORMAL_SPAWN	(30)		//�ʏ�X�|�[���̊Ԋu
#define MAX_MOVE		(451)		//���u�̈ړ����Ԃ̍ő�l
#define MIN_MOVE		(100)		//���u�̈ړ����Ԃ̍ŏ��l
#define NUM_BILL		(51)		//�����̐�
#define MOVE_APPIEAR	(0.7f)		//�o�����̈ړ���
#define MOVE_MANY_APPIEAR	(2.1f)	//�t�B�[�o�[�o�����̈ړ���
#define START_MOB		(200)		//�J�n���̃��u�̐�
#define MAX_SPAWN_MOB	(700)		//��ɏo��ő僂�u��
#define STOP_SPAWN_TIME	(70)		//��ʔ����J�n����I���܂ł̎���	����35�b	60*75	60*175�ɏI��
#define MANY_TIME		(30)		//��ʔ������̃X�|�[���Ԋu
#define SPWAN_DISTANCE	(100.0f)	//��ʔ������̃X�|�[������
//#define WAVE_CHANGE_TIME	(60 * 3)	//�E�F�[�u�����ꏊ�̕ύX����
#define WAVE_SPAWN		(5)			//�E�F�[�u�����̊Ԋu
#define WAVE_START		(60 * 3)	//�E�F�[�u�����X�^�[�g����
#define WAVE_STOP		(60 * 5)	//�E�F�[�u�I������
#define FLEN            (450.0f)     //

//�v���g�^�C�v�錾
void ControllMob(int nCntMob);
void UpdateStateMob(int nCntMob);
void LimitPosMob(int nCntMob);
void CollisionModelMob(int nCntMob);
void ReadSetMobPos(void);
void UpdateCrawlMob(int nCntMob);
void UpdateSpawnMob(int nCntMob);
void ManySpawnMob(int nCntMob);
void ManySpawnSetPos(void);
void MobSerifSet(int nCntMob);

//�O���[�o���ϐ��錾
const char *c_apFilenameMob[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\crowd02.jpg",
	"data\\TEXTURE\\crowd05.jpg",
	"data\\TEXTURE\\crowd04.jpg",
	"data\\TEXTURE\\crowd03.jpg",
	"data\\TEXTURE\\crowd07.jpg",
	"data\\TEXTURE\\crowd08.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureMob[(sizeof c_apFilenameMob) / sizeof(*c_apFilenameMob)] = {};	//�e�N�X�`���̃|�C���^
Mob g_aMob[MAX_MOB];		//���u�̏��
ManySpawn g_aManySpawn;		//��ʔ����̏��
SwitchSpawn g_aUseSpawn;
WaveSpawn g_aWaveSpawn;		//�ʏ픭���i�E�F�[�u�j�̏��
SwitchSE g_aSwitchSE;		//SE�g�p��

int g_nSpawnCounter;		//�X�|�[�����鎞��
int g_nManySpawnCounter;	//��ʔ����ŃX�|�[�����鎞��
int g_nNumMob;
bool g_bManySpawn;
int g_nGameTime;		//�Q�[������
int g_nMobSerifTime;		//�Z���t�ݒ肷�鎞��

//==================================================================================
//���u�̏���������
//==================================================================================
void InitMob(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMob) / sizeof(*c_apFilenameMob); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMob[nCntTex],
			&g_apTextureMob[nCntTex]);
	}

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�e�v�f������
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		g_aMob[nCntMob].nHypnosis = 0;			//�Ö��x
		g_aMob[nCntMob].nSubHypCounter = HYP_CNTTIME;		//�Ö��x��������܂ł̃J�E���^�[
		g_aMob[nCntMob].nSubHypInter = 0;		//�Ö��x�����炷�Ԋu
		g_aMob[nCntMob].nAddHypInter = HYPADD_INTER;		//�Ö��x�𑝂₷�Ԋu
		g_aMob[nCntMob].nSwitchCounter = 0;		//rot�̐؂�ւ�����
		g_aMob[nCntMob].nTurnSwitchX = 1;		//rot.x��+-�؂�ւ�
		g_aMob[nCntMob].nTurnSwitchZ = 1;		//rot.z��+-�؂�ւ�
		g_aMob[nCntMob].nSpawnCounter = 0;		//���u�̃X�|�[�����鎞��
		g_aMob[nCntMob].nSpawnNumber = 0;		//���u����x�ɃX�|�[�����鐔
		g_aMob[nCntMob].nSpawnType = 0;			//�X�|�[���n�_�̎��
		g_aMob[nCntMob].nMoveCounter = 0;		//���u�̈ړ��̃J�E���^�[
		g_aMob[nCntMob].nIdxIcon = 0;			//�~�j�}�b�v�A�C�R���̃C���f�b�N�X�ԍ�
		g_aMob[nCntMob].nTexType = 0;			//�e�N�X�`���^�C�v

		g_aMob[nCntMob].fAlpha = 0.4f;				//�����x
		g_aMob[nCntMob].fRandAlpha = (float)(rand() % 51 + 13) * 0.0001f;			//�����x�̑�����


		g_aMob[nCntMob].aModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aMob[nCntMob].aModel.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aMob[nCntMob].aModel.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
		g_aMob[nCntMob].aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aMob[nCntMob].aModel.rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
		g_aMob[nCntMob].aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMob[nCntMob].aModel.mtxWorld);
		D3DXMatrixIdentity(&g_aMob[nCntMob].aModel.OldmtxWorld);

		g_aMob[nCntMob].aModel.nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aMob[nCntMob].aModel.nIdxRotate = -1;	//���邮��̃C���f�b�N�X�ԍ�
		g_aMob[nCntMob].aModel.nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
		g_aMob[nCntMob].aModel.nIdxSerif = -1;			//�Z���t�̃C���f�b�N�X�ԍ�
		g_aMob[nCntMob].aModel.nShadow = 1;			//�e���g�����ǂ���

		g_aMob[nCntMob].aModel.nNumVtx = 0;			//���_��
		g_aMob[nCntMob].aModel.dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aMob[nCntMob].aModel.pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aMob[nCntMob].aModel.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
		g_aMob[nCntMob].aModel.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

		g_aMob[nCntMob].aModel.nType = 0;			//�ǂݍ��ރ��f���̎��
		g_aMob[nCntMob].aModel.nParent = -1;		//�e�̔ԍ�
		g_aMob[nCntMob].aModel.bUse = false;			//�g�p���Ă��邩
		g_aMob[nCntMob].aModel.nState = 0;			//���

		g_aMob[nCntMob].aModel.pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
		g_aMob[nCntMob].aModel.pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aMob[nCntMob].aModel.pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
			g_aMob[nCntMob].aModel.pMatData[nCntMat].pTextureFilename = NULL;	//�}�e���A���̃f�[�^
			g_aMob[nCntMob].aModel.acFilename[nCntMat] = NULL;	//�t�@�C����
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aMob[nCntMob].aModel.pTexture[nCntTex] = NULL;	//�e�N�X�`���ւ̃|�C���^
		}
		g_aMob[nCntMob].aModel.dwNumMat = NULL;			//�}�e���A���̐�
	}

	//��ʔ������̏�����
	g_aManySpawn.nSpawnCounter = 0;		//��ʔ�������܂ł̎���
	g_aManySpawn.nStopTime = 0;			//��ʔ����I���܂ł̎���
	g_aManySpawn.nBillNumber = rand() % NUM_BILL;	//�����̏ꏊ
	g_aManySpawn.nCounter = 0;
	g_aManySpawn.bFirst = false;		//1�Ԗڂ̃X�|�[��
	g_aManySpawn.bSecond = false;		//2�Ԗڂ̃X�|�[��
	g_aManySpawn.bThird = false;		//3�Ԗڂ̃X�|�[��

	//�E�F�[�u�����̏�����
	g_aWaveSpawn.nStartSpawn = 0;			//�����J�n����
	g_aWaveSpawn.nStopSpawn = 0;			//�����I������
	g_aWaveSpawn.nPosChangeCounter = 0;		//�����ʒu�ς���܂ł̎���
	g_aWaveSpawn.nSpawnCounter = 0;			//��������܂ł̎���
	g_aWaveSpawn.nBillNumber = rand() % NUM_BILL;	//�����̏ꏊ

	//�X�|�[�����
	for (int nCntSpawn = 0; nCntSpawn < MAX_SPAWN; nCntSpawn++)
	{
		g_aManySpawn.aSpawn[nCntSpawn].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�X�|�[���n�_
		g_aManySpawn.aSpawn[nCntSpawn].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�X�|�[���n�_�̌���
		g_aManySpawn.aSpawn[nCntSpawn].nType = 0;				//�X�|�[������n�_�̎��
		g_aManySpawn.aSpawn[nCntSpawn].nMany = 0;				//��ʔ������邩�̔���
		g_aManySpawn.aSpawn[nCntSpawn].bUse = false;            //��������ʔ����n�_���̔���
	}

	g_nSpawnCounter = 0;		//�X�|�[�����鎞��
	g_nManySpawnCounter = 0;
	g_nNumMob = 0;
	g_aUseSpawn.bUseSpawn = false;	//��ʔ������Ă邩
	g_nGameTime = 0;	//�Q�[������
	g_nMobSerifTime = 0;	//�Z���t�̐ݒ肷�鎞��
	g_aSwitchSE.bUseRotate = false;		//���邮��SE�g�p���Ă邩

#if _DEBUG

	//��ʔ������Ȃ�
	g_bManySpawn = false;

#else NDEBUG

	//��ʔ�������
	g_bManySpawn = true;

#endif

	//���u�̃X�|�[���ʒu�̓ǂݍ���
	ReadSetMobPos();

	if (GetMode() == MODE_GAME)
	{
		while (1)
		{//��ʔ�������ꏊ�����߂�

			g_aManySpawn.nBillNumber = rand() % NUM_BILL;

			if (g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nMany == 1)
			{//��ʔ�������ʒu��������

				break;
			}
		}

		//�J�n���̃��u��ݒ�
		for (int nCntMob = 0; nCntMob < START_MOB; nCntMob++)
		{
			int nCntNumber = rand() % NUM_BILL;		//�����̎��

			//���u�̐ݒ�
			SetMob(g_aManySpawn.aSpawn[nCntNumber].pos, g_aManySpawn.aSpawn[nCntNumber].rot, g_aManySpawn.aSpawn[nCntNumber].nType, rand() % 3 + MOB_NUMBER);
		}
	}
}

//==================================================================================
//���u�̏I������
//==================================================================================
void UninitMob(void)
{

	StopSound();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMob) / sizeof(*c_apFilenameMob); nCntTex++)
	{
		if (g_apTextureMob[nCntTex] != NULL)
		{
			g_apTextureMob[nCntTex]->Release();
			g_apTextureMob[nCntTex] = NULL;
		}
	}

}

//==================================================================================
//���u�̍X�V����
//==================================================================================
void UpdateMob(void)
{
	//���f���̏��擾
	ModelModel *pModel = GetModel();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();
	Rotate *pRotate = GetRotate();

	if (GetGameState() == GAMESTATE_NONE)
	{
		//�X�|�[�����鎞�Ԃ𑝉�����
		g_nSpawnCounter++;
		g_nManySpawnCounter++;
		g_aWaveSpawn.nPosChangeCounter++;
		g_aWaveSpawn.nSpawnCounter++;
		g_aWaveSpawn.nStartSpawn++;

		g_nGameTime++;

		if (g_bManySpawn == true)
		{//ON�̎�

			//��ʔ�������܂ł̎���
			g_aManySpawn.nSpawnCounter++;

			PrintDebugProc("[F9]�F�x�������E��ʔ���[ON]\n");
		}
		else
		{
			PrintDebugProc("[F9]�F�x�������E��ʔ���[OFF]\n");
		}
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F9) == true)
	{//F9�������ꂽ

		//��ʔ����؂�ւ�
		g_bManySpawn = g_bManySpawn ? false : true;
	}
#endif

	int nUse = 0;
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		if (g_aMob[nCntMob].aModel.bUse == true)
		{//�g�p���Ă�����

			//�g���Ă鐔���Z
			nUse++;

			//�Z���t�ݒ莞�ԉ��Z
			g_nMobSerifTime++;

			//�ߋ��̈ʒu�ۑ�
			g_aMob[nCntMob].aModel.posOld = g_aMob[nCntMob].aModel.pos;

			//���u�̑���
			ControllMob(nCntMob);

			//���u���m�̓����蔻��
			CollisionModelMob(nCntMob);

			//���W����
			LimitPosMob(nCntMob);

			//��ԍX�V
			UpdateStateMob(nCntMob);

			//�Ö��x�Q�[�W���g�p���Ă����Ԃɂ���
			/*if (g_aMob[nCntMob].aModel.nIdxHypGauge >= MAX_MOB)
			{
			int a = 0;
			}*/

			//���邮��̈ʒu�X�V
			SetPositionRotate(g_aMob[nCntMob].aModel.nIdxRotate, g_aMob[nCntMob].aModel.pos);

			//�Ö��x�Q�[�W�̈ʒu�X�V
			SetPositionHypnosisGauge(g_aMob[nCntMob].aModel.nIdxHypGauge, g_aMob[nCntMob].aModel.pos, g_aMob[nCntMob].nHypnosis);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aMob[nCntMob].aModel.nIdxShadow, g_aMob[nCntMob].aModel.pos);

			//�A�C�R���̈ʒu�X�V
			UpdatePositionIcon(g_aMob[nCntMob].nIdxIcon, g_aMob[nCntMob].aModel.pos);

			//�Z���t�̐ݒ菈��
			MobSerifSet(nCntMob);

			if (g_aMob[nCntMob].aModel.nState != MOBSTATE_APPIEAR)
			{
				//�Z���t�̈ʒu�X�V
				SetPositionSerif(g_aMob[nCntMob].aModel.nIdxSerif, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			}

			if (g_aMob[nCntMob].aModel.nState != MOBSTATE_HYPNOSIS && g_aMob[nCntMob].aModel.nState != MOBSTATE_KEEP)
			{//�Ö���ԁA�Ö������Ă�Œ��ȊO�̎�

				//���邮��폜
				pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = false;
			}
		}

		//���u�̔���
		UpdateSpawnMob(nCntMob);
	}

	g_nNumMob = nUse;

	PrintDebugProc("���u�̐�%d\n", g_nNumMob);

	PrintDebugProc("��ʔ����̈ʒu [X:%f Y:%f Z:%f]\n",
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x, g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y, g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z);

	if (g_aUseSpawn.bUseSpawn == true)
	{
		PrintDebugProc("<<<<<��ʔ�����>>>>>\n");
	}

	//�Q�[������
	PrintDebugProc("�Q�[�����ԁF%d\n", g_nGameTime / 60);

	//�Q�[���I��30�b�O
	if (g_nGameTime == GAMETIME - (60 * 30))
	{
		SetEndmessage({ 1280.0f * 0.5f, 300.0f, 0.0f }, 150);
	}

	//�Q�[���I��5�b�O
	if (g_nGameTime == GAMETIME - (60 * 5))
	{
		SetEndCountDown(0);
	}

	//�Q�[���I��
	if (g_nGameTime == GAMETIME)
	{
		SetFinish(0);
	}
}

//==================================================================================
//���u�̑���
//==================================================================================
void ControllMob(int nCntMob)
{

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�d�͏���
	g_aMob[nCntMob].aModel.move.y -= GRAVITY;

	//�ʒu�X�V
	g_aMob[nCntMob].aModel.pos += g_aMob[nCntMob].aModel.move;

	//����
	g_aMob[nCntMob].aModel.move.x += (0.0f - g_aMob[nCntMob].aModel.move.x) * 0.15f;
	g_aMob[nCntMob].aModel.move.z += (0.0f - g_aMob[nCntMob].aModel.move.z) * 0.15f;
}

//==================================================================================
//���u�̍��W����
//==================================================================================
void LimitPosMob(int nCntMob)
{

	if (g_aMob[nCntMob].aModel.pos.y < 0.0f)
	{//�n�ʂ�艺�ɍs������

		g_aMob[nCntMob].aModel.pos.y = 0.0f;

		//�d�͏���
		g_aMob[nCntMob].aModel.move.y = 0.0f;
	}

	//�ړ�����
	if (g_aMob[nCntMob].aModel.pos.x + g_aMob[nCntMob].aModel.vtxMin.x < -LIMIT_POS) { g_aMob[nCntMob].aModel.pos.x = -LIMIT_POS - g_aMob[nCntMob].aModel.vtxMin.x; }
	if (g_aMob[nCntMob].aModel.pos.x + g_aMob[nCntMob].aModel.vtxMax.x > LIMIT_POS) { g_aMob[nCntMob].aModel.pos.x = LIMIT_POS - g_aMob[nCntMob].aModel.vtxMax.x; }
	if (g_aMob[nCntMob].aModel.pos.z + g_aMob[nCntMob].aModel.vtxMax.z > LIMIT_POS) { g_aMob[nCntMob].aModel.pos.z = LIMIT_POS - g_aMob[nCntMob].aModel.vtxMax.z; }
	if (g_aMob[nCntMob].aModel.pos.z + g_aMob[nCntMob].aModel.vtxMin.z < -LIMIT_POS) { g_aMob[nCntMob].aModel.pos.z = -LIMIT_POS - g_aMob[nCntMob].aModel.vtxMin.z; }

}

//==================================================================================
//���u�̔�������
//==================================================================================
void UpdateSpawnMob(int nCntMob)
{
	//�C�x���g�T�C���̏��擾
	IventSign *pIventSign = GetIventSign();

	//��ʔ����n�_�̏��擾
	MANYPOINT *pManyPoint = GetManyPoint();

	//�C�x���g�X�^���v�̏��擾
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//�R�����g�̏��擾
	Commnet *pComment = GetCommnet();

	if (g_aManySpawn.nStopTime < STOP_SPAWN_TIME)
	{//��莞�Ԍo�߂������ʔ���

		if (g_nManySpawnCounter >= MANY_TIME)
		{//�X�|�[���̑���

			if (g_aManySpawn.nSpawnCounter >= SECOND_SPAWNMOB - 330 && g_aManySpawn.bSecond == false)
			{//��莞�Ԍo�߂�����

				//�R�����g���g�p�����Ԃɂ���
				pComment->bUse = true;
			}
			else if (g_aManySpawn.nSpawnCounter >= FIRST_SPAWNMOB - 330 && g_aManySpawn.bFirst == false)
			{//��莞�Ԍo�߂�����

				//�R�����g���g�p�����Ԃɂ���
				pComment->bUse = true;
			}
			else if (g_aManySpawn.nSpawnCounter >= THIRD_SPAWNMOB - 330 && g_aManySpawn.bThird == false)
			{//��莞�Ԍo�߂�����

				//�R�����g���g�p�����Ԃɂ���
				pComment->bUse = true;
			}

			if (g_aManySpawn.nSpawnCounter >= SECOND_SPAWNMOB && g_aManySpawn.bSecond == false)
			{//��莞�Ԍo�߂�����

				//��ʔ����̃X�|�[��
				ManySpawnMob(nCntMob);

				//�R�����g������
				pComment->bUse = false;
			}
			else if (g_aManySpawn.nSpawnCounter >= FIRST_SPAWNMOB && g_aManySpawn.bFirst == false)
			{//��莞�Ԍo�߂�����

				//��ʔ����̃X�|�[��
				ManySpawnMob(nCntMob);

				//�R�����g������
				pComment->bUse = false;
			}
			else if (g_aManySpawn.nSpawnCounter >= THIRD_SPAWNMOB && g_aManySpawn.bThird == false)
			{//��莞�Ԍo�߂�����

				//��ʔ����̃X�|�[��
				ManySpawnMob(nCntMob);

				//�R�����g������
				pComment->bUse = false;
			}
		}
	}
	else if (g_aManySpawn.nStopTime >= STOP_SPAWN_TIME)
	{//��莞�Ԍo�߂����珉����

		//����ڂ̑�ʔ�����
		if (g_aManySpawn.nSpawnCounter >= (THIRD_SPAWNMOB + STOP_SPAWN_TIME) && g_aManySpawn.bThird == false)
		{//3�Ԗ�

			g_aManySpawn.bThird = true;			//3�Ԗڂ̃X�|�[��

			//��ʔ����ʒu�ݒ�
			ManySpawnSetPos();

		}
		else if (g_aManySpawn.nSpawnCounter >= (SECOND_SPAWNMOB + STOP_SPAWN_TIME) && g_aManySpawn.bSecond == false)
		{//2�Ԗ�

			g_aManySpawn.bSecond = true;		//2�Ԗڂ̃X�|�[��

			//��ʔ����ʒu�ݒ�
			ManySpawnSetPos();

		}
		else if (g_aManySpawn.nSpawnCounter >= (FIRST_SPAWNMOB + STOP_SPAWN_TIME) && g_aManySpawn.bFirst == false)
		{//1�Ԗ�

			g_aManySpawn.bFirst = true;			//1�Ԗڂ̃X�|�[��

			//��ʔ����ʒu�ݒ�
			ManySpawnSetPos();

		}
	}

	//�ʏ�X�|�[��
	if (g_nSpawnCounter >= NORMAL_SPAWN && g_aMob[nCntMob].aModel.bUse == false)
	{//��莞�Ԍo������

		if (g_nNumMob <= MAX_SPAWN_MOB)
		{//���u700�ȉ�
			int nBillNumber = rand() % NUM_BILL;		//�����̎��

			//���u�̔z�u
			SetMob(g_aManySpawn.aSpawn[nBillNumber].pos, g_aManySpawn.aSpawn[nBillNumber].rot, g_aManySpawn.aSpawn[nBillNumber].nType, rand() % 3 + MOB_NUMBER);
		}

		//�X�|�[�����鎞�Ԃ�����������
		g_nSpawnCounter = 0;
	}

	//�E�F�[�u�X�|�[��
	if (g_aWaveSpawn.nStartSpawn > WAVE_START && g_aMob[nCntMob].aModel.bUse == false)
	{//�����J�n

		g_aWaveSpawn.nStopSpawn++;		//�I���܂ł̎��ԉ��Z

		if (g_aWaveSpawn.nSpawnCounter >= WAVE_SPAWN)
		{//��莞�Ԍo������

			if (g_nNumMob <= MAX_SPAWN_MOB)
			{//���u700�ȉ�

				//���u�̔z�u
				SetMob(g_aManySpawn.aSpawn[g_aWaveSpawn.nBillNumber].pos, g_aManySpawn.aSpawn[g_aWaveSpawn.nBillNumber].rot, g_aManySpawn.aSpawn[g_aWaveSpawn.nBillNumber].nType, rand() % 3 + MOB_NUMBER);
			}

			//�X�|�[�����鎞�Ԃ�����������
			g_aWaveSpawn.nSpawnCounter = 0;

			if (g_aWaveSpawn.nStopSpawn > WAVE_STOP * MAX_MOB)
			{//�I������

				g_aWaveSpawn.nBillNumber = rand() % NUM_BILL;		//�����̎��

				//������
				g_aWaveSpawn.nStartSpawn = 0;
				g_aWaveSpawn.nStopSpawn = 0;
			}
		}
	}
}

//==================================================================================
//��ʔ����̃X�|�[������
//==================================================================================
void ManySpawnMob(int nCntMob)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�C�x���g�T�C���̏��擾
	IventSign *pIventSign = GetIventSign();

	//��ʔ����n�_�̏��擾
	MANYPOINT *pManyPoint = GetManyPoint();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���u�̔z�u

	//���̈ʒu
	SetMob(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos, g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot, g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType, rand() % 3 + MOB_NUMBER);
	
	//��1
	SetMob(
		D3DXVECTOR3(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x + sinf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * -SPWAN_DISTANCE,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z + cosf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * -SPWAN_DISTANCE),
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot,
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType,
		rand() % 3 + MOB_NUMBER);

	//��2
	SetMob(
		D3DXVECTOR3(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x + sinf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * (-SPWAN_DISTANCE * 2.0f),
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z + cosf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * (-SPWAN_DISTANCE * 2.0f)),
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot,
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType,
		rand() % 3 + MOB_NUMBER);

	//��3
	SetMob(
		D3DXVECTOR3(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x + sinf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * SPWAN_DISTANCE,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z + cosf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * SPWAN_DISTANCE),
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot,
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType,
		rand() % 3 + MOB_NUMBER);

	//��4
	SetMob(
		D3DXVECTOR3(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x + sinf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * (SPWAN_DISTANCE * 2.0f),
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z + cosf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * (SPWAN_DISTANCE * 2.0f)),
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot,
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType,
		rand() % 3 + MOB_NUMBER);

	g_nManySpawnCounter = 0;		//�X�|�[�����鑬��������

	if (g_aManySpawn.nStopTime == 0)
	{//��ʔ����̏I�����Ԃ�0�̎�

		//��ʔ����n�_��ڕW�̏ꏊ�Ƃ��ēn��
		pCamera->aMany.posRDest = g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos;
		pCamera->aMany.rotVDest = g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot;
		pCamera->nState = CAMERASTATE_MANYSPAWN;

		if (pPlayer->nNowMotionNum == PLAYERMOTION_ACTION || pPlayer->nNowMotionNum == PLAYERMOTION_MOVEACTION)
		{//�Ö��A�N�V�������Ă�����

			//�Ö��̉�������
			StopSound(SOUND_LABEL_BGM_HYPNOSIS);
		}

		//�C�x���g�T�C��ON
		pIventSign->bUse = true;

		//��ʔ����n�_ON
		pManyPoint->bUse = true;

		//UI�̐ݒ�
		SetManyUI(0);

		//�C�x���g�T�C���̐ݒ�
		SetIventSign(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos);
	}

	//��ʔ����̎��Ԃ𑫂�
	g_aManySpawn.nStopTime++;

	g_aUseSpawn.bUseSpawn = true;	//��ʔ������Ă��Ԃɂ���
}

//==================================================================================
//��ʔ����̈ʒu�ݒ�
//==================================================================================
void ManySpawnSetPos(void)
{
	//�C�x���g�T�C���̏��擾
	IventSign *pIventSign = GetIventSign();

	//��ʔ����n�_�̏��擾
	MANYPOINT *pManyPoint = GetManyPoint();

	//�C�x���g�X�^���v�̏��擾
	EVENTSTAMP *pEventStamp = GetEventStamp();

	while (1)
	{
		g_aManySpawn.nBillNumber = rand() % NUM_BILL; 		//��ʔ�������ꏊ

		if (g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nMany == 1 && g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].bUse == false)
		{//��ʔ�������ʒu��������

			break;
		}

		else if (g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nMany == 1 && g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].bUse == true)
		{
			g_aManySpawn.nCounter++;

			if (g_aManySpawn.nCounter >= NUM_BILL - 1)
			{
				for (int nCount = 0; nCount < NUM_BILL; nCount++)
				{
					g_aManySpawn.aSpawn[nCount].bUse = false;
				}

				while (1)
				{//��ʔ�������ꏊ�����߂�

					g_aManySpawn.nBillNumber = rand() % NUM_BILL;

					if (g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nMany == 1)
					{//��ʔ�������ʒu��������

						break;
					}
				}

				g_aManySpawn.nCounter = 0;

				break;
			}
		}
	}

	//��ʔ����I�����Ԃ̏�����
	g_aManySpawn.nStopTime = 0;

	g_aUseSpawn.bUseSpawn = false;	//��ʔ������Ȃ���Ԃɂ���

	//�C�x���g�T�C��OFF
	pIventSign->bUse = false;

	//��ʔ����n�_OFF
	pManyPoint->bUse = false;

	//�C�x���g�X�^���v����
	pEventStamp[EVENTTYPE_MANYMOB].bUse = false;
}

//==================================================================================
//���u�̏�ԍX�V����
//==================================================================================
void UpdateStateMob(int nCntMob)
{
	Rotate *pRotate = GetRotate();
	Player *pPlayer = GetPlayer();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	switch (g_aMob[nCntMob].aModel.nState)
	{
	case MOBSTATE_NONE:		//�ʏ���

		if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_USEFEVER)
		{
			//���낿��돈��
			UpdateCrawlMob(nCntMob);
		}
		else if(GetMode() == MODE_GAME)
		{
			//���낿��돈��
			UpdateCrawlMob(nCntMob);
		}

		//�Ö��x�𑝂₷�Ԋu��������
		g_aMob[nCntMob].nAddHypInter = 0;

		if (g_aMob[nCntMob].aModel.rot.x >= 0.01f || g_aMob[nCntMob].aModel.rot.x <= -0.01f ||
			g_aMob[nCntMob].aModel.rot.z >= 0.01f || g_aMob[nCntMob].aModel.rot.z <= -0.01f)
		{//���u��rot��߂�

			g_aMob[nCntMob].aModel.rot.x = 0.0f;
			g_aMob[nCntMob].aModel.rot.z = 0.0f;
		}

		//�Ö��x��������܂ł̃J�E���^�[�����炷
		g_aMob[nCntMob].nSubHypCounter--;

		//�Ö��x�����炷�Ԋu
		g_aMob[nCntMob].nSubHypInter++;

		//���Ԋu�ŃQ�[�W�����炷
		if (g_aMob[nCntMob].nHypnosis > 0 && g_aMob[nCntMob].nSubHypCounter <= 0 && (g_aMob[nCntMob].nSubHypInter % HYPSUB_INTER) == 0)
		{
			//�Ö��x�Q�[�W�����炷
			g_aMob[nCntMob].nHypnosis--;
		}

		if (g_aMob[nCntMob].nHypnosis <= 0)
		{//�Ö��x�Q�[�W���O�̎�

		    //���邮����g�p���Ă��Ȃ���Ԃɂ���
			pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = false;

			//�Ö��x�Q�[�W���g�p���Ă��Ȃ���Ԃɂ���
			pHypnosis[g_aMob[nCntMob].aModel.nIdxHypGauge].bDisp = false;
		}

		break;

	case MOBSTATE_DMG:			//�_���[�W���

								//�Ö��x���O�ɂ���
		g_aMob[nCntMob].nHypnosis = 0;

		break;

	case MOBSTATE_KEEP:		//�Ö��ɂ������Ă�����


		pHypnosis[g_aMob[nCntMob].aModel.nIdxHypGauge].bDisp = true;

		//rot�؂�ւ��̎��Ԃ𑝂₷
		g_aMob[nCntMob].nSwitchCounter++;

		//�Ö��x�����J�E���^�[�����炷
		g_aMob[nCntMob].nSubHypCounter--;
		g_aMob[nCntMob].nSubHypInter++;

		if (g_aMob[nCntMob].nSubHypCounter <= 0 && (g_aMob[nCntMob].nSubHypInter % HYPSUB_INTER) == 0)
		{//�Ö��x0�A��������܂ł̃J�E���^�[��0�A��莞�Ԍo������

			//�Ö��x�Q�[�W�����炷
			g_aMob[nCntMob].nHypnosis--;

			if (g_aMob[nCntMob].nHypnosis <= 0)
			{
				g_aMob[nCntMob].nHypnosis = 0;
			}

			g_aMob[nCntMob].nSubHypInter = 0;
			g_aMob[nCntMob].nAddHypInter = 0;
		}

		if (g_aMob[nCntMob].nHypnosis <= 0 && g_aMob[nCntMob].nAddHypInter <= 0)
		{
			//�ʏ��Ԃɂ���
			g_aMob[nCntMob].aModel.nState = MOBSTATE_NONE;
		}

		if (g_aMob[nCntMob].nSwitchCounter == ROT_CNT / 2)
		{//��莞�Ԍo������rot+-�؂�ւ�
			g_aMob[nCntMob].nTurnSwitchX *= -1;
		}

		if (g_aMob[nCntMob].nSwitchCounter == ROT_CNT)
		{//��莞�Ԍo������rot+-�؂�ւ�
			g_aMob[nCntMob].nTurnSwitchZ *= -1;

			g_aMob[nCntMob].nSwitchCounter = 0;
		}

		//���u���X���ĉ�
		g_aMob[nCntMob].aModel.rot.x += ROT_ROUND * g_aMob[nCntMob].nTurnSwitchX;
		g_aMob[nCntMob].aModel.rot.z += ROT_ROUND * g_aMob[nCntMob].nTurnSwitchZ;

		g_aMob[nCntMob].aModel.rot.x = RotNormalize(g_aMob[nCntMob].aModel.rot.x);
		g_aMob[nCntMob].aModel.rot.z = RotNormalize(g_aMob[nCntMob].aModel.rot.z);


		//if (pPlayer->nNowMotionNum != PLAYERMOTION_ACTION && g_aMob[nCntMob].nHypnosis != 10)
		//{//�v���C���[���Ö�����߂��Ƃ� && �Ö��x��10����Ȃ��Ƃ�

		//    //�Ö��x����������܂ł̃J�E���^�[��������
		//	g_aMob[nCntMob].nSubHypCounter = HYP_CNTTIME;

		//	g_aMob[nCntMob].aModel.nState = MOBSTATE_NONE;		//�ʏ��Ԃɂ���
		//}
		break;

	case MOBSTATE_HYPNOSIS:		//�Ö����

		if (g_aMob[nCntMob].aModel.rot.x >= 0.01f || g_aMob[nCntMob].aModel.rot.x <= -0.01f ||
			g_aMob[nCntMob].aModel.rot.z >= 0.01f || g_aMob[nCntMob].aModel.rot.z <= -0.01f)
		{//���u���X���Ă�ꍇ

		    //���u��rot��߂�
			g_aMob[nCntMob].aModel.rot.x = 0.0f;
			g_aMob[nCntMob].aModel.rot.z = 0.0f;
		}

		//�Ö��x�����J�E���^�[�����炷
		g_aMob[nCntMob].nSubHypCounter--;
		g_aMob[nCntMob].nSubHypInter++;

		if (g_aMob[nCntMob].nHypnosis > 0 && g_aMob[nCntMob].nSubHypCounter <= 0 && (g_aMob[nCntMob].nSubHypInter % HYPSUB_INTER) == 0)
		{//�Ö��x0�A��������܂ł̃J�E���^�[��0�A��莞�Ԍo������

		    //�Ö��x�Q�[�W�����炷
			g_aMob[nCntMob].nHypnosis--;
			g_aMob[nCntMob].nSubHypInter = 0;

			//�ʏ��Ԃɂ���
			g_aMob[nCntMob].aModel.nState = MOBSTATE_NONE;
		}

		break;

	case MOBSTATE_WEAR:			//�������J�o�[����Ă�����

		break;

	case MOBSTATE_APPIEAR:		//�o�����

		//���u�̈ړ�
		if (g_aUseSpawn.bUseSpawn == false)
		{
			g_aMob[nCntMob].aModel.pos.x = g_aMob[nCntMob].aModel.pos.x + sinf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * MOVE_APPIEAR;
			g_aMob[nCntMob].aModel.pos.z = g_aMob[nCntMob].aModel.pos.z + cosf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * MOVE_APPIEAR;
		}
		else if (g_aUseSpawn.bUseSpawn == true)
		{//��ʔ����̎�

			g_aMob[nCntMob].aModel.pos.x = g_aMob[nCntMob].aModel.pos.x + sinf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * MOVE_MANY_APPIEAR;
			g_aMob[nCntMob].aModel.pos.z = g_aMob[nCntMob].aModel.pos.z + cosf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * MOVE_MANY_APPIEAR;
		}

		if (g_aMob[nCntMob].fAlpha < 1.0f)
		{//�����x��1.0�����̎�

		    //�����x���グ��
			g_aMob[nCntMob].fAlpha += g_aMob[nCntMob].fRandAlpha;

		}
		else if (g_aMob[nCntMob].fAlpha >= 1.0f)
		{//�����x��1.0�ȏ�̏ꍇ

			g_aMob[nCntMob].fAlpha = 0.4f;	//�����x������

			g_aMob[nCntMob].aModel.rot.y = RotNormalize(g_aMob[nCntMob].aModel.rot.y);		//�p�x�̐��K��
			g_aMob[nCntMob].aModel.rot.y += D3DXToRadian(90.0f);	//�������X�O�x�X����
			g_aMob[nCntMob].aModel.rot.y = RotNormalize(g_aMob[nCntMob].aModel.rot.y);		//�p�x�̐��K��

			g_aMob[nCntMob].aModel.nState = MOBSTATE_NONE;			//�ʏ���
		}

		//�����x�̐ݒ�
		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aMob[nCntMob].aModel.pMatData[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aMob[nCntMob].fAlpha);
			g_aMob[nCntMob].aModel.pMatData[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aMob[nCntMob].fAlpha);
		}

		break;
	}

}

//==================================================================================
//���u�̍s���X�V����
//==================================================================================
void UpdateActionMob(int nCntMob)
{

}

//==================================================================================
//���u�̂��낿��돈��
//==================================================================================
void UpdateCrawlMob(int nCntMob)
{
	int nMoveTime = 0;		//�ړ��̖ڕW����

							//���u�̈ړ��̃J�E���^�[�𑝂₷
	g_aMob[nCntMob].nMoveCounter++;

	//�ړ��̖ڕW���Ԃ����߂�
	nMoveTime = rand() % MAX_MOVE + MIN_MOVE;

	if ((g_aMob[nCntMob].nMoveCounter % nMoveTime) == 0)
	{//��莞�Ԍo������

	 //�ړ��̌����؂�ւ�
		g_aMob[nCntMob].aModel.rot.y += D3DX_PI;

		//�p�x�̐��K��
		g_aMob[nCntMob].aModel.rot.y = RotNormalize(g_aMob[nCntMob].aModel.rot.y);
	}

	//���u�̈ړ�
	g_aMob[nCntMob].aModel.pos.x = g_aMob[nCntMob].aModel.pos.x + sinf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * 1.0f;
	g_aMob[nCntMob].aModel.pos.z = g_aMob[nCntMob].aModel.pos.z + cosf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * 1.0f;
}

//==================================================================================
//���u�̃Z���t�ݒ菈��
//==================================================================================
void MobSerifSet(int nCntMob)
{
	if (g_nMobSerifTime >= 2000 && g_aMob[nCntMob].aModel.nState != MOBSTATE_APPIEAR && g_aMob[nCntMob].aModel.nIdxSerif == -1)
	{//��莞�Ԍo������

		switch (g_aMob[nCntMob].nTexType)
		{
		case 0:		//��Ј�

			//�Z���t�̐ݒ�
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_WORK, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		//case 1:		//�W���C�A���H

		//	//�Z���t�̐ݒ�
		//	g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_WORK, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
		//	break;

		case 2:			//�₭��

			//�Z���t�̐ݒ�
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_YAKUZA, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		case 3:			//��Ј�

			//�Z���t�̐ݒ�
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_WORK, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		case 4:			//�ڂ���

			//�Z���t�̐ݒ�
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_BOCCHI, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		case 5:			//�ڂ����n��

			//�Z���t�̐ݒ�
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_BOCCHI, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		}
		

		//�Z���t�̈ʒu�X�V
		SetPositionSerif(g_aMob[nCntMob].aModel.nIdxSerif, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });

		//���ԏ�����
		g_nMobSerifTime = 0;
	}
}

//==================================================================================
//���u�̃q�b�g����
//==================================================================================
void HitMob(int nCntMob, int nValue)
{
	//���擾
	Rotate *pRotate = GetRotate();
	Player *pPlayer = GetPlayer();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	//�Ö���Ԃ���Ȃ��Ƃ�
	if (g_aMob[nCntMob].aModel.nState != MOBSTATE_HYPNOSIS)
	{
		//�Ƃǂ܂点��
		g_aMob[nCntMob].aModel.nState = MOBSTATE_KEEP;
	}

	//�Ö��x�����J�E���^�[�𑝂₷
	g_aMob[nCntMob].nAddHypInter += nValue;

	//�Ö��x����
	if (g_aMob[nCntMob].nAddHypInter >= HYPADD_INTER)
	{//�Ö��x�����J�E���^�[���K��l�𒴂�����

		//�Ö��x�����J�E���^�[���Z�b�g
		g_aMob[nCntMob].nAddHypInter = 0;

		//�Ö��x�𑝂₷
		if (g_aMob[nCntMob].nHypnosis < 10)
		{//�Ö��x��10�����̎�

			//�Ö��x�㏸
			g_aMob[nCntMob].nHypnosis++;
		}
	}

	//���邮��ǉ�
	if (pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp == false && g_aMob[nCntMob].nHypnosis >= 10 && g_aSwitchSE.bUseRotate == false)
	{
		pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = true;

		//SE���Z�b�g
		PlaySound(SOUND_LABEL_SE_PIYOPIYO);

		g_aSwitchSE.bUseRotate = true;		//SE�g�p���Ă��Ԃɂ���
	}

	//�Ö��x���ő�
	if (g_aMob[nCntMob].nHypnosis >= 10)
	{
		//�Ö���Ԃɂ���
		g_aMob[nCntMob].aModel.nState = MOBSTATE_HYPNOSIS;
	    
		//�`���[�g���A���̎�����
		if (GetMode() == MODE_TUTORIAL)
		{
			//�Ö��������
			pTutorial->aControl.bHypnosis = true;
		}
	}
}

//==================================================================================
//�I�u�W�F�N�g�Ƃ̓����蔻��
//==================================================================================
void CollisionModelMob(int nCntMob)
{

	//���f���̏��擾
	ModelModel *pModel = GetModel();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true)
		{//���f�����g�p����Ă�����

			//�L�����N�^�[���m�̓����蔻��
			CollisionCharacter(
				&g_aMob[nCntMob].aModel.pos, &g_aMob[nCntMob].aModel.posOld, g_aMob[nCntMob].aModel.vtxMax, g_aMob[nCntMob].aModel.vtxMin,
				&pModel->pos, pModel->vtxMax, pModel->vtxMin);
		}
	}
}

//==================================================================================
//���u�̕`�揈��
//==================================================================================
void DrawMob(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL mMatRed;			//�_���[�W�}�e���A��

									//���̏��N���A
	ZeroMemory(&mMatRed, sizeof(D3DXMATERIAL));
	mMatRed.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	mMatRed.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		if (g_aMob[nCntMob].aModel.bUse == true)
		{//�g�p���Ă�����

		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMob[nCntMob].aModel.mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMob[nCntMob].aModel.rot.y, g_aMob[nCntMob].aModel.rot.x, g_aMob[nCntMob].aModel.rot.z);
			D3DXMatrixMultiply(&g_aMob[nCntMob].aModel.mtxWorld, &g_aMob[nCntMob].aModel.mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y, g_aMob[nCntMob].aModel.pos.z);
			D3DXMatrixMultiply(&g_aMob[nCntMob].aModel.mtxWorld, &g_aMob[nCntMob].aModel.mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMob[nCntMob].aModel.mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aMob[nCntMob].aModel.pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aMob[nCntMob].aModel.dwNumMat; nCntMat++)
			{
				switch (g_aMob[nCntMob].aModel.nState)
				{
				case MOBSTATE_APPIEAR:		//�o�����

					pDevice->SetMaterial(&g_aMob[nCntMob].aModel.pMatData[nCntMat].MatD3D);
					break;

				default:
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					break;
				}

				//�e�N�X�`���̐ݒ�
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					pDevice->SetTexture(0, g_apTextureMob[g_aMob[nCntMob].nTexType]);
				}
				else
				{
					pDevice->SetTexture(0, NULL);
				}

				

				//���u(�p�[�c)�̕`��
				g_aMob[nCntMob].aModel.pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//���u�̐ݒ菈��
//==================================================================================
void SetMob(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nSpawnType, int nModelType)
{
	//���f���̏��擾
	Model *pModel = GetXLoadData();
	Rotate *pRotate = GetRotate();
	Player *pPlayer = GetPlayer();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		if (g_aMob[nCntMob].aModel.bUse == false)
		{//�g�p���Ă��Ȃ�������

			//���f�����擾
			g_aMob[nCntMob].aModel = pModel[nModelType];

			g_aMob[nCntMob].nSpawnType = nSpawnType;	//�o�����錚���̎��
			g_aMob[nCntMob].aModel.nType = nModelType;	//���u�̎��
			g_aMob[nCntMob].nTexType = ((nModelType - MOB_NUMBER) * 2) + (rand() % 2);	//�e�N�X�`���^�C�v

			g_aMob[nCntMob].aModel.pos = pos;	//�ʒu
			g_aMob[nCntMob].aModel.rot = rot;	//����
			g_aMob[nCntMob].aModel.rot.y += D3DX_PI;
			g_aMob[nCntMob].aModel.nState = MOBSTATE_APPIEAR;	//�o����Ԃɂ���
			g_aMob[nCntMob].aModel.bUse = true;					//�g�p���Ă���󋵂ɂ���
			g_aMob[nCntMob].nHypnosis = 0;
			g_aMob[nCntMob].fRandAlpha = (float)(rand() % 51 + 13) * 0.0001f;		//�����x�̑�����

			if (g_aUseSpawn.bUseSpawn == true)
			{//��ʔ�����

				//�{
				g_aMob[nCntMob].fRandAlpha *= 3.0f;
			}

			//���f���̑S���_�`�F�b�N
			CheckVtx(g_aMob[nCntMob].aModel.rot.y, &g_aMob[nCntMob].aModel.vtxMax, &g_aMob[nCntMob].aModel.vtxMin, g_aMob[nCntMob].aModel.pMesh, g_aMob[nCntMob].aModel.pVtxBuff);

			//�Ö��x�Q�[�W�̐ݒ�
			g_aMob[nCntMob].aModel.nIdxHypGauge = SetHypnosisGauge(25.0f, 5.0f, 10);

			//�Ö��x�Q�[�W�̈ʒu�X�V
			SetPositionHypnosisGauge(g_aMob[nCntMob].aModel.nIdxHypGauge, g_aMob[nCntMob].aModel.pos, g_aMob[nCntMob].nHypnosis);

			//���邮���ݒ�
			g_aMob[nCntMob].aModel.nIdxRotate = SetRotate();

			//�e��ݒ�
			g_aMob[nCntMob].aModel.nIdxShadow = SetShadow(g_aMob[nCntMob].aModel.vtxMax.x * 1.5f, g_aMob[nCntMob].aModel.vtxMax.z * 1.5f);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aMob[nCntMob].aModel.nIdxShadow, g_aMob[nCntMob].aModel.pos);

			//�A�C�R���̐ݒ菈��
			g_aMob[nCntMob].nIdxIcon = SetIcon(MINIMAP_ICON_MOB, 40.0f);
			UpdatePositionIcon(g_aMob[nCntMob].nIdxIcon, g_aMob[nCntMob].aModel.pos);

			//���邮����g�p���Ă��Ȃ���Ԃɂ���
			pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = false;

			//�Ö��x�Q�[�W���g�p���Ă��Ȃ���Ԃɂ���
			pHypnosis[g_aMob[nCntMob].aModel.nIdxHypGauge].bDisp = false;

			break;
		}
	}
}

//==================================================================================
//���u�̃X�|�[���ʒu�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetMobPos(void)
{

	char aComment[MAX_COMMENT];	//�R�����g�p
	int nCntSpawn = 0;
	int nCntFileName = 0;

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

						//�t�@�C�����J��
	pFile = fopen("data\\TEXT\\mob_set.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

		 //������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//���f���̐ݒ�
			if (strcmp(aComment, "MOBSET") == 0)
			{//���f���̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_MOBSET"))
				{//END_ENEMYSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "MANY") == 0)
					{//MANY���������ޓǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aManySpawn.aSpawn[nCntSpawn].nMany);		//��ʔ����g�p�̔���
					}

					if (strcmp(aComment, "TYPE") == 0)
					{//TYPE���������ޓǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						g_aManySpawn.aSpawn[nCntSpawn].nType = nCntSpawn;
					}

					if (strcmp(aComment, "POS") == 0)
					{//POS��������ʒu�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].pos.x);	//X���W
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].pos.y);	//Y���W
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].pos.z);	//Z���W
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROT������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].rot.x);	//X�̌���
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].rot.y);	//Y�̌���
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].rot.z);	//Z�̌���

						g_aManySpawn.aSpawn[nCntSpawn].rot = D3DXToRadian(g_aManySpawn.aSpawn[nCntSpawn].rot);
					}

				}//END_MOBSET�̂�����

				 //�X�|�[���n�_�̔ԍ����Z
				nCntSpawn++;
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
//���u�̃`���[�g���A���X�V����
//==================================================================================
void UpdateTutorialMob(void)
{
	//���f���̏��擾
	ModelModel *pModel = GetModel();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();
	Rotate *pRotate = GetRotate();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	if (pTutorial->nStep >= TUTORIAL_STEP_USEFEVER)
	{
		if (pTutorial->aControl.bSetMobFever == false)
		{//�܂��t�B�[�o�[���̃��u��ݒu���Ă��Ȃ�������

			//�J�n���̃��u��ݒ�
			for (int nCntMob = 0; nCntMob < START_MOB; nCntMob++)
			{
				int nCntNumber = rand() % NUM_BILL;		//�����̎��

				//���u�̐ݒ�
				SetMob(g_aManySpawn.aSpawn[nCntNumber].pos, g_aManySpawn.aSpawn[nCntNumber].rot, g_aManySpawn.aSpawn[nCntNumber].nType, rand() % 3 + MOB_NUMBER);
			}

			//�ݒu��Ԃɂ���
			pTutorial->aControl.bSetMobFever = true;
		}

		//�X�|�[�����鎞�Ԃ𑝉�����
		g_nSpawnCounter++;
	}

	if (pTutorial->nStep >= TUTORIAL_STEP_HYPNOSIS && pTutorial->aControl.bSetMob == false)
	{//�Ö��̃X�e�b�v�ȏ� && ���u��5�l�ȉ�

		for (int nCntMob = 0; nCntMob < 5; nCntMob++)
		{
			int nCntNumber = rand() % NUM_BILL;		//�����̎��

			//���u�̐ݒ�
			SetMob(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, nCntMob * (D3DX_PI / 5.0f), 0.0f), g_aManySpawn.aSpawn[nCntNumber].nType, rand() % 3 + MOB_NUMBER);
		}

		//���u�z�u����
		pTutorial->aControl.bSetMob = true;
	}

	int nUse = 0;
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		if (g_aMob[nCntMob].aModel.bUse == true)
		{//�g�p���Ă�����

			//�g���Ă鐔���Z
			nUse++;

			//�ߋ��̈ʒu�ۑ�
			g_aMob[nCntMob].aModel.posOld = g_aMob[nCntMob].aModel.pos;

			//���u�̑���
			ControllMob(nCntMob);

			//���u���m�̓����蔻��
			CollisionModelMob(nCntMob);

			//���W����
			LimitPosMob(nCntMob);

			//��ԍX�V
			UpdateStateMob(nCntMob);

			//���邮��̈ʒu�X�V
			SetPositionRotate(g_aMob[nCntMob].aModel.nIdxRotate, g_aMob[nCntMob].aModel.pos);

			//�Ö��x�Q�[�W�̈ʒu�X�V
			SetPositionHypnosisGauge(g_aMob[nCntMob].aModel.nIdxHypGauge, g_aMob[nCntMob].aModel.pos, g_aMob[nCntMob].nHypnosis);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aMob[nCntMob].aModel.nIdxShadow, g_aMob[nCntMob].aModel.pos);

			//�A�C�R���̈ʒu�X�V
			UpdatePositionIcon(g_aMob[nCntMob].nIdxIcon, g_aMob[nCntMob].aModel.pos);

			if (g_aMob[nCntMob].aModel.nState != MOBSTATE_HYPNOSIS && g_aMob[nCntMob].aModel.nState != MOBSTATE_KEEP)
			{//�Ö���ԁA�Ö������Ă�Œ��ȊO�̎�

				//���邮��폜
				pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = false;
			}

		}
		else if (g_aMob[nCntMob].aModel.bUse == false)
		{//�g�p���Ă��Ȃ�������

			//���u�̔���
			UpdateSpawnMob(nCntMob);
		}
	}

	g_nNumMob = nUse;
}

//==================================================================================
//���u�̏��擾
//==================================================================================
Mob *GetMob(void)
{
	return &g_aMob[0];
}

//==================================================================================
//��ʔ����n�_�̏��擾
//==================================================================================
ManySpawn *GetManySpawn(void)
{
	return &g_aManySpawn;
}

//==================================================================================
//��ʔ����n�_�̏��擾
//==================================================================================
SwitchSpawn *GetSwitchSpawn(void)
{
	return &g_aUseSpawn;
}

//==================================================================================
//SE�̏��擾
//==================================================================================
SwitchSE *GetSwitchSE(void)
{
	return &g_aSwitchSE;
}