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

//プロトタイプ宣言
void SetDestLight(void);

//グローバル変数宣言
int nLightType;	//ライトの種類
int nLightCount;//ライトのカウント

LIGHT g_light[LIGHTTYPE_MAX];	//ライトの情報

//==============================================================
//ライトの初期化処理
//==============================================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ
	int nCntLight;

	//デバイスの取得
	pDevice = GetDevice();

	//ライトの種類を設定
	for (nCntLight = 0; nCntLight < LIGHTTYPE_MAX; nCntLight++)
	{
		//ライトの情報をクリアする
		ZeroMemory(&g_light[nCntLight], sizeof(D3DLIGHT9));

		g_light[nCntLight].light.Type = D3DLIGHT_DIRECTIONAL;

		
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
		
		
		D3DXVec3Normalize(&g_light[nCntLight].vecDir, &g_light[nCntLight].vecDir);		//ベクトルを正規化する
		g_light[nCntLight].light.Direction = g_light[nCntLight].vecDir;

		//ライトを設定する
		pDevice->SetLight(nCntLight, &g_light[nCntLight].light);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
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
	
}