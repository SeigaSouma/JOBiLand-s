//=============================================================================
//
// �G���� [enemy.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "calculation.h"
#include "search_circle.h"
#include "crowd.h"
#include "shadow.h"
#include "minimap_icon.h"
#include "watergun.h"
#include "effect.h"
#include "particle.h"
#include "EventWindow.h"
#include "search_fan.h"
#include "tutorial.h"
#include "buddy.h"
#include "dispatch.h"
#include "sound.h"
#include "eventstamp.h"
#include "serif.h"
#include "sound.h"

//�}�N����`
#define MOVE			(0.4f)
#define CRAWL_MOVE		(0.1f)		//����̈ړ���
#define MAX_LENGTH		(100.0f)	//���̋���
#define ROT_MOVE		(0.03f)		//��]�ړ���
#define DMG_TIME		(20)		//�_���[�W��Ԃ̎���
#define SEARCH_INTERVAL	(90)
#define ATK_INTERVAL	(100)
#define LIFE			(2)
#define CHASE_RADIUS	(240.0f)
//#define SPWAN_TIME      (4500)    //�x��������Ԃɓ���܂ł̎���
#define FIRST_SPAWN		(60 * 85)	//1�Ԗڂ̌x������
#define SECOND_SPAWN	(60 * 185)	//2�Ԗڂ̌x������
//#define THIRD_SPAWN		(60 * 280)	//3�Ԗڂ̌x������
#define STRENG_TIME     (60 * 40)   //�x��������Ԃ̎���
//#define STRENG_TIME     (60 * 30)   //�x��������Ԃ̎���
#define STOP_MANYTIME		(60 * 35)	//��ʔ������̌x���I������
#define SOUNDCOOLTIME   (10)            //�T�E���h�̃N�[���^�C��
#define ENEMY_TXT		"data\\TEXT\\motion_set_enemy.txt"
#define ENEMYSET_TXT	"data\\TEXT\\model_set.txt"

//�v���g�^�C�v�錾
void XFileLoadEnemy(void);
void ReadSetMotionEnemy(void);

void ControllEnemy(int nCntEnemy);
void UpdateStateEnemy(int nCntEnemy);
void UpdateActionEnemy(int nCntEnemy);
void UpdateMotionEnemy(int nCntEnemy);
void LimitPosEnemy(int nCntEnemy);
void RotFacePlayerEnemy(int nCntEnemy);
void UpdateCrawlEnemy(int nCntEnemy);
void ATKEnemy(int nCntEnemy);
void CollisionModelEnemy(int nCntEnemy);
void ShotBulletEnemy(int nCntEnemy, D3DXVECTOR3 pos);
void MoreSpawnEnemy(void);
void EnemySerifSet(int nCntEnemy);
void UpdateGameEndEnemy(int nCntEnemy);
void ManyMoreSpawnEnemy(void);

//�O���[�o���ϐ��錾
Enemy g_aEnemy[MAX_ENEMY];				//�G�̏��
Model g_aXLoadMeshEnemy[MAX_MODEL];		//X�t�@�C���̃f�[�^
EnemyInfo g_aEnemyInfo;	//���
MoreEnemy g_MoreEnemy;  //�x������
int g_nCntWaterBullet = 0;
bool g_bManyMORS;
int g_nEnemySerifTime;		//�Z���t�̐ݒ莞��
int g_SoundCoolTime;        //���S�C��SE�̃N�[���^�C��

//==================================================================================
//�G�̏���������
//==================================================================================
void InitEnemy(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�v�f������
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//�G�̍\����
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aEnemy[nCntEnemy].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���݂̈ʒu
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

																		//���[���h�}�g���b�N�X
		D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);
		g_aEnemy[nCntEnemy].bUse = false;				//�g�p���Ă��邩
		g_aEnemy[nCntEnemy].bMoreSpawn = false;        //�x���������Ă��邩�ǂ���
		g_aEnemy[nCntEnemy].nState = 0;				//���
		g_aEnemy[nCntEnemy].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aEnemy[nCntEnemy].nIdxSearchRadius = -1;	//�T���͈͉~�̃C���f�b�N�X�ԍ�
		g_aEnemy[nCntEnemy].nIdxIcon = -1;			//�~�j�}�b�v�A�C�R���̃C���f�b�N�X�ԍ�
		g_aEnemy[nCntEnemy].nIdxSearchFan = -1;		//��̃C���f�b�N�X�ԍ�
		g_aEnemy[nCntEnemy].nIdxSerif = -1;			//�Z���t�̃C���f�b�N�X�ԍ�
		g_aEnemy[nCntEnemy].nCntState = 0;			//��ԃJ�E���^�[
		g_aEnemy[nCntEnemy].nCntAction = 0;			//�s���J�E���^�[
		g_aEnemy[nCntEnemy].nATKInterval = 0;		//�U���̊Ԋu
		g_aEnemy[nCntEnemy].nCntATK = 0;			//�U���̊Ԋu�J�E���g
		g_aEnemy[nCntEnemy].nCntSearchCircle = 0;	//�T���͈͉~�̃J�E���g
		g_aEnemy[nCntEnemy].fChaseRadius = 0.0f;		//�ǂ��|���͈�
		g_aEnemy[nCntEnemy].fSearchRadius = 0.0f;	//�T���͈�
		g_aEnemy[nCntEnemy].fSearchLength = 0.0f;	//�T���̒���
		g_aEnemy[nCntEnemy].fSearchRot = 0.0f;		//�T���̌���
		g_aEnemy[nCntEnemy].fRadius = 0.0f;			//�T���̌���

		g_aEnemy[nCntEnemy].nNumPosCrawl = 0;		//���񂷂�n�_�̔ԍ�
		g_aEnemy[nCntEnemy].nNumMaxCrawl = 0;		//���񂷂�n�_�̍ő吔
		g_aEnemy[nCntEnemy].MassGeneration = 0;     //��ʔ������ɏo��G
		g_aEnemy[nCntEnemy].nCntWait = 0;           //�o���҂��̃J�E���g

													//����|�C���g
		for (int nCntCrawl = 0; nCntCrawl < MAX_POSCRAWL; nCntCrawl++)
		{
			g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		//���[�V�����n
		g_aEnemy[nCntEnemy].nNowMotionNum = 0;		//���݂̃��[�V�����ԍ�
		g_aEnemy[nCntEnemy].nCntFrame = 0;			//�t���[���̃J�E���g
		g_aEnemy[nCntEnemy].nPatternKey = 0;		//���ڂ̃L�[��
		g_aEnemy[nCntEnemy].nPartsNum = 0;			//�p�[�c��
		g_aEnemy[nCntEnemy].nCntAllFrame = 0;		//���t���[���J�E���g

													//���f���\���̂̏�����
		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			g_aEnemy[nCntEnemy].aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
			g_aEnemy[nCntEnemy].aModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
			g_aEnemy[nCntEnemy].aModel[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
			g_aEnemy[nCntEnemy].aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
			g_aEnemy[nCntEnemy].aModel[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
			g_aEnemy[nCntEnemy].aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

																							//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorld);
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntModel].OldmtxWorld);

			g_aEnemy[nCntEnemy].aModel[nCntModel].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
			g_aEnemy[nCntEnemy].aModel[nCntModel].nIdxRotate = -1;	//���邮��̃C���f�b�N�X�ԍ�
			g_aEnemy[nCntEnemy].aModel[nCntModel].nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
			g_aEnemy[nCntEnemy].aModel[nCntModel].nShadow = 1;			//�e���g�����ǂ���

			g_aEnemy[nCntEnemy].aModel[nCntModel].nNumVtx = 0;			//���_��
			g_aEnemy[nCntEnemy].aModel[nCntModel].dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
			g_aEnemy[nCntEnemy].aModel[nCntModel].pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
			g_aEnemy[nCntEnemy].aModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
			g_aEnemy[nCntEnemy].aModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

			g_aEnemy[nCntEnemy].aModel[nCntModel].nType = 0;			//�ǂݍ��ރ��f���̎��
			g_aEnemy[nCntEnemy].aModel[nCntModel].nParent = -1;		//�e�̔ԍ�
			g_aEnemy[nCntEnemy].aModel[nCntModel].bUse = false;			//�g�p���Ă��邩
			g_aEnemy[nCntEnemy].aModel[nCntModel].nState = 0;			//���

			for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
			{
				g_aEnemy[nCntEnemy].aModel[nCntModel].pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
				g_aEnemy[nCntEnemy].aModel[nCntModel].pMatData[nCntMat].pTextureFilename = NULL;	//�}�e���A���̃f�[�^
				g_aEnemy[nCntEnemy].aModel[nCntModel].acFilename[nCntMat] = NULL;	//�t�@�C����
			}

			for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
			{
				g_aEnemy[nCntEnemy].aModel[nCntModel].pTexture[nCntTex] = NULL;	//�e�N�X�`���ւ̃|�C���^
			}

			g_aEnemy[nCntEnemy].aModel[nCntModel].pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
			g_aEnemy[nCntEnemy].aModel[nCntModel].pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^
			g_aEnemy[nCntEnemy].aModel[nCntModel].dwNumMat = NULL;			//�}�e���A���̐�
		}
	}

	//���f���f�[�^
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aXLoadMeshEnemy[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aXLoadMeshEnemy[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aXLoadMeshEnemy[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
		g_aXLoadMeshEnemy[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aXLoadMeshEnemy[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
		g_aXLoadMeshEnemy[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

																				//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aXLoadMeshEnemy[nCntModel].mtxWorld);
		D3DXMatrixIdentity(&g_aXLoadMeshEnemy[nCntModel].OldmtxWorld);
		g_aXLoadMeshEnemy[nCntModel].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aXLoadMeshEnemy[nCntModel].nIdxRotate = -1;	//���邮��̃C���f�b�N�X�ԍ�
		g_aXLoadMeshEnemy[nCntModel].nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
		g_aXLoadMeshEnemy[nCntModel].nShadow = 1;			//�e���g�����ǂ���

		g_aXLoadMeshEnemy[nCntModel].nNumVtx = 0;			//���_��
		g_aXLoadMeshEnemy[nCntModel].dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aXLoadMeshEnemy[nCntModel].pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aXLoadMeshEnemy[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
		g_aXLoadMeshEnemy[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

		g_aXLoadMeshEnemy[nCntModel].nType = 0;			//�ǂݍ��ރ��f���̎��
		g_aXLoadMeshEnemy[nCntModel].nParent = -1;		//�e�̔ԍ�
		g_aXLoadMeshEnemy[nCntModel].bUse = false;			//�g�p���Ă��邩
		g_aXLoadMeshEnemy[nCntModel].nState = 0;			//���

		g_aXLoadMeshEnemy[nCntModel].pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
		g_aXLoadMeshEnemy[nCntModel].pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^
		g_aXLoadMeshEnemy[nCntModel].dwNumMat = NULL;		//�}�e���A���̐�

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aXLoadMeshEnemy[nCntModel].pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
			g_aXLoadMeshEnemy[nCntModel].pMatData[nCntMat].pTextureFilename = NULL;	//�}�e���A���̃f�[�^
			g_aXLoadMeshEnemy[nCntModel].acFilename[nCntMat] = NULL;	//�t�@�C����
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aXLoadMeshEnemy[nCntModel].pTexture[nCntTex] = NULL;	//�e�N�X�`���ւ̃|�C���^
		}
	}

	//�G�̏��
	g_aEnemyInfo.nNumEnemy = 0;			//�G�̐�

	//�x���������
	g_MoreEnemy;

	g_MoreEnemy.MoreSecurityCounter = 0;		//�x����������܂ł̎���
	g_MoreEnemy.SpawnCounter = STRENG_TIME;     //�x�������I���܂ł̎���
	g_MoreEnemy.ManySpawnCounter = STOP_MANYTIME;		//��ʔ������̌x�������I������
	g_MoreEnemy.bMoreSpawn = false;             //�x�����������ǂ���
	g_MoreEnemy.bManyMoreSpawn = false;		//��ʔ������Ɍx�����������ǂ���
	g_MoreEnemy.bFirst = false;				//1�Ԗ�
	g_MoreEnemy.bSecond = false;			//2�Ԗ�
	g_MoreEnemy.bManyFirst = false;			//1�Ԗڂ̑�ʔ������̌x������
	g_MoreEnemy.bManySecond = false;		//2�Ԗڂ̑�ʔ������̌x������
	g_MoreEnemy.bManyThird = false;			//3�Ԗڂ̑�ʔ������̌x������

	g_nCntWaterBullet = 0;   //�e�𔭎˂���J�E���g
	g_nEnemySerifTime = 0;	//�G�̃Z���t�ݒ莞��

#if _DEBUG

	g_bManyMORS = false;	//�x���������邩�ǂ���

#else NDEBUG

	g_bManyMORS = true;	//�x���������邩�ǂ���

#endif

	//�O���t�@�C���ǂݍ���
	ReadSetMotionEnemy();

	if (GetMode() == MODE_TUTORIAL)
	{
		ReadSetEnemy();
	}

}

//==================================================================================
//�G�̏I������
//==================================================================================
void UninitEnemy(void)
{
	//SE�I��
	StopSound();

	for (int nCntParts = 0; nCntParts < MAX_MODEL; nCntParts++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aXLoadMeshEnemy[nCntParts].pTexture[nCntTex] != NULL)
			{
				g_aXLoadMeshEnemy[nCntParts].pTexture[nCntTex]->Release();
				g_aXLoadMeshEnemy[nCntParts].pTexture[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_aXLoadMeshEnemy[nCntParts].pMesh != NULL)
		{
			g_aXLoadMeshEnemy[nCntParts].pMesh->Release();
			g_aXLoadMeshEnemy[nCntParts].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aXLoadMeshEnemy[nCntParts].pBuffMat != NULL)
		{
			g_aXLoadMeshEnemy[nCntParts].pBuffMat->Release();
			g_aXLoadMeshEnemy[nCntParts].pBuffMat = NULL;
		}
	}
}

//==================================================================================
//�G�̍X�V����
//==================================================================================
void UpdateEnemy(void)
{
	//�v���C���[�̏��擾
	ModelModel *pModel = GetModel();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F9) == true)
	{//F9�������ꂽ

	 //��ʔ����؂�ւ�
		g_bManyMORS = g_bManyMORS ? false : true;
	}
#endif

	if (GetGameState() == GAMESTATE_NONE)
	{
		if (/*g_MoreEnemy.bMoreSpawn == false && */g_bManyMORS == true)
		{//�g�p���Ă��Ȃ�������

			g_MoreEnemy.MoreSecurityCounter++;      //�x����������܂ł̎��Ԃ����Z
		}
	}

	if (g_MoreEnemy.bMoreSpawn == true)
	{//�g�p���Ă�����

		g_MoreEnemy.SpawnCounter--;
	}

	if (g_MoreEnemy.bManyMoreSpawn == true)
	{//��ʔ������Ɍx���������Ă���
		g_MoreEnemy.ManySpawnCounter--;		//��ʔ������̌x�������I������
	}

	//�x������
	MoreSpawnEnemy();

	//��ʔ������̌x������
	ManyMoreSpawnEnemy();

	g_SoundCoolTime--;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bMoreSpawn == true)
		{//�g�p���Ă�����

			//�ߋ��̈ʒu�ۑ�
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			if (g_aEnemy[nCntEnemy].nCntSearchCircle >= SEARCH_INTERVAL - 1 && GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_END)
			{//�K��l��������

				g_aEnemy[nCntEnemy].nIdxSearchRadius = SetSearchCircle(
					D3DXVECTOR3(
						g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 10.0f, g_aEnemy[nCntEnemy].pos.z),
					g_aEnemy[nCntEnemy].rot,
					D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.5f),
					10.0f, 0.0f,
					(int)(SEARCH_RADIUS * 0.5f), 2.0f,
					SEARCH_CIRCLE_TYPE_ORANGE2);
			}

			//�T���͈͉~�̃J�E���g���Z
			g_aEnemy[nCntEnemy].nCntSearchCircle = (g_aEnemy[nCntEnemy].nCntSearchCircle + 1) % SEARCH_INTERVAL;

			if (GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_AFTEREND && GetGameState() != GAMESTATE_END)
			{//�I���O����Ȃ�������

				if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT || g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_WAIT || g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_LOST)
				{//���[�V�������ˌ��A�G�̏�Ԃ��o���҂��܂��́A���ő҂��ȊO�̎�

					//�G�̑���
					ControllEnemy(nCntEnemy);
				}

				//�G���m�̓����蔻��
				CollisionModelEnemy(nCntEnemy);
			}
			else if(GetGameState() == GAMESTATE_BEFOREEND || GetGameState() == GAMESTATE_END)
			{
				//�I���O����
				UpdateGameEndEnemy(nCntEnemy);
			}

			//���W����
			LimitPosEnemy(nCntEnemy);

			if (GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_AFTEREND && GetGameState() != GAMESTATE_END)
			{//�I���O����Ȃ�������

				//��ԍX�V
				UpdateStateEnemy(nCntEnemy);
			}

			//�s���X�V����
			UpdateActionEnemy(nCntEnemy);

			//���[�V�����X�V����
			UpdateMotionEnemy(nCntEnemy);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);

			//�T���~�̈ʒu�X�V
			SetPositionSearchCircle(g_aEnemy[nCntEnemy].nIdxSearchRadius, g_aEnemy[nCntEnemy].pos);

			//�A�C�R���̈ʒu�X�V
			UpdatePositionIcon(g_aEnemy[nCntEnemy].nIdxIcon, g_aEnemy[nCntEnemy].pos);

			//��̌`�̈ʒu�X�V
			SetPositionSearchFan(g_aEnemy[nCntEnemy].nIdxSearchFan, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);

			//�G�̃Z���t�ݒ菈��
			EnemySerifSet(nCntEnemy);

			//�Z���t�̈ʒu�X�V
			SetPositionSerif(g_aEnemy[nCntEnemy].nIdxSerif, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 150.0f, g_aEnemy[nCntEnemy].pos.z });
		}
	}
}

//==================================================================================
//�G�̑���
//==================================================================================
void ControllEnemy(int nCntEnemy)
{

	//�d�͏���
	g_aEnemy[nCntEnemy].move.y -= GRAVITY;

	//�ʒu�X�V
	g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
	g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * 0.1f;
	g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z) * 0.1f;
}

//==================================================================================
//�G�̍U������
//==================================================================================
void ATKEnemy(int nCntEnemy)
{

	//�v���C���[�̏��擾
	//Player *pPlayer = GetPlayer();

	//�U���Ԋu�J�E���g���Z
	g_aEnemy[nCntEnemy].nCntATK++;

	if (g_aEnemy[nCntEnemy].nCntATK >= g_aEnemy[nCntEnemy].nATKInterval && g_aEnemy[nCntEnemy].nCntATK <= g_aEnemy[nCntEnemy].nATKInterval + 5)
	{
		if (g_aEnemy[nCntEnemy].nCntATK >= g_aEnemy[nCntEnemy].nATKInterval + 5)
		{
			//�J�E���g���Z�b�g
			g_aEnemy[nCntEnemy].nCntATK = 0;
		}

		g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 3.0f;
		g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 3.0f;

		//�G�ƃv���C���[�̋���
		//float fLength = pPlayer->fRadius + g_aEnemy[nCntEnemy].aModel.vtxMax.x;

		//if (SphereRange(g_aEnemy[nCntEnemy].pos, pPlayer->pos) <= (fLength * fLength))
		//{//���̒��ɓ�������

		//	//�v���C���[�̃q�b�g����
		//	HitPlayer(D3DXVECTOR3(sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 15.0f, 10.0f, cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 15.0f), 1);
		//}
	}
}

//==================================================================================
//�v���C���[�̕������鏈��
//==================================================================================
void RotFacePlayerEnemy(int nCntEnemy)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�G�ƃv���C���[�̋���
	float fLength = pPlayer->fRadius + g_aEnemy[nCntEnemy].fChaseRadius;

	float fRotDest = g_aEnemy[nCntEnemy].rot.y;	//�ڕW�̊p�x
	float fRotDiff = 0.0f;	//���݂ƖڕW�̍���

							//�ڕW�̊p�x�����߂�
	fRotDest = atan2f((g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x), (g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z));
	fRotDiff = fRotDest - g_aEnemy[nCntEnemy].rot.y;

	//�p�x�̐��K��
	RotNormalize(&fRotDiff);

	//�p�x�̕␳������
	g_aEnemy[nCntEnemy].rot.y += fRotDiff * 0.15f;

	//�p�x�̐��K��
	RotNormalize(&g_aEnemy[nCntEnemy].rot.y);

	//�����Ă�����Ƀ_�b�V��
	g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * MOVE;
	g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * MOVE;
}

//==================================================================================
//�G�̏��񏈗�
//==================================================================================
void UpdateCrawlEnemy(int nCntEnemy)
{

	float fRotDest = g_aEnemy[nCntEnemy].rot.y;	//�ڕW�̊p�x
	float fRotDiff = 0.0f;	//���݂ƖڕW�̍���

							//�ڕW�̊p�x�����߂�
	fRotDest = atan2f(
		(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].PosCrawl[g_aEnemy[nCntEnemy].nNumPosCrawl].x),
		(g_aEnemy[nCntEnemy].pos.z - g_aEnemy[nCntEnemy].PosCrawl[g_aEnemy[nCntEnemy].nNumPosCrawl].z));

	//�ڕW�Ƃ̍���
	fRotDiff = fRotDest - g_aEnemy[nCntEnemy].rot.y;

	//�p�x�̐��K��
	RotNormalize(&fRotDiff);

	//�p�x�̕␳������
	g_aEnemy[nCntEnemy].rot.y += fRotDiff * 0.2f;

	//�p�x�̐��K��
	RotNormalize(&g_aEnemy[nCntEnemy].rot.y);

	if (CircleRange(g_aEnemy[nCntEnemy].PosCrawl[g_aEnemy[nCntEnemy].nNumPosCrawl], g_aEnemy[nCntEnemy].pos) >= (60.0f * 60.0f))
	{//�܂��K��l�ɒ����Ă��Ȃ�������

	 //�����Ă�����Ƀ_�b�V��
		g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * CRAWL_MOVE;
		g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * CRAWL_MOVE;
	}
	else
	{//�K��l�ɒ�������

	 //����n�_�X�V
		g_aEnemy[nCntEnemy].nNumPosCrawl = (g_aEnemy[nCntEnemy].nNumPosCrawl + 1) % g_aEnemy[nCntEnemy].nNumMaxCrawl;
	}
}

//==================================================================================
//�G�̍��W����
//==================================================================================
void LimitPosEnemy(int nCntEnemy)
{

	if (g_aEnemy[nCntEnemy].pos.y < 0.0f)
	{//�n�ʂ�艺�ɍs������

		g_aEnemy[nCntEnemy].pos.y = 0.0f;

		//�d�͏���
		g_aEnemy[nCntEnemy].move.y = 0.0f;
	}

	//�ړ�����
	if (g_aEnemy[nCntEnemy].pos.x + -g_aEnemy[nCntEnemy].fRadius < -LIMIT_POS) { g_aEnemy[nCntEnemy].pos.x = -LIMIT_POS - -g_aEnemy[nCntEnemy].fRadius; }
	if (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fRadius > LIMIT_POS) { g_aEnemy[nCntEnemy].pos.x = LIMIT_POS - g_aEnemy[nCntEnemy].fRadius; }
	if (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].fRadius > LIMIT_POS) { g_aEnemy[nCntEnemy].pos.z = LIMIT_POS - g_aEnemy[nCntEnemy].fRadius; }
	if (g_aEnemy[nCntEnemy].pos.z + -g_aEnemy[nCntEnemy].fRadius < -LIMIT_POS) { g_aEnemy[nCntEnemy].pos.z = -LIMIT_POS - -g_aEnemy[nCntEnemy].fRadius; }

}

//==================================================================================
//�e�̃q�b�g����
//==================================================================================
void HitEnemy(int nCntEnemy, D3DXVECTOR3 move, int nDMG)
{


}

//==================================================================================
//�G�̏�ԍX�V����
//==================================================================================
void UpdateStateEnemy(int nCntEnemy)
{

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//��̓����蔻��̏����擾
	SearchFan *pSearchFan = GetSearchFan();

	//�C�x���g�X�^���v�̏��擾
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//�G�ƃv���C���[�̋���
	float fLength = pPlayer->fRadius + g_aEnemy[nCntEnemy].fChaseRadius;

	switch (g_aEnemy[nCntEnemy].nState)
	{
	case ENEMYSTATE_NONE:

		if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT)
		{//���[�V�������ˌ��ȊO�̎�

			UpdateCrawlEnemy(nCntEnemy);
		}

		bool bRange;

		//�T���͈͂̔���
		bRange = CollisionFan(g_aEnemy[nCntEnemy].pos,
			D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + sinf(D3DX_PI * 0.25f + g_aEnemy[nCntEnemy].rot.y) * -g_aEnemy[nCntEnemy].fChaseRadius, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z + cosf(D3DX_PI * 0.25f + g_aEnemy[nCntEnemy].rot.y) * -g_aEnemy[nCntEnemy].fChaseRadius),
			D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + sinf(-D3DX_PI * 0.25f + g_aEnemy[nCntEnemy].rot.y) * -g_aEnemy[nCntEnemy].fChaseRadius, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z + cosf(-D3DX_PI * 0.25f + g_aEnemy[nCntEnemy].rot.y) * -g_aEnemy[nCntEnemy].fChaseRadius),
			pPlayer->pos, pPlayer->posOld);

		if (bRange == true)
		{//��̒��ɓ�������

			g_aEnemy[nCntEnemy].nState = ENEMYSTATE_CHASE;

			//���[�V�����̐ݒ�
			SetMotionEnemy(nCntEnemy, ENEMYMOTION_DASH);

			//SE�Z�b�g
			PlaySound(SOUND_LABEL_SE_GET);

			if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
			{
				//�Z���t�̐ݒ�
				g_aEnemy[nCntEnemy].nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_ENEMYDISC, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 150.0f, g_aEnemy[nCntEnemy].pos.z });
			}

			//�Z���t�̈ʒu�X�V
			SetPositionSerif(g_aEnemy[nCntEnemy].nIdxSerif, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 150.0f, g_aEnemy[nCntEnemy].pos.z });
		}

		break;

	case ENEMYSTATE_CHASE:

		if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT)
		{//���[�V�������ˌ��ȊO�̎�

		 //�v���C���[�̕������鏈��
			RotFacePlayerEnemy(nCntEnemy);

			if (CircleRange(g_aEnemy[nCntEnemy].pos, pPlayer->pos) >= (fLength * fLength))
			{//�T���T�[�N���̒��ɓ�������

				float MaxLength = 9999999.9f;	//�����̍ő�l
				int nIdxCrawl = g_aEnemy[nCntEnemy].nNumPosCrawl;	//���݂̏���ʒu�ԍ�

				for (int nCntCrawl = 0; nCntCrawl < g_aEnemy[nCntEnemy].nNumMaxCrawl; nCntCrawl++)
				{
					float fNowLength =
						(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].x) * (g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].x)
						+ (g_aEnemy[nCntEnemy].pos.z - g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].z) * (g_aEnemy[nCntEnemy].pos.z - g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].z);

					//�ő�l������̒������傫��������
					if (MaxLength >= fNowLength)
					{
						MaxLength = fNowLength;	//�ő�l����ւ�
						nIdxCrawl = nCntCrawl;	//�ԍ�����ւ�
					}
				}

				//��ԋ߂��n�_�̔ԍ��ɂ���
				g_aEnemy[nCntEnemy].nNumPosCrawl = nIdxCrawl;

				g_aEnemy[nCntEnemy].nState = ENEMYSTATE_NONE;

				//���[�V�����̐ݒ�
				SetMotionEnemy(nCntEnemy, ENEMYMOTION_DEF);
			}
		}
		break;

	case ENEMYSTATE_WAIT:

		if (g_aEnemy[nCntEnemy].nCntAllFrame < 150)
		{
			SetParticle(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 50.0f, g_aEnemy[nCntEnemy].pos.z), PARTICLE_TYPE_MORSSPAWN);
		}

		if (g_aEnemy[nCntEnemy].nCntAllFrame == 230)
		{//�t���[������30�ɂȂ�����

			//�G��T����Ԃɂ���
			g_aEnemy[nCntEnemy].nState = ENEMYSTATE_NONE;

			//���[�V�����̐ݒ�
			SetMotionEnemy(nCntEnemy, ENEMYMOTION_DEF);

			//��̓����蔻��̕`����o��
			pSearchFan[g_aEnemy[nCntEnemy].nIdxSearchFan].bDisp = true;
		}
		break;

	case ENEMYSTATE_LOST:

		//�e�̏��擾
		SHADOW *pShadow = GetShadow();

		//�}�b�v�A�C�R���̏��擾
		MINIICON *pMiniIcon = GetMiniIcon();

		if (g_aEnemy[nCntEnemy].nCntAllFrame < 60)
		{
			SetParticle(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 50.0f, g_aEnemy[nCntEnemy].pos.z), PARTICLE_TYPE_MORSSPAWN);
		}

		if (g_aEnemy[nCntEnemy].nCntAllFrame == 90)
		{//�t���[������60�ɂȂ�����

		 //�o�����Ă��Ȃ���Ԃɂ���
			g_aEnemy[nCntEnemy].bMoreSpawn = false;

			//�e���g�p���Ă��Ȃ���Ԃɂ���
			pShadow[g_aEnemy[nCntEnemy].nIdxShadow].bUse = false;

			//��̓����蔻��̕`�������
			pSearchFan[g_aEnemy[nCntEnemy].nIdxSearchFan].bDisp = false;

			////��̌`�̈ʒu�X�V
			//SetPositionSearchFan(g_aEnemy[nCount].nIdxSearchFan, g_aEnemy[nCount].pos, g_aEnemy[nCount].rot);

			//�}�b�v�A�C�R�����g�p���Ă��Ȃ���Ԃɂ���
			pMiniIcon[g_aEnemy[nCntEnemy].nIdxIcon].bUse = false;

			//�x�������̎��Ԃ�߂�
			if (g_MoreEnemy.SpawnCounter <= 0)
			{
				//�x��������Ԃ�����
				g_MoreEnemy.bMoreSpawn = false;
				g_MoreEnemy.SpawnCounter = STRENG_TIME;
			}
			if (g_MoreEnemy.ManySpawnCounter <= 0)
			{
				//�x��������Ԃ�����
				g_MoreEnemy.bManyMoreSpawn = false;
				g_MoreEnemy.ManySpawnCounter = STOP_MANYTIME;		//��ʔ������̌x�������I������
			}

			//�C�x���g�X�^���v����
			pEventStamp[EVENTTYPE_MORS].bUse = false;

			//�x����������܂ł̎��Ԃ�0�ɂ���
			//g_MoreEnemy.MoreSecurityCounter = 0;
		}
		break;
	}
}

//==================================================================================
//�G�̍s���X�V����
//==================================================================================
void UpdateActionEnemy(int nCntEnemy)
{

}

//==================================================================================
//�G�̃Z���t�ݒ菈��
//==================================================================================
void EnemySerifSet(int nCntEnemy)
{
	if (g_nEnemySerifTime >= 432 && GetMode() == MODE_GAME || g_nEnemySerifTime >= 432 && GetMode() == MODE_TUTORIAL)
	{//��莞�Ԍo������

		 //�Z���t�̐ݒ�
		g_aEnemy[nCntEnemy].nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_ENEMYNONE, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 110.0f, g_aEnemy[nCntEnemy].pos.z });

		//�Z���t�̈ʒu�X�V
		SetPositionSerif(g_aEnemy[nCntEnemy].nIdxSerif, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 110.0f, g_aEnemy[nCntEnemy].pos.z });

		//���ԏ�����
		g_nEnemySerifTime = 0;
	}
	else
	{
		//�Z���t�ݒ莞�ԉ��Z
		g_nEnemySerifTime++;
	}
}

//==================================================================================
//�G�Ƃ̓����蔻��
//==================================================================================
void CollisionModelEnemy(int nCntEnemy)
{
	//�G�̏��擾
	Enemy *pEnemy = GetEnemy();

	if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_WAIT)
	{
		for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{//�G���g�p����Ă�����

			 //�L�����N�^�[���m�̓����蔻��
				CollisionCharacter(
					&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, D3DXVECTOR3(g_aEnemy[nCntEnemy].fRadius, 0.0f, g_aEnemy[nCntEnemy].fRadius), D3DXVECTOR3(-g_aEnemy[nCntEnemy].fRadius, 0.0f, -g_aEnemy[nCntEnemy].fRadius),
					&pEnemy->pos, D3DXVECTOR3(pEnemy->fRadius, 0.0f, pEnemy->fRadius), D3DXVECTOR3(-pEnemy->fRadius, 0.0f, -pEnemy->fRadius));
			}
		}

		float nnn = D3DX_PI * 0.5f;

		//���f���̏��擾
		ModelModel *pModel = GetModel();

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
		{
			if (pModel->bUse == true && pModel->nType < SMALL_NUM)
			{//���f�����g�p����Ă�����

			 //�L�����N�^�[���m�̓����蔻��
				CollisionCharacter(
					&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, D3DXVECTOR3(g_aEnemy[nCntEnemy].fRadius, 0.0f, g_aEnemy[nCntEnemy].fRadius), D3DXVECTOR3(-g_aEnemy[nCntEnemy].fRadius, 0.0f, -g_aEnemy[nCntEnemy].fRadius),
					&pModel->pos, pModel->vtxMax, pModel->vtxMin);
			}
		}

		//�Q�O�̏��擾
		Crowd *pCrowd = GetCrowd();

		if (g_aEnemy[nCntEnemy].nState != ENEMYSTATE_LOST)
		{
			int nnnn = 0;
			for (int nCount = 0; nCount < MAX_CROWD; nCount++, pCrowd++)
			{
				if (pCrowd->aModel.bUse == true)
				{//�Q�O���g�p����Ă�����

					//�G�ƌQ�O�̋���
					float fLength = pCrowd->aModel.vtxMax.x + SEARCH_RADIUS;

					if (SphereRange(g_aEnemy[nCntEnemy].pos, pCrowd->aModel.pos) <= (fLength * fLength))
					{//���̒��ɓ�������

						nnnn++;

						if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT)
						{
							//���[�V�����̐ݒ�
							SetMotionEnemy(nCntEnemy, ENEMYMOTION_SHOT);
						}

						if (pCrowd->nState != CROWDSTATE_WAIT && pCrowd->nState != CROWDSTATE_FADE)
						{
							//�ˌ��҂���Ԃɂ���
							pCrowd->nState = CROWDSTATE_WAIT;
							pCrowd->nCntChase = 0;
						}

						if (pCrowd->nState == CROWDSTATE_WAIT)
						{
							//�ˌ��G�t�F�N�g
							ShotBulletEnemy(nCntEnemy, pCrowd->aModel.pos);
						}
					}
				}
			}

			if (nnnn == 0 && g_aEnemy[nCntEnemy].nNowMotionNum == ENEMYMOTION_SHOT)
			{//���̒��ɌQ�O�������ĂȂ��ēG�̃��[�V�������ˌ����[�V��������������

				g_aEnemy[nCntEnemy].nCntState = ENEMYSTATE_NONE;

				//���[�V�����̐ݒ�
				SetMotionEnemy(nCntEnemy, ENEMYMOTION_DEF);
			}
		}
	}
}

//==================================================================================
//���S�C���ˏ���
//==================================================================================
void ShotBulletEnemy(int nCntEnemy, D3DXVECTOR3 pos)
{
	//�Q�O�̏����擾
	Crowd *pCrowd = GetCrowd();

	if (g_aEnemy[nCntEnemy].nNowMotionNum == ENEMYMOTION_SHOT)
	{
		float fRotDest = g_aEnemy[nCntEnemy].rot.y;	//�ڕW�̊p�x
		float fRotDiff = 0.0f;	//���݂ƖڕW�̍���

								//�ڕW�̊p�x�����߂�
		fRotDest = atan2f(
			(g_aEnemy[nCntEnemy].pos.x - pos.x),
			(g_aEnemy[nCntEnemy].pos.z - pos.z));

		//�ڕW�Ƃ̍���
		fRotDiff = fRotDest - g_aEnemy[nCntEnemy].rot.y;

		//�p�x�̐��K��
		RotNormalize(&fRotDiff);

		//�p�x�̕␳������
		g_aEnemy[nCntEnemy].rot.y += fRotDiff * 1.0f;

		//�p�x�̐��K��
		RotNormalize(&g_aEnemy[nCntEnemy].rot.y);

		if (g_aEnemy[nCntEnemy].nCntAllFrame == 15)
		{//�J�E���g��15�ɒB������

			float fMove, fMoveY, ffffff;

			for (int nCntUse = 0; nCntUse < 40; nCntUse++)
			{
				fMove = (float)(rand() % 50) / 10 + 1.0f;		//�ړ���
				fMoveY = (float)(rand() % 11 - 5) / 10;	//�ړ���

				D3DXVECTOR3 move;

				//�ړ��ʂ̐ݒ�
				move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
				move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
				ffffff = (float)(rand() % 51 - 25) / 100.0f;

				float fRadius = 10.0f;

				if (rand() % 3 == 0)
				{
					fRadius *= 0.95f;
				}
				else if (rand() % 3 == 1)
				{
					fRadius *= 0.98f;
				}
				else if (rand() % 3 == 2)
				{
					fRadius *= 0.92f;
				}

				//�G�t�F�N�g�̐ݒ�
				SetWatergun({ g_aEnemy[nCntEnemy].pos.x + sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 0.0f, g_aEnemy[nCntEnemy].pos.y + 75.0f, g_aEnemy[nCntEnemy].pos.z + cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 35.0f },
				{ 0.5f , 0.5f , 1.0f , 1.0f },
					fRadius,
					30,
					D3DXVECTOR3(sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y + ffffff) * fMove, fMoveY, cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y + ffffff) * fMove),
					MOVEEFFECT_SUB, WATERBULLETTYPE_BLACK);
			}

			if (GetMode() != MODE_RESULT && g_SoundCoolTime <= 0)
			{//�T�E���h�̃N�[���^�C�����I����Ă�����

				//SE���Z�b�g
				PlaySound(SOUND_LABEL_SE_SHIBUKI);

				//�N�[���^�C���ݒ�
				g_SoundCoolTime = SOUNDCOOLTIME;
			}
		}

	}
}

//==================================================================================
//�G�̌x����������
//==================================================================================
void MoreSpawnEnemy(void)
{
	//��̓����蔻��̏����擾
	SearchFan *pSearchFan = GetSearchFan();

	int nMoreCount = 0;		//�E�B���h�E�\������
	int nSpawnTime = 0;		//�X�|�[������

							//���Ԑݒ�
	if (g_MoreEnemy.bFirst == false)
	{//1�Ԗ�
		nMoreCount = FIRST_SPAWN - 250;		//�E�B���h�E����
		nSpawnTime = FIRST_SPAWN;			//�X�|�[������
	}
	else if (g_MoreEnemy.bSecond == false)
	{//2�Ԗ�
		nMoreCount = SECOND_SPAWN - 250;	//�E�B���h�E����
		nSpawnTime = SECOND_SPAWN;			//�X�|�[������
	}
	//else if (g_MoreEnemy.bThird == false)
	//{//3�Ԗ�
	//	nMoreCount = THIRD_SPAWN - 250;	//�E�B���h�E����
	//	nSpawnTime = THIRD_SPAWN;			//�X�|�[������
	//}
	else
	{
		nMoreCount = 10000000;
		nSpawnTime = 10000000;
	}

	if (g_MoreEnemy.MoreSecurityCounter == nMoreCount)
	{
		//�C�x���g�E�B���h�E�̐ݒ�
		SetEventWindow();

		PlaySound(SOUND_LABEL_SE_KEIBIKYOUKA);
	}


	if (g_MoreEnemy.MoreSecurityCounter >= nSpawnTime)
	{//�x����������܂ł̎��Ԃ�3000�ɂȂ�����

		if (g_MoreEnemy.bFirst == false)
		{//1�Ԗ�
			g_MoreEnemy.bFirst = true;		//�g�p����
		}
		else if (g_MoreEnemy.bSecond == false)
		{//2�Ԗ�
			g_MoreEnemy.bSecond = true;		//�g�p����
		}
		//else if (g_MoreEnemy.bThird == false)
		//{//3�Ԗ�
		//	g_MoreEnemy.bThird = true;		//�g�p����
		//}

		//g_MoreEnemy.MoreSecurityCounter = 0;

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			if (g_aEnemy[nCount].bMoreSpawn == false)
			{//�g�p���Ă��Ȃ���Ԃ�������

				if (g_aEnemy[nCount].MassGeneration == 1)
				{//MassGeneration��1��������

					//�G���o���҂���Ԃɂ���
					g_aEnemy[nCount].nState = ENEMYSTATE_WAIT;

					//���[�V�����̐ݒ�
					SetMotionEnemy(nCount, ENEMYMOTION_WAIT);

					//�o�����Ă����Ԃɂ���
					g_aEnemy[nCount].bMoreSpawn = true;

					//�x��������Ԃɂ���
					g_MoreEnemy.bMoreSpawn = true;

					//���_�ɖ߂�
					g_aEnemy[nCount].pos = g_aEnemy[nCount].posOrigin;

					//����n�_�̃��Z�b�g
					g_aEnemy[nCount].nNumPosCrawl = 0;

					//�e��ݒ�
					g_aEnemy[nCount].nIdxShadow = SetShadow(25.0f, 25.0f);

					//�e�̈ʒu�X�V
					SetPositionShadow(g_aEnemy[nCount].nIdxShadow, g_aEnemy[nCount].pos);

					//�A�C�R���̐ݒ菈��
					g_aEnemy[nCount].nIdxIcon = SetIcon(MINIMAP_ICON_ENEMY, 80.0f);
					UpdatePositionIcon(g_aEnemy[nCount].nIdxIcon, g_aEnemy[nCount].pos);
				}
			}
		}
	}

	//�e�̏��擾
	SHADOW *pShadow = GetShadow();

	//�}�b�v�A�C�R���̏��擾
	MINIICON *pMiniIcon = GetMiniIcon();

	if (g_MoreEnemy.SpawnCounter <= 0 && g_MoreEnemy.bMoreSpawn == true)
	{//�J�E���g��0�Ȃ�����

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			if (g_aEnemy[nCount].MassGeneration == 1)
			{
				if (g_aEnemy[nCount].nNowMotionNum != ENEMYMOTION_LOST)
				{

					SetMotionEnemy(nCount, ENEMYMOTION_LOST);
				}

				g_aEnemy[nCount].nState = ENEMYSTATE_LOST;
			}
		}
	}
}

//==================================================================================
//�G�̑�ʔ������̌x����������
//==================================================================================
void ManyMoreSpawnEnemy(void)
{
	//��̓����蔻��̏����擾
	SearchFan *pSearchFan = GetSearchFan();
	int nSpawnTime = 0;		//�X�|�[������

	//���Ԑݒ�
	if (g_MoreEnemy.bManyFirst == false)
	{//1�Ԗ�
		nSpawnTime = FIRST_SPAWNMOB;			//�X�|�[������
	}
	else if (g_MoreEnemy.bManySecond == false)
	{//2�Ԗ�
		nSpawnTime = SECOND_SPAWNMOB;			//�X�|�[������
	}
	else if (g_MoreEnemy.bManyThird == false)
	{//3�Ԗ�
		nSpawnTime = THIRD_SPAWNMOB;			//�X�|�[������
	}
	else
	{
		nSpawnTime = 10000000;
	}

	if (g_MoreEnemy.MoreSecurityCounter >= nSpawnTime)
	{//�x����������܂ł̎��Ԃ�3000�ɂȂ�����

		if (g_MoreEnemy.bManyFirst == false)
		{//1�Ԗ�
			g_MoreEnemy.bManyFirst = true;		//�g�p����
		}
		else if (g_MoreEnemy.bManySecond == false)
		{//2�Ԗ�
			g_MoreEnemy.bManySecond = true;		//�g�p����
		}
		else if (g_MoreEnemy.bManyThird == false)
		{//3�Ԗ�
			g_MoreEnemy.bManyThird = true;		//�g�p����
		}

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			if (g_aEnemy[nCount].bMoreSpawn == false)
			{//�g�p���Ă��Ȃ���Ԃ�������

				if (g_aEnemy[nCount].MassGeneration == 2)
				{//MassGeneration��2��������

					//�G���o���҂���Ԃɂ���
					g_aEnemy[nCount].nState = ENEMYSTATE_WAIT;

					//���[�V�����̐ݒ�
					SetMotionEnemy(nCount, ENEMYMOTION_WAIT);

					//�o�����Ă����Ԃɂ���
					g_aEnemy[nCount].bMoreSpawn = true;

					//�x��������Ԃɂ���
					g_MoreEnemy.bManyMoreSpawn = true;

					//���_�ɖ߂�
					g_aEnemy[nCount].pos = g_aEnemy[nCount].posOrigin;

					//����n�_�̃��Z�b�g
					g_aEnemy[nCount].nNumPosCrawl = 0;

					//�e��ݒ�
					g_aEnemy[nCount].nIdxShadow = SetShadow(25.0f, 25.0f);

					//�e�̈ʒu�X�V
					SetPositionShadow(g_aEnemy[nCount].nIdxShadow, g_aEnemy[nCount].pos);

					//�A�C�R���̐ݒ菈��
					g_aEnemy[nCount].nIdxIcon = SetIcon(MINIMAP_ICON_ENEMY, 80.0f);
					UpdatePositionIcon(g_aEnemy[nCount].nIdxIcon, g_aEnemy[nCount].pos);
				}
			}
		}
	}

	//�e�̏��擾
	SHADOW *pShadow = GetShadow();

	//�}�b�v�A�C�R���̏��擾
	MINIICON *pMiniIcon = GetMiniIcon();

	if (g_MoreEnemy.ManySpawnCounter <= 0 && g_MoreEnemy.bManyMoreSpawn == true)
	{//�J�E���g��0�Ȃ�����

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			if (g_aEnemy[nCount].MassGeneration == 2)
			{
				if (g_aEnemy[nCount].nNowMotionNum != ENEMYMOTION_LOST)
				{

					SetMotionEnemy(nCount, ENEMYMOTION_LOST);
				}

				g_aEnemy[nCount].nState = ENEMYSTATE_LOST;
			}
		}
	}
}

//==================================================================================
//�G�̕`�揈��
//==================================================================================
void DrawEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxnParent;			//�e�̌v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL mMatRed;			//�_���[�W�}�e���A��

	//���̏��N���A
	ZeroMemory(&mMatRed, sizeof(D3DXMATERIAL));
	mMatRed.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	mMatRed.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bMoreSpawn == true)
		{//�g�p���Ă�����

		 //���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			for (int nCntParts = 0; nCntParts < g_aEnemy[nCntEnemy].nPartsNum; nCntParts++)
			{

				//���[���h�}�g���b�N�X�̏�����
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				//�����𔽉f����
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxRot);

				//�ʒu�𔽉f����
				D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxTrans);


				if (g_aEnemy[nCntEnemy].aModel[nCntParts].nParent == -1)
				{//NULL�̏ꍇ,�e

				 //�v���C���[�̌����𔽉f����
					mtxnParent = g_aEnemy[nCntEnemy].mtxWorld;
				}
				else
				{//�p�[�c

				 //�e�̌����𔽉f����
					mtxnParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntParts].nParent].mtxWorld;
				}

				//�����̂�ɐe�̃��[���h�}�g���b�N�X���|���Z
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxnParent);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat->GetBufferPointer();

				if (g_aEnemy[nCntEnemy].aModel[nCntParts].bUse == true)
				{//�p�[�c���g�p����Ă�����

				 //���_�����J��Ԃ�
					for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].aModel[nCntParts].dwNumMat; nCntMat++)
					{

						//�}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//�e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_aEnemy[nCntEnemy].aModel[nCntParts].pTexture[nCntMat]);

						//�v���C���[(�p�[�c)�̕`��
						g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh->DrawSubset(nCntMat);
					}
				}
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//�G�̃��[�V��������
//==================================================================================
void SetMotionEnemy(int nCntEnemy, int nMotionType)
{

	//���̃��[�V�����̍X�V
	g_aEnemy[nCntEnemy].nNowMotionNum = nMotionType;
	g_aEnemy[nCntEnemy].nPatternKey = 0;	//���݂̃L�[�ԍ����Z�b�g
	g_aEnemy[nCntEnemy].nCntFrame = 0;	//�t���[���J�E���g���Z�b�g
	g_aEnemy[nCntEnemy].nCntAllFrame = 0;	//���t���[�������Z�b�g
}

//==================================================================================
//�G�̃��[�V�����X�V����
//==================================================================================
void UpdateMotionEnemy(int nCntEnemy)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	for (int nCntParts = 0; nCntParts < g_aEnemy[nCntEnemy].nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

	 //���̃L�[
		int nNextKey = (g_aEnemy[nCntEnemy].nPatternKey + 1) % g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].nNumKey;

		//�O��Ǝ��̃L�[�̍����擾
		float rotDiffX = g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.z;

		//�p�[�c�̌�����ݒ�
		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x =
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)g_aEnemy[nCntEnemy].nCntFrame /
				(float)g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].nFrame
				);

		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y =
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)g_aEnemy[nCntEnemy].nCntFrame /
				(float)g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].nFrame
				);

		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z =
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)g_aEnemy[nCntEnemy].nCntFrame /
				(float)g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].nFrame
				);

		//�p�x�̐��K��
		RotNormalize(&g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].nNowMotionNum].rot.x);
		RotNormalize(&g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].nNowMotionNum].rot.y);
		RotNormalize(&g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].nNowMotionNum].rot.z);

	}

	//�t���[���̃J�E���g�����Z
	g_aEnemy[nCntEnemy].nCntFrame++;
	g_aEnemy[nCntEnemy].nCntAllFrame++;

	if (g_aEnemy[nCntEnemy].nCntFrame >= g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].nFrame)
	{//�t���[���̃J�E���g���t���[�����ɒB������

	 //�t���[���̃J�E���g���[���ɖ߂�
		g_aEnemy[nCntEnemy].nCntFrame = 0;

		//�p�^�[��NO.�X�V
		g_aEnemy[nCntEnemy].nPatternKey = (g_aEnemy[nCntEnemy].nPatternKey + 1) % g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].nNumKey;

		if (g_aEnemy[nCntEnemy].nPatternKey == 0)
		{//�L�[���[���̎�

		 //�U����ԉ���
			g_aEnemy[nCntEnemy].nCntAllFrame = 0;	//���t���[�������Z�b�g

			if (g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//���[�V�����̐ݒ�
				SetMotionEnemy(nCntEnemy, ENEMYMOTION_DEF);
			}
			else
			{
				//���[�V�����̐ݒ�
				SetMotionEnemy(nCntEnemy, g_aEnemy[nCntEnemy].nNowMotionNum);
			}
		}
	}
}

//==================================================================================
//�G�̐ݒ菈��
//==================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//���f���̏��擾
	Model *pModel = GetXLoadData();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//��̓����蔻��̏����擾
	SearchFan *pSearchFan = GetSearchFan();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//�g�p���Ă��Ȃ�������

			for (int nCntFileNum = 0; nCntFileNum < g_aEnemy[nCntEnemy].nPartsNum; nCntFileNum++)
			{//�p�[�c�����ǂݍ���

			 //���f�����擾
				g_aEnemy[nCntEnemy].aModel[nCntFileNum] = g_aXLoadMeshEnemy[nCntFileNum];
			}

			//���f�����擾
			g_aEnemy[nCntEnemy].pos = pos;	//�ʒu
			g_aEnemy[nCntEnemy].posOrigin = g_aEnemy[nCntEnemy].pos;
			g_aEnemy[nCntEnemy].rot = rot;	//����

			g_aEnemy[nCntEnemy].bMoreSpawn = false;	//�g�p���Ă���󋵂ɂ���

													//��̌`�̐ݒ�
			g_aEnemy[nCntEnemy].nIdxSearchFan = SetSearchFan();

			//��̌`�̈ʒu�X�V
			SetPositionSearchFan(g_aEnemy[nCntEnemy].nIdxSearchFan, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);

			//�����Ȃ�����
			pSearchFan[g_aEnemy[nCntEnemy].nIdxSearchFan].bDisp = false;

			if (g_aEnemy[nCntEnemy].MassGeneration != 1 && g_aEnemy[nCntEnemy].MassGeneration != 2)
			{
				g_aEnemy[nCntEnemy].bMoreSpawn = true;	//�g�p���Ă���󋵂ɂ���

														//�e��ݒ�
				g_aEnemy[nCntEnemy].nIdxShadow = SetShadow(25.0f, 25.0f);

				//�e�̈ʒu�X�V
				SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);

				//�A�C�R���̐ݒ菈��
				g_aEnemy[nCntEnemy].nIdxIcon = SetIcon(MINIMAP_ICON_ENEMY, 80.0f);
				UpdatePositionIcon(g_aEnemy[nCntEnemy].nIdxIcon, g_aEnemy[nCntEnemy].pos);

				//������悤�ɂ���
				pSearchFan[g_aEnemy[nCntEnemy].nIdxSearchFan].bDisp = true;
			}

			g_aEnemy[nCntEnemy].bUse = true;     //�g�p���Ă���󋵂ɂ���
			g_aEnemy[nCntEnemy].nATKInterval = ATK_INTERVAL;		//�U���Ԋu
			g_aEnemy[nCntEnemy].nState = ENEMYSTATE_WAIT;

			g_aEnemy[nCntEnemy].nCntState = 0;
			g_aEnemy[nCntEnemy].nCntAction = 200;
			g_aEnemy[nCntEnemy].fChaseRadius = CHASE_RADIUS;	//�ǂ��|���͈͂̐ݒ�
			g_aEnemy[nCntEnemy].fSearchRadius = SEARCH_RADIUS;	//�T���͈͂̐ݒ�
			g_aEnemy[nCntEnemy].fSearchRot = 0.0f;	//�T���̌���
			g_aEnemy[nCntEnemy].fSearchLength = 100.0f + (float)(rand() % 10) * 10;	//�T���̒���
			g_aEnemy[nCntEnemy].nCntATK = 0;	//�U���J�E���g���Z�b�g

			for (int nCntParts = 0; nCntParts < g_aEnemy[nCntEnemy].nPartsNum; nCntParts++)
			{
				g_aEnemy[nCntEnemy].aModel[nCntParts].bUse = true;
			}
			g_aEnemyInfo.nNumEnemy++;	//�G�̐����Z

			//���[�V�����̐ݒ�
			SetMotionEnemy(nCntEnemy, ENEMYSTATE_WAIT);

			break;
		}
	}
}

//==================================================================================
//�G��X�t�@�C���ǂݍ��ݏ���
//==================================================================================
void XFileLoadEnemy(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntFileNum = 0; nCntFileNum < g_aEnemy[0].nPartsNum; nCntFileNum++)
	{//�p�[�c�����ǂݍ���

	 //X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aXLoadMeshEnemy[nCntFileNum].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aXLoadMeshEnemy[nCntFileNum].pBuffMat,
			NULL,
			&g_aXLoadMeshEnemy[nCntFileNum].dwNumMat,
			&g_aXLoadMeshEnemy[nCntFileNum].pMesh);

		D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

								//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aXLoadMeshEnemy[nCntFileNum].pBuffMat->GetBufferPointer();

		//���_�����J��Ԃ�
		for (int nCntMat = 0; nCntMat < (int)g_aXLoadMeshEnemy[nCntFileNum].dwNumMat; nCntMat++)
		{
			g_aXLoadMeshEnemy[nCntFileNum].pTexture[nCntMat] = NULL;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�t�@�C�������݂���

			 //�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aXLoadMeshEnemy[nCntFileNum].pTexture[nCntMat]);
			}
		}

		CheckVtx(g_aXLoadMeshEnemy[nCntFileNum].rot.y, &g_aXLoadMeshEnemy[nCntFileNum].vtxMax, &g_aXLoadMeshEnemy[nCntFileNum].vtxMin, g_aXLoadMeshEnemy[nCntFileNum].pMesh, g_aXLoadMeshEnemy[nCntFileNum].pVtxBuff);

	}
}

//==================================================================================
//�G�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetEnemy(void)
{
	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

						//�t�@�C�����J��
	pFile = fopen(ENEMYSET_TXT, "r");

	char aComment[MAX_COMMENT];	//�R�����g�A�m�F��
	int nCntEnemy = 0;

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

		 //������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//�G�̐ݒ�
			if (strcmp(aComment, "ENEMYSET") == 0)
			{//���f���̓ǂݍ��݂��J�n

			 //����n�_�̃J�E���g
				int nCntCrawl = 0;

				while (strcmp(aComment, "END_ENEMYSET"))
				{//END_ENEMYSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "POS") == 0)
					{//POS��������ʒu�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.x);	//X���W
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.y);	//Y���W
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.z);	//Z���W
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROT������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.x);	//X�̌���
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.y);	//Y�̌���
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.z);	//Z�̌���
					}

					if (strcmp(aComment, "CRAWLPOS") == 0)
					{//CRAWLPOS�������珄��n�_�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].x);	//X���W
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].y);	//Y���W
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].z);	//Z���W

																							//����n�_�̔ԍ����Z
						nCntCrawl++;
					}

					if (strcmp(aComment, "SPAWN") == 0)
					{
						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aEnemy[nCntEnemy].MassGeneration);   //��ɏ��񂵂Ă��邩��ʔ������ɏ��񂷂邩
					}

					if (strcmp(aComment, "LIFE") == 0)
					{//LIFE��������̗͓ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
					}

				}//END_ENEMYSET�̂�����

				 //����n�_�̍ő吔
				g_aEnemy[nCntEnemy].nNumMaxCrawl = nCntCrawl;

				//�G�̔ԍ����Z
				nCntEnemy++;

			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//�I�������Ń��[�v�𔲂���

				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	//X�t�@�C���̓ǂݍ���
	XFileLoadEnemy();

	if (GetMode() == MODE_GAME)
	{
		//�G�̔z�u
		for (int nCount = 0; nCount < nCntEnemy; nCount++)
		{
			SetEnemy(g_aEnemy[nCount].pos, g_aEnemy[nCount].rot);
		}
	}
}

//==================================================================================
//�G�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetMotionEnemy(void)
{

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	char aComment[MAX_COMMENT];	//�R�����g�p
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0, nCntFileName = 0, nCntSetParts = 0;

	//�t�@�C�����J��
	pFile = fopen(ENEMY_TXT, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

		 //������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//==================================================
			//�K�w�\���̐ݒ�
			//==================================================
			//���f�����̐ݒ�
			if (strcmp(aComment, "NUM_MODEL") == 0)
			{//NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	//=�̕�
				fscanf(pFile, "%d", &g_aEnemy[0].nPartsNum);	//���f����
			}

			while (nCntFileName != g_aEnemy[0].nPartsNum)
			{//���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			 //������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);

				//���f�����̐ݒ�
				if (strcmp(aComment, "MODEL_FILENAME") == 0)
				{//NUM_MODEL��������

					fscanf(pFile, "%s", &aComment[0]);	//=�̕�
					fscanf(pFile, "%s", &g_aXLoadMeshEnemy[nCntFileName].acFilename[0]);	//�t�@�C����
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
								fscanf(pFile, "%d", &g_aXLoadMeshEnemy[nCntSetParts].nType);	//���f����ނ̗�

							}

							if (strcmp(aComment, "PARENT") == 0)
							{//PARENT��������e�̔ԍ��ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aXLoadMeshEnemy[nCntSetParts].nParent);	//�e�̔ԍ�

							}

							if (strcmp(aComment, "POS") == 0)
							{//POS��������ʒu�ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].pos.x);	//X���W
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].pos.y);	//Y���W
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].pos.z);	//Z���W

																								//���̈ʒu���蓖��
								g_aXLoadMeshEnemy[nCntSetParts].posOrigin = g_aXLoadMeshEnemy[nCntSetParts].pos;

							}

							if (strcmp(aComment, "ROT") == 0)
							{//ROT������������ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].rot.x);	//X�̊p�x
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].rot.y);	//Y�̊p�x
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].rot.z);	//Z�̊p�x

								g_aXLoadMeshEnemy[nCntSetParts].rot = D3DXToRadian(g_aXLoadMeshEnemy[nCntSetParts].rot);

							}

							if (strcmp(aComment, "WEAPON") == 0)
							{//WEAPON�������畐�킩�ǂ����ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�

							}

						}//END_PARTSSET�̂�����

						nCntSetParts++;	//�p�[�c�̃J�E���g�����Z
					}

				}//END_CHARACTERSET�̂�����

			}

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
						fscanf(pFile, "%d", &g_aEnemy[0].aMotionData[nCntMotion].nLoop);	//���[�v0��1

					}

					if (strcmp(aComment, "NUM_KEY") == 0)
					{//NUM_KEY��������L�[���ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aEnemy[0].aMotionData[nCntMotion].nNumKey);	//�L�[��

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
								fscanf(pFile, "%d", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//�Đ��t���[��

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
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X���W
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y���W
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z���W
									}

									if (strcmp(aComment, "ROT") == 0)
									{//ROT��������p�x�ǂݍ���

										fscanf(pFile, "%s", &aComment[0]);		//=�̕�
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X�p�x
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y�p�x
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z�p�x
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

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].nPartsNum = g_aEnemy[0].nPartsNum;

		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
		{//���[�V�����̐��J��Ԃ�

			g_aEnemy[nCntEnemy].aMotionData[nCntMotion].nLoop = g_aEnemy[0].aMotionData[nCntMotion].nLoop;
			g_aEnemy[nCntEnemy].aMotionData[nCntMotion].nNumKey = g_aEnemy[0].aMotionData[nCntMotion].nNumKey;

			for (int nCntKey = 0; nCntKey < g_aEnemy[0].aMotionData[nCntMotion].nNumKey; nCntKey++)
			{//�L�[�̐��J��Ԃ�

				g_aEnemy[nCntEnemy].aMotionData[nCntMotion].aKey[nCntKey].nFrame = g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].nFrame;

				for (int nCntParts = 0; nCntParts < g_aEnemy[0].nPartsNum; nCntParts++)
				{//�p�[�c���J��Ԃ�

					g_aEnemy[nCntEnemy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot;
					g_aEnemy[nCntEnemy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos;
				}
			}
		}
	}
}

//==================================================================================
//�G�̏I�����X�V����
//==================================================================================
void UpdateGameEndEnemy(int nCntEnemy)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�����Œ�
	g_aEnemy[nCntEnemy].rot.y = 0.0f;

	if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_DASH)
	{
		//���[�V�����̐ݒ�
		SetMotionEnemy(nCntEnemy, ENEMYMOTION_DASH);
	}

	//�����Ă�����Ƀ_�b�V��
	g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 1.7f;
	g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 1.7f;

	//�ʒu�X�V
	g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

	//��������
	g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * MOVE_FACTOR;
	g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z) * MOVE_FACTOR;
}

//==================================================================================
//�G�̃`���[�g���A���X�V����
//==================================================================================
void UpdateTutorialEnemy(void)
{
	//�v���C���[�̏��擾
	ModelModel *pModel = GetModel();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();

	//�Q�O�̏��擾
	Crowd *pCrowd = GetCrowd();

	//�x������
	MoreSpawnEnemy();

	if (pTutorial->nStep >= TUTORIAL_STEP_MORS && pTutorial->aControl.bSetEnemy == false)
	{//�Ö��̃X�e�b�v�ȏ� && ���u��0

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//����n�_�̍ő吔
			g_aEnemy[nCnt].nNumMaxCrawl = 1;
			g_aEnemy[nCnt].PosCrawl[0] = pPlayer->pos;

			//�݂�Ȓʏ�X�|�[��
			g_aEnemy[nCnt].MassGeneration = 99;

			//�G���o���҂���Ԃɂ���
			g_aEnemy[nCnt].nState = ENEMYSTATE_WAIT;

			//���[�V�����̐ݒ�
			SetMotionEnemy(nCnt, ENEMYMOTION_WAIT);

			g_aEnemy[nCnt].bMoreSpawn = true;	//�g�p���Ă���󋵂ɂ���
		}

		SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * 1.0f) * 150.0f, pPlayer->pos.y, pPlayer->pos.z + cosf(D3DX_PI * 1.0f) * 150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * 0.5f) * 150.0f, pPlayer->pos.y, pPlayer->pos.z + cosf(D3DX_PI * 0.5f) * 150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * 0.0f) * 150.0f, pPlayer->pos.y, pPlayer->pos.z + cosf(D3DX_PI * 0.0f) * 150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * -0.5f) * 150.0f, pPlayer->pos.y, pPlayer->pos.z + cosf(D3DX_PI * -0.5f) * 150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//�G�z�u����
		pTutorial->aControl.bSetEnemy = true;

		//�v���C���[�~�߂�
		pPlayer->bATK = false;
		pPlayer->bMove = false;
		SetMotisonPlayer(PLAYERMOTION_DEF);
		pBuddy[BUDDYTYPE_HYOROGARI].bATK = false;
		pBuddy[BUDDYTYPE_DEPPA].bATK = false;

		if (pBuddy[BUDDYTYPE_HYOROGARI].nState == BUDDYSTATE_DISPATCH)
		{//�h����ԂȂ�

		 //�Ăі߂�
			SetLeftDisPatch();
			pBuddy[BUDDYTYPE_HYOROGARI].nState = BUDDYSTATE_NONE;
			SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_DEF);
		}

		if (pBuddy[BUDDYTYPE_DEPPA].nState == BUDDYSTATE_DISPATCH)
		{//�h����ԂȂ�

			//�Ăі߂�
			SetRightDisPatch();
			pBuddy[BUDDYTYPE_DEPPA].nState = BUDDYSTATE_NONE;
			SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_DEF);
		}
	}

	if (pTutorial->nStep == TUTORIAL_STEP_END)
	{//�Ö��̃X�e�b�v�ȏ� && ���u��0

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if (g_aEnemy[nCnt].nState != ENEMYSTATE_LOST)
			{
				g_aEnemy[nCnt].nState = ENEMYSTATE_LOST;
			}

			if (g_aEnemy[nCnt].nNowMotionNum != ENEMYMOTION_LOST)
			{

				SetMotionEnemy(nCnt, ENEMYMOTION_LOST);
			}
		}
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		g_aEnemy[nCnt].PosCrawl[0] = pPlayer->pos;
	}


	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bMoreSpawn == true)
		{//�g�p���Ă�����

		 //�ߋ��̈ʒu�ۑ�
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			if (g_aEnemy[nCntEnemy].nCntSearchCircle >= SEARCH_INTERVAL - 1)
			{//�K��l��������

				g_aEnemy[nCntEnemy].nIdxSearchRadius = SetSearchCircle(
					D3DXVECTOR3(
						g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 10.0f, g_aEnemy[nCntEnemy].pos.z),
					g_aEnemy[nCntEnemy].rot,
					D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.5f),
					10.0f, 0.0f,
					(int)(SEARCH_RADIUS * 0.5f), 2.0f,
					SEARCH_CIRCLE_TYPE_ORANGE2);
			}

			//�T���͈͉~�̃J�E���g���Z
			g_aEnemy[nCntEnemy].nCntSearchCircle = (g_aEnemy[nCntEnemy].nCntSearchCircle + 1) % SEARCH_INTERVAL;

			if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT || g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_WAIT || g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_LOST)
			{//���[�V�������ˌ��A�G�̏�Ԃ��o���҂��܂��́A���ő҂��ȊO�̎�

			 //�G�̑���
				ControllEnemy(nCntEnemy);
			}

			//�G���m�̓����蔻��
			CollisionModelEnemy(nCntEnemy);

			//���W����
			LimitPosEnemy(nCntEnemy);

			//��ԍX�V
			UpdateStateEnemy(nCntEnemy);

			//�s���X�V����
			UpdateActionEnemy(nCntEnemy);

			//���[�V�����X�V����
			UpdateMotionEnemy(nCntEnemy);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);

			//�T���~�̈ʒu�X�V
			SetPositionSearchCircle(g_aEnemy[nCntEnemy].nIdxSearchRadius, g_aEnemy[nCntEnemy].pos);

			//�A�C�R���̈ʒu�X�V
			UpdatePositionIcon(g_aEnemy[nCntEnemy].nIdxIcon, g_aEnemy[nCntEnemy].pos);

			//��̌`�̈ʒu�X�V
			SetPositionSearchFan(g_aEnemy[nCntEnemy].nIdxSearchFan, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);

			//�Z���t�̃Z�b�g
			EnemySerifSet(nCntEnemy);

			//�Z���t�̈ʒu�X�V
			SetPositionSerif(g_aEnemy[nCntEnemy].nIdxSerif, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 150.0f, g_aEnemy[nCntEnemy].pos.z });
		}
	}
}

//==================================================================================
//�G�̏��擾
//==================================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==================================================================================
//�G�̏��擾
//==================================================================================
EnemyInfo *GetEnemyInfo(void)
{
	return &g_aEnemyInfo;
}
