//=============================================================================
//
// 発射物処理 [launch.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "launch.h"
#include "input.h"
#include "model.h"
#include "game.h"
#include "edit.h"
#include "calculation.h"
#include "shadow.h"
#include "particle.h"
#include "debugproc.h"
#include "sound.h"

//マクロ定義
#define MAX_LAUNCH		(128)			// 発射物の最大数
#define LAUNCH_GRAVITY	(-1.0f)			// 発射物の重力
#define LAUNCH_FLY		(-40.0f)		// 発射物の飛ぶ勢い

//グローバル変数宣言
Launch g_aLaunch[MAX_LAUNCH];		//発射物の情報

//==================================================================================
//発射物の初期化処理
//==================================================================================
void InitLaunch(void)
{
	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{ // 情報の初期化

		// モデル情報の初期化
		ZeroMemory(&g_aLaunch[nCntLaunch].modelData, sizeof(Model));

		// モデル情報の設定
		g_aLaunch[nCntLaunch].modelData.nIdxShadow = -1;								// 影のインデックス番号
		g_aLaunch[nCntLaunch].modelData.nShadow = 1;									// 影を使うかどうか
		g_aLaunch[nCntLaunch].modelData.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		// モデルの最小値
		g_aLaunch[nCntLaunch].modelData.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);	// モデルの最大値
		g_aLaunch[nCntLaunch].modelData.nParent = -1;									// 親の番号
		g_aLaunch[nCntLaunch].modelData.bUse = false;									// 使用状況
	}
}

//==================================================================================
//発射物の終了処理
//==================================================================================
void UninitLaunch(void)
{

}

//==================================================================================
//発射物の更新処理
//==================================================================================
void UpdateLaunch(void)
{
	if (GetKeyboardTrigger(DIK_0) == true)
	{ // 0キーを押した場合

		// 発射物の設定処理
		SetLaunch();
	}

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{
		if (g_aLaunch[nCntLaunch].modelData.bUse == true)
		{ // 使用している場合
			
			// 重力をかける
			g_aLaunch[nCntLaunch].modelData.move.y += LAUNCH_GRAVITY;

			// 位置を更新する
			g_aLaunch[nCntLaunch].modelData.pos += g_aLaunch[nCntLaunch].modelData.move;

			if (g_aLaunch[nCntLaunch].modelData.pos.y <= 0.0f)
			{ // 位置が一定数以下になると

				// 位置を補正する
				g_aLaunch[nCntLaunch].modelData.pos.y = 0.0f;

				// 使用しない
				g_aLaunch[nCntLaunch].modelData.bUse = false;
			}
		}
	}
}

//==================================================================================
//発射物の描画処理
//==================================================================================
void DrawLaunch(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATRIX mtxnParent;			//親の計算用マトリックス

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{
		if (g_aLaunch[nCntLaunch].modelData.bUse == true)
		{//使用していたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aLaunch[nCntLaunch].modelData.mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aLaunch[nCntLaunch].modelData.rot.y, g_aLaunch[nCntLaunch].modelData.rot.x, g_aLaunch[nCntLaunch].modelData.rot.z);
			D3DXMatrixMultiply(&g_aLaunch[nCntLaunch].modelData.mtxWorld, &g_aLaunch[nCntLaunch].modelData.mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aLaunch[nCntLaunch].modelData.pos.x, g_aLaunch[nCntLaunch].modelData.pos.y, g_aLaunch[nCntLaunch].modelData.pos.z);
			D3DXMatrixMultiply(&g_aLaunch[nCntLaunch].modelData.mtxWorld, &g_aLaunch[nCntLaunch].modelData.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aLaunch[nCntLaunch].modelData.mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aLaunch[nCntLaunch].modelData.pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aLaunch[nCntLaunch].modelData.dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aLaunch[nCntLaunch].modelData.pTexture[nCntMat]);

				//発射物(パーツ)の描画
				g_aLaunch[nCntLaunch].modelData.pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//発射物の設定処理
//==================================================================================
void SetLaunch(void)
{
	Model *pModel = GetXLoadData();		// モデルの情報

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{//パーツ分繰り返す

		if (g_aLaunch[nCntLaunch].modelData.bUse == false)
		{ // 使用していない場合

			g_aLaunch[nCntLaunch].modelData.nType = LAUNCHTYPE_GOOD;					// 良い物

			switch (g_aLaunch[nCntLaunch].modelData.nType)
			{
			case LAUNCHTYPE_GOOD:	// 良い奴

				// モデル情報を取得する
				g_aLaunch[nCntLaunch].modelData = pModel[52];

				break;				// 抜け出す

			case LAUNCHTYPE_EVIL:	// 悪い奴

				// モデル情報を取得する
				g_aLaunch[nCntLaunch].modelData = pModel[33];

				break;				// 抜け出す
			}

			// 発射物の設定
			g_aLaunch[nCntLaunch].modelData.pos = D3DXVECTOR3(0.0f, 500.0f, 1000.0f);	// 位置
			g_aLaunch[nCntLaunch].modelData.move = D3DXVECTOR3(0.0f, 0.0f, LAUNCH_FLY);		// 移動量
			g_aLaunch[nCntLaunch].modelData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
			g_aLaunch[nCntLaunch].modelData.bUse = true;								// 使用状況

			break;		// 抜け出す
		}
	}
}