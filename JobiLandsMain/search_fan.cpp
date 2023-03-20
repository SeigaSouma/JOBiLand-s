//==============================================================
//
//敵の前に出す扇の形[search_fan.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include"search_fan.h"
#include"enemy.h"
#include"calculation.h"

//マクロ定義
#define WIDTH_VTX		(10)	//横の頂点数
#define HEIGHT_VTX		(1)		//縦の頂点数
#define FAN_RANGE		(-D3DX_PI * 0.25f)

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureSearchFan = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSearchFan = NULL;		//頂点バッファへのポインタ
SearchFan g_aSearchFan[MAX_ENEMY];			//扇の情報

int g_nNumVtx;		//バーテックス数

//==============================================================
//扇の初期化処理
//==============================================================
void InitSearchFan(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\searchfan000.png",
		&g_pTextureSearchFan);

	//初期化
	for (int nCntFan = 0; nCntFan < MAX_ENEMY; nCntFan++)
	{
		g_aSearchFan[nCntFan].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSearchFan[nCntFan].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSearchFan[nCntFan].mtxWorld = {};		//ワールドマトリックス
		g_aSearchFan[nCntFan].bUse = false;
		g_aSearchFan[nCntFan].bDisp = false;
	}

	g_nNumVtx = WIDTH_VTX;		//頂点数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVtx * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSearchFan,
		NULL);

	VERTEX_3D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSearchFan->Lock(0, 0, (void**)&pVtx, 0);

	float fRot = (D3DX_PI * 0.5f) / (float)WIDTH_VTX;
	D3DXVECTOR3 pos[128];

	for (int nCntFan = 0; nCntFan < MAX_ENEMY; nCntFan++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(
			0.0f,
			0.0f,
			0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);

		pVtx += 1;

		for (int nCntWidth = 1; nCntWidth < WIDTH_VTX - 1; nCntWidth++)
		{//横

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(
				sinf(nCntWidth * fRot + FAN_RANGE) * -SEARCH_RADIUS * 2,
				0.0f,
				cosf(nCntWidth * fRot + FAN_RANGE) * -SEARCH_RADIUS * 2);

			pos[0] = D3DXVECTOR3(
				sinf(nCntWidth * fRot + FAN_RANGE) * -SEARCH_RADIUS * 2,
				0.0f,
				cosf(nCntWidth * fRot + FAN_RANGE) * -SEARCH_RADIUS * 2);

			//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.2f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);

			pVtx += 1;

		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSearchFan->Unlock();
}

//==============================================================
//扇の終了処理
//==============================================================
void UnInitSearchFan(void)
{
	//テクスチャの破棄
	if (g_pTextureSearchFan != NULL)
	{
		g_pTextureSearchFan->Release();
		g_pTextureSearchFan = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSearchFan != NULL)
	{
		g_pVtxBuffSearchFan->Release();
		g_pVtxBuffSearchFan = NULL;
	}
}

//==============================================================
//扇の更新処理
//==============================================================
void UpdateSearchFan(void)
{
	
}

//==============================================================
//扇の描画処理
//==============================================================
void DrawSearchFan(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;						//ビューマトリックス取得用

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntFan = 0; nCntFan < MAX_ENEMY; nCntFan++)
	{
		if (g_aSearchFan[nCntFan].bUse == true && g_aSearchFan[nCntFan].bDisp == true)
		{
			//ワールドマトリックスを初期化
			D3DXMatrixIdentity(&g_aSearchFan[nCntFan].mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSearchFan[nCntFan].rot.y, g_aSearchFan[nCntFan].rot.x, g_aSearchFan[nCntFan].rot.z);
			D3DXMatrixMultiply(&g_aSearchFan[nCntFan].mtxWorld, &g_aSearchFan[nCntFan].mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aSearchFan[nCntFan].pos.x, g_aSearchFan[nCntFan].pos.y, g_aSearchFan[nCntFan].pos.z);
			D3DXMatrixMultiply(&g_aSearchFan[nCntFan].mtxWorld, &g_aSearchFan[nCntFan].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSearchFan[nCntFan].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSearchFan, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureSearchFan);

			//扇の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,		//プリミティブの種類
				g_nNumVtx * nCntFan,						//プリミティブ
				8);									//描画するプリミティブ数
		}
	}

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==============================================================
//扇の位置更新処理
//==============================================================
void SetPositionSearchFan(int nIdxSearchFan, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

	g_aSearchFan[nIdxSearchFan].rot.y = rot.y;

	RotNormalize(&g_aSearchFan[nIdxSearchFan].rot.y);

	//指定した扇の位置を設定
	g_aSearchFan[nIdxSearchFan].pos.x = pos.x;
	g_aSearchFan[nIdxSearchFan].pos.y = pos.y + 0.5f;
	g_aSearchFan[nIdxSearchFan].pos.z = pos.z;


}

//==============================================================
//扇の設定処理
//==============================================================
int SetSearchFan()
{
	int nCntFan;
	int nCounter = -1;

	for (nCntFan = 0; nCntFan < MAX_ENEMY; nCntFan++)
	{
		if (g_aSearchFan[nCntFan].bUse == false)
		{
			nCounter = nCntFan;

			g_aSearchFan[nCntFan].bUse = true;
			g_aSearchFan[nCntFan].bDisp = true;

			break;
		}
	}

	return nCounter;
}

//==============================================================
//扇の情報取得
//==============================================================
SearchFan *GetSearchFan(void)
{
	return &g_aSearchFan[0];
}
