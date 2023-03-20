//=============================================================================
//
// �`���[�g���A�������o������ [tutorial_window.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _TUTORIAL_WINDOW_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_WINDOW_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�񋓌^��`
typedef enum
{
	TUTORIAL_WINDOW_VTX_WINDOW = 0,
	TUTORIAL_WINDOW_VTX_TEXT,
	TUTORIAL_WINDOW_VTX_SKIP,
	TUTORIAL_WINDOW_VTX_MAX,
}TUTORIAL_WINDOW_VTX;

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	//D3DXVECTOR3 posSkip;	//�ʒu
	int nStep;		//�i�s�x
	int nOldStep;	//�i�s�x
	float fAlpha;	//�s�����x
	int nWaitTime;	//�҂�����
	int nIdxMoveLog;	//�������S�̃C���f�b�N�X�ԍ�
	bool bMove;		//���������̔���
}TutorialWindow;

//�v���g�^�C�v�錾
void InitTutorialWindow(void);
void UninitTutorialWindow(void);
void UpdateTutorialWindow(void);
void DrawTutorialWindow(void);
TutorialWindow *GetTutorialWindow(void);

#endif