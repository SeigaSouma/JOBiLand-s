//=============================================================================
//
// �������J�o�[���� [cutepillow.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _CUTEPILLOW_H_		//���̃}�N����`������Ă��Ȃ�������
#define _CUTEPILLOW_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_CUTEPILLOW	(512)	//�e�̍ő吔

//�񋓌^��`
typedef enum
{
	CUTEPILLOW_STATE_TROW = 0,	//�������
	CUTEPILLOW_STATE_FALL,		//�~�����
}CUTEPILLOW_STATE;

//�񋓌^��`
typedef enum
{
	PILLOWPARENT_PLAYER = 0,		//�v���C���[
	PILLOWPARENT_GARI,			//����
	PILLOWPARENT_CHIBI,			//�`�r
	PILLOWPARENT_MAX
}PILLOWPARENT;

//�������J�o�[�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 posOld;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	int nNumIndex;			//�C���f�b�N�X��
	int nNumVertex;			//���_��
	int nState;				//���
	int nType;				//���
	int nParent;			//�e
	bool bUse;				//�g�p���Ă��邩
	float fWidth;			//��
	float fHeight;			//�c
}Cutepillow;

//�v���g�^�C�v�錾
void InitCutepillow(void);
void UninitCutepillow(void);
void UpdateCutepillow(void);
void DrawCutepillow(void);
void SetCutepillow(D3DXVECTOR3 pos, D3DXVECTOR3 rot ,int nParent);
Cutepillow *GetCutepillow(void);

#endif