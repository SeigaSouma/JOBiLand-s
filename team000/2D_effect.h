//=============================================================================
//
// �G�t�F�N�g���� [2D_effect.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _EFFECT_2D_H_		//���̃}�N����`������Ă��Ȃ�������
#define _EFFECT_2D_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�C���N���[�h
#include "main.h"

//�}�N����`
#define EFFECT_2D_LIFE		(30)

//�G�t�F�N�g�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXCOLOR col;		//�F
	float fRadius;		//���a
	float fMaxRadius;	//�ő唼�a
	int nLife;			//����
	int nMaxLife;		//�ő����(�Œ�)
	int moveType;		//�ړ��̎��
	int nType;			//���
	bool bUse;			//�g�p���Ă��邩�ǂ���
	bool bAddAlpha;		//���Z�����̔���
}Effect_2D;

//�v���g�^�C�v�錾
void InitEffect_2D(void);
void UninitEffect_2D(void);
void UpdateEffect_2D(void);
void DrawEffect_2D(void);
void SetEffect_2D(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType);
int GetEffect_2DNum(void);
bool GetEffect_2DUse(void);

#endif