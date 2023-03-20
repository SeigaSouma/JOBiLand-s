//=============================================================================
//
// タイトル処理 [title.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "title.h"
#include "edit.h"
#include "fade.h"
#include "game.h"
#include "light.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "particle.h"
#include "effect.h"
#include "2D_effect.h"
#include "player.h"
#include "buddy.h"
#include "sound.h"
#include "titleselect.h"

#define MIN_ALPHA	(0.4f)		//不透明度最小値
#define MAX_ALPHA	(1.0f)		//不透明度最大値
#define CHANGE_TIME	(60 * 25)	//切り替えタイマー
#define MAX_TKTK	(25)		//チカチカの最大数
#define DISTANCE_X	(500.0f)	//間隔
#define POS_X		(180.0f)	//相対X
#define POS_Y		(100.0f)	//相対Y
#define LOG_WIDTH	(400.0f)
#define LOG_HEIGHT	(180.0f)
#define START_POSX	(390.0f)
#define MOVE_TIME	(20)

//プロトタイプ宣言
void UpdateMoveLog(void);

//グローバル変数宣言
const char *c_apFilenameTitle[] =					//ファイル読み込み
{
	NULL,
	"data\\TEXTURE\\titlelog_01.png",
	"data\\TEXTURE\\titlelog_02.png",
	"data\\TEXTURE\\titlelog_03.png",
	"data\\TEXTURE\\titlelog_04.png",
	"data\\TEXTURE\\titlelog_00.png",
};
LPDIRECT3DTEXTURE9 g_pTextureTitle[(sizeof c_apFilenameTitle) / sizeof(*c_apFilenameTitle)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;		//頂点バッファへのポインタ
bool g_bEndFrag;									//終了のフラグ
Title g_Title;

//==================================================================================
//タイトルの初期化処理
//==================================================================================
void InitTitle(void)
{
	g_Title.nTKTKFrame = 0;					//チカチカのフレームカウント
	g_Title.nTKTKCounter = 0;				//チカチカの回数
	g_Title.OldSelect = TITLESELECT_START;	//前回の選択肢
	g_Title.nSelect = TITLESELECT_START;	//今回の選択肢
	g_Title.nCntChange = 0;
	g_Title.nCntMoveLog = 0;
	g_Title.fVolume = 1.0f;
	g_Title.bMoveLog = false;
	g_Title.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntLog = 0; nCntLog < TITLELOG_MAX; nCntLog++)
	{
		g_Title.Logcol[nCntLog] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	g_Title.Logcol[TITLELOG_LINE] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Title.Logcol[TITLELOG_BLACK] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_Title.pos = D3DXVECTOR3(START_POSX, 580.0f, 0.0f);
	g_Title.posLog[TITLELOG_BLACK] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	g_Title.posLog[TITLELOG_LOG] = D3DXVECTOR3(640.0f, SCREEN_HEIGHT + 360.0f, 0.0f);
	g_Title.posLog[TITLELOG_LINE] = D3DXVECTOR3(-640.0f, 360.0f, 0.0f);
	g_Title.posLog[TITLELOG_LIBELLION] = D3DXVECTOR3(SCREEN_WIDTH + 640.0f, 360.0f, 0.0f);
	g_Title.posLog[TITLELOG_HONKI] = D3DXVECTOR3(640.0f, -360.0f, 0.0f);

	g_Title.posLog[TITLELOG_TITLELOGO] = D3DXVECTOR3(640.0f, -360.0f, 0.0f);
	g_Title.Logcol[TITLELOG_TITLELOGO] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Title.nType = 0;

	g_Title.nState = TITLESTATE_MOVELOG;	//状態
	g_bEndFrag = false;

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTitle) / sizeof(*c_apFilenameTitle); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTitle[nCntTex],
			&g_pTextureTitle[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLEVTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCount = 0; nCount < TITLEVTX_MAX; nCount++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Title.posLog[nCount].x - LOG_WIDTH, g_Title.posLog[nCount].y - LOG_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Title.posLog[nCount].x + LOG_WIDTH, g_Title.posLog[nCount].y - LOG_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Title.posLog[nCount].x - LOG_WIDTH, g_Title.posLog[nCount].y + LOG_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Title.posLog[nCount].x + LOG_WIDTH, g_Title.posLog[nCount].y + LOG_HEIGHT, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_Title.Logcol[nCount];
		pVtx[1].col = g_Title.Logcol[nCount];
		pVtx[2].col = g_Title.Logcol[nCount];
		pVtx[3].col = g_Title.Logcol[nCount];
	

		//rhwの設定
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	//影の初期化処理
	InitShadow();

	//モデルの初期化処理
	InitModel();

	//エディットの初期化
	InitEdit();

	//タイトルの選択肢初期化処理
	InitTitleSelect();

	//プレイヤーの初期化処理
	InitPlayer();

	//相棒の初期化処理
	InitBuddy();

	//メッシュフィールドの初期化処理
	InitMeshField();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	//メッシュドームの初期化処理
	InitMeshDome();

	//エフェクトの初期化処理
	InitEffect();

	//2Dエフェクトの初期化処理
	InitEffect_2D();

	//パーティクルの初期化処理
	InitParticle();

	//タイトルのカメラ初期化処理
	InitTitleCamera();

	//BGMをセット
	PlaySound(SOUND_LABEL_BGM_TITLE);
}

//==================================================================================
//タイトルの終了処理
//==================================================================================
void UninitTitle(void)
{
	StopSound();

	//モデルの終了処理
	UninitModel();

	//エディットの終了処理
	UninitEdit();

	//タイトルの選択肢終了処理
	UninitTitleSelect();

	//プレイヤーの終了処理
	UninitPlayer();

	//相棒の終了処理
	UninitBuddy();

	//影の終了処理
	UninitShadow();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	//メッシュウォールの終了処理
	UninitMeshDome();

	//エフェクトの終了処理
	UninitEffect();

	//2Dエフェクトの終了処理
	UninitEffect_2D();

	//パーティクルの終了処理
	UninitParticle();

	for (int nCount = 0; nCount < (sizeof c_apFilenameTitle) / sizeof(*c_apFilenameTitle); nCount++)
	{
		if (g_pTextureTitle[nCount] != NULL)
		{
			g_pTextureTitle[nCount]->Release();
			g_pTextureTitle[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

}

//==================================================================================
//タイトルの更新処理
//==================================================================================
void UpdateTitle(void)
{

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (GetFade() == FADE_NONE && g_Title.nState == TITLESTATE_MOVELOG)
	{//何もしていないとき

		//ロゴの移動処理処理
		UpdateMoveLog();

	}
	else if (GetFade() == FADE_NONE && g_Title.nState == TITLESTATE_NONE)
	{//何もしていないとき

		//自動遷移カウンター加算
		g_Title.nCntChange++;

		//選択処理
		UpdateSelectTitle();

	}
	else if (g_Title.nState == TITLESTATE_TKTK)
	{//チカチカ状態のとき

		//選択肢のチカチカ処理
		UpdateTitleTKTK();
	}
	else if (g_Title.nState == TITLESTATE_FADE)
	{//遷移状態のとき

		switch (g_Title.nSelect)
		{

		case TITLESELECT_START:
			SetFade(MODE_TUTORIAL);
			break;

		case TITLESELECT_END:
			g_bEndFrag = true;	//終了のフラグON
			break;

		}

	}

	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();

	//カメラの更新処理
	UpdateCamera();

	//モデルの更新処理
	UpdateModel();

	//エディットの更新処理
	UpdateEdit();

	//タイトルの選択肢更新処理
	UpdateTitleSelect();

	//プレイヤーの更新処理
	UpdateTitlePlayer();

	//相棒の更新処理
	UpdateTitleBuddy();

	//影の更新処理
	UpdateShadow();

	//エフェクトの更新処理
	UpdateEffect();

	//2Dエフェクトの更新処理
	UpdateEffect_2D();

	//パーティクルの更新処理
	UpdateParticle();

	if (g_Title.nCntChange >= CHANGE_TIME)
	{

		SetFade(MODE_RANKING);
	}
}

//==================================================================================
//タイトルの描画処理
//==================================================================================
void DrawTitle(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{
		//メッシュフィールドの描画処理
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュウォールの描画処理
		DrawMeshDome();

		//影の描画処理
		DrawShadow();

		//モデルの描画処理
		DrawModel(0);

		//エディットの描画処理
		DrawEdit();

		//プレイヤーの描画処理
		DrawPlayer();

		//相棒の描画処理
		DrawBuddy();

		//選択肢の描画
		DrawTitleSelect();

		//エフェクトの描画処理
		DrawEffect();

		//パーティクルの描画処理
		DrawParticle();
	}

	if (nType == DRAWTYPE_UI)
	{
		//デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureTitle[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


		//2Dエフェクトの描画処理
		DrawEffect_2D();


		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCount = 1; nCount < TITLEVTX_MAX; nCount++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureTitle[nCount]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//==================================================================================
//ロゴの移動処理
//==================================================================================
void UpdateMoveLog(void)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//カメラの更新処理
	UpdateTitleCamera(CAMERATYPE_MAIN);

	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Title.nCntMoveLog <= 30 && g_Title.nType == 0)
	{//まだ規定値に達していないとき

		//ロゴの動く時間加算
		g_Title.nCntMoveLog++;
	}
	else if (g_Title.nCntMoveLog > 20 && g_Title.nType == 0)
	{
		g_Title.nType = 1;
		g_Title.nCntMoveLog = 0;
	}

	if (g_Title.nCntMoveLog <= 60 && g_Title.nType == 1)
	{//まだ規定値に達していないとき

		//ロゴの動く時間加算
		g_Title.nCntMoveLog++;

		g_Title.posLog[TITLELOG_LINE].x += SCREEN_WIDTH / 60;

		//エフェクトの設定
		SetEffect_2D(D3DXVECTOR3(g_Title.posLog[TITLELOG_LINE].x + 600.0f, g_Title.posLog[TITLELOG_LINE].y + 60.0f, g_Title.posLog[TITLELOG_LINE].z),
			D3DXCOLOR(0.1f, 0.5f, 0.9f, 1.0f),
			40.0f,
			10,
			D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
			MOVEEFFECT_NONE, EFFECTTYPE_NORMAL);

		SetEffect_2D(D3DXVECTOR3(g_Title.posLog[TITLELOG_LINE].x + 600.0f, g_Title.posLog[TITLELOG_LINE].y + 60.0f, g_Title.posLog[TITLELOG_LINE].z),
			D3DXCOLOR(0.1f, 0.5f, 0.9f, 1.0f),
			40.0f,
			10,
			D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
			MOVEEFFECT_NONE, EFFECTTYPE_NORMAL);
	}
	else if (g_Title.nCntMoveLog > 60 && g_Title.nType == 1)
	{
		g_Title.nType = 2;
		g_Title.nCntMoveLog = 0;
	}

	if (g_Title.nCntMoveLog < MOVE_TIME && g_Title.nType == 2)
	{//まだ規定値に達していないとき

		//ロゴの動く時間加算
		g_Title.nCntMoveLog++;

		//不透明度加算
		g_Title.Logcol[TITLELOG_BLACK].a += 0.05f;

		g_Title.posLog[TITLELOG_LOG].y -= SCREEN_HEIGHT / MOVE_TIME;
		g_Title.posLog[TITLELOG_HONKI].y += SCREEN_HEIGHT / MOVE_TIME;
		g_Title.posLog[TITLELOG_LIBELLION].x -= SCREEN_WIDTH / MOVE_TIME;

		g_Title.Logcol[TITLELOG_LOG].a += 1.0f / (float)MOVE_TIME;
		g_Title.Logcol[TITLELOG_HONKI].a += 1.0f / (float)MOVE_TIME;
		g_Title.Logcol[TITLELOG_LIBELLION].a += 1.0f / (float)MOVE_TIME;

		if (g_Title.Logcol[TITLELOG_LOG].a >= 1.0f)
		{
			g_Title.Logcol[TITLELOG_LOG].a = 1.0f;
			g_Title.Logcol[TITLELOG_HONKI].a = 1.0f;
			g_Title.Logcol[TITLELOG_LIBELLION].a = 1.0f;
		}

		if (g_Title.Logcol[TITLELOG_BLACK].a >= 1.0f)
		{
			g_Title.Logcol[TITLELOG_BLACK].a = 1.0f;
		}

		if (g_Title.nCntMoveLog == 12)
		{
			SetParticle(D3DXVECTOR3(640.0f, 360.0f, 0.0f), PARTICLE_TYPE_TITLELOG);
		}

	}
	else if (g_Title.nCntMoveLog >= MOVE_TIME && g_Title.nType == 2)
	{
		pCamera->nMoveStep = 1;
		g_Title.nCntMoveLog++;

		//タイトルの選択肢セット
		SetTitleSelect(D3DXVECTOR3(-100.0f, 0.0f, -300.0f), 25);
		SetTitleSelect(D3DXVECTOR3(100.0f, 0.0f, -300.0f), 26);

		if (g_Title.nCntMoveLog >= 150)
		{
			g_Title.nType = 3;
			g_Title.nCntMoveLog = 0;

			pPlayer->bUse = true;
			pPlayer->nIdxShadow = SetShadow(pPlayer->fRadius * 0.5f, pPlayer->fRadius * 0.5f);
			SetPositionShadow(pPlayer->nIdxShadow, pPlayer->pos);
			pBuddy[BUDDYTYPE_HYOROGARI].bUse = true;
			pBuddy[BUDDYTYPE_DEPPA].bUse = true;

			pBuddy[BUDDYTYPE_HYOROGARI].nIdxShadow = SetShadow(pBuddy[BUDDYTYPE_HYOROGARI].fRadius * 0.5f, pBuddy[BUDDYTYPE_HYOROGARI].fRadius * 0.5f);
			SetPositionShadow(pBuddy[BUDDYTYPE_HYOROGARI].nIdxShadow, pBuddy[BUDDYTYPE_HYOROGARI].pos);

			pBuddy[BUDDYTYPE_DEPPA].nIdxShadow = SetShadow(pBuddy[BUDDYTYPE_DEPPA].fRadius * 0.5f, pBuddy[BUDDYTYPE_DEPPA].fRadius * 0.5f);
			SetPositionShadow(pBuddy[BUDDYTYPE_DEPPA].nIdxShadow, pBuddy[BUDDYTYPE_DEPPA].pos);
		}
	}

	int nMoveTime = 60;
	if (g_Title.nCntMoveLog <= nMoveTime && g_Title.nType == 3)
	{//まだ規定値に達していないとき

		//消していく
		g_Title.Logcol[TITLEVTX_BLACK].a -= 1.0f / (float)nMoveTime;
		g_Title.Logcol[TITLEVTX_LOG].a -= 1.0f / (float)nMoveTime;
		g_Title.Logcol[TITLELOG_LINE].a -= 1.0f / (float)nMoveTime;
		g_Title.Logcol[TITLEVTX_LIBELLION].a -= 1.0f / (float)nMoveTime;
		g_Title.Logcol[TITLEVTX_HONKI].a -= 1.0f / (float)nMoveTime;

		g_Title.posLog[TITLELOG_TITLELOGO].y += (360.0f + 130.0f) / (float)nMoveTime;

		if (g_Title.Logcol[TITLEVTX_BLACK].a <= 0.0f)
		{
			g_Title.Logcol[TITLEVTX_BLACK].a = 0.0f;
			g_Title.Logcol[TITLEVTX_LOG].a = 0.0f;
			g_Title.Logcol[TITLELOG_LINE].a = 0.0f;
			g_Title.Logcol[TITLEVTX_LIBELLION].a = 0.0f;
			g_Title.Logcol[TITLEVTX_HONKI].a = 0.0f;

			g_Title.nType = 4;
			g_Title.nCntMoveLog = 0;
			g_Title.nState = TITLESTATE_NONE;
		}
	}



	for (int nCntLog = 0; nCntLog < TITLEVTX_MAX; nCntLog++)
	{

		//背景
		if (nCntLog != TITLEVTX_TITLELOGO)
		{
			pVtx[0].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x - 640.0f, g_Title.posLog[nCntLog].y - 360.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x + 640.0f, g_Title.posLog[nCntLog].y - 360.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x - 640.0f, g_Title.posLog[nCntLog].y + 360.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x + 640.0f, g_Title.posLog[nCntLog].y + 360.0f, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x - LOG_WIDTH, g_Title.posLog[nCntLog].y - LOG_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x + LOG_WIDTH, g_Title.posLog[nCntLog].y - LOG_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x - LOG_WIDTH, g_Title.posLog[nCntLog].y + LOG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x + LOG_WIDTH, g_Title.posLog[nCntLog].y + LOG_HEIGHT, 0.0f);
		}

		//頂点カラーの設定
		pVtx[0].col = g_Title.Logcol[nCntLog];
		pVtx[1].col = g_Title.Logcol[nCntLog];
		pVtx[2].col = g_Title.Logcol[nCntLog];
		pVtx[3].col = g_Title.Logcol[nCntLog];

		pVtx += 4;
	}


	//頂点バッファをアンロックする
	g_pVtxBuffTitle->Unlock();
}

//==================================================================================
//選択肢別の更新処理
//==================================================================================
void UpdateSelectTitle(void)
{

	if (GetKeyboardTrigger(DIK_D) == true || GetGamepadTrigger(BUTTON_RIGHT, 0) ||
		GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0)
	{//右系が押された

	 //左スティックの判定を渡す
		SetEnableStickSelect(true);

		//パターンNo.を更新
		g_Title.nSelect = (g_Title.nSelect + 1) % TITLESELECT_MAX;
		g_Title.col.a = MAX_ALPHA;

		//自動遷移カウンター加算
		g_Title.nCntChange = 0;

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
	}
	else if (GetKeyboardTrigger(DIK_A) == true || GetGamepadTrigger(BUTTON_LEFT, 0) ||
		GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0)
	{//左系が押された

	 //左スティックの判定を渡す
		SetEnableStickSelect(true);

		//パターンNo.を更新
		g_Title.nSelect = (g_Title.nSelect + (TITLESELECT_MAX - 1)) % TITLESELECT_MAX;
		g_Title.col.a = MAX_ALPHA;

		//自動遷移カウンター加算
		g_Title.nCntChange = 0;

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
	}


	if (GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0)
	{//左に倒された, スティックの判定がOFFの場合

	 //左スティックの判定を渡す
		SetEnableStickSelect(true);

		//パターンNo.を更新
		g_Title.nSelect = (g_Title.nSelect + (TITLESELECT_MAX - 1)) % TITLESELECT_MAX;
		g_Title.col.a = MAX_ALPHA;

		//自動遷移カウンター加算
		g_Title.nCntChange = 0;

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
	}
	else if (GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0)
	{//右に倒された, スティックの判定がOFFの場合

	 //左スティックの判定を渡す
		SetEnableStickSelect(true);

		//パターンNo.を更新
		g_Title.nSelect = (g_Title.nSelect + 1) % TITLESELECT_MAX;
		g_Title.col.a = MAX_ALPHA;

		//自動遷移カウンター加算
		g_Title.nCntChange = 0;

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
	}


	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0))
	{//Enterが押された

		if (g_Title.nSelect == TITLESELECT_START)
		{//スタートの時だけチカチカ

			//チカチカ状態に移行
			g_Title.nState = TITLESTATE_TKTK;
		}
		else
		{//他はすぐ遷移

			//フェード状態に移行
			g_Title.nState = TITLESTATE_FADE;
		}

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_SYUTUGEKI);
	}

	//選択肢の色更新
	UpdateSelectTitleCol(g_Title.nSelect);

}

//==================================================================================
// 選択肢の色更新処理
//==================================================================================
void UpdateSelectTitleCol(int Select)
{
	//VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//					//頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	////選択肢までポインタ移動
	//pVtx += 4 * TITLEVTX_MAX;

	//static float fMoveAlpha = 0.008f;

	//if (g_Title.col.a > 1.0f)
	//{//不透明度100%

	//	g_Title.col.a = 1.0f;
	//	fMoveAlpha *= -1;
	//}
	//else if (g_Title.col.a < 0.4f)
	//{//不透明度20%

	//	g_Title.col.a = 0.4f;
	//	fMoveAlpha *= -1;
	//}

	////不透明度の更新
	//g_Title.col.a += fMoveAlpha;

	////選択肢から始める
	//for (int nCount = TITLEVTX_MAX; nCount < TITLEVTX_MAX; nCount++)
	//{
	//	if (nCount != Select + TITLEVTX_MAX)
	//	{//今の選択肢じゃないとき

	//	 //選択肢
	//		pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X - POS_X, g_Title.pos.y - POS_Y, 0.0f);
	//		pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X + POS_X, g_Title.pos.y - POS_Y, 0.0f);
	//		pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X - POS_X, g_Title.pos.y + POS_Y, 0.0f);
	//		pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X + POS_X, g_Title.pos.y + POS_Y, 0.0f);

	//		//頂点カラーの設定
	//		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//	}
	//	else
	//	{//現在の選択肢

	//		float LengthX = POS_X;
	//		float LengthY = POS_Y;

	//		//選択肢
	//		pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X - LengthX, g_Title.pos.y - LengthY, 0.0f);
	//		pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X + LengthX, g_Title.pos.y - LengthY, 0.0f);
	//		pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X - LengthX, g_Title.pos.y + LengthY, 0.0f);
	//		pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X + LengthX, g_Title.pos.y + LengthY, 0.0f);

	//		//頂点カラーの設定
	//		pVtx[0].col = g_Title.col;
	//		pVtx[1].col = g_Title.col;
	//		pVtx[2].col = g_Title.col;
	//		pVtx[3].col = g_Title.col;
	//	}

	//	pVtx += 4;
	//}

	////頂点バッファをアンロックする
	//g_pVtxBuffTitle->Unlock();
}

//==================================================================================
// 選択肢のチカチカ処理
//==================================================================================
void UpdateTitleTKTK(void)
{

	//VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//					//頂点バッファをロックし、頂点情報へのポインタを取得
	//g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//チカチカフレーム加算
	/*g_Title.nTKTKFrame++;

	if (g_Title.nTKTKFrame >= 120)
	{
		g_Title.nState = TITLESTATE_FADE;
	}*/

	//if ((g_Title.nTKTKFrame % 2) == 0)
	//{//2フレームに1回

	//	g_Title.nTKTKFrame = 0;
	//	g_Title.nTKTKCounter++;	//チカチカ回数を加算

	//	if (g_Title.col.a >= MAX_ALPHA)
	//	{//不透明度100%

	//		g_Title.col.a = MIN_ALPHA;
	//	}
	//	else
	//	{//不透明度最小値

	//		g_Title.col.a = MAX_ALPHA;
	//	}

	//}

	//if (g_Title.nTKTKCounter >= MAX_TKTK)
	//{//チカチカが終わったら

	//	g_Title.nState = TITLESTATE_FADE;

	//	//最大値に設定
	//	g_Title.col.a = MAX_ALPHA;
	//}

	////モードがチカチカ
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 0].col = g_Title.col;
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 1].col = g_Title.col;
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 2].col = g_Title.col;
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 3].col = g_Title.col;

	////選択肢
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 0].pos = D3DXVECTOR3(g_Title.pos.x + g_Title.nSelect * DISTANCE_X - POS_X, g_Title.pos.y - POS_Y, 0.0f);
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 1].pos = D3DXVECTOR3(g_Title.pos.x + g_Title.nSelect * DISTANCE_X + POS_X, g_Title.pos.y - POS_Y, 0.0f);
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 2].pos = D3DXVECTOR3(g_Title.pos.x + g_Title.nSelect * DISTANCE_X - POS_X, g_Title.pos.y + POS_Y, 0.0f);
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 3].pos = D3DXVECTOR3(g_Title.pos.x + g_Title.nSelect * DISTANCE_X + POS_X, g_Title.pos.y + POS_Y, 0.0f);

	////頂点バッファをアンロックする
	//g_pVtxBuffTitle->Unlock();
}

//==================================================================================
//終了のフラグを取得
//==================================================================================
bool GetEndFrag(void)
{
	return g_bEndFrag;
}

//==================================================================================
//タイトルの情報取得
//==================================================================================
Title *GetTitle(void)
{
	return &g_Title;
}