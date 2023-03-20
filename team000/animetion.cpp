//==========================================================================
//
//アニメーション[animation.cpp]
//Author:日野澤匠泉
//
//==========================================================================
#include"main.h"
#include"animation.h"
#include"input.h"
#include"model.h"
#include"player.h"

//マクロ定義
#define SIZE	(300.0f)		//壁のサイズ
#define MOVE_U	(1.0f / 6.0f)
#define ANIM_TIME	(100)

//グローバル変数宣言
LPDIRECT3DTEXTURE9	g_pTextureAnimation[MAX_TEX] = {};		//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffAnimation = NULL;			//頂点のバッファ
Animation g_aAnimation[NUM_TEX];

//テクスチャファイル名
const char *c_apFilenameAnimation[] =
{
	"data\\TEXTURE\\manga000.png",
	"data\\TEXTURE\\mang001.png"
};

//==========================================================================
//アニメーションの初期化処理
//==========================================================================
void InitAnimation(void)
{

	//デバイス情報取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	

	//テクスチャの読み込み
	for (int nCntAnim = 0; nCntAnim < MAX_TEX; nCntAnim++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameAnimation[nCntAnim],
			&g_pTextureAnimation[nCntAnim]);
	}

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		g_aAnimation[nCntAnim].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		g_aAnimation[nCntAnim].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向きを初期化する(Z値を使用する)
		g_aAnimation[nCntAnim].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAnimation,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		//頂点座標の設定
		pVtx[0].pos.x = (-SIZE);
		pVtx[1].pos.x = (+SIZE * 2);
		pVtx[2].pos.x = (-SIZE);
		pVtx[3].pos.x = (+SIZE * 2);

		pVtx[0].pos.y = (+SIZE * 2);
		pVtx[1].pos.y = (+SIZE * 2);
		pVtx[2].pos.y = (+0.0f);
		pVtx[3].pos.y = (+0.0f);

		pVtx[0].pos.z = (0.0f);
		pVtx[1].pos.z = (0.0f);
		pVtx[2].pos.z = (0.0f);
		pVtx[3].pos.z = (0.0f);

		////向きの設定(y軸)
		//pVtx[0].rot.y = D3DX_PI * 0.0f; //正面
		//pVtx[1].rot.y = D3DX_PI * 0.5f; //右向き
		//pVtx[2].rot.y = D3DX_PI * 1.0f; //後ろ向き
		//pVtx[3].rot.y = D3DX_PI * -0.5f; //左向き

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
		pVtx[1].tex = D3DXVECTOR2(1.0f / 6.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 6.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffAnimation->Unlock();

	//--------------------------------------
	//壁の設置
	//--------------------------------------
	SetAnimation(D3DXVECTOR3(-845.00f, 525.0f, 758.00f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 0, 85.0f, 100.0f);  //左0
	SetAnimation(D3DXVECTOR3(-830.00f, 283.0f, 50.00f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 1, 68.0f, 65.0f); //左1
	SetAnimation(D3DXVECTOR3(-845.10f, 718.0f, 715.00f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 1, 142.0f, 135.0f);  //左2
	SetAnimation(D3DXVECTOR3(2780.0f, 235.0f, 78.00f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 0, 70.0f, 64.0f); //後ろ0
	SetAnimation(D3DXVECTOR3(2950.00f, 410.0f, 93.00f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 1, 160.0f, 155.0f); //後ろ1
	SetAnimation(D3DXVECTOR3(1890.00f, 535.0f, -3270.00f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 1, 112.0f, 162.0f); //後ろ2
}

//==========================================================================
//アニメーションの終了処理
//==========================================================================
void UninitAnimation(void)
{
	for (int nCntAnim = 0; nCntAnim < MAX_TEX; nCntAnim++)
	{
		//テクスチャの破棄
		if (g_pTextureAnimation[nCntAnim] != NULL)
		{
			g_pTextureAnimation[nCntAnim]->Release();
			g_pTextureAnimation[nCntAnim] = NULL;
		}
	}

	//頂点バッファの処理
	if (g_pVtxBuffAnimation != NULL)
	{
		g_pVtxBuffAnimation->Release();
		g_pVtxBuffAnimation = NULL;
	}
}

//==========================================================================
//アニメーションの更新処理
//==========================================================================
void UpdateAnimation(void)
{
	VERTEX_3D *pVtx;
	Player *pPlayer = GetPlayer();

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		if (g_aAnimation[nCntAnim].bUse == true)
		{
			g_aAnimation[nCntAnim].nCountAnim++;	//カウンターを加算

			if ((g_aAnimation[nCntAnim].nCountAnim % ANIM_TIME) == 0)
			{//規定カウント進んだら

				//パターンNo.を更新
				g_aAnimation[nCntAnim].nPatternAnim = (g_aAnimation[nCntAnim].nPatternAnim + 1) % 6;
				g_aAnimation[nCntAnim].nCountAnim = 0;

			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2((g_aAnimation[nCntAnim].nPatternAnim % 6) * MOVE_U, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_aAnimation[nCntAnim].nPatternAnim % 6 + 1) * MOVE_U, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_aAnimation[nCntAnim].nPatternAnim % 6) * MOVE_U, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_aAnimation[nCntAnim].nPatternAnim % 6 + 1) * MOVE_U, 1.0f);
		}

		pVtx += 4;

	}


	//頂点バッファをアンロックする
	g_pVtxBuffAnimation->Unlock();

}

//==========================================================================
//アニメーションの描画処理
//==========================================================================
void DrawAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans;					//計算用マトリックス

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		if (g_aAnimation[nCntAnim].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aAnimation[nCntAnim].g_mtxWorld);

			//向きを反映
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAnimation[nCntAnim].rot.y, g_aAnimation[nCntAnim].rot.x, g_aAnimation[nCntAnim].rot.z);
			D3DXMatrixMultiply(&g_aAnimation[nCntAnim].g_mtxWorld, &g_aAnimation[nCntAnim].g_mtxWorld, &mtxRot);

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aAnimation[nCntAnim].pos.x, g_aAnimation[nCntAnim].pos.y, g_aAnimation[nCntAnim].pos.z);
			D3DXMatrixMultiply(&g_aAnimation[nCntAnim].g_mtxWorld, &g_aAnimation[nCntAnim].g_mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aAnimation[nCntAnim].g_mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffAnimation, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			switch (g_aAnimation[nCntAnim].nType)
			{
			case 0:
				//描画処理
				pDevice->SetTexture(0, g_pTextureAnimation[0]);
				break;

			case 1:
				//描画処理
				pDevice->SetTexture(0, g_pTextureAnimation[1]);
				break;
			}

			//壁の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAnim * 4, 2);		//プリミティブの種類
		}
	}


	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==========================================================================
//アニメーションの設定処理
//==========================================================================
void SetAnimation(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight)
{
	VERTEX_3D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		if (g_aAnimation[nCntAnim].bUse == false)
		{//敵が使用されない
			g_aAnimation[nCntAnim].pos = pos;
			g_aAnimation[nCntAnim].rot = rot;
			g_aAnimation[nCntAnim].nType = nType;
			g_aAnimation[nCntAnim].bUse = true;
			g_aAnimation[nCntAnim].fWidth = fWidth;
			g_aAnimation[nCntAnim].fHeight = fHeight;

			//頂点座標の設定
			pVtx[0].pos.x = (-fWidth);
			pVtx[1].pos.x = (+fWidth * 2);
			pVtx[2].pos.x = (-fWidth);
			pVtx[3].pos.x = (+fWidth * 2);

			pVtx[0].pos.y = (+fHeight * 2);
			pVtx[1].pos.y = (+fHeight * 2);
			pVtx[2].pos.y = (+0.0f);
			pVtx[3].pos.y = (+0.0f);

			pVtx[0].pos.z = (0.0f);
			pVtx[1].pos.z = (0.0f);
			pVtx[2].pos.z = (0.0f);
			pVtx[3].pos.z = (0.0f);

			break;
		}
		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffAnimation->Unlock();
}

//==========================================================================
//アニメーションの取得処理
//==========================================================================
Animation *GetAnimation(void)
{
	return &g_aAnimation[0];
}
