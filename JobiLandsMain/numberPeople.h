//==============================================================
//
//�\������l��[numberPeople.h]
//Author:����������
//
//==============================================================
#ifndef _NUMBERPEOPLE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _NUMBERPEOPLE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�l
typedef enum
{
	PEOPLESTATE_NEET = 0,	//�j�[�g
	PEOPLESTATE_ENEMY,		//�G
	PEOPLESTATE_MAX
}PEOPLESTATE;

//�l���̍\����
typedef struct
{
	D3DXVECTOR3 pos[4];		//�ʒu
	float aDestTexV[4];		//�ڕW��V���W
	float aTexV[4];			//���݂�V���W
	int nPeople;			//�l��
	bool bUse;				//�g�p���Ă��邩
}People;

//�v���g�^�C�v�錾
void InitPeople(void);
void UninitPeople(void);
void UpdatePeople(void);
void DrawPeople(void);

#endif
