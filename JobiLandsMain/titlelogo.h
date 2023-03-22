//=============================================================================
//
// �^�C�g�����S���� [titlelogo.h]
// Author : ��������
//
//=============================================================================
#ifndef _TITLELOGO_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TITLELOGO_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "model.h"

//=======================================
// �}�N����`
//=======================================

//=======================================
// �񋓌^��`(TITLE_LOGO_STATE)
//=======================================
typedef enum
{
	TITLE_LOGO_STATE_APPEAR = 0,		// �o�����
	TITLE_LOGO_STATE_NONE,				// �ʏ���
	TITLE_LOGO_STATE_MAX				// ���̗񋓌^�̑���
}TITLE_LOGO_STATE;

//=======================================
// �\���̒�`(TitleLogo)
//=======================================
typedef struct
{
	Model		aModel;		// ���f���̍\����
}TitleLogo;

//=======================================
//�v���g�^�C�v�錾
//=======================================
void InitTitleLogo(void);
void UninitTitleLogo(void);
void UpdateTitleLogo(void);
void DrawTitleLogo(void);

#endif