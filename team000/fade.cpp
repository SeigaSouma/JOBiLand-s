//==============================================================
//
//DirectX[fade.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include"fade.h"

//マクロ定義
#define WIDTH	(960.0f)
#define HEIGHT	(360.0f)
#define WIDTH_MOVE	(640.0f * 0.05f)
#define HEIGHT_MOVE	(360.0f * 0.05f)
#define MOVE	(35.0f)
#define POS_X_DEST	(SCREEN_WIDTH + WIDTH)

//グローバル変数
const char *c_apFilenameFade[] =	//ファイル読み込み
{
	"data\\TEXTURE\\fade_01.png",
	"data\\TEXTURE\\fade_02.png",
	"data\\TEXTURE\\fade_03.png",
};

LPDIRECT3DTEXTURE9 g_pTextureFade[(sizeof c_apFilenameFade) / sizeof(*c_apFilenameFade)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		//頂点バッファへのポインタ
MODE g_modeNext;									//次の画面（モード）
Fade g_aFade;

//==============================================================
//フェードの初期化処理
//==============================================================
void InitFade(MODE modeNext)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameFade) / sizeof(*c_apFilenameFade); CntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameFade[CntTex],
			&g_pTextureFade[CntTex]);
	}


	for (int nCntFade = 0; nCntFade < FADE_VTX_MAX; nCntFade++)
	{
		g_aFade.fWidth[nCntFade] = WIDTH;		//幅
		g_aFade.fHeight[nCntFade] = HEIGHT;		//高さ
	}
	g_aFade.pos = D3DXVECTOR3(320.0f, 360.0f, 0.0f);	//位置
	g_aFade.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		//色
	g_aFade.nState = FADE_IN;			//種類
	g_aFade.nCntZoom = 0;		//大きくなる時間
	g_aFade.nCntVoice = 0;		//ボイスのカウンター
	g_aFade.nTexType = rand() % 3;		//テクスチャの種類
	g_modeNext = modeNext;		//次の画面（モード）を設定

								//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * FADE_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFade = 0; nCntFade < FADE_VTX_MAX; nCntFade++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aFade.pos.x - g_aFade.fWidth[nCntFade], g_aFade.pos.y - g_aFade.fHeight[nCntFade], 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFade.pos.x + g_aFade.fWidth[nCntFade], g_aFade.pos.y - g_aFade.fHeight[nCntFade], 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFade.pos.x - g_aFade.fWidth[nCntFade], g_aFade.pos.y + g_aFade.fHeight[nCntFade], 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFade.pos.x + g_aFade.fWidth[nCntFade], g_aFade.pos.y + g_aFade.fHeight[nCntFade], 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

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

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();

	//モードの設定
	SetMode(g_modeNext);
}

//==============================================================
//フェードの終了処理
//==============================================================
void UninitFade(void)
{
	//テクスチャの破棄
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameFade) / sizeof(*c_apFilenameFade); CntTex++)
	{
		if (g_pTextureFade[CntTex] != NULL)
		{
			g_pTextureFade[CntTex]->Release();
			g_pTextureFade[CntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//==============================================================
//フェードの更新処理
//==============================================================
void UpdateFade(void)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ

							//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aFade.nState != FADE_NONE)
	{
		if (g_aFade.nState == FADE_IN)
		{//フェードイン状態

		 //ポリゴンをおおきくしていく
			g_aFade.pos.x += MOVE;

			if (g_aFade.pos.x >= POS_X_DEST)
			{
				//初期値で固定
				g_aFade.pos.x = POS_X_DEST;

				//通常状態に
				g_aFade.nState = FADE_NONE;
			}
		}
		else if (g_aFade.nState == FADE_OUT)
		{//フェードアウト状態

		 //ポリゴンを小さくしていく
			g_aFade.pos.x -= MOVE;
			//g_aFade.fWidth[FADE_VTX_SHAPE] -= WIDTH_MOVE;
			//g_aFade.fHeight[FADE_VTX_SHAPE] -= HEIGHT_MOVE;

			if (g_aFade.pos.x <= 320.0f)
			{
				//0.0fで固定
				g_aFade.pos.x = 320.0f;

				//フェードイン状態に
				g_aFade.nState = FADE_IN;

				//モード設定（次の画面に移行）
				SetMode(g_modeNext);
			}
		}
		else if (g_aFade.nState == FADE_VOICEOUT)
		{//フェードアウト状態

			g_aFade.nCntVoice++;

			if (g_aFade.nCntVoice >= 100)
			{
				//ポリゴンを小さくしていく
				g_aFade.pos.x -= MOVE;
			}

			if (g_aFade.pos.x <= 320.0f)
			{
				//0.0fで固定
				g_aFade.pos.x = 320.0f;

				//フェードイン状態に
				g_aFade.nState = FADE_IN;

				g_aFade.nCntVoice = 0;

				//モード設定（次の画面に移行）
				SetMode(g_modeNext);
			}
		}
	}

	//頂点カラーの設定
	pVtx[0].pos = D3DXVECTOR3(g_aFade.pos.x - g_aFade.fWidth[FADE_VTX_FADE], g_aFade.pos.y - g_aFade.fHeight[FADE_VTX_FADE], 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aFade.pos.x + g_aFade.fWidth[FADE_VTX_FADE], g_aFade.pos.y - g_aFade.fHeight[FADE_VTX_FADE], 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aFade.pos.x - g_aFade.fWidth[FADE_VTX_FADE], g_aFade.pos.y + g_aFade.fHeight[FADE_VTX_FADE], 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aFade.pos.x + g_aFade.fWidth[FADE_VTX_FADE], g_aFade.pos.y + g_aFade.fHeight[FADE_VTX_FADE], 0.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffFade->Unlock();
}

//==============================================================
//フェードの描画処理
//==============================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

									//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntFade = 0; nCntFade < FADE_VTX_MAX; nCntFade++)
	{
		//テクスチャセット
		pDevice->SetTexture(0, g_pTextureFade[g_aFade.nTexType]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFade * 4, 2);	//プリミティブの種類
	}

}

//==============================================================
//フェードの設定処理
//==============================================================
void SetFade(MODE modeNext)
{
	if (g_aFade.nState == FADE_NONE)
	{
		g_aFade.nState = FADE_OUT;										//フェードアウト状態
		g_modeNext = modeNext;									//次の画面（モード）を設定

																//初期値で固定
		g_aFade.pos.x = POS_X_DEST;

		g_aFade.nTexType = rand() % 3;		//テクスチャの種類

	}
}

//==============================================================
//フェードの設定処理
//==============================================================
void SetVoiceFade(MODE modeNext)
{
	if (g_aFade.nState == FADE_NONE)
	{
		g_aFade.nState = FADE_VOICEOUT;										//フェードアウト状態
		g_modeNext = modeNext;									//次の画面（モード）を設定
																//初期値で固定
		g_aFade.pos.x = POS_X_DEST;
		g_aFade.nTexType = rand() % 3;		//テクスチャの種類
	}
}

//==============================================================
//フェードの取得処理
//==============================================================
FADE GetFade(void)
{
	return g_aFade.nState;
}