//=============================================================================
//
// �I�� [finish.h]
// Author : �匴�叫
//
//=============================================================================
#ifndef _FENISH_H_
#define _FENISH_H_

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
} FENISH;

//�v���g�^�C�v�錾
void InitFinish(void);
void UninitFinish(void);
void UpdateFinish(void);
void DrawFinish(void);
void SetFinish(int nType);
FENISH *GetFinish(void);



#endif
