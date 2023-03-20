//===========================================================================
//
//�A�j���[�V����[animation.h]
//Author:�����V����	
//
//===========================================================================

#ifndef _ANIMATION_H_	//���̒�`������Ă��Ȃ�������
#define _ANIMATION_H_	//��d�C���N���[�h�h�~�̃}�N����`������

#include "main.h"

//�}�N����`
#define MAX_TEX	(2)
#define NUM_TEX	(128)			

//�ǂ̍\���̒�`
typedef struct
{
	D3DXVECTOR3 pos;			//���݂̈ʒu
	D3DXVECTOR3 posOld;			//�ߋ��̈ʒu
	D3DXVECTOR3 rot;			//����
	D3DXMATRIX g_mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;					//�g�p���Ă��邩�ǂ���
	int nType;					//�ǂ̎��
	int nCountAnim;				//
	int nPatternAnim;			//
	float fWidth;                 //��
	float fHeight;                //����
}Animation;

typedef enum
{
	ANIMATION_000,
	ANIMATION_001,
	ANIMATION_MAX
}ANIMATION_TYPE;

//�v���g�^�C�v�錾
void InitAnimation(void);
void UninitAnimation(void);
void UpdateAnimation(void);
void DrawAnimation(void);
void SetAnimation(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight);
Animation *GetAnimation(void);

#endif
