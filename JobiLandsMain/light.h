//==============================================================
//
//DirectX[light.h]
//Author:����������
//
//==============================================================
#ifndef _LIGHT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _LIGHT_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

typedef enum
{
	LIGHTTYPE_MAIN0 = 0,
	LIGHTTYPE_MAIN1,
	LIGHTTYPE_MAIN2,
	LIGHTTYPE_MAX
}LIGHTTYPE;

//�\����
typedef struct
{
	D3DLIGHT9 light;		//���̃��C�g�\����
	D3DXVECTOR3 vecDir;		//�ݒ�p�����x�N�g��
	D3DXVECTOR3 vecDirDest;	//�ڕW�̕����x�N�g��
	D3DXVECTOR3 vecDirDiff;	//�����x�N�g���̍���
	D3DXCOLOR DiffuseDest;	//�ڕW�̊g�U��
	D3DXCOLOR DiffuseDiff;	//�g�U���̍���
}LIGHT;

//�v���g�^�C�v�錾
void InitLight(void);
void UnInitLight(void);
void UpdateLight(void);

#endif
