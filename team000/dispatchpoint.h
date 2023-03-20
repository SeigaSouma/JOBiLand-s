//=============================================================================
//
// ���b�V���V�����_�[���� [dispatchpoint.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _DISPATCHPOINT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _DISPATCHPOINT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//���b�V���V�����_�[�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nNumIndex;			//�C���f�b�N�X��
	int nNumVertex;			//���_��
	bool bUse;				//�g�p���Ă��邩
}DISPATCHPOINT;

//�v���g�^�C�v�錾
void InitDispatchPoint(void);
void UninitDispatchPoint(void);
void UpdateDispatchPoint(void);
void DrawDispatchPoint(void);
DISPATCHPOINT *GetDispatchPoint(void);
#endif