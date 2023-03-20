//=============================================================================
//
// ���u���� [mob.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MOB_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MOB_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_MOB			(1024)		//���f���̍ő吔
#define HYP_CNTTIME		(180)		//�Ö��x����������܂ł̎���
#define MAX_SPAWN		(64)		//�X�|�[���n�_�̍ő吔
#define STOP_HYPNOSIS	(15)	//�~�܂��Ă鎞�̍Ö��x
#define MOVE_HYPNOSIS	(8)	//�����Ă鎞�̍Ö��x
#define FIRST_SPAWNMOB	(60 * 30)	//1�Ԗڂ̑�ʔ���
#define SECOND_SPAWNMOB	(60 * 140)	//2�Ԗڂ̑�ʔ���
#define THIRD_SPAWNMOB	(60 * 200)	//3�Ԗڂ̑�ʔ���
#define STOP_FEVER_HYPNOSIS	(20)	//�t�B�[�o�[���̍Ö��x
#define MOVE_FEVER_HYPNOSIS	(15)	//�t�B�[�o�[���̍Ö��x

#include "main.h"
#include "model.h"
#include "serif.h"

typedef enum
{
	MOBSTATE_APPIEAR = 0,	//�������Ă��Ȃ����
	MOBSTATE_DMG,		//�_���[�W���
	MOBSTATE_KEEP,		//�Ö��ɂ������Ă�����
	MOBSTATE_HYPNOSIS,	//�Ö����
	MOBSTATE_WEAR,		//�������J�o�[����Ă���
	MOBSTATE_NONE,
}MOBSTATE;

typedef enum
{
	MOBACTION_NONE = 0,	//�������Ă��Ȃ����
	MOBACTION_SEARCH,		//�T�����
	MOBACTION_CHASE,		//�ǂ����
	MOBACTION_WAIT,		//�҂����
	MOBACTION_MAX
}MOBACTION;

typedef enum
{
	MOBTARGET_PLAYER = 0,
	MOBTARGET_SHINBOKU,
	MOBTARGET_MAX
}MOBTARGET;

//SE�g�p��
typedef struct
{
	bool bUseRotate;		//SE�g�p���Ă邩
} SwitchSE;


//�X�|�[���n�_�̍\����
typedef struct
{
	bool bUseSpawn;
} SwitchSpawn;

//�X�|�[���n�_�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�X�|�[���n�_
	D3DXVECTOR3 rot;		//�X�|�[���n�_�̌���
	int nType;				//�X�|�[������n�_�̎��
	int nMany;				//��ʔ������邩�̔���
	bool bUse;              //���̌�������ʔ����n�_������
} Spawn;

//��ʔ����̍\����
typedef struct
{
	int nSpawnCounter;	//��ʔ�������܂ł̎���
	int nStopTime;		//��ʔ����I���܂ł̎���
	int nBillNumber;	//�����̏ꏊ
	int nCounter;

	bool bFirst;		//1�Ԗڂ̃X�|�[��
	bool bSecond;		//2�Ԗڂ̃X�|�[��
	bool bThird;		//3�Ԗڂ̃X�|�[��

	Spawn aSpawn[MAX_SPAWN];	//�X�|�[�����
} ManySpawn;

//�ʏ픭���̍\���́i�E�F�[�u�j
typedef struct
{
	int nStartSpawn;		//�����J�n����
	int nStopSpawn;			//�����I������
	int nSpawnCounter;		//��������Ԋu
	int nPosChangeCounter;	//��������ꏊ�ς���܂ł̎���
	int nBillNumber;		//�����̏ꏊ

} WaveSpawn;

//�Q�O�̍\����
typedef struct
{
	int nHypnosis;			//�Ö��x
	int nSubHypCounter;		//�Ö��x��������܂ł̃J�E���^�[
	int nSubHypInter;		//�Ö��x�����炷�Ԋu
	int nAddHypInter;		//�Ö��x�𑝂₷�Ԋu
	int nSwitchCounter;		//rot�̐؂�ւ�����
	int nTurnSwitchX;		//rot.x��+-�؂�ւ�
	int nTurnSwitchZ;		//rot.z��+-�؂�ւ�
	int nSpawnCounter;		//���u�̃X�|�[�����鎞��
	int nSpawnNumber;		//���u����x�ɃX�|�[�����鐔
	int nSpawnType;			//�X�|�[���n�_�̎��
	int nMoveCounter;		//���u�̈ړ��̃J�E���^�[
	int nIdxIcon;			//�~�j�}�b�v�A�C�R���̃C���f�b�N�X�ԍ�
	int nTexType;			//�e�N�X�`���^�C�v

	float fAlpha;				//�����x
	float fRandAlpha;			//�����x�̑�����

	Model aModel;					//���f���̍\����
}Mob;

//�v���g�^�C�v�錾
void InitMob(void);
void UninitMob(void);
void UpdateMob(void);
void UpdateTutorialMob(void);
void DrawMob(void);
Mob *GetMob(void);
ManySpawn *GetManySpawn(void);
SwitchSpawn *GetSwitchSpawn(void);
SwitchSE *GetSwitchSE(void);
void SetMob(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nSpawnType, int nModelType);
void HitMob(int nCntMob, int nValue);

#endif