//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// �T�E���h�ꗗ
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,	// �^�C�g��
	SOUND_LABEL_BGM_GAME,		// �Q�[��
	SOUND_LABEL_BGM_RESULT,		// ���U���g
	SOUND_LABEL_BGM_RANKING,	// �����L���O
	SOUND_LABEL_BGM_TUTORIAL,	// �`���[�g���A��
	SOUND_LABEL_SE_CURSOR,		// �J�[�\���ړ�
	SOUND_LABEL_SE_ARMMOVE,		//���{�b�g�̘r��������
	SOUND_LABEL_SE_HIT,			//���{�b�g�̘r�������鉹
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void VolumeChange(float fVolume);
int GetVolume(void);

#endif
