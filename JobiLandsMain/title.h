//=============================================================================
//
// �^�C�g����ʏ��� [title.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _TITLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _TITLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�񋓌^��`
typedef enum
{
	TITLESTATE_NONE = 0,	//�Ȃɂ����ĂȂ����
	TITLESTATE_MOVELOG,		//���S�������
	TITLESTATE_TKTK,		//�`�J�`�J���
	TITLESTATE_FADE,		//�t�F�[�h���
	TITLESTATE_MAX

}TITLESTATE;

typedef enum
{
	TITLEVTX_BLACK = 0,	//����
	TITLEVTX_LOG,		//���S
	TITLEVTX_LINE,		//��
	TITLEVTX_LIBELLION,	//�p��
	TITLEVTX_HONKI,		//�{�C�o��
	TITLEVTX_TITLELOGO,	//�^�C�g�����S�S��
	TITLEVTX_MAX
}TITLEVTX;

typedef enum
{
	TITLELOG_BLACK = 0,	//����
	TITLELOG_LOG,		//���S
	TITLELOG_LINE,		//��
	TITLELOG_LIBELLION,	//�p��
	TITLELOG_HONKI,		//�{�C�o��
	TITLELOG_TITLELOGO,	//�^�C�g�����S�S��
	TITLELOG_MAX
}TITLELOG;

typedef enum
{
	TITLESELECT_START = 0,	//�X�^�[�g
	TITLESELECT_END,		//�I��
	TITLESELECT_MAX
}TITLESELECT;

//�^�C�g���\����
typedef struct
{
	int nTKTKFrame;	//�`�J�`�J�̃t���[���J�E���g
	int nTKTKCounter;	//�`�J�`�J�̉�
	int OldSelect;	//�O��̑I����
	int nSelect;	//���݂̑I����
	int nCntChange;	//�؂�ւ�鎞��
	int nCntMoveLog;	//���S�̓����J�E���g
	float fVolume;	//����
	bool bMoveLog;	//���O�̓�������
	D3DXCOLOR col;	//�F
	D3DXCOLOR Logcol[TITLELOG_MAX];	//�F
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posLog[TITLELOG_MAX];	//�ʒu
	int nType;
	int nState;	//���
}Title;

//�v���g�^�C�v�錾
void InitTitle(void);
void UninitTitle(void);
void UpdateTitle(void);
void DrawTitle(int nType);
void UpdateSelectTitle(void);
void UpdateTitleTKTK(void);
void UpdateSelectTitleCol(int Select);
void UpdateOptionTitle(int nType);
bool GetEndFrag(void);
Title *GetTitle(void);

#endif