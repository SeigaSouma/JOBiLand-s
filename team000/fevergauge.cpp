//=============================================================================
//
// フィーバーゲージ処理 [fevergauge.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "fevergauge.h"
#include "input.h"
#include "debugproc.h"
#include "crowd.h"
#include "game.h"
#include "tutorial.h"
#include "player.h"
#include "buddy.h"
#include "debugproc.h"
#include "sound.h"
#include "cutin.h"
#include "mapshape.h"
#include "feverpush.h"

//マクロ定義
#define POS_X		(250.0f)	//ゲージの長さ
#define POS_Y		(80.0f)	//ゲージの長さ
#define DOWN_MOVE	(0.05f)	//減る速度
#define UP_POS		(4.0f)	//増える大きさ

//プロタイプ宣言
void DownFeverGauge(void);
void UpdateGameFeverGauge(void);
void UpdateTutorialFeverGauge(void);

//グローバル変数
const char *c_apFilenameFeverGauge[] =		//ファイル読み込み
{
	"data\\TEXTURE\\fever02.png",
	"data\\TEXTURE\\fever01.png",
	NULL,
};
LPDIRECT3DTEXTURE9 g_apTextureFeverGauge[(sizeof c_apFilenameFeverGauge) / sizeof(*c_apFilenameFeverGauge)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFeverGauge = NULL;
FeverGauge g_aFeverGauge;	//フィーバーゲージの情報

//==============================================================
//フィーバーゲージの初期化処理
//==============================================================
void InitFeverGauge(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameFeverGauge) / sizeof(*c_apFilenameFeverGauge); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameFeverGauge[nCntTex],
			&g_apTextureFeverGauge[nCntTex]);
	}

	//各種変数初期化
	g_aFeverGauge.bFever = false;	//フィーバーかどうか
	g_aFeverGauge.bUseButton = false;	//ボタン押したか

	for (int nCntFeverGauge = 0; nCntFeverGauge < FEVERGAUGE_VTX_MAX; nCntFeverGauge++)
	{
		g_aFeverGauge.aUI[nCntFeverGauge].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f);	//頂点座標
		g_aFeverGauge.aUI[nCntFeverGauge].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//頂点カラー
		g_aFeverGauge.aUI[nCntFeverGauge].fWidth = POS_X;		//幅
		g_aFeverGauge.aUI[nCntFeverGauge].fHeight = POS_Y;		//高さ
		g_aFeverGauge.aUI[nCntFeverGauge].nPatternAnim = 0;	//アニメーションパターンNo
		g_aFeverGauge.aUI[nCntFeverGauge].nCntAnim = 0;		//アニメーションカウンター
		g_aFeverGauge.aUI[nCntFeverGauge].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_aFeverGauge.aUI[nCntFeverGauge].fAngle = 0.0f;		//角度
		g_aFeverGauge.aUI[nCntFeverGauge].fLength = 0.0f;		//対角線の長さ
		g_aFeverGauge.aUI[nCntFeverGauge].nCount = 0;			//雑カウント
		g_aFeverGauge.aUI[nCntFeverGauge].bUse = false;			//使用しているか
		g_aFeverGauge.aUI[nCntFeverGauge].aTexU = 0.0f;		//テクスチャ座標U[スクロール用]
		g_aFeverGauge.aUI[nCntFeverGauge].aTexV = 0.0f;		//テクスチャ座標V[スクロール用]
	}
	g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);
	g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.4f);
	g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight = 0.0f;


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * FEVERGAUGE_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFeverGauge,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffFeverGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFeverGauge = 0; nCntFeverGauge < FEVERGAUGE_VTX_MAX; nCntFeverGauge++)
	{
		//頂点座標の設定
		if (nCntFeverGauge != FEVERGAUGE_VTX_GAUGE)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y - POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y - POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y + POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y + POS_Y, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y - POS_Y * 2.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y - POS_Y * 2.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[1].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[2].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[3].col = g_aFeverGauge.aUI[nCntFeverGauge].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffFeverGauge->Unlock();
}

//==============================================================
//フィーバーゲージの画面の終了処理
//==============================================================
void UninitFeverGauge(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameFeverGauge) / sizeof(*c_apFilenameFeverGauge); nCntTex++)
	{
		//テクスチャの破棄
		if (g_apTextureFeverGauge[nCntTex] != NULL)
		{
			g_apTextureFeverGauge[nCntTex]->Release();
			g_apTextureFeverGauge[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffFeverGauge != NULL)
	{
		g_pVtxBuffFeverGauge->Release();
		g_pVtxBuffFeverGauge = NULL;
	}

}

//==============================================================
//フィーバーゲージの画面の更新処理
//==============================================================
void UpdateFeverGauge(void)
{
	switch (GetMode())
	{
	case MODE_GAME:
		UpdateGameFeverGauge();
		break;

	case MODE_TUTORIAL:
		UpdateTutorialFeverGauge();
		break;
	}
}

//==============================================================
//フィーバーゲージのゲーム時更新処理
//==============================================================
void UpdateGameFeverGauge(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFeverGauge->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//群衆の情報取得
	Crowd *pCrowd = GetCrowd();

	//群衆の数の情報取得
	CrowdNum *pCrowdNum = GetCrowdNum();

	//フィーバープッシュの情報取得
	FeverPush *pFeverPush = GetFeverPush();

#if _DEBUG
	if (GetKeyboardPress(DIK_DOWN) == true)
	{//↓キーが押された

		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight += 1.0f;
	}

	if (GetKeyboardPress(DIK_UP) == true)
	{//↓キーが押された

		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight -= 1.0f;
	}
#endif

	if (g_aFeverGauge.bFever == true)
	{//フィーバー状態

		//ゲージ減少処理
		DownFeverGauge();
	}

	if ((g_aFeverGauge.bFever == true && g_aFeverGauge.bUseButton == false && GetKeyboardTrigger(DIK_SPACE)) ||
		(g_aFeverGauge.bFever == true && g_aFeverGauge.bUseButton == false && GetGamepadTrigger(BUTTON_Y, 0)))
	{//SPACEキーかYボタン押したとき

		//カットイン配置
		SetCutin(D3DXVECTOR3(-500.0f, 400.0f, 0.0f), 0, TEXT_Y_CUTIN, TEXT_X_CUTIN);

		//セットカットイン 文字
		SetCutin(D3DXVECTOR3(1000.0f, 300.0f, 0.0f), 1, TEXT_Y_TEXT, TEXT_X_TEXT);

		//ボタンを押した状態にする
		g_aFeverGauge.bUseButton = true;

		//フィーバープッシュを表示しない
		pFeverPush->bUse = false;

		//フィーバー状態にする
		pPlayer->nState = PLAYERSTATE_FEAVER;
		pPlayer->bFever = true;

		//抱き枕カバーをかぶせる
		for (int nCntParts = 0; nCntParts < pPlayer->nPartsNum; nCntParts++)
		{
			pPlayer->aModel[nCntParts].bUse = true;

			if (pPlayer->aModel[nCntParts].nFever == -1)
			{
				pPlayer->aModel[nCntParts].bUse = false;
			}
		}

		//ユニコーーーーン！！！
		PlaySound(SOUND_LABEL_SE_UC);

		//ずあーん
		PlaySound(SOUND_LABEL_SE_CUTIN);

		//フィーバー状態にする
		for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++, pBuddy++)
		{
			if (pBuddy->bUse == true)
			{
				pBuddy->bFever = true;

				//抱き枕カバーをかぶせる
				for (int nCntParts = 0; nCntParts < pBuddy->nPartsNum; nCntParts++)
				{
					pBuddy->aModel[nCntParts].bUse = true;

					if (pBuddy->aModel[nCntParts].nFever == -1)
					{
						pBuddy->aModel[nCntParts].bUse = false;
					}
				}
			}
		}

		for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pCrowd++)
		{
			if (pCrowd->aModel.bUse == true)
			{
				pCrowd->bFever = true;
			}
		}
	}

	if (pCrowdNum->nNum > pCrowdNum->nNumOld && g_aFeverGauge.bFever == false)
	{//群衆の数が増えた時

		//ゲージ増加
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight += UP_POS;
	}

	if (g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight >= POS_Y)
	{//規定値超えたら固定

		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight = POS_Y;
		g_aFeverGauge.bFever = true;

		if (g_aFeverGauge.bUseButton == false)
		{//ボタン押してなかったら

			//フィーバープッシュを表示する
			pFeverPush->bUse = true;
		}
	}

	if (g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight <= 0.0f)
	{//規定値超えたら固定

		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight = 0.0f;
	}

	for (int nCntFeverGauge = 0; nCntFeverGauge < FEVERGAUGE_VTX_MAX; nCntFeverGauge++)
	{
		//頂点座標の設定
		if (nCntFeverGauge != FEVERGAUGE_VTX_GAUGE)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y - POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y - POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y + POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y + POS_Y, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y - g_aFeverGauge.aUI[nCntFeverGauge].fHeight * 2.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y - g_aFeverGauge.aUI[nCntFeverGauge].fHeight * 2.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y, 0.0f);
		}

		//頂点カラー
		pVtx[0].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[1].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[2].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[3].col = g_aFeverGauge.aUI[nCntFeverGauge].col;

		pVtx += 4;
	}


	//頂点バッファをアンロックする
	g_pVtxBuffFeverGauge->Unlock();
}

//==============================================================
//フィーバーゲージのチュートリアル時更新処理
//==============================================================
void UpdateTutorialFeverGauge(void)
{

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffFeverGauge->Lock(0, 0, (void**)&pVtx, 0);

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//群衆の情報取得
	Crowd *pCrowd = GetCrowd();

	//群衆の数の情報取得
	CrowdNum *pCrowdNum = GetCrowdNum();

	//フィーバープッシュの情報取得
	FeverPush *pFeverPush = GetFeverPush();

	if (pTutorial->nStep == TUTORIAL_STEP_USEFEVER && g_aFeverGauge.bFever == false)
	{//フィーバーを使うステップの時 && まだ溜まってないとき

		//ゲージ増加
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight += UP_POS;
	}

	if (pTutorial->nStep == TUTORIAL_STEP_MORS && g_aFeverGauge.bFever == true)
	{//フィーバーを使うステップの時 && まだ溜まってないとき

		//ゲージ増加
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight -= UP_POS;
	}


	if (g_aFeverGauge.bFever == true)
	{//フィーバー状態

		//ゲージ減少処理
		DownFeverGauge();
	}

	if ((g_aFeverGauge.bFever == true && g_aFeverGauge.bUseButton == false && GetKeyboardTrigger(DIK_SPACE)) ||
		(g_aFeverGauge.bFever == true && g_aFeverGauge.bUseButton == false && GetGamepadTrigger(BUTTON_Y, 0)))
	{//SPACEキーかYボタン押したとき

		//カットイン配置
		SetCutin(D3DXVECTOR3(-500.0f, 400.0f, 0.0f), 0, TEXT_Y_CUTIN, TEXT_X_CUTIN);

		//セットカットイン 文字
		SetCutin(D3DXVECTOR3(1000.0f, 300.0f, 0.0f), 1, TEXT_Y_TEXT, TEXT_X_TEXT);

		//ボタンを押した状態にする
		g_aFeverGauge.bUseButton = true;

		//フィーバープッシュを表示しない
		pFeverPush->bUse = false;

		//フィーバー状態にする
		pPlayer->nState = PLAYERSTATE_FEAVER;
		pPlayer->bFever = true;

		//フィーバーを使った状態にする
		pTutorial->aControl.bFeverUse = true;

		//抱き枕カバーをかぶせる
		for (int nCntParts = 0; nCntParts < pPlayer->nPartsNum; nCntParts++)
		{
			pPlayer->aModel[nCntParts].bUse = true;

			if (pPlayer->aModel[nCntParts].nFever == -1)
			{
				pPlayer->aModel[nCntParts].bUse = false;
			}
		}

		//ユニコーーーーン！！！
		PlaySound(SOUND_LABEL_SE_UC);

		//ずあーん
		PlaySound(SOUND_LABEL_SE_CUTIN);

		//フィーバー状態にする
		for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++, pBuddy++)
		{
			if (pBuddy->bUse == true)
			{
				pBuddy->bFever = true;

				//抱き枕カバーをかぶせる
				for (int nCntParts = 0; nCntParts < pBuddy->nPartsNum; nCntParts++)
				{
					pBuddy->aModel[nCntParts].bUse = true;

					if (pBuddy->aModel[nCntParts].nFever == -1)
					{
						pBuddy->aModel[nCntParts].bUse = false;
					}
				}
			}
		}

		for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pCrowd++)
		{
			if (pCrowd->aModel.bUse == true)
			{
				pCrowd->bFever = true;
			}
		}
	}

	

	if (g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight >= POS_Y)
	{//規定値超えたら固定

		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight = POS_Y;
		g_aFeverGauge.bFever = true;

		if (g_aFeverGauge.bUseButton == false)
		{//ボタン押してなかったら

			//フィーバープッシュを表示する
			pFeverPush->bUse = true;
		}
	}

	if (g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight <= 0.0f)
	{//規定値超えたら固定

		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight = 0.0f;
	}

	for (int nCntFeverGauge = 0; nCntFeverGauge < FEVERGAUGE_VTX_MAX; nCntFeverGauge++)
	{
		//頂点座標の設定
		if (nCntFeverGauge != FEVERGAUGE_VTX_GAUGE)
		{
			pVtx[0].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y - POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y - POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y + POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, g_aFeverGauge.aUI[nCntFeverGauge].pos.y + POS_Y, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y - g_aFeverGauge.aUI[nCntFeverGauge].fHeight * 2.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y - g_aFeverGauge.aUI[nCntFeverGauge].fHeight * 2.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x - POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aFeverGauge.aUI[nCntFeverGauge].pos.x + POS_X, POS_Y + g_aFeverGauge.aUI[nCntFeverGauge].pos.y, 0.0f);
		}

		//頂点カラー
		pVtx[0].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[1].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[2].col = g_aFeverGauge.aUI[nCntFeverGauge].col;
		pVtx[3].col = g_aFeverGauge.aUI[nCntFeverGauge].col;

		pVtx += 4;
	}


	//頂点バッファをアンロックする
	g_pVtxBuffFeverGauge->Unlock();
}

//==============================================================
//フィーバーゲージの減少処理
//==============================================================
void DownFeverGauge(void)
{

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//群衆の情報取得
	Crowd *pCrowd = GetCrowd();

	static int nMove = -1;

	//点滅処理
	g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.r += 0.08f * nMove;

	//全色同期
	g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.g = g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.b = g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.r;

	//フィーバー時点滅
	if (g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.r >= 1.0f)
	{
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.r = 1.0f;
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.g = 1.0f;
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.b = 1.0f;
		nMove *= -1;
	}
	if (g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.r <= 0.4f)
	{
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.r = 0.4f;
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.g = 0.4f;
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col.b = 0.4f;
		nMove *= -1;
	}

	if ((GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_END) || GetMode() == MODE_GAME)
	{
		if (g_aFeverGauge.bUseButton == true)
		{//ボタンを押したとき

			//減少させる
			g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight -= DOWN_MOVE;
		}
	}

	if (g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight <= 0.0f)
	{//規定値超えたら固定
	
		g_aFeverGauge.aUI[FEVERGAUGE_VTX_GAUGE].fHeight = 0.0f;
		g_aFeverGauge.bFever = false;

		for (int nCntParts = 0; nCntParts < pPlayer->nPartsNum; nCntParts++)
		{
			if (pPlayer->aModel[nCntParts].nFever == 0)
			{
				pPlayer->aModel[nCntParts].bUse = false;
			}
			
			if (pPlayer->aModel[nCntParts].nFever == -1)
			{
				pPlayer->aModel[nCntParts].bUse = true;
			}
		}
		
		for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++, pBuddy++)
		{
			if (pBuddy->bUse == true)
			{
				pBuddy->bFever = false;

				for (int nCntParts = 0; nCntParts < pBuddy->nPartsNum; nCntParts++)
				{
					if (pBuddy->aModel[nCntParts].nFever == 0)
					{
						pBuddy->aModel[nCntParts].bUse = false;
					}

					if (pBuddy->aModel[nCntParts].nFever == -1)
					{
						pBuddy->aModel[nCntParts].bUse = true;
					}
				}
			}
		}

		for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pCrowd++)
		{
			if (pCrowd->aModel.bUse == true)
			{
				pCrowd->bFever = false;
			}
		}

		//ボタンを押してない状態にする
		g_aFeverGauge.bUseButton = false;

		g_aFeverGauge.aUI[FEVERGAUGE_VTX_TEX].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

		//フィーバー状態解除
		pPlayer->nState = PLAYERSTATE_NONE;
		pPlayer->bFever = false;

		//サウンドの停止
		StopSound(SOUND_LABEL_BGM_FEVER);
	}
}

//==============================================================
//フィーバーゲージの画面の描画処理
//==============================================================
void DrawFeverGauge(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffFeverGauge, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntFeverGauge = 0; nCntFeverGauge < FEVERGAUGE_VTX_MAX; nCntFeverGauge++)
	{
		switch (nCntFeverGauge)
		{
		case FEVERGAUGE_VTX_FRAM:

			//アルファテストを有効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 250);

			//αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;

		case FEVERGAUGE_VTX_TEX:

			//Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
			break;

		case FEVERGAUGE_VTX_GAUGE:

			//Zテストを無効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
			break;
		}

		//テクスチャセット
		pDevice->SetTexture(0, g_apTextureFeverGauge[nCntFeverGauge]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFeverGauge * 4, 2);	//プリミティブの種類

		//αブレンディングを元に戻す
		pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		//アルファテストを無効にする
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

		//Zテストを有効にする
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//==============================================================
//フィーバーゲージの情報
//==============================================================
FeverGauge *GetFeverGauge(void)
{
	return &g_aFeverGauge;
}