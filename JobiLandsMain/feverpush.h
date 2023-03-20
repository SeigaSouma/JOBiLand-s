//=============================================================================
//
// �t�B�[�o�[�v�b�V������ [feverpush.h]
// Author : ����������
//
//=============================================================================
#ifndef _FEVERPUSH_H_				//���̃}�N����`������Ă��Ȃ�������
#define _FEVERPUSH_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�t�B�[�o�[�v�b�V���̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	float fWidth;			//����
	float fHeight;			//�c��
	float fWidthMove;		//����
	float fHeightMove;		//�c��
	bool bUse;				//�g�p���Ă邩
} FeverPush;

//�v���g�^�C�v�錾
void InitFeverPush(void);
void UninitFeverPush(void);
void UpdateFeverPush(void);
void DrawFeverPush(void);
FeverPush *GetFeverPush(void);

#endif
