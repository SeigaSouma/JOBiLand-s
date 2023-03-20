//=============================================================================
//
// �G�f�B�b�g���� [edit.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _EDIT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _EDIT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "model.h"

//�G�f�B�b�g�̍\����
typedef struct
{
	D3DXVECTOR3 pos;	//���݂̈ʒu
	D3DXVECTOR3 rot;	//����
	int nNowModelType;	//���̃��f�����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nShadow;		//�e�g�����ǂ���[0��1]
	bool bUse;			//�g�p���Ă��邩

	Model aXLoadMeshData[MAX_MODEL];	//X�t�@�C���œǂݍ��񂾃f�[�^
}Edit;

//�v���g�^�C�v�錾
void InitEdit(void);
void UninitEdit(void);
void UpdateEdit(void);
void DrawEdit(void);
Edit *GetEdit(void);

#endif