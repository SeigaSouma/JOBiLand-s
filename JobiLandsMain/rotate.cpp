//==============================================================
//
//ぐるぐる処理[rotate.h]
//Author:佐藤根詩音
//
//==============================================================
#include"rotate.h"
#include"mob.h"
#include"crowd.h"
#include"calculation.h"
#include "sound.h"

//マクロ定義
#define POS_X				(10.0f)		//ぐるぐるの大きさX
#define POS_Y				(5.0f)		//ぐるぐるの大きさY

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureRotate = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRotate = NULL;		//頂点バッファへのポインタ
Rotate g_aRotate[MAX_MOB];							//ぐるぐるの情報

//==============================================================
//ぐるぐるの初期化処理
//==============================================================
void InitRotate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\rotate000.png",
		&g_pTextureRotate);


	//変数の初期化
	for (int nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
		{
			g_aRotate[nCntRotate].pos[nCntNumber] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置の初期化
		}

		g_aRotate[nCntRotate].rot[0] = D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f);			//１つ目の向きの初期化
		g_aRotate[nCntRotate].rot[1] = D3DXVECTOR3(0.0f, 0.8f * D3DX_PI, 0.0f);			//２つ目の向きの初期化
		g_aRotate[nCntRotate].rot[2] = D3DXVECTOR3(0.0f, 1.4f * D3DX_PI, 0.0f);			//３つ目の向きの初期化

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aRotate[nCntRotate].mtxWorld);
		g_aRotate[nCntRotate].bUse = false;												//使用していない状態にする
		g_aRotate[nCntRotate].bDisp = false;											//描画していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MOB * NUM_ROTATE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRotate,
		NULL);

	VERTEX_3D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRotate->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
		{

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-POS_X, POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-POS_X, -POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(POS_X, -POS_Y, 0.0f);

			//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//頂点カラーの設定
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRotate->Unlock();
}

//==============================================================
//ぐるぐるの終了処理
//==============================================================
void UnInitRotate(void)
{
	StopSound();

	//テクスチャの破棄
	if (g_pTextureRotate != NULL)
	{
		g_pTextureRotate->Release();
		g_pTextureRotate = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRotate != NULL)
	{
		g_pVtxBuffRotate->Release();
		g_pVtxBuffRotate = NULL;
	}
}

//==============================================================
//ぐるぐるの更新処理
//==============================================================
void UpdateRotate(void)
{
	for (int nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		if (g_aRotate[nCntRotate].bUse == true && g_aRotate[nCntRotate].bDisp == true)
		{//ぐるぐるが使用されているとき

			for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
			{
				//回転させる
				g_aRotate[nCntRotate].rot[nCntNumber].y += 0.05f;

				RotNormalize(&g_aRotate[nCntRotate].rot[nCntNumber].y);
			}
		}
	}
}

//==============================================================
//ぐるぐるの描画処理
//==============================================================
void DrawRotate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		if (g_aRotate[nCntRotate].bUse == true && g_aRotate[nCntRotate].bDisp == true)
		{
			for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
			{

				//ワールドマトリックスを初期化
				D3DXMatrixIdentity(&g_aRotate[nCntRotate].mtxWorld);

				//ビューマトリックスを取得
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//ポリゴンをカメラに対して正面に向ける
				D3DXMatrixInverse(&g_aRotate[nCntRotate].mtxWorld, NULL, &mtxView);		//逆行列を求める
				g_aRotate[nCntRotate].mtxWorld._41 = 0.0f;
				g_aRotate[nCntRotate].mtxWorld._42 = 0.0f;
				g_aRotate[nCntRotate].mtxWorld._43 = 0.0f;

				//位置を反映
				D3DXMatrixTranslation(&mtxTrans, g_aRotate[nCntRotate].pos[nCntNumber].x,
					g_aRotate[nCntRotate].pos[nCntNumber].y,
					g_aRotate[nCntRotate].pos[nCntNumber].z);

				D3DXMatrixMultiply(&g_aRotate[nCntRotate].mtxWorld,
					&g_aRotate[nCntRotate].mtxWorld, &mtxTrans);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aRotate[nCntRotate].mtxWorld);

				//頂点バッファをデータストリームに設定
				pDevice->SetStreamSource(0, g_pVtxBuffRotate, 0, sizeof(VERTEX_3D));

				//頂点フォーマットの設定
				pDevice->SetFVF(FVF_VERTEX_3D);

				//テクスチャの設定
				pDevice->SetTexture(0, g_pTextureRotate);

				//ぐるぐるの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
					(4 * nCntRotate * 3) + (nCntNumber * 4),		//プリミティブ（ぐるぐるの数）
					2);												//描画するプリミティブ数
			}
		}
	}

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==============================================================
//ぐるぐるの位置更新処理
//==============================================================
void SetPositionRotate(int nIdxRotate, D3DXVECTOR3 pos)
{
	if (nIdxRotate >= 0 && nIdxRotate < MAX_MOB)
	{
		for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
		{
			//指定したぐるぐるの位置を設定
			g_aRotate[nIdxRotate].pos[nCntNumber].x = pos.x + sinf(g_aRotate[nIdxRotate].rot[nCntNumber].y) * 25.0f;
			g_aRotate[nIdxRotate].pos[nCntNumber].y = pos.y + 60.0f;
			g_aRotate[nIdxRotate].pos[nCntNumber].z = pos.z + cosf(g_aRotate[nIdxRotate].rot[nCntNumber].y) * 25.0f;
		}
	}
	else
	{
		int nn = 0;
	}
}

//==============================================================
//ぐるぐるの設定処理
//==============================================================
int SetRotate()
{
	int nCntRotate;
	int nCounter = -1;

	for (nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		if (g_aRotate[nCntRotate].bUse == false)
		{
			nCounter = nCntRotate;

			g_aRotate[nCntRotate].bUse = true;
			g_aRotate[nCntRotate].bDisp = false;

			break;
		}
	}

	return nCounter;
}

//==============================================================
//ぐるぐるの情報取得
//==============================================================
Rotate *GetRotate(void)
{
	return &g_aRotate[0];
}
