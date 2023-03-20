//=============================================================================
//
// �^�C�g����ʏ��� [logo.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _LOGO_H_		//���̃}�N����`������Ă��Ȃ�������
#define _LOGO_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�񋓌^��`
typedef enum
{
	LOGOSTATE_NONE = 0,	//�Ȃɂ����ĂȂ����
	LOGOSTATE_MOVELOG,		//���S�������
	LOGOSTATE_TKTK,		//�`�J�`�J���
	LOGOSTATE_FADE,		//�t�F�[�h���
	LOGOSTATE_MAX

}LOGOSTATE;

typedef enum
{
	LOGOVTX_WHITE = 0,	//��
	LOGOVTX_LOG,		//���S
	LOGOVTX_MAX
}LOGOVTX;

//�^�C�g���\����
typedef struct
{
	int nCntChange;	//�؂�ւ�鎞��
	D3DXCOLOR col;	//�F
	D3DXVECTOR3 pos;	//�ʒu
}Logo;

//�v���g�^�C�v�錾
void InitLogo(void);
void UninitLogo(void);
void UpdateLogo(void);
void DrawLogo(int nType);
Logo *GetLogo(void);

#endif