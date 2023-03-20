//=============================================================================
//
// �h������ [dispatch.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _DISPATCH_H_		//���̃}�N����`������Ă��Ȃ�������
#define _DISPATCH_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_DISPACTH	(1)	//�h���̍ő吔

//�h���̗񋓌^��`
typedef enum
{
	DISPATCH_TYPE_LEFT = 0,
	DISPATCH_TYPE_RIGHT,
	DISPATCH_TYPE_MAX
}DISPATCH_TYPE;

//�h���\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩
	int nType;				//���
}DISPATCH;

//�v���g�^�C�v�錾
void InitDisPatch(void);
void UninitDisPatch(void);
void UpdateDisPatch(void);
void UpdateTutorialDisPatch(void);
void DrawDisPatch(void);
void SetRightDisPatch(void);
void SetLeftDisPatch(void);
DISPATCH *GetDispatch(void);

#endif