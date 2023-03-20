//=============================================================================
//
// モデル処理 [edit.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "edit.h"
#include "input.h"
#include "camera.h"
#include "game.h"

#include "calculation.h"

//マクロ定義
#define MOVE		(5.0f)
#define MOVE_SLOW	(1.0f)

//プロトタイプ宣言
void ControllEdit(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot);
void ChangeEditType(void);
void GrabEditModel(void);
void DeleteEditModel(int nIdxDelete);

//グローバル変数宣言
Edit g_aEdit;	//モデルの情報

//==================================================================================
//エディットの初期化処理
//==================================================================================
void InitEdit(void)
{
	//各要素初期化
	g_aEdit.pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);	//現在の位置
	g_aEdit.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	g_aEdit.nNowModelType = 0;	//今のモデル種類

	//ワールドマトリックス
	D3DXMatrixIdentity(&g_aEdit.mtxWorld);
	g_aEdit.nShadow = 1;		//影使うかどうか[0か1]
	g_aEdit.bUse = false;			//使用しているか

	for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	{
		g_aEdit.aXLoadMeshData[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aEdit.aXLoadMeshData[nCntEdit].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aEdit.aXLoadMeshData[nCntEdit].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
		g_aEdit.aXLoadMeshData[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aEdit.aXLoadMeshData[nCntEdit].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
		g_aEdit.aXLoadMeshData[nCntEdit].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

		//ワールドマトリックス
		D3DXMatrixIdentity(&g_aEdit.aXLoadMeshData[nCntEdit].mtxWorld);
		D3DXMatrixIdentity(&g_aEdit.aXLoadMeshData[nCntEdit].OldmtxWorld);

		g_aEdit.aXLoadMeshData[nCntEdit].nIdxShadow = -1;			//影のインデックス番号
		g_aEdit.aXLoadMeshData[nCntEdit].nShadow = 1;			//影を使うかどうか

		g_aEdit.aXLoadMeshData[nCntEdit].nNumVtx = 0;			//頂点数
		g_aEdit.aXLoadMeshData[nCntEdit].dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aEdit.aXLoadMeshData[nCntEdit].pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aEdit.aXLoadMeshData[nCntEdit].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
		g_aEdit.aXLoadMeshData[nCntEdit].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

		g_aEdit.aXLoadMeshData[nCntEdit].nType = 0;			//読み込むモデルの種類
		g_aEdit.aXLoadMeshData[nCntEdit].nParent = -1;		//親の番号
		g_aEdit.aXLoadMeshData[nCntEdit].bUse = false;			//使用しているか
		g_aEdit.aXLoadMeshData[nCntEdit].nState = 0;			//状態

		g_aEdit.aXLoadMeshData[nCntEdit].pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
		g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat = NULL;		//マテリアルへのポインタ

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aEdit.aXLoadMeshData[nCntEdit].pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
			g_aEdit.aXLoadMeshData[nCntEdit].pMatData[nCntMat].pTextureFilename = NULL;	//マテリアルのデータ
			g_aEdit.aXLoadMeshData[nCntEdit].acFilename[nCntMat] = NULL;	//ファイル名
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntTex] = NULL;	//テクスチャへのポインタ
		}
		g_aEdit.aXLoadMeshData[nCntEdit].dwNumMat = NULL;			//マテリアルの数
	}


	//モデルの情報取得
	Model *pModelData = GetXLoadData();

	for (int nCntEdit = 0; nCntEdit < GetModelFileNum(); nCntEdit++)
	{//モデルのファイル数分読み込む

		//Xファイルのデータ取得
		g_aEdit.aXLoadMeshData[nCntEdit] = pModelData[nCntEdit];

		//モデルの全頂点チェック
		CheckVtx(g_aEdit.aXLoadMeshData[nCntEdit].rot.y, &g_aEdit.aXLoadMeshData[nCntEdit].vtxMax, &g_aEdit.aXLoadMeshData[nCntEdit].vtxMin, g_aEdit.aXLoadMeshData[nCntEdit].pMesh, g_aEdit.aXLoadMeshData[nCntEdit].pVtxBuff);
	}
}

//==================================================================================
//エディットの終了処理
//==================================================================================
void UninitEdit(void)
{

	//for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	//{
	//	for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
	//	{
	//		//テクスチャの破棄
	//		if (g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntTex] != NULL)
	//		{
	//			g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntTex]->Release();
	//			g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntTex] = NULL;
	//		}
	//	}

	//	//メッシュの破棄
	//	if (g_aEdit.aXLoadMeshData[nCntEdit].pMesh != NULL)
	//	{
	//		g_aEdit.aXLoadMeshData[nCntEdit].pMesh->Release();
	//		g_aEdit.aXLoadMeshData[nCntEdit].pMesh = NULL;
	//	}

	//	//マテリアルの破棄
	//	if (g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat != NULL)
	//	{
	//		g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat->Release();
	//		g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat = NULL;
	//	}
	//}
}

//==================================================================================
//エディットの更新処理
//==================================================================================
void UpdateEdit(void)
{

	//モデルの情報取得
	ModelModel *pModel = GetModel();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8が押された,使用状況切り替え

		g_aEdit.bUse = g_aEdit.bUse ? false : true;

		if (g_aEdit.bUse == false)
		{//エディット状態
			SetGameState(GAMESTATE_NONE, 0);
		}
	}
#endif

	if (g_aEdit.bUse == true)
	{//使用していたら

		//エディット状態
		//SetGameState(GAMESTATE_EDIT, 0);

		//操作処理
		ControllEdit(&g_aEdit.pos, &g_aEdit.rot);

		//ファイル種類変更
		ChangeEditType();

		//つかむ処理
		GrabEditModel();

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//ENTERが押された,,モデル設置

			if (g_aEdit.nNowModelType != GetModelFileNum() - 1)
			{//目印は置かない

				SetModel(g_aEdit.pos, g_aEdit.rot, g_aEdit.nNowModelType, g_aEdit.nShadow);
			}
		}
	}
}

//==================================================================================
//エディットの操作処理
//==================================================================================
void ControllEdit(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//モデルの情報取得
	ModelModel *pModel = GetModel();

	if (GetKeyboardPress(DIK_LEFT) == true)
	{//←キーが押された,左移動

		if (GetKeyboardPress(DIK_UP) == true)
		{//A+W,左上移動

			pPos->x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
			pPos->z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//A+S,左下移動

			pPos->x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
			pPos->z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
		}
		else
		{//A,左移動

			pPos->x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
			pPos->z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{//Dキーが押された,右移動

		if (GetKeyboardPress(DIK_UP) == true)
		{//D+W,右上移動

			pPos->x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
			pPos->z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//D+S,右下移動

			pPos->x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
			pPos->z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
		}
		else
		{//D,右移動

			pPos->x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
			pPos->z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{//Wが押された、奥移動

		pPos->x += sinf(pCamera->rot.y) * MOVE;
		pPos->z += cosf(pCamera->rot.y) * MOVE;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{//Sが押された、手前移動

		pPos->x += sinf(D3DX_PI + pCamera->rot.y) * MOVE;
		pPos->z += cosf(D3DX_PI + pCamera->rot.y) * MOVE;
	}

	if (GetKeyboardPress(DIK_A) == true)
	{//←キーが押された,左移動

		if (GetKeyboardPress(DIK_W) == true)
		{//A+W,左上移動

			pPos->x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE_SLOW;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//A+S,左下移動

			pPos->x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE_SLOW;
		}
		else
		{//A,左移動

			pPos->x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE_SLOW;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//Dキーが押された,右移動

		if (GetKeyboardPress(DIK_W) == true)
		{//D+W,右上移動

			pPos->x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE_SLOW;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//D+S,右下移動

			pPos->x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE_SLOW;
		}
		else
		{//D,右移動

			pPos->x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE_SLOW;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//Wが押された、奥移動

		pPos->x += sinf(pCamera->rot.y) * MOVE_SLOW;
		pPos->z += cosf(pCamera->rot.y) * MOVE_SLOW;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//Sが押された、手前移動

		pPos->x += sinf(D3DX_PI + pCamera->rot.y) * MOVE_SLOW;
		pPos->z += cosf(D3DX_PI + pCamera->rot.y) * MOVE_SLOW;
	}

	if (GetKeyboardTrigger(DIK_LSHIFT) == true)
	{//回転

		pRot->y += D3DX_PI * 0.1f;
	}
	if (GetKeyboardTrigger(DIK_RSHIFT) == true)
	{//回転

		pRot->y -= D3DX_PI * 0.1f;
	}

	if (GetKeyboardPress(DIK_I) == true)
	{//Iキーが押された,上昇

		pPos->y += MOVE;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//Kキーが押された,下降

		pPos->y -= MOVE;
	}

	//角度正規化
	RotNormalize(&pRot->y);

}

//==================================================================================
//エディットのファイル種類変更
//==================================================================================
void ChangeEditType(void)
{
	//ファイルの種類
	if (GetKeyboardTrigger(DIK_1) == true)
	{//1が押された

		//色の種類更新
		g_aEdit.nNowModelType = (g_aEdit.nNowModelType + (GetModelFileNum() - 1)) % GetModelFileNum();

	}
	else if (GetKeyboardTrigger(DIK_2) == true)
	{//2が押された

		//色の種類更新
		g_aEdit.nNowModelType = (g_aEdit.nNowModelType + 1) % GetModelFileNum();
	}
}

//==================================================================================
//エディットの掴み処理
//==================================================================================
void GrabEditModel(void)
{
	//モデルの情報取得
	ModelModel *pModel = GetModel();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true)
		{//使用されていたら

			if (pModel->pos.x + pModel->vtxMax.x >= g_aEdit.pos.x + g_aEdit.aXLoadMeshData[g_aEdit.nNowModelType].vtxMin.x &&	//右からめり込んでる
				pModel->pos.x + pModel->vtxMin.x <= g_aEdit.pos.x + g_aEdit.aXLoadMeshData[g_aEdit.nNowModelType].vtxMax.x &&	//左からめり込んでる
				pModel->pos.z + pModel->vtxMax.z >= g_aEdit.pos.z + g_aEdit.aXLoadMeshData[g_aEdit.nNowModelType].vtxMin.z &&	//からめり込んでる
				pModel->pos.z + pModel->vtxMin.z <= g_aEdit.pos.z + g_aEdit.aXLoadMeshData[g_aEdit.nNowModelType].vtxMax.z)		//前からめり込んでる
			{//当たり判定内に入っていたら掴める

				if (GetKeyboardPress(DIK_SPACE) == true)
				{//SPACEが押された,,つかんで移動

					//操作処理
					ControllEdit(&pModel->pos, &pModel->rot);
					pModel->posOrigin = pModel->pos;

					if (GetKeyboardTrigger(DIK_LSHIFT) == true)
					{
						pModel->rot.y += D3DX_PI * 0.1f;
					}
					if (GetKeyboardTrigger(DIK_RSHIFT) == true)
					{//回転

						pModel->rot.y -= D3DX_PI * 0.1f;
					}
					//break;
				}

				if (GetKeyboardTrigger(DIK_DELETE) == true)
				{//Deleteが押された、削除

					//削除処理
					DeleteEditModel(nCntModel);
				}
			}
		}
	}
}

//==================================================================================
//エディットの削除処理
//==================================================================================
void DeleteEditModel(int nIdxDelete)
{
	//モデルの情報取得
	ModelModel *pModel = GetModel();

	//ポインタをインデックス分進める
	pModel += nIdxDelete;

	//使用してない状態にする
	pModel->bUse = false;

	if (pModel->nShadow != 0)
	{//影を使用している場合

		//SetEnableShadow(pModel->nIdxShadow);
	}

	for (int nCntModel = nIdxDelete; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{//消した番号のモデルから入れ替え

		if (nCntModel + 1 < MAX_MODEL)
		{//最後の1つ前まで

			//入れ替え
			pModel = &pModel[nCntModel + 1];
		}
		else
		{//最後だけ

			pModel = NULL;
		}
	}
}

//==================================================================================
//エディットの描画処理
//==================================================================================
void DrawEdit(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATERIAL mInvisible;			//ダメージマテリアル
	ZeroMemory(&mInvisible, sizeof(D3DXMATERIAL));

	for (int nCntEdit = 0; nCntEdit < GetModelFileNum(); nCntEdit++)
	{
		if (g_aEdit.nNowModelType == nCntEdit && g_aEdit.bUse == true)
		{//今のモデル種類と同じだったら

			if (g_aEdit.nNowModelType == 33)
			{
				int nnn = 0;
			}

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEdit.mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEdit.rot.y, g_aEdit.rot.x, g_aEdit.rot.z);
			D3DXMatrixMultiply(&g_aEdit.mtxWorld, &g_aEdit.mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aEdit.pos.x, g_aEdit.pos.y, g_aEdit.pos.z);
			D3DXMatrixMultiply(&g_aEdit.mtxWorld, &g_aEdit.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEdit.mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aEdit.aXLoadMeshData[nCntEdit].dwNumMat; nCntMat++)
			{

				mInvisible.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, 0.5f);
				mInvisible.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, 0.5f);

				//マテリアルの設定
				pDevice->SetMaterial(&mInvisible.MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_aEdit.aXLoadMeshData[nCntEdit].pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//エディットの情報取得
//==================================================================================
Edit *GetEdit(void)
{
	return &g_aEdit;
}