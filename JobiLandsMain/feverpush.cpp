//=============================================================================
//
// フィーバープッシュ処理 [feverpush.cpp]
// Author : 佐藤根詩音
//
//=============================================================================
#include "feverpush.h"
#include "fevergauge.h"
#include "particle.h"

//マクロ定義
#define MAX_WIDTH		(320.0f)		//横
#define MAX_HEIGHT		(120.0f)		//縦
#define MIN_WIDTH		(320.0f * 0.8f)		//横
#define MIN_HEIGHT		(120.0f * 0.8f)		//縦

//グローバル変数
const char *c_apFilenameFeverPush[] =		//ファイル読み込み
{
	"data\\TEXTURE\\feverpush.png",
};
LPDIRECT3DTEXTURE9 g_apTextureFeverPush[(sizeof c_apFilenameFeverPush) / sizeof(*c_apFilenameFeverPush)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFeverPush = NULL;
FeverPush g_aFeverPush;

//==============================================================
//フィーバープッシュの初期化処理
//==============================================================
void InitFeverPush(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameFeverPush) / sizeof(*c_apFilenameFeverPush); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameFeverPush[nCntTex],
			&g_apTextureFeverPush[nCntTex]);
	}

	//変数初期化
	g_aFeverPush.pos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);	//位置
	g_aFeverPush.fWidth = MIN_WIDTH;			//横幅
	g_aFeverPush.fHeight = MIN_HEIGHT;			//縦幅
	g_aFeverPush.fWidthMove = (MAX_WIDTH * 0.035f);		//横幅
	g_aFeverPush.fHeightMove = (MAX_HEIGHT * 0.035f);		//縦幅
	g_aFeverPush.bUse = false;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFeverPush,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFeverPush->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_aFeverPush.pos.x - g_aFeverPush.fWidth, g_aFeverPush.pos.y - g_aFeverPush.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aFeverPush.pos.x + g_aFeverPush.fWidth, g_aFeverPush.pos.y - g_aFeverPush.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aFeverPush.pos.x - g_aFeverPush.fWidth, g_aFeverPush.pos.y + g_aFeverPush.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aFeverPush.pos.x + g_aFeverPush.fWidth, g_aFeverPush.pos.y + g_aFeverPush.fHeight, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFeverPush->Unlock();
}

//==============================================================
//フィーバープッシュの画面の終了処理
//==============================================================
void UninitFeverPush(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameFeverPush) / sizeof(*c_apFilenameFeverPush); nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureFeverPush[nCntTex] != NULL)
		{
			g_apTextureFeverPush[nCntTex]->Release();
			g_apTextureFeverPush[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffFeverPush != NULL)
	{
		g_pVtxBuffFeverPush->Release();
		g_pVtxBuffFeverPush = NULL;
	}
}

//==============================================================
//フィーバープッシュの画面の更新処理
//==============================================================
void UpdateFeverPush(void)
{

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFeverPush->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aFeverPush.bUse == true)
	{//使用してるとき

		g_aFeverPush.fWidth += g_aFeverPush.fWidthMove;
		g_aFeverPush.fHeight += g_aFeverPush.fHeightMove;

		if (g_aFeverPush.fWidth >= MAX_WIDTH)
		{//最大値を超えたら

			//最大値に固定
			g_aFeverPush.fWidth = MAX_WIDTH;
			g_aFeverPush.fHeight = MAX_HEIGHT;

			//移動逆転
			g_aFeverPush.fWidthMove = -(MAX_WIDTH * 0.005f);
			g_aFeverPush.fHeightMove = -(MAX_HEIGHT * 0.005f);
		}
		else if (g_aFeverPush.fWidth <= MIN_WIDTH)
		{//最小値下回ったら

			//最大値に固定
			g_aFeverPush.fWidth = MIN_WIDTH;
			g_aFeverPush.fHeight = MIN_HEIGHT;

			//移動逆転
			g_aFeverPush.fWidthMove = (MAX_WIDTH * 0.025f);		//横幅
			g_aFeverPush.fHeightMove = (MAX_HEIGHT * 0.025f);		//縦幅
		}

		//押せのパーティクル
		SetParticle(D3DXVECTOR3(g_aFeverPush.pos.x - 130.0f, g_aFeverPush.pos.y + 30.0f, 0.0f), PARTICLE_TYPE_FEVERPUSH);
		SetParticle(D3DXVECTOR3(g_aFeverPush.pos.x + 130.0f, g_aFeverPush.pos.y + 30.0f, 0.0f), PARTICLE_TYPE_FEVERPUSH);
	}


	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aFeverPush.pos.x - g_aFeverPush.fWidth, g_aFeverPush.pos.y - g_aFeverPush.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aFeverPush.pos.x + g_aFeverPush.fWidth, g_aFeverPush.pos.y - g_aFeverPush.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aFeverPush.pos.x - g_aFeverPush.fWidth, g_aFeverPush.pos.y + g_aFeverPush.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aFeverPush.pos.x + g_aFeverPush.fWidth, g_aFeverPush.pos.y + g_aFeverPush.fHeight, 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFeverPush->Unlock();
}

//==============================================================
//フィーバープッシュの画面の描画処理
//==============================================================
void DrawFeverPush(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFeverPush, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntFeverPush = 0; nCntFeverPush < 1; nCntFeverPush++)
	{
		if (g_aFeverPush.bUse == true)
		{
			//テクスチャセット
			pDevice->SetTexture(0, g_apTextureFeverPush[nCntFeverPush]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFeverPush * 4, 2);	//プリミティブの種類
		}
	}

	////Zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================
//フィーバープッシュの情報
//==============================================================
FeverPush *GetFeverPush(void)
{
	return &g_aFeverPush;
}