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

//�}�N����`
#define MAX_LIGHT			(4)			//���C�g�̍ő吔

//�v���g�^�C�v�錾
void SetDestLight(void);

//�O���[�o���ϐ��錾
int nLightType;	//���C�g�̎��
int nLightCount;//���C�g�̃J�E���g

LIGHT g_light[MAX_LIGHT];	//���C�g�̏��

//==============================================================
//���C�g�̏���������
//==============================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntLight;

	nLightType = LIGHTCOL_TYPE_DAYTIME;
	nLightCount = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nLightNum = 0;
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		nLightNum = MAX_LIGHT;
	}
	else
	{
		nLightNum = 3;
	}

	//���C�g�̎�ނ�ݒ�
	for (nCntLight = 0; nCntLight < nLightNum; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		g_light[nCntLight].light.Type = D3DLIGHT_DIRECTIONAL;

		if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
		{

			if (nCntLight == 0)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.18f, 0.10f, 0.4f, 0.0f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.5f, 0.8f);
			}
			else if (nCntLight == 1)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.00f, 0.10f, 0.0f, 0.00f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
			}
			else if (nCntLight == 2)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.00f, 0.10f, 0.00f, 0.00f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
			}
			else if (nCntLight == 3)
			{
				//���C�g�̎�ނ�ݒ�
				g_light[nCntLight].light.Type = D3DLIGHT_POINT;

				//���C�g�̈ʒu�ݒ�
				g_light[nCntLight].light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

				//���C�g�̐F�ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f);

				//���C�g�͈̔͐ݒ�
				g_light[nCntLight].light.Range = 2400.0f;

				//���C�g�̌���0�̐ݒ�
				g_light[nCntLight].light.Attenuation0 = 0.94f;

				//���C�g�̌���1�̐ݒ�
				g_light[nCntLight].light.Attenuation1 = 0.0f;

				//���C�g�̌���2�̐ݒ�
				g_light[nCntLight].light.Attenuation2 = 0.0f;

				////�����R�[��
				//g_light[nCntLight].light.Theta = 1.5f;

				////�O���R�[��
				//g_light[nCntLight].light.Phi = D3DX_PI;
			}
		}
		else
		{
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
			else if (nCntLight == 3)
			{
				//���C�g�̎�ނ�ݒ�
				g_light[nCntLight].light.Type = D3DLIGHT_POINT;

				//���C�g�̈ʒu�ݒ�
				g_light[nCntLight].light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

				//���C�g�̐F�ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f);

				//���C�g�͈̔͐ݒ�
				g_light[nCntLight].light.Range = 2400.0f;

				//���C�g�̌���0�̐ݒ�
				g_light[nCntLight].light.Attenuation0 = 0.94f;

				//���C�g�̌���1�̐ݒ�
				g_light[nCntLight].light.Attenuation1 = 0.0f;

				//���C�g�̌���2�̐ݒ�
				g_light[nCntLight].light.Attenuation2 = 0.0f;

				////�����R�[��
				//g_light[nCntLight].light.Theta = 1.5f;

				////�O���R�[��
				//g_light[nCntLight].light.Phi = D3DX_PI;
			}
		}
		
		D3DXVec3Normalize(&g_light[nCntLight].vecDir, &g_light[nCntLight].vecDir);		//�x�N�g���𐳋K������
		g_light[nCntLight].light.Direction = g_light[nCntLight].vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_light[nCntLight].light);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	//�ڕW�̃��C�g��ݒ肷��
	SetDestLight();

}

//==============================================================
//�ڕW�̃��C�g�̐ݒ菈��
//==============================================================
void SetDestLight(void)
{

	for (int nCntType = 0; nCntType < LIGHTCOL_TYPE_MAX; nCntType++)
	{
		for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
		{
			switch (nCntType)
			{
			case LIGHTCOL_TYPE_MIDNIGHT:

				if (nCntLight == 0)
				{
					////���C�g�̊g�U����ݒ�
					//g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.40f, 0.33f, 0.74f, 0.0f);

					////���C�g�̕�����ݒ�
					//g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, 0.8f);

					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.18f, 0.10f, 0.4f, 0.0f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.5f, 0.8f);
				}
				else if (nCntLight == 1)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.00f, 0.10f, 0.0f, 0.00f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
				}
				else if (nCntLight == 2)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.00f, 0.10f, 0.00f, 0.00f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
				}

				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_MIDNIGHT].x = g_light[LIGHTCOL_TYPE_MIDNIGHT].vecDirDest[LIGHTCOL_TYPE_MIDNIGHT + 1].x - g_light[LIGHTCOL_TYPE_MIDNIGHT].vecDirDest[LIGHTCOL_TYPE_MIDNIGHT].x;
				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_MIDNIGHT].y = g_light[LIGHTCOL_TYPE_MIDNIGHT].vecDirDest[LIGHTCOL_TYPE_MIDNIGHT + 1].y - g_light[LIGHTCOL_TYPE_MIDNIGHT].vecDirDest[LIGHTCOL_TYPE_MIDNIGHT].y;
				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_MIDNIGHT].z = g_light[LIGHTCOL_TYPE_MIDNIGHT].vecDirDest[LIGHTCOL_TYPE_MIDNIGHT + 1].z - g_light[LIGHTCOL_TYPE_MIDNIGHT].vecDirDest[LIGHTCOL_TYPE_MIDNIGHT].z;

				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_MIDNIGHT].r = g_light[LIGHTCOL_TYPE_MIDNIGHT].DiffuseDest[LIGHTCOL_TYPE_MIDNIGHT + 1].r - g_light[LIGHTCOL_TYPE_MIDNIGHT].DiffuseDest[LIGHTCOL_TYPE_MIDNIGHT].r;
				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_MIDNIGHT].g = g_light[LIGHTCOL_TYPE_MIDNIGHT].DiffuseDest[LIGHTCOL_TYPE_MIDNIGHT + 1].g - g_light[LIGHTCOL_TYPE_MIDNIGHT].DiffuseDest[LIGHTCOL_TYPE_MIDNIGHT].g;
				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_MIDNIGHT].b = g_light[LIGHTCOL_TYPE_MIDNIGHT].DiffuseDest[LIGHTCOL_TYPE_MIDNIGHT + 1].b - g_light[LIGHTCOL_TYPE_MIDNIGHT].DiffuseDest[LIGHTCOL_TYPE_MIDNIGHT].b;
				break;

			case LIGHTCOL_TYPE_DAWN:
				if (nCntLight == 0)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.40f, 0.33f, 0.74f, 0.0f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
				}
				else if (nCntLight == 1)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.25f, 0.25f, 0.25f, 0.0f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
				}
				else if (nCntLight == 2)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.25f, 0.25f, 0.25f, 0.0f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
				}

				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_DAWN].x = g_light[LIGHTCOL_TYPE_DAWN].vecDirDest[LIGHTCOL_TYPE_DAWN + 1].x - g_light[LIGHTCOL_TYPE_DAWN].vecDirDest[LIGHTCOL_TYPE_DAWN].x;
				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_DAWN].y = g_light[LIGHTCOL_TYPE_DAWN].vecDirDest[LIGHTCOL_TYPE_DAWN + 1].y - g_light[LIGHTCOL_TYPE_DAWN].vecDirDest[LIGHTCOL_TYPE_DAWN].y;
				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_DAWN].z = g_light[LIGHTCOL_TYPE_DAWN].vecDirDest[LIGHTCOL_TYPE_DAWN + 1].z - g_light[LIGHTCOL_TYPE_DAWN].vecDirDest[LIGHTCOL_TYPE_DAWN].z;

				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_DAWN].r = g_light[LIGHTCOL_TYPE_DAWN].DiffuseDest[LIGHTCOL_TYPE_DAWN + 1].r - g_light[LIGHTCOL_TYPE_DAWN].DiffuseDest[LIGHTCOL_TYPE_DAWN].r;
				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_DAWN].g = g_light[LIGHTCOL_TYPE_DAWN].DiffuseDest[LIGHTCOL_TYPE_DAWN + 1].g - g_light[LIGHTCOL_TYPE_DAWN].DiffuseDest[LIGHTCOL_TYPE_DAWN].g;
				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_DAWN].b = g_light[LIGHTCOL_TYPE_DAWN].DiffuseDest[LIGHTCOL_TYPE_DAWN + 1].b - g_light[LIGHTCOL_TYPE_DAWN].DiffuseDest[LIGHTCOL_TYPE_DAWN].b;
				break;

			case LIGHTCOL_TYPE_EARLY:
				if (nCntLight == 0)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.48f, 0.45f, 0.30f, 0.00f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -1.0f, 0.8f);
				}
				else if (nCntLight == 1)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.50f, 0.44f, 0.50f, 0.00f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.5f, -0.8f, -1.0f);
				}
				else if (nCntLight == 2)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.55f, 0.44f, 0.50f, 0.00f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(-0.5f, -0.8f, 0.4f);
				}

				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_EARLY].x = g_light[LIGHTCOL_TYPE_EARLY].vecDirDest[LIGHTCOL_TYPE_EARLY + 1].x - g_light[LIGHTCOL_TYPE_EARLY].vecDirDest[LIGHTCOL_TYPE_EARLY].x;
				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_EARLY].y = g_light[LIGHTCOL_TYPE_EARLY].vecDirDest[LIGHTCOL_TYPE_EARLY + 1].y - g_light[LIGHTCOL_TYPE_EARLY].vecDirDest[LIGHTCOL_TYPE_EARLY].y;
				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_EARLY].z = g_light[LIGHTCOL_TYPE_EARLY].vecDirDest[LIGHTCOL_TYPE_EARLY + 1].z - g_light[LIGHTCOL_TYPE_EARLY].vecDirDest[LIGHTCOL_TYPE_EARLY].z;

				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_EARLY].r = g_light[LIGHTCOL_TYPE_EARLY].DiffuseDest[LIGHTCOL_TYPE_EARLY + 1].r - g_light[LIGHTCOL_TYPE_EARLY].DiffuseDest[LIGHTCOL_TYPE_EARLY].r;
				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_EARLY].g = g_light[LIGHTCOL_TYPE_EARLY].DiffuseDest[LIGHTCOL_TYPE_EARLY + 1].g - g_light[LIGHTCOL_TYPE_EARLY].DiffuseDest[LIGHTCOL_TYPE_EARLY].g;
				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_EARLY].b = g_light[LIGHTCOL_TYPE_EARLY].DiffuseDest[LIGHTCOL_TYPE_EARLY + 1].b - g_light[LIGHTCOL_TYPE_EARLY].DiffuseDest[LIGHTCOL_TYPE_EARLY].b;
				break;

			case LIGHTCOL_TYPE_DAYTIME:

				if (nCntLight == 0)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
				}
				else if (nCntLight == 1)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
				}
				else if (nCntLight == 2)
				{
					//���C�g�̊g�U����ݒ�
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
				}

				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_DAYTIME].x = g_light[LIGHTCOL_TYPE_DAYTIME].vecDirDest[LIGHTCOL_TYPE_DAYTIME].x - g_light[LIGHTCOL_TYPE_DAYTIME].vecDirDest[LIGHTCOL_TYPE_DAYTIME].x;
				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_DAYTIME].y = g_light[LIGHTCOL_TYPE_DAYTIME].vecDirDest[LIGHTCOL_TYPE_DAYTIME].y - g_light[LIGHTCOL_TYPE_DAYTIME].vecDirDest[LIGHTCOL_TYPE_DAYTIME].y;
				g_light[nCntLight].vecDirDiff[LIGHTCOL_TYPE_DAYTIME].z = g_light[LIGHTCOL_TYPE_DAYTIME].vecDirDest[LIGHTCOL_TYPE_DAYTIME].z - g_light[LIGHTCOL_TYPE_DAYTIME].vecDirDest[LIGHTCOL_TYPE_DAYTIME].z;

				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_DAYTIME].r = g_light[LIGHTCOL_TYPE_DAYTIME].DiffuseDest[LIGHTCOL_TYPE_DAYTIME].r - g_light[LIGHTCOL_TYPE_DAYTIME].DiffuseDest[LIGHTCOL_TYPE_DAYTIME].r;
				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_DAYTIME].g = g_light[LIGHTCOL_TYPE_DAYTIME].DiffuseDest[LIGHTCOL_TYPE_DAYTIME].g - g_light[LIGHTCOL_TYPE_DAYTIME].DiffuseDest[LIGHTCOL_TYPE_DAYTIME].g;
				g_light[nCntLight].DiffuseDiff[LIGHTCOL_TYPE_DAYTIME].b = g_light[LIGHTCOL_TYPE_DAYTIME].DiffuseDest[LIGHTCOL_TYPE_DAYTIME].b - g_light[LIGHTCOL_TYPE_DAYTIME].DiffuseDest[LIGHTCOL_TYPE_DAYTIME].b;
				break;
			}
		}
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
	
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		if (GetMode() == MODE_GAME && GetGameState() != GAMESTATE_START)
		{
			//���C�g�̃J�E���g���Z
			nLightCount++;

			if (nLightCount >= LIGHTCHANGE_TIME)
			{
				nLightCount = 0;
				nLightType += 1;

				int nNextType = nLightType + 1;
				if (nNextType >= LIGHTCOL_TYPE_DAYTIME)
				{
					if (nNextType == LIGHTCOL_TYPE_DAYTIME + 1)
					{
						//���C�g��L���ɂ���
						pDevice->LightEnable(LIGHTTYPE_POINT0, FALSE);
					}

					nNextType = LIGHTCOL_TYPE_DAYTIME;
				}

				//���������߂�
				for (int nCntLight = 0; nCntLight < LIGHTTYPE_POINT0; nCntLight++)
				{
					g_light[nCntLight].vecDirDiff[nLightType].x = g_light[nCntLight].vecDirDest[nNextType].x - g_light[nCntLight].vecDirDest[nLightType].x;
					g_light[nCntLight].vecDirDiff[nLightType].y = g_light[nCntLight].vecDirDest[nNextType].y - g_light[nCntLight].vecDirDest[nLightType].y;
					g_light[nCntLight].vecDirDiff[nLightType].z = g_light[nCntLight].vecDirDest[nNextType].z - g_light[nCntLight].vecDirDest[nLightType].z;

					g_light[nCntLight].DiffuseDiff[nLightType].r = g_light[nCntLight].DiffuseDest[nNextType].r - g_light[nCntLight].DiffuseDest[nLightType].r;
					g_light[nCntLight].DiffuseDiff[nLightType].g = g_light[nCntLight].DiffuseDest[nNextType].g - g_light[nCntLight].DiffuseDest[nLightType].g;
					g_light[nCntLight].DiffuseDiff[nLightType].b = g_light[nCntLight].DiffuseDest[nNextType].b - g_light[nCntLight].DiffuseDest[nLightType].b;
				}

				if (nLightType >= LIGHTCOL_TYPE_DAYTIME)
				{
					nLightType = LIGHTCOL_TYPE_DAYTIME;
				}
			}

			//���X�ɖ��邭���Ă���
			if (nLightType <= LIGHTCOL_TYPE_DAYTIME)
			{//�Ō�܂ł����ĂȂ�������␳

				//���C�g�̎�ނ�ݒ�
				for (int nCntLight = 0; nCntLight < LIGHTTYPE_POINT0; nCntLight++)
				{
					g_light[nCntLight].light.Type = D3DLIGHT_DIRECTIONAL;

					//���C�g�̕�����ݒ�
					g_light[nCntLight].vecDir.x +=
						g_light[nCntLight].vecDirDiff[nLightType].x / (float)LIGHTCHANGE_TIME;

					g_light[nCntLight].vecDir.y +=
						g_light[nCntLight].vecDirDiff[nLightType].y / (float)LIGHTCHANGE_TIME;

					g_light[nCntLight].vecDir.z +=
						g_light[nCntLight].vecDirDiff[nLightType].z / (float)LIGHTCHANGE_TIME;

					/*g_light[nCntLight].vecDir.x += (g_light[nCntLight].vecDirDest[nLightType].x - g_light[nCntLight].vecDir.x) * LIGHT_CORRECTION;
					g_light[nCntLight].vecDir.y += (g_light[nCntLight].vecDirDest[nLightType].y - g_light[nCntLight].vecDir.y) * LIGHT_CORRECTION;
					g_light[nCntLight].vecDir.z += (g_light[nCntLight].vecDirDest[nLightType].z - g_light[nCntLight].vecDir.z) * LIGHT_CORRECTION;*/

					//�g�U���̖ڕW�Ƃ̍����ŕ␳����
					g_light[nCntLight].light.Diffuse.r +=
						g_light[nCntLight].DiffuseDiff[nLightType].r / (float)LIGHTCHANGE_TIME;

					g_light[nCntLight].light.Diffuse.g +=
						g_light[nCntLight].DiffuseDiff[nLightType].g / (float)LIGHTCHANGE_TIME;

					g_light[nCntLight].light.Diffuse.b +=
						g_light[nCntLight].DiffuseDiff[nLightType].b / (float)LIGHTCHANGE_TIME;

					/*g_light[nCntLight].light.Diffuse.r += (g_light[nCntLight].DiffuseDest[nLightType].r - g_light[nCntLight].light.Diffuse.r) * LIGHT_CORRECTION;
					g_light[nCntLight].light.Diffuse.g += (g_light[nCntLight].DiffuseDest[nLightType].g - g_light[nCntLight].light.Diffuse.g) * LIGHT_CORRECTION;
					g_light[nCntLight].light.Diffuse.b += (g_light[nCntLight].DiffuseDest[nLightType].b - g_light[nCntLight].light.Diffuse.b) * LIGHT_CORRECTION;*/

					//�x�N�g���𐳋K������
					D3DXVec3Normalize(&g_light[nCntLight].vecDir, &g_light[nCntLight].vecDir);
					g_light[nCntLight].light.Direction = g_light[nCntLight].vecDir;

					//���C�g��ݒ肷��
					pDevice->SetLight(nCntLight, &g_light[nCntLight].light);
				}
			}
		}

		//�v���C���[�̏��擾
		Player *pPlayer = GetPlayer();

		//���C�g�̎�ނ�ݒ�
		g_light[LIGHTTYPE_POINT0].light.Type = D3DLIGHT_POINT;

		//���C�g�̈ʒu�ݒ�
		g_light[LIGHTTYPE_POINT0].light.Position = D3DXVECTOR3(pPlayer->pos.x, 200.0f, pPlayer->pos.z);

		////���C�g�̌���0�̐ݒ�
		//g_light[LIGHTTYPE_POINT0].light.Attenuation0 = ff;

		////���C�g�̌���1�̐ݒ�
		//g_light[LIGHTTYPE_POINT0].light.Attenuation1 = 0.0f;

		////���C�g�̌���2�̐ݒ�
		//g_light[LIGHTTYPE_POINT0].light.Attenuation2 = 0.0f;

		////���C�g�͈̔͐ݒ�
		//g_light[LIGHTTYPE_POINT0].light.Range = fffff;


		//���C�g��ݒ肷��
		pDevice->SetLight(LIGHTTYPE_POINT0, &g_light[LIGHTTYPE_POINT0].light);
	
}

//==============================================================
//���C�g�̃��Z�b�g����
//==============================================================
void ResetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^
	int nCntLight;

	nLightType = LIGHTCOL_TYPE_MIDNIGHT;
	nLightCount = 0;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	int nLightNum = 0;
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		nLightNum = MAX_LIGHT;
	}
	else
	{
		nLightNum = MAX_LIGHT-1;
	}

	//���C�g�̎�ނ�ݒ�
	for (nCntLight = 0; nCntLight < nLightNum; nCntLight++)
	{
		//���C�g�̏����N���A����
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		g_light[nCntLight].light.Type = D3DLIGHT_DIRECTIONAL;

		if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
		{

			if (nCntLight == 0)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.40f, 0.33f, 0.74f, 0.0f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
			}
			else if (nCntLight == 1)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.00f, 0.10f, 0.0f, 0.00f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
			}
			else if (nCntLight == 2)
			{
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.00f, 0.10f, 0.00f, 0.00f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
			}
			else if (nCntLight == 3)
			{
				//���C�g�̎�ނ�ݒ�
				g_light[nCntLight].light.Type = D3DLIGHT_POINT;

				//���C�g�̈ʒu�ݒ�
				g_light[nCntLight].light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

				//���C�g�̐F�ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

				//���C�g�͈̔͐ݒ�
				g_light[nCntLight].light.Range = 2400.0f;

				//���C�g�̌���0�̐ݒ�
				g_light[nCntLight].light.Attenuation0 = 0.94f;

				//���C�g�̌���1�̐ݒ�
				g_light[nCntLight].light.Attenuation1 = 0.0f;

				//���C�g�̌���2�̐ݒ�
				g_light[nCntLight].light.Attenuation2 = 0.0f;

				////�����R�[��
				//g_light[nCntLight].light.Theta = 1.5f;

				////�O���R�[��
				//g_light[nCntLight].light.Phi = D3DX_PI;
			}
		}
		else
		{
			switch (nCntLight)
			{
			case 0:
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
				break;

			case 1:
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
				break;

			case 2:
				//���C�g�̊g�U����ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

				//���C�g�̕�����ݒ�
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
				break;

			case 3:
				//���C�g�̎�ނ�ݒ�
				g_light[nCntLight].light.Type = D3DLIGHT_POINT;

				//���C�g�̈ʒu�ݒ�
				g_light[nCntLight].light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

				//���C�g�̐F�ݒ�
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

				//���C�g�͈̔͐ݒ�
				g_light[nCntLight].light.Range = 2400.0f;

				//���C�g�̌���0�̐ݒ�
				g_light[nCntLight].light.Attenuation0 = 0.94f;

				//���C�g�̌���1�̐ݒ�
				g_light[nCntLight].light.Attenuation1 = 0.0f;

				//���C�g�̌���2�̐ݒ�
				g_light[nCntLight].light.Attenuation2 = 0.0f;

				////�����R�[��
				//g_light[nCntLight].light.Theta = 1.5f;

				////�O���R�[��
				//g_light[nCntLight].light.Phi = D3DX_PI;
				break;
			}
			
		}

		D3DXVec3Normalize(&g_light[nCntLight].vecDir, &g_light[nCntLight].vecDir);		//�x�N�g���𐳋K������
		g_light[nCntLight].light.Direction = g_light[nCntLight].vecDir;

		//���C�g��ݒ肷��
		pDevice->SetLight(nCntLight, &g_light[nCntLight].light);

		//���C�g��L���ɂ���
		pDevice->LightEnable(nCntLight, TRUE);
	}

	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		//�ڕW�̃��C�g��ݒ肷��
		SetDestLight();
	}
}
