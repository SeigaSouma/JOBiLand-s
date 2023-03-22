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

//グローバル変数宣言
bool g_bEndFrag;									//終了のフラグ
Title g_Title;

//==================================================================================
//タイトルの初期化処理
//==================================================================================
void InitTitle(void)
{
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
}

//==================================================================================
//タイトルの更新処理
//==================================================================================
void UpdateTitle(void)
{

	if (GetFade() == FADE_NONE && g_Title.nState == TITLESTATE_NONE)
	{//何もしていないとき

		//自動遷移カウンター加算
		g_Title.nCntChange++;

		//選択処理
		UpdateSelectTitle();

	}
	else if (g_Title.nState == TITLESTATE_TKTK)
	{//チカチカ状態のとき

		//選択肢のチカチカ処理
		//UpdateTitleTKTK();
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