//=============================================================================
//
// �ŏ��̉�b���� [start_talk.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _START_TALK_H_		//���̃}�N����`������Ă��Ȃ�������
#define _START_TALK_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�񋓌^��`
typedef enum
{
	START_TALK_VTX_FADE = 0,
	START_TALK_VTX_TEXT1,
	START_TALK_VTX_TEXT2,
	START_TALK_VTX_TEXT3,
	START_TALK_VTX_TEXT4,
	START_TALK_VTX_TEXT5,
	START_TALK_VTX_SKIP,
	START_TALK_VTX_MAX,
}START_TALK_VTX;

//�\���̂̒�`
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	bool bUse;	//�g�p���Ă��邩
}StartTalk;

//�v���g�^�C�v�錾
void InitStartTalk(void);
void UninitStartTalk(void);
void UpdateStartTalk(void);
void DrawStartTalk(void);
void SetStartTalk(void);
void SetTartgetWindow(void);
StartTalk *GetStartTalk(void);

#endif