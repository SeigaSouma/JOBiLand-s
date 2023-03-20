//==============================================================
//
//�C�x���g�T�C���̉��n[iventradar.h]
//Author:�匴�叫
//
//==============================================================
#ifndef _IVENTRADAR_H_				//���̃}�N����`������Ă��Ȃ�������
#define _IVENTRADAR_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�l
typedef enum
{
	RADARTYPE_MANYPEOPLE = 0,	//��ʔ�����
	RADARTYPE_FEWPEOPLE,		//��ʔ����Ȃ�
	RADARTYPE_MAX
}RADARTYPE;

//�l���̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	RADARTYPE type;			//���
	D3DXCOLOR col;          //�F
	float fWidth;			//����
	float fHeight;			//�c��
	bool bUse;				//�g�p���Ă��邩
}IventRadar;

//�v���g�^�C�v�錾
void InitIventRadar(void);
void UninitIventRadar(void);
void UpdateIventRadar(void);
void DrawIventRadar(void);
void SetIventRadar(void);
IventRadar *GetIventRadar(void);

#endif
