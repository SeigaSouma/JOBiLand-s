//=============================================================================
//
// 派遣テクスチャ処理 [dispatch_texture.cpp]
// Author : 佐藤根詩音
//
//=============================================================================
#include "dispatch_texture.h"
#include "buddy.h"

//マクロ定義
#define NUM_TEX			(2)				//テクスチャの数
#define MAX_WIDTH		(80.0f)		//横
#define MAX_HEIGHT		(80.0f)		//縦
#define POS_X			(310.0f)		//Xの位置
#define POS_Y			(620.0f)		//Yの位置

//グローバル変数
const char *c_apFilenameDispatchTex[] =		//ファイル読み込み
{
	"data\\TEXTURE\\dispatch_gari.png",
	"data\\TEXTURE\\dispatch_tibi.png",
};
LPDIRECT3DTEXTURE9 g_apTextureDispatchTex[(sizeof c_apFilenameDispatchTex) / sizeof(*c_apFilenameDispatchTex)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDispatchTex = NULL;
DispatchTex g_aDispatchTex[NUM_TEX];

//==============================================================
//派遣テクスチャの初期化処理
//==============================================================
void InitDispatchTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameDispatchTex) / sizeof(*c_apFilenameDispatchTex); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameDispatchTex[nCntTex],
			&g_apTextureDispatchTex[nCntTex]);
	}

	//変数初期化
	for (int nCntDisTex = 0; nCntDisTex < NUM_TEX; nCntDisTex++)
	{
		if (nCntDisTex == 0)
		{
			g_aDispatchTex[nCntDisTex].pos = D3DXVECTOR3(640.0f - POS_X, POS_Y, 0.0f);		//位置

		}
		else if (nCntDisTex == 1)
		{
			g_aDispatchTex[nCntDisTex].pos = D3DXVECTOR3(640.0f + POS_X, POS_Y, 0.0f);		//位置
		}

		g_aDispatchTex[nCntDisTex].nCntDispatch = 0;
		g_aDispatchTex[nCntDisTex].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDispatchTex,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDispatchTex->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisTex = 0; nCntDisTex < NUM_TEX; nCntDisTex++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_aDispatchTex[nCntDisTex].pos.x - MAX_WIDTH, g_aDispatchTex[nCntDisTex].pos.y - MAX_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aDispatchTex[nCntDisTex].pos.x + MAX_WIDTH, g_aDispatchTex[nCntDisTex].pos.y - MAX_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aDispatchTex[nCntDisTex].pos.x - MAX_WIDTH, g_aDispatchTex[nCntDisTex].pos.y + MAX_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aDispatchTex[nCntDisTex].pos.x + MAX_WIDTH, g_aDispatchTex[nCntDisTex].pos.y + MAX_HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDispatchTex->Unlock();
}

//==============================================================
//派遣テクスチャの画面の終了処理
//==============================================================
void UninitDispatchTex(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameDispatchTex) / sizeof(*c_apFilenameDispatchTex); nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureDispatchTex[nCntTex] != NULL)
		{
			g_apTextureDispatchTex[nCntTex]->Release();
			g_apTextureDispatchTex[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffDispatchTex != NULL)
	{
		g_pVtxBuffDispatchTex->Release();
		g_pVtxBuffDispatchTex = NULL;
	}
}

//==============================================================
//派遣テクスチャの画面の更新処理
//==============================================================
void UpdateDispatchTex(void)
{
	VERTEX_2D *pVtx;

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffDispatchTex->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisTex = 0; nCntDisTex < NUM_TEX; nCntDisTex++, pBuddy++)
	{
		if (pBuddy->nState == BUDDYSTATE_DISPATCH)
		{
			g_aDispatchTex[nCntDisTex].nCntDispatch = 1;		//派遣
		}
		else
		{
			g_aDispatchTex[nCntDisTex].nCntDispatch = 0;		//派遣
		}

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aDispatchTex[nCntDisTex].nCntDispatch * 0.5f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f + g_aDispatchTex[nCntDisTex].nCntDispatch * 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + g_aDispatchTex[nCntDisTex].nCntDispatch * 0.5f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f + g_aDispatchTex[nCntDisTex].nCntDispatch * 0.5f, 1.0f);
		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDispatchTex->Unlock();
}

//==============================================================
//派遣テクスチャの画面の描画処理
//==============================================================
void DrawDispatchTex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffDispatchTex, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDispatchTex = 0; nCntDispatchTex < NUM_TEX; nCntDispatchTex++)
	{
		//テクスチャセット
		pDevice->SetTexture(0, g_apTextureDispatchTex[nCntDispatchTex]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDispatchTex * 4, 2);	//プリミティブの種類
	}

	////Zテストを有効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================
//派遣テクスチャの情報
//==============================================================
DispatchTex *GetDispatchTex(void)
{
	return &g_aDispatchTex[0];
}