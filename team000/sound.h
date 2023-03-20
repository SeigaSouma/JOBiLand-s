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
	SOUND_LABEL_BGM_GAME,		// �Q�[��BGM
	SOUND_LABEL_BGM_FEVER,   // �t�B�[�o�[�˓���
	SOUND_LABEL_SE_PIYOPIYO,	// ���ɂЂ悱
	SOUND_LABEL_SE_GET,	 // ���������I�I
	SOUND_LABEL_SE_HYPNOSISEND,	// �Ö�����
	SOUND_LABEL_SE_ASIOTO,	// ����
	SOUND_LABEL_SE_KEIBIKYOUKA,	// �x������
	SOUND_LABEL_SE_DAKIMAKURA,  //����������
	SOUND_LABEL_SE_AMURO,	// �A�����s���܁`��
	SOUND_LABEL_SE_DHUHUHUHU,	// �f���t�t�t
	SOUND_LABEL_SE_ONMINO,	// ��g�̑O��
	SOUND_LABEL_SE_ORANGEKUN,	// �߂��Ă܂���܂���
	SOUND_LABEL_SE_SYUTUGEKI,	// �o���ł�����
	SOUND_LABEL_SE_YATTYAE,	// ������Ⴆ
	SOUND_LABEL_SE_CLICK,   //����͎g��Ȃ���I�I
	SOUND_LABEL_SE_WHEEL,   //����͎g��Ȃ���I�I
	SOUND_LABEL_BGM_HYPNOSIS,   //�Ö���
	SOUND_LABEL_SE_EVENTTEXT,   //�C�x���g�e�L�X�g
	SOUND_LABEL_SE_WATERGUN,   //����͎g��Ȃ���I�I
	SOUND_LABEL_SE_SHIBUKI,   // ���Ԃ�
	SOUND_LABEL_SE_KYUWAWA,   // ������`��
	SOUND_LABEL_SE_DOGAN,   // �h�K�[��
	SOUND_LABEL_SE_GEKIATSU,   // ���A�c�L�^�[�I
	SOUND_LABEL_SE_UC,   // ���j�R�[�[�[�[���I�I�I
	SOUND_LABEL_SE_MODORU,   // ���ǂ邨
	SOUND_LABEL_SE_SECURITY,   // �x������������
	SOUND_LABEL_SE_THEWORLD,   // �U�E���[���h
	SOUND_LABEL_SE_MANYPOS,   // ��ʔ����n�_
	SOUND_LABEL_SE_DHUFU,	// �f���t�P��
	SOUND_LABEL_SE_CUTIN,	// �J�b�g�C��
	SOUND_LABEL_SE_BACHIBACHI,	// �x�������o�`�o�`
	SOUND_LABEL_BGM_TUTORIAL,	// �`���[�g���A����BGM
	SOUND_LABEL_BGM_RESULT,	// ���U���g��BGM
	SOUND_LABEL_BGM_RANKING,	// �����L���O��BGM
	SOUND_LABEL_SE_IDOU,	// �����L���O�A���U���g�Ƃ��̐������ړ�����Ƃ��̉�
	SOUND_LABEL_SE_KOKE,	// ������
	SOUND_LABEL_SE_TUTORIALWINDOW,	// �`���[�g���A���E�B���h�E�ݒu
	SOUND_LABEL_SE_ENDCOUNTDOWN,	// �J�E���g�_�E��
	SOUND_LABEL_SE_FINISH,	// �I��
	SOUND_LABEL_SE_TUTORIALCLEAR,	//  �`���[�g���A���̃X�e�b�v�N���A
	SOUND_LABEL_SE_GAMECLEAR,	//  �Q�[���N���A
	SOUND_LABEL_SE_GAMEFAILED,	//  �Q�[���N���A����Ȃ�
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
