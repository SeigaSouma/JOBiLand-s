//=============================================================================
//
// �|�[�Y���� [pause.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _TUTORIALSKIP_H_	//���̃}�N����`������Ă��Ȃ�������
#define _TUTORIALSKIP_H_	//��d�C���N���[�h�h�~�̃}�N�����`����

//�񋓌^��`
//�|�[�Y���j���[
typedef enum
{
	SKIPMENU_SKIP = 0,		//�X�L�b�v����
	SKIPMENU_TUTORIAL,		//�`���[�g���A���ɖ߂�	
	SKIPMENU_MAX
}SKIPMENU;

typedef enum
{
	SKIPVTX_FADE = 0,		//������
	SKIPVTX_WINDOW,			//���j���[�E�B���h�E
	SKIPVTX_SKIP,			//�X�L�b�v����
	SKIPVTX_TUTORIAL,		//�`���[�g���A��������	
	SKIPVTX_MAX
}SKIPVTX;

//�|�[�Y�̏��
typedef enum
{
	SKIPSTATE_SELECT = 0,	//�I�ׂ���
	SKIPSTATE_FADE,		//�t�F�[�h��
	SKIPSTATE_MAX
}SKIPSTATE;

//�v���g�^�C�v�錾
void InitTutorialSkip(void);
void UninitTutorialSkip(void);
void UpdateTutorialSkip(void);
void DrawTutorialSkip(void);
void UpdateTutorialSelect(int TutorialSkipSelect);
void SetTutorialSkip(void);

#endif