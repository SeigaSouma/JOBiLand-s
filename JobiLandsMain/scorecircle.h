//=============================================================================
//
// �Ռ��g���� [impactwave.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _IMPACTWAVE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _IMPACTWAVE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_IMPACTWAVE	(256)

//�񋓌^��`
typedef enum
{
	INPACTWAVE_TYPE_BLACK = 0,
	INPACTWAVE_TYPE_BLACK2,
	INPACTWAVE_TYPE_ORANGE,
	INPACTWAVE_TYPE_ORANGE2,
	INPACTWAVE_TYPE_BLUE,
	INPACTWAVE_TYPE_PURPLE,
	INPACTWAVE_TYPE_GREEN,
	INPACTWAVE_TYPE_PURPLE2,
	INPACTWAVE_TYPE_PURPLE3,
	INPACTWAVE_TYPE_PINK,
	INPACTWAVE_TYPE_MAX
}INPACTWAVE_TYPE;

//���b�V���t�B�[���h�\����
typedef struct
{
	D3DXVECTOR3 posOrigin;		//�N�_�̈ʒu
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ�
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 nor[MAX_IMPACTWAVE];	//�@���̌���
	D3DXCOLOR col;			//�F
	D3DXCOLOR colOrigin;	//���̐F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fOutWidth;		//����(�O)
	float fInWidth;			//��(��)
	float fRotWidth;		//1������������̊p�x����
	float fHeight;
	float fMove;			//�L���鑬�x
	int nLife;				//����
	int nMaxLife;			//�ő����
	int nTexType;			//�e�N�X�`�����
	bool bUse;				//�g�p���Ă��邩
}IMPACTWAVE;

//�v���g�^�C�v�錾
void InitImpactWave(void);
void UninitImpactWave(void);
void UpdateImpactWave(void);
void DrawImpactWave(void);
int SetImpactWave(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fMove, int nTexType);
void SetPositionImpactWave(int nIdxWave, D3DXVECTOR3 pos);

#endif