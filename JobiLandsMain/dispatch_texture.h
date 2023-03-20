//=============================================================================
//
// �h���e�N�X�`������ [dispatch_texture.h]
// Author : ����������
//
//=============================================================================
#ifndef _DISPATCHTEXTURE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _DISPATCHTEXTURE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�h���e�N�X�`���̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	int nCntDispatch;		//�h��
	bool bUse;				//�g�p���Ă邩
} DispatchTex;

//�v���g�^�C�v�錾
void InitDispatchTex(void);
void UninitDispatchTex(void);
void UpdateDispatchTex(void);
void DrawDispatchTex(void);
DispatchTex *GetDispatchTex(void);

#endif
