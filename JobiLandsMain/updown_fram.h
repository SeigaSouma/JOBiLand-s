//=============================================================================
//
// �㉺�̘g���� [updown_fram.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _UPDOWN_FRAM_H_
#define _UPDOWN_FRAM_H_

#include "main.h"

//�񋓌^��`
typedef enum
{
	FRAMTYPE_UP = 0,
	FRAMTYPE_DOWN,
	FRAMTYPE_MAX
}FRAMTYPE;

typedef enum
{
	UPDOWN_FRAM_STATE_IN = 0,
	UPDOWN_FRAM_STATE_OUT,
	UPDOWN_FRAM_STATE_MAX
}UPDOWN_FRAM_STATE;

//�㉺�̘g�\����
typedef struct
{
	D3DXVECTOR3 pos[2];	//�ʒu
	D3DXCOLOR col;		//�F
	int nState;			//���
	bool bUse;			//�g�p���Ă��邩
	bool bEnd;			//�������Ă��邩
}UPDOWN_FRAM;

//�v���g�^�C�v�錾
void InitUpDownFram(void);
void UninitUpDownFram(void);
void UpdateUpDownFram(void);
void DrawUpDownFram(void);
void SetUpDownFram(void);
UPDOWN_FRAM *GetUpDownFram(void);



#endif
