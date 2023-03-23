//=============================================================================
//
// �v���C���[���� [player.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _PLAYER_H_		//���̃}�N����`������Ă��Ȃ�������
#define _PLAYER_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define JUMP		(20.0f)	//�W�����v�ړ���
#define GRAVITY		(1.0f)	//�d��
#define PENLIGHT_RADIUS	(300.0f)
#define PENLIGHT_RADIUS_MOVE	(200.0f)
#define PENLIGHT_ANGLE	(D3DX_PI * 0.18f)
#define PENLIGHT_ANGLE_MOVE	(D3DX_PI * 0.1f)
#define MAX_WAVEINDEX	(16)

#include "main.h"
#include "model.h"
#include "motion.h"

//���f���̎��
typedef enum
{
	PLAYERPARTS_BODY = 0,	//��
	PLAYERPARTS_HEAD,		//��
	PLAYERPARTS_R_NINO,		//�E��̘r
	PLAYERPARTS_R_UDE,		//�E�r
	PLAYERPARTS_R_HAND,		//�E��
	PLAYERPARTS_R_MOMO,		//�E����
	PLAYERPARTS_R_LEG,		//�E�ӂ���͂�
	PLAYERPARTS_R_ASIKUBI,	//�E����
	PLAYERPARTS_R_FOOT,		//�E��
	PLAYERPARTS_L_NINO,		//����̘r
	PLAYERPARTS_L_UDE,		//���r
	PLAYERPARTS_L_HAND,		//����
	PLAYERPARTS_L_MOMO,		//������
	PLAYERPARTS_L_LEG,		//���ӂ���͂�
	PLAYERPARTS_L_ASIKUBI,	//������
	PLAYERPARTS_L_FOOT,		//����
	PLAYERPARTS_HANDGUN,
	PLAYERPARTS_WOODSTICK,
	PLAYERPARTS_STONE,
	PLAYERPARTS_MAX
}PLAYERPARTS;

typedef enum
{
	PLAYERMOTION_DEF = 0,	//�j���[�g�������[�V����
	PLAYERMOTION_UP,	//�ł��グ���[�V����
	PLAYERMOTION_DOWN,	//���������[�V����
	PLAYERMOTION_MAX
}PLAYERMOTION;

//�v���C���[�̏��
typedef enum
{
	PLAYERSTATE_NONE = 0,	//�������Ă��Ȃ����
	PLAYERSTATE_DMG,		//�_���[�W���
	PLAYERSTATE_INVINCIBLE,	//�������
	PLAYERSTATE_MAX
}PLAYERSTATE;

//�v���C���[�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 vecMove;	//�ړ��x�N�g��
	D3DXVECTOR3 vecToPos;	//�x�N�g���̒���
	float fRotDiff;			//�����̍���
	float fRotDest;			//�����̍���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	int nIdxHPGauge;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
	int nState;				//���
	int nCntState;			//��ԃJ�E���g
	bool bUse;				//�g�p���Ă��邩
	bool bDisp;				//�`�悵�Ă��邩
	float fRadius;			//���a

	//���[�V�����n
	bool bMove;				//�ړ����Ă��邩
	bool bJump;				//�W�����v���Ă��邩
	bool bATK;				//�U�����Ă邩
	int nCntWalk;			//�ړ��̃J�E���g
	int nCntPenlight;		//�Ö��̃J�E���g
	int nCntDis;            //�h���̃J�E���g
	int nCntReturn;         //�A�҂̃J�E���g

	Model aModel[MAX_MODEL];	//�p�[�c���
	MotionData aMotionData[MAX_MOTION];		//�e���[�V�����̏��
	Motion aMotion;			//���[�V�����̑S�̏��
}Player;

//�v���g�^�C�v�錾
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void SetPlayer(void);
D3DXMATRIX GetParentMatrix(int nIdxParts);
void HitPlayer(void);

#endif