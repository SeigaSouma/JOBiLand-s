//=============================================================================
//
// �`���[�g���A�������o������ [tutorial_talk.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _TUTORIAL_TALK_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TUTORIAL_TALK_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�񋓌^��`
typedef enum
{
	TUTORIAL_TALK_VTX_FADE = 0,
	TUTORIAL_TALK_VTX_TEXT,
	TUTORIAL_TALK_VTX_MAX,
}TUTORIAL_TALK_VTX;

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int nStep;		//�i�s�x
	int nCntSet;	//�ݒ�̃J�E���g
	int nCntEnter;	//Enter�����܂ł̃N�[���^�C��
	bool bUse;		//�g�p���Ă��邩
	bool bSet;		//�ݒ肵�Ă��邩
}TutorialTalk;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	bool bUse;			//�g�p���Ă��邩
}TutorialOK;

//�v���g�^�C�v�錾
void InitTutorialTalk(void);
void UninitTutorialTalk(void);
void UpdateTutorialTalk(void);
void DrawTutorialTalk(void);
void SetTutorialTalk(void);
void SetTutorialOK(void);
TutorialTalk *GetTutorialTalk(void);

#endif