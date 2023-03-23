//=============================================================================
//
// �R���g���[���p�l������ [controllpanel.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _CONTROLLPANEL_H_				//���̃}�N����`������Ă��Ȃ�������
#define _CONTROLLPANEL_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�R���g���[���p�l���̗񋓌^
typedef enum
{
	CONTROLLPANEL_VTX_FRAM = 0,
	CONTROLLPANEL_VTX_UPBUTTON,
	CONTROLLPANEL_VTX_DWBUTTON,
	CONTROLLPANEL_VTX_MAX
}CONTROLLPANEL_VTX;

//�R���g���[���p�l���̍\����
typedef struct
{
	UI_2D aUI[CONTROLLPANEL_VTX_MAX];	//UI���
}ControllPanel;

//�v���g�^�C�v�錾
void InitControllPanel(void);
void UninitControllPanel(void);
void UpdateControllPanel(void);
void DrawControllPanel(void);
ControllPanel *GetControllPanel(void);

#endif
