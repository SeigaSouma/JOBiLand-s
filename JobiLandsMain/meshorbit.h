//=============================================================================
//
// �O�Տ��� [meshorbit.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _ORBIT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _ORBIT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_ORBIT	(1)	//�ǂ̍ő吔

//�}�N����`
#define POS_ORBIT_HEIGHT	(250.0f)
#define POS_ORBIT_WIDTH	(0.0f)
#define OFFSET_POINT		(2)
#define ALL_POINT		(OFFSET_POINT * 15)

//���b�V���t�B�[���h�\����
typedef struct
{
	D3DXMATRIX *pMtxparent;				//�e�̃}�g���b�N�X�̃|�C���^
	D3DXVECTOR3 aOffSet[OFFSET_POINT];	//���[�̃I�t�Z�b�g
	D3DXCOLOR aCol[OFFSET_POINT];		//���[�̊�̐F
	D3DXMATRIX amtxWorldPoint[OFFSET_POINT];	//���[�̃��[���h�}�g���b�N�X
	D3DXVECTOR3 aPosPoint[ALL_POINT];	//�v�Z��̊e���_���W
	D3DXCOLOR aColPoint[ALL_POINT];		//�e���_�̐F
}ORBIT;

//�v���g�^�C�v�錾
void InitMeshOrbit(void);
void UninitMeshOrbit(void);
void UpdateMeshOrbit(void);
void DrawMeshOrbit(void);
void SetMeshOrbit(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height);
ORBIT *GetMeshOrbit(void);
LPDIRECT3DVERTEXBUFFER9 GetBuffMeshOrbit(void);

#endif