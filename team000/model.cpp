//=============================================================================
//
// モデル処理 [model.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "minimap.h"
#include "calculation.h"
#include "debugproc.h"
#include "edit.h"

//マクロ定義
#define MOVE		(1.0f)
#define DMG_TIME	(20)		//ダメージ状態の時間
#define MODEL_LIFE	(2)

//プロトタイプ宣言
void ReadXFileModel(void);
void SaveSetModel(void);
void XLoadMeshModel(void);
void UpdateStateModel(int nCntModel);
void UpdateAlphaModel(void);
void DrawSkeltonModel(void);
void DrawUnSkeltonModel(void);
void ReadSetModel(void);

//グローバル変数宣言
ModelModel g_aModel[MAX_MODEL];		//モデルの情報
int g_nModelFileNum;				//モデルの数
Model g_aXLoadMeshModel[MAX_MODEL];	//Xファイルの読み込み
int g_nModelNum;					//モデルの数
D3DXVECTOR3 g_RandAngleModel;

//==================================================================================
//モデルの初期化処理
//==================================================================================
void InitModel(void)
{
	//各要素初期化
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aModel[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aModel[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
		g_aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);
		g_aModel[nCntModel].nIdxShadow = -1;			//影のインデックス番号
		g_aModel[nCntModel].nShadow = 1;	//影を使うかどうか
		g_aModel[nCntModel].nNumVtx = 0;			//頂点数
		g_aModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
		g_aModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値
		g_aModel[nCntModel].nState = 0;			//状態
		g_aModel[nCntModel].nCntState = 0;		//状態カウンター
		g_aModel[nCntModel].nType = 0;			//読み込むモデルの種類
		g_aModel[nCntModel].nParent = -1;		//親の番号
		g_aModel[nCntModel].nWeapon = 0;		//武器かどうか
		g_aModel[nCntModel].bUse = false;			//使用しているか
		g_aModel[nCntModel].bDisp = false;
		g_aModel[nCntModel].bSkelton = false;	//透明になるかどうか

		g_aModel[nCntModel].aXLoadMeshData.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aModel[nCntModel].aXLoadMeshData.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aModel[nCntModel].aXLoadMeshData.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
		g_aModel[nCntModel].aXLoadMeshData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aModel[nCntModel].aXLoadMeshData.rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
		g_aModel[nCntModel].aXLoadMeshData.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aModel[nCntModel].aXLoadMeshData.mtxWorld);
		D3DXMatrixIdentity(&g_aModel[nCntModel].aXLoadMeshData.OldmtxWorld);
		g_aModel[nCntModel].aXLoadMeshData.nIdxShadow = -1;			//影のインデックス番号
		g_aModel[nCntModel].aXLoadMeshData.nIdxRotate = -1;	//ぐるぐるのインデックス番号
		g_aModel[nCntModel].aXLoadMeshData.nIdxHypGauge = -1;		//体力ゲージのインデックス番号
		g_aModel[nCntModel].aXLoadMeshData.nShadow = 1;			//影を使うかどうか

		g_aModel[nCntModel].aXLoadMeshData.nNumVtx = 0;			//頂点数
		g_aModel[nCntModel].aXLoadMeshData.dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aModel[nCntModel].aXLoadMeshData.pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aModel[nCntModel].aXLoadMeshData.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
		g_aModel[nCntModel].aXLoadMeshData.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

		g_aModel[nCntModel].aXLoadMeshData.nType = 0;			//読み込むモデルの種類
		g_aModel[nCntModel].aXLoadMeshData.nParent = -1;		//親の番号
		g_aModel[nCntModel].aXLoadMeshData.bUse = false;			//使用しているか
		g_aModel[nCntModel].aXLoadMeshData.nState = 0;			//状態

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aModel[nCntModel].aXLoadMeshData.pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
			g_aModel[nCntModel].aXLoadMeshData.pMatData[nCntMat].pTextureFilename = {};	//マテリアルのデータ
			g_aModel[nCntModel].aXLoadMeshData.acFilename[nCntMat] = {};	//ファイル名
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aModel[nCntModel].aXLoadMeshData.pTexture[nCntTex] = {};	//テクスチャへのポインタ
		}
		g_aModel[nCntModel].aXLoadMeshData.dwNumMat = NULL;			//マテリアルの数
		g_aModel[nCntModel].aXLoadMeshData.pBuffMat = NULL;		//マテリアルへのポインタ
		g_aModel[nCntModel].aXLoadMeshData.pMesh = NULL;			//メッシュ(頂点情報)へのポインタ


		//Xファイル読み込みデータの初期化
		g_aXLoadMeshModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aXLoadMeshModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aXLoadMeshModel[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
		g_aXLoadMeshModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aXLoadMeshModel[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
		g_aXLoadMeshModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aXLoadMeshModel[nCntModel].mtxWorld);
		D3DXMatrixIdentity(&g_aXLoadMeshModel[nCntModel].OldmtxWorld);

		g_aXLoadMeshModel[nCntModel].nIdxShadow = -1;			//影のインデックス番号
		g_aXLoadMeshModel[nCntModel].nIdxRotate = -1;	//ぐるぐるのインデックス番号
		g_aXLoadMeshModel[nCntModel].nIdxHypGauge = -1;		//体力ゲージのインデックス番号
		g_aXLoadMeshModel[nCntModel].nIdxSerif = -1;		//セリフのインデックス番号
		g_aXLoadMeshModel[nCntModel].nShadow = 1;			//影を使うかどうか

		g_aXLoadMeshModel[nCntModel].nNumVtx = 0;			//頂点数
		g_aXLoadMeshModel[nCntModel].dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aXLoadMeshModel[nCntModel].pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aXLoadMeshModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
		g_aXLoadMeshModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

		g_aXLoadMeshModel[nCntModel].nType = 0;			//読み込むモデルの種類
		g_aXLoadMeshModel[nCntModel].nParent = -1;		//親の番号
		g_aXLoadMeshModel[nCntModel].bUse = false;			//使用しているか
		g_aXLoadMeshModel[nCntModel].nState = 0;			//状態

		g_aXLoadMeshModel[nCntModel].pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
		g_aXLoadMeshModel[nCntModel].pBuffMat = NULL;		//マテリアルへのポインタ
		g_aXLoadMeshModel[nCntModel].dwNumMat = NULL;			//マテリアルの数

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aXLoadMeshModel[nCntModel].pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
			g_aXLoadMeshModel[nCntModel].pMatData[nCntMat].pTextureFilename = NULL;	//マテリアルのデータ
			g_aXLoadMeshModel[nCntModel].acFilename[nCntMat] = NULL;	//ファイル名
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aXLoadMeshModel[nCntModel].pTexture[nCntTex] = NULL;	//テクスチャへのポインタ
		}

	}
	g_nModelFileNum = 0;
	g_nModelNum = 0;	//モデルの数
	g_RandAngleModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//モデル読み込み
	ReadXFileModel();
	ReadSetModel();
}

//==================================================================================
//モデルの終了処理
//==================================================================================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//テクスチャの破棄
			if (g_aXLoadMeshModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				g_aXLoadMeshModel[nCntModel].pTexture[nCntTex]->Release();
				g_aXLoadMeshModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_aXLoadMeshModel[nCntModel].pMesh != NULL)
		{
			g_aXLoadMeshModel[nCntModel].pMesh->Release();
			g_aXLoadMeshModel[nCntModel].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aXLoadMeshModel[nCntModel].pBuffMat != NULL)
		{
			g_aXLoadMeshModel[nCntModel].pBuffMat->Release();
			g_aXLoadMeshModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//==================================================================================
//モデルの更新処理
//==================================================================================
void UpdateModel(void)
{
	//エディット情報取得
	Edit *pEdit = GetEdit();

	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		UpdateAlphaModel();
	}

	if (GetKeyboardTrigger(DIK_F9) == true)
	{//F9を押したら,セーブ

		SaveSetModel();
	}

	PrintDebugProc("モデルの数：%d\n", g_nModelNum);
}

//==================================================================================
//モデルの状態更新処理
//==================================================================================
void UpdateStateModel(int nCntModel)
{
	//switch (g_aModel[nCntModel].nState)
	//{
	//case MODELSTATE_NONE:
	//	g_aModel[nCntModel].pos = g_aModel[nCntModel].posOrigin;
	//	break;

	//case MODELSTATE_DMG:

	//	//揺らす
	//	if (g_aModel[nCntModel].nCntState % 10 == 0)
	//	{
	//		g_aModel[nCntModel].move.x *= -1;
	//		g_aModel[nCntModel].move.z *= -1;
	//	}

	//	//ダメージカウンター減算
	//	g_aModel[nCntModel].nCntState--;

	//	g_aModel[nCntModel].move.x += sinf(g_RandAngleModel.x) * 0.4f;
	//	g_aModel[nCntModel].move.z += cosf(g_RandAngleModel.z) * 0.4f;

	//	if (g_aModel[nCntModel].nCntState <= 0)
	//	{//ダメージカウンターが0以下

	//		g_aModel[nCntModel].nState = MODELSTATE_NONE;
	//		g_aModel[nCntModel].move.x = 0.0f;
	//		g_aModel[nCntModel].move.z = 0.0f;
	//	}
	//	break;

	//case MODELSTATE_INVISIBLE:
	//	break;

	//case MODELSTATE_FIRE:

	//	
	//	break;
	//}
}

//==================================================================================
//モデルの状態更新処理
//==================================================================================
void UpdateAlphaModel(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{//使用されていたら

			//モデルとの距離
			float fLenModel =
				sqrtf((g_aModel[nCntModel].pos.x - pCamera->posV.x) * (g_aModel[nCntModel].pos.x - pCamera->posV.x)
					+ (g_aModel[nCntModel].pos.z - pCamera->posV.z) * (g_aModel[nCntModel].pos.z - pCamera->posV.z));

			if (fLenModel <= 0)
			{
				int n = 0;
			}

			//注視点との距離
			float fLenPosV =
				sqrtf((pCamera->posR.x - pCamera->posV.x) * (pCamera->posR.x - pCamera->posV.x)
					+ (pCamera->posR.z - pCamera->posV.z) * (pCamera->posR.z - pCamera->posV.z));

			if (fLenPosV <= 0)
			{
				int n = 0;
			}

			//視点から見てモデルの向き
			float fRot = atan2f((g_aModel[nCntModel].pos.x - pCamera->posV.x), (g_aModel[nCntModel].pos.z - pCamera->posV.z));

			fRot = RotNormalize(fRot);

			//目標との差分
			float fRotDiff = pCamera->rot.y - fRot;

			fRotDiff = RotNormalize(fRotDiff);

			//距離の比較
			if (fLenModel < fLenPosV &&
				fRotDiff >= -0.5f && fRotDiff <= 0.5f &&
				g_aModel[nCntModel].nType < 40)
			{//モデルと注視点の距離の方が短い

				//透明にする
				g_aModel[nCntModel].bSkelton = true;
			}
			else
			{//注視点とカメラの距離の方が短い

				//透明にはしない
				g_aModel[nCntModel].bSkelton = false;
			}


			//当たっていたら
			if (fLenModel < fLenPosV && bHitCharacter(
				&g_aModel[nCntModel].pos, &g_aModel[nCntModel].posOld, g_aModel[nCntModel].vtxMax, g_aModel[nCntModel].vtxMin,
				&pCamera->posV, D3DXVECTOR3(500.0f, 0.0f, 500.0f), D3DXVECTOR3(-500.0f, 0.0f, -500.0f)) == true &&
				g_aModel[nCntModel].nType < 40)
			{
				//透明にする
				g_aModel[nCntModel].bSkelton = true;

			}

		}
	}
}

//==================================================================================
//モデルの描画処理
//==================================================================================
void DrawModel(int nType)
{
	switch (nType)
	{
	case 0:
		//不透明モデルの描画
		DrawUnSkeltonModel();
		break;

	case 1:
		//透明モデルの描画
		DrawSkeltonModel();
		break;
	}
	
}

//==================================================================================
//不透明のモデル描画処理
//==================================================================================
void DrawUnSkeltonModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true && g_aModel[nCntModel].bSkelton == false)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			if (g_aModel[nCntModel].aXLoadMeshData.pBuffMat != NULL)
			{
				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aModel[nCntModel].aXLoadMeshData.pBuffMat->GetBufferPointer();

				if (pMat != NULL)
				{
					//頂点数分繰り返し
					for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].aXLoadMeshData.dwNumMat; nCntMat++)
					{

						if (nCntMat > MAX_TX)
						{
							int n = 9;
						}

						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, g_aModel[nCntModel].aXLoadMeshData.pTexture[nCntMat]);

						//モデル(パーツ)の描画
						g_aModel[nCntModel].aXLoadMeshData.pMesh->DrawSubset(nCntMat);
					}
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//透明のモデル描画処理
//==================================================================================
void DrawSkeltonModel(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATERIAL mInvisible;			//透明マテリアル
	ZeroMemory(&mInvisible, sizeof(D3DXMATERIAL));

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true && g_aModel[nCntModel].bSkelton == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			if (g_aModel[nCntModel].aXLoadMeshData.pBuffMat != NULL)
			{
				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aModel[nCntModel].aXLoadMeshData.pBuffMat->GetBufferPointer();

				if (pMat != NULL)
				{
					//頂点数分繰り返し
					for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].aXLoadMeshData.dwNumMat; nCntMat++)
					{
						//マテリアルの設定
						mInvisible.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, 0.3f);
						mInvisible.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, 0.3f);
						pDevice->SetMaterial(&mInvisible.MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, g_aModel[nCntModel].aXLoadMeshData.pTexture[nCntMat]);

						//モデル(パーツ)の描画
						g_aModel[nCntModel].aXLoadMeshData.pMesh->DrawSubset(nCntMat);
					}
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//モデルのファイル数取得
//==================================================================================
int GetModelFileNum(void)
{
	return g_nModelFileNum;
}

//==================================================================================
//モデルの読み込み処理
//==================================================================================
void ReadXFileModel(void)
{

	char aComment[MAX_COMMENT];	//コメント用
	int nCntModel = 0;
	int nCntFileName = 0;

	FILE *pFile = NULL;	//ファイルポインタを宣言

	//ファイルを開く
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
	}
	else if (GetMode() == MODE_TITLE)
	{
		pFile = fopen("data\\TEXT\\titleedit_info.txt", "r");
	}
	else if (GetMode() == MODE_RESULT)
	{
		pFile = fopen("data\\TEXT\\resultedit_info.txt", "r");
	}
	else if (GetMode() == MODE_RANKING)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
	}

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{//END_SCRIPTが来るまで繰り返す

			//文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//モデル数の設定
			if (strcmp(aComment, "NUM_MODEL") == 0)
			{//NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	//=の分
				fscanf(pFile, "%d", &g_nModelFileNum);	//モデル数
			}

			while (nCntFileName != g_nModelFileNum)
			{//モデルの数分読み込むまで繰り返し

				//文字列の読み込み
				fscanf(pFile, "%s", &aComment[0]);

				//モデル名の設定
				if (strcmp(aComment, "MODEL_FILENAME") == 0)
				{//NUM_MODELがきたら

					fscanf(pFile, "%s", &aComment[0]);	//=の分
					fscanf(pFile, "%s", &g_aXLoadMeshModel[nCntFileName].acFilename[0]);	//ファイル名
					nCntFileName++;	//ファイル数加算
				}
			}
			

			//モデルの設定
			if (strcmp(aComment, "MODELSET") == 0)
			{//モデルの読み込みを開始

				while (strcmp(aComment, "END_MODELSET"))
				{//END_MODELSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "TYPE") == 0)
					{//TYPEが来たら種類読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aModel[nCntModel].nType);	//モデル種類の列挙
					}

					if (strcmp(aComment, "POS") == 0)
					{//POSが来たら位置読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.x);	//X座標
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.y);	//Y座標
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.z);	//Z座標
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROTが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.x);	//Xの向き
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.y);	//Yの向き
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.z);	//Zの向き
					}

					if (strcmp(aComment, "SHADOW") == 0)
					{//SHADOWが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);	//=の分
						fscanf(pFile, "%d", &g_aModel[nCntModel].nShadow);		//影を使うかどうか
					}

				}//END_MODELSETのかっこ

				//モデルの番号加算
				nCntModel++;
			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//終了文字でループを抜ける

				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	//Xファイル読み込み
	XLoadMeshModel();
}

//==================================================================================
//モデルの読み込み処理
//==================================================================================
void ReadSetModel(void)
{

	char aComment[MAX_COMMENT];	//コメント用
	int nCntModel = 0;
	int nCntFileName = 0;

	FILE *pFile = NULL;	//ファイルポインタを宣言

	//ファイルを開く
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
	}
	else if (GetMode() == MODE_TITLE)
	{
		pFile = fopen("data\\TEXT\\titleedit_info.txt", "r");
	}
	else if (GetMode() == MODE_RESULT)
	{
		pFile = fopen("data\\TEXT\\resultedit_info.txt", "r");
	}
	else if (GetMode() == MODE_RANKING)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
	}

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{//END_SCRIPTが来るまで繰り返す

			//文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//モデルの設定
			if (strcmp(aComment, "MODELSET") == 0)
			{//モデルの読み込みを開始

				while (strcmp(aComment, "END_MODELSET"))
				{//END_MODELSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "TYPE") == 0)
					{//TYPEが来たら種類読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aModel[nCntModel].nType);	//モデル種類の列挙
					}

					if (strcmp(aComment, "POS") == 0)
					{//POSが来たら位置読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.x);	//X座標
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.y);	//Y座標
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.z);	//Z座標
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROTが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.x);	//Xの向き
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.y);	//Yの向き
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.z);	//Zの向き
					}

					if (strcmp(aComment, "SHADOW") == 0)
					{//SHADOWが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);	//=の分
						fscanf(pFile, "%d", &g_aModel[nCntModel].nShadow);		//影を使うかどうか
					}

				}//END_MODELSETのかっこ

				 //モデルの番号加算
				nCntModel++;
			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//終了文字でループを抜ける

				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	for (int nCount = 0; nCount < nCntModel; nCount++)
	{
		//モデルの配置
		SetModel(g_aModel[nCount].pos, g_aModel[nCount].rot, g_aModel[nCount].nType, g_aModel[nCount].nShadow);
	}

}

//==================================================================================
//モデル情報の保存処理
//==================================================================================
void SaveSetModel(void)
{

	FILE *pFile = NULL;	//ファイルポインタを宣言

	//ファイルを開く
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "w");
	}
	if (GetMode() == MODE_TITLE)
	{
		pFile = fopen("data\\TEXT\\titleedit_info.txt", "w");
	}
	else if (GetMode() == MODE_RESULT)
	{
		pFile = fopen("data\\TEXT\\resultedit_info.txt", "w");
	}
	else if (GetMode() == MODE_RANKING)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "w");
	}

	if (pFile != NULL)
	{//ファイルが開けた場合

		//ファイルに書き出す
		fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# モデル数\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", g_nModelFileNum);

		fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# モデルファイル名\n"
		"#------------------------------------------------------------------------------\n");

		//モデルファイル名分
		for (int nCntFileNum = 0; nCntFileNum < g_nModelFileNum; nCntFileNum++)
		{
			fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", &g_aXLoadMeshModel[nCntFileNum].acFilename[0], nCntFileNum);
		}

		fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# モデルの配置\n"
		"#==============================================================================\n");

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			if (g_aModel[nCntModel].bUse == true)
			{//使用されていたら

				fprintf(pFile,
					"MODELSET\n"
					"\tTYPE = %d\n"
					"\tPOS = %.2f %.2f %.2f\n"
					"\tROT = %.2f %.2f %.2f\n"
					"\tSHADOW = %d\n"
					"END_MODELSET\n\n", 
					g_aModel[nCntModel].nType,
					g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z,
					g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.z,
					g_aModel[nCntModel].nShadow);
			}
		}

		fprintf(pFile, "END_SCRIPT		# この行は絶対消さないこと！");

		//ファイルを閉じる
		fclose(pFile);
	}
}

//==================================================================================
//Xファイルの読み込み処理
//==================================================================================
void XLoadMeshModel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntFileNum = 0; nCntFileNum < g_nModelFileNum; nCntFileNum++)
	{//モデル数分読み込む

		//Xファイルの読み込み
		HRESULT hr = D3DXLoadMeshFromX(&g_aXLoadMeshModel[nCntFileNum].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aXLoadMeshModel[nCntFileNum].pBuffMat,
			NULL,
			&g_aXLoadMeshModel[nCntFileNum].dwNumMat,
			&g_aXLoadMeshModel[nCntFileNum].pMesh);

		if (hr == E_FAIL)
		{
			int nnnnn = 0;
		}

		D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aXLoadMeshModel[nCntFileNum].pBuffMat->GetBufferPointer();

		//頂点数分繰り返し
		for (int nCntMat = 0; nCntMat < (int)g_aXLoadMeshModel[nCntFileNum].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//ファイルが存在する

				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aXLoadMeshModel[nCntFileNum].pTexture[nCntMat]);
			}
		}
	}
}

//==================================================================================
//モデルの情報取得
//==================================================================================
ModelModel *GetModel(void)
{
	return &g_aModel[0];
}

//==================================================================================
//Xファイルの読み込み情報取得
//==================================================================================
Model *GetXLoadData(void)
{
	return &g_aXLoadMeshModel[0];
}

//==================================================================================
//モデル数取得
//==================================================================================
int GetModelNum(void)
{
	return g_nModelNum;
}

//==================================================================================
//モデルの配置処理
//==================================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nShadow)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{//使用されていなかったら

			//モデル情報取得
			g_aModel[nCntModel].aXLoadMeshData = g_aXLoadMeshModel[nType];

			//モデルの全頂点チェック
			CheckVtx(g_aModel[nCntModel].rot.y, &g_aModel[nCntModel].vtxMax, &g_aModel[nCntModel].vtxMin, g_aXLoadMeshModel[nType].pMesh, g_aXLoadMeshModel[nType].pVtxBuff);

			//if (nType >= SMALL_NUM)
			//{//小物は判定削除

			//	g_aModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);
			//	g_aModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
			//}

			g_aModel[nCntModel].pos = pos;
			g_aModel[nCntModel].posOrigin = pos;
			g_aModel[nCntModel].rot = rot;
			g_aModel[nCntModel].nType = nType;
			g_aModel[nCntModel].bUse = true;	//使用状態にする
			g_aModel[nCntModel].bDisp = true;
			//g_aModel[nCntModel].nState = MODELSTATE_NONE;
			g_nModelNum++;

			////影の位置更新
			//SetPositionShadow(g_aModel[nCntModel].nIdxShadow, g_aModel[nCntModel].pos);

			if ((GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL) && (g_aModel[nCntModel].nType < SMALL_NUM || g_aModel[nCntModel].nType >= CONE_NUM))
			{
				//ミニマップ上のビル設置
				SetMiniMap(g_aModel[nCntModel].pos, g_aModel[nCntModel].vtxMax, g_aModel[nCntModel].vtxMin);
			}
			break;
		}
	}
}