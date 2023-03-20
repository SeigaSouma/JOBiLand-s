//=============================================================================
//
// �I��5�b�O�J�E���g���� [endcountdown.h]
// Author : �匴�叫
//
//=============================================================================
#ifndef _ENDCOUNTDOWN_H_
#define _ENDCOUNTDOWN_H_

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
} ENDCOUNTDOWN;

//�v���g�^�C�v�錾
void InitEndCountDown(void);
void UninitEndCountDown(void);
void UpdateEndCountDown(void);
void DrawEndCountDown(void);
void SetEndCountDown(int nType);
ENDCOUNTDOWN *GetEndCountDown(void);



#endif
