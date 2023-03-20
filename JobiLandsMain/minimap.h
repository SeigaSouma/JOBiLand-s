//=============================================================================
//
// �|���S������ [minimap.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MINIMAP_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MINIMAP_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�|���S���\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 vtxMax;		//�ő�T�C�Y
	D3DXVECTOR3 vtxMin;		//�ŏ��T�C�Y
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p����Ă��邩�ǂ���

}MINIMAP;

//�v���g�^�C�v�錾
void InitMinimap(void);
void UninitMinimap(void);
void UpdateMinimap(void);
void DrawMinimap(void);
void SetMiniMap(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin);

#endif