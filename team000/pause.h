//=============================================================================
//
// �|�[�Y���� [pause.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _PAUSE_H_	//���̃}�N����`������Ă��Ȃ�������
#define _PAUSE_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

//�񋓌^��`
//�|�[�Y���j���[
typedef enum
{
	PAUSEMENU_CONTINUE = 0,		//�|�[�Y�̉���
	PAUSEMENU_RETRY,			//���g���C
	PAUSEMENU_LEVELSELECT,		//���x���Z���N�g�ɖ߂�	
	PAUSEMENU_MAX
}PAUSEMENU;

typedef enum
{
	PAUSEVTX_FADE = 0,		//������
	PAUSEVTX_WINDOW,		//���j���[�E�B���h�E
	PAUSEVTX_CONTINUE,		//�Q�[���ɖ߂�
	PAUSEVTX_RETRY,			//���g���C
	PAUSEVTX_QUIT,			//�^�C�g���ɖ߂�	
	PAUSEVTX_MAX
}PAUSEVTX;

//�|�[�Y�̏��
typedef enum
{
	PAUSESTATE_SELECT = 0,	//�I�ׂ���
	PAUSESTATE_FADE,		//�t�F�[�h��
	PAUSESTATE_MAX
}PAUSESTATE;

//�v���g�^�C�v�錾
void InitPause(void);
void UninitPause(void);
void UpdatePause(void);
void DrawPause(void);
void UpdateSelect(int PauseSelect);
void SetPause(void);

#endif