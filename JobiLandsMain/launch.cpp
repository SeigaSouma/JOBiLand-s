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
#include "player.h"
#include"score.h"

//マクロ定義
#define LAUNCH_GRAVITY			(-1.0f)									// 発射物の重力
#define LAUNCH_FLY				(-13.0f)								// 発射物の飛ぶ勢い
#define LAUNCH_POS				(D3DXVECTOR3(400.0f, 0.0f, 0.0f))		// 発射物の位置
#define LAUNCH_RETURN_POS		(130.0f)								// 発射物の跳ね返し可能座標
#define LAUNCH_LEVEL			(4)										// 発射物のレベル
#define LAUNCH_NUM_RANGE		(3)										// 発射物の範囲の数

// 評価関係のマクロ定義
#define LAUNCH_GOOD_RANGE		(LAUNCH_RETURN_POS)						// 発射物の範囲(最低評価)
#define LAUNCH_GREAT_RANGE		(8.0f)									// 発射物の範囲(中評価)
#define LAUNCH_PERFECT_RANGE	(-100.0f)								// 発射物の範囲(最高評価)
#define LAUNCH_GOOD_SCORE		(100)									// 発射物の範囲(最低評価)

// 跳ね返し関係のマクロ定義
#define LAUNCH_RETURN_GOOD		(D3DXVECTOR3(400.0f,0.0f,0.0f))			// 良い物の行く先
#define LAUNCH_RETURN_EVIL		(D3DXVECTOR3(700.0f,0.0f,0.0f))			// 悪い物の行く先

// プロトタイプ宣言
void FlyLaunch(Launch *pLaunch);						// 発射物の飛ぶ処理
void ReturnLaunch(Launch *pLaunch);						// 発射物の跳ね返し処理
void DistanceReturnLaunch(Launch *pLaunch);				// 発射物の距離演算処理
void LaunchReturnRange(Launch *pLaunch);				// 発射物の範囲測定処理

//グローバル変数宣言
Launch g_aLaunch[MAX_LAUNCH];		// 発射物の情報
int nSetLaunchCount;				// 発射物の設定カウント

// 発射物のレベル設定
Launch_Info g_aLaunchInfo[LAUNCH_LEVEL] =
{
	{ 0, 0.015f, -0.07f},
	{ 1, 0.022f,-0.14f},
	{ 2, 0.025f,-0.16f},
	{ 3, 0.005f,-0.5f},
};

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
		g_aLaunch[nCntLaunch].modelData.nType = LAUNCHTYPE_GOOD;						// 種類
		g_aLaunch[nCntLaunch].modelData.nState = LAUNCHSTATE_FLY;						// 状態
		g_aLaunch[nCntLaunch].modelData.bUse = false;									// 使用状況

		// 情報の初期化
		g_aLaunch[nCntLaunch].fGravity = 0.0f;			// 重力
		g_aLaunch[nCntLaunch].nScore = 0;				// スコア
		g_aLaunch[nCntLaunch].fSpeed = 0.0f;			// スピード
		g_aLaunch[nCntLaunch].nLevel = 0;				// レベル
	}

	// 設定カウントを設定する
	nSetLaunchCount = 0;
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
	if (GetKeyboardPress(DIK_0) == true)
	{ // 0キーを押している場合
		// 設定カウントを加算する
		nSetLaunchCount++;

		if (nSetLaunchCount % 30 == 0)
		{ // 0キーを押した場合

		  // 発射物の設定処理
			SetLaunch(3);
		}
	}

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{
		if (g_aLaunch[nCntLaunch].modelData.bUse == true)
		{ // 使用している場合

			// 前回の位置を記録する
			g_aLaunch[nCntLaunch].modelData.posOld = g_aLaunch[nCntLaunch].modelData.pos;

			switch (g_aLaunch[nCntLaunch].modelData.nState)
			{
			case LAUNCHSTATE_FLY:		// 飛ぶ状態

				// 重力をかける
				g_aLaunch[nCntLaunch].fGravity += g_aLaunchInfo[g_aLaunch[nCntLaunch].nLevel].fGravity;

				// 発射物の飛ぶ処理
				FlyLaunch(&g_aLaunch[nCntLaunch]);

				if (g_aLaunch[nCntLaunch].modelData.pos.x <= LAUNCH_RETURN_POS && g_aLaunch[nCntLaunch].modelData.pos.y <= 250.0f)
				{ // 位置が一定を過ぎた場合

					// 跳ね返し可能状態にする
					g_aLaunch[nCntLaunch].modelData.nState = LAUNCHSTATE_RETURN_POSSIBLE;
				}

				break;					// 抜け出す

			case LAUNCHSTATE_RETURN_POSSIBLE:		// 跳ね返し可能状態

				// 重力をかける
				g_aLaunch[nCntLaunch].fGravity += g_aLaunchInfo[g_aLaunch[nCntLaunch].nLevel].fGravity;

				// 発射物の飛ぶ処理
				FlyLaunch(&g_aLaunch[nCntLaunch]);

				// 発射物の跳ね返し処理
				ReturnLaunch(&g_aLaunch[nCntLaunch]);

				break;					// 抜け出す

			case LAUNCHSTATE_RETURN:	// 跳ね返し状態

				// 重力をかける
				g_aLaunch[nCntLaunch].fGravity += LAUNCH_GRAVITY;

				// 発射物の飛ぶ処理
				FlyLaunch(&g_aLaunch[nCntLaunch]);

				break;					// 抜け出す
			}

			if (g_aLaunch[nCntLaunch].modelData.pos.y <= -50.0f)
			{ // 位置が一定数以下になると

				// 位置を補正する
				g_aLaunch[nCntLaunch].modelData.pos.y = -50.0f;

				// 移動量を初期化する
				g_aLaunch[nCntLaunch].modelData.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// 使用しない
				g_aLaunch[nCntLaunch].modelData.bUse = false;
			}
		}
	}

	// 発射物の状態のデバッグ表示
	PrintDebugProc("発射物の状態：[%d]\n", g_aLaunch[0].modelData.nState);
	PrintDebugProc("発射物の範囲：[%d]\n", g_aLaunch[0].nScore);
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
void SetLaunch(int nLevel)
{
	Model *pModel = GetXLoadData();		// モデルの情報
	Player *pPlayer = GetPlayer();		// プレイヤーの情報を取得する
	D3DXVECTOR3 distance;				// 距離
	int nType;							// ランダムで算出する種類

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{//パーツ分繰り返す

		if (g_aLaunch[nCntLaunch].modelData.bUse == false)
		{ // 使用していない場合

			// 情報の設定
			g_aLaunch[nCntLaunch].nScore = 0;								// スコア
			g_aLaunch[nCntLaunch].nLevel = nLevel;							// レベル
			g_aLaunch[nCntLaunch].fSpeed = g_aLaunchInfo[nLevel].fFrame;	// スピード
			nType = rand() % 2;												// 良い物

			switch (nType)
			{
			case LAUNCHTYPE_GOOD:	// 良い奴

				// モデル情報を取得する
				g_aLaunch[nCntLaunch].modelData = pModel[7];

				break;				// 抜け出す

			case LAUNCHTYPE_EVIL:	// 悪い奴

				// モデル情報を取得する
				g_aLaunch[nCntLaunch].modelData = pModel[2];

				break;				// 抜け出す
			}

			// 情報の初期化
			g_aLaunch[nCntLaunch].modelData.pos = LAUNCH_POS;	// 位置
			g_aLaunch[nCntLaunch].modelData.posOld = g_aLaunch[nCntLaunch].modelData.pos;		// 前回の位置
			g_aLaunch[nCntLaunch].modelData.nType = nType;		// 種類
			g_aLaunch[nCntLaunch].modelData.nState = LAUNCHSTATE_FLY;		// 状態

			// 距離を測る
			distance.x = (pPlayer->pos.x - g_aLaunch[nCntLaunch].modelData.pos.x);
			distance.y = (pPlayer->pos.y - g_aLaunch[nCntLaunch].modelData.pos.y);

			// 移動量を決める
			distance.x *= g_aLaunch[nCntLaunch].fSpeed;

			g_aLaunch[nCntLaunch].fGravity = 40.0f;				// 重力

			// 発射物の設定
			g_aLaunch[nCntLaunch].modelData.move = D3DXVECTOR3(distance.x, 0.0f, 0.0f);	// 移動量
			g_aLaunch[nCntLaunch].modelData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
			g_aLaunch[nCntLaunch].modelData.bUse = true;								// 使用状況

			break;		// 抜け出す
		}
	}
}

//==================================================================================
//発射物の情報取得
//==================================================================================
Launch *GetLaunch(void)
{
	return &g_aLaunch[0];
}

//==================================================================================
// 発射物のレベルの取得処理
//==================================================================================
Launch_Info *GetLaundhLevel(void)
{
	// 発射物のレベルを返す
	return &g_aLaunchInfo[0];
}

//==================================================================================
// 発射物の飛ぶ処理
//==================================================================================
void FlyLaunch(Launch *pLaunch)
{
	// 重力を更新する
	pLaunch->modelData.move.y = pLaunch->fGravity;

	// 位置を更新する
	pLaunch->modelData.pos += pLaunch->modelData.move;
}

//==================================================================================
// 発射物の跳ね返し処理
//==================================================================================
void ReturnLaunch(Launch *pLaunch)
{
	Player *pPlayer = GetPlayer();		// プレイヤーの情報を取得する

	if (GetKeyboardTrigger(DIK_W) == true)
	{ // ENTERキーを押した場合

		// 重力を初期化する
		pLaunch->fGravity = 40.0f;

		// 跳ね返り状態にする
		pLaunch->modelData.nState = LAUNCHSTATE_RETURN;

		// 発射物の距離演算処理
		DistanceReturnLaunch(pLaunch);

		// 発射物の範囲測定処理
		LaunchReturnRange(pLaunch);
		PlaySound(SOUND_LABEL_SE_ARMMOVE);
		SetMotion(&pPlayer->aMotion, PLAYERMOTION_UP);

		if (LAUNCHTYPE_EVIL == pLaunch->modelData.nType)
		{
			AddScore(1);
		}
	}
	if (GetKeyboardTrigger(DIK_S) == true)
	{ // ENTERキーを押した場合

		// 重力を初期化する
		pLaunch->fGravity = 4.0f;

		// 跳ね返り状態にする
		pLaunch->modelData.nState = LAUNCHSTATE_RETURN;

		// 発射物の距離演算処理
		DistanceReturnLaunch(pLaunch);

		// 発射物の範囲測定処理
		LaunchReturnRange(pLaunch);
		PlaySound(SOUND_LABEL_SE_ARMMOVE);
		SetMotion(&pPlayer->aMotion, PLAYERMOTION_DOWN);

		if (LAUNCHTYPE_GOOD == pLaunch->modelData.nType)
		{
			AddScore(1);
		}
	}


}

//==================================================================================
// 発射物の距離演算処理
//==================================================================================
void DistanceReturnLaunch(Launch *pLaunch)
{
	D3DXVECTOR3 distance;

	// 移動量を設定する
	pLaunch->modelData.move = D3DXVECTOR3(-LAUNCH_FLY, 0.0f, 0.0f);
}

//==================================================================================
// 発射物の範囲測定処理
//==================================================================================
void LaunchReturnRange(Launch *pLaunch)
{
	// 得点を算出する
	if (pLaunch->modelData.pos.x <= LAUNCH_PERFECT_RANGE)
	{ // 範囲が PERFECT 判定内だった場合

		// スコアを設定する
		pLaunch->nScore = 300;
	}
	else if (pLaunch->modelData.pos.x <= LAUNCH_GREAT_RANGE)
	{ // 範囲が GREAT 判定内だった場合

		// スコアを設定する
		pLaunch->nScore = 200;
	}
	else if (pLaunch->modelData.pos.x <= LAUNCH_GOOD_RANGE)
	{ // 範囲が GOOD 判定内だった場合

		// スコアを設定する
		pLaunch->nScore = LAUNCH_GOOD_SCORE;
	}
}