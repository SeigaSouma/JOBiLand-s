//=============================================================================
//
// ���ʂ̃��b�Z�[�W���� [resultmessage.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _RESULT_MESSAGE_H_
#define _RESULT_MESSAGE_H_

#include "main.h"

//���ʂ̃��b�Z�[�W�̗񋓌^��`
typedef enum
{
	RESULTTYPE_CLEAR = 0,
	RESULTTYPE_FAILED,
	RESULTTYPE_MAX
}RESULTTYPE;

//���ʂ̃��b�Z�[�W�\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posOrigin;	//�ʒu
	D3DXCOLOR col;		//�F
	int nType;			//���
	float fWidth;		//��
	float fHeight;		//����
	int nCntZoom;		//�傫���Ȃ鎞��
	bool bUse;			//�g�p���Ă��邩
}RESULT_MESSAGE;

//�v���g�^�C�v�錾
void InitResultMessage(void);
void UninitResultMessage(void);
void UpdateResultMessage(void);
void DrawResultMessage(void);
void SetResultMessage(int nType);
RESULT_MESSAGE *GetResultMessage(void);



#endif
