//=============================================================================
//
// 最初の会話処理 [start_talk.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "start_talk.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
#include "updown_fram.h"
#include "enemy.h"

//マクロ定義
#define MAX_TOP			(4)		//頂点数
#define WIDTH		(470.0f)
#define HEIGHT		(110.0f)
#define WINDOW_WIDTH	(350.0f)
#define WINDOW_HEIGHT	(150.0f)
#define SKIP_WIDTH	(160.0f * 1.2f)
#define SKIP_HEIGHT	(60.0f * 1.2f)
#define ALPHA_MOVE	(0.02f)

//プロトタイプ宣言
void CreatVtxTextStartTalk(void);
void CreatVtxWindowStartTalk(void);
void UpdateTextStartTalk(void);
void UpdateFadeStartTalk(void);
void UpdateSkipStartTalk(void);
void UpdateEndStartTalk(void);
void UpdateTargetWindow(void);
void DrawStartTalkBuff(void);
void DrawTargetWindowBuff(void);

//グローバル変数宣言
const char *c_apFilenameStartTalk[] =		//ファイル読み込み
{
	NULL,
	"data\\TEXTURE\\talk_011.png",
	"data\\TEXTURE\\talk_022.png",
	"data\\TEXTURE\\talk_033.png",
	"data\\TEXTURE\\talk_044.png",
	"data\\TEXTURE\\talk_055.png",
	"data\\TEXTURE\\talk_skip.png",
};

const char *c_apFilenameTargetWindow[] =		//ファイル読み込み
{
	"data\\TEXTURE\\targetwindow_01.png",
};
LPDIRECT3DTEXTURE9 g_pTextureStartTalk[(sizeof c_apFilenameStartTalk) / sizeof(*c_apFilenameStartTalk)] = {};			//テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureTargetWindow[(sizeof c_apFilenameTargetWindow) / sizeof(*c_apFilenameTargetWindow)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStartTalk = NULL;	//頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTargetWindow = NULL;	//頂点バッファへのポインタ
StartTalk g_aStartTalk[START_TALK_VTX_MAX];						//最初の会話の情報
int g_nStartTalkStep;	//ステップ
bool g_bEndStartTalk;	//終了状態かどうか
int g_nIdxMoveLogTargetWindow;	//動くUIのインデックス番号
bool g_bUseTargetWindow;	//使用しているか

//==================================================================================
//最初の会話の初期化処理
//==================================================================================
void InitStartTalk(void)
{

	//各変数初期化
	for (int nCntTalk = 0; nCntTalk < START_TALK_VTX_MAX; nCntTalk++)
	{
		g_aStartTalk[nCntTalk].pos = D3DXVECTOR3(640.0f, 500.0f, 0.0f);
		g_aStartTalk[nCntTalk].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		//色
		g_aStartTalk[nCntTalk].bUse = false;	//使用していない状態にする
	}
	g_aStartTalk[START_TALK_VTX_FADE].pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);		//幕の位置
	g_aStartTalk[START_TALK_VTX_FADE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		//色
	g_aStartTalk[START_TALK_VTX_SKIP].pos = D3DXVECTOR3(1100.0f, 650.0f, 0.0f);
	g_aStartTalk[START_TALK_VTX_SKIP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);		//色
	g_nStartTalkStep = -1;
	g_bEndStartTalk = false;	//終了状態かどうか

	//動くロゴのインデックス番号取得
	g_nIdxMoveLogTargetWindow = SetMoveLog();

	//ウィンドウの使用状況
	g_bUseTargetWindow = false;

	//会話の頂点バッファ生成
	CreatVtxTextStartTalk();

	//ウィンドウの頂点バッファ生成
	CreatVtxWindowStartTalk();

}

//==================================================================================
//最初の会話の終了処理
//==================================================================================
void UninitStartTalk(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameStartTalk) / sizeof(*c_apFilenameStartTalk); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureStartTalk[nCntTex] != NULL)
		{
			g_pTextureStartTalk[nCntTex]->Release();
			g_pTextureStartTalk[nCntTex] = NULL;
		}
	}

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTargetWindow) / sizeof(*c_apFilenameTargetWindow); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureTargetWindow[nCntTex] != NULL)
		{
			g_pTextureTargetWindow[nCntTex]->Release();
			g_pTextureTargetWindow[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffStartTalk != NULL)
	{
		g_pVtxBuffStartTalk->Release();
		g_pVtxBuffStartTalk = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTargetWindow != NULL)
	{
		g_pVtxBuffTargetWindow->Release();
		g_pVtxBuffTargetWindow = NULL;
	}
}

//==================================================================================
//最初の会話の更新処理
//==================================================================================
void UpdateStartTalk(void)
{
	if (GetGameState() == GAMESTATE_START)
	{
		if (g_bEndStartTalk == false && g_aStartTalk[START_TALK_VTX_TEXT1].bUse == true && (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0)))
		{//終了していない && 使用されている && EnterかA

			SetStartTalk();
		}

		if (g_bEndStartTalk == false && g_aStartTalk[START_TALK_VTX_TEXT1].bUse == true && (GetKeyboardTrigger(DIK_BACKSPACE) == true || GetGamepadTrigger(BUTTON_START, 0)))
		{
			g_bEndStartTalk = true;

			//枠の状態変更
			GetUpDownFram()->nState = UPDOWN_FRAM_STATE_OUT;
		}

		if (g_bEndStartTalk == true)
		{//終了していたら

			UpdateEndStartTalk();
		}
		else
		{
			//各要素の更新処理
			UpdateTextStartTalk();
			UpdateFadeStartTalk();
			UpdateSkipStartTalk();
		}

		//ウィンドウの更新処理
		UpdateTargetWindow();
	}
}

//==================================================================================
//最初の会話のテキスト更新処理
//==================================================================================
void UpdateTextStartTalk(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * START_TALK_VTX_TEXT1;

	for (int nCntTalk = START_TALK_VTX_TEXT1; nCntTalk < START_TALK_VTX_SKIP; nCntTalk++)
	{
		if (g_aStartTalk[nCntTalk].bUse == true)
		{//使用されていたら

			//不透明度の補正
			if (g_aStartTalk[nCntTalk].col.a < 1.0f)
			{
				g_aStartTalk[nCntTalk].col.a += 0.05f;
			}
			else
			{//1.0f以上で固定

				g_aStartTalk[nCntTalk].col.a = 1.0f;
			}

			//上に移動させる
			g_aStartTalk[nCntTalk].pos.y -= 15.0f;

			//流れる処理
			if ((400.0f + (nCntTalk - 1) * 240.0f) - 240.0f * g_nStartTalkStep > g_aStartTalk[nCntTalk].pos.y)
			{
				g_aStartTalk[nCntTalk].pos.y = (400.0f + (nCntTalk - 1) * 240.0f) - 240.0f * g_nStartTalkStep;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aStartTalk[nCntTalk].col;
			pVtx[1].col = g_aStartTalk[nCntTalk].col;
			pVtx[2].col = g_aStartTalk[nCntTalk].col;
			pVtx[3].col = g_aStartTalk[nCntTalk].col;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//最初の会話の幕更新処理
//==================================================================================
void UpdateFadeStartTalk(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aStartTalk[START_TALK_VTX_FADE].bUse == true)
	{//使用されていたら

		//不透明度の補正
		if (g_aStartTalk[START_TALK_VTX_FADE].col.a < 0.3f)
		{
			g_aStartTalk[START_TALK_VTX_FADE].col.a += 0.025f;
		}
		else
		{//0.3f以上で固定

			g_aStartTalk[START_TALK_VTX_FADE].col.a = 0.3f;

			if (g_aStartTalk[START_TALK_VTX_TEXT1].bUse == false)
			{
				//会話の設定
				SetStartTalk();

				g_aStartTalk[START_TALK_VTX_SKIP].bUse = true;
			}
		}

		//頂点カラーの設定
		pVtx[0].col = g_aStartTalk[START_TALK_VTX_FADE].col;
		pVtx[1].col = g_aStartTalk[START_TALK_VTX_FADE].col;
		pVtx[2].col = g_aStartTalk[START_TALK_VTX_FADE].col;
		pVtx[3].col = g_aStartTalk[START_TALK_VTX_FADE].col;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//最初の会話のスキップ更新処理
//==================================================================================
void UpdateSkipStartTalk(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aStartTalk[START_TALK_VTX_SKIP].bUse == true)
	{//使用されていたら

		static float fMoveAlpha = 0.01f;

		if (g_aStartTalk[START_TALK_VTX_SKIP].col.a > 1.0f)
		{//不透明度100%

			g_aStartTalk[START_TALK_VTX_SKIP].col.a = 1.0f;
			fMoveAlpha *= -1;
		}
		else if (g_aStartTalk[START_TALK_VTX_SKIP].col.a < 0.4f)
		{//不透明度20%

			g_aStartTalk[START_TALK_VTX_SKIP].col.a = 0.4f;
			fMoveAlpha *= -1;
		}

		//不透明度の更新
		g_aStartTalk[START_TALK_VTX_SKIP].col.a += fMoveAlpha;

		//頂点カラーの設定
		pVtx[START_TALK_VTX_SKIP * 4 + 0].col = g_aStartTalk[START_TALK_VTX_SKIP].col;
		pVtx[START_TALK_VTX_SKIP * 4 + 1].col = g_aStartTalk[START_TALK_VTX_SKIP].col;
		pVtx[START_TALK_VTX_SKIP * 4 + 2].col = g_aStartTalk[START_TALK_VTX_SKIP].col;
		pVtx[START_TALK_VTX_SKIP * 4 + 3].col = g_aStartTalk[START_TALK_VTX_SKIP].col;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//最初の会話の終了(フェード)処理
//==================================================================================
void UpdateEndStartTalk(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	int nCntUse = 0;
	for (int nCntTalk = 0; nCntTalk < START_TALK_VTX_MAX; nCntTalk++)
	{
		if (g_aStartTalk[nCntTalk].bUse == true)
		{//使用されていたら

			//仕様カウント加算
			nCntUse++;

			//不透明度の更新
			g_aStartTalk[nCntTalk].col.a -= 0.025f;

			//不透明度の補正
			if (g_aStartTalk[nCntTalk].col.a < 0.0f)
			{
				g_aStartTalk[nCntTalk].col.a = 0.0f;
				g_aStartTalk[nCntTalk].bUse = false;
			}

			//頂点カラーの設定
			pVtx[0].col = g_aStartTalk[nCntTalk].col;
			pVtx[1].col = g_aStartTalk[nCntTalk].col;
			pVtx[2].col = g_aStartTalk[nCntTalk].col;
			pVtx[3].col = g_aStartTalk[nCntTalk].col;
		}

		pVtx += 4;
	}

	if (nCntUse == 0 && g_bUseTargetWindow == false)
	{//全部使っていなかったら

		//ウィンドウの設定処理
		SetTartgetWindow();
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//目標枠の更新処理
//==================================================================================
void UpdateTargetWindow(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTargetWindow->Lock(0, 0, (void**)&pVtx, 0);

	if (g_bUseTargetWindow == true)
	{//使用されていたら

		//大きくなる→縮む→大きくなって規定値
		float fWidth = MoveLog(g_nIdxMoveLogTargetWindow, WINDOW_WIDTH, 15);
		float fHeight = MoveLog(g_nIdxMoveLogTargetWindow, WINDOW_HEIGHT, 15);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(640.0f - fWidth, 360.0f - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(640.0f + fWidth, 360.0f - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(640.0f - fWidth, 360.0f + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(640.0f + fWidth, 360.0f + fHeight, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0) ||
			GetKeyboardTrigger(DIK_BACKSPACE) == true || GetGamepadTrigger(BUTTON_START, 0))
		{//終了していない && 使用されている && EnterかA

			g_bUseTargetWindow = false;
			SetGameState(GAMESTATE_NONE, 0);
			ReadSetEnemy();
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTargetWindow->Unlock();
}

//==================================================================================
//最初の会話の描画処理
//==================================================================================
void DrawStartTalk(void)
{
	//テキスト描画処理
	DrawStartTalkBuff();

	//ウィンドウ描画処理
	DrawTargetWindowBuff();
}

//==================================================================================
//最初の会話のテキスト描画処理
//==================================================================================
void DrawStartTalkBuff(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffStartTalk, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テキストの描画
	for (int nCntTalk = 0; nCntTalk < START_TALK_VTX_MAX; nCntTalk++)
	{
		if (g_aStartTalk[nCntTalk].bUse == true)
		{//使用されていたら

		 //テクスチャの設定
			pDevice->SetTexture(0, g_pTextureStartTalk[nCntTalk]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTalk * 4, 2);
		}
	}
}

//==================================================================================
//最初の会話のウィンドウ描画処理
//==================================================================================
void DrawTargetWindowBuff(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTargetWindow, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//ウィンドウの描画
	if (g_bUseTargetWindow == true)
	{//使用されていたら

		 //テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTargetWindow[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//==================================================================================
//最初の会話設定処理
//==================================================================================
void SetStartTalk(void)
{

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4;

	int nCntUse = 0;
	for (int nCntTalk = START_TALK_VTX_TEXT1; nCntTalk < START_TALK_VTX_SKIP; nCntTalk++)
	{
		if (g_aStartTalk[nCntTalk].bUse == false)
		{//使用していなかったら

			//使用カウント加算
			nCntUse++;

			g_aStartTalk[nCntTalk].pos.y = 640.0f;
			g_aStartTalk[nCntTalk].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);		//色
			g_aStartTalk[nCntTalk].bUse = true;
			g_aStartTalk[START_TALK_VTX_FADE].bUse = true;

			if (nCntTalk == START_TALK_VTX_TEXT1 || nCntTalk == START_TALK_VTX_TEXT5)
			{
				PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
			}

			if (nCntTalk == START_TALK_VTX_TEXT3)
			{
				PlaySound(SOUND_LABEL_SE_DHUFU);
			}

			//ステップ加算
			g_nStartTalkStep++;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
			break;
		}

		pVtx += 4;
	}

	if (nCntUse == 0)
	{//全て使用していたら

		//終了判定
		g_bEndStartTalk = true;

		//枠の状態変更
		GetUpDownFram()->nState = UPDOWN_FRAM_STATE_OUT;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//最初の会話目標ウィンドウ表示
//==================================================================================
void SetTartgetWindow(void)
{
	if (g_bUseTargetWindow == false)
	{
		g_bUseTargetWindow = true;
	}
}

//==================================================================================
//テキストの頂点バッファ生成
//==================================================================================
void CreatVtxTextStartTalk(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameStartTalk) / sizeof(*c_apFilenameStartTalk); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameStartTalk[nCntTex],
			&g_pTextureStartTalk[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * START_TALK_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStartTalk,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTalk = 0; nCntTalk < START_TALK_VTX_MAX; nCntTalk++)
	{
		if (nCntTalk == START_TALK_VTX_FADE)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - 640.0f, g_aStartTalk[nCntTalk].pos.y - 360.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + 640.0f, g_aStartTalk[nCntTalk].pos.y - 360.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - 640.0f, g_aStartTalk[nCntTalk].pos.y + 360.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + 640.0f, g_aStartTalk[nCntTalk].pos.y + 360.0f, 0.0f);
		}
		else if (nCntTalk == START_TALK_VTX_SKIP)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - SKIP_WIDTH, g_aStartTalk[nCntTalk].pos.y - SKIP_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + SKIP_WIDTH, g_aStartTalk[nCntTalk].pos.y - SKIP_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - SKIP_WIDTH, g_aStartTalk[nCntTalk].pos.y + SKIP_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + SKIP_WIDTH, g_aStartTalk[nCntTalk].pos.y + SKIP_HEIGHT, 0.0f);
		}
		else
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aStartTalk[nCntTalk].col;
		pVtx[1].col = g_aStartTalk[nCntTalk].col;
		pVtx[2].col = g_aStartTalk[nCntTalk].col;
		pVtx[3].col = g_aStartTalk[nCntTalk].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//ウィンドウの頂点バッファ生成
//==================================================================================
void CreatVtxWindowStartTalk(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTargetWindow) / sizeof(*c_apFilenameTargetWindow); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTargetWindow[nCntTex],
			&g_pTextureTargetWindow[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTargetWindow,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTargetWindow->Lock(0, 0, (void**)&pVtx, 0);
	
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(640.0f - WINDOW_WIDTH, 360.0f - WINDOW_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(640.0f + WINDOW_WIDTH, 360.0f - WINDOW_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(640.0f - WINDOW_WIDTH, 360.0f + WINDOW_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(640.0f + WINDOW_WIDTH, 360.0f + WINDOW_HEIGHT, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffTargetWindow->Unlock();
}

//==================================================================================
//最初の会話情報取得
//==================================================================================
StartTalk *GetStartTalk(void)
{
	return &g_aStartTalk[0];
}