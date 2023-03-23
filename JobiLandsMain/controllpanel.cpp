//=============================================================================
//
// コントロールパネル処理 [controllpanel.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "controllpanel.h"
#include "input.h"
#include "debugproc.h"
#include "game.h"
#include "player.h"
#include "debugproc.h"
#include "sound.h"

//マクロ定義
#define POS_X		(50.0f)	//ゲージの長さ
#define POS_Y		(50.0f)	//ゲージの長さ
#define POS_X_FRAM		(150.0f)	//ゲージの長さ
#define POS_Y_FRAM		(70.0f)	//ゲージの長さ
#define DOWN_MOVE	(0.05f)	//減る速度
#define UP_POS		(4.0f)	//増える大きさ

//プロタイプ宣言
void UpdateGameControllPanel(void);

//グローバル変数
const char *c_apFilenameControllPanel[] =		//ファイル読み込み
{
	NULL,
	"data\\TEXTURE\\up.png",
	"data\\TEXTURE\\down.png",
};
LPDIRECT3DTEXTURE9 g_apTextureControllPanel[(sizeof c_apFilenameControllPanel) / sizeof(*c_apFilenameControllPanel)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffControllPanel = NULL;
ControllPanel g_aControllPanel;	//コントロールパネルの情報

//==============================================================
//コントロールパネルの初期化処理
//==============================================================
void InitControllPanel(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameControllPanel) / sizeof(*c_apFilenameControllPanel); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameControllPanel[nCntTex],
			&g_apTextureControllPanel[nCntTex]);
	}

	//各種変数初期化
	for (int nCntControllPanel = 0; nCntControllPanel < CONTROLLPANEL_VTX_MAX; nCntControllPanel++)
	{
		g_aControllPanel.aUI[nCntControllPanel].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f);	//頂点座標
		g_aControllPanel.aUI[nCntControllPanel].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//頂点カラー
		g_aControllPanel.aUI[nCntControllPanel].fWidth = POS_X;		//幅
		g_aControllPanel.aUI[nCntControllPanel].fHeight = POS_Y;		//高さ
		g_aControllPanel.aUI[nCntControllPanel].nPatternAnim = 0;	//アニメーションパターンNo
		g_aControllPanel.aUI[nCntControllPanel].nCntAnim = 0;		//アニメーションカウンター
		g_aControllPanel.aUI[nCntControllPanel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_aControllPanel.aUI[nCntControllPanel].fAngle = 0.0f;		//角度
		g_aControllPanel.aUI[nCntControllPanel].fLength = 0.0f;		//対角線の長さ
		g_aControllPanel.aUI[nCntControllPanel].nCount = 0;			//雑カウント
		g_aControllPanel.aUI[nCntControllPanel].bUse = false;			//使用しているか
		g_aControllPanel.aUI[nCntControllPanel].aTexU = 0.0f;		//テクスチャ座標U[スクロール用]
		g_aControllPanel.aUI[nCntControllPanel].aTexV = 0.0f;		//テクスチャ座標V[スクロール用]
	}
	g_aControllPanel.aUI[CONTROLLPANEL_VTX_FRAM].pos = D3DXVECTOR3(1000.0f, 600.0f, 0.0f);	//頂点座標
	g_aControllPanel.aUI[CONTROLLPANEL_VTX_UPBUTTON].pos = D3DXVECTOR3(700.0f, 600.0f, 0.0f);	//頂点座標
	g_aControllPanel.aUI[CONTROLLPANEL_VTX_DWBUTTON].pos = D3DXVECTOR3(800.0f, 600.0f, 0.0f);	//頂点座標
	g_aControllPanel.nSelect = -1;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * CONTROLLPANEL_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffControllPanel,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffControllPanel->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntControllPanel = 0; nCntControllPanel < CONTROLLPANEL_VTX_MAX; nCntControllPanel++)
	{
		//頂点座標の設定
		if (nCntControllPanel != CONTROLLPANEL_VTX_FRAM)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aControllPanel.aUI[nCntControllPanel].pos.x - POS_X, g_aControllPanel.aUI[nCntControllPanel].pos.y - POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aControllPanel.aUI[nCntControllPanel].pos.x + POS_X, g_aControllPanel.aUI[nCntControllPanel].pos.y - POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aControllPanel.aUI[nCntControllPanel].pos.x - POS_X, g_aControllPanel.aUI[nCntControllPanel].pos.y + POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aControllPanel.aUI[nCntControllPanel].pos.x + POS_X, g_aControllPanel.aUI[nCntControllPanel].pos.y + POS_Y, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(g_aControllPanel.aUI[nCntControllPanel].pos.x - POS_X_FRAM, g_aControllPanel.aUI[nCntControllPanel].pos.y - POS_Y_FRAM, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aControllPanel.aUI[nCntControllPanel].pos.x + POS_X_FRAM, g_aControllPanel.aUI[nCntControllPanel].pos.y - POS_Y_FRAM, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aControllPanel.aUI[nCntControllPanel].pos.x - POS_X_FRAM, g_aControllPanel.aUI[nCntControllPanel].pos.y + POS_Y_FRAM, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aControllPanel.aUI[nCntControllPanel].pos.x + POS_X_FRAM, g_aControllPanel.aUI[nCntControllPanel].pos.y + POS_Y_FRAM, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = g_aControllPanel.aUI[nCntControllPanel].col;
		pVtx[1].col = g_aControllPanel.aUI[nCntControllPanel].col;
		pVtx[2].col = g_aControllPanel.aUI[nCntControllPanel].col;
		pVtx[3].col = g_aControllPanel.aUI[nCntControllPanel].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffControllPanel->Unlock();
}

//==============================================================
//コントロールパネルの画面の終了処理
//==============================================================
void UninitControllPanel(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameControllPanel) / sizeof(*c_apFilenameControllPanel); nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureControllPanel[nCntTex] != NULL)
		{
			g_apTextureControllPanel[nCntTex]->Release();
			g_apTextureControllPanel[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffControllPanel != NULL)
	{
		g_pVtxBuffControllPanel->Release();
		g_pVtxBuffControllPanel = NULL;
	}

}

//==============================================================
//コントロールパネルの画面の更新処理
//==============================================================
void UpdateControllPanel(void)
{
	switch (GetMode())
	{
	case MODE_GAME:
		UpdateGameControllPanel();
		break;
	}
}

//==============================================================
//コントロールパネルのゲーム時更新処理
//==============================================================
void UpdateGameControllPanel(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffControllPanel->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntControllPanel = 0; nCntControllPanel < CONTROLLPANEL_VTX_MAX; nCntControllPanel++)
	{
		if (g_aControllPanel.nSelect != -1 && g_aControllPanel.nSelect == nCntControllPanel)
		{
			//頂点カラー
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			//頂点カラー
			pVtx[0].col = g_aControllPanel.aUI[nCntControllPanel].col;
			pVtx[1].col = g_aControllPanel.aUI[nCntControllPanel].col;
			pVtx[2].col = g_aControllPanel.aUI[nCntControllPanel].col;
			pVtx[3].col = g_aControllPanel.aUI[nCntControllPanel].col;
		}

		pVtx += 4;
	}
	//頂点バッファをアンロックする
	g_pVtxBuffControllPanel->Unlock();
}

//==============================================================
//コントロールパネルの画面の描画処理
//==============================================================
void DrawControllPanel(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffControllPanel, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntControllPanel = 0; nCntControllPanel < CONTROLLPANEL_VTX_MAX; nCntControllPanel++)
	{
		//テクスチャセット
		pDevice->SetTexture(0, g_apTextureControllPanel[nCntControllPanel]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntControllPanel * 4, 2);	//プリミティブの種類
	}
}

//==============================================================
//コントロールパネルの情報
//==============================================================
ControllPanel *GetControllPanel(void)
{
	return &g_aControllPanel;
}