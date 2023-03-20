//==============================================================
//
//DirectX[light.h]
//Author:����������
//
//==============================================================
#include "light.h"
#include "input.h"
#include "game.h"
#include "player.h"
#include "debugproc.h"

//�v���g�^�C�v�錾
void SetDestLight(void);

//�O���[�o���ϐ��錾
int nLightType;	//���C�g�̎��
int nLightCount;//���C�g�̃J�E���g

LIGHT g_light[LIGHTTYPE_MAX];	//���C�g�̏��

//==============================================================
//���C�g�̏���������
//==============================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntLight;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���C�g�̎�ނ�ݒ�
	for (nCntLight = 0; nCntLight < LIGHTTYPE_MAX; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		g_light[nCntLight].light.Type = D3DLIGHT_DIRECTIONAL;

		
			if (nCntLight == 0)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
			}
			else if (nCntLight == 1)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
			}
			else if (nCntLight == 2)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
			}
		
		
		D3DXVec3Normalize(&g_light[nCntLight].vecDir, &g_light[nCntLight].vecDir);		//�x�N�g���𐳋K������
		g_light[nCntLight].light.Direction = g_light[nCntLight].vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_light[nCntLight].light);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

}



//==============================================================
//���C�g�̏I������
//==============================================================
void UnInitLight(void)
{

}

//==============================================================
//���C�g�̍X�V����
//==============================================================
void UpdateLight(void)
{
	
}