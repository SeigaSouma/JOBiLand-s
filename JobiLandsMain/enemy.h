//=============================================================================
//
// �G���� [enemy.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _ENEMY_H_		//���̃}�N����`������Ă��Ȃ�������
#define _ENEMY_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_ENEMY	(32)	//���f���̍ő吔
#define TARGETNUM_ENEMY	(60)	//�|���ڕW��
#define MAX_POSCRAWL	(32)	//����|�C���g�̍ő吔
#define SEARCH_RADIUS	(120.0f)

#include "main.h"
#include "model.h"

typedef enum
{
	ENEMYSTATE_NONE = 0,	//�������Ă��Ȃ����
	ENEMYSTATE_SEARCH,		//�T�����
	ENEMYSTATE_CHASE,		//�ǂ����
	ENEMYSTATE_WAIT,        //�o���҂����
	ENEMYSTATE_LOST,        //���ő҂����
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{
	ENEMYMOTION_DEF = 0,	//�j���[�g�������[�V����
	ENEMYMOTION_DASH,		//�_�b�V��
	ENEMYMOTION_SHOT,       //�ˌ�
	ENEMYMOTION_WAIT,       //�o���҂�
	ENEMYMOTION_LOST,       //���ő҂�
	ENEMYMOTION_MAX
}ENEMYMOTION;

typedef enum
{
	ENEMYTARGET_PLAYER = 0,
	ENEMYTARGET_SHINBOKU,
	ENEMYTARGET_MAX
}ENEMYTARGET;

//�G�̍\����
typedef struct
{
	int nNumEnemy;			//�G�̐�
}EnemyInfo;

typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posOrigin;	//���݂̈ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩
	bool bMoreSpawn;        //�x���������Ă��邩�ǂ���
	int nState;				//���
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	int nIdxSearchRadius;	//�T���͈͉~�̃C���f�b�N�X�ԍ�
	int nIdxIcon;			//�~�j�}�b�v�A�C�R���̃C���f�b�N�X�ԍ�
	int nIdxSearchFan;		//��̃C���f�b�N�X�ԍ�
	int nIdxSerif;			//�Z���t�̃C���f�b�N�X�ԍ�
	int nCntState;			//��ԃJ�E���^�[
	int nCntAction;			//�s���J�E���^�[
	int nATKInterval;		//�U���̊Ԋu
	int nCntATK;			//�U���̊Ԋu�J�E���g
	int nCntSearchCircle;	//�T���͈͉~�̃J�E���g
	float fChaseRadius;		//�ǂ��|���͈�
	float fSearchRadius;	//�T���͈�
	float fSearchLength;	//�T���̒���
	float fSearchRot;		//�T���̌���
	float fRadius;			//�T���̌���
	D3DXVECTOR3 PosCrawl[MAX_POSCRAWL];	//����|�C���g
	int nNumPosCrawl;		//���񂷂�n�_�̔ԍ�
	int nNumMaxCrawl;		//���񂷂�n�_�̍ő吔
	int MassGeneration;     //��ʔ������ɏo��G
	int nCntWait;           //�o���҂��̃J�E���g

	//���[�V�����n
	int nNowMotionNum;		//���݂̃��[�V�����ԍ�
	int nCntFrame;			//�t���[���̃J�E���g
	int nPatternKey;		//���ڂ̃L�[��
	int nPartsNum;			//�p�[�c��
	int nCntAllFrame;		//���t���[���J�E���g

	Model aModel[MAX_MODEL];	//���f���̍\����
	MotionData aMotionData[MAX_MOTION];		//���[�V�����p�̏��
}Enemy;

typedef struct
{
	int MoreSecurityCounter;    //�x����������܂ł̎���
	int SpawnCounter;           //�x�������I���܂ł̎���
	int ManySpawnCounter;		//��ʔ������̌x�������I���܂ł̎���
	bool bMoreSpawn;            //�x�����������ǂ���
	bool bManyMoreSpawn;		//��ʔ������Ɍx�����������ǂ���
	bool bFirst;				//1�Ԗڂ̃X�|�[��
	bool bSecond;				//2�Ԗڂ̃X�|�[��
	bool bThird;				//3�Ԗڂ̃X�|�[��
	bool bManyFirst;			//1�Ԗڂ̑�ʔ������̃X�|�[��
	bool bManySecond;			//2�Ԗڂ̑�ʔ������̃X�|�[��
	bool bManyThird;			//3�Ԗڂ̑�ʔ������̃X�|�[��

} MoreEnemy;

//�v���g�^�C�v�錾
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void UpdateTutorialEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
EnemyInfo *GetEnemyInfo(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void HitEnemy(int nCntEnemy, D3DXVECTOR3 move, int nDMG);
void SetMotionEnemy(int nCntEnemy, int nMotionType);
void ReadSetEnemy(void);

#endif