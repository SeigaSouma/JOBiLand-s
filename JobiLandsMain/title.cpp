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
#include "sound.h"
#include "titleselect.h"
#include "titlelogo.h"

//マクロ定義
#define CHANGE_TIME	(60 * 25)	//切り替えタイマー
#define TITLETEX	"data/TEXTURE/title.png"
#define PRESSENTERTEX	"data/TEXTURE/pressenter.png"
#define FLASHING_TIME   (40)  //点滅時間
#define MAX_TITLE		(2)		//テクスチャの最大数

//=======================================
//列挙型
//=======================================
typedef enum
{
	TITLE_LOGO,  //タイトルロゴ
	TITLE_ENTER, //PressEnter
	TITLE_MAX,
}TITLE;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_aTextureTitle[MAX_TITLE] = {};      //テクスチャ(3枚分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;       //頂点バッファへのポインタ
bool g_bEndFrag;									//終了のフラグ
Title g_Title;
int g_EnterCounter;   //PressEnterの点滅カウンター
int g_EnterState;     //現在の色
bool g_TitleFade;     //falseに使用するときに必要

//==================================================================================
//タイトルの初期化処理
//==================================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	g_Title.OldSelect = TITLESELECT_START;	//前回の選択肢
	g_Title.nSelect = TITLESELECT_START;	//今回の選択肢
	g_Title.nCntChange = 0;
	g_Title.nState = TITLESTATE_NONE;	//状態
	g_bEndFrag = false;

	//影の初期化処理
	InitShadow();

	//モデルの初期化処理
	InitModel();

	//エディットの初期化
	InitEdit();

	//タイトルの選択肢初期化処理
	InitTitleSelect();

	// タイトルのロゴの初期化処理
	InitTitleLogo();

	//プレイヤーの初期化処理
	InitPlayer();

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
	InitCamera();

	//BGMをセット
	PlaySound(SOUND_LABEL_BGM_TITLE);

	//0番目のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TITLETEX, &g_aTextureTitle[0]);

	//1番目のテクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, PRESSENTERTEX, &g_aTextureTitle[1]);

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//PressEnterの点滅カウンター
	g_EnterCounter = 0;

	//現在の色
	g_EnterState = 0;


	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();

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

	// タイトルのロゴの終了処理
	UninitTitleLogo();

	//プレイヤーの終了処理
	UninitPlayer();

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

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		//テクスチャの破棄
		if (g_aTextureTitle[nCntTitle] != NULL)
		{
			g_aTextureTitle[nCntTitle]->Release();

			g_aTextureTitle[nCntTitle] = NULL;
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
	//頂点情報へのポインタ
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case TITLE_LOGO:  //タイトルロゴ
			break;
		case TITLE_ENTER: //PressEnter
			g_EnterCounter++;

			if (g_EnterCounter >= FLASHING_TIME)
			{
				g_EnterCounter = 0;

				g_EnterState ^= 1;

				if (g_EnterState == 1)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				}
				if (g_EnterState == 0)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
			break;
		}

		//テクスチャ座標の更新
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロック
	g_pVtxBuffTitle->Unlock();

	//決定キー(ENTERキー)が押された
	if (GetKeyboardTrigger(DIK_RETURN) == true/* || GetPadTrigger(BUTTON_A, 0) == true*/ && g_TitleFade == false)
	{
		//モード設定(チュートリアルに移行)
		SetFade(MODE_TUTORIAL);

		g_TitleFade = true;
	}

	//カメラの更新処理
	UpdateCamera();

	//モデルの更新処理
	UpdateModel();

	//エディットの更新処理
	UpdateEdit();

	//タイトルの選択肢更新処理
	UpdateTitleSelect();

	// タイトルのロゴの更新処理
	UpdateTitleLogo();

	//影の更新処理
	UpdateShadow();

	//エフェクトの更新処理
	UpdateEffect();

	//2Dエフェクトの更新処理
	UpdateEffect_2D();

	//パーティクルの更新処理
	UpdateParticle();

	if (g_Title.nCntChange >= CHANGE_TIME)
	{//自動遷移のカウンターが規定値に達したら

		//ランキングに遷移
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

		// タイトルのロゴの描画処理
		DrawTitleLogo();

		//プレイヤーの描画処理
		DrawPlayer();

		//選択肢の描画
		DrawTitleSelect();

		//エフェクトの描画処理
		DrawEffect();

		//パーティクルの描画処理
		DrawParticle();

		LPDIRECT3DDEVICE9 pDevice = GetDevice();	//デバイスの取得

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntTitle = 0; nCntTitle < 2; nCntTitle++)
		{
			pDevice->SetTexture(0, g_aTextureTitle[nCntTitle]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
			
		}
	}

	if (nType == DRAWTYPE_UI)
	{
		//2Dエフェクトの描画処理
		DrawEffect_2D();
	}
}

//==================================================================================
//選択肢別の更新処理
//==================================================================================
void UpdateSelectTitle(void)
{

	if (GetKeyboardTrigger(DIK_D) == true || GetGamepadTrigger(BUTTON_RIGHT, 0) ||
		(GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0))
	{//右系が押された

	 //左スティックの判定を渡す
		SetEnableStickSelect(true);

		//パターンNo.を更新
		g_Title.nSelect = (g_Title.nSelect + 1) % TITLESELECT_MAX;

		//自動遷移カウンター加算
		g_Title.nCntChange = 0;

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_CURSOR);
	}
	else if (GetKeyboardTrigger(DIK_A) == true || GetGamepadTrigger(BUTTON_LEFT, 0) ||
		(GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0))
	{//左系が押された

	 //左スティックの判定を渡す
		SetEnableStickSelect(true);

		//パターンNo.を更新
		g_Title.nSelect = (g_Title.nSelect + (TITLESELECT_MAX - 1)) % TITLESELECT_MAX;

		//自動遷移カウンター加算
		g_Title.nCntChange = 0;

		//サウンド再生
		PlaySound(SOUND_LABEL_SE_CURSOR);
	}


	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0))
	{//Enterが押された

		if (g_Title.nSelect == TITLESELECT_START)
		{//スタートの時だけチカチカ

			//チカチカ状態に移行
			g_Title.nState = TITLESTATE_FADE;
		}
		else
		{//他はすぐ遷移

			//フェード状態に移行
			g_Title.nState = TITLESTATE_FADE;
		}

		//サウンド再生
		//PlaySound(SOUND_LABEL_SE_SYUTUGEKI);
	}

	//選択肢の色更新
	//UpdateSelectTitleCol(g_Title.nSelect);

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