//==============================================================
//
//DirectX[rotate.h]
//Author:����������
//
//==============================================================
#ifndef _ROTATE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _ROTATE_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�}�N����`
#define NUM_ROTATE			(3)			//���邮��̌�

//���邮��\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos[NUM_ROTATE];	//�ʒu
	D3DXVECTOR3 rot[NUM_ROTATE];	//����
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X

	bool bUse;						//���邮����g�p���Ă��邩
	bool bDisp;						//���邮���`�悷�邩
} Rotate;

//�v���g�^�C�v�錾
void InitRotate(void);
void UnInitRotate(void);
void UpdateRotate(void);
void DrawRotate(void);
void SetPositionRotate(int nIdxRotate, D3DXVECTOR3 pos);
int SetRotate(void);
Rotate *GetRotate(void);

#endif
