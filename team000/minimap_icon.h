//=============================================================================
//
// �~�j�}�b�v�A�C�R�� [minimap_icon.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MINIMAP_ICON_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MINIMAP_ICON_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�A�C�R���̗񋓌^
typedef enum
{
	MINIMAP_ICON_PLAYER,
	MINIMAP_ICON_CHIBI,
	MINIMAP_ICON_KODEBU,
	MINIMAP_ICON_ENEMY,
	MINIMAP_ICON_MOB,
	MINIMAP_ICON_MAX
}MINIMAP_ICON;

//�X�|�[���n�_�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	int nType;				//�A�C�R���̃^�C�v
	bool bUse;				//�g�p����Ă��邩�ǂ���
}MINIICON;


//�v���g�^�C�v�錾
void InitMiniIcon(void);
void UninitMiniIcon(void);
void UpdateMiniIcon(void);
void DrawMiniIcon(void);
void UpdatePositionIcon(int nIdxIcon, D3DXVECTOR3 pos);
int SetIcon(int nType, float size);
MINIICON *GetMiniIcon(void);

#endif