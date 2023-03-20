//=========================================================================
//
//イベントサイン処理
//Author:日野澤匠泉	[iventsign.cpp]
//
//=========================================================================
#include "iventsign.h"
#include "player.h"

//グローバル変数宣言
LPD3DXMESH g_pMeshIventSign = NULL;				//メッシュ(頂点情報)へのポインタ
LPD3DXBUFFER g_pBuffMatIventSign = NULL;		//マテリアルのポインタ
LPDIRECT3DTEXTURE9 g_pTextureIventSign = NULL;	//テクスチャへのポインタ
DWORD g_dwNumMatIventSign = 0;					//マテリアルの数
D3DXMATRIX g_mtxWorldIventSign;					//ワールドマトリックス

IventSign g_IventSign;				//イベントサインの情報を取得

//=========================================================================
//イベントサインの初期化処理
//=========================================================================
void InitIventSign(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//デバイスの取得
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ

	//各種変数初期化
	g_IventSign.pos = D3DXVECTOR3(0.0f, -100.0f, 0.0f);			//位置の初期化
	g_IventSign.Iventpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//イベント位置
	g_IventSign.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期化
	g_IventSign.Dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の移動方向
	g_IventSign.bUse = false;			//使用しているかどうか

	//Xファイルの読み込み
	D3DXLoadMeshFromX("data\\MODEL\\sign.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatIventSign,
		NULL,
		&g_dwNumMatIventSign,
		&g_pMeshIventSign);

	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)g_pBuffMatIventSign->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatIventSign; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//テクスチャファイルが存在する

		 //テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureIventSign);
		}
	}
}

//=========================================================================
//イベントサインの終了処理
//=========================================================================
void UninitIventSign(void)
{
	//メッシュの破棄
	if (g_pMeshIventSign != NULL)
	{
		g_pMeshIventSign->Release();
		g_pMeshIventSign = NULL;
	}

	//マテリアルの破棄
	if (g_pBuffMatIventSign != NULL)
	{
		g_pBuffMatIventSign->Release();
		g_pBuffMatIventSign = NULL;
	}

	if (g_pTextureIventSign != NULL)
	{
		g_pTextureIventSign->Release();
		g_pTextureIventSign = NULL;
	}
}

//=========================================================================
//イベントサインの更新処理
//=========================================================================
void UpdateIventSign(void)
{
	//プレイヤー情報取得
	Player *pPlayer = GetPlayer();

	//目的の距離算出
	g_IventSign.Dest.x = g_IventSign.Iventpos.x - pPlayer->pos.x;
	g_IventSign.Dest.z = g_IventSign.Iventpos.z - pPlayer->pos.z;

	/*g_IventSign.pos.x = pPlayer->pos.x;
	g_IventSign.pos.z = pPlayer->pos.z;*/

	g_IventSign.rot.y = atan2f(g_IventSign.Dest.x, g_IventSign.Dest.z);		//目標の移動方向(角度)
}

//=========================================================================
//イベントサインの描画処理
//=========================================================================
void DrawIventSign(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;					//計算用
	D3DMATERIAL9 matDef;		//現在のマテリアル保存用
	D3DXMATERIAL *pMat;			//マテリアルデータへのポインタ
	
	// ライティングを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	if (g_IventSign.bUse == true)
	{//使用しているとき

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_mtxWorldIventSign);

		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_IventSign.rot.y, g_IventSign.rot.x, g_IventSign.rot.z);
		D3DXMatrixMultiply(&g_mtxWorldIventSign, &g_mtxWorldIventSign, &mtxRot);

		//位置を反映
		D3DXMatrixTranslation(&mtxTrans, g_IventSign.pos.x, g_IventSign.pos.y, g_IventSign.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldIventSign, &g_mtxWorldIventSign, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldIventSign);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_pBuffMatIventSign->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatIventSign; nCntMat++)
		{
			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureIventSign);

			//モデル(パーツ)の描画
			g_pMeshIventSign->DrawSubset(nCntMat);
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=========================================================================
//イベントサインの取得
//=========================================================================
void SetIventSign(D3DXVECTOR3 pos)
{
	//プレイヤー情報取得
	Player *pPlayer = GetPlayer();

	g_IventSign.Iventpos = pos;		//位置

	//目的の距離算出
	g_IventSign.Dest.x = g_IventSign.Iventpos.x - pPlayer->pos.x;
	g_IventSign.Dest.z = g_IventSign.Iventpos.z - pPlayer->pos.z;

	g_IventSign.rot.y = atan2f(g_IventSign.Dest.x, g_IventSign.Dest.z);		//目標の移動方向(角度)
}

//=========================================================================
//イベントサインの取得
//=========================================================================
IventSign *GetIventSign(void)
{
	return &g_IventSign;
}
