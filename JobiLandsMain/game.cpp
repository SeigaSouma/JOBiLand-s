//==============================================================
//
//ゲーム処理[game.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "game.h"
#include "input.h"
#include "camera.h"
#include "light.h"
#include "meshcylinder.h"
#include "impactwave.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshdome.h"
#include "model.h"
#include "player.h"
#include "edit.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "debugproc.h"
#include "shadow.h"
#include "particle.h"
#include "2D_effect.h"
#include "effect.h"
#include "pause.h"
#include "sound.h"
#include "nofade.h"
#include "launch.h"
#include "scorecircle.h"
#include "controllpanel.h"

//プロトタイプ宣言
void DebugEdit(void);

//グローバル変数
GAMESTATE g_gameState;		//ゲームの状態
int g_nCounterGameState;	//状態管理カウンター
bool g_bPause = false;		//ポーズ状態のON/OFF

//==============================================================
//ゲーム画面の初期化処理
//==============================================================
void InitGame(void)
{
	//影の初期化処理
	InitShadow();

	//エフェクトの初期化処理
	InitEffect();

	//2Dエフェクトの初期化処理
	InitEffect_2D();

	//パーティクルの初期化処理
	InitParticle();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	//衝撃波の初期化処理
	InitImpactWave();

	//メッシュの床の初期化処理
	InitMeshField();

	//メッシュの壁の初期化処理
	//InitMeshWall();

	//メッシュドームの初期化処理
	InitMeshDome();

	//モデルの初期化処理
	InitModel();

	//プレイヤーの初期化処理
	InitPlayer();

	//エディットの初期化
	InitEdit();

	//ポーズメニューの初期化処理
	InitPause();

	//スコアの初期化処理
	InitScore();

	//ゲームのカメラ初期化処理
	InitCamera();

	// 発射物の初期化処理
	InitLaunch();

	// スコアサークルの初期化処理
	InitScoreCircle();

	// 操作パネルの初期化処理
	InitControllPanel();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM_GAME);

	//各種変数初期化
	g_gameState = GAMESTATE_NONE;
	g_nCounterGameState = 0;
	g_bPause = false;
}

//==============================================================
//ゲーム画面の終了処理
//==============================================================
void UninitGame(void)
{
	//サウンドの停止
	StopSound();

	//ポーズメニューの終了処理
	UninitPause();
	SetEnablePause(false);		//ポーズを解除する

	//エフェクトの終了処理
	UninitEffect();

	//2Dエフェクトの終了処理
	UninitEffect_2D();

	//パーティクルの終了処理
	UninitParticle();

	//影の終了処理
	UninitShadow();

	//エディットの終了処理
	UninitEdit();

	//プレイヤーの終了処理
	UninitPlayer();

	//モデルの終了処理
	UninitModel();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	//メッシュドームの終了処理
	UninitMeshDome();

	//衝撃波の終了処理
	UninitImpactWave();

	//メッシュの壁の終了処理
	//UninitMeshWall();

	//メッシュの床の終了処理
	UninitMeshField();

	// 発射物の終了
	UninitLaunch();

	// スコアサークルの終了処理
	UninitScoreCircle();

	// 操作パネルの終了処理
	UninitControllPanel();

	//スコアの終了処理
	UninitScore();
}

//==============================================================
//ゲーム画面の更新処理
//==============================================================
void UpdateGame(void)
{
	//エディットの情報取得
	Edit *pEdit = GetEdit();

	if (GetFade() == FADE_NONE)
	{//何もしていないとき
		if (GetKeyboardTrigger(DIK_F2) == true)
		{
			SetFade(MODE_RESULT);
		}
	}

	if (g_bPause == false)
	{
		//エディットの更新処理
		UpdateEdit();

		//モデルの更新処理
		UpdateModel();

		//2Dエフェクトの更新処理
		UpdateEffect_2D();

		if (pEdit->bUse == false)
		{
			//if (pCamera->nState != CAMERASTATE_MANYSPAWN && pCutin->bUse == false)
			{//大量発生のカメラじゃなかったら && カットイン中じゃなかったら

				//エフェクトの更新処理
				UpdateEffect();

				//パーティクルの更新処理
				UpdateParticle();

				//メッシュの床の更新処理
				UpdateMeshField();

				//メッシュの壁の更新処理
				//UpdateMeshWall();

				//メッシュシリンダーの更新処理
				UpdateMeshCylinder();

				//メッシュドームの更新処理
				UpdateMeshDome();

				// 発射物の更新
				UpdateLaunch();

				//プレイヤーの更新処理
				UpdatePlayer();

				//影の更新処理
				UpdateShadow();

				//衝撃波の更新処理
				UpdateImpactWave();

				////軌跡の更新処理
				//UpdateMeshOrbit();

				//スコアの更新処理
				UpdateScore();

				// スコアサークルの更新処理
				UpdateScoreCircle();

				// 操作パネルの更新処理
				UpdateControllPanel();

			}
		}
	}
	else if (g_bPause == true)
	{//ポーズボタンが押されているとき

		UpdatePause();		//ポーズメニューを表示する
	}

	if (*GetGameState() != GAMESTATE_START)
	{
		if ((GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(BUTTON_START, 0)) &&
			GetFade() == FADE_NONE)
		{//Pキーが押された時

			if (g_bPause == false)
			{
				//サウンド再生
				//PlaySound(SOUND_LABEL_SE_THEWORLD);
			}

			g_bPause = (g_bPause == false) ? true : false;		//ポーズメニューが使われている状態にする
		}
	}

	
	switch (g_gameState)
	{
	case GAMESTATE_SHOT:		// 打ち出す状態

		// 発射する処理
		ShotLaunchChunk();

		break;

	case GAMESTATE_END:

		// 遷移カウンター減算
		g_nCounterGameState--;

		if (g_nCounterGameState <= 0)
		{
			// モード設定(リザルト画面に移行)
			SetFade(MODE_RESULT);		// フ ェ ー ド ア ウ ト
		}
		break;
	}
}

//==============================================================
//ゲーム画面の描画処理
//==============================================================
void DrawGame(int nType)
{

	if (nType == DRAWTYPE_MAIN)
	{

		//モデルの描画処理
		DrawModel(0);

		// 発射物の描画処理
		DrawLaunch();

		//メッシュの床の描画処理
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//メッシュの壁の描画処理
		//DrawMeshWall();

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュドームの描画処理
		DrawMeshDome();

		//軌跡の描画処理
		//DrawMeshOrbit();

		//影の描画処理
		DrawShadow();

		//プレイヤーの描画処理
		DrawPlayer();

		//エフェクトの描画処理
		DrawEffect();

		//パーティクルの描画処理
		DrawParticle();

		//エディットの描画処理
		DrawEdit();

		////スコアの描画処理
		//DrawScore();

		//エディットの描画処理
		DebugEdit();

		//衝撃波の描画処理
		DrawImpactWave();

		// スコアサークルの描画処理
		DrawScoreCircle();

		//モデルの描画処理
		DrawModel(1);

	}

	if (nType == DRAWTYPE_UI)
	{

		if (g_gameState != GAMESTATE_START)
		{
			//2Dエフェクトの描画処理
			DrawEffect_2D();
		}

		//スコアの描画処理
		DrawScore();

		// 操作パネルの描画処理
		DrawControllPanel();

		if (g_bPause == true)
		{//ポーズメニューが使われているとき

			//ポーズ画面を表示する
			DrawPause();
		}
	}
}

//==============================================================
//ゲーム状態の設定
//==============================================================
void SetGameState(GAMESTATE state, int nCounter)
{
	g_gameState = state;
	g_nCounterGameState = nCounter;
}

//==============================================================
//ゲーム状態の取得
//==============================================================
GAMESTATE *GetGameState(void)
{
	return &g_gameState;
}

//==============================================================
//ポーズ画面の設定処理
//==============================================================
void SetEnablePause(bool bPouse)
{
	g_bPause = bPouse;
}

//==============================================================
//ポーズの情報取得
//==============================================================
bool GetPause(void)
{
	return g_bPause;
}

//==============================================================
//エディットの操作説明
//==============================================================
void DebugEdit(void)
{
	//エディットの情報取得
	Edit *pEdit = GetEdit();

	PrintDebugProc("[F3]：ワイヤーフレーム\n");
	PrintDebugProc("[F6]：背面追従切り替え\n");
	PrintDebugProc("[F7]：カメラ追従切り替え\n");
	PrintDebugProc("[F8]：エディットモード\n");

	if (pEdit->bUse == true)
	{//エディットモードじゃない時

		//文字列に代入
		PrintDebugProc(
			"<モデル配置>   [ENTER]\n"
			"<移動>         高速：[↑,↓,←,→]　低速：[W,A,S,D]\n"
			"<回転>         [LSHIFT, RSHIFT]\n"
			"<上昇,下降>    [I,K]\n"
			"<種類変更>     [1, 2][%d]\n"
			"<掴み移動>     [SPACE]\n"
			"<削除>         [DELETE]\n"
			"<位置>         [X：%f Y：%f Z：%f]\n"
			"<向き>         [X：%f Y：%f Z：%f]\n", pEdit->nNowModelType, pEdit->pos.x, pEdit->pos.y, pEdit->pos.z, pEdit->rot.x, pEdit->rot.y, pEdit->rot.z);

	}
}

//==============================================================
//ゲーム終了時の更新
//==============================================================
void SetGameEnd(void)
{
	////プレイヤーの情報取得
	//Player *pPlayer = GetPlayer();

	////カメラの情報取得
	//Camera *pCamera = GetCamera();

	////カメラの位置設定
	//pCamera->rot = D3DXVECTOR3(0.0f, 0.0f, -0.35f);
	//pCamera->fDistance = 800;	//距離

	////プレイヤーの位置設定
	//pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	////ニュートラルモーション設定
	//SetMotionPlayer(PLAYERMOTION_DEF);
}