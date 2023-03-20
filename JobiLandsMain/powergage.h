//==============================================================
//
//DirectX[powergage.h]
//Author:�����V����
//
//==============================================================
#ifndef _POWERGAGE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _POWERGAGE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//���̓Q�[�W�̗񋓌^
typedef enum
{
	POWERGAGE_VTX_FRAM = 0,
	POWERGAGE_VTX_NEET,
	POWERGAGE_VTX_ENEMY,
	POWERGAGE_VTX_TEX,
	POWERGAGE_VTX_MAX
}POWERGAGE_VTX;

//���̓Q�[�W�̍\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	int population;		//�l��
	float result;		//�䗦�v�Z����
	bool bUse;			//�g�p����Ă��邩�ǂ���
}PowerGage;

//�v���g�^�C�v�錾
void InitPowerGage(void);
void UninitPowerGage(void);
void UpdatePowerGage(void);
void DrawPowerGage(void);
PowerGage *GetPowerGage(void);

#endif
