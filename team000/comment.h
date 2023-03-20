//=============================================================================
//
// �R�����g���� [comment.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _COMMENT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _COMMENT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_NICOCOME	(256)	//�j�R�R���̂�����������

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 move;	//�ړ���
	int nType;	//���
	int nTexV;	//�e�N�X�`���̉������ڂ�
	int nNumIndex;	//�C���f�b�N�X�ԍ�
	bool bUse;	//�g�p���Ă��邩
}CommnetInfo;

typedef struct
{
	bool bUse;		//�g�p���Ă��邩
	int nCntSpawn;	//�o���܂ł̃J�E���^�[
	int nNumSpawn;	//�o��������
	CommnetInfo aInfo[MAX_NICOCOME];
}Commnet;

//�v���g�^�C�v�錾
void InitCommnet(void);
void UninitCommnet(void);
void UpdateCommnet(void);
void DrawCommnet(void);
void SetComment(void);
Commnet *GetCommnet(void);

#endif