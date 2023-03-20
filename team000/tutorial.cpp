//=============================================================================
//
// チュートリアル処理 [tutorial.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "tutorial.h"
#include "tutorial_window.h"
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
#include "enemy.h"
#include "player.h"
#include "fade.h"
#include "ranking.h"
#include "score.h"
#include "TimeCycle.h"
#include "debugproc.h"
#include "crowd.h"
#include "mob.h"
#include "buddy.h"
#include "rotate.h"
#include "hypnosis_gauge.h"
#include "dispatch.h"
#include "search_circle.h"
#include "iventsign.h"
#include "cutepillow.h"
#include "shadow.h"
#include "meshorbit.h"
#include "minimap.h"
#include "minimap_icon.h"
#include "particle.h"
#include "2D_effect.h"
#include "effect.h"
#include "pause.h"
#include "powergage.h"
#include "EventWindow.h"
#include "watergun.h"
#include "search_fan.h"
#include "manyUI.h"
#include "manypoint.h"
#include "manyMORS.h"
#include "eventstamp.h"
#include "dispatchpoint.h"
#include "serif.h"
#include "fevergauge.h"
#include "cutin.h"
#include "minimapframe.h"
#include "tutorial_talk.h"
#include "numberPeople.h"
#include "ground.h"
#include "tutorial_skip.h"
#include "feverpush.h"
#include "dispatch_texture.h"

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

	//ライトのリセット処理
	ResetLight();

	//影の初期化処理
	InitShadow();

	//エフェクトの初期化処理
	InitEffect();

	//2Dエフェクトの初期化処理
	InitEffect_2D();

	//パーティクルの初期化処理
	InitParticle();

	//ぐるぐるの初期化処理
	InitRotate();

	//催眠度ゲージの初期化処理
	InitHypnosisGauge();

	//扇の形の初期化
	InitSearchFan();

	//派遣ターゲットの初期化処理
	InitDisPatch();

	//派遣シリンダーの初期化処理
	InitDispatchPoint();

	//大量発生地点の初期化処理
	InitManyPoint();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	//衝撃波の初期化処理
	InitImpactWave();

	//探索範囲の初期化処理
	InitSearchCircle();

	//メッシュの床の初期化処理
	InitMeshField();

	//メッシュドームの初期化処理
	InitMeshDome();

	//ミニマップの初期化
	InitMinimap();

	//アイコンの初期化処理
	InitMiniIcon();

	//モデルの初期化処理
	InitModel();

	//プレイヤーの初期化処理
	InitPlayer();

	//相棒の初期化処理
	InitBuddy();

	//敵の初期化処理
	InitEnemy();

	//イベント矢印の初期化処理
	InitIventSign();

	//タイムサイクルの初期化処理
	InitTimeCycle();

	//フィーバーゲージの初期化処理
	InitFeverGauge();

	//群衆の初期化処理
	InitCrowd();

	//モブの初期化処理
	InitMob();

	//抱き枕カバーの初期化処理
	InitCutepillow();

	//ポーズメニューの初期化処理
	InitPause();

	//勢力ゲージの初期化処理
	InitPowerGage();

	//イベント関連の初期化処理
	InitEventWindow();

	//大量発生のUI
	InitManyUI();

	//警備強化のUI初期化処理
	InitManyMORS();

	//イベントスタンプの初期化処理
	InitEventStamp();

	//水鉄砲の弾の初期化処理
	InitWatergun();

	//セリフの初期化処理
	InitSerif();

	//チュートリアルウィンドウの初期化処理
	InitTutorialWindow();

	//チュートリアルトークの初期化処理
	InitTutorialTalk();

	//ゲームのカメラ初期化処理
	InitGameCamera();

	//カットインの初期化処理
	InitCutin();

	//ミニマップフレームの初期化処理
	InitMinimapframe();

	//人数表示の背景の初期化処理
	InitGround();

	//人数表示の初期化処理
	InitPeople();

	//スキップ確認の初期化処理
	InitTutorialSkip();

	//フィーバープッシュの初期化処理
	InitFeverPush();

	//派遣テクスチャの初期化処理
	InitDispatchTex();

	//サウンドの再生
	//PlaySound();

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

	//イベント矢印の終了処理
	UninitIventSign();

	//ミニマップの終了処理
	UninitMinimap();

	//アイコンの終了処理
	UninitMiniIcon();

	//敵の終了処理
	UninitEnemy();

	//プレイヤーの終了処理
	UninitPlayer();

	//相棒の終了処理
	UninitBuddy();

	//モデルの終了処理
	UninitModel();

	//派遣ターゲットの終了処理
	UninitDisPatch();

	//派遣シリンダーの終了処理
	UninitDispatchPoint();

	//大量発生地点の終了処理
	UninitManyPoint();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	//メッシュドームの終了処理
	UninitMeshDome();

	//衝撃波の終了処理
	UninitImpactWave();

	//探索範囲の終了処理
	UninitSearchCircle();

	//メッシュの床の終了処理
	UninitMeshField();

	//タイムサイクルの終了処理
	UninitTimeCycle();

	//フィーバーゲージの終了処理
	UninitFeverGauge();

	//群衆の終了処理
	UninitCrowd();

	//モブの終了処理
	UninitMob();

	//ぐるぐるの終了処理
	UnInitRotate();

	//催眠度ゲージの終了処理
	UninitHypnosisGauge();

	//抱き枕カバーの終了処理
	UninitCutepillow();

	//勢力ゲージの終了処理
	UninitPowerGage();

	//イベント関連の終了処理
	UninitEventWindow();

	//大量発生のUI終了処理
	UninitManyUI();

	//警備強化のUI終了処理
	UninitManyMORS();

	//イベントスタンプの終了処理
	UninitEventStamp();

	//水鉄砲の弾の終了処理
	UninitWatergun();

	//扇の形の終了処理
	UnInitSearchFan();

	//セリフの終了処理
	UninitSerif();

	//チュートリアルウィンドウの終了処理
	UninitTutorialWindow();

	//チュートリアルトークの終了処理
	UninitTutorialTalk();

	//カットインの終了処理
	UninitCutin();

	//ミニマップフレームの終了処理
	UninitMinimapframe();

	//人数表示の背景の終了処理
	UninitGround();

	//人数表示の終了処理
	UninitPeople();

	//スキップ確認の終了処理
	UninitTutorialSkip();

	//フィーバープッシュの終了処理
	UninitFeverPush();

	//派遣テクスチャの終了処理
	UninitDispatchTex();
}

//==============================================================
//チュートリアル画面の更新処理
//==============================================================
void UpdateTutorial(void)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//カットイン情報取得
	CUTIN *pCutin = GetCutin();

	//チュートリアルトークの情報取得
	TutorialTalk *pTutorialTalk = GetTutorialTalk();

	if (g_bSkip == false)
	{
		//モデルの更新処理
		UpdateModel();

		//2Dエフェクトの更新処理
		UpdateEffect_2D();

		//if (pEdit->bUse == false)
		{
			if (pCamera->nState != CAMERASTATE_MANYSPAWN && pCutin->bUse == false && pTutorialTalk->bUse == false)
			{//大量発生のカメラじゃなかったら && カットイン中じゃなかったら

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

				//プレイヤーの更新処理
				UpdateTutorialPlayer();

				//相棒の更新処理
				UpdateBuddy();

				//敵の更新処理
				UpdateTutorialEnemy();

				//扇の形の更新処理
				UpdateSearchFan();

				//タイムサイクルの更新処理
				UpdateTimeCycle();

				//モブの更新処理
				UpdateTutorialMob();

				//群衆の更新処理
				UpdateCrowd();

				//ぐるぐるの更新処理
				UpdateRotate();

				//催眠度ゲージの更新処理
				UpdateHypnosisGauge();

				//派遣ターゲットの更新処理
				UpdateTutorialDisPatch();

				//派遣シリンダーの更新処理
				UpdateDispatchPoint();

				//抱き枕カバーの更新処理
				UpdateCutepillow();

				//影の更新処理
				UpdateShadow();

				//衝撃波の更新処理
				UpdateImpactWave();

				//探索範囲の更新処理
				UpdateSearchCircle();

				//ミニマップの更新処理
				UpdateMinimap();

				//アイコンの更新処理
				UpdateMiniIcon();

				//勢力ゲージの更新処理
				UpdatePowerGage();

				//フィーバーゲージの更新処理
				UpdateFeverGauge();

				//フィーバープッシュの更新処理
				UpdateFeverPush();

				//水鉄砲の弾の更新処理
				UpdateWatergun();

				//セリフの更新処理
				UpdateSerif();

				//人数表示の更新処理
				UpdateGround();

				//人数表示の更新処理
				UpdatePeople();
			}

			//イベント関連の更新処理
			UpdateEventWindow();

			//大量発生のUI更新処理
			UpdateManyUI();

			//警備強化のUI更新処理
			UpdateManyMORS();

			//イベントスタンプの更新処理
			UpdateEventStamp();

			//大量発生地点の更新処理
			UpdateManyPoint();

			//イベント矢印の更新処理
			UpdateIventSign();

			//カットインの更新処理
			UpdateCutin();

			//ミニマップフレームの更新処理
			UpdateMinimapframe();

			//チュートリアルウィンドウの更新処理
			UpdateTutorialWindow();

			//チュートリアルトークの更新処理
			UpdateTutorialTalk();

			//派遣テクスチャの更新処理
			UpdateDispatchTex();
		}
	}
	else if (g_bSkip == true)
	{
		//スキップ確認の更新処理
		UpdateTutorialSkip();
	}

	if ((GetKeyboardTrigger(DIK_BACKSPACE) == true || GetGamepadTrigger(BUTTON_START, 0)) &&
		GetFade() == FADE_NONE && g_aTutorial.nStep < TUTORIAL_STEP_END)
	{//Pキーが押された時

		if (g_bSkip == false)
		{
			//サウンド再生
			PlaySound(SOUND_LABEL_SE_THEWORLD);
		}

		g_bSkip = (g_bSkip == false) ? true : false;		//ポーズメニューが使われている状態にする

		//SetFade(MODE_GAME);
	}

	if (pCamera->nState != CAMERASTATE_MANYSPAWN && pCutin->bUse == false)
	{//大量発生のカメラじゃなかったら && カットイン中じゃなかったら

		switch (g_aTutorial.nStep)
		{
		case TUTORIAL_STEP_START:

			if (GetFade() == FADE_NONE)
			{//全方位移動終わったら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_MOVE:

			if (g_aTutorial.aControl.bUPMove == true &&
				g_aTutorial.aControl.bLMove == true &&
				g_aTutorial.aControl.bRMove == true &&
				g_aTutorial.aControl.bDWMove == true)
			{//全方位移動終わったら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_DISPATCH:

			if (g_aTutorial.aControl.bDispatchR == true &&
				g_aTutorial.aControl.bDispatchL == true)
			{//両方派遣できたら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_RETURN:

			if (g_aTutorial.aControl.bReturnL == true &&
				g_aTutorial.aControl.bReturnR == true)
			{//両方帰還出来たら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_HYPNOSIS:

			if (g_aTutorial.aControl.bSetMob == true &&
				g_aTutorial.aControl.bHypnosis == true)
			{//モブ設置と催眠ができたら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_SENNOU:

			if (g_aTutorial.aControl.bCutePillow == true)
			{//洗脳できたら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_USEFEVER:

			if (g_aTutorial.aControl.bFeverUse == true)
			{//フィーバー状態に入ったら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_HYPNOSISFEVER:

			if (g_aTutorial.aControl.nCntHypnosisFever >= FEVERHYPNOSIS_NUM)
			{//フィーバーで洗脳できたら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_MORS:

			if (g_aTutorial.aControl.bSetEnemy == true &&
				g_aTutorial.aControl.bReleaseCrowd == true)
			{//敵設置と群衆の洗脳解除できたら

				SetTutorialTalk();
			}
			break;

		case TUTORIAL_STEP_END:

			if ((GetKeyboardTrigger(DIK_BACKSPACE) == true || GetGamepadTrigger(BUTTON_START, 0)) &&
				GetFade() == FADE_NONE)
			{//Pキーが押された時

				SetFade(MODE_GAME);
			}
			break;
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

		//派遣ターゲットの描画処理
		DrawDisPatch();

		//影の描画処理
		DrawShadow();

		//扇の形の描画処理
		DrawSearchFan();

		//プレイヤーの描画処理
		DrawPlayer();

		//相棒の描画処理
		DrawBuddy();

		//敵の描画処理
		DrawEnemy();

		//群衆の描画処理
		DrawCrowd();

		//モブの描画処理
		DrawMob();

		//エフェクトの描画処理
		DrawEffect();

		//パーティクルの描画処理
		DrawParticle();

		//水鉄砲の弾の描画処理
		DrawWatergun();

		//ぐるぐるの描画処理
		DrawRotate();

		//催眠度ゲージの描画処理
		DrawHypnosisGauge();

		//セリフの描画処理
		DrawSerif();

		//大量発生地点の描画処理
		DrawManyPoint();

		//派遣シリンダーの描画処理
		DrawDispatchPoint();

		//探索範囲の描画処理
		DrawSearchCircle();

		//抱き枕カバーの描画処理
		DrawCutepillow();

		//衝撃波の描画処理
		DrawImpactWave();

		//モデルの描画処理
		DrawModel(1);

		//ミニマップフレームの描画処理
		DrawMinimapframe();
	}

	if (nType == DRAWTYPE_MAP)
	{
		//フィールドの描画
		DrawMeshField(DRAWFIELD_TYPE_MAP);

		//プレイヤーの描画
		DrawMiniIcon();

		//ミニマップの描画
		DrawMinimap();
	}

	if (nType == DRAWTYPE_UI)
	{
		//タイムサイクルの描画処理
		DrawTimeCycle();

		//勢力ゲージの描画処理
		DrawPowerGage();

		//フィーバーゲージの描画処理
		DrawFeverGauge();

		//2Dエフェクトの描画処理
		DrawEffect_2D();

		//人数表示の背景の描画処理
		DrawGround();

		//人数表示の描画処理
		DrawPeople();

		//フィーバープッシュの描画処理
		DrawFeverPush();

		//派遣テクスチャの描画処理
		DrawDispatchTex();

		//イベント関連の描画処理
		DrawEventWindow();

		//大量発生のUI描画処理
		DrawManyUI();

		//警備強化のUI描画処理
		DrawManyMORS();

		//イベントスタンプの描画処理
		DrawEventStamp();

		//チュートリアルウィンドウの描画処理
		DrawTutorialWindow();

		//カットインの描画処理
		DrawCutin();

		//チュートリアルトークの描画処理
		DrawTutorialTalk();

		if (g_bSkip == true)
		{
			//スキップ確認の描画処理
			DrawTutorialSkip();
		}

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
