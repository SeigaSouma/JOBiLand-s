//=============================================================================
//
// �C�x���g�X�^���v���� [eventstamp.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _EVENTSTAMP_H_
#define _EVENTSTAMP_H_

#include "main.h"

//�}�N����`
#define MAX_EVENTSTAMP	(2)	//�C�x���g�X�^���v�̍ő吔

//�񋓌^��`
typedef enum
{
	EVENTTYPE_MANYMOB = 0,
	EVENTTYPE_MORS,
	EVENTTYPE_MAX
}EVENTTYPE;

//�C�x���g�E�B���h�E�̍\����
typedef struct
{
	bool bUse;		//�g�p���Ă��邩
	int nType;		//���
	UI_2D aUI;	//UI���
}EVENTSTAMP;

//�v���g�^�C�v�錾
void InitEventStamp(void);
void UninitEventStamp(void);
void UpdateEventStamp(void);
void DrawEventStamp(void);
void SetEventStamp(int nType);
EVENTSTAMP *GetEventStamp(void);



#endif
