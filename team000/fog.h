//<====================================================
//
//�t�H�O[��]����(fog.h)
//Author kazuki watanabe
//
//<====================================================
#ifndef _FOG_H_		//���̃}�N����`������Ă��Ȃ�������
#define _FOG_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�����̍\����
typedef struct
{
	D3DXCOLOR FogCol;//���̐F
	D3DFOGMODE FogType;//���̎��

}Fog;

//�v���g�^�C�v�錾
void InitFog(void);
void UninitFog(void);
void UpdateFog(void);//�K�����̏��Ԃŏ����B(����͏d�v)

#endif
