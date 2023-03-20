//=============================================================================
//
// イベントウィンドウ処理 [EventWindow.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "EventWindow.h"
#include "manyMORS.h"
#include "input.h"
#include "light.h"
#include "sound.h"

#define MOVE_WINDOW (1.5f)
#define MAX_TIME	(1350)
#define TEXT_X		(500.0f)
#define WINDOW_X	(700.0f)
#define WINDOW_Y	(100.0f)
#define LINE_Y		(25.0f)

//プロトタイプ宣言
void EWindowStepRed(void);
void EWindowStepFramMove(void);
void EWindowStepTextLine(void);
void EWindowStepOut(void);

//グローバル変数宣言
const char *c_apFilenameE_Window[] =	//ファイル読み込み
{
	NULL,	//大量発生中の文字
	"data\\TEXTURE\\warning_fram.png",	//警備強化の枠
	"data\\TEXTURE\\warning_text.png",	//警備強化のテキスト
	"data\\TEXTURE\\warning_01.png",	//警備強化のテキスト
	"data\\TEXTURE\\warning_01.png",	//警備強化のテキスト
};

LPDIRECT3DTEXTURE9 g_pTextureE_Window[(sizeof c_apFilenameE_Window) / sizeof(*c_apFilenameE_Window)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffE_Window = NULL;		//頂点バッファへのポインタ
EVENTWINDOW g_E_Window;			//文章

//=============================================================================
//イベントウィンドウの初期化処理
//=============================================================================					   
void InitEventWindow(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameE_Window) / sizeof(*c_apFilenameE_Window); CntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameE_Window[CntTex],
			&g_pTextureE_Window[CntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * EVENTWINDOW_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffE_Window,
		NULL);

	//各要素初期化
	g_E_Window.bUse = false;		//使用しているか
	g_E_Window.nStep = EVENTWINDOW_STEP_RED;		//ステップ

	//UI情報初期化
	for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
	{
		g_E_Window.aUI[nCntWindow].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f, 0.0f);	//頂点座標
		g_E_Window.aUI[nCntWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//頂点カラー
		g_E_Window.aUI[nCntWindow].fWidth = WINDOW_X;		//幅
		g_E_Window.aUI[nCntWindow].fHeight = WINDOW_Y;		//高さ
		g_E_Window.aUI[nCntWindow].nPatternAnim = 0;	//アニメーションパターンNo
		g_E_Window.aUI[nCntWindow].nCntAnim = 0;		//アニメーションカウンター
		g_E_Window.aUI[nCntWindow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_E_Window.aUI[nCntWindow].fAngle = 0.0f;		//角度
		g_E_Window.aUI[nCntWindow].fLength = 0.0f;		//対角線の長さ
		g_E_Window.aUI[nCntWindow].nCount = 0;			//雑カウント
		g_E_Window.aUI[nCntWindow].bUse = false;			//使用しているか
		g_E_Window.aUI[nCntWindow].aTexU = 0.0f;		//テクスチャ座標U[スクロール用]
		g_E_Window.aUI[nCntWindow].aTexV = 0.0f;		//テクスチャ座標V[スクロール用]
	}

	//色の設定
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//テクスチャ座標の設定
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU = 0.0f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU = 1.0f;

	//位置の設定
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f);
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f);

	//幅高さの設定
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].fWidth = 640.0f;
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].fHeight = 360.0f;
	g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].fWidth = WINDOW_X;
	g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].fHeight = WINDOW_Y;
	g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].fWidth = TEXT_X;
	g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].fHeight = WINDOW_Y;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].fWidth = WINDOW_X;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].fHeight = LINE_Y;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].fWidth = WINDOW_X;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].fHeight = LINE_Y;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffE_Window->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x - g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y - g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x + g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y - g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x - g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y + g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x + g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y + g_E_Window.aUI[nCntWindow].fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = g_E_Window.aUI[nCntWindow].col;
		pVtx[1].col = g_E_Window.aUI[nCntWindow].col;
		pVtx[2].col = g_E_Window.aUI[nCntWindow].col;
		pVtx[3].col = g_E_Window.aUI[nCntWindow].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffE_Window->Unlock();
}

//=============================================================================
//イベントウィンドウの終了処理
//=============================================================================
void UninitEventWindow(void)
{

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameE_Window) / sizeof(*c_apFilenameE_Window); CntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureE_Window[CntTex] != NULL)
		{
			g_pTextureE_Window[CntTex]->Release();
			g_pTextureE_Window[CntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffE_Window != NULL)
	{
		g_pVtxBuffE_Window->Release();
		g_pVtxBuffE_Window = NULL;
	}
}

//=============================================================================
//イベントウィンドウの更新処理
//=============================================================================
void UpdateEventWindow(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffE_Window->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_E_Window.bUse == true)
	{//使用されていたら

		switch (g_E_Window.nStep)
		{
		case EVENTWINDOW_STEP_RED:
			EWindowStepRed();
			break;

		case EVENTWINDOW_STEP_FRAMMOVE:
			EWindowStepFramMove();
			break;

		case EVENTWINDOW_STEP_TEXTLINE:
			EWindowStepTextLine();
			break;

		case EVENTWINDOW_STEP_OUT:
			EWindowStepOut();
			break;
		}

		for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x - g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y - g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x + g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y - g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x - g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y + g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x + g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y + g_E_Window.aUI[nCntWindow].fHeight, 0.0f);

			//頂点カラー
			pVtx[0].col = g_E_Window.aUI[nCntWindow].col;
			pVtx[1].col = g_E_Window.aUI[nCntWindow].col;
			pVtx[2].col = g_E_Window.aUI[nCntWindow].col;
			pVtx[3].col = g_E_Window.aUI[nCntWindow].col;

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(g_E_Window.aUI[nCntWindow].aTexU, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_E_Window.aUI[nCntWindow].aTexU + 1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_E_Window.aUI[nCntWindow].aTexU, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_E_Window.aUI[nCntWindow].aTexU + 1.0f, 1.0f);

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffE_Window->Unlock();

}

//=============================================================================
//イベントウィンドウのステップ(赤幕ウィンウィン)
//=============================================================================
void EWindowStepRed(void)
{
	static int nMove = 1;

	//不透明度更新
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a += (0.4f / (float)25) * nMove;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a > 0.4f)
	{//0.4f以上を超えたら

		g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a = 0.4f;
		nMove *= -1;
	}
	else if (g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a < 0.0f)
	{//0.0f未満になったら

		g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a = 0.0f;
		nMove *= -1;

		//回数加算
		g_E_Window.aUI[EVENTWINDOW_VTX_RED].nCount++;
	}

	
	if(g_E_Window.aUI[EVENTWINDOW_VTX_RED].nCount == 2)
	{//規定回数赤点滅したら

		//1.0f固定
		g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a = 0.0f;

		//ステップを次に
		g_E_Window.nStep = EVENTWINDOW_STEP_FRAMMOVE;
		PlaySound(SOUND_LABEL_SE_BACHIBACHI);
	}
}

//=============================================================================
//イベントウィンドウのステップ(枠移動)
//=============================================================================
void EWindowStepFramMove(void)
{
	//中央に移動させる
	g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x += (630.0f - g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x) * 0.20f;

	//不透明度更新
	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a < 0.4f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a += (0.4f / (float)15);

		//上のラインと色同期
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].col = g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col;
	}

	if (g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x <= 640.0f)
	{//真ん中に着いたら

		//位置固定
		g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x = 640.0f;

		//ステップを次に
		g_E_Window.nStep = EVENTWINDOW_STEP_TEXTLINE;
	}
}

//=============================================================================
//イベントウィンドウのステップ(テキスト&ライン移動)
//=============================================================================
void EWindowStepTextLine(void)
{
	static int nMove = 1;

	//中央に移動させる(最後はゆっくり)
	g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x += (660.0f - g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x) * 0.1f;

	//不透明度更新
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a += (0.2f / (float)15) * nMove;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a > 1.0f)
	{//1.0f以上を超えたら

		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a = 1.0f;
		nMove *= -1;

		//回数加算
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].nCount++;
	}
	else if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a < 0.4f)
	{//0.4f未満になったら

		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a = 0.4f;
		nMove *= -1;
	}

	//上のラインと色同期
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].col = g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].nCount == 0)
	{
		//警備強化きたお
		PlaySound(SOUND_LABEL_SE_SECURITY);
	}

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].nCount == 3)
	{
		//ステップを次に
		g_E_Window.nStep = EVENTWINDOW_STEP_OUT;

		//イベント文章の設定
		SetManyMORS(0);
	}

	//ラインはスクロールさせる
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU += 0.001f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU -= 0.001f;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU >= 1.0f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU = 0.0f;
	}
	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU <= 0.0f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU = 1.0f;
	}

}

//=============================================================================
//イベントウィンドウのステップ(退場)
//=============================================================================
void EWindowStepOut(void)
{
	//中央に移動させる(最後はゆっくり)
	g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x += (-700.0f - g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x) * 0.1f;
	g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x += (-700.0f - g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x) * 0.1f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].pos.x += (-700.0f - g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].pos.x) * 0.1f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].pos.x += (-700.0f - g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].pos.x) * 0.1f;

	//ラインはスクロールさせる
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU += 0.001f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU -= 0.001f;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU >= 1.0f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU = 0.0f;
	}
	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU <= 0.0f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU = 1.0f;
	}

	if (g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x < -640.0f)
	{//みんな退場したら

		g_E_Window.bUse = false;
	}
}

//=============================================================================
//イベントウィンドウの描画処理
//=============================================================================
void DrawEventWindow(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffE_Window, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_E_Window.bUse == true)
	{//使用されていたら

		for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureE_Window[nCntWindow]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);
		}
	}
}

//=============================================================================
//イベントウィンドウの設定処理
//=============================================================================
void SetEventWindow(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffE_Window->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_E_Window.bUse == false)
	{//使用されていなかったら

		g_E_Window.bUse = true;

		for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
		{
			g_E_Window.aUI[nCntWindow].nCount = 0;
		}

		g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x = SCREEN_WIDTH + SCREEN_WIDTH;
		g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x = SCREEN_WIDTH + SCREEN_WIDTH;
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f);
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f);

		//色の設定
		g_E_Window.aUI[EVENTWINDOW_VTX_RED].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//テクスチャ座標の設定
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU = 0.0f;
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU = 1.0f;

		g_E_Window.nStep = EVENTWINDOW_STEP_RED;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffE_Window->Unlock();
}

//=============================================================================
//イベントウィンドウの情報取得
//=============================================================================
EVENTWINDOW *GetEventWindow(void)
{
	return &g_E_Window;
}