//=============================================================================
//
// タイトルロゴ処理 [titlelogo.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "titlelogo.h"
#include "shadow.h"
#include "calculation.h"

//==================================================================================
//マクロ定義
//==================================================================================
#define TITLE_LOGO_INIT_POS		(D3DXVECTOR3(0.0f,1000.0f,0.0f))		// タイトルの初期位置
#define TITLE_LOGO_POS_DEST		(D3DXVECTOR3(0.0f,350.0f,0.0f))			// タイトルの目的の位置
#define TITLE_LOGO_MOVE			(3.0f)									// タイトルの移動量

//==================================================================================
// プロトタイプ宣言
//==================================================================================
void SetTitleLogo(void);	// タイトルロゴの設定処理

//==================================================================================
//グローバル変数宣言
//==================================================================================
TitleLogo g_titleLogo;		//タイトルロゴの情報

//==================================================================================
//タイトルロゴの初期化処理
//==================================================================================
void InitTitleLogo(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// モデル情報の初期化
	ZeroMemory(&g_titleLogo.aModel, sizeof(Model));

	// モデル情報の設定
	g_titleLogo.aModel.nIdxShadow	= -1;									// 影のインデックス番号
	g_titleLogo.aModel.nShadow		= 1;									// 影を使うかどうか
	g_titleLogo.aModel.vtxMin		= D3DXVECTOR3(10.0f, 10.0f, 10.0f);		// モデルの最小値
	g_titleLogo.aModel.vtxMax		= D3DXVECTOR3(-10.0f, -10.0f, -10.0f);	// モデルの最大値
	g_titleLogo.aModel.nParent		= -1;									// 親の番号
	g_titleLogo.aModel.nState		= TITLE_LOGO_STATE_APPEAR;				// 状態
	g_titleLogo.aModel.bUse			= false;								// 使用状況
	
	// タイトルロゴの設定処理
	SetTitleLogo();
}

//==================================================================================
//タイトルロゴの終了処理
//==================================================================================
void UninitTitleLogo(void)
{

}

//==================================================================================
//タイトルロゴの更新処理
//==================================================================================
void UpdateTitleLogo(void)
{
	if (g_titleLogo.aModel.bUse == true)
	{//使用していた場合

		switch (g_titleLogo.aModel.nState)
		{
		case TITLE_LOGO_STATE_APPEAR:		// 出現状態

			// 移動量を加算する
			g_titleLogo.aModel.pos.y -= g_titleLogo.aModel.move.y;

			if (g_titleLogo.aModel.pos.y <= g_titleLogo.aModel.posDest.y)
			{ // 位置が目的の位置を通り過ぎた場合

				// 目的の位置に設定する
				g_titleLogo.aModel.pos = TITLE_LOGO_POS_DEST;

				// 状態を通常状態にする
				g_titleLogo.aModel.nState = TITLE_LOGO_STATE_NONE;
			}

			break;

		case TITLE_LOGO_STATE_NONE:			// 通常状態

			// 特に無し

			break;
		}
	}
}

//==================================================================================
//タイトルロゴの描画処理
//==================================================================================
void DrawTitleLogo(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

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

	if (g_titleLogo.aModel.bUse == true)
	{//使用していたら

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_titleLogo.aModel.mtxWorld);

		//向きを反映する
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_titleLogo.aModel.rot.y, g_titleLogo.aModel.rot.x, g_titleLogo.aModel.rot.z);
		D3DXMatrixMultiply(&g_titleLogo.aModel.mtxWorld, &g_titleLogo.aModel.mtxWorld, &mtxRot);

		//位置を反映する
		D3DXMatrixTranslation(&mtxTrans, g_titleLogo.aModel.pos.x, g_titleLogo.aModel.pos.y, g_titleLogo.aModel.pos.z);
		D3DXMatrixMultiply(&g_titleLogo.aModel.mtxWorld, &g_titleLogo.aModel.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_titleLogo.aModel.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_titleLogo.aModel.pBuffMat->GetBufferPointer();

		//頂点数分繰り返し
		for (int nCntMat = 0; nCntMat < (int)g_titleLogo.aModel.dwNumMat; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_titleLogo.aModel.pTexture[nCntMat]);

			//モデル(パーツ)の描画
			g_titleLogo.aModel.pMesh->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		// ライティングを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	//デフォルト
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==================================================================================
// タイトルロゴの設定処理
//==================================================================================
void SetTitleLogo(void)
{
	//モデルの情報取得
	Model *pModel = GetXLoadData();

	if (g_titleLogo.aModel.bUse == false)
	{ // 使用していなかった場合

		//モデル情報取得
		g_titleLogo.aModel = pModel[25];

		//モデルの全頂点チェック
		CheckVtx(g_titleLogo.aModel.rot.y, &g_titleLogo.aModel.vtxMax, &g_titleLogo.aModel.vtxMin, g_titleLogo.aModel.pMesh, g_titleLogo.aModel.pVtxBuff);

		// 情報の設定
		g_titleLogo.aModel.pos = TITLE_LOGO_INIT_POS;		// 位置
		g_titleLogo.aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// 向き
		g_titleLogo.aModel.move = D3DXVECTOR3(0.0f, TITLE_LOGO_MOVE, 0.0f);		// 移動量
		g_titleLogo.aModel.posDest = TITLE_LOGO_POS_DEST;	// 目的の位置

		g_titleLogo.aModel.bUse = true;						// 使用状況
	}
}