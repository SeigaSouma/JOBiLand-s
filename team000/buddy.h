//=============================================================================
//
// �G���� [buddy.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _BUDDY_H_		//���̃}�N����`������Ă��Ȃ�������
#define _BUDDY_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define JUMP		(20.0f)	//�W�����v�ړ���
#define GRAVITY		(1.0f)	//�d��
#define MAX_BUDDY	(2)		//���_�̍ő吔
#define BUDDY_DISTANCE	(60.0f)		//�v���C���[�Ƃ̊Ԋu
#define DIS_ROT		(D3DX_PI * 0.45f)

#include "main.h"
#include "model.h"
#include "crowd.h"
#include "mob.h"
#include "enemy.h"
#include "player.h"

//���f���̎��
typedef enum
{
	BUDDYTYPE_HYOROGARI = 0,
	BUDDYTYPE_DEPPA,
	BUDDYTYPE_MAX
}BUDDYTYPE;

typedef enum
{
	BUDDYMOTION_DEF = 0,	//�j���[�g�������[�V����
	BUDDYMOTION_WALK,		//�ړ����[�V����
	BUDDYMOTION_ACTION,		//�A�N�V�������[�V����
	BUDDYMOTION_KYOROKYORO,	//�L�����L�������[�V����
	BUDDYMOTION_MOVEACTION,	//�ړ��Ö����[�V����
	BUDDYMOTION_FEVERWALK,	//�t�B�[�o�[���̈ړ����[�V����
	BUDDYMOTION_FEVERACTION,	  //�t�B�[�o�[���̃A�N�V�������[�V����
	BUDDYMOTION_FEVERMOVEACTION,  //�t�B�[�o�[���̈ړ��A�N�V�������[�V����
	BUDDYMOTION_RANKING,	//�����L���O�̃��[�V����
	BUDDYMOTION_MAX
}BUDDYMOTION;

//�v���C���[�̏��
typedef enum
{
	BUDDYSTATE_NONE = 0,	//�������Ă��Ȃ����
	BUDDYSTATE_DISPATCH,	//�h�����
	BUDDYSTATE_RETURN,		//�A�ҏ��
	BUDDYSTATE_MAX
}BUDDYSTATE;

//�ړ�AI�̏��
typedef enum
{
	MOVEAI_NONE = 0,		//�������Ă��Ȃ����
	MOVEAI_WIDTH,			//���ړ�
	MOVEAI_HEIGHT,			//�c�ړ�
	MOVEAI_DIAGONAL_FORM,	//�΂߈ړ�(��)
	MOVEAI_DIAGONAL_BACK,	//�΂߈ړ�(��O)
	MOVEAI_STOP,			//�~�܂�(�����ύX)
	MOVEAI_CHASE,			//�ǂ��������
	MOVEAI_ESCAPE,			//�������
	MOVEAI_MAX

}MOVEAISTATE;

//�ړ�AI�̍\����
typedef struct
{
	int nRotCounter;		//�����ύX�J�E���^�[
	int nMoveCounter;		//�ړ��ύX�J�E���^�[
	int nRot;				//������+-
	int nMoveState;			//�ړ����
	int nCntStop;			//�~�܂��Ă����Ԃ��o�₷������J�E���^�[
	int nChaseCounter;		//�ǂ������Ă鎞��
	int nNotChaseCounter;	//�ǂ������Ȃ�����
	int nEscapeCounter;		//�����鎞��

	float fDistance;		//����

	bool bPosDest;				//�ړI�n�ɂ�����
	bool bMobDisc;				//���u�𔭌��������ǂ���
	bool bMobNum[MAX_MOB];		//���Ԗڂ̃��u�𔭌�������
	bool bChaseTime;			//�ǂ�������܂ł̎��Ԃ𑫂���
	bool bPenUse;				//�y�����C�g���g���Ă邩
	bool bPenRange[MAX_MOB];	//�y�����C�g�͈̔͂ɓ����Ă��邩
	bool bEscape;				//�����邩
	bool bEscapeNum[MAX_ENEMY];	//���Ԗڂ̓G��

} MoveAI;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 formationPos;	//����̈ʒu
	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩
	bool bFever;			//�t�B�[�o�[���
	float fRotDiff;			//�����̍���
	float fRotDest;			//�����̍���
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	int nIdxIcon;			//�~�j�}�b�v�A�C�R���̃C���f�b�N�X�ԍ�
	int nIdxWave[MAX_WAVEINDEX];	//�Ռ��g�̃C���f�b�N�X�ԍ�
	int nState;				//���
	int nCntState;			//��ԃJ�E���g
	int nCntChase;			//�ǂ��|���J�E���g
	bool bCollisionModel;	//���f���Ƃ̔������邩�ǂ���

	//���[�V�����n
	int nNowMotionNum;		//���݂̃��[�V�����ԍ�
	int nCntFrame;			//�t���[���̃J�E���g
	int nPatternKey;		//���ڂ̃L�[��
	int nPartsNum;			//�p�[�c��
	bool bMove;				//�ړ����Ă��邩
	bool bATK;				//�U�����Ă邩
	int nCntAllFrame;		//���t���[���J�E���g

	int nCntWalk;			//�ړ��̃J�E���g
	int nCntPenlight;		//�Ö��̃J�E���g
	float fRadius;			//���a

	float fThrowRadius;		//������͈�
	int nThrowCnt;			//������J�E���g

	Model aModel[MAX_MODEL];	//�p�[�c���
	MotionData aMotionData[MAX_MOTION];		//���[�V�����p�̏��
	MoveAI aMoveAI;			//�ړ�AI
	Formation aFormation[MAX_CROWD];	//������
}Buddy;

//�v���g�^�C�v�錾
void InitBuddy(void);
void UninitBuddy(void);
void UpdateBuddy(void);
void UpdateTitleBuddy(void);
void UpdateResultBuddy(void);
void DrawBuddy(void);
Buddy *GetBuddy(void);
void SetBuddy(int nCntBuddy);
void SetBuddyDispatch(D3DXVECTOR3 pos, int nType);
void SetMotisonBuddy(int nCntBuddy, int nMotionType);
int SetFormationPosBuddy(int nCntBuddy);
void UpdateRankingBuddy(void);

#endif