//==========================================================================
//
//�C�x���g�T�C������
//Author:�����V����	[iventsign.h]
//
//==========================================================================
#ifndef _IVENTSIGN_H_
#define _IVENTSIGN_H_

#include"main.h"

//�C�x���g�T�C���̍\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 Iventpos;	//�C�x���g�ʒu
	D3DXVECTOR3 rot;	//����
	D3DXVECTOR3 Dest;	//�ڕW�̈ړ�����
	bool bUse;			//�g�p���Ă��邩�ǂ���
}IventSign;

//�v���g�^�C�v�錾
void InitIventSign(void);
void UninitIventSign(void);
void UpdateIventSign(void);
void DrawIventSign(void);
IventSign *GetIventSign(void);
void SetIventSign(D3DXVECTOR3 pos);
#endif

