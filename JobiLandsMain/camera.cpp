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
#include "dispatch.h"
#include "buddy.h"
#include "player.h"
#include "calculation.h"
#include "iventsign.h"
#include "manypoint.h"
#include "serif.h"
#include "cutin.h"
#include "tutorial_talk.h"
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

			g_aCamera[nCntCamera].bSPush = false;           //S系が押されているかどうか

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
		g_aCamera[nCntCamera].bManySE = false;	//大量発生SE使わない

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

		//矢印カメラをメインと同期させる
		IventSign *pIventSign = GetIventSign();		//イベントサインの取得

		/*g_aCamera[CAMERATYPE_SIGN].fDistance = g_aCamera[CAMERATYPE_MAIN].fDistance - 500.0f;

		if (g_aCamera[CAMERATYPE_SIGN].fDistance <= 70.0f)
		{
			g_aCamera[CAMERATYPE_SIGN].fDistance = 70.0f;
		}*/
	}

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();
}

//==================================================================================
//チュートリアル時の更新処理
//==================================================================================
void UpdateTutorialCamera(int nCntCamera)
{
	//カットイン情報取得
	CUTIN *pCutin = GetCutin();

	//チュートリアルトークの情報取得
	TutorialTalk *pTutorialTalk = GetTutorialTalk();

	if (pCutin->bUse == false && pTutorialTalk->bUse == false)
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
	else if (g_aCamera[nCntCamera].nState == CAMERASTATE_MANYSPAWN)
	{//大量発生の時

		UpdateManySpawnCamera(nCntCamera);
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
//大量発生時の更新
//==================================================================================
void UpdateManySpawnCamera(int nCntCamera)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//派遣カーソルの情報取得
	DISPATCH *pDispatch = GetDispatch();

	if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
	{//Lが押された && Aボタンが離された

		//攻撃状態解除
		pBuddy[BUDDYTYPE_HYOROGARI].bATK = false;
		pBuddy[BUDDYTYPE_DEPPA].bATK = false;
		pPlayer->bATK = false;

		//モーションの設定
		SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_DEF);
		SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_DEF);
		SetMotisonPlayer(PLAYERMOTION_DEF);
	}

	if (pDispatch->bUse == true)
	{
		if (GetGamepadRelease(BUTTON_LB + pDispatch->nType, 0) || GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
		{//派遣の使用状況入れ替え

			if (pDispatch->nType == 0)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DISPATCHL);

				//セリフをセット
				//pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });
			}
			else if (pDispatch->nType == 1)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DISPATCHR);

				//セリフをセット
				//pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });
			}

			//派遣する
			SetBuddyDispatch(pDispatch->pos, pDispatch->nType);
			pDispatch->bUse = false;
			pDispatch->nType = -1;
		}
	}

	StopSound(SOUND_LABEL_BGM_HYPNOSIS);
	StopSound(SOUND_LABEL_SE_PIYOPIYO);

	if (g_aCamera[nCntCamera].nMoveStep == CAMERAMOVE_SET)
	{
		g_aCamera[nCntCamera].aMany.BeforRotV = g_aCamera[nCntCamera].rot;
		g_aCamera[nCntCamera].aMany.rotVDest.y = (D3DX_PI + g_aCamera[nCntCamera].aMany.rotVDest.y) + D3DX_PI * 0.25f;
		g_aCamera[nCntCamera].aMany.BeforPosR = g_aCamera[nCntCamera].posR;
		g_aCamera[nCntCamera].aMany.BeforPosV = g_aCamera[nCntCamera].posV;
		g_aCamera[nCntCamera].aMany.fBeforDistance = g_aCamera[nCntCamera].fDistance;

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].aMany.rotVDest.y);
	}
	else if (g_aCamera[nCntCamera].nMoveStep == CAMERAMOVE_UP)
	{
		//段階のカウント加算
		g_aCamera[nCntCamera].nCntStep++;

		//上を目標にする
		g_aCamera[nCntCamera].aMany.rotVDest.z = D3DX_PI * 1.54f;

		//補正する
		g_aCamera[nCntCamera].aMany.rotVDiff = (g_aCamera[nCntCamera].aMany.rotVDest.z - g_aCamera[nCntCamera].rot.z);

		if (g_aCamera[nCntCamera].aMany.rotVDiff > D3DX_PI)
		{
			g_aCamera[nCntCamera].aMany.rotVDiff += (-D3DX_PI * 2.0f);
		}
		else if (g_aCamera[nCntCamera].aMany.rotVDiff < -D3DX_PI)
		{
			g_aCamera[nCntCamera].aMany.rotVDiff += (D3DX_PI * 2.0f);
		}

		//差分で補正する
		g_aCamera[nCntCamera].rot.z += g_aCamera[nCntCamera].aMany.rotVDiff * 0.035f;

		if (g_aCamera[nCntCamera].rot.z > D3DX_PI)
		{
			g_aCamera[nCntCamera].rot.z += (-D3DX_PI * 2.0f);
		}
		else if (g_aCamera[nCntCamera].rot.z < -D3DX_PI)
		{
			g_aCamera[nCntCamera].rot.z += (D3DX_PI * 2.0f);
		}

		//距離も補正
		g_aCamera[nCntCamera].fDistance += (3500.0f - g_aCamera[nCntCamera].fDistance) * 0.035f;

		//回転の補正
		SetCameraRot(nCntCamera);

		//注視点の位置更新
		SetCameraR(nCntCamera);

		//視点の代入処理
		g_aCamera[nCntCamera].posV.x = g_aCamera[nCntCamera].posR.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.z = g_aCamera[nCntCamera].posR.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.y = g_aCamera[nCntCamera].posR.y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

		if (g_aCamera[nCntCamera].nCntStep >= 110)
		{
			g_aCamera[nCntCamera].nCntStep = 0;
			g_aCamera[nCntCamera].nMoveStep = CAMERAMOVE_MOVE;
		}

	}
	else if (g_aCamera[nCntCamera].nMoveStep == CAMERAMOVE_MOVE)
	{
		//段階のカウント加算
		g_aCamera[nCntCamera].nCntStep++;

		g_aCamera[nCntCamera].posR.x += (g_aCamera[nCntCamera].aMany.posRDest.x - g_aCamera[nCntCamera].posR.x) * 0.035f;
		g_aCamera[nCntCamera].posR.z += (g_aCamera[nCntCamera].aMany.posRDest.z - g_aCamera[nCntCamera].posR.z) * 0.035f;

		//視点の代入処理
		g_aCamera[nCntCamera].posV.x = g_aCamera[nCntCamera].posR.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.z = g_aCamera[nCntCamera].posR.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;

		if (g_aCamera[nCntCamera].nCntStep >= 100)
		{
			g_aCamera[nCntCamera].nCntStep = 0;
			g_aCamera[nCntCamera].nMoveStep = CAMERAMOVE_DOWN;
		}

		if (g_aCamera[nCntCamera].bManySE == true)
		{
			g_aCamera[nCntCamera].bManySE = false;	//SE使用してない状態にする
		}
	}
	else if (g_aCamera[nCntCamera].nMoveStep == CAMERAMOVE_DOWN)
	{
		if (g_aCamera[nCntCamera].bManySE == false)
		{//SE鳴らしてないとき

			//大量発生SE
			PlaySound(SOUND_LABEL_SE_MANYPOS);

			g_aCamera[nCntCamera].bManySE = true;	//SE鳴らした
		}

		//段階のカウント加算
		g_aCamera[nCntCamera].nCntStep++;

		//上を目標にする
		g_aCamera[nCntCamera].aMany.rotVDest.z = -0.45f;

		//補正する
		g_aCamera[nCntCamera].aMany.rotVDiff = (g_aCamera[nCntCamera].aMany.rotVDest.z - g_aCamera[nCntCamera].rot.z);

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].aMany.rotVDiff);

		//差分で補正する
		g_aCamera[nCntCamera].rot.z += g_aCamera[nCntCamera].aMany.rotVDiff * 0.025f;

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].rot.z);



		//補正する
		g_aCamera[nCntCamera].aMany.rotVDiff = (g_aCamera[nCntCamera].aMany.rotVDest.y - g_aCamera[nCntCamera].rot.y);

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].aMany.rotVDiff);

		//差分で補正する
		g_aCamera[nCntCamera].rot.y += g_aCamera[nCntCamera].aMany.rotVDiff * 0.025f;

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].rot.y);



		//距離補正
		g_aCamera[nCntCamera].fDistance += (1500.0f - g_aCamera[nCntCamera].fDistance) * 0.025f;

		//視点の代入処理
		g_aCamera[nCntCamera].posV.x = g_aCamera[nCntCamera].posR.x + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.z = g_aCamera[nCntCamera].posR.z + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posV.y = 100.0f + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

		if (g_aCamera[nCntCamera].nCntStep >= 270)
		{
			g_aCamera[nCntCamera].nCntStep = 0;
			g_aCamera[nCntCamera].nMoveStep = CAMERAMOVE_RETURN;
		}
	}
	else if (g_aCamera[nCntCamera].nMoveStep == CAMERAMOVE_RETURN)
	{
		//段階のカウント加算
		g_aCamera[nCntCamera].nCntStep++;

		//補正する
		g_aCamera[nCntCamera].aMany.rotVDiff = (g_aCamera[nCntCamera].aMany.BeforRotV.z - g_aCamera[nCntCamera].rot.z);

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].aMany.rotVDiff);

		//差分で補正する
		g_aCamera[nCntCamera].rot.z += g_aCamera[nCntCamera].aMany.rotVDiff * 0.04f;

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].rot.z);

		//中視点の位置を元に戻す
		g_aCamera[nCntCamera].posR.x += (g_aCamera[nCntCamera].aMany.BeforPosR.x - g_aCamera[nCntCamera].posR.x) * 0.04f;
		g_aCamera[nCntCamera].posR.z += (g_aCamera[nCntCamera].aMany.BeforPosR.z - g_aCamera[nCntCamera].posR.z) * 0.04f;
		g_aCamera[nCntCamera].posR.y += (g_aCamera[nCntCamera].aMany.BeforPosR.y - g_aCamera[nCntCamera].posR.y) * 0.04f;


		//補正する
		g_aCamera[nCntCamera].aMany.rotVDiff = (g_aCamera[nCntCamera].aMany.BeforRotV.y - g_aCamera[nCntCamera].rot.y);

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].aMany.rotVDiff);

		//差分で補正する
		g_aCamera[nCntCamera].rot.y += g_aCamera[nCntCamera].aMany.rotVDiff * 0.04f;

		//角度の正規化
		RotNormalize(&g_aCamera[nCntCamera].rot.y);


		//距離補正
		g_aCamera[nCntCamera].fDistance += (g_aCamera[nCntCamera].aMany.fBeforDistance - g_aCamera[nCntCamera].fDistance) * 0.04f;

		//視点の代入処理
		g_aCamera[nCntCamera].posV.x += (g_aCamera[nCntCamera].aMany.BeforPosV.x - g_aCamera[nCntCamera].posV.x) * 0.04f;
		g_aCamera[nCntCamera].posV.z += (g_aCamera[nCntCamera].aMany.BeforPosV.z - g_aCamera[nCntCamera].posV.z) * 0.04f;
		g_aCamera[nCntCamera].posV.y += (g_aCamera[nCntCamera].aMany.BeforPosV.y - g_aCamera[nCntCamera].posV.y) * 0.04f;

		if (g_aCamera[nCntCamera].nCntStep >= 150)
		{
			g_aCamera[nCntCamera].nCntStep = 0;
			g_aCamera[nCntCamera].nMoveStep = CAMERAMOVE_SET;
			g_aCamera[nCntCamera].nState = CAMERASTATE_NONE;
			g_aCamera[nCntCamera].move = D3DXVECTOR3(0.0f,  0.0f, 0.0f);
		}
	}
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

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();
	pBuddy++;

	//群衆の情報取得
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

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

		
		g_aCamera[nCntCamera].fDistance = 800.0f;

		if (pCrowdInfo->nCrowdValue >= 1 && pCrowdInfo->nCrowdValue < 3)
		{//群衆の数が2以上かつ3未満だったら
			g_aCamera[nCntCamera].fDistance = 900.0f;
		}

		if (pCrowdInfo->nCrowdValue >= 3 && pCrowdInfo->nCrowdValue < 7)
		{//群衆の数が3以上かつ5未満だったら
			g_aCamera[nCntCamera].fDistance = 1000.0f;
		}

		if (pCrowdInfo->nCrowdValue >= 7 && pCrowdInfo->nCrowdValue < 15)
		{//群衆の数が5以上かつ9未満だったら
			g_aCamera[nCntCamera].fDistance = 1100.0f;
		}

		if (pCrowdInfo->nCrowdValue >= 15 && pCrowdInfo->nCrowdValue < 31)
		{//群衆の数が9以上かつ17未満だったら
			g_aCamera[nCntCamera].fDistance = 1200.0f;
		}

		if (pCrowdInfo->nCrowdValue >= 31 && pCrowdInfo->nCrowdValue < 63)
		{//群衆の数が17以上かつ33未満だったら
			g_aCamera[nCntCamera].fDistance = 1300.0f;
		}

		if (pCrowdInfo->nCrowdValue >= 63)
		{//群衆の数が33以上かつ65未満だったら

			int nNum = pCrowdInfo->nParentPlayerNum - 63;
			int nCntFormation = 1;
			while (1)
			{
				nNum -= 49;

				if (nNum > 0)
				{//0より大きかったら

					nCntFormation++;
				}
				else
				{//0以下で抜ける

					break;
				}
			}

			g_aCamera[nCntCamera].fDistance = 1300.0f + (nCntFormation * 100);
		}

		//if (g_aCamera[nCntCamera].bBackFollow == true)
		//{
		//	g_aCamera[nCntCamera].rotVDest.y = (D3DX_PI + pPlayer->rot.y);

		//	//補正する
		//	g_aCamera[nCntCamera].rotVDiff = (g_aCamera[nCntCamera].rotVDest.y - g_aCamera[nCntCamera].rot.y);

		//	if (g_aCamera[nCntCamera].rotVDiff > D3DX_PI)
		//	{
		//		g_aCamera[nCntCamera].rotVDiff += (-D3DX_PI * 2.0f);
		//	}
		//	else if (g_aCamera[nCntCamera].rotVDiff < -D3DX_PI)
		//	{
		//		g_aCamera[nCntCamera].rotVDiff += (D3DX_PI * 2.0f);
		//	}

		//	//差分で補正する
		//	g_aCamera[nCntCamera].rot.y += g_aCamera[nCntCamera].rotVDiff * 0.025f;

		//	if (g_aCamera[nCntCamera].rot.y > D3DX_PI)
		//	{
		//		g_aCamera[nCntCamera].rot.y += (-D3DX_PI * 2.0f);
		//	}
		//	else if (g_aCamera[nCntCamera].rot.y < -D3DX_PI)
		//	{
		//		g_aCamera[nCntCamera].rot.y += (D3DX_PI * 2.0f);
		//	}
		//}

#if 1
		//視点の代入処理
		g_aCamera[nCntCamera].posVDest.x = (pPlayer->pos.x + sinf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posVDest.z = (pPlayer->pos.z + cosf(D3DX_PI + pPlayer->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posVDest.y = fYcamera + DIS_Y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;
#else
		//視点の代入処理
		g_aCamera[nCntCamera].posVDest.x = (pBuddy->pos.x + sinf(D3DX_PI + pBuddy->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * sinf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posVDest.z = (pBuddy->pos.z + cosf(D3DX_PI + pBuddy->rot.y) * 90.0f) + cosf(g_aCamera[nCntCamera].rot.z) * cosf(g_aCamera[nCntCamera].rot.y) * -g_aCamera[nCntCamera].fDistance;
		g_aCamera[nCntCamera].posVDest.y = fYcamera + DIS_Y + sinf(g_aCamera[nCntCamera].rot.z) * -g_aCamera[nCntCamera].fDistance;

#endif
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

		if (g_aCamera[nCntCamera].bSPush == false)
		{//S系が押されてない

			g_aCamera[nCntCamera].nCntBackFollow = BACKFOLLOW_TIME;
		}
		else
		{//S系が押されてるとき

			//g_aCamera[nCntCamera].nCntBackFollow++;	//カウント加算
		}
		

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