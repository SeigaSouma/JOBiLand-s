//==============================================================
//
//DirectX[light.h]
//Author:佐藤根詩音
//
//==============================================================
#include "light.h"
#include "input.h"
#include "game.h"
#include "player.h"
#include "debugproc.h"

//マクロ定義
#define MAX_LIGHT			(4)			//ライトの最大数

//プロトタイプ宣言
void SetDestLight(void);

//グローバル変数宣言
int nLightType;	//ライトの種類
int nLightCount;//ライトのカウント

LIGHT g_light[MAX_LIGHT];	//ライトの情報

//==============================================================
//ライトの初期化処理
//==============================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntLight;

	nLightType = LIGHTCOL_TYPE_DAYTIME;
	nLightCount = 0;

	//デバイスの取得
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

	//ライトの種類を設定
	for (nCntLight = 0; nCntLight < nLightNum; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		g_light[nCntLight].light.Type = D3DLIGHT_DIRECTIONAL;

		if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
		{

			if (nCntLight == 0)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.18f, 0.10f, 0.4f, 0.0f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.5f, 0.8f);
			}
			else if (nCntLight == 1)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.00f, 0.10f, 0.0f, 0.00f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
			}
			else if (nCntLight == 2)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.00f, 0.10f, 0.00f, 0.00f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
			}
			else if (nCntLight == 3)
			{
				//ライトの種類を設定
				g_light[nCntLight].light.Type = D3DLIGHT_POINT;

				//ライトの位置設定
				g_light[nCntLight].light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

				//ライトの色設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f);

				//ライトの範囲設定
				g_light[nCntLight].light.Range = 2400.0f;

				//ライトの減衰0の設定
				g_light[nCntLight].light.Attenuation0 = 0.94f;

				//ライトの減衰1の設定
				g_light[nCntLight].light.Attenuation1 = 0.0f;

				//ライトの減衰2の設定
				g_light[nCntLight].light.Attenuation2 = 0.0f;

				////内部コーン
				//g_light[nCntLight].light.Theta = 1.5f;

				////外部コーン
				//g_light[nCntLight].light.Phi = D3DX_PI;
			}
		}
		else
		{
			if (nCntLight == 0)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
			}
			else if (nCntLight == 1)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
			}
			else if (nCntLight == 2)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
			}
			else if (nCntLight == 3)
			{
				//ライトの種類を設定
				g_light[nCntLight].light.Type = D3DLIGHT_POINT;

				//ライトの位置設定
				g_light[nCntLight].light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

				//ライトの色設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.4f);

				//ライトの範囲設定
				g_light[nCntLight].light.Range = 2400.0f;

				//ライトの減衰0の設定
				g_light[nCntLight].light.Attenuation0 = 0.94f;

				//ライトの減衰1の設定
				g_light[nCntLight].light.Attenuation1 = 0.0f;

				//ライトの減衰2の設定
				g_light[nCntLight].light.Attenuation2 = 0.0f;

				////内部コーン
				//g_light[nCntLight].light.Theta = 1.5f;

				////外部コーン
				//g_light[nCntLight].light.Phi = D3DX_PI;
			}
		}
		
		D3DXVec3Normalize(&g_light[nCntLight].vecDir, &g_light[nCntLight].vecDir);		//ベクトルを正規化する
		g_light[nCntLight].light.Direction = g_light[nCntLight].vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight].light);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	//目標のライトを設定する
	SetDestLight();

}

//==============================================================
//目標のライトの設定処理
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
					////ライトの拡散光を設定
					//g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.40f, 0.33f, 0.74f, 0.0f);

					////ライトの方向を設定
					//g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, 0.8f);

					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.18f, 0.10f, 0.4f, 0.0f);

					//ライトの方向を設定
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.5f, 0.8f);
				}
				else if (nCntLight == 1)
				{
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.00f, 0.10f, 0.0f, 0.00f);

					//ライトの方向を設定
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
				}
				else if (nCntLight == 2)
				{
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.00f, 0.10f, 0.00f, 0.00f);

					//ライトの方向を設定
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
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.40f, 0.33f, 0.74f, 0.0f);

					//ライトの方向を設定
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
				}
				else if (nCntLight == 1)
				{
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.25f, 0.25f, 0.25f, 0.0f);

					//ライトの方向を設定
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
				}
				else if (nCntLight == 2)
				{
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.25f, 0.25f, 0.25f, 0.0f);

					//ライトの方向を設定
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
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.48f, 0.45f, 0.30f, 0.00f);

					//ライトの方向を設定
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -1.0f, 0.8f);
				}
				else if (nCntLight == 1)
				{
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.50f, 0.44f, 0.50f, 0.00f);

					//ライトの方向を設定
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.5f, -0.8f, -1.0f);
				}
				else if (nCntLight == 2)
				{
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(0.55f, 0.44f, 0.50f, 0.00f);

					//ライトの方向を設定
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
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//ライトの方向を設定
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
				}
				else if (nCntLight == 1)
				{
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//ライトの方向を設定
					g_light[nCntLight].vecDirDest[nCntType] = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
				}
				else if (nCntLight == 2)
				{
					//ライトの拡散光を設定
					g_light[nCntLight].DiffuseDest[nCntType] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//ライトの方向を設定
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
//ライトの終了処理
//==============================================================
void UnInitLight(void)
{

}

//==============================================================
//ライトの更新処理
//==============================================================
void UpdateLight(void)
{
	
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		if (GetMode() == MODE_GAME && GetGameState() != GAMESTATE_START)
		{
			//ライトのカウント加算
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
						//ライトを有効にする
						pDevice->LightEnable(LIGHTTYPE_POINT0, FALSE);
					}

					nNextType = LIGHTCOL_TYPE_DAYTIME;
				}

				//差分を求める
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

			//徐々に明るくしていく
			if (nLightType <= LIGHTCOL_TYPE_DAYTIME)
			{//最後までいってなかったら補正

				//ライトの種類を設定
				for (int nCntLight = 0; nCntLight < LIGHTTYPE_POINT0; nCntLight++)
				{
					g_light[nCntLight].light.Type = D3DLIGHT_DIRECTIONAL;

					//ライトの方向を設定
					g_light[nCntLight].vecDir.x +=
						g_light[nCntLight].vecDirDiff[nLightType].x / (float)LIGHTCHANGE_TIME;

					g_light[nCntLight].vecDir.y +=
						g_light[nCntLight].vecDirDiff[nLightType].y / (float)LIGHTCHANGE_TIME;

					g_light[nCntLight].vecDir.z +=
						g_light[nCntLight].vecDirDiff[nLightType].z / (float)LIGHTCHANGE_TIME;

					/*g_light[nCntLight].vecDir.x += (g_light[nCntLight].vecDirDest[nLightType].x - g_light[nCntLight].vecDir.x) * LIGHT_CORRECTION;
					g_light[nCntLight].vecDir.y += (g_light[nCntLight].vecDirDest[nLightType].y - g_light[nCntLight].vecDir.y) * LIGHT_CORRECTION;
					g_light[nCntLight].vecDir.z += (g_light[nCntLight].vecDirDest[nLightType].z - g_light[nCntLight].vecDir.z) * LIGHT_CORRECTION;*/

					//拡散光の目標との差分で補正する
					g_light[nCntLight].light.Diffuse.r +=
						g_light[nCntLight].DiffuseDiff[nLightType].r / (float)LIGHTCHANGE_TIME;

					g_light[nCntLight].light.Diffuse.g +=
						g_light[nCntLight].DiffuseDiff[nLightType].g / (float)LIGHTCHANGE_TIME;

					g_light[nCntLight].light.Diffuse.b +=
						g_light[nCntLight].DiffuseDiff[nLightType].b / (float)LIGHTCHANGE_TIME;

					/*g_light[nCntLight].light.Diffuse.r += (g_light[nCntLight].DiffuseDest[nLightType].r - g_light[nCntLight].light.Diffuse.r) * LIGHT_CORRECTION;
					g_light[nCntLight].light.Diffuse.g += (g_light[nCntLight].DiffuseDest[nLightType].g - g_light[nCntLight].light.Diffuse.g) * LIGHT_CORRECTION;
					g_light[nCntLight].light.Diffuse.b += (g_light[nCntLight].DiffuseDest[nLightType].b - g_light[nCntLight].light.Diffuse.b) * LIGHT_CORRECTION;*/

					//ベクトルを正規化する
					D3DXVec3Normalize(&g_light[nCntLight].vecDir, &g_light[nCntLight].vecDir);
					g_light[nCntLight].light.Direction = g_light[nCntLight].vecDir;

					//ライトを設定する
					pDevice->SetLight(nCntLight, &g_light[nCntLight].light);
				}
			}
		}

		//プレイヤーの情報取得
		Player *pPlayer = GetPlayer();

		//ライトの種類を設定
		g_light[LIGHTTYPE_POINT0].light.Type = D3DLIGHT_POINT;

		//ライトの位置設定
		g_light[LIGHTTYPE_POINT0].light.Position = D3DXVECTOR3(pPlayer->pos.x, 200.0f, pPlayer->pos.z);

		////ライトの減衰0の設定
		//g_light[LIGHTTYPE_POINT0].light.Attenuation0 = ff;

		////ライトの減衰1の設定
		//g_light[LIGHTTYPE_POINT0].light.Attenuation1 = 0.0f;

		////ライトの減衰2の設定
		//g_light[LIGHTTYPE_POINT0].light.Attenuation2 = 0.0f;

		////ライトの範囲設定
		//g_light[LIGHTTYPE_POINT0].light.Range = fffff;


		//ライトを設定する
		pDevice->SetLight(LIGHTTYPE_POINT0, &g_light[LIGHTTYPE_POINT0].light);
	
}

//==============================================================
//ライトのリセット処理
//==============================================================
void ResetLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntLight;

	nLightType = LIGHTCOL_TYPE_MIDNIGHT;
	nLightCount = 0;

	//デバイスの取得
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

	//ライトの種類を設定
	for (nCntLight = 0; nCntLight < nLightNum; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		g_light[nCntLight].light.Type = D3DLIGHT_DIRECTIONAL;

		if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
		{

			if (nCntLight == 0)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.40f, 0.33f, 0.74f, 0.0f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
			}
			else if (nCntLight == 1)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.00f, 0.10f, 0.0f, 0.00f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
			}
			else if (nCntLight == 2)
			{
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.00f, 0.10f, 0.00f, 0.00f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
			}
			else if (nCntLight == 3)
			{
				//ライトの種類を設定
				g_light[nCntLight].light.Type = D3DLIGHT_POINT;

				//ライトの位置設定
				g_light[nCntLight].light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

				//ライトの色設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

				//ライトの範囲設定
				g_light[nCntLight].light.Range = 2400.0f;

				//ライトの減衰0の設定
				g_light[nCntLight].light.Attenuation0 = 0.94f;

				//ライトの減衰1の設定
				g_light[nCntLight].light.Attenuation1 = 0.0f;

				//ライトの減衰2の設定
				g_light[nCntLight].light.Attenuation2 = 0.0f;

				////内部コーン
				//g_light[nCntLight].light.Theta = 1.5f;

				////外部コーン
				//g_light[nCntLight].light.Phi = D3DX_PI;
			}
		}
		else
		{
			switch (nCntLight)
			{
			case 0:
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.22f, -0.87f, 0.44f);
				break;

			case 1:
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(-0.18f, 0.88f, -0.44f);
				break;

			case 2:
				//ライトの拡散光を設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);

				//ライトの方向を設定
				g_light[nCntLight].vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
				break;

			case 3:
				//ライトの種類を設定
				g_light[nCntLight].light.Type = D3DLIGHT_POINT;

				//ライトの位置設定
				g_light[nCntLight].light.Position = D3DXVECTOR3(0.0f, 50.0f, 0.0f);

				//ライトの色設定
				g_light[nCntLight].light.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);

				//ライトの範囲設定
				g_light[nCntLight].light.Range = 2400.0f;

				//ライトの減衰0の設定
				g_light[nCntLight].light.Attenuation0 = 0.94f;

				//ライトの減衰1の設定
				g_light[nCntLight].light.Attenuation1 = 0.0f;

				//ライトの減衰2の設定
				g_light[nCntLight].light.Attenuation2 = 0.0f;

				////内部コーン
				//g_light[nCntLight].light.Theta = 1.5f;

				////外部コーン
				//g_light[nCntLight].light.Phi = D3DX_PI;
				break;
			}
			
		}

		D3DXVec3Normalize(&g_light[nCntLight].vecDir, &g_light[nCntLight].vecDir);		//ベクトルを正規化する
		g_light[nCntLight].light.Direction = g_light[nCntLight].vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight].light);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		//目標のライトを設定する
		SetDestLight();
	}
}
