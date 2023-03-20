//==============================================================
//
//旗[flag.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "flag.h"
#include "crowd.h"
#include "calculation.h"
#include "sound.h"
#include "game.h"
#include "camera.h"

//マクロ定義
#define MAX_FLAG			(3)			//旗の最大値
#define POS_X				(25.0f)		//旗の大きさX
#define POS_Y				(40.0f)		//旗の大きさY
#define ANIM_CHANGECNT		(7)		//アニメーション変更時間
#define TEX_POS				(0.125f)	//テクスチャの間隔
#define TEX_DIV				(8)			//テクスチャの分割数

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_pTextureFlag = NULL;				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFlag = NULL;		//頂点バッファへのポインタ
Flag g_aFlag[MAX_FLAG];		//旗の情報

//==============================================================
//旗の初期化処理
//==============================================================
void InitFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flag.png",
		&g_pTextureFlag);

	//変数初期化
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		g_aFlag[nCntFlag].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		D3DXMatrixIdentity(&g_aFlag[nCntFlag].mtxWorld);				//ワールドマトリックス
		g_aFlag[nCntFlag].nCntAnim = 0;		//アニメーション変更カウンター
		g_aFlag[nCntFlag].nPatternAnim = 0;	//アニメーションNo.
		g_aFlag[nCntFlag].bUse = false;		//使用しているか
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FLAG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFlag,
		NULL);

	VERTEX_3D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEX_POS, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEX_POS, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFlag->Unlock();
}

//==============================================================
//旗の終了処理
//==============================================================
void UnInitFlag(void)
{
	//テクスチャの破棄
	if (g_pTextureFlag != NULL)
	{
		g_pTextureFlag->Release();
		g_pTextureFlag = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffFlag != NULL)
	{
		g_pVtxBuffFlag->Release();
		g_pVtxBuffFlag = NULL;
	}
}

//==============================================================
//旗の更新処理
//==============================================================
void UpdateFlag(void)
{
	VERTEX_3D *pVtx;				//頂点情報へのポインタ

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	float fCamera;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == true)
		{//使用してたら

			g_aFlag[nCntFlag].nCntAnim++;		//アニメーションカウンター加算

			fCamera = (pCamera->fDistance - 800.0f) * 0.00077f;		//旗の大きさ計算

			if (GetGameState() == GAMESTATE_BEFOREEND)
			{//終了したら

				g_aFlag[nCntFlag].bUse = false;		//使用してない状態にする
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3((-POS_X * fCamera) - POS_X, (POS_Y * fCamera) + POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((POS_X * fCamera) + POS_X, (POS_Y * fCamera) + POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-POS_X * fCamera) - POS_X, (-POS_Y * fCamera) - POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((POS_X * fCamera) + POS_X, (-POS_Y * fCamera) - POS_Y, 0.0f);

			if ((g_aFlag[nCntFlag].nCntAnim % ANIM_CHANGECNT) == 0)
			{//一定時間経過

				//アニメーションNo.変更
				g_aFlag[nCntFlag].nPatternAnim = (g_aFlag[nCntFlag].nPatternAnim + 1) % TEX_DIV;

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2((g_aFlag[nCntFlag].nPatternAnim % TEX_DIV) * TEX_POS, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aFlag[nCntFlag].nPatternAnim % TEX_DIV) * TEX_POS + TEX_POS, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aFlag[nCntFlag].nPatternAnim % TEX_DIV) * TEX_POS, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aFlag[nCntFlag].nPatternAnim % TEX_DIV) * TEX_POS + TEX_POS, 1.0f);

			}
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFlag->Unlock();
}

//==============================================================
//旗の描画処理
//==============================================================
void DrawFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	D3DXMATRIX mtxTrans;			//計算用マトリックス
	D3DXMATRIX mtxView;				//ビューマトリックス取得用

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == true)
		{//使用してたら

			//ワールドマトリックスを初期化
			D3DXMatrixIdentity(&g_aFlag[nCntFlag].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aFlag[nCntFlag].mtxWorld, NULL, &mtxView);		//逆行列を求める
			g_aFlag[nCntFlag].mtxWorld._41 = 0.0f;
			g_aFlag[nCntFlag].mtxWorld._42 = 0.0f;
			g_aFlag[nCntFlag].mtxWorld._43 = 0.0f;

			//位置を反映
			D3DXMatrixTranslation(&mtxTrans, g_aFlag[nCntFlag].pos.x,
				g_aFlag[nCntFlag].pos.y,
				g_aFlag[nCntFlag].pos.z);

			D3DXMatrixMultiply(&g_aFlag[nCntFlag].mtxWorld,
				&g_aFlag[nCntFlag].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aFlag[nCntFlag].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffFlag, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureFlag);

			//旗の描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				4 * nCntFlag,									//プリミティブ（旗の数）
				2);												//描画するプリミティブ数
		}
	}

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==============================================================
//旗の位置更新処理
//==============================================================
void SetPositionFlag(int nIdxFlag, D3DXVECTOR3 pos)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	float fCamera;

	fCamera = (pCamera->fDistance - 800.0f) * 0.0001f;		//旗の大きさ計算

	if (g_aFlag[nIdxFlag].bUse == true)
	{
		//指定した旗の位置を設定
		g_aFlag[nIdxFlag].pos.x = pos.x;
		g_aFlag[nIdxFlag].pos.y = pos.y * fCamera + pos.y;
		g_aFlag[nIdxFlag].pos.z = pos.z;

	}
}

//==============================================================
//旗の設定処理
//==============================================================
int SetFlag(void)
{
	int nCounter = -1;

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == false)
		{
			nCounter = nCntFlag;

			g_aFlag[nCntFlag].bUse = true;

			break;
		}
	}

	return nCounter;
}

//==============================================================
//旗の情報取得
//==============================================================
Flag *GetFlag(void)
{
	return &g_aFlag[0];
}