//=============================================================================
//
// ���b�V���V�����_�[���� [manypoint.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MANYPOINT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MANYPOINT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define POS_MANYPOINT	(200.0f)

//���b�V���V�����_�[�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nNumIndex;			//�C���f�b�N�X��
	int nNumVertex;			//���_��
	bool bUse;				//�g�p���Ă��邩
}MANYPOINT;

//�v���g�^�C�v�錾
void InitManyPoint(void);
void UninitManyPoint(void);
void UpdateManyPoint(void);
void DrawManyPoint(void);
MANYPOINT *GetManyPoint(void);
#endif