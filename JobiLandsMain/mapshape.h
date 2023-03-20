//=============================================================================
//
// �}�b�v�̌`���� [mapshape.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _MAPSHAPE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _MAPSHAPE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�b�v�̌`�̗񋓌^
typedef enum
{
	MAPSHAPE_VTX_FRAM = 0,
	MAPSHAPE_VTX_MAX
}MAPSHAPE_VTX;

//�}�b�v�̌`�̍\����
typedef struct
{
	UI_2D aUI[MAPSHAPE_VTX_MAX];	//UI���
}MapShape;

//�v���g�^�C�v�錾
void InitMapShape(void);
void UninitMapShape(void);
void UpdateMapShape(void);
void DrawMapShape(void);
MapShape *GetMapShape(void);

#endif
