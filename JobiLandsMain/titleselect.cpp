//=============================================================================
//
// タイトル選択肢処理 [titleselect.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "titleselect.h"
#include "title.h"
#include "input.h"
#include "shadow.h"
#include "calculation.h"
#include "camera.h"
#include "buddy.h"

//マクロ定義
#define MOVE		(4.0f)
#define ROT_MOVE	(0.01f)

//プロトタイプ宣言
void UpdateTitleSelectTKTK(int nSelect);

//グローバル変数宣言
TitleSelect g_aTitleSelect[MAX_TITLESELECT];		//タイトル選択肢の情報

//==================================================================================
//タイトル選択肢の初期化処理
//==================================================================================
void InitTitleSelect(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各要素初期化
	for (int nCntTitleSelect = 0; nCntTitleSelect < MAX_TITLESELECT; nCntTitleSelect++)
	{
		g_aTitleSelect[nCntTitleSelect].nFloating = 1;	//浮遊の向き
		g_aTitleSelect[nCntTitleSelect].nTurnCounter = 0;	//回転した回数カウント
		g_aTitleSelect[nCntTitleSelect].fPosDiffX = 0.0f;	//目的の位置との差分
		g_aTitleSelect[nCntTitleSelect].fPosDiffY = 0.0f;	//目的の位置との差分

		g_aTitleSelect[nCntTitleSelect].aModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aTitleSelect[nCntTitleSelect].aModel.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aTitleSelect[nCntTitleSelect].aModel.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
		g_aTitleSelect[nCntTitleSelect].aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aTitleSelect[nCntTitleSelect].aModel.rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
		g_aTitleSelect[nCntTitleSelect].aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld);
		D3DXMatrixIdentity(&g_aTitleSelect[nCntTitleSelect].aModel.OldmtxWorld);

		g_aTitleSelect[nCntTitleSelect].aModel.nIdxShadow = -1;			//影のインデックス番号
		g_aTitleSelect[nCntTitleSelect].aModel.nIdxRotate = -1;	//ぐるぐるのインデックス番号
		g_aTitleSelect[nCntTitleSelect].aModel.nIdxHypGauge = -1;		//体力ゲージのインデックス番号
		g_aTitleSelect[nCntTitleSelect].aModel.nShadow = 1;			//影を使うかどうか

		g_aTitleSelect[nCntTitleSelect].aModel.nNumVtx = 0;			//頂点数
		g_aTitleSelect[nCntTitleSelect].aModel.dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aTitleSelect[nCntTitleSelect].aModel.pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aTitleSelect[nCntTitleSelect].aModel.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
		g_aTitleSelect[nCntTitleSelect].aModel.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

		g_aTitleSelect[nCntTitleSelect].aModel.nType = 0;			//読み込むモデルの種類
		g_aTitleSelect[nCntTitleSelect].aModel.nParent = -1;		//親の番号
		g_aTitleSelect[nCntTitleSelect].aModel.bUse = false;			//使用しているか
		g_aTitleSelect[nCntTitleSelect].aModel.nState = 0;			//状態

		g_aTitleSelect[nCntTitleSelect].aModel.pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
		g_aTitleSelect[nCntTitleSelect].aModel.pBuffMat = NULL;		//マテリアルへのポインタ

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aTitleSelect[nCntTitleSelect].aModel.pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
			g_aTitleSelect[nCntTitleSelect].aModel.pMatData[nCntMat].pTextureFilename = NULL;	//マテリアルのデータ
			g_aTitleSelect[nCntTitleSelect].aModel.acFilename[nCntMat] = NULL;	//ファイル名
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aTitleSelect[nCntTitleSelect].aModel.pTexture[nCntTex] = NULL;	//テクスチャへのポインタ
		}
		g_aTitleSelect[nCntTitleSelect].aModel.dwNumMat = NULL;			//マテリアルの数
	}

}

//==================================================================================
//タイトル選択肢の終了処理
//==================================================================================
void UninitTitleSelect(void)
{

	

}

//==================================================================================
//タイトル選択肢の更新処理
//==================================================================================
void UpdateTitleSelect(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//タイトルの情報取得
	Title *pTitle = GetTitle();

	for (int nCntTitleSelect = 0; nCntTitleSelect < MAX_TITLESELECT; nCntTitleSelect++)
	{
		if (g_aTitleSelect[nCntTitleSelect].aModel.bUse == true)
		{//使用していたら

			//回転の方向
			int nAngle = 1;

			if (pTitle->nSelect == TITLESELECT_END)
			{//終了の場合

				//逆回転
				nAngle *= -1;
			}

			//タイトルの選択肢と同じものを回す
			g_aTitleSelect[pTitle->nSelect].aModel.rot.y += ROT_MOVE * nAngle;

			//角度の正規化
			RotNormalize(&g_aTitleSelect[pTitle->nSelect].aModel.rot.y);

			//選択肢は浮かせる
			g_aTitleSelect[pTitle->nSelect].aModel.pos.y += 0.08f * g_aTitleSelect[nCntTitleSelect].nFloating;

			if (g_aTitleSelect[pTitle->nSelect].aModel.pos.y >= 20.0f)
			{
				g_aTitleSelect[nCntTitleSelect].nFloating *= -1;
			}
			else if (g_aTitleSelect[pTitle->nSelect].aModel.pos.y < 0.0f && g_aTitleSelect[nCntTitleSelect].nFloating == -1)
			{
				g_aTitleSelect[nCntTitleSelect].nFloating *= -1;
			}


			float fRotDest = 0.0f;	//目標の角度
			float fRotDiff = 0.0f;	//現在と目標の差分
			float fPosDest = 0.0f;	//目標の角度
			float fPosDiff = 0.0f;	//現在と目標の差分

			//目標との差分
			fPosDiff = fPosDest - g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.pos.y;

			//タイトルの選択肢と違うものは0.0fに向かせる
			g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.pos.y += fPosDiff * 0.25f;

			//目標との差分
			fRotDiff = fRotDest - g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.rot.y;

			//角度の正規化
			RotNormalize(&fRotDiff);

			//タイトルの選択肢と違うものは0.0fに向かせる
			g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.rot.y += fRotDiff * 0.25f;

			//角度の正規化
			RotNormalize(&g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.rot.y);
		}
	}

	if (pTitle->nState == TITLESTATE_TKTK)
	{//タイトルがチカチカ状態

		//タイトル選択肢のチカチカ処理
		UpdateTitleSelectTKTK(pTitle->nSelect);
	}

}

//==================================================================================
//タイトル選択肢のチカチカ処理
//==================================================================================
void UpdateTitleSelectTKTK(int nSelect)
{
	//タイトルの情報取得
	Title *pTitle = GetTitle();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	if (g_aTitleSelect[nSelect].nTurnCounter == 0)
	{
		//目標との差分
		g_aTitleSelect[nSelect].fPosDiffX = 0.0f - g_aTitleSelect[nSelect].aModel.pos.x;
		g_aTitleSelect[nSelect].fPosDiffY = 60.0f - g_aTitleSelect[nSelect].aModel.pos.y;
	}

	if (g_aTitleSelect[nSelect].nTurnCounter <= 10)
	{//回転数が30以下の時

		pBuddy[BUDDYTYPE_HYOROGARI].formationPos.z = -350.0f;
		pBuddy[BUDDYTYPE_HYOROGARI].formationPos.x = -100.0f;
		pBuddy[BUDDYTYPE_DEPPA].formationPos.z = -350.0f;
		pBuddy[BUDDYTYPE_DEPPA].formationPos.x = 100.0f;

		//タイトルの選択肢と違うものは0.0fに向かせる
		g_aTitleSelect[nSelect].aModel.pos.x += g_aTitleSelect[nSelect].fPosDiffX / (float)50;
		g_aTitleSelect[nSelect].aModel.pos.y += g_aTitleSelect[nSelect].fPosDiffY / (float)50;

		if (g_aTitleSelect[nSelect].aModel.pos.x >= 0.0f)
		{
			g_aTitleSelect[nSelect].aModel.pos.x = 0.0f;
			g_aTitleSelect[nSelect].aModel.pos.y = 60.0f;
		}

		//タイトルの選択肢と同じものを回す
		g_aTitleSelect[nSelect].aModel.rot.y += 0.8f;

		if (g_aTitleSelect[nSelect].aModel.rot.y >= D3DX_PI)
		{
			//回転数加算
			g_aTitleSelect[nSelect].nTurnCounter++;
		}

		//角度の正規化
		RotNormalize(&g_aTitleSelect[nSelect].aModel.rot.y);
	}
	else if (g_aTitleSelect[nSelect].nTurnCounter > 10)
	{//30回転激アツ後

		pBuddy[BUDDYTYPE_HYOROGARI].formationPos.z = -700.0f;
		pBuddy[BUDDYTYPE_HYOROGARI].formationPos.x = -100.0f;
		pBuddy[BUDDYTYPE_DEPPA].formationPos.z = -700.0f;
		pBuddy[BUDDYTYPE_DEPPA].formationPos.x = 100.0f;

		float fRotDest = 0.0f;	//目標の角度
		float fRotDiff = 0.0f;	//現在と目標の差分

		//目標との差分
		fRotDiff = fRotDest - g_aTitleSelect[nSelect].aModel.rot.y;

		//角度の正規化
		RotNormalize(&fRotDiff);

		//タイトルの選択肢と違うものは0.0fに向かせる
		g_aTitleSelect[nSelect].aModel.rot.y = 0.0f;

		//角度の正規化
		RotNormalize(&g_aTitleSelect[nSelect].aModel.rot.y);

		g_aTitleSelect[nSelect].aModel.pos.z -= 5.0f;

		if (g_aTitleSelect[nSelect].aModel.pos.z <= -700.0f)
		{
			pTitle->nState = TITLESTATE_FADE;
			g_aTitleSelect[nSelect].aModel.pos.z = -700.0f;
		}
	}


}

//==================================================================================
//タイトル選択肢の描画処理
//==================================================================================
void DrawTitleSelect(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//タイトルの情報取得
	Title *pTitle = GetTitle();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATERIAL mMatRed;			//ダメージマテリアル

	//他の情報クリア
	ZeroMemory(&mMatRed, sizeof(D3DXMATERIAL));
	mMatRed.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	mMatRed.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// 背面のカリングはしません。
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntTitleSelect = 0; nCntTitleSelect < MAX_TITLESELECT; nCntTitleSelect++)
	{
		if (g_aTitleSelect[nCntTitleSelect].aModel.bUse == true)
		{//使用していたら

			if (pTitle->nSelect == nCntTitleSelect)
			{//選択肢と同じものはライティングOFF

				// ライティングを無効にする
				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			}

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aTitleSelect[nCntTitleSelect].aModel.rot.y, g_aTitleSelect[nCntTitleSelect].aModel.rot.x, g_aTitleSelect[nCntTitleSelect].aModel.rot.z);
			D3DXMatrixMultiply(&g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld, &g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aTitleSelect[nCntTitleSelect].aModel.pos.x, g_aTitleSelect[nCntTitleSelect].aModel.pos.y, g_aTitleSelect[nCntTitleSelect].aModel.pos.z);
			D3DXMatrixMultiply(&g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld, &g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aTitleSelect[nCntTitleSelect].aModel.pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aTitleSelect[nCntTitleSelect].aModel.dwNumMat; nCntMat++)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_aTitleSelect[nCntTitleSelect].aModel.pTexture[nCntMat]);

				//モデル(パーツ)の描画
				g_aTitleSelect[nCntTitleSelect].aModel.pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);

			// ライティングを有効にする
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	//デフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==================================================================================
//タイトル選択肢の設定処理
//==================================================================================
void SetTitleSelect(D3DXVECTOR3 pos, int nModelType)
{
	//モデルの情報取得
	Model *pModel = GetXLoadData();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTitleSelect = 0; nCntTitleSelect < MAX_TITLESELECT; nCntTitleSelect++)
	{
		if (g_aTitleSelect[nCntTitleSelect].aModel.bUse == false)
		{//使用していなかったら

			//モデル情報取得
			g_aTitleSelect[nCntTitleSelect].aModel = pModel[nModelType];

			g_aTitleSelect[nCntTitleSelect].aModel.pos = pos;	//位置
			g_aTitleSelect[nCntTitleSelect].aModel.bUse = true;					//使用している状況にする

			//モデルの全頂点チェック
			CheckVtx(g_aTitleSelect[nCntTitleSelect].aModel.rot.y, &g_aTitleSelect[nCntTitleSelect].aModel.vtxMax, &g_aTitleSelect[nCntTitleSelect].aModel.vtxMin, g_aTitleSelect[nCntTitleSelect].aModel.pMesh, g_aTitleSelect[nCntTitleSelect].aModel.pVtxBuff);
			break;
		}
	}
}

//==================================================================================
//タイトル選択肢の情報取得
//==================================================================================
TitleSelect *GetTitleSelect(void)
{
	return &g_aTitleSelect[0];
}
