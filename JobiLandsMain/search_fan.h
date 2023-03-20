//==============================================================
//
//�G�̑O�ɏo����[search_fan.h]
//Author:����������
//
//==============================================================
#ifndef _SEARCH_FAN_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SEARCH_FAN_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//��̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;				//�ʒu
	D3DXVECTOR3 rot;				//����
	D3DXMATRIX mtxWorld;			//���[���h�}�g���b�N�X

	bool bUse;						//��̌`���g�p���Ă��邩
	bool bDisp;						//��̌`��`�悵�Ă��邩
} SearchFan;

//�v���g�^�C�v�錾
void InitSearchFan(void);
void UnInitSearchFan(void);
void UpdateSearchFan(void);
void DrawSearchFan(void);
void SetPositionSearchFan(int nIdxSearchFan, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
int SetSearchFan(void);
SearchFan *GetSearchFan(void);

#endif
