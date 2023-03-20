//==============================================================
//
//DirectX[nofade.h]
//Author:�匴�叫
//
//==============================================================
#ifndef _NOFADE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _NOFADE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�t�F�[�h�̏��
typedef enum
{
	NOFADE_OUT = 0,			//�t�F�[�h�A�E�g���
	NOFADE_IN,			//�t�F�[�h�C�����
	NOFADE_MAX
} NOFADESTATE;

//�t�F�[�h���Ȃ��t�F�[�h�̍\����
typedef struct
{
	NOFADESTATE nState;
	D3DXCOLOR colorNoFade;		//�|���S���i�t�F�[�h�j�̐F
	int nCounter;               //�t�F�[�h�ɓ���܂ł̎���
	bool bUse;
} NOFADE;

//�v���g�^�C�v�錾
void InitNoFade(void);
void UninitNoFade(void);
void UpdateNoFade(void);
void DrawNoFade(void);
void SetNoFade(int nCounter);
NOFADE GetNoFade(void);

#endif
