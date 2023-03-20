//==============================================================
//
//��[flag.h]
//Author:����������
//
//==============================================================
#ifndef _FLAG_H_		//���̃}�N����`������Ă��Ȃ�������
#define _FLAG_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//���̍\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;			//�ʒu
	D3DXMATRIX mtxWorld;		//���[���h�}�g���b�N�X

	int nCntAnim;				//�A�j���[�V�����ύX�J�E���^�[
	int nPatternAnim;			//�A�j���[�V����No.
	bool bUse;					//�g�p���Ă��邩
} Flag;

//�v���g�^�C�v�錾
void InitFlag(void);
void UnInitFlag(void);
void UpdateFlag(void);
void DrawFlag(void);
void SetPositionFlag(int nIdxFlag, D3DXVECTOR3 pos);
int SetFlag(void);
Flag *GetFlag(void);

#endif
