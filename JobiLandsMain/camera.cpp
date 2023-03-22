//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "debugproc.h"
#include "main.h"
#include "camera.h"
#include "game.h"
#include "input.h"
#include "model.h"
#include "tutorial.h"
#include "player.h"
#include "calculation.h"
#include "sound.h"

//マクロ
#define MOVE			(2.5f)		//移動量
#define MAX_LENGTH		(20000.0f)	//最大距離
#define MIN_LENGTH		(10.0f)		//最少距離
#define ROT_MOVE_MOUSE	(0.01f)		//回転移動量
#define ROT_MOVE_STICK	(0.0015f)		//回転移動量
#define ROT_MOVE		(0.025f)		//回転移動量
#define MIN_ROT			(-D3DX_PI * 0.49f)	//カメラ固定用
#define MAX_ROT			(D3DX_PI * 0.49f)	//カメラ固定用
#define BACKFOLLOW_TIME	(20)		//背面補正までの時間
#define DIS_Y			(50.0f)
#define MANY_RADIUS	(POS_MANYPOINT + 430.0f)	//大量発生SEの半径

//プロトタイプ宣言
void MoveCameraInput(int nCntCamera);
void MoveCameraStick(int nCntCamera);
void MoveCameraMouse(int nCntCamera);
void MoveCameraVR(int nCntCamera);
void MoveCameraV(int nCntCamera);
void MoveCameraR(int nCntCamera);
void MoveCameraDistance(int nCntCamera);
void SetCameraV(int nCntCamera);
void SetCameraR(int nCntCamera);
void SetCameraRot(int nCntCamera);
void MoveCameraFollow(int nCntCamera);
void UpdateManySpawnCamera(int nCntCamera);
//void LimitPosCamera(void);
//void CollisionModelCamera(void);
//void UpdateResultCamera(void);

//グローバル変数宣言
Camera g_aCamera[CAMERATYPE_MAX];	//カメラの情報

//==================================================================================
//カメラの初期化処理
//==================================================================================
void InitCamera(void)
{
	
	switch (GetMode())
	{
	case MODE_LOGO:
		InitTitleCamera();
		break;

	case MODE_TITLE:
		InitTitleCamera();
		break;

	case MODE_GAME:
		InitGameCamera();
		break;

	case MODE_RESULT:
		InitResultCamera();
		break;

	case MODE_RANKING:
		InitRankingCamera();
		break;

	}

}

//==================================================================================
//タイトルのカメラ初期化処理
//==================================================================================
void InitTitleCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < CAMERATYPE_MAX; nCntCamera++)
	{
		if (nCntCamera == CAMERATYPE_MAIN)
		{
			g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 200.0f, -0.0f);		//注視点
			g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 300.0f, g_aCamera[nCntCamera].posR.z - 200.0f);	//視点
			g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトル
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//状態
			g_aCamera[nCntCamera].fDistance = 500.0f;	//距離
			g_aCamera[nCntCamera].nMoveStep = 0;			//段階
			g_aCamera[nCntCamera].nCntStep = 0;			//段階のカウント

			g_aCamera[nCntCamera].bFollow = false;	//追従ON
			g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き

			g_aCamera[nCntCamera].bBackFollow = false;		//背面追従
			g_aCamera[nCntCamera].nCntBackFollow = 0;		//追従カウント

			g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//目標の視点
			g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//目標の注視点

			g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//目標の向き
			g_aCamera[nCntCamera].rotVDiff = 0.0f;			//目標の視点の差分

			g_aCamera[nCntCamera].viewport.X = 0;	//描画する画面の左上X座標
			g_aCamera[nCntCamera].viewport.Y = 0;	//描画する画面の左上Y座標
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//描画する画面の幅
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//描画する画面の高さ

			//視点の代入処理
			SetCameraV(nCntCamera);
		}
		
		if (nCntCamera == CAMERATYPE_UI)
		{
			g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 200.0f, 0.0f);		//注視点
			g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 300.0f, g_aCamera[nCntCamera].posR.z + -400.0f);	//視点
			g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトル
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//状態
			g_aCamera[nCntCamera].fDistance = 1500.0f;	//距離
			g_aCamera[nCntCamera].nMoveStep = 0;			//段階
			g_aCamera[nCntCamera].nCntStep = 0;			//段階のカウント

			g_aCamera[nCntCamera].bFollow = false;	//追従ON
			g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き

			g_aCamera[nCntCamera].bBackFollow = false;		//背面追従
			g_aCamera[nCntCamera].nCntBackFollow = 0;		//追従カウント

			g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//目標の視点
			g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//目標の注視点

			g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//目標の向き
			g_aCamera[nCntCamera].rotVDiff = 0.0f;			//目標の視点の差分

			g_aCamera[nCntCamera].viewport.X = 0;	//描画する画面の左上X座標
			g_aCamera[nCntCamera].viewport.Y = 0;	//描画する画面の左上Y座標
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//描画する画面の幅
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//描画する画面の高さ

																	//視点の代入処理
			SetCameraV(nCntCamera);
		}

		g_aCamera[nCntCamera].viewport.MinZ = 0.0f;
		g_aCamera[nCntCamera].viewport.MaxZ = 1.0f;
	}
}

//==================================================================================
//ランキングのカメラ初期化処理
//==================================================================================
void InitRankingCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < CAMERATYPE_MAX; nCntCamera++)
	{
		if (nCntCamera == CAMERATYPE_MAIN)
		{
			g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//注視点
			g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 300.0f, g_aCamera[nCntCamera].posR.z + -800.0f);	//視点
			g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトル
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//状態
			g_aCamera[nCntCamera].fDistance = 1500.0f;	//距離
			g_aCamera[nCntCamera].nMoveStep = 0;			//段階
			g_aCamera[nCntCamera].nCntStep = 0;			//段階のカウント

			g_aCamera[nCntCamera].bFollow = false;	//追従ON
			g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き

			g_aCamera[nCntCamera].bBackFollow = false;		//背面追従
			g_aCamera[nCntCamera].nCntBackFollow = 0;		//追従カウント

			g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//目標の視点
			g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//目標の注視点

			g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//目標の向き
			g_aCamera[nCntCamera].rotVDiff = 0.0f;			//目標の視点の差分

			g_aCamera[nCntCamera].viewport.X = 0;	//描画する画面の左上X座標
			g_aCamera[nCntCamera].viewport.Y = 0;	//描画する画面の左上Y座標
			g_aCamera[nCntCamera].viewport.Width = (DWORD)(SCREEN_WIDTH * 0.7f);	//描画する画面の幅
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//描画する画面の高さ
		}
		if (nCntCamera == CAMERATYPE_UI)
		{
			g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 100.0f, 0.0f);		//注視点
			g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 300.0f, g_aCamera[nCntCamera].posR.z + -500.0f);	//視点
			g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトル
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//状態
			g_aCamera[nCntCamera].fDistance = 1500.0f;	//距離
			g_aCamera[nCntCamera].nMoveStep = 0;			//段階
			g_aCamera[nCntCamera].nCntStep = 0;			//段階のカウント

			g_aCamera[nCntCamera].bFollow = false;	//追従ON
			g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き

			g_aCamera[nCntCamera].bBackFollow = false;		//背面追従
			g_aCamera[nCntCamera].nCntBackFollow = 0;		//追従カウント

			g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//目標の視点
			g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//目標の注視点

			g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//目標の向き
			g_aCamera[nCntCamera].rotVDiff = 0.0f;			//目標の視点の差分

			g_aCamera[nCntCamera].viewport.X = 0;	//描画する画面の左上X座標
			g_aCamera[nCntCamera].viewport.Y = 0;	//描画する画面の左上Y座標
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//描画する画面の幅
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//描画する画面の高さ
		}

		g_aCamera[nCntCamera].viewport.MinZ = 0.0f;
		g_aCamera[nCntCamera].viewport.MaxZ = 1.0f;
	}
}

//==================================================================================
//ゲームのカメラ初期化処理
//==================================================================================
void InitGameCamera(void)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	for (int nCntCamera = 0; nCntCamera < CAMERATYPE_MAX; nCntCamera++)
	{
		g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 50.0f, -500.0f);		//注視点
		g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 200.0f, g_aCamera[nCntCamera].posR.z + -400.0f);	//視点
		g_aCamera[nCntCamera].vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);		//上方向ベクトル
		g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;				//状態
		g_aCamera[nCntCamera].nMoveStep = 0;			//段階
		g_aCamera[nCntCamera].nCntStep = 0;			//段階のカウント

		g_aCamera[nCntCamera].fDistance = 800;	//距離
		g_aCamera[nCntCamera].rot = D3DXVECTOR3(0.0f, 0.0f, -0.35f);	//向き

		g_aCamera[nCntCamera].bBackFollow = false;		//背面追従
		g_aCamera[nCntCamera].nCntBackFollow = 0;		//追従カウント

		g_aCamera[nCntCamera].posVDest = g_aCamera[nCntCamera].posV;	//目標の視点
		g_aCamera[nCntCamera].posRDest = g_aCamera[nCntCamera].posR;	//目標の注視点

		g_aCamera[nCntCamera].rotVDest = g_aCamera[nCntCamera].rot;	//目標の向き
		g_aCamera[nCntCamera].rotVDiff = 0.0f;			//目標の視点の差分
		g_aCamera[nCntCamera].bFollow = true;	//追従ON

		//カメラごとの設定
		switch (nCntCamera)
		{
		case CAMERATYPE_MAIN:
			g_aCamera[nCntCamera].viewport.X = 0;	//描画する画面の左上X座標
			g_aCamera[nCntCamera].viewport.Y = 0;	//描画する画面の左上Y座標
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//描画する画面の幅
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//描画する画面の高さ
			
			g_aCamera[nCntCamera].posV.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posV.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posV.y = DIS_Y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

			g_aCamera[nCntCamera].posV.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posV.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posV.y = DIS_Y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

			//注視点の代入処理
			g_aCamera[nCntCamera].posR.x = g_aCamera[nCntCamera].posV.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posR.z = g_aCamera[nCntCamera].posV.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
			g_aCamera[nCntCamera].posR.y = g_aCamera[nCntCamera].posV.y + sinf(g_aCamera[nCntCamera].rot.z) * g_aCamera[nCntCamera].fDistance;

			//注視点の位置更新
			SetCameraR(nCntCamera);
		break;

		case CAMERATYPE_UI:
			g_aCamera[nCntCamera].viewport.X = 0;	//描画する画面の左上X座標
			g_aCamera[nCntCamera].viewport.Y = 0;	//描画する画面の左上Y座標
			g_aCamera[nCntCamera].viewport.Width = SCREEN_WIDTH;	//描画する画面の幅
			g_aCamera[nCntCamera].viewport.Height = SCREEN_HEIGHT;	//描画する画面の高さ
			break;
		}

		g_aCamera[nCntCamera].viewport.MinZ = 0.0f;
		g_aCamera[nCntCamera].viewport.MaxZ = 1.0f;
	}
}

//==================================================================================
//リザルトカメラの初期化処理
//==================================================================================
void InitResultCamera(void)
{
	g_aCamera[CAMERATYPE_MAIN].posR = D3DXVECTOR3(0.0f, 214.0f, 408.0f);		//注視点
	g_aCamera[CAMERATYPE_MAIN].posV = D3DXVECTOR3(-4.5f, 161.0f, -569.0f);		//視点
	g_aCamera[CAMERATYPE_MAIN].viewport.X = 0;	//描画する画面の左上X座標
	g_aCamera[CAMERATYPE_MAIN].viewport.Y = 0;	//描画する画面の左上Y座標
	g_aCamera[CAMERATYPE_MAIN].viewport.Width = SCREEN_WIDTH;	//描画する画面の幅
	g_aCamera[CAMERATYPE_MAIN].viewport.Height = SCREEN_HEIGHT;	//描画する画面の高さ
	g_aCamera[CAMERATYPE_MAIN].viewport.MinZ = 0.0f;
	g_aCamera[CAMERATYPE_MAIN].viewport.MaxZ = 1.0f;

}

//==================================================================================
//カメラの終了処理
//==================================================================================
void UninitCamera(void)
{

}

//==================================================================================
//カメラの更新処理
//==================================================================================
void UpdateCamera(void)
{
	for (int nCntCamera = 0; nCntCamera < CAMERATYPE_MAX; nCntCamera++)
	{
		if (nCntCamera == CAMERATYPE_MAIN)
		{//メインカメラのみ

			switch (GetMode())
			{
			case MODE_GAME:
				UpdateGameCamera(nCntCamera);
				break;

			case MODE_TUTORIAL:
				UpdateTutorialCamera(nCntCamera);
				break;

			case MODE_TITLE:
				//UpdateTitleCamera(nCntCamera);
				break;

			case MODE_RESULT:
				//テキストの描画
				PrintDebugProc(
					"---------------- カメラ情報 ----------------\n"
					"【向き】[X：%f Y：%f Z：%f]\n"
					"【距離】[%f]\n"
					"【視点】[X：%f Y：%f Z：%f]\n"
					"【注視点】[X：%f Y：%f Z：%f]\n",
					g_aCamera[nCntCamera].rot.x, g_aCamera[nCntCamera].rot.y, g_aCamera[nCntCamera].rot.z,
					g_aCamera[nCntCamera].fDistance,
					g_aCamera[nCntCamera].posV.x, g_aCamera[nCntCamera].posV.y, g_aCamera[nCntCamera].posV.z,
					g_aCamera[nCntCamera].posR.x, g_aCamera[nCntCamera].posR.y, g_aCamera[nCntCamera].posR.z);

				//UpdateGameCamera(nCntCamera);
				break;

			case MODE_RANKING:
				//UpdateGameCamera(nCntCamera);
				break;
			}
		}
	}
}

//==================================================================================
//チュートリアル時の更新処理
//==================================================================================
void UpdateTutorialCamera(int nCntCamera)
{
	//if (pCutin->bUse == false && pTutorialTalk->bUse == false)
	{//大量発生のカメラじゃなかったら && カットイン中じゃなかったら

		UpdateGameCamera(nCntCamera);
	}
}

//==================================================================================
//ゲーム時の更新
//==================================================================================
void UpdateGameCamera(int nCntCamera)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	if (g_aCamera[nCntCamera].nState == CAMERASTATE_NONE)
	{
		//視点・注視点移動
		MoveCameraFollow(nCntCamera);
		MoveCameraInput(nCntCamera);
		MoveCameraVR(nCntCamera);
		MoveCameraR(nCntCamera);
		MoveCameraV(nCntCamera);
		MoveCameraDistance(nCntCamera);
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F6) == true)
	{//F6が押された,背面追従切り替え

		g_aCamera[nCntCamera].bBackFollow = g_aCamera[nCntCamera].bBackFollow ? false : true;
	}
	if (GetKeyboardTrigger(DIK_F7) == true)
	{//F7が押された,追従切り替え

		g_aCamera[nCntCamera].bFollow = g_aCamera[nCntCamera].bFollow ? false : true;
	}
#endif

	//テキストの描画
	PrintDebugProc(
	"---------------- カメラ情報 ----------------\n"
	"【向き】[X：%f Y：%f Z：%f]\n"
	"【距離】[%f]\n"
	"【視点】[X：%f Y：%f Z：%f]\n"
	"【注視点】[X：%f Y：%f Z：%f]\n", 
		g_aCamera[nCntCamera].rot.x, g_aCamera[nCntCamera].rot.y, g_aCamera[nCntCamera].rot.z,
		g_aCamera[nCntCamera].fDistance, 
		g_aCamera[nCntCamera].posV.x, g_aCamera[nCntCamera].posV.y, g_aCamera[nCntCamera].posV.z,
		g_aCamera[nCntCamera].posR.x, g_aCamera[nCntCamera].posR.y, g_aCamera[nCntCamera].posR.z);
}

//==================================================================================
//タイトル時の更新
//==================================================================================
void UpdateTitleCamera(int nCntCamera)
{

	if (g_aCamera[nCntCamera].nMoveStep == 0)
	{
		//上に向ける
		g_aCamera[nCntCamera].rot.z += 0.04f;

		if (g_aCamera[nCntCamera].rot.z >= D3DX_PI * 0.4f)
		{//上向いたら

			g_aCamera[nCntCamera].rot.z = D3DX_PI * 0.4f;
		}
	}

	if (g_aCamera[nCntCamera].nMoveStep == 1)
	{
		g_aCamera[nCntCamera].posR = D3DXVECTOR3(0.0f, 80.0f, -200.0f);		//注視点
		g_aCamera[nCntCamera].posV = D3DXVECTOR3(0.0f, 100.0f, g_aCamera[nCntCamera].posR.z + -400.0f);	//視点
		g_aCamera[nCntCamera].fDistance = 800;	//距離

		//上に向ける
		g_aCamera[nCntCamera].rot.z -= 0.04f;

		if (g_aCamera[nCntCamera].rot.z <= -0.1f)
		{//上向いたら

			g_aCamera[nCntCamera].rot.z = -0.1f;
		}
	}

	//回転の補正
	SetCameraRot(nCntCamera);

	//注視点の位置更新
	SetCameraR(nCntCamera);
	SetCameraV(nCntCamera);
}

//==================================================================================
//入力機器のカメラ移動
//==================================================================================
void MoveCameraInput(int nCntCamera)
{
	if ((GetMode() == MODE_GAME && GetGameState() == GAMESTATE_NONE && g_aCamera[nCntCamera].nState == CAMERASTATE_NONE) ||
		GetMode() == MODE_TUTORIAL)
	{
		//マウスでの移動処理
		MoveCameraMouse(nCntCamera);

		//スティックでの移動処理
		MoveCameraStick(nCntCamera);
	}
}

//==================================================================================
//コントローラーでの移動処理
//==================================================================================
void MoveCameraStick(int nCntCamera)
{
	g_aCamera[nCntCamera].rot.y += GetStickMoveR(0).x * ROT_MOVE_STICK;
#if _DEBUG
	g_aCamera[nCntCamera].rot.z += GetStickMoveR(0).y * ROT_MOVE_STICK;
#endif

	//回転の補正
	SetCameraRot(nCntCamera);

	//視点の代入処理
	SetCameraV(nCntCamera);
}

//==================================================================================
//マウスでの移動処理
//==================================================================================
void MoveCameraMouse(int nCntCamera)
{

	#if _DEBUG

	#else NDEBUG

	#endif

	if (GetMousePress(MOUSE_BUTTON_LEFT) == true &&
		GetMousePress(MOUSE_BUTTON_RIGHT) == true)
	{//左右同時押し

#if _DEBUG
		g_aCamera[nCntCamera].move.x += (GetMouseMove().x * sinf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE) -
			(GetMouseMove().y * cosf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE);

		g_aCamera[nCntCamera].move.z += (GetMouseMove().x * cosf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE) +
			(GetMouseMove().y * sinf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE);
#endif

		//移動量補正
		MoveCameraVR(nCntCamera);

		//回転の補正
		SetCameraRot(nCntCamera);

		//注視点設定
		SetCameraR(nCntCamera);
	}
	else if (GetMousePress(MOUSE_BUTTON_LEFT) == true)
	{//左クリックしてるとき,視点回転

		g_aCamera[nCntCamera].rot.y += GetMouseMove().x * ROT_MOVE_MOUSE;

#if _DEBUG

		g_aCamera[nCntCamera].rot.z += GetMouseMove().y * ROT_MOVE_MOUSE;	
#endif

		//回転の補正
		SetCameraRot(nCntCamera);

		//視点の代入処理
		SetCameraV(nCntCamera);

	}
	else if (GetMousePress(MOUSE_BUTTON_RIGHT) == true)
	{//右クリックしてるとき,注視点回転

		g_aCamera[nCntCamera].rot.y += GetMouseMove().x * ROT_MOVE_MOUSE;

#if _DEBUG
		g_aCamera[nCntCamera].rot.z += GetMouseMove().y * ROT_MOVE_MOUSE;
#endif

		//回転の補正
		SetCameraRot(nCntCamera);

		//注視点の位置更新
		SetCameraR(nCntCamera);
	}

	//マウスホイールで距離調整
	g_aCamera[nCntCamera].fDistance += GetMouseMove().z * (MOVE * 0.3f);
	MoveCameraDistance(nCntCamera);

	//視点の代入処理
	SetCameraV(nCntCamera);
}

//==================================================================================
//カメラの視点移動
//==================================================================================
void MoveCameraV(int nCntCamera)
{

	if ((GetMode() == MODE_GAME && GetGameState() == GAMESTATE_NONE && g_aCamera[nCntCamera].nState == CAMERASTATE_NONE) ||
		GetMode() == MODE_TUTORIAL)
	{

#ifdef _DEBUG
		//視点移動
		if (GetKeyboardPress(DIK_Y) == true)
		{//Yキーが押された,視点上移動

			g_aCamera[nCntCamera].rot.z -= ROT_MOVE;
		}
		else if (GetKeyboardPress(DIK_N) == true)
		{//Nキーが押された,視点下移動

			g_aCamera[nCntCamera].rot.z += ROT_MOVE;
		}
#endif

		if (GetKeyboardPress(DIK_Z) == true/* || GetGamepadPress(BUTTON_RB, 0)*/)
		{//Zキーが押された

			g_aCamera[nCntCamera].rot.y += ROT_MOVE;
			//pPlayer->aTutorial.bLTurn = true;
		}
		if (GetKeyboardPress(DIK_C) == true/* || GetGamepadPress(BUTTON_LB, 0)*/)
		{//Cキーが押された

			g_aCamera[nCntCamera].rot.y -= ROT_MOVE;
			//pPlayer->aTutorial.bRTurn = true;

		}
	}

	//回転の補正
	SetCameraRot(nCntCamera);

	//視点の代入処理
	SetCameraV(nCntCamera);

}

//==================================================================================
//カメラの注視点移動
//==================================================================================
void MoveCameraR(int nCntCamera)
{
	if ((GetMode() == MODE_GAME && GetGameState() == GAMESTATE_NONE && g_aCamera[nCntCamera].nState == CAMERASTATE_NONE) ||
		GetMode() == MODE_TUTORIAL)
	{
		//旋回
		if (GetKeyboardPress(DIK_Q) == true)
		{//Qキーが押された,左旋回


			g_aCamera[nCntCamera].rot.y -= ROT_MOVE;

		}
		else if (GetKeyboardPress(DIK_E) == true)
		{//Eキーが押された,左旋回

			g_aCamera[nCntCamera].rot.y += ROT_MOVE;
		}

#ifdef _DEBUG
		//上昇下降
		if (GetKeyboardPress(DIK_T) == true)
		{//Tキーが押された

			g_aCamera[nCntCamera].rot.z += ROT_MOVE;

		}
		else if (GetKeyboardPress(DIK_B) == true)
		{//Bキーが押された

			g_aCamera[nCntCamera].rot.z -= ROT_MOVE;

		}
#endif
	}

	//回転の補正
	SetCameraRot(nCntCamera);

	//注視点の位置更新
	SetCameraR(nCntCamera);
}

//==================================================================================
//カメラの視点・注視点移動
//==================================================================================
void MoveCameraVR(int nCntCamera)
{

	//if (g_aCamera[nCntCamera].bFollow == false)
	//{//追従がOFFの時だけ

	//	if (GetKeyboardPress(DIK_A) == true || XGetStickPressR(BUTTON_LX, 0) < 0)
	//	{//Aキーが押された,視点左移動

	//		if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
	//		{//A+W,左上移動

	//			g_aCamera[nCntCamera].move.x += sinf(-D3DX_PI * MOVE_LRDW + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(-D3DX_PI * MOVE_LRDW + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}
	//		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
	//		{//A+S,左下移動

	//			g_aCamera[nCntCamera].move.x += sinf(-D3DX_PI * MOVE_LRUP + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(-D3DX_PI * MOVE_LRUP + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}
	//		else
	//		{//A,左移動

	//			g_aCamera[nCntCamera].move.x += sinf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(-D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}

	//	}
	//	else if (GetKeyboardPress(DIK_D) == true || XGetStickPressR(BUTTON_LX, 0) > 0)
	//	{//Dキーが押された,視点右移動

	//		if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
	//		{//D+W,右上移動

	//			g_aCamera[nCntCamera].move.x += sinf(D3DX_PI * MOVE_LRDW + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(D3DX_PI * MOVE_LRDW + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}
	//		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
	//		{//D+S,右下移動

	//			g_aCamera[nCntCamera].move.x += sinf(D3DX_PI * MOVE_LRUP + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(D3DX_PI * MOVE_LRUP + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}
	//		else
	//		{//D,右移動

	//			g_aCamera[nCntCamera].move.x += sinf(D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE;
	//			g_aCamera[nCntCamera].move.z += cosf(D3DX_PI * MOVE_LR + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		}

	//	}
	//	else if (GetKeyboardPress(DIK_W) == true || YGetStickPressR(BUTTON_LY, 0) > 0)
	//	{//Wが押された、奥移動

	//		g_aCamera[nCntCamera].move.x += sinf(g_aCamera[nCntCamera].rot.y) * MOVE;
	//		g_aCamera[nCntCamera].move.z += cosf(g_aCamera[nCntCamera].rot.y) * MOVE;
	//	}
	//	else if (GetKeyboardPress(DIK_S) == true || YGetStickPressR(BUTTON_LY, 0) < 0)
	//	{//Sが押された、手前移動

	//		g_aCamera[nCntCamera].move.x += sinf(D3DX_PI + g_aCamera[nCntCamera].rot.y) * MOVE;
	//		g_aCamera[nCntCamera].move.z += cosf(D3DX_PI + g_aCamera[nCntCamera].rot.y) * MOVE;
	//	}

	//}

	//移動量分を加算
	g_aCamera[nCntCamera].posV.x += g_aCamera[nCntCamera].move.x;
	g_aCamera[nCntCamera].posV.z += g_aCamera[nCntCamera].move.z;

	//移動量をリセット
	g_aCamera[nCntCamera].move.x = 0.0f;
	g_aCamera[nCntCamera].move.z = 0.0f;

	//注視点の位置更新
	SetCameraR(nCntCamera);
}

//==================================================================================
// 視点・注視点間距離
//==================================================================================
void MoveCameraDistance(int nCntCamera)
{

	//限界値補正
	if (g_aCamera[nCntCamera].fDistance >= MAX_LENGTH)
	{
		g_aCamera[nCntCamera].fDistance = MAX_LENGTH;
	}
	else if (g_aCamera[nCntCamera].fDistance <= MIN_LENGTH)
	{
		g_aCamera[nCntCamera].fDistance = MIN_LENGTH;
	}

}

//==================================================================================
//カメラの視点代入処理
//==================================================================================
void SetCameraV(int nCntCamera)
{

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	if (g_aCamera[nCntCamera].bFollow == false)
	{//追従しないとき

		//視点の代入処理
		g_aCamera[nCntCamera].posV.x = g_aCamera[nCntCamera].posR.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.z = g_aCamera[nCntCamera].posR.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.y = g_aCamera[nCntCamera].posR.y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;
	}
	else if(g_aCamera[nCntCamera].bFollow == true)
	{//追従ON

		float fYcamera = 0.0f;

		//視点の代入処理
		g_aCamera[nCntCamera].posVDest.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posVDest.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posVDest.y = fYcamera + DIS_Y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

		//補正する
		g_aCamera[nCntCamera].posV += (g_aCamera[nCntCamera].posVDest - g_aCamera[nCntCamera].posV) * 0.12f;
	}

}

//==================================================================================
//カメラの注視点代入処理
//==================================================================================
void SetCameraR(int nCntCamera)
{

	if (g_aCamera[nCntCamera].bFollow == false)
	{//追従しないとき

		//注視点の代入処理
		g_aCamera[nCntCamera].posR.x = g_aCamera[nCntCamera].posV.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posR.z = g_aCamera[nCntCamera].posV.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posR.y = g_aCamera[nCntCamera].posV.y + sinf(g_aCamera[nCntCamera].rot.z) * g_aCamera[nCntCamera].fDistance;
	}
	else
	{//追従ON

		//注視点の代入処理
		g_aCamera[nCntCamera].posRDest.x = g_aCamera[nCntCamera].posV.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posRDest.z = g_aCamera[nCntCamera].posV.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posRDest.y = g_aCamera[nCntCamera].posV.y + sinf(g_aCamera[nCntCamera].rot.z) * g_aCamera[nCntCamera].fDistance;

		//補正する
		g_aCamera[nCntCamera].posR += (g_aCamera[nCntCamera].posRDest - g_aCamera[nCntCamera].posR) * 0.08f;

	}

}

//==================================================================================
//カメラの回転補正
//==================================================================================
void SetCameraRot(int nCntCamera)
{
	//Y軸の補正
	if (g_aCamera[nCntCamera].rot.y > D3DX_PI)
	{
		g_aCamera[nCntCamera].rot.y += -D3DX_PI * 2.0f;
	}
	else if (g_aCamera[nCntCamera].rot.y < -D3DX_PI)
	{
		g_aCamera[nCntCamera].rot.y += D3DX_PI * 2.0f;
	}

	//Z軸の補正
	if (g_aCamera[nCntCamera].rot.z > D3DX_PI)
	{
		g_aCamera[nCntCamera].rot.z += -D3DX_PI * 2.0f;
	}
	else if (g_aCamera[nCntCamera].rot.z < -D3DX_PI)
	{
		g_aCamera[nCntCamera].rot.z += D3DX_PI * 2.0f;
	}

	//Z軸の固定
	if (g_aCamera[nCntCamera].rot.z > MAX_ROT)
	{
		g_aCamera[nCntCamera].rot.z = MAX_ROT;
	}
	else if (g_aCamera[nCntCamera].rot.z < MIN_ROT)
	{
		g_aCamera[nCntCamera].rot.z = MIN_ROT;
	}

}

//==================================================================================
//カメラの背面自動追従
//==================================================================================
void MoveCameraFollow(int nCntCamera)
{

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	if (g_aCamera[nCntCamera].bBackFollow == true)
	{//追従するとき

		g_aCamera[nCntCamera].rotVDest.y = (D3DX_PI + pPlayer->rot.y);

		//if (pPlayer->move.x > 1.0f || pPlayer->move.x < -1.0f || pPlayer->move.z > 1.0f || pPlayer->move.z < -1.0f)
		//{//移動してるとき

		//	g_aCamera[nCntCamera].nCntBackFollow = 0;	//カウントリセット
		//}
		//else
		//{
		//	g_aCamera[nCntCamera].nCntBackFollow++;	//カウント加算
		//}

		if (g_aCamera[nCntCamera].nCntBackFollow >= BACKFOLLOW_TIME)
		{//背面補正のカウントが規定値超えたら

			g_aCamera[nCntCamera].nCntBackFollow = 0;

			//補正する
			g_aCamera[nCntCamera].rotVDiff = (g_aCamera[nCntCamera].rotVDest.y - g_aCamera[nCntCamera].rot.y);

			if (g_aCamera[nCntCamera].rotVDiff > D3DX_PI)
			{
				g_aCamera[nCntCamera].rotVDiff += (-D3DX_PI * 2.0f);
			}
			else if (g_aCamera[nCntCamera].rotVDiff < -D3DX_PI)
			{
				g_aCamera[nCntCamera].rotVDiff += (D3DX_PI * 2.0f);
			}

			//差分で補正する
			g_aCamera[nCntCamera].rot.y += g_aCamera[nCntCamera].rotVDiff * 0.015f;

			if (g_aCamera[nCntCamera].rot.y > D3DX_PI)
			{
				g_aCamera[nCntCamera].rot.y += (-D3DX_PI * 2.0f);
			}
			else if (g_aCamera[nCntCamera].rot.y < -D3DX_PI)
			{
				g_aCamera[nCntCamera].rot.y += (D3DX_PI * 2.0f);
			}

			//回転の補正
			SetCameraRot(nCntCamera);

			////注視点の位置更新
			//SetCameraR(nCntCamera);

			////視点の代入処理
			//SetCameraV(nCntCamera);

			////視点の代入処理
			//g_aCamera[nCntCamera].posV.x = g_aCamera[nCntCamera].posR.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			//g_aCamera[nCntCamera].posV.z = g_aCamera[nCntCamera].posR.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
			//g_aCamera[nCntCamera].posV.y = g_aCamera[nCntCamera].posR.y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

		}
	}
}

//==================================================================================
//カメラの設定処理
//==================================================================================
void SetCamera(int nIdx)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ビューポートの設定
	pDevice->SetViewport(&g_aCamera[nIdx].viewport);

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera[nIdx].mtxProjection);

	//プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&g_aCamera[nIdx].mtxProjection,
								D3DXToRadian(45.0f),	//視野角
								(float)g_aCamera[nIdx].viewport.Width / (float)g_aCamera[nIdx].viewport.Height,	//アスペクト比
								10.0f,		//奥行きの制限
								20000.0f);	//奥行きの制限

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &g_aCamera[nIdx].mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&g_aCamera[nIdx].mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&g_aCamera[nIdx].mtxView,
		&g_aCamera[nIdx].posV,	//視点
		&g_aCamera[nIdx].posR,	//注視点
		&g_aCamera[nIdx].vecU);//上方向ベクトル

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &g_aCamera[nIdx].mtxView);

}

//==================================================================================
//カメラの情報取得
//==================================================================================
Camera *GetCamera(void)
{
	return &g_aCamera[0];
}