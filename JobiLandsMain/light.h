//==============================================================
//
//DirectX[light.h]
//Author:����������
//
//==============================================================
#ifndef _LIGHT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _LIGHT_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�񋓌^��`
typedef enum
{
	LIGHTCOL_TYPE_MIDNIGHT = 0,	//�钆
	LIGHTCOL_TYPE_DAWN,			//�閾��
	LIGHTCOL_TYPE_EARLY,		//����
	LIGHTCOL_TYPE_DAYTIME,		//����
	LIGHTCOL_TYPE_MAX
}LIGHTCOL_TYPE;

typedef enum
{
	LIGHTTYPE_MAIN0 = 0,
	LIGHTTYPE_MAIN1,
	LIGHTTYPE_MAIN2,
	LIGHTTYPE_POINT0,
}LIGHTTYPE;

//�\����
typedef struct
{
	D3DLIGHT9 light;		//���̃��C�g�\����
	D3DXVECTOR3 vecDir;		//�ݒ�p�����x�N�g��
	D3DXVECTOR3 vecDirDest[LIGHTCOL_TYPE_MAX];	//�ڕW�̕����x�N�g��
	D3DXVECTOR3 vecDirDiff[LIGHTCOL_TYPE_MAX];	//�����x�N�g���̍���
	D3DXCOLOR DiffuseDest[LIGHTCOL_TYPE_MAX];	//�ڕW�̊g�U��
	D3DXCOLOR DiffuseDiff[LIGHTCOL_TYPE_MAX];	//�g�U���̍���
}LIGHT;

//�v���g�^�C�v�錾
void InitLight(void);
void UnInitLight(void);
void UpdateLight(void);
void ResetLight(void);

#endif
