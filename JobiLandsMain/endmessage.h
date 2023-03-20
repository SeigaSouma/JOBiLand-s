//==============================================================
//
//�I���O�ɏo�����b�Z�[�W[endmessage.h]
//Author:�匴�叫
//
//==============================================================
#ifndef _ENDMESSAGE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _ENDMESSAGE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�l
typedef enum
{
	MESSAGETYPE_NONE = 0,	//���ʂɏo����
	MESSAGETYPE_BIG,		//���ɏo����
	MESSAGETYPE_MAX
}MESSAGETYPE;

//�~�j�}�b�v�t���[���̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXCOLOR col;          //�F
	MESSAGETYPE ntype;        //���
	int nCounter;           //������������܂ł̃J�E���g
	int nCntZoom;           //�������傫���Ȃ�J�E���g
	float fWidth;			//����
	float fHeight;			//�c��
	bool bUse;              //�g�p���Ă��邩�ǂ���
}Endmessage;


//�v���g�^�C�v�錾
void InitEndmessage(void);
void UninitEndmessage(void);
void UpdateEndmessage(void);
void DrawEndmessage(void);
Endmessage *GetEndmessage(void);


void SetEndmessage(D3DXVECTOR3 pos,int nCounter);


#endif