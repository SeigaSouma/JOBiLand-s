//=============================================================================
//
// ��ʔ�����UI���� [manyMORS.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MANY_MORS_H_
#define _MANY_MORS_H_

#include "main.h"

//��ʔ�����UI�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	int nType;			//���
	float fWidth;		//��
	float fHeight;		//����
	int nCntZoom;		//�傫���Ȃ鎞��
	bool bUse;			//�g�p���Ă��邩
}MANY_MORS;

//�v���g�^�C�v�錾
void InitManyMORS(void);
void UninitManyMORS(void);
void UpdateManyMORS(void);
void DrawManyMORS(void);
void SetManyMORS(int nType);
MANY_MORS *GetManyMORS(void);



#endif
