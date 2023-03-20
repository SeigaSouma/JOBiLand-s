//=============================================================================
//
// ��ʔ�����UI���� [manyUI.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MANY_UI_H_
#define _MANY_UI_H_

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
}MANY_UI;

//�v���g�^�C�v�錾
void InitManyUI(void);
void UninitManyUI(void);
void UpdateManyUI(void);
void DrawManyUI(void);
void SetManyUI(int nType);
MANY_UI *GetManyUI(void);



#endif
