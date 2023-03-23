//=============================================================================
//
// ���f������ [model.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MODEL_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MODEL_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define MAX_MODEL	(312)	//���f���̍ő吔

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 posOrigin;	//���̈ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 rotOrigin;	//���̌���
	D3DXVECTOR3 move;		//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	int nShadow;	//�e���g�����ǂ���

	int nNumVtx;			//���_��
	D3DXVECTOR3 vtxMin;		//���f���̍ŏ��l
	D3DXVECTOR3	vtxMax;		//���f���̍ő�l

	int nState;			//���
	int nCntState;		//��ԃJ�E���^�[
	int nType;			//�ǂݍ��ރ��f���̎��
	int nParent;		//�e�̔ԍ�
	int nWeapon;		//���킩�ǂ���
	bool bUse;			//�g�p���Ă��邩
	bool bDisp;
	
	bool bSkelton;	//�����ɂȂ邩�ǂ���

	Model aXLoadMeshData;	//X�t�@�C���œǂݍ��񂾃f�[�^
}ModelModel;

//�v���g�^�C�v�錾
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(int nType);
ModelModel *GetModel(void);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nShadow);
int GetModelFileNum(void);
Model *GetXLoadData(void);
int GetModelNum(void);

#endif