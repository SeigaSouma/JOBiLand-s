//==========================================
//
//�J�b�g�C������
//Author:�����V���� [cutin.h]
//
//==========================================

#ifndef _CUTIN_H_	//���̒�`������Ă��Ȃ�������
#define _CUTIN_H_	//��d�C���N���[�h�h�~�̃}�N����`������

//�J�b�g�C���\����
typedef struct
{
	D3DXVECTOR3 move;	//�ړ���
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����

	D3DXCOLOR col;		//�F

	float fHeight;		//����
	float fWidth;		//��

	int nType;		//���

	bool bUse;		//�g�p����Ă��邩�ǂ���

}CUTIN;

//�}�N����`
#define MAX_TEX	(2)			//�e�N�X�`���ő吔
#define IMAGE	(0)			//�摜
#define SENTENCE	(1)		//����

#define TEXT_X_CUTIN	(640.0f)	//��		
#define TEXT_Y_CUTIN	(350.0f)	//����

#define TEXT_X_TEXT		(300.0f)	//��
#define TEXT_Y_TEXT		(300.0f)	//����


//�v���g�^�C�v�錾
void InitCutin(void);
void UninitCutin(void);
void UpdateCutin(void);
void DrawCutin(void);
void SetCutin(D3DXVECTOR3 pos, int nType, float fHeight, float fWidth);
CUTIN *GetCutin(void);

#endif