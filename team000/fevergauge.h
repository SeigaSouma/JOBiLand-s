//=============================================================================
//
// �t�B�[�o�[�Q�[�W���� [fevergauge.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _FEVERGAUGE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _FEVERGAUGE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//���̓Q�[�W�̗񋓌^
typedef enum
{
	FEVERGAUGE_VTX_FRAM = 0,
	FEVERGAUGE_VTX_TEX,
	FEVERGAUGE_VTX_GAUGE,
	FEVERGAUGE_VTX_MAX
}FEVERGAUGE_VTX;

//���̓Q�[�W�̍\����
typedef struct
{
	bool bFever;		//�t�B�[�o�[���ǂ���
	bool bUseButton;	//�{�^������������
	UI_2D aUI[FEVERGAUGE_VTX_MAX];	//UI���
}FeverGauge;

//�v���g�^�C�v�錾
void InitFeverGauge(void);
void UninitFeverGauge(void);
void UpdateFeverGauge(void);
void DrawFeverGauge(void);
FeverGauge *GetFeverGauge(void);

#endif
