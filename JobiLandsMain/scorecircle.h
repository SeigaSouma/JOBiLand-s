//=============================================================================
//
// �Ռ��g���� [scorecircle.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _SCORECIRCLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SCORECIRCLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_SCORECIRCLE	(3)

//���b�V���t�B�[���h�\����
typedef struct
{
	D3DXVECTOR3 posOrigin;		//�N�_�̈ʒu
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ�
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 nor[256];	//�@���̌���
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
}SCORECIRCLE;

//�v���g�^�C�v�錾
void InitScoreCircle(void);
void UninitScoreCircle(void);
void UpdateScoreCircle(void);
void DrawScoreCircle(void);
void SetScoreCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nTexType);
void SetPositionScoreCircle(int nIdxWave, D3DXVECTOR3 pos);

#endif