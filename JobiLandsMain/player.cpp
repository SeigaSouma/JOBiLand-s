//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "meshwall.h"
#include "game.h"
#include "edit.h"
#include "tutorial.h"
#include "impactwave.h"
#include "calculation.h"
#include "shadow.h"
#include "particle.h"
#include "debugproc.h"
#include "sound.h"
#include "motion.h"
#include "launch.h"

//マクロ定義
#define DMG_TIME	(10)	//ダメージ状態の時間
#define ULT_WAIT	(100)	//待ち状態
#define FLAG_POS	(150.0f)	//旗の位置
#define FLEN		(800.0f)
#define PLAYER_TXT	"data\\TEXT\\motion_set_player.txt"

//プロトタイプ宣言
void UpdateGamePlayer(void);
void ControllPlayer(void);
void ReadSetPlayer(void);
void ReadSetMotionPlayer(void);
void CollisionCharPlayer(void);
void UpdateStatePlayer(void);
void UpdateATKPlayer(void);
void CollisionATKPlayer(float fDistance, float fAngle, int nValue);

//グローバル変数宣言
Player g_aPlayer;		//プレイヤーの情報
int g_nGameTimePlayer;

//==================================================================================
//プレイヤーの初期化処理
//==================================================================================
void InitPlayer(void)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各要素初期化
	g_aPlayer.pos = D3DXVECTOR3(-200, 0.0f, 0.0f);
	g_aPlayer.posOld = g_aPlayer.pos;
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.bUse = true;										//使用しているか

	g_aPlayer.vecMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動ベクトル
	g_aPlayer.vecToPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//ベクトルの長さ
	g_aPlayer.fRotDiff = 0.0f;									//向きの差分
	g_aPlayer.fRotDest = g_aPlayer.rot.y;						//向きの差分

																//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);
	g_aPlayer.nIdxShadow = -1;									//影のインデックス番号
	g_aPlayer.nIdxHPGauge = -1;									//体力ゲージのインデックス番号
	g_aPlayer.nState = PLAYERSTATE_NONE;						//状態
	g_aPlayer.nCntState = 0;									//状態カウント
	g_aPlayer.bDisp = true;										//描画しているか

	//モーション系初期化
	g_aPlayer.aMotion.nNowMotionNum = -1;		//現在のモーション番号
	g_aPlayer.aMotion.nCntAllFrame = 0;			//総フレームカウント
	g_aPlayer.aMotion.nCntFrame = 0;			//フレームのカウント
	g_aPlayer.aMotion.nPatternKey = 0;			//何個目のキーか
	g_aPlayer.aMotion.nPartsNum = 0;			//パーツ数
	g_aPlayer.bMove = false;			//移動しているか
	g_aPlayer.bJump = false;			//ジャンプしているか
	g_aPlayer.bATK = false;				//攻撃してるか

	g_aPlayer.nCntWalk = 0;				//移動のカウント
	g_aPlayer.nCntPenlight = 0;			//催眠のカウント
	g_aPlayer.nCntDis = 0;				//派遣のカウント
	g_aPlayer.nCntReturn = 0;			//帰還のカウント
	g_aPlayer.fRadius = 50.0f;			//半径

	for (int nCount = 0; nCount < MAX_MODEL; nCount++)
	{
		g_aPlayer.aModel[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//現在の位置
		g_aPlayer.aModel[nCount].posOld = g_aPlayer.aModel[nCount].pos;			//前回の位置
		g_aPlayer.aModel[nCount].posOrigin = g_aPlayer.aModel[nCount].pos;		//元の位置
		g_aPlayer.aModel[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
		g_aPlayer.aModel[nCount].rotOrigin = g_aPlayer.aModel[nCount].rot;		//元の向き
		g_aPlayer.aModel[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量

																				//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].OldmtxWorld);

		g_aPlayer.aModel[nCount].nIdxShadow = -1;		//影のインデックス番号
		g_aPlayer.aModel[nCount].nShadow = -1;			//影を使うかどうか

		g_aPlayer.aModel[nCount].nNumVtx = 0;			//頂点数
		g_aPlayer.aModel[nCount].dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aPlayer.aModel[nCount].pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aPlayer.aModel[nCount].vtxMin = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);		//モデルの最小値
		g_aPlayer.aModel[nCount].vtxMax = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);			//モデルの最大値

		g_aPlayer.aModel[nCount].nType = -1;		//読み込むモデルの種類
		g_aPlayer.aModel[nCount].nParent = -1;		//親の番号
		g_aPlayer.aModel[nCount].bUse = false;		//使用しているか
		g_aPlayer.aModel[nCount].nState = 0;		//状態

		g_aPlayer.aModel[nCount].pMesh = NULL;				//メッシュ(頂点情報)へのポインタ
		g_aPlayer.aModel[nCount].pBuffMat = NULL;			//マテリアルへのポインタ

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aPlayer.aModel[nCount].pMatData[nCntMat].MatD3D = {};
			g_aPlayer.aModel[nCount].pMatData[nCntMat].pTextureFilename = {};	//マテリアルのデータ
			g_aPlayer.aModel[nCount].acFilename[nCntMat] = {};		//ファイル名
		}

		g_aPlayer.aModel[nCount].dwNumMat = NULL;			//マテリアルの数

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aPlayer.aModel[nCount].pTexture[nCntTex] = {};		//テクスチャへのポインタ
		}

	}

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//モーションの数繰り返す

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{//キーの数繰り返す

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{//パーツの数繰り返す

				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の向き
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向きの差分
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の位置
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の差分

			}

			g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].nFrame = 0;	//再生フレーム
		}

		g_aPlayer.aMotionData[nCntMotion].nNumKey = 0;				//キーの数
		g_aPlayer.aMotionData[nCntMotion].nLoop = MOTION_LOOP_ON;	//ループ判定
		g_aPlayer.aMotionData[nCntMotion].bUse = false;				//使用しているか
	}

	g_nGameTimePlayer = 0;

	//外部ファイル読み込み
	ReadSetPlayer();
	ReadSetMotionPlayer();

	//モーションの設定
	SetMotion(&g_aPlayer.aMotion, PLAYERMOTION_DEF);
}

//==================================================================================
//プレイヤーの終了処理
//==================================================================================
void UninitPlayer(void)
{

	for (int nCount = 0; nCount < MAX_MODEL; nCount++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//テクスチャの破棄
			if (g_aPlayer.aModel[nCount].pTexture[nCntTex] != NULL)
			{
				g_aPlayer.aModel[nCount].pTexture[nCntTex]->Release();
				g_aPlayer.aModel[nCount].pTexture[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_aPlayer.aModel[nCount].pMesh != NULL)
		{
			g_aPlayer.aModel[nCount].pMesh->Release();
			g_aPlayer.aModel[nCount].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aPlayer.aModel[nCount].pBuffMat != NULL)
		{
			g_aPlayer.aModel[nCount].pBuffMat->Release();
			g_aPlayer.aModel[nCount].pBuffMat = NULL;
		}
	}

}

//==================================================================================
//プレイヤーの更新処理
//==================================================================================
void UpdatePlayer(void)
{
	switch (GetMode())
	{
	case MODE_GAME:

		//ゲーム時の更新処理
		UpdateGamePlayer();
		break;
	}
}

//==================================================================================
//ゲーム時の更新処理
//==================================================================================
void UpdateGamePlayer(void)
{
	if (g_aPlayer.bUse == true)
	{//使用していたら

		//過去の位置保存
		g_aPlayer.posOld = g_aPlayer.pos;

		for (int nCntParts = 0; nCntParts < g_aPlayer.aMotion.nPartsNum; nCntParts++)
		{//パーツ分繰り返す

			//パーツごとのマトリックス取得
			D3DXMATRIX mtxParts = GetParentMatrix(nCntParts);

			g_aPlayer.aModel[nCntParts].OldmtxWorld = mtxParts;
		}

		//プレイヤーの操作
		ControllPlayer();

		//攻撃処理
		UpdateATKPlayer();

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//移動してるとき

			g_aPlayer.nCntWalk++;	//カウント加算

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//歩行パーティクルの設定
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				//PlaySound(SOUND_LABEL_SE_ASIOTO);
			}

			//モーションの設定
			//SetMotion(&g_aPlayer.aMotion, PLAYERMOTION_WALK);
		}
		else
		{//ニュートラルモーション

			if (g_aPlayer.aMotion.nNowMotionNum != PLAYERMOTION_ACTION)
			{
				//攻撃状態解除
				g_aPlayer.bATK = false;

				//モーションの設定
				SetMotion(&g_aPlayer.aMotion, PLAYERMOTION_DEF);
			}
		}

		//モーションの更新
		UpdateMotion(&g_aPlayer.aMotionData[0], &g_aPlayer.aMotion, &g_aPlayer.aModel[0], &g_aPlayer.pos);

		//キャラクターとの当たり判定
		CollisionCharPlayer();

		//影の位置更新
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
	}

	//状態更新
	UpdateStatePlayer();
}

//==================================================================================
//ゲーム終了時の更新処理
//==================================================================================
void ControllEndPlayer(void)
{
	////カメラの情報取得
	//Camera *pCamera = GetCamera();

	//g_aPlayer.bMove = true;

	//g_aPlayer.rot.y = 0.0f;

	//g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
	//g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
	//g_aPlayer.fRotDest = pCamera->rot.y;

	////重力処理
	////g_aPlayer.move.y -= 1.0f;

	////位置更新
	//g_aPlayer.pos += g_aPlayer.move;

	////慣性
	//g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
	//g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;
}

//==================================================================================
//モデルの操作
//==================================================================================
void ControllPlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//エディットの情報取得
	Edit  *pEdit = GetEdit();

	if (GetGameState() == GAMESTATE_NONE && pEdit->bUse == false)
	{//ターゲット中以外

		if (GetKeyboardRelease(DIK_W) == true)
		{//Wが押された,嫌なもの

			SetMotion(&g_aPlayer.aMotion, PLAYERMOTION_ACTION);
		}

		if (GetKeyboardRelease(DIK_S) == true)
		{//Wが押された,嫌なもの

			SetMotion(&g_aPlayer.aMotion, PLAYERMOTION_ACTION);
		}
	}

	PrintDebugProc(
		"\n------プレイヤーの操作------\n"
		"<はじき返し(仮) [ENTER]>\n"
		"<位置> [%f, %f, %f]\n",
		g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);

}

//==================================================================================
//攻撃処理
//==================================================================================
void UpdateATKPlayer(void)
{
	switch (g_aPlayer.aMotion.nNowMotionNum)
	{
	case PLAYERMOTION_ACTION:

		//攻撃の当たり判定
		CollisionATKPlayer(PENLIGHT_RADIUS, PENLIGHT_ANGLE, 1);
		break;
	}
}

//==================================================================================
//プレイヤーの攻撃
//==================================================================================
void CollisionATKPlayer(float fDistance, float fAngle, int nValue)
{
	//ペンライトの攻撃
	bool bAtk = false;

	//ペンライトの当たり判定
	/*bAtk = CollisionFan(g_aPlayer.pos,
	D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y) * -fDistance),
	D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y) * -fDistance),
	pMob->aModel.pos, pMob->aModel.posOld);*/

	if (bAtk == true)
	{//扇の範囲に入ったら

	 //モブのヒット処理
	 //HitMob(nCntMob, nValue);
	}
}

//==================================================================================
//プレイヤーの状態更新
//==================================================================================
void UpdateStatePlayer(void)
{

	D3DXMATERIAL *pMat;	//マテリアルデータへのポインタ

	switch (g_aPlayer.nState)
	{
	case PLAYERSTATE_NONE:

		for (int nCntParts = 0; nCntParts < g_aPlayer.aMotion.nPartsNum; nCntParts++)
		{//パーツ分繰り返す

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
			{
				//元の色に戻す
				pMat[nCntMat].MatD3D.Diffuse = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Ambient = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Ambient;
			}
		}
		break;

	case PLAYERSTATE_DMG:

		//ダメージカウンター減算
		g_aPlayer.nCntState--;

		if (g_aPlayer.nCntState <= 0)
		{//ダメージカウンターが0以下

			//状態をもとに戻す
			g_aPlayer.nState = PLAYERSTATE_INVINCIBLE;
			g_aPlayer.nCntState = 30;

			for (int nCntParts = 0; nCntParts < g_aPlayer.aMotion.nPartsNum; nCntParts++)
			{//パーツ分繰り返す

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

				//頂点数分繰り返し
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
				{
					//元の色に戻す
					pMat[nCntMat].MatD3D.Diffuse = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Diffuse;
					pMat[nCntMat].MatD3D.Ambient = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Ambient;
				}
			}
		}

		break;

	case PLAYERSTATE_INVINCIBLE:

		//ダメージカウンター減算
		g_aPlayer.nCntState--;

		for (int nCntParts = 0; nCntParts < g_aPlayer.aMotion.nPartsNum; nCntParts++)
		{//パーツ分繰り返す

			D3DXMATERIAL *pMat;	//マテリアルデータへのポインタ

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
			{

				if (g_aPlayer.nCntState % 2 == 0)
				{//元の色に戻す

					pMat[nCntMat].MatD3D.Diffuse = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Diffuse;
					pMat[nCntMat].MatD3D.Ambient = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Ambient;
				}
				else
				{//透明

					pMat[nCntMat].MatD3D.Diffuse.a = 0.5f;
					pMat[nCntMat].MatD3D.Ambient.a = 0.5f;
				}
			}
		}

		if (g_aPlayer.nCntState <= 0)
		{//カウンターが0以下

			//状態をもとに戻す
			g_aPlayer.nState = PLAYERSTATE_NONE;
		}
		break;
	}
}

//==================================================================================
//キャラクターとの当たり判定
//==================================================================================
void CollisionCharPlayer(void)
{
	//モデルの情報取得
	ModelModel *pModel = GetModel();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true)
		{//モデルが使用されていたら

			//キャラクター同士の当たり判定
			CollisionCharacter(
				&g_aPlayer.pos, &g_aPlayer.posOld, D3DXVECTOR3(g_aPlayer.fRadius, 0.0f, g_aPlayer.fRadius), D3DXVECTOR3(-g_aPlayer.fRadius, 0.0f, -g_aPlayer.fRadius),
				&pModel->posOrigin, pModel->vtxMax, pModel->vtxMin);
		}
	}

	//発射物の情報取得
	Launch *pLaunch = GetLaunch();

	bool bHit = false;

	if (g_aPlayer.nState == PLAYERSTATE_NONE)
	{
		for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++, pLaunch++)
		{
			if (pLaunch->modelData.bUse == true)
			{//モデルが使用されていたら

				//キャラクター同士の当たり判定
				bHit = bHitCharacter(
					&g_aPlayer.pos, &g_aPlayer.posOld, D3DXVECTOR3(g_aPlayer.fRadius, 0.0f, g_aPlayer.fRadius), D3DXVECTOR3(-g_aPlayer.fRadius, 0.0f, -g_aPlayer.fRadius),
					&pLaunch->modelData.pos, pLaunch->modelData.vtxMax, pLaunch->modelData.vtxMin);

				if (bHit == true)
				{//当たったら

					//プレイヤーのヒット処理
					HitPlayer();

					break;
				}
			}
		}
	}
}

//==================================================================================
//弾のヒット処理
//==================================================================================
void HitPlayer(void)
{

	if (g_aPlayer.nState == PLAYERSTATE_NONE)
	{//ウルト状態じゃないとき

		g_aPlayer.nState = PLAYERSTATE_DMG;	//ダメージ状態へ
		g_aPlayer.nCntState = DMG_TIME;		//ダメージ状態を保つ時間を与える

		//サウンド再生
		//PlaySound(SOUND_LABEL_SE_DMG);

		for (int nCntParts = 0; nCntParts < g_aPlayer.aMotion.nPartsNum; nCntParts++)
		{//パーツ分繰り返す

			D3DXMATERIAL *pMat;	//マテリアルデータへのポインタ

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
			{//マテリアルを赤くする

				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			}
		}
	}
}

//==================================================================================
//プレイヤーの描画処理
//==================================================================================
void DrawPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATRIX mtxnParent;			//親の計算用マトリックス

	if (g_aPlayer.bDisp == true && g_aPlayer.bUse == true)
	{//使用していたら

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//向きを反映する
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//位置を反映する
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCount = 0; nCount < g_aPlayer.aMotion.nPartsNum; nCount++)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCount].rot.y, g_aPlayer.aModel[nCount].rot.x, g_aPlayer.aModel[nCount].rot.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCount].pos.x, g_aPlayer.aModel[nCount].pos.y, g_aPlayer.aModel[nCount].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxTrans);


			if (g_aPlayer.aModel[nCount].nParent == -1)
			{//NULLの場合,親

			 //プレイヤーの向きを反映する
				mtxnParent = g_aPlayer.mtxWorld;
			}
			else
			{//パーツ

			 //親の向きを反映する
				mtxnParent = g_aPlayer.aModel[g_aPlayer.aModel[nCount].nParent].mtxWorld;
			}

			//自分のやつに親のワールドマトリックスを掛け算
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxnParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCount].mtxWorld);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCount].pBuffMat->GetBufferPointer();

			if (g_aPlayer.aModel[nCount].bUse == true)
			{//パーツが使用されていたら

				//頂点数分繰り返し
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCount].dwNumMat; nCntMat++)
				{
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0, g_aPlayer.aModel[nCount].pTexture[nCntMat]);

					//プレイヤー(パーツ)の描画
					g_aPlayer.aModel[nCount].pMesh->DrawSubset(nCntMat);
				}
			}
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

}

//==================================================================================
//プレイヤーの設定処理
//==================================================================================
void SetPlayer(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParts = 0; nCntParts < g_aPlayer.aMotion.nPartsNum; nCntParts++)
	{//パーツ分繰り返す

	 //Xファイルの読み込み
		D3DXLoadMeshFromX(&g_aPlayer.aModel[nCntParts].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aPlayer.aModel[nCntParts].pBuffMat,
			NULL,
			&g_aPlayer.aModel[nCntParts].dwNumMat,
			&g_aPlayer.aModel[nCntParts].pMesh);

		D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

		//頂点数分繰り返し
		for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
		{
			//マテリアルのデータ取得
			g_aPlayer.aModel[nCntParts].pMatData[nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//ファイルが存在する

				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aPlayer.aModel[nCntParts].pTexture[nCntMat]);
			}
		}

		//パーツを使用状態
		g_aPlayer.aModel[nCntParts].bUse = true;

		//モデルの全頂点チェック
		CheckVtx(g_aPlayer.aModel[nCntParts].rot.y, &g_aPlayer.aModel[nCntParts].vtxMax, &g_aPlayer.aModel[nCntParts].vtxMin, g_aPlayer.aModel[nCntParts].pMesh, g_aPlayer.aModel[nCntParts].pVtxBuff);
	}

	//影の設定
	g_aPlayer.nIdxShadow = SetShadow(g_aPlayer.fRadius * 0.5f, g_aPlayer.fRadius * 0.5f);
	SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
}

//==================================================================================
//絶対マトリックスの取得
//==================================================================================
D3DXMATRIX GetParentMatrix(int nIdxParts)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxnParent;			//親の計算用マトリックス

									//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

	//向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

	//位置を反映する
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

	for (int nCount = 0; nCount < g_aPlayer.aMotion.nPartsNum; nCount++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);

		//向きを反映する
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCount].rot.y, g_aPlayer.aModel[nCount].rot.x, g_aPlayer.aModel[nCount].rot.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxRot);

		//位置を反映する
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCount].pos.x, g_aPlayer.aModel[nCount].pos.y, g_aPlayer.aModel[nCount].pos.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxTrans);


		if (g_aPlayer.aModel[nCount].nParent == -1)
		{//NULLの場合,親

		 //プレイヤーの向きを反映する
			mtxnParent = g_aPlayer.mtxWorld;
		}
		else
		{//パーツ

		 //親の向きを反映する
			mtxnParent = g_aPlayer.aModel[g_aPlayer.aModel[nCount].nParent].mtxWorld;
		}

		//自分のやつに親のワールドマトリックスを掛け算
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxnParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCount].mtxWorld);
	}

	return g_aPlayer.aModel[nIdxParts].mtxWorld;
}

//==================================================================================
//プレイヤーの読み込み処理
//==================================================================================
void ReadSetPlayer(void)
{
	FILE *pFile = NULL;	//ファイルポインタを宣言

						//ファイルを開く
	pFile = fopen(PLAYER_TXT, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		char aComment[MAX_COMMENT];	//コメント、確認等

		int nCntSetParts = 0;
		int nCntFileName = 0;

		while (1)
		{//END_SCRIPTが来るまで繰り返す

		 //文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//モデル数の設定
			if (strcmp(aComment, "NUM_MODEL") == 0)
			{//NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	//=の分
				fscanf(pFile, "%d", &g_aPlayer.aMotion.nPartsNum);	//モデル数
			}

			while (nCntFileName != g_aPlayer.aMotion.nPartsNum)
			{//モデルの数分読み込むまで繰り返し

			 //文字列の読み込み
				fscanf(pFile, "%s", &aComment[0]);

				//モデル名の設定
				if (strcmp(aComment, "MODEL_FILENAME") == 0)
				{//NUM_MODELがきたら

					fscanf(pFile, "%s", &aComment[0]);	//=の分
					fscanf(pFile, "%s", &g_aPlayer.aModel[nCntFileName].acFilename[0]);	//ファイル名
					nCntFileName++;	//ファイル数加算
				}
			}

			//各パーツの設定
			if (strcmp(aComment, "CHARACTERSET") == 0)
			{//キャラクター情報の読み込みを開始

				while (strcmp(aComment, "END_CHARACTERSET") != 0)
				{//END_CHARACTERSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "PARTSSET") == 0)
					{//PARTSSETでパーツ情報の読み込み開始

						while (strcmp(aComment, "END_PARTSSET") != 0)
						{//END_PARTSSETが来るまで繰り返す

							fscanf(pFile, "%s", &aComment[0]);	//確認する

							if (strcmp(aComment, "INDEX") == 0)
							{//INDEXが来たらモデル番号読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nType);	//モデル種類の列挙

							}

							if (strcmp(aComment, "PARENT") == 0)
							{//PARENTが来たら親の番号読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nParent);	//親の番号

							}

							if (strcmp(aComment, "POS") == 0)
							{//POSが来たら位置読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.x);	//X座標
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.y);	//Y座標
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.z);	//Z座標

																							//元の位置割り当て
								g_aPlayer.aModel[nCntSetParts].posOrigin = g_aPlayer.aModel[nCntSetParts].pos;

							}

							if (strcmp(aComment, "ROT") == 0)
							{//ROTが来たら向き読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.x);	//Xの角度
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.y);	//Yの角度
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.z);	//Zの角度

							}

							if (strcmp(aComment, "WEAPON") == 0)
							{//WEAPONが来たら武器かどうか読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分

							}

						}//END_PARTSSETのかっこ

						nCntSetParts++;	//パーツのカウントを加算
					}

				}//END_CHARACTERSETのかっこ

			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//終了文字でループを抜ける

				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	//プレイヤーの配置
	SetPlayer();

}

//==================================================================================
//プレイヤーの読み込み処理
//==================================================================================
void ReadSetMotionPlayer(void)
{

	FILE *pFile = NULL;	//ファイルポインタを宣言

	char aComment[MAX_COMMENT];	//コメント用
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0;

	//ファイルを開く
	pFile = fopen(PLAYER_TXT, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{//END_SCRIPTが来るまで繰り返す

		 //文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//==================================================
			//各モーションの設定
			//==================================================
			if (strcmp(aComment, "MOTIONSET") == 0)
			{//モーション情報の読み込みを開始

				while (strcmp(aComment, "END_MOTIONSET") != 0)
				{//END_MOTIONSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "LOOP") == 0)
					{//LOOPが来たらループON/OFF読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aPlayer.aMotionData[nCntMotion].nLoop);	//ループ0か1

					}

					if (strcmp(aComment, "NUM_KEY") == 0)
					{//NUM_KEYが来たらキー数読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aPlayer.aMotionData[nCntMotion].nNumKey);	//キー数

					}

					//==================================================
					//各キーの設定
					//==================================================
					if (strcmp(aComment, "KEYSET") == 0)
					{//KEYSETでキー情報の読み込み開始

						while (strcmp(aComment, "END_KEYSET") != 0)
						{//END_KEYSETが来るまで繰り返し

							fscanf(pFile, "%s", &aComment[0]);	//確認する

							if (strcmp(aComment, "FRAME") == 0)
							{//FRAMEが来たら再生フレーム数読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//再生フレーム

							}

							//==================================================
							//パーツの設定
							//==================================================
							if (strcmp(aComment, "PARTS") == 0)
							{//PARTSでパーツ情報の読み込み開始

								while (strcmp(aComment, "END_PARTS") != 0)
								{//END_PARTSが来るまで繰り返し

									fscanf(pFile, "%s", &aComment[0]);	//確認する

									if (strcmp(aComment, "POS") == 0)
									{//POSが来たら位置読み込み

										fscanf(pFile, "%s", &aComment[0]);		//=の分
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X座標
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y座標
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z座標
									}

									if (strcmp(aComment, "ROT") == 0)
									{//ROTが来たら角度読み込み

										fscanf(pFile, "%s", &aComment[0]);		//=の分
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X角度
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y角度
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z角度
									}

								}//END_PARTSのかっこ

								nCntParts++;	//パーツのカウント加算
							}


						}//END_KEYSETのかっこ

						nCntParts = 0;	//パーツのカウントリセット
						nCntKey++;	//キーのカウント加算
					}

				}//END_MOTIONSETのかっこ

				nCntKey = 0;	//キーのカウントリセット
				nCntMotion++;	//モーションのカウント加算

			}


			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//終了文字でループを抜ける

				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//==================================================================================
//プレイヤーの情報取得
//==================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}