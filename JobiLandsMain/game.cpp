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
#include "enemy.h"
#include "player.h"
#include "edit.h"
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
#include "serif.h"
#include "numberPeople.h"
#include "ground.h"
#include "sound.h"
#include "fevergauge.h"
#include "dispatchpoint.h"
#include "updown_fram.h"
#include "start_talk.h"
#include "cutin.h"
#include "comment.h"
#include "realtime_ranking.h"
#include "nofade.h"
#include "iventradar.h"
#include "mapshape.h"
#include "minimapframe.h"
#include "endmessage.h"
#include "resultmessage.h"
#include "flag.h"
#include "endcountdown.h"
#include "animation.h"
#include "fenish.h"
#include "feverpush.h"
#include "dispatch_texture.h"

//プロトタイプ宣言
void DebugEdit(void);

//グローバル変数
GAMESTATE g_gameState = GAMESTATE_START;		//ゲームの状態
TARGETNOP g_nTargetNoP;                     //目標の人数の情報
int g_nCounterGameState = 0;				//状態管理カウンター
bool g_bPause = false;						//ポーズ状態のON/OFF
bool g_bStart = true;						//スタートを使用しているか

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

	//旗の初期化処理
	InitFlag();

	//メッシュの床の初期化処理
	InitMeshField();

	//メッシュの壁の初期化処理
	//InitMeshWall();

	//メッシュドームの初期化処理
	InitMeshDome();

	////軌跡の初期化処理
	//InitMeshOrbit();

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

	//エディットの初期化
	InitEdit();

	//イベント矢印の初期化処理
	InitIventSign();

	//タイムサイクルの初期化処理
	InitTimeCycle();

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

	//偽ゲージ
	//InitNiseGauge();

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

	//人数表示の初期化処理
	InitPeople();

	//フィーバーゲージの初期化処理
	InitFeverGauge();

	//人数表示の背景の初期化
	InitGround();

	//上下の枠の初期化処理
	InitUpDownFram();

	//最初の会話の初期化処理
	InitStartTalk();

	//ゲームのカメラ初期化処理
	InitGameCamera();

	//カットインの初期化処理
	InitCutin();

	//コメントの初期化処理
	InitCommnet();

	//リアルタイムランキングの初期化処理
	InitRealTimeRanking();

	//レーダーの初期化処理
	InitIventRadar();

	//ミニマップフレームの初期化処理
	InitMinimapframe();

	//ゲーム終了前メッセージの初期化処理
	InitEndmessage();

	//結果のメッセージの初期化処理
	InitResultMessage();

	//マップの形初期化処理
	//InitMapShape();

	//ゲーム終了5秒前カウントの初期化処理
	InitEndCountDown();

	//アニメーションの初期化処理
	InitAnimation();

	//終了の初期化処理
	InitFinish();

	//フィーバープッシュの初期化処理
	InitFeverPush();

	//派遣テクスチャの初期化処理
	InitDispatchTex();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM_GAME);

	g_gameState = GAMESTATE_START;		//通常状態に設定
	g_nTargetNoP.nCntPeople = TARGETHITO;      //仮の目標の人数
	g_nCounterGameState = 0;
	g_bStart = true;
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

	//メッシュの壁の終了処理
	//UninitMeshWall();

	//メッシュの床の終了処理
	UninitMeshField();

	////軌跡の終了処理
	//UninitMeshOrbit();

	//タイムサイクルの終了処理
	UninitTimeCycle();

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

	//偽ゲージ
	//UninitNiseGauge();

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

	//セリフの終了処理
	UninitSerif();

	//扇の形の終了処理
	UnInitSearchFan();

	//人数表示の終了処理
	UninitPeople();

	//フィーバーゲージの終了処理
	UninitFeverGauge();

	//人数表示の背景の終了処理
	UninitGround();

	//上下の枠の終了処理
	UninitUpDownFram();

	//最初の会話の終了処理
	UninitStartTalk();

	//カットインの終了処理
	UninitCutin();

	//コメントの終了処理
	UninitCommnet();

	//リアルタイムランキングの終了処理
	UninitRealTimeRanking();

	//レーダーの終了処理
	UninitIventRadar();

	//ミニマップフレームの終了処理
	UninitMinimapframe();

	//ゲーム終了前メッセージの終了処理
	UninitEndmessage();

	//結果のメッセージの終了処理
	UninitResultMessage();

	//マップの形終了処理
	//UninitMapShape();

	//旗の終了処理
	UnInitFlag();

	//ゲーム終了5秒前カウントの終了処理
	UninitEndCountDown();

	//アニメーションの終了処理
	UninitAnimation();

	//終了の終了処理
	UninitFinish();

	//フィーバープッシュの終了処理
	UninitFeverPush();

	//派遣テクスチャの終了処理
	UninitDispatchTex();

}

//==============================================================
//ゲーム画面の更新処理
//==============================================================
void UpdateGame(void)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//エディットの情報取得
	Edit *pEdit = GetEdit();

	//カットイン情報取得
	CUTIN *pCutin = GetCutin();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

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
			if (pCamera->nState != CAMERASTATE_MANYSPAWN && pCutin->bUse == false)
			{//大量発生のカメラじゃなかったら && カットイン中じゃなかったら

				//エフェクトの更新処理
				UpdateEffect();

				//パーティクルの更新処理
				UpdateParticle();

				//メッシュの床の更新処理
				UpdateMeshField();

				//アニメーションの更新処理
				UpdateAnimation();

				//メッシュの壁の更新処理
				//UpdateMeshWall();

				//メッシュシリンダーの更新処理
				UpdateMeshCylinder();

				//メッシュドームの更新処理
				UpdateMeshDome();

				//プレイヤーの更新処理
				UpdatePlayer();

				//相棒の更新処理
				UpdateBuddy();

				//敵の更新処理
				UpdateEnemy();
				
				//扇の形の更新処理
				UpdateSearchFan();

				//タイムサイクルの更新処理
				UpdateTimeCycle();

				//モブの更新処理
				UpdateMob();

				//群衆の更新処理
				UpdateCrowd();

				//ぐるぐるの更新処理
				UpdateRotate();

				//催眠度ゲージの更新処理
				UpdateHypnosisGauge();

				//派遣ターゲットの更新処理
				UpdateDisPatch();

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

				////軌跡の更新処理
				//UpdateMeshOrbit();

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

				//人数表示の更新処理
				UpdatePeople();

				//人数表示の背景の更新処理
				UpdateGround();

				//偽ゲージ
				//UpdateNiseGauge();

				//水鉄砲の弾の更新処理
				UpdateWatergun();

				//セリフの更新処理
				UpdateSerif();

				//旗の更新処理
				UpdateFlag();

				//ゲーム終了5秒前カウントの更新処理
				UpdateEndCountDown();
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

			//上下の枠の更新処理
			UpdateUpDownFram();

			//最初の会話の更新処理
			UpdateStartTalk();

			//カットインの更新処理
			UpdateCutin();

			//コメントの更新処理
			UpdateCommnet();

			//リアルタイムランキングの更新処理
			UpdateRealTimeRanking();

			//レーダーの更新処理
			UpdateIventRadar();

			//ミニマップフレームの更新処理
			UpdateMinimapframe();

			//ゲーム終了前メッセージの更新処理
			UpdateEndmessage();

			//結果のメッセージの更新処理
			UpdateResultMessage();

			//派遣テクスチャの更新処理
			UpdateDispatchTex();

			//終了の更新処理
			UpdateFinish();

			//マップの形更新処理
			//UpdateMapShape();

		}
	}
	else if (g_bPause == true)
	{//ポーズボタンが押されているとき

		UpdatePause();		//ポーズメニューを表示する
	}

	if (GetGameState() != GAMESTATE_START && GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_AFTEREND)
	{
		if ((GetKeyboardTrigger(DIK_P) == true || GetGamepadTrigger(BUTTON_START, 0)) &&
			GetFade() == FADE_NONE)
		{//Pキーが押された時

			if (g_bPause == false)
			{
				//サウンド再生
				PlaySound(SOUND_LABEL_SE_THEWORLD);
			}

			g_bPause = (g_bPause == false) ? true : false;		//ポーズメニューが使われている状態にする
		}
	}

	//群衆情報取得
	CrowdInfo *pCrowdInfo = GetCrowdInfo();
	
	switch (g_gameState)
	{
	case GAMESTATE_BEFOREEND:

		if (g_nCounterGameState <= 0)
		{
			//位置設定
			SetGameEnd();
		}

		g_nCounterGameState++;

		if (g_nCounterGameState >= 60 * 3)
		{//3秒以降

			//カメラ追従OFF
			pCamera->bFollow = false;

			//結果のメッセージ設定
			if (pPlayer->pos.z <= pCamera->posV.z * 0.5f &&
				GetResultMessage()->bUse == false)
			{
				if (TARGETHITO <= pCrowdInfo->nCrowdValue)
				{//目的の数よりも多かったら

					//成功
					SetResultMessage(RESULTTYPE_CLEAR);
				}
				else
				{
					//失敗
					SetResultMessage(RESULTTYPE_FAILED);
				}
			}

			if (pPlayer->pos.z <= pCamera->posV.z)
			{
				SetGameState(GAMESTATE_END, 0);
			}
		}

		break;

	case GAMESTATE_END:							//終了状態
		g_nCounterGameState--;
		if (g_nCounterGameState <= 0)
		{

			//モード設定(リザルト画面に移行)
			SetFade(MODE_RESULT);				//フェードアウト
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

		//メッシュの床の描画処理
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//アニメーションの描画処理
		DrawAnimation();

		//メッシュの壁の描画処理
		//DrawMeshWall();

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュドームの描画処理
		DrawMeshDome();

		//軌跡の描画処理
		//DrawMeshOrbit();

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

		//旗の描画処理
		DrawFlag();

		//エディットの描画処理
		DrawEdit();

		//大量発生地点の描画処理
		DrawManyPoint();

		//派遣シリンダーの描画処理
		DrawDispatchPoint();

		//探索範囲の描画処理
		DrawSearchCircle();

		//抱き枕カバーの描画処理
		DrawCutepillow();

		////スコアの描画処理
		//DrawScore();

		//エディットの描画処理
		DebugEdit();

		//衝撃波の描画処理
		DrawImpactWave();

		//モデルの描画処理
		DrawModel(1);

	}

	if (nType == DRAWTYPE_UI)
	{

		//ミニマップフレームの描画処理
		DrawMinimapframe();

		if (g_gameState != GAMESTATE_START && g_gameState != GAMESTATE_BEFOREEND && g_gameState != GAMESTATE_END)
		{
			//タイムサイクルの描画処理
			DrawTimeCycle();

			//勢力ゲージの描画処理
			DrawPowerGage();

			//フィーバーゲージの描画処理
			DrawFeverGauge();

			//2Dエフェクトの描画処理
			DrawEffect_2D();

			//フィーバープッシュの描画処理
			DrawFeverPush();

			//派遣テクスチャの描画処理
			DrawDispatchTex();

			//コメントの描画処理
			DrawCommnet();

			//人数表示の背景の描画処理
			DrawGround();

			//人数表示の描画処理
			DrawPeople();

			//イベント関連の描画処理
			DrawEventWindow();

			//イベントスタンプの描画処理
			DrawEventStamp();

			//リアルタイムランキングの描画処理
			DrawRealTimeRanking();

			//大量発生のUI描画処理
			DrawManyUI();

			//警備強化のUI描画処理
			DrawManyMORS();

			//ゲーム終了前メッセージの描画処理
			DrawEndmessage();

			//ゲーム終了5秒前カウントの描画処理
			DrawEndCountDown();

			//終了の描画処理
			DrawFinish();

		}

		//カットインの描画処理
		DrawCutin();

		//上下の枠の描画処理
		DrawUpDownFram();

		//結果のメッセージの描画処理
		DrawResultMessage();

		//最初の会話の描画処理
		DrawStartTalk();

		if (g_bPause == true)
		{//ポーズメニューが使われているとき

			//ポーズ画面を表示する
			DrawPause();
		}
	}

	if (nType == DRAWTYPE_MAP)
	{
		//フィールドの描画処理
		DrawMeshField(DRAWFIELD_TYPE_MAP);

		//プレイヤーの描画処理
		DrawMiniIcon();

		//ミニマップの描画処理
		DrawMinimap();

		////マップの形描画処理
		//DrawMapShape();
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
GAMESTATE GetGameState(void)
{
	return g_gameState;
}

TARGETNOP *GetnTargetNop(void)
{
	return &g_nTargetNoP;
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
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//敵の情報取得
	Enemy *pEnemy = GetEnemy();

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//扇の当たり判定の情報を取得
	SearchFan *pSearchFan = GetSearchFan();

	//終了UIの情報取得
	FENISH *pFenish = GetFinish();

	//カメラの位置設定
	pCamera->rot = D3DXVECTOR3(0.0f, 0.0f, -0.35f);
	pCamera->fDistance = 800;	//距離

	//プレイヤーの位置設定
	pPlayer->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	SetMotisonPlayer(PLAYERMOTION_DEF);

	pFenish->bUse = false;

	//相棒の位置設定
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++, pBuddy++)
	{
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:
			pBuddy->pos = D3DXVECTOR3(pPlayer->pos.x + sinf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			pBuddy->nState = BUDDYSTATE_NONE;
			break;

		case BUDDYTYPE_DEPPA:
			pBuddy->pos = D3DXVECTOR3(pPlayer->pos.x + sinf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			pBuddy->nState = BUDDYSTATE_NONE;
			break;
		}

		SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
	}

	pBuddy -= MAX_BUDDY;

	int nCntSet = 0;

	//敵の位置設定
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bMoreSpawn == true && nCntSet < 3)
		{//使用していたら

			if (nCntSet == 0)
			{
				pEnemy->pos = D3DXVECTOR3(pPlayer->pos.x, 0.0f, pPlayer->pos.z + 300.0f);	//位置設定
				pSearchFan[pEnemy->nIdxSearchFan].bUse = false;
			}
			else if (nCntSet == 1)
			{
				pEnemy->pos = D3DXVECTOR3(pBuddy[BUDDYTYPE_HYOROGARI].pos.x, 0.0f, pBuddy[BUDDYTYPE_HYOROGARI].pos.z + 300.0f);		//位置設定
				pSearchFan[pEnemy->nIdxSearchFan].bUse = false;
			}
			else if (nCntSet == 2)
			{
				pEnemy->pos = D3DXVECTOR3(pBuddy[BUDDYTYPE_DEPPA].pos.x, 0.0f, pBuddy[BUDDYTYPE_DEPPA].pos.z + 300.0f);		//位置設定
				pSearchFan[pEnemy->nIdxSearchFan].bUse = false;
			}

			nCntSet++;

		}
	}

	//枠移動
	SetUpDownFram();

}