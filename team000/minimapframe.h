//==============================================================
//
//�~�j�}�b�v�̃t���[��[minimapframe.h]
//Author:�匴�叫
//
//==============================================================
#ifndef _MINIMAPFRAME_H_				//���̃}�N����`������Ă��Ȃ�������
#define _MINIMAPFRAME_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�l
//typedef enum
//{
//	RADARTYPE_MANYPEOPLE = 0,	//��ʔ�����
//	RADARTYPE_FEWPEOPLE,		//��ʔ����Ȃ�
//	RADARTYPE_MAX
//}RADARTYPE;

//�~�j�}�b�v�t���[���̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	float fWidth;			//����
	float fHeight;			//�c��
	bool bUse;              //�g�p���Ă��邩�ǂ���
}Minimapframe;

//�v���g�^�C�v�錾
void InitMinimapframe(void);
void UninitMinimapframe(void);
void UpdateMinimapframe(void);
void DrawMinimapframe(void);
Minimapframe *GetMinimapframe(void);

#endif
