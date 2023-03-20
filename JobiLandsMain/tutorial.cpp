//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "tutorial.h"
#include "sound.h"
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
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "debugproc.h"
#include "shadow.h"
#include "particle.h"
#include "2D_effect.h"
#include "effect.h"
#include "pause.h"

//マクロ定義
#define FEVERHYPNOSIS_NUM	(10)

//グローバル変数
Tutorial g_aTutorial;	//チュートリアルの情報
bool g_bSkip;

//==============================================================
//チュートリアル画面の初期化処理
//==============================================================
void InitTutorial(void)
{
	//各種変数初期化
	g_aTutorial.nStep = 0;	//ステップの初期化
	g_aTutorial.aControl.bUPMove = false;		//上移動
	g_aTutorial.aControl.bLMove = false;		//左移動
	g_aTutorial.aControl.bRMove = false;		//右移動
	g_aTutorial.aControl.bDWMove = false;		//下移動

	g_aTutorial.aControl.bSetMob = false;		//モブ配置したか
	g_aTutorial.aControl.bDispatchR = false;	//右派遣
	g_aTutorial.aControl.bDispatchL = false;	//左派遣
	g_aTutorial.aControl.bReturnL = false;		//左帰還
	g_aTutorial.aControl.bReturnR = false;		//右帰還
	
	g_aTutorial.aControl.bHypnosis = false;		//催眠度MAX
	g_aTutorial.aControl.bCutePillow = false;	//抱き枕カバー被せた

	g_aTutorial.aControl.bFeverUse = false;		//フィーバー状態にしたか
	g_aTutorial.aControl.bSetMobFever = false;	//フィーバー時にモブ配置したか
	g_aTutorial.aControl.nCntHypnosisFever = 0;	//フィーバー時にモブ捕まえた数

	g_aTutorial.aControl.bSetEnemy = false;		//敵配置したか
	g_aTutorial.aControl.bReleaseCrowd = false;	//洗脳解除

	g_bSkip = false;		//スキップ確認画面描画しない

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

	//メッシュドームの初期化処理
	InitMeshDome();

	//モデルの初期化処理
	InitModel();

	//プレイヤーの初期化処理
	InitPlayer();

	//ポーズメニューの初期化処理
	InitPause();

	//カメラ初期化処理
	InitCamera();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM_TUTORIAL);
}

//==============================================================
//チュートリアル画面の終了処理
//==============================================================
void UninitTutorial(void)
{
	//サウンドの停止
	StopSound();

	//ポーズメニューの終了処理
	UninitPause();

	//エフェクトの終了処理
	UninitEffect();

	//2Dエフェクトの終了処理
	UninitEffect_2D();

	//パーティクルの終了処理
	UninitParticle();

	//影の終了処理
	UninitShadow();

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

	//メッシュの床の終了処理
	UninitMeshField();
}

//==============================================================
//チュートリアル画面の更新処理
//==============================================================
void UpdateTutorial(void)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	if (g_bSkip == false)
	{
		//モデルの更新処理
		UpdateModel();

		//2Dエフェクトの更新処理
		UpdateEffect_2D();

		//if (pEdit->bUse == false)
		{
			//エフェクトの更新処理
			UpdateEffect();

			//パーティクルの更新処理
			UpdateParticle();

			//メッシュの床の更新処理
			UpdateMeshField();

			//メッシュシリンダーの更新処理
			UpdateMeshCylinder();

			//メッシュドームの更新処理
			UpdateMeshDome();

			//影の更新処理
			UpdateShadow();

			//衝撃波の更新処理
			UpdateImpactWave();
		}
	}

	PrintDebugProc("チュートリアルのステップ：%d", g_aTutorial.nStep);

}

//==============================================================
//チュートリアル画面の描画処理
//==============================================================
void DrawTutorial(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{

		//モデルの描画処理
		DrawModel(0);

		//メッシュの床の描画処理
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュドームの描画処理
		DrawMeshDome();

		//影の描画処理
		DrawShadow();

		//プレイヤーの描画処理
		DrawPlayer();

		//エフェクトの描画処理
		DrawEffect();

		//パーティクルの描画処理
		DrawParticle();

		//衝撃波の描画処理
		DrawImpactWave();

		//モデルの描画処理
		DrawModel(1);
	}

	if (nType == DRAWTYPE_UI)
	{
		//2Dエフェクトの描画処理
		DrawEffect_2D();
	}
}

//==============================================================
//チュートリアル画面の情報取得
//==============================================================
Tutorial *GetTutorial(void)
{
	return &g_aTutorial;
}

//==============================================================
//スキップ確認の設定処理
//==============================================================
void SetEnableSkip(bool bSkip)
{
	g_bSkip = bSkip;
}
