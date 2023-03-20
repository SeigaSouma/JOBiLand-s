//=============================================================================
//
// �Q�O���� [crowd.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CROWD_H_		//���̃}�N����`������Ă��Ȃ�������
#define _CROWD_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_CROWD	(1600)	//���f���̍ő吔
#define FORMATION_DISTANCE	(60.0f)	//�Ԋu

#include "main.h"
#include "model.h"

typedef enum
{
	CROWDSTATE_NONE = 0,	//�������Ă��Ȃ����
	CROWDSTATE_JOIN,		//�Q�����
	CROWDSTATE_CHASE,		//�ǂ��|�����
	CROWDSTATE_FADE,		//�����Ă������
	CROWDSTATE_WAIT,        //�ˌ��҂����
	CROWDSTATE_RETURN,		//�A����
	CROWDSTATE_MAX
}CROWDSTATE;

typedef enum
{
	CROWDTARGET_PLAYER = 0,
	CROWDTARGET_SHINBOKU,
	CROWDTARGET_MAX
}CROWDTARGET;

typedef enum
{
	PARENT_PLAYER = 0,		//�v���C���[
	PARENT_GARI,			//�q�����K���A�L��
	PARENT_CHIBI,			//�N�\�ዾ�`�r
	PARENT_MAX
}PARENT;

//�Q�O�̍\����
typedef struct
{
	int nCrowdValue;        //�G�̑���
	int nCrowdCnter;		//������r
	int nCrowdKeep;			//�ۑ��p
	int nParentPlayerNum;	//�e���v���C���[�̐�
	int nCroedTutorial;		//�`���[�g���A���ۑ��p
}CrowdInfo;

//�Q�O�̐�
typedef struct
{
	int nNum;			//�Q�O�̐�
	int nNumOld;		//�ߋ��̌Q�O�̐�

}CrowdNum;

typedef struct
{
	D3DXVECTOR3 pos;	//����̈ʒu
	int nIndex;			//����̃C���f�b�N�X�ԍ�
	bool bUse;			//������g�p���Ă��邩
}Formation;

typedef struct
{
	int nState;				//���
	int nCntChase;			//�ǂ��|����J�E���g
	float fFadeAlpha;		//�t�F�[�h�̕s�����x
	D3DXVECTOR3 formationPos;	//����̈ʒu
	D3DXVECTOR3 ParentPos;	//�e�̈ʒu
	D3DXVECTOR3 ParentRot;	//�e�̌���
	float fThrowRadius;		//������͈�
	int nThrowCnt;			//������J�E���g
	int nType;				//���
	int nParent;			//�e
	int nTexType;			//�e�N�X�`���^�C�v

	bool bFever;			//�t�B�[�o�[��Ԃ�

	Model aModel;			//���f���̍\����
	Formation aFormation;	//����̏��
}Crowd;

//�v���g�^�C�v�錾
void InitCrowd(void);
void UninitCrowd(void);
void UpdateCrowd(void);
void DrawCrowd(void);
Crowd *GetCrowd(void);
CrowdInfo *GetCrowdInfo(void);
CrowdNum *GetCrowdNum(void);
void SetCrowd(D3DXVECTOR3 pos, int nType, int nTexType, int nParent);
void SetCrowdResult(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexType, int nParent);
int SetFormationPos(int nParent);
void UpdateResultCrowd(void);

#endif