//==============================================================
//
//�\���̔w�i[ground.h]
//Author:����������
//
//==============================================================
#ifndef _GROUND_H_				//���̃}�N����`������Ă��Ȃ�������
#define _GROUND_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�l���̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	float fHeight;			//�c��
	float fWidth;			//����
	bool bUse;				//�g�p���Ă��邩
}Ground;

//�v���g�^�C�v�錾
void InitGround(void);
void UninitGround(void);
void UpdateGround(void);
void DrawGround(void);

#endif
