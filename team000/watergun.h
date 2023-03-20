//=============================================================================
//
// �G�t�F�N�g���� [watergun.h]
// Author : �匴�叫
//
//=============================================================================

#ifndef _WATERGUN_H_		//���̃}�N����`������Ă��Ȃ�������
#define _WATERGUN_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�C���N���[�h
#include "main.h"

//�}�N����`
#define WATERBULLET_LIFE		(30)

//�񋓌^��`
typedef enum
{
	WATERBULLETTYPE_NORMAL = 0,	//�ʏ�G�t�F�N�g
	WATERBULLETTYPE_SMOKE,		//���G�t�F�N�g
	WATERBULLETTYPE_SMOKEBLACK,	//����
	WATERBULLETTYPE_BLACK,		//���G�t�F�N�g
	WATERBULLETTYPE_HART,        //�n�[�g�G�t�F�N�g
	WATERBULLETTYPE_MAX
}WATERBULLETTYPE;

//�G�t�F�N�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXCOLOR col;		//�F
	float fRadius;		//���a
	float fMaxRadius;	//�ő唼�a
	float fHeight;      //����
	float fWidth;       //��
	int nLife;			//����
	int nMaxLife;		//�ő����(�Œ�)
	int moveType;		//�ړ��̎��
	int nType;			//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
	bool bAddAlpha;		//���Z�����̔���
}Watergun;

//�v���g�^�C�v�錾
void InitWatergun(void);
void UninitWatergun(void);
void UpdateWatergun(void);
void DrawWatergun(void);
void SetWatergun(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType);
int GetWatergunNum(void);
bool GetWatergunUse(void);

#endif
