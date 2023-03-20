//=============================================================================
//
// ���u���� [titleselect.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _TITLESELECT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TITLESELECT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "model.h"

//�}�N����`
#define MAX_TITLESELECT	(2)

//�Q�O�̍\����
typedef struct
{
	int nFloating;	//���V�̌���
	int nTurnCounter;	//��]�����񐔃J�E���g
	float fPosDiffX;	//�ړI�̈ʒu�Ƃ̍���
	float fPosDiffY;	//�ړI�̈ʒu�Ƃ̍���
	Model aModel;					//���f���̍\����
}TitleSelect;

//�v���g�^�C�v�錾
void InitTitleSelect(void);
void UninitTitleSelect(void);
void UpdateTitleSelect(void);
void UpdateTutorialTitleSelect(void);
void DrawTitleSelect(void);
TitleSelect *GetTitleSelect(void);
void SetTitleSelect(D3DXVECTOR3 pos, int nModelType);

#endif