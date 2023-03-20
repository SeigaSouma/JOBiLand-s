//=============================================================================
//
// 催眠ゲージ処理 [hypnosis_gauge.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "hypnosis_gauge.h"
#include "player.h"
#include "camera.h"
#include "mob.h"

//マクロ定義
#define MAX_WIDTH	(25.0f)	//横幅
#define MAX_HEIGHT	(5.0f)		//縦幅
#define MAX_TOP		(4)
#define MAX_HYP		(10)		//催眠度の最大値

//プロトタイプ宣言
void GaugeDecrement(int nCntGauge);
void ChangeColorHypnosisGauge(int nCntGauge);

//グローバル変数宣言
const char *c_apFilenameHypnosisGauge[] =		//ファイル読み込み
{
	"data\\TEXTURE\\hypnosis_fram.png",
};
LPDIRECT3DTEXTURE9 g_pTextureHypnosisGauge[(sizeof c_apFilenameHypnosisGauge) / sizeof(*c_apFilenameHypnosisGauge)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHypnosisGauge = NULL;	//頂点バッファへのポインタ
HypnosisGauge g_aHypnosisGauge[MAX_MOB];				//HPゲージの情報

//==================================================================================
//催眠度ゲージの初期化処理
//==================================================================================
void InitHypnosisGauge(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameHypnosisGauge) / sizeof(*c_apFilenameHypnosisGauge); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameHypnosisGauge[nCntTex],
			&g_pTextureHypnosisGauge[nCntTex]);
	}

	//各要素初期化
	for (int nCntGauge = 0; nCntGauge < MAX_MOB; nCntGauge++)
	{//ゲージ分初期化

		g_aHypnosisGauge[nCntGauge].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aHypnosisGauge[nCntGauge].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aHypnosisGauge[nCntGauge].mtxWorld);
		g_aHypnosisGauge[nCntGauge].bUse = false;				//使用しているか
		g_aHypnosisGauge[nCntGauge].bDisp = false;				//描画するか
		g_aHypnosisGauge[nCntGauge].nMaxHyp = MAX_HYP;			//最大値
		g_aHypnosisGauge[nCntGauge].nHypnosis = 0;			//現在地

		for (int nCntFrame = 0; nCntFrame < HYPNISISGAUGE_VTX_MAX; nCntFrame++)
		{//枠分初期化

			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);		//色
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth = MAX_WIDTH;		//横幅
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fMaxWidth = MAX_WIDTH;	//横最大幅
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight = MAX_HEIGHT;		//縦幅
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidthDest = MAX_WIDTH;	//目標の幅
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidthDiff = 0.0f;	//幅の差分
		}
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * (MAX_MOB * HYPNISISGAUGE_VTX_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHypnosisGauge,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHypnosisGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_MOB; nCntGauge++)
	{
		for (int nCntFrame = 0; nCntFrame < HYPNISISGAUGE_VTX_MAX; nCntFrame++)
		{//枠分初期化

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth, g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth, g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth, -g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth, -g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);

			//法線ベクトルの設定
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//頂点カラーの設定
			switch (nCntFrame)
			{
			case HYPNISISGAUGE_VTX_BLACK:		//下地

				pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				break;

			case HYPNISISGAUGE_VTX_PINK:		//変動するゲージ

				pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
				break;

			case HYPNISISGAUGE_VTX_FRAM:		//フレーム

				pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += MAX_TOP;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHypnosisGauge->Unlock();

	
}

//==================================================================================
//催眠度ゲージの終了処理
//==================================================================================
void UninitHypnosisGauge(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameHypnosisGauge) / sizeof(*c_apFilenameHypnosisGauge); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureHypnosisGauge[nCntTex] != NULL)
		{
			g_pTextureHypnosisGauge[nCntTex]->Release();
			g_pTextureHypnosisGauge[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffHypnosisGauge != NULL)
	{
		g_pVtxBuffHypnosisGauge->Release();
		g_pVtxBuffHypnosisGauge = NULL;
	}
}

//==================================================================================
//催眠度ゲージの更新処理
//==================================================================================
void UpdateHypnosisGauge(void)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHypnosisGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_MOB; nCntGauge++)
	{
		if (g_aHypnosisGauge[nCntGauge].bUse == true && g_aHypnosisGauge[nCntGauge].bDisp == true)
		{//使用されていたら

			//ゲージ減少処理
			GaugeDecrement(nCntGauge);

			//色更新
			//ChangeColorHypnosisGauge(nCntGauge);

			//頂点座標の設定
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 0].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth), g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fHeight, 0.0f);
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 1].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth), g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fHeight, 0.0f);
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 2].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth), -g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fHeight, 0.0f);
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 3].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth), -g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fHeight, 0.0f);

			//頂点カラーの設定
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 0].col =
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 1].col =
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 2].col =
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 3].col = g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col;
		}

		pVtx += HYPNISISGAUGE_VTX_MAX * 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHypnosisGauge->Unlock();
}

//==================================================================================
//催眠度ゲージの減少処理
//==================================================================================
void GaugeDecrement(int nCntGauge)
{
	//差分で徐々に減らしていく
	g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth +=
		(g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidthDest - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth) * 0.025f;
}

//==================================================================================
//催眠度ゲージの色更新
//==================================================================================
void ChangeColorHypnosisGauge(int nCntGauge)
{

	if (
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth <= 0.95f &&
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth > 0.6f)
	{//HPゲージ8割
		
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.2f, 0.6f, 0.2f, 1.0f);
	}
	else if (
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth <= 0.6f &&
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth > 0.4f)
	{//HPゲージ6割

		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.8f, 0.7f, 0.2f, 1.0f);
	}
	else if (
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth <= 0.4f &&
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth > 0.2f)
	{//HPゲージ4割

		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.8f, 0.5f, 0.2f, 1.0f);
	}
	else if (g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth <= 0.2f)
	{//危険エリア

		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	}
	else
	{//満タン

		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}

}

//==================================================================================
//催眠度ゲージの位置更新
//==================================================================================
void SetPositionHypnosisGauge(int nIdxGauge, D3DXVECTOR3 pos, int nHypnosis)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	if (g_aHypnosisGauge[nIdxGauge].bUse == true && g_aHypnosisGauge[nIdxGauge].bDisp == true)
	{//使用されていたら

			g_aHypnosisGauge[nIdxGauge].pos.x = pos.x;
			g_aHypnosisGauge[nIdxGauge].pos.y = pos.y + 70.0f;
			g_aHypnosisGauge[nIdxGauge].pos.z = pos.z;
			g_aHypnosisGauge[nIdxGauge].nHypnosis = nHypnosis;

			//目標の幅設定
			g_aHypnosisGauge[nIdxGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidthDest =
				g_aHypnosisGauge[nIdxGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth * ((float)g_aHypnosisGauge[nIdxGauge].nHypnosis / (float)g_aHypnosisGauge[nIdxGauge].nMaxHyp);
	}
}

//==================================================================================
//催眠度ゲージの描画処理
//==================================================================================
void DrawHypnosisGauge(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_MOB; nCntGauge++)
	{
		if (g_aHypnosisGauge[nCntGauge].bUse == true && g_aHypnosisGauge[nCntGauge].bDisp == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aHypnosisGauge[nCntGauge].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aHypnosisGauge[nCntGauge].mtxWorld, NULL, &mtxView);	//逆配列を求める

			//上で消える
			g_aHypnosisGauge[nCntGauge].mtxWorld._21 = 0.0f;
			g_aHypnosisGauge[nCntGauge].mtxWorld._23 = 0.0f;
			g_aHypnosisGauge[nCntGauge].mtxWorld._24 = 0.0f;

			g_aHypnosisGauge[nCntGauge].mtxWorld._41 = 0.0f;
			g_aHypnosisGauge[nCntGauge].mtxWorld._42 = 0.0f;
			g_aHypnosisGauge[nCntGauge].mtxWorld._43 = 0.0f;

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aHypnosisGauge[nCntGauge].pos.x, g_aHypnosisGauge[nCntGauge].pos.y, g_aHypnosisGauge[nCntGauge].pos.z);
			D3DXMatrixMultiply(&g_aHypnosisGauge[nCntGauge].mtxWorld, &g_aHypnosisGauge[nCntGauge].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aHypnosisGauge[nCntGauge].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffHypnosisGauge, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			for (int nCntFrame = 0; nCntFrame < HYPNISISGAUGE_VTX_MAX; nCntFrame++)
			{//枠分

				//テクスチャの設定
				switch (nCntFrame)
				{
				case HYPNISISGAUGE_VTX_FRAM:
					pDevice->SetTexture(0, g_pTextureHypnosisGauge[0]);
					break;

				default:
					pDevice->SetTexture(0, NULL);
					break;
				}

				//HPゲージの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntGauge * 4 * HYPNISISGAUGE_VTX_MAX) + (nCntFrame * 4), 2);

			}
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==================================================================================
//催眠度ゲージの設定処理
//==================================================================================
int SetHypnosisGauge(float fWidth, float fHeight, int nMaxHyp)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffHypnosisGauge->Lock(0, 0, (void**)&pVtx, 0);

	int nCntGauge = -1;
	int nn = 0;

	for (int nCount = 0; nCount < MAX_MOB; nCount++)
	{
		if (g_aHypnosisGauge[nCount].bUse == false)
		{//使用していなかったら

			for (int nCntFrame = 0; nCntFrame < HYPNISISGAUGE_VTX_MAX; nCntFrame++)
			{//枠分

				nCntGauge = nCount;		//インデックス番号取得
				g_aHypnosisGauge[nCntGauge].bUse = true;		//使用している状態にする
				g_aHypnosisGauge[nCntGauge].bDisp = true;		//描画している状態にする
				g_aHypnosisGauge[nCntGauge].nMaxHyp = MAX_HYP;	//最大値設定

				g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight = fHeight;

				if (nCntFrame == HYPNISISGAUGE_VTX_PINK)
				{//ピンクの時
					g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth = 0;
					g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fMaxWidth = fWidth;

				}
				else
				{
					g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth = fWidth;
					g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fMaxWidth = g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth;
				}



				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth), g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth), g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth), -g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth), -g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);

				pVtx += MAX_TOP;
			}
			break;
		}

		pVtx += HYPNISISGAUGE_VTX_MAX * 4;
	
	}

	//頂点バッファをアンロックする
	g_pVtxBuffHypnosisGauge->Unlock();

	return nCntGauge;
}

//==================================================================================
//催眠度ゲージの情報取得
//==================================================================================
HypnosisGauge *GetHypnosisGauge(void)
{
	return &g_aHypnosisGauge[0];
}

//==================================================================================
//催眠度ゲージが使用されてない状態にする処理
//==================================================================================
void GetHypnosisUse(int nIdxGauge, bool bUse)
{
	g_aHypnosisGauge[nIdxGauge].bUse = bUse;
}