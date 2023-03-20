//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MESHFIELD_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MESHFIELD_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_FIELD	(256)	//�n�ʂ̍ő吔

//���b�V���t�B�[���h�̃^�C�v
typedef enum
{
	DRAWFIELD_TYPE_MAIN = 0,
	DRAWFIELD_TYPE_MAP,
	DRAWFIELD_TYPE_MAX
}DRAWFIELD_TYPE;

//���b�V���t�B�[���h�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nNumIndex;			//�C���f�b�N�X��
	int nNumVertex;			//���_��
	int nWidth;				//��������
	int nHeight;			//�c������
	float fWidthLen;		//���̒���
	float fHeightLen;		//�c�̒���
	bool bUse;				//�g�p���Ă��邩
	int nType;				//���
}MESHFIELD;

//�v���g�^�C�v�錾
void InitMeshField(void);
void UninitMeshField(void);
void UpdateMeshField(void);
void DrawMeshField(DRAWFIELD_TYPE nType);
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height, float fWidthLen, float fHeightLen, int nType);

#endif