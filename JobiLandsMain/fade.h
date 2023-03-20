//==============================================================
//
//DirectX[fade.h]
//Author:����������
//
//==============================================================
#ifndef _FADE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _FADE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�t�F�[�h�̏��
typedef enum
{
	FADE_NONE = 0,		//�������Ă��Ȃ����
	FADE_IN,			//�t�F�[�h�C�����
	FADE_OUT,			//�t�F�[�h�A�E�g���
	FADE_VOICEOUT,      //�|�[�Y�̃t�F�[�h�A�E�g
	FADE_MAX
} FADE;

typedef enum
{
	FADE_VTX_FADE = 0,
	FADE_VTX_MAX
} FADE_VTX;

typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXCOLOR col;		//�F
	FADE nState;		//���
	float fWidth[FADE_VTX_MAX];		//��
	float fHeight[FADE_VTX_MAX];		//����
	int nCntZoom;		//�傫���Ȃ鎞��
	int nCntVoice;		//�{�C�X�̃J�E���^�[
	int nTexType;		//�e�N�X�`���̎��
}Fade;

//�v���g�^�C�v�錾
void InitFade(MODE modeNext);
void UninitFade(void);
void UpdateFade(void);
void DrawFade(void);
void SetFade(MODE modeNext);
void SetVoiceFade(MODE modeNext);
FADE GetFade(void);

#endif
