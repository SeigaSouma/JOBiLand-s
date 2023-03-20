//<====================================================
//
//�C�x���g�E�B���h�E����(EventWindow.h)
//Author kazuki watanabe
//
//<====================================================
#ifndef  _EVENTWINDOW_H_
#define _EVENTWINDOW_H_

#include "main.h"

#define NUM_E_WINDOW (1)	//�C�x���g�E�B���h�E�̐�

//�C�x���g�E�B���h�E�̗񋓌^
typedef enum
{
	EVENTWINDOW_VTX_RED = 0,	//�Ԗ�
	EVENTWINDOW_VTX_FRAM,		//�x���̘g
	EVENTWINDOW_VTX_TEXT,		//�x���̃e�L�X�g
	EVENTWINDOW_VTX_LINEUP,		//�x���̃��C����
	EVENTWINDOW_VTX_LINEDW,		//�x���̃��C����
	EVENTWINDOW_VTX_MAX
}EVENTWINDOW_VTX;

typedef enum
{
	EVENTWINDOW_STEP_RED = 0,	//�Ԗ��E�B���E�B��
	EVENTWINDOW_STEP_FRAMMOVE,	//�g�ړ�
	EVENTWINDOW_STEP_TEXTLINE,	//�e�L�X�g&���C���ړ�
	EVENTWINDOW_STEP_OUT,		//�ޏ�
}EVENTWINDOW_STEP;


//�C�x���g�E�B���h�E�̍\����
typedef struct
{
	bool bUse;		//�g�p���Ă��邩
	int nStep;		//�X�e�b�v
	UI_2D aUI[EVENTWINDOW_VTX_MAX];	//UI���
}EVENTWINDOW;

//�v���g�^�C�v�錾
void InitEventWindow(void);
void UninitEventWindow(void);
void UpdateEventWindow(void);
void DrawEventWindow(void);
void SetEventWindow(void);
EVENTWINDOW *GetEventWindow(void);



#endif
