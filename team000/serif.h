//=============================================================================
//
// �Z���t���� [serif.h]
// Author : �匴�叫
//
//=============================================================================
#ifndef _SERIF_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SERIF_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_SERIF	(1024)	//�Z���t�̍ő吔

//�N�̃Z���t�H
typedef enum
{
	SERIFSTATE_DISPATCH = 0,    //�h�����̃Z���t
	SERIFSTATE_RETURN,          //�A�Ҏ��̃Z���t
	SERIFSTATE_ENEMYNONE,		//�G�̕��ʂ̃Z���t
	SERIFSTATE_ENEMYDISC,		//�G�������̃Z���t
	SERIFSTATE_MOB_BOCCHI,		//�ڂ����̃Z���t
	SERIFSTATE_MOB_YAKUZA,		//�₭���̃Z���t
	SERIFSTATE_MOB_WORK,		//��Ј��̃Z���t
	SERIFSTATE_MAX
} SERIFSTATE;

//�G�i���낿���j
typedef enum
{
	ENEMYSERIFSTATE_SEEK = 0,	//�j�[�g�͂ǂ���
	ENEMYSERIFSTATE_ERADIC,		//�j�[�g�o��
	ENEMYSERIFSTATE_AKIBA,		//�H�t�����Ȃ���
	ENEMYSERIFSTATE_ZINKEN,		//�j�[�g�ɐl���Ȃ�
	ENEMYSERIFSTATE_WORK,		//�������Ǝv��Ȃ���
	ENEMYSERIFSTATE_OBUTSU,		//�����͏��ł��[�I
}ENEMYSERIFSTATE;

//�G�i�������j
typedef enum
{
	DISCSERIFSTATE_BIKKURI = 0,		//�I
	DISCSERIFSTATE_NEET,			//�j�[�g����
	DISCSERIFSTATE_MADAMADA,		//�܂��܂�����
	DISCSERIFSTATE_TAIDA,			//�ӑăf�X�˂�
	DISCSERIFSTATE_HANZAI,			//�o���Ȃ���ƍ߂���Ȃ�
}ENEMYDISCSERIFSTATE;

//�ڂ���
typedef enum
{
	BOCCHISERIFSTATE_CLUB = 0,	//�ڂ����i�����j
	BOCCHISERIFSTATE_REAL,		//�ڂ����i�����͕|���j
	BOCCHISERIFSTATE_EEE,		//�ڂ����i���A�j
	BOCCHISERIFSTATE_SORRY,		//�ڂ����i�C�L���Ă��݂܂���j
	BOCCHISERIFSTATE_WORK,		//�ڂ����i�������畉���j
}BOCCHISERIFSTATE;

//�₭��
typedef enum
{
	YAKUZASERIFSTATE_BOSS,			//�₭���i�{�X�ƌĂׁj
	YAKUZASERIFSTATE_RAPPER,		//�₭���i���N�U�����b�p�[�j
	YAKUZASERIFSTATE_KOYUBI,		//�₭���i���w�Ȃ��j
	YAKUZASERIFSTATE_TOKYO,			//�₭���i�����p�j
	YAKUZASERIFSTATE_HEKE,			//�₭���i�w�P�j
}YAKUZASERIFSTATE;

//��Ј�
typedef enum
{
	WORKSERIFSTATE_BLACK,		//��Ј��i��Ђ�߂����j
	WORKSERIFSTATE_STATION,		//��Ј��i�d�Ԓx���j
	WORKSERIFSTATE_ANIME,		//��Ј��i�A�j�������j
	WORKSERIFSTATE_KEEP,		//��Ј��i�Q�O�A�΁j
}WORKSERIFSTATE;

typedef struct
{
	D3DXVECTOR3 pos;    //�ʒu
	D3DXVECTOR3 rot;    //����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fWidth;	    //����
	float fHeight;		//�c��
	int nType;          //���
	int nLife;          //����
	int nBocchiSelect;		//�ڂ��������̃Z���t�̑I��
	int nYakuzaSelect;		//�₭���̃Z���t�̑I��
	int nWorkSelect;		//��Ј��̃Z���t�̑I��

	int nEnemyNoneSelect;	//�G�̕��ʂ̃Z���t�̑I��
	int nEnemyDiscSelect;	//�G�̔������̃Z���t�̑I��
	bool bUse;          //�g�p���Ă��邩�ǂ���
	bool bDisp;         //�`�悷�邩�ǂ���

} Serif;

//�v���g�^�C�v�錾
void InitSerif(void);
void UninitSerif(void);
void UpdateSerif(void);
void DrawSerif(void);
int SetSerif(float fWidth, float fHeight, int nLife, int nType, D3DXVECTOR3 pos);
void SetPositionSerif(int nIdxGauge, D3DXVECTOR3 pos);
Serif *GetSerif(void);
//void GetSerifUse(int nIdxGauge, bool bUse);

#endif
