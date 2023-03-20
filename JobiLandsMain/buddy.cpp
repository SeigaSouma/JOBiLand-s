//=============================================================================
//
// 相棒処理 [buddy.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "buddy.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "player.h"
#include "meshwall.h"
#include "game.h"
#include "edit.h"
#include "impactwave.h"
#include "calculation.h"
#include "hypnosis_gauge.h"
#include "rotate.h"
#include "crowd.h"
#include "shadow.h"
#include "cutepillow.h"
#include "minimap_icon.h"
#include "particle.h"
#include "tutorial.h"
#include "title.h"
#include "sound.h"

//マクロ定義
#define RETURN_MOVE		(2.5f)
#define THROW_COOLTIME	(90)	//投げたときのクールタイム
#define MOVE_DEF	(70)		//移動変更カウンターの初期値
#define MOVE_MAX	(100)		//移動変更カウンターに足す最大値
#define MOVE_MIN	(50)		//移動変更カウンターに足す最小値
#define ROT_DEF		(100)		//向きカウンターの初期値
#define ROT_MAX		(100)		//向きカウンターに足す最大値
#define ROT_MIN		(50)		//向きカウンターに足す最小値
#define STOP_CNT	(200)		//止まるカウンターを出す間隔
#define MOVE_DISTANCE	(1000.0f)	//移動範囲
#define RANGE_DISTANCE	(280.0f)	//派遣のモブ発見範囲
#define ADD_DISTANCE	(2.2f)		//止まってるときの発見範囲
#define CHASE_TIME		(60 * 5)	//追いかける時間
#define NOTCHASE_TIME	(60 * 2)	//追いかけない時間
#define ESCAPE_DISTANCE	(SEARCH_RADIUS + 400.0f)		//逃げ判定の範囲
#define ESCAPE_TIME		(60 * 1)	//逃げる時間
#define BUDDY1_TXT	"data\\TEXT\\motion_set_buddy1.txt"
#define BUDDY2_TXT	"data\\TEXT\\motion_set_buddy2.txt"

//プロトタイプ宣言
void ReadSetBuddy(void);
void ReadSetMotionBuddy(void);
void UpdateMotionBuddy(int nCntBuddy);
void LimitPosBuddy(int nCntBuddy);
void ControllBuddy(int nCntBuddy);
void ControllTutorialBuddy(int nCntBuddy);
void CollisionCharBuddy(int nCntBuddy);
void CollisionWallBuddy(int nCntBuddy);
void CollisionATKBuddy(int nCntBuddy, float fDistance, float fRadius, float fAngle, int nValue);
void ThrowPillowBuddy(int nCntBuddy);
void UpdateStateBuddy(int nCntBuddy);
void RotCorrect(int nCntBuddy);
void UpdateStateMove(int nCntBuddy);
void MoveCount(int nCntBuddy);
void CollisionChaseBuddy(int nCntBuddy);
void UpdateMoveBuddy(int nCntBuddy);
void CalFormationPosBuddy(int nCntBuddy);
void CollisionEscapeBuddy(int nCntBuddy);
int SetHypnosisWaveBuddy(int nCntBuddy, float fRot, float fWidthMove);
void UpdateGameEndBuddy(int nCntBuddy);

//グローバル変数宣言
Buddy g_aBuddy[MAX_BUDDY];		//相棒の情報

//==================================================================================
//相棒の初期化処理
//==================================================================================
void InitBuddy(void)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各要素初期化
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;

		case BUDDYTYPE_DEPPA:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;
		}

		if (GetMode() == MODE_GAME)
		{
			g_aBuddy[nCntBuddy].rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);		//向き
		}
		else if (GetMode() == MODE_RANKING)
		{
			g_aBuddy[nCntBuddy].rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);		//向き
		}
		else
		{
			g_aBuddy[nCntBuddy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		}

		//初期位置設定
		if (GetMode() != MODE_TITLE && GetMode() != MODE_RESULT)
		{//タイトルとリザルト以外

			g_aBuddy[nCntBuddy].pos = g_aBuddy[nCntBuddy].formationPos;		//現在の位置
			g_aBuddy[nCntBuddy].posOld = g_aBuddy[nCntBuddy].formationPos;		//前回の位置
			g_aBuddy[nCntBuddy].bUse = true;	//使用している状況にする
		}
		else if (GetMode() == MODE_TITLE)
		{
			switch (nCntBuddy)
			{
			case BUDDYTYPE_HYOROGARI:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);
				break;

			case BUDDYTYPE_DEPPA:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);
				break;
			}
			g_aBuddy[nCntBuddy].bUse = false;	//使用している状況にする
		}
		else if (GetMode() == MODE_RESULT)
		{
			switch (nCntBuddy)
			{
			case BUDDYTYPE_HYOROGARI:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(-160.0f, 76.0f, -30.0f);
				break;

			case BUDDYTYPE_DEPPA:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(140.0f, 76.0f, -30.0f);
				break;
			}
		}
		if (GetMode() == MODE_RANKING)
		{
			switch (nCntBuddy)
			{
			case BUDDYTYPE_HYOROGARI:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);

				//中心からD3DX_PI * -0.5fの方向に-150.0f
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(sinf(D3DX_PI * -0.25f + g_aBuddy[nCntBuddy].rot.y) * -150.0f, 0.0f, cosf(D3DX_PI * -0.25f + g_aBuddy[nCntBuddy].rot.y) * -150.0f);
				break;

			case BUDDYTYPE_DEPPA:
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(-100.0f, 0.0f, 0.0f);

				//中心からD3DX_PI * -0.5fの方向に-150.0f
				g_aBuddy[nCntBuddy].pos = D3DXVECTOR3(sinf(D3DX_PI * -0.75f + g_aBuddy[nCntBuddy].rot.y) * -150.0f, 0.0f, cosf(D3DX_PI * -0.75f + g_aBuddy[nCntBuddy].rot.y) * -150.0f);
				break;
			}
		}

		g_aBuddy[nCntBuddy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_aBuddy[nCntBuddy].mtxWorld = {};	//ワールドマトリックス
		g_aBuddy[nCntBuddy].bFever = false;	//フィーバー			//フィーバー状態
		g_aBuddy[nCntBuddy].fRotDiff = 0.0f;			//向きの差分
		g_aBuddy[nCntBuddy].fRotDest = g_aBuddy[nCntBuddy].rot.y;			//向きの差分
		g_aBuddy[nCntBuddy].nIdxShadow = -1;			//影のインデックス番号
		g_aBuddy[nCntBuddy].nIdxIcon = -1;			//ミニマップアイコンのインデックス番号

		for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
		{
			g_aBuddy[nCntBuddy].nIdxWave[nCntWave] = -1;			//衝撃波のインデックス番号
		}
		
		g_aBuddy[nCntBuddy].nState = 0;				//状態
		g_aBuddy[nCntBuddy].nCntState = 0;			//状態カウント
		g_aBuddy[nCntBuddy].nCntChase = 0;			//追い掛けカウント
		g_aBuddy[nCntBuddy].bCollisionModel = true;	//モデルとの判定を取るかどうか

		//モーション系
		g_aBuddy[nCntBuddy]. nNowMotionNum = 0;		//現在のモーション番号
		g_aBuddy[nCntBuddy]. nCntFrame = 0;			//フレームのカウント
		g_aBuddy[nCntBuddy]. nPatternKey = 0;		//何個目のキーか
		g_aBuddy[nCntBuddy]. nPartsNum = 0;			//パーツ数
		g_aBuddy[nCntBuddy]. bMove = false;				//移動しているか
		g_aBuddy[nCntBuddy]. bATK = false;				//攻撃してるか
		g_aBuddy[nCntBuddy].nCntAllFrame = 0;		//総フレームカウント

		g_aBuddy[nCntBuddy].nCntWalk = 0;			//移動のカウント
		g_aBuddy[nCntBuddy].nCntPenlight = 0;		//催眠のカウント
		g_aBuddy[nCntBuddy].fRadius = 50.0f;	//半径

		g_aBuddy[nCntBuddy].fThrowRadius = 240.0f;	//投げる範囲
		g_aBuddy[nCntBuddy].nThrowCnt = 0;			//投げるカウント

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			g_aBuddy[nCntBuddy].aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
			g_aBuddy[nCntBuddy].aModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
			g_aBuddy[nCntBuddy].aModel[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
			g_aBuddy[nCntBuddy].aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
			g_aBuddy[nCntBuddy].aModel[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
			g_aBuddy[nCntBuddy].aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
			g_aBuddy[nCntBuddy].aModel[nCntModel].mtxWorld = {};	//ワールドマトリックス
			g_aBuddy[nCntBuddy].aModel[nCntModel].OldmtxWorld = {};	//ワールドマトリックス


			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBuddy[nCntBuddy].aModel[nCntModel].mtxWorld);
			D3DXMatrixIdentity(&g_aBuddy[nCntBuddy].aModel[nCntModel].OldmtxWorld);

			g_aBuddy[nCntBuddy].aModel[nCntModel].nIdxShadow = -1;			//影のインデックス番号
			g_aBuddy[nCntBuddy].aModel[nCntModel].nIdxRotate = -1;	//ぐるぐるのインデックス番号
			g_aBuddy[nCntBuddy].aModel[nCntModel].nIdxHypGauge = -1;		//体力ゲージのインデックス番号
			g_aBuddy[nCntBuddy].aModel[nCntModel].nShadow = 1;			//影を使うかどうか
			g_aBuddy[nCntBuddy].aModel[nCntModel].nFever = -1;

			g_aBuddy[nCntBuddy].aModel[nCntModel].nNumVtx = 0;			//頂点数
			g_aBuddy[nCntBuddy].aModel[nCntModel].dwSizeFVF = NULL;		//頂点フォーマットのサイズ
			g_aBuddy[nCntBuddy].aModel[nCntModel].pVtxBuff = NULL;			//頂点バッファのポインタ
			g_aBuddy[nCntBuddy].aModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
			g_aBuddy[nCntBuddy].aModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

			g_aBuddy[nCntBuddy].aModel[nCntModel].nType = 0;			//読み込むモデルの種類
			g_aBuddy[nCntBuddy].aModel[nCntModel].nParent = -1;		//親の番号
			g_aBuddy[nCntBuddy].aModel[nCntModel].bUse = false;			//使用しているか
			g_aBuddy[nCntBuddy].aModel[nCntModel].nState = 0;			//状態

			g_aBuddy[nCntBuddy].aModel[nCntModel].pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
			g_aBuddy[nCntBuddy].aModel[nCntModel].pBuffMat = NULL;		//マテリアルへのポインタ

			for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
			{
				g_aBuddy[nCntBuddy].aModel[nCntModel].pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
				g_aBuddy[nCntBuddy].aModel[nCntModel].pMatData[nCntMat].pTextureFilename = {};	//マテリアルのデータ
				g_aBuddy[nCntBuddy].aModel[nCntModel].acFilename[nCntMat] = {};	//ファイル名
			}
			g_aBuddy[nCntBuddy].aModel[nCntModel].dwNumMat = NULL;			//マテリアルの数

			for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
			{
				g_aBuddy[nCntBuddy].aModel[nCntModel].pTexture[nCntTex] = NULL;	//テクスチャへのポインタ
			}

		}

		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
		{//モーションの数繰り返す

			g_aBuddy[nCntBuddy].aMotionData[nCntMotion].bUse = false;	//使用しているか
			g_aBuddy[nCntBuddy].aMotionData[nCntMotion].nLoop = MOTION_LOOP_ON;	//ループ判定
			g_aBuddy[nCntBuddy].aMotionData[nCntMotion].nNumKey = 0;				//キーの数

			for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
			{//キーの数繰り返す

				g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].nFrame = 0;	//再生フレーム

				for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
				{//パーツの数繰り返す

					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の位置
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の差分
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の向き
					g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向きの差分
				}
			}
		}

		g_aBuddy[nCntBuddy].aMoveAI.nRotCounter = 0;			//向き変更カウンター
		g_aBuddy[nCntBuddy].aMoveAI.nMoveCounter = MOVE_DEF + MOVE_MAX;		//移動変更カウンター
		g_aBuddy[nCntBuddy].aMoveAI.nRot = 1;					//向きの+-
		g_aBuddy[nCntBuddy].aMoveAI.nMoveState = MOVEAI_NONE;	//移動状態
		g_aBuddy[nCntBuddy].aMoveAI.nCntStop = 0;				//止まっている状態を出しやすくするカウンター
		g_aBuddy[nCntBuddy].aMoveAI.nChaseCounter = 0;			//何秒追いかけてるか
		g_aBuddy[nCntBuddy].aMoveAI.nNotChaseCounter = NOTCHASE_TIME;		//追いかけるまでの時間
		g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter = 0;			//逃げる時間

		g_aBuddy[nCntBuddy].aMoveAI.fDistance = 0.0f;			//距離

		g_aBuddy[nCntBuddy].aMoveAI.bPosDest = false;				//目的地についたか
		g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;				//モブを発見したかどうか
		g_aBuddy[nCntBuddy].aMoveAI.bChaseTime = false;			//追いかけるまでの時間を足すか
		g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;				//ペンライトを使ってるか
		g_aBuddy[nCntBuddy].aMoveAI.bEscape = false;				//逃げるか

		for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
		{
			g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] = false;	//何番目のモブを見つけたか
			g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = false;	//ペンライトの範囲に入っているか
		}

		for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
		{
			g_aBuddy[nCntBuddy].aMoveAI.bEscapeNum[nCntEnemy] = false;	//何番目の敵か
		}

		for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
		{
			g_aBuddy[nCntBuddy].aFormation[nCntCrowd].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//隊列の位置
			g_aBuddy[nCntBuddy].aFormation[nCntCrowd].nIndex = -1;			//隊列のインデックス番号
			g_aBuddy[nCntBuddy].aFormation[nCntCrowd].bUse = false;			//隊列を使用しているか
		}
	}


	//外部ファイル読み込み
	ReadSetBuddy();
	ReadSetMotionBuddy();
}

//==================================================================================
//相棒の終了処理
//==================================================================================
void UninitBuddy(void)
{
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		for (int nCount = 0; nCount < MAX_MODEL; nCount++)
		{
			for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
			{
				//テクスチャの破棄
				if (g_aBuddy[nCntBuddy].aModel[nCount].pTexture[nCntTex] != NULL)
				{
					g_aBuddy[nCntBuddy].aModel[nCount].pTexture[nCntTex]->Release();
					g_aBuddy[nCntBuddy].aModel[nCount].pTexture[nCntTex] = NULL;
				}
			}

			//メッシュの破棄
			if (g_aBuddy[nCntBuddy].aModel[nCount].pMesh != NULL)
			{
				g_aBuddy[nCntBuddy].aModel[nCount].pMesh->Release();
				g_aBuddy[nCntBuddy].aModel[nCount].pMesh = NULL;
			}

			//マテリアルの破棄
			if (g_aBuddy[nCntBuddy].aModel[nCount].pBuffMat != NULL)
			{
				g_aBuddy[nCntBuddy].aModel[nCount].pBuffMat->Release();
				g_aBuddy[nCntBuddy].aModel[nCount].pBuffMat = NULL;
			}
		}
	}
}

//==================================================================================
//相棒の更新処理
//==================================================================================
void UpdateBuddy(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//モブの情報取得
	Mob *pMob = GetMob();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++, pMob++)
	{
		if (g_aBuddy[nCntBuddy].bUse == true)
		{//使用していたら

			//過去の位置保存
			g_aBuddy[nCntBuddy].posOld = g_aBuddy[nCntBuddy].pos;

			//相棒の操作
			if (GetMode() == MODE_GAME && GetGameState() != GAMESTATE_BEFOREEND)
			{
				ControllBuddy(nCntBuddy);
			}
			else if (GetMode() == MODE_GAME && GetGameState() == GAMESTATE_BEFOREEND)
			{
				UpdateGameEndBuddy(nCntBuddy);
			}
			else if (GetMode() == MODE_TUTORIAL)
			{
				ControllTutorialBuddy(nCntBuddy);
			}

			//隊列の位置計算
			CalFormationPosBuddy(nCntBuddy);

			//抱き枕カバー投げ処理
			if (GetMode() == MODE_GAME || (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_END))
			{
				ThrowPillowBuddy(nCntBuddy);
			}

			if (g_aBuddy[nCntBuddy].bMove == true)
			{//移動してるとき

				g_aBuddy[nCntBuddy].nCntWalk++;	//カウント加算

				if (g_aBuddy[nCntBuddy].nCntWalk > 15)
				{
					g_aBuddy[nCntBuddy].nCntWalk = 0;

					//歩行パーティクルの設定
					SetParticle(g_aBuddy[nCntBuddy].pos, PARTICLE_TYPE_WALK);
				}


				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_WALK &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION &&
					g_aBuddy[nCntBuddy].bFever == false)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_WALK);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERWALK &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION && g_aBuddy[nCntBuddy].bFever == true)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERWALK);
				}
			}
			else if (g_aBuddy[nCntBuddy].bATK == true)
			{//攻撃してるとき

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION && g_aBuddy[nCntBuddy].bFever == false)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_ACTION);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION && g_aBuddy[nCntBuddy].bFever == true)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERACTION);
				}
			}
			else
			{//ニュートラルモーション

				if (g_aBuddy[nCntBuddy].bATK == false &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_DEF && 
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_KYOROKYORO &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION &&
					g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION &&
					g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH ||
					(g_aBuddy[nCntBuddy].aMoveAI.bPenUse == false && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH && g_aBuddy[nCntBuddy].aMoveAI.nMoveState != MOVEAI_STOP) 
					|| g_aBuddy[nCntBuddy].aMoveAI.nMoveState == MOVEAI_ESCAPE)
				{
					//攻撃状態解除
					g_aBuddy[nCntBuddy].bATK = false;

					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
				}
			}

			if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_ACTION || 
				g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION || 
				g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERACTION ||
				g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERMOVEACTION)
			{//催眠アクション中

				if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION &&
					pPlayer->nNowMotionNum != PLAYERMOTION_MOVEACTION ||
					g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERMOVEACTION &&
					pPlayer->nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
				{//移動催眠中 && プレイヤーが移動催眠をやめていたら

					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
				}

				g_aBuddy[nCntBuddy].nCntPenlight++;

				if (g_aBuddy[nCntBuddy].nCntPenlight == 20 || g_aBuddy[nCntBuddy].nCntPenlight == 40)
				{
					if (g_aBuddy[nCntBuddy].nCntPenlight >= 40)
					{
						g_aBuddy[nCntBuddy].nCntPenlight = 0;
					}

					//催眠のホワンホワン設定
					if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_ACTION)
					{//止まって催眠

						if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
						{
							g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						}
						else
						{
							g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * (-0.25f + (0.5f * nCntBuddy)), (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						}
					}
					else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION)
					{//動いて催眠

						if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
						{

							g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
						}
						else
						{

							g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * (-0.25f + (0.5f * nCntBuddy)), (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
						}
					}
					else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERACTION)
					{//フィーバー中の催眠

						g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
					}
					else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERMOVEACTION)
					{//フィーバー中の催眠

						g_aBuddy[nCntBuddy].nIdxWave[0] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
					}

					if (g_aBuddy[nCntBuddy].bFever == true)
					{//フィーバー中なら

						//催眠のホワンホワン設定
						if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERACTION)
						{//止まって催眠

							g_aBuddy[nCntBuddy].nIdxWave[1] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
							g_aBuddy[nCntBuddy].nIdxWave[2] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
							g_aBuddy[nCntBuddy].nIdxWave[3] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						}
						else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_FEVERMOVEACTION)
						{//動いて催眠

							g_aBuddy[nCntBuddy].nIdxWave[1] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
							g_aBuddy[nCntBuddy].nIdxWave[2] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
							g_aBuddy[nCntBuddy].nIdxWave[3] = SetHypnosisWaveBuddy(nCntBuddy, g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						}
					}
				}
			}
			else if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION &&
				g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION &&
				g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION &&
				g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION)
			{

				//攻撃状態解除
				g_aBuddy[nCntBuddy].bATK = false;
			}

			//モーションの更新
			UpdateMotionBuddy(nCntBuddy);

			//キャラクターとの当たり判定
			CollisionCharBuddy(nCntBuddy);

			//壁との当たり判定
			//CollisionWall(&g_aBuddy[nCntBuddy].pos, g_aBuddy[nCntBuddy].posOld);

			//座標制限
			LimitPosBuddy(nCntBuddy);

			//状態更新
			UpdateStateBuddy(nCntBuddy);

			switch (g_aBuddy[nCntBuddy].nNowMotionNum)
			{
			case BUDDYMOTION_ACTION:

				CollisionATKBuddy(nCntBuddy, PENLIGHT_RADIUS, PENLIGHT_RADIUS, PENLIGHT_ANGLE, STOP_HYPNOSIS);
				break;


			case BUDDYMOTION_MOVEACTION:

				CollisionATKBuddy(nCntBuddy, PENLIGHT_RADIUS_MOVE, PENLIGHT_RADIUS, PENLIGHT_ANGLE_MOVE, MOVE_HYPNOSIS);
				break;

			case BUDDYMOTION_FEVERACTION:

				CollisionATKBuddy(nCntBuddy, PENLIGHT_RADIUS, PENLIGHT_RADIUS, PENLIGHT_ANGLE, STOP_FEVER_HYPNOSIS);
				break;

			case BUDDYMOTION_FEVERMOVEACTION:

				CollisionATKBuddy(nCntBuddy, PENLIGHT_RADIUS_MOVE, PENLIGHT_RADIUS, PENLIGHT_ANGLE_MOVE, MOVE_FEVER_HYPNOSIS);
				break;
			}

			//影の位置更新
			SetPositionShadow(g_aBuddy[nCntBuddy].nIdxShadow, g_aBuddy[nCntBuddy].pos);

			//アイコンの位置更新
			UpdatePositionIcon(g_aBuddy[nCntBuddy].nIdxIcon, g_aBuddy[nCntBuddy].pos);

			//衝撃波の位置更新
			if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION)
			{//移動催眠アクション中

				for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
				{
					SetPositionImpactWave(g_aBuddy[nCntBuddy].nIdxWave[nCntWave], g_aBuddy[nCntBuddy].move);
				}
			}
		}
	}
}

//==================================================================================
//モデルの操作
//==================================================================================
void ControllBuddy(int nCntBuddy)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//if (GetGameState() == GAMESTATE_NONE)
	{//何もしていない状態

		if (pPlayer->nNowMotionNum != PLAYERMOTION_ACTION && g_aBuddy[nCntBuddy].aMoveAI.bPosDest == false)
		{//アクションしていないとき && 目的地についてないとき

			if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (BUDDY_DISTANCE * BUDDY_DISTANCE))
			{//まだ規定値に着いていなかったら

				//移動量加算
				if (g_aBuddy[nCntBuddy].nState != BUDDYSTATE_RETURN)
				{//帰還状態じゃないとき

					g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
					g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
				}
				else if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_RETURN)
				{//帰還状態のとき

					g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * RETURN_MOVE;
					g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * RETURN_MOVE;
				}
			}
		}

		if ((GetKeyboardPress(DIK_L) == true || GetGamepadPress(BUTTON_A, 0)) &&
			g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH && GetGameState() == GAMESTATE_NONE)
		{//Lが押された || ペンライトが使わてれいるとき

			//攻撃してる状態にする
			g_aBuddy[nCntBuddy].bATK = true;

			if (g_aBuddy[nCntBuddy].move.x <= 1.0f && g_aBuddy[nCntBuddy].move.x >= -1.0f &&
				g_aBuddy[nCntBuddy].move.z <= 1.0f && g_aBuddy[nCntBuddy].move.z >= -1.0f)
			{//止まっているときは強力

				g_aBuddy[nCntBuddy].bMove = false;
				g_aBuddy[nCntBuddy].move.x = 0.0f;
				g_aBuddy[nCntBuddy].move.z = 0.0f;

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION && g_aBuddy[nCntBuddy].bFever == false)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_ACTION);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION && g_aBuddy[nCntBuddy].bFever == true)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERACTION);
				}
			}
			else
			{//動いてるときは弱

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION && g_aBuddy[nCntBuddy].bFever == false)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_MOVEACTION);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION && g_aBuddy[nCntBuddy].bFever == true/* && g_aBuddy[nCntBuddy].bATK == false*/)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERMOVEACTION);
				}
			}
		}

		if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
		{//Lが押された && Aボタンが離された

			//攻撃状態解除
			g_aBuddy[nCntBuddy].bATK = false;

			//モーションの設定
			SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
		}

		if (g_aBuddy[nCntBuddy].aMoveAI.bPenUse == true)
		{//ペンライトが使わてれいるとき

			//攻撃してる状態にする
			g_aBuddy[nCntBuddy].bATK = true;
			g_aBuddy[nCntBuddy].move.x = 0.0f;
			g_aBuddy[nCntBuddy].move.z = 0.0f;
		}
	}

	if (g_aBuddy[nCntBuddy].move.x >= 1.0f || g_aBuddy[nCntBuddy].move.x <= -1.0f || g_aBuddy[nCntBuddy].move.z >= 1.0f || g_aBuddy[nCntBuddy].move.z <= -1.0f)
	{//移動してるとき

		g_aBuddy[nCntBuddy].bMove = true;
	}
	else
	{//移動してないとき

		g_aBuddy[nCntBuddy].bMove = false;

		//if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION || g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_ACTION)
		//{//移動催眠中の時

		//	//攻撃状態解除
		//	g_aBuddy[nCntBuddy].bATK = false;
		//}
	}

	//重力処理
	g_aBuddy[nCntBuddy].move.y -= 1.0f;
	g_aBuddy[nCntBuddy].move.y = 0.0f;

	if (g_aBuddy[nCntBuddy].bFever == true || (g_aBuddy[nCntBuddy].bCollisionModel == false && g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH))
	{//フィーバー中なら

		//加速
		g_aBuddy[nCntBuddy].move.x *= 1.1f;
		g_aBuddy[nCntBuddy].move.z *= 1.1f;
	}

	//位置更新
	g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

	//慣性つける
	if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION)
	{
		g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
		g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;
	}
	else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION)
	{
		g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR_HYPNOSIS;
		g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR_HYPNOSIS;
	}

}

//==================================================================================
//相棒の座標制限
//==================================================================================
void LimitPosBuddy(int nCntBuddy)
{
	if (g_aBuddy[nCntBuddy].pos.y < 0.0f)
	{//地面より下に行ったら

		g_aBuddy[nCntBuddy].pos.y = 0.0f;

		//重力処理
		g_aBuddy[nCntBuddy].move.y = 0.0f;
	}

	//移動制限
	if (GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_END)
	{
		if (g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].fRadius <= -LIMIT_POS && g_aBuddy[nCntBuddy].posOld.x - g_aBuddy[nCntBuddy].fRadius >= -LIMIT_POS)
		{//今回が-1000以下, 前回が-1000以上

			g_aBuddy[nCntBuddy].pos.x = -LIMIT_POS + g_aBuddy[nCntBuddy].fRadius;
		}
		if (g_aBuddy[nCntBuddy].pos.x + g_aBuddy[nCntBuddy].fRadius >= LIMIT_POS && g_aBuddy[nCntBuddy].posOld.x + g_aBuddy[nCntBuddy].fRadius <= LIMIT_POS)
		{//今回が1000以上, 前回が1000以下

			g_aBuddy[nCntBuddy].pos.x = LIMIT_POS - g_aBuddy[nCntBuddy].fRadius;
		}
		if (g_aBuddy[nCntBuddy].pos.z + g_aBuddy[nCntBuddy].fRadius >= LIMIT_POS && g_aBuddy[nCntBuddy].posOld.z + g_aBuddy[nCntBuddy].fRadius <= LIMIT_POS)
		{//今回が1000以上, 前回が1000以下

			g_aBuddy[nCntBuddy].pos.z = LIMIT_POS - g_aBuddy[nCntBuddy].fRadius;
		}
		if (g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].fRadius <= -LIMIT_POS && g_aBuddy[nCntBuddy].posOld.z - g_aBuddy[nCntBuddy].fRadius >= -LIMIT_POS)
		{//今回が-1000以下, 前回が-1000以上

			g_aBuddy[nCntBuddy].pos.z = -LIMIT_POS + g_aBuddy[nCntBuddy].fRadius;
		}
	}

}

//==================================================================================
//相棒の隊列計算処理
//==================================================================================
void CalFormationPosBuddy(int nCntBuddy)
{
	//群衆の情報を取得
	Crowd *pCrowd = GetCrowd();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	for (int nCntCrowd = 0, nCntformation = 0; nCntCrowd < MAX_CROWD; nCntformation++)
	{
		//横の人数
		int nWidth = 2 + nCntCrowd;

		if (nWidth >= 50)
		{
			nWidth = 50;
		}

		//横の人数分繰り返し
		for (int nCntWidth = 0; nCntWidth < nWidth; nCntWidth++)
		{
			//向きの設定
			float fRotWidth = 2.0f / (float)(nWidth - 1);
			float fRot = fRotWidth * nCntWidth + (D3DX_PI * MOVE_LRDW);

			if (nCntCrowd + nCntWidth < MAX_CROWD)
			{//最大値以下

				g_aBuddy[nCntBuddy].aFormation[nCntCrowd + nCntWidth].pos =
					D3DXVECTOR3
					(
						g_aBuddy[nCntBuddy].pos.x + sinf(-D3DX_PI * MOVE_LR + fRot + g_aBuddy[nCntBuddy].rot.y) * (FORMATION_DISTANCE * (nCntformation + 1)),
						0.0f,
						g_aBuddy[nCntBuddy].pos.z + cosf(-D3DX_PI * MOVE_LR + fRot + g_aBuddy[nCntBuddy].rot.y) * (FORMATION_DISTANCE * (nCntformation + 1)));
			}
			else
			{
				break;
			}
		}

		nCntCrowd += (nWidth - 1);
	}
}

//==================================================================================
//相棒の隊列計算処理
//==================================================================================
int SetFormationPosBuddy(int nCntBuddy)
{
	int nCntIdx = 0;

	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aBuddy[nCntBuddy].aFormation[nCntCrowd].bUse == false)
		{//使用していなかったら

			//インデックス番号を代入
			nCntIdx = nCntCrowd;

			g_aBuddy[nCntBuddy].aFormation[nCntCrowd].bUse = true;
			break;
		}
	}

	return nCntIdx;
}

//==================================================================================
//催眠中のホワンホワン設定処理
//==================================================================================
int SetHypnosisWaveBuddy(int nCntBuddy, float fRot, float fWidthMove)
{
	//催眠のホワンホワン
	int nIdxWave = SetImpactWave
	(
		D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x, g_aBuddy[nCntBuddy].pos.y + 50.0f, g_aBuddy[nCntBuddy].pos.z),
		D3DXVECTOR3(PENLIGHT_RADIUS / (float)40, 0.0f, PENLIGHT_RADIUS / (float)40),
		D3DXVECTOR3(D3DX_PI * 0.5f, fRot, 0.0f),
		D3DXCOLOR(1.0f, 0.3f, 1.0f, 0.8f),
		40.0f,	//幅
		15.0f,	//高さ
		40,		//寿命
		fWidthMove,	//幅の移動量
		INPACTWAVE_TYPE_PINK		//テクスチャタイプ
	);

	return nIdxWave;
}

//==================================================================================
//相棒の抱き枕カバー投げ処理
//==================================================================================
void ThrowPillowBuddy(int nCntBuddy)
{
	//モブの情報取得
	Mob *pMob = GetMob();

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if ((pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS && g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_NONE) || 
			(pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH))
		{//使用されている && クールタイムが終わってる && ペンラ中じゃない && 何もしてない ||
		 //使用されている && クールタイムが終わってる && 派遣状態

			if (CircleRange(g_aBuddy[nCntBuddy].pos, pMob->aModel.pos) <= (g_aBuddy[nCntBuddy].fThrowRadius * g_aBuddy[nCntBuddy].fThrowRadius))
			{//探索サークルの中に入ったら

				if (g_aBuddy[nCntBuddy].nThrowCnt >= 1)
				{//クールタイム中だったら

					g_aBuddy[nCntBuddy].nThrowCnt = (g_aBuddy[nCntBuddy].nThrowCnt + 1) % THROW_COOLTIME;
				}

				if (g_aBuddy[nCntBuddy].nThrowCnt <= 0)
				{
					//抱き枕カバーの向きを求める
					float fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - pMob->aModel.pos.x), (g_aBuddy[nCntBuddy].pos.z - pMob->aModel.pos.z));

					//抱き枕の設定処理
					if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
					{//派遣中は自分達が親

						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(0.0f, fRotDest, 0.0f), 1 + nCntBuddy);
					}
					else
					{//ついて行ってるときはプレイヤーが親

						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(0.0f, fRotDest, 0.0f), PARENT_PLAYER);
					}

					if (g_aBuddy[nCntBuddy].bFever == true)
					{//フィーバー中なら

						//全方位抱き枕
						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(g_aBuddy[nCntBuddy].rot.x, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f, g_aBuddy[nCntBuddy].rot.z), PILLOWPARENT_PLAYER);
						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(g_aBuddy[nCntBuddy].rot.x, g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f, g_aBuddy[nCntBuddy].rot.z), PILLOWPARENT_PLAYER);
						SetCutepillow(g_aBuddy[nCntBuddy].pos, D3DXVECTOR3(g_aBuddy[nCntBuddy].rot.x, g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f, g_aBuddy[nCntBuddy].rot.z), PILLOWPARENT_PLAYER);
					}

					//クールタイム設定
					g_aBuddy[nCntBuddy].nThrowCnt = 1;
				}
				break;
			}
		}
	}
}

//==================================================================================
//相棒の攻撃
//==================================================================================
void CollisionATKBuddy(int nCntBuddy, float fDistance, float fRadius, float fAngle, int nValue)
{
	//モブの情報取得
	Mob *pMob = GetMob();

	bool bPenLight = false, bSearch = false;

	//ペンライトの攻撃
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState != MOBSTATE_APPIEAR)
		{//モブが使用されていたら

			//ペンライトの当たり判定
			bSearch = CollisionFan(g_aBuddy[nCntBuddy].pos,
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f), g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f)),
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f), g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f)),
				pMob->aModel.pos, pMob->aModel.posOld);


			//ペンライトの当たり判定
			bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(fAngle + g_aBuddy[nCntBuddy].rot.y) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(fAngle + g_aBuddy[nCntBuddy].rot.y) * -fDistance),
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-fAngle + g_aBuddy[nCntBuddy].rot.y) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-fAngle + g_aBuddy[nCntBuddy].rot.y) * -fDistance),
				pMob->aModel.pos, pMob->aModel.posOld);

			if (g_aBuddy[nCntBuddy].bFever == true)
			{//フィーバー中なら

				if (bPenLight == false)
				{//まだ当たっていなかったら

					bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f) * -fDistance),
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.5f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}

				if (bPenLight == false)
				{//まだ当たっていなかったら

					bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f) * -fDistance),
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-fAngle + g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 1.0f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}

				if (bPenLight == false)
				{//まだ当たっていなかったら

					bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(fAngle + g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(fAngle + g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f) * -fDistance),
						D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-fAngle + g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f) * -fDistance, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-fAngle + g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.5f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}
			}


			if (bPenLight == true)
			{//扇の範囲に入ったら

				if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
				{//派遣状態

					//モブを発見していない状態にする
					g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

					//ペンライトを使用している状態にする
					g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = true;
					g_aBuddy[nCntBuddy].aMoveAI.bPenUse = true;

				}

				//催眠度が減少するまでのカウンターを初期化
				pMob->nSubHypCounter = HYP_CNTTIME;

				//モブのヒット処理
				HitMob(nCntMob, nValue);
			}

			if (bPenLight == false && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
			{//扇の範囲に入ってない && ペンライトを使っている && 派遣状態

				 //ペンライトを使用していない状態にする
				g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = false;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;
			}
		}
		else if (pMob->aModel.bUse == false && pMob->aModel.nState != MOBSTATE_APPIEAR)
		{
			if (bPenLight == false && g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] == true && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
			{//扇の範囲に入ってない && ペンライトを使っている && 派遣状態

			 //ペンライトを使用していない状態にする
				g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = false;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;
			}
		}
	}

	if (bSearch == false && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH/* && g_aBuddy[nCntBuddy].bATK == true*/)
	{//扇の範囲に入ってない && ペンライトを使っている && 派遣状態

	 //ペンライトを使用していない状態にする
		g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;
	}
}

//==================================================================================
//壁との当たり判定
//==================================================================================
void CollisionWallBuddy(int nCntBuddy)
{
	//壁の情報取得
	MESHWALL *pMeshWall = GetMeshWall();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pMeshWall++)
	{//壁の数分繰り返す

		if (pMeshWall->bUse == true)
		{//壁が使用されているとき

			//壁の幅
			float fLen = (pMeshWall->nWidth * POS_MESHWALL) * 0.5f;

			//左の頂点座標
			D3DXVECTOR3 pos0 = D3DXVECTOR3(
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * -fLen,
				g_aBuddy[nCntBuddy].pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * -fLen);

			//右の頂点座標
			D3DXVECTOR3 pos1 = D3DXVECTOR3(
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen,
				g_aBuddy[nCntBuddy].pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen);

			//境界線のベクトル
			D3DXVECTOR3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			//相棒の境界線のベクトル
			D3DXVECTOR3 vecLineBuddy;
			vecLineBuddy.x = g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].posOld.x;
			vecLineBuddy.z = g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].posOld.z;

			//相棒と壁のベクトル
			D3DXVECTOR3 vecToPosBuddy;
			vecToPosBuddy.x = pos1.x - g_aBuddy[nCntBuddy].posOld.x;
			vecToPosBuddy.z = pos1.z - g_aBuddy[nCntBuddy].posOld.z;

			//面積の最大値
			float fMaxAreaBuddy = (vecLineBuddy.z * vecLine.x) - (vecLineBuddy.x * vecLine.z);

			//今回の面積
			float fNowAreaBuddy = (vecToPosBuddy.z * vecLine.x) - (vecToPosBuddy.x * vecLine.z);

			//割合
			float fRateBuddy = fNowAreaBuddy / fMaxAreaBuddy;

			if (fRateBuddy >= 0.0f && fRateBuddy <= 1.0f)
			{//面積の範囲内にいたら判定

				if ((vecLineBuddy.z * vecToPosBuddy.x) - (vecLineBuddy.x * vecToPosBuddy.z) > 0)
				{//壁に当たったら

					//交点からはみ出た分
					D3DXVECTOR3 CollisionPointBuddy = D3DXVECTOR3(0.0f, g_aBuddy[nCntBuddy].pos.y, 0.0f);
					CollisionPointBuddy.x = (g_aBuddy[nCntBuddy].pos.x + (vecLineBuddy.x * (fRateBuddy - 1.0f)));
					CollisionPointBuddy.z = (g_aBuddy[nCntBuddy].pos.z + (vecLineBuddy.z * (fRateBuddy - 1.0f)));

					//法線ベクトル(境界線ベクトルのXとZ反転)
					D3DXVECTOR3 vecNor = D3DXVECTOR3(vecLine.z, 0.0f, -vecLine.x);

					//ベクトルの正規化
					D3DXVec3Normalize(&vecNor, &vecNor);

					//相棒の逆移動量
					D3DXVECTOR3 BuddyInverceMove;
					BuddyInverceMove.z = vecLineBuddy.z * (fRateBuddy - 1.0f);
					BuddyInverceMove.x = vecLineBuddy.x * (fRateBuddy - 1.0f);

					//内積(壁の法線と相棒の逆移動量)
					float fDot = (BuddyInverceMove.x * vecNor.x) + (BuddyInverceMove.z * vecNor.z);

					//壁ずり移動量
					D3DXVECTOR3 MoveWall = vecNor * fDot;

					D3DXVECTOR3 ppp = (CollisionPointBuddy - g_aBuddy[nCntBuddy].pos);
					D3DXVECTOR3 pppp = ppp + MoveWall;

					//ぶつかった点に補正
					g_aBuddy[nCntBuddy].pos += MoveWall + (vecNor * 0.1f);

				}
			}
		}
	}
}

//==================================================================================
//相棒の状態更新
//==================================================================================
void UpdateStateBuddy(int nCntBuddy)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	bool fRange = false;		//モブが範囲に入ったかの判定

	switch (g_aBuddy[nCntBuddy].nState)
	{
	case BUDDYSTATE_NONE:

		//目的地についてない状態にする
		g_aBuddy[nCntBuddy].aMoveAI.bPosDest = false;

		//モブを発見してない状態にする
		g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

		//ペンライトを使用していない状態にする
		g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;

		//隊列の位置設定
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;

		case BUDDYTYPE_DEPPA:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;
		}

		if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) < (BUDDY_DISTANCE * BUDDY_DISTANCE))
		{//目的地に着いた時

			g_aBuddy[nCntBuddy].bCollisionModel = true;
			g_aBuddy[nCntBuddy].nCntChase = 0;
		}

		break;

	case BUDDYSTATE_DISPATCH:

		if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) < (BUDDY_DISTANCE * BUDDY_DISTANCE))
		{//目的地についたとき

			//目的地についた状態にする
			g_aBuddy[nCntBuddy].aMoveAI.bPosDest = true;
		}

		if (g_aBuddy[nCntBuddy].aMoveAI.bPosDest == true)
		{//目的地についた状態のとき

			//MORSから逃げる判定
			CollisionEscapeBuddy(nCntBuddy);

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION)
			{//攻撃モーションじゃないとき

				if (g_aBuddy[nCntBuddy].aMoveAI.bMobDisc == false && g_aBuddy[nCntBuddy].aMoveAI.bEscape == false)
				{//モブを発見してないとき && 逃げてないとき

					//移動カウンター更新
					MoveCount(nCntBuddy);
				}

				if (g_aBuddy[nCntBuddy].aMoveAI.nMoveState != MOVEAI_STOP)
				{//止まる状態じゃないとき

					//移動の更新
					UpdateMoveBuddy(nCntBuddy);
				}
				else if(g_aBuddy[nCntBuddy].aMoveAI.nMoveState == MOVEAI_STOP)
				{//きょろきょろしてる時

					if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_KYOROKYORO)
					{
						//モーションの設定
						SetMotisonBuddy(nCntBuddy, BUDDYMOTION_KYOROKYORO);
					}
				}

				if (g_aBuddy[nCntBuddy].aMoveAI.bChaseTime == true)
				{//追いかけるまでのカウントするとき

					//追いかけるまでの時間
					g_aBuddy[nCntBuddy].aMoveAI.nNotChaseCounter++;
				}

				if (g_aBuddy[nCntBuddy].aMoveAI.bEscape == false)
				{
					//モブ追いかけ判定
					CollisionChaseBuddy(nCntBuddy);

				}

				//移動の状態更新
				UpdateStateMove(nCntBuddy);

			}

		}

		break;

	case BUDDYSTATE_RETURN:

		//目的地についてない状態にする
		g_aBuddy[nCntBuddy].aMoveAI.bPosDest = false;

		//モブを発見してない状態にする
		g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

		//ペンライトを使用していない状態にする
		g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;

		//隊列の位置設定
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;

		case BUDDYTYPE_DEPPA:
			g_aBuddy[nCntBuddy].formationPos = D3DXVECTOR3(pPlayer->pos.x + sinf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE, 0.0f, pPlayer->pos.z + cosf(-DIS_ROT + pPlayer->rot.y) * BUDDY_DISTANCE);
			break;
		}

		if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) <= (BUDDY_DISTANCE * BUDDY_DISTANCE))
		{//プレイヤーのところに着いていたら

			g_aBuddy[nCntBuddy].nState = BUDDYSTATE_NONE;

			if (nCntBuddy == BUDDYTYPE_HYOROGARI)
			{
				//SEをセット
				PlaySound(SOUND_LABEL_SE_ONMINO);
			}

			if (nCntBuddy == BUDDYTYPE_DEPPA)
			{
				//SEをセット
				PlaySound(SOUND_LABEL_SE_ORANGEKUN);
			}
			
		}

		break;
	}

	if (((GetMode() == MODE_GAME && GetGameState() != GAMESTATE_BEFOREEND) || GetMode() == MODE_TUTORIAL) &&
		CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (BUDDY_DISTANCE * BUDDY_DISTANCE) && g_aBuddy[nCntBuddy].aMoveAI.bPosDest == false)
	{//まだ目的地についてないとき

		//目標の角度を求める
		g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].formationPos.x), (g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].formationPos.z));
		
		//角度の補正
		RotCorrect(nCntBuddy);
	}
}

//==================================================================================
//移動のカウンター更新処理
//==================================================================================
void MoveCount(int nCntBuddy)
{
	//移動変更カウンターを足す
	g_aBuddy[nCntBuddy].aMoveAI.nMoveCounter++;
	g_aBuddy[nCntBuddy].aMoveAI.nCntStop++;


	if (g_aBuddy[nCntBuddy].aMoveAI.nMoveCounter >= MOVE_DEF + rand() % MOVE_MAX + MOVE_MIN)
	{//一定時間経ったら

		if (g_aBuddy[nCntBuddy].aMoveAI.nCntStop >= STOP_CNT)
		{
			//止まっている状態にする
			g_aBuddy[nCntBuddy].aMoveAI.nMoveState = MOVEAI_STOP;

			g_aBuddy[nCntBuddy].aMoveAI.nCntStop = 0;
		}
		else
		{
			//移動状態を変更する
			g_aBuddy[nCntBuddy].aMoveAI.nMoveState = rand() % MOVEAI_STOP + 1;
		}

		g_aBuddy[nCntBuddy].aMoveAI.nMoveCounter = 0;

	}


	//向きの+-変更カウンター
	g_aBuddy[nCntBuddy].aMoveAI.nRotCounter++;

	if (g_aBuddy[nCntBuddy].aMoveAI.nRotCounter >= ROT_DEF + rand() % ROT_MAX + ROT_MIN)
	{//一定時間経ったら

		//向きを真逆にする
		g_aBuddy[nCntBuddy].aMoveAI.nRot *= -1;

		//向き+-変更カウンターを初期化
		g_aBuddy[nCntBuddy].aMoveAI.nRotCounter = 0;
	}
}

//==================================================================================
//移動の状態更新処理
//==================================================================================
void UpdateStateMove(int nCntBuddy)
{

	//モブの情報取得
	Mob *pMob = GetMob();

	bool bSearch = false;

	//ペンライトの攻撃
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState != MOBSTATE_APPIEAR && g_aBuddy[nCntBuddy].aMoveAI.bEscape == false)
		{//モブが使用されていたら

			//ペンライトの当たり判定
			bSearch = CollisionFan(g_aBuddy[nCntBuddy].pos,
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f), g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f)),
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f), g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -(RANGE_DISTANCE * 0.5f)),
				pMob->aModel.pos, pMob->aModel.posOld);

			//ペンライトの当たり判定
			bool bPenLight = CollisionFan(g_aBuddy[nCntBuddy].pos,
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -RANGE_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -RANGE_DISTANCE),
				D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -RANGE_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(-PENLIGHT_ANGLE + g_aBuddy[nCntBuddy].rot.y) * -RANGE_DISTANCE),
				pMob->aModel.pos, pMob->aModel.posOld);

			if (bSearch == true)
			{//扇の範囲に入ったら

				//モブを発見していない状態にする
				g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

				//ペンライトを使用している状態にする
				g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = true;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = true;
			}

			
			if (bPenLight == false && g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] == true && g_aBuddy[nCntBuddy].nState == BUDDYSTATE_DISPATCH)
			{//扇の範囲に入ってない && ペンライトを使っている && 派遣状態

				//ペンライトを使用していない状態にする
				g_aBuddy[nCntBuddy].aMoveAI.bPenRange[nCntMob] = false;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;
			}
			
		}
	}

	switch (g_aBuddy[nCntBuddy].aMoveAI.nMoveState)
	{
	case MOVEAI_WIDTH:			//横移動

		g_aBuddy[nCntBuddy].fRotDest = 0.5f * D3DX_PI * g_aBuddy[nCntBuddy].aMoveAI.nRot;

		break;

	case MOVEAI_HEIGHT:			//縦移動

		if (g_aBuddy[nCntBuddy].aMoveAI.nRot == -1)
		{
			//奥
			g_aBuddy[nCntBuddy].fRotDest = 1.0f * D3DX_PI;
		}
		else if (g_aBuddy[nCntBuddy].aMoveAI.nRot == 1)
		{
			//手前
			g_aBuddy[nCntBuddy].fRotDest = 0.0f * D3DX_PI;
		}

		break;

	case MOVEAI_DIAGONAL_FORM:		//斜め移動(奥)

		g_aBuddy[nCntBuddy].fRotDest = 0.25f * D3DX_PI * g_aBuddy[nCntBuddy].aMoveAI.nRot;

		break;

	case MOVEAI_DIAGONAL_BACK:		//斜め移動(手前)

		g_aBuddy[nCntBuddy].fRotDest = 0.75f * D3DX_PI * g_aBuddy[nCntBuddy].aMoveAI.nRot;

		break;

	case MOVEAI_STOP:				//向きだけ（移動無し）

		//rotに直接代入
		g_aBuddy[nCntBuddy].rot.y += 0.01f * D3DX_PI * g_aBuddy[nCntBuddy].aMoveAI.nRot;

		break;

	case MOVEAI_CHASE:		//追いかけ状態

		if (g_aBuddy[nCntBuddy].aMoveAI.bMobDisc == true)
		{//モブを発見してるとき

			//追いかけカウンター
			g_aBuddy[nCntBuddy].aMoveAI.nChaseCounter++;
		}

		if (g_aBuddy[nCntBuddy].aMoveAI.nChaseCounter >= CHASE_TIME)
		{
			//モブを発見してない状態にする
			g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = false;

			//カウンター初期化
			g_aBuddy[nCntBuddy].aMoveAI.nChaseCounter = 0;
		}
		break;

	case MOVEAI_ESCAPE:				//逃げ状態

		if (g_aBuddy[nCntBuddy].aMoveAI.bEscape == true)
		{
			g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter++;
		}

		break;
	}

}

//==================================================================================
//移動の更新処理
//==================================================================================
void UpdateMoveBuddy(int nCntBuddy)
{
	if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (MOVE_DISTANCE * MOVE_DISTANCE))
	{//移動範囲外の時

		//目標の角度を求める
		g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].formationPos.x), (g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].formationPos.z));
	}

	//角度の補正
	RotCorrect(nCntBuddy);

	//ニート自動移動
	g_aBuddy[nCntBuddy].move.x += sinf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI) * CHARA_MOVE;
	g_aBuddy[nCntBuddy].move.z += cosf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI) * CHARA_MOVE;
}

//==================================================================================
//角度の補正
//==================================================================================
void RotCorrect(int nCntBuddy)
{
	//目標の角度を求める
	g_aBuddy[nCntBuddy].fRotDiff = g_aBuddy[nCntBuddy].fRotDest - g_aBuddy[nCntBuddy].rot.y;

	//角度の正規化
	RotNormalize(&g_aBuddy[nCntBuddy].fRotDiff);

	//角度の補正をする
	g_aBuddy[nCntBuddy].rot.y += g_aBuddy[nCntBuddy].fRotDiff * 0.2f;

	//角度の正規化
	RotNormalize(&g_aBuddy[nCntBuddy].rot.y);
}

//==================================================================================
//派遣のモブ追いかけ判定
//==================================================================================
void CollisionChaseBuddy(int nCntBuddy)
{
	//モブの情報取得
	Mob *pMob = GetMob();

	bool fRange = false;

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState != MOBSTATE_APPIEAR)
		{//モブが使用されていたら

			if (g_aBuddy[nCntBuddy].aMoveAI.nMoveState == MOVEAI_STOP)
			{//止まってるとき

				//扇の当たり判定
				fRange = CollisionFan(g_aBuddy[nCntBuddy].pos,
					D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.25f) * -RANGE_DISTANCE * ADD_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.25f) * -RANGE_DISTANCE * ADD_DISTANCE),
					D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.25f) * -RANGE_DISTANCE * ADD_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.25f) * -RANGE_DISTANCE * ADD_DISTANCE),
					pMob->aModel.pos, pMob->aModel.posOld);

			}
			else if (g_aBuddy[nCntBuddy].aMoveAI.nMoveState != MOVEAI_STOP)
			{//移動してるとき

				//扇の当たり判定
				fRange = CollisionFan(g_aBuddy[nCntBuddy].pos,
					D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.25f) * -RANGE_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI * 0.25f) * -RANGE_DISTANCE),
					D3DXVECTOR3(g_aBuddy[nCntBuddy].pos.x + sinf(g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.25f) * -RANGE_DISTANCE, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z + cosf(g_aBuddy[nCntBuddy].rot.y + -D3DX_PI * 0.25f) * -RANGE_DISTANCE),
					pMob->aModel.pos, pMob->aModel.posOld);

			}


			if (g_aBuddy[nCntBuddy].aMoveAI.bMobDisc == false && g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] == true)
			{//モブを発見してない && この番号のモブを発見してる状態

				//このモブを発見してない状態にする
				g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] = false;

				//追いかけるまでのカウントする
				g_aBuddy[nCntBuddy].aMoveAI.bChaseTime = true;

			}
			else if (fRange == true && g_aBuddy[nCntBuddy].aMoveAI.bMobDisc == false && pMob->aModel.nState != MOBSTATE_APPIEAR && pMob->nHypnosis < 10 && g_aBuddy[nCntBuddy].aMoveAI.nNotChaseCounter >= NOTCHASE_TIME)
			{//扇の範囲に入っているとき && モブを発見してないとき && モブが出現状態じゃないとき && 一定時間経ったとき

				//モブを発見した状態にする
				g_aBuddy[nCntBuddy].aMoveAI.bMobDisc = true;

				//この番号のモブを発見した状態にする
				g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] = true;

				//追いかけ状態にする
				g_aBuddy[nCntBuddy].aMoveAI.nMoveState = MOVEAI_CHASE;

				//追いかけるまでのカウントしない
				g_aBuddy[nCntBuddy].aMoveAI.bChaseTime = false;

				//カウント初期化
				g_aBuddy[nCntBuddy].aMoveAI.nNotChaseCounter = 0;

			}

			if (g_aBuddy[nCntBuddy].aMoveAI.bMobNum[nCntMob] == true)
			{//発見したモブの場合

				//目標の角度を求める
				g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - pMob->aModel.pos.x), (g_aBuddy[nCntBuddy].pos.z - pMob->aModel.pos.z));

			}
		}
	}
}

//==================================================================================
//派遣のMORSから逃げる判定
//==================================================================================
void CollisionEscapeBuddy(int nCntBuddy)
{
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bMoreSpawn == true)
		{//使用していたら

			if (CircleRange(g_aBuddy[nCntBuddy].pos, pEnemy->pos) < (ESCAPE_DISTANCE * ESCAPE_DISTANCE))
			{//範囲内にMORSがいたら

				//逃げ状態にする
				g_aBuddy[nCntBuddy].aMoveAI.nMoveState = MOVEAI_ESCAPE;

				//逃げる
				g_aBuddy[nCntBuddy].aMoveAI.bEscape = true;
				g_aBuddy[nCntBuddy].aMoveAI.bEscapeNum[nCntEnemy] = true;
				g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter = 0;

				//ペンライトを使用しない状態にする
				g_aBuddy[nCntBuddy].bATK = false;
				g_aBuddy[nCntBuddy].aMoveAI.bPenUse = false;

				//MORSと真反対に目標地点設定
				g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - pEnemy->pos.x), (g_aBuddy[nCntBuddy].pos.z - pEnemy->pos.z)) + D3DX_PI;
			}
			else if (g_aBuddy[nCntBuddy].aMoveAI.bEscapeNum[nCntEnemy] == true && g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter >= ESCAPE_TIME)
			{

				//逃げる
				g_aBuddy[nCntBuddy].aMoveAI.bEscape = false;
				g_aBuddy[nCntBuddy].aMoveAI.bEscapeNum[nCntEnemy] = false;

				g_aBuddy[nCntBuddy].aMoveAI.nEscapeCounter = 0;

			}
		}
	}

}

//==================================================================================
//キャラクターとの当たり判定
//==================================================================================
void CollisionCharBuddy(int nCntBuddy)
{
	//敵の情報取得
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bMoreSpawn == true)
		{//敵が使用されていたら

			//キャラクター同士の当たり判定
			CollisionCharacter(
				&g_aBuddy[nCntBuddy].pos, &g_aBuddy[nCntBuddy].posOld, D3DXVECTOR3(g_aBuddy[nCntBuddy].fRadius, 0.0f, g_aBuddy[nCntBuddy].fRadius), D3DXVECTOR3(-g_aBuddy[nCntBuddy].fRadius, 0.0f, -g_aBuddy[nCntBuddy].fRadius),
				&pEnemy->pos, D3DXVECTOR3(pEnemy->fRadius, 0.0f, pEnemy->fRadius), D3DXVECTOR3(-pEnemy->fRadius, 0.0f, -pEnemy->fRadius));
		}
	}

	
	//モデルの情報取得
	ModelModel *pModel = GetModel();

	//当たったかの判定
	bool bHit = false;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true && pModel->nType < SMALL_NUM && g_aBuddy[nCntBuddy].nState != BUDDYSTATE_RETURN && g_aBuddy[nCntBuddy].bCollisionModel == true)
		{//モデルが使用されていたら && 帰還状態じゃないとき && 判定を取る時

			//当たっていたら
			if (bHitCharacter(
				&g_aBuddy[nCntBuddy].pos, &g_aBuddy[nCntBuddy].posOld, D3DXVECTOR3(g_aBuddy[nCntBuddy].fRadius, 0.0f, g_aBuddy[nCntBuddy].fRadius), D3DXVECTOR3(-g_aBuddy[nCntBuddy].fRadius, 0.0f, -g_aBuddy[nCntBuddy].fRadius),
				&pModel->posOrigin, pModel->vtxMax, pModel->vtxMin) == true)
			{
				bHit = true;
			}

			//キャラクター同士の当たり判定
			CollisionCharacter(
				&g_aBuddy[nCntBuddy].pos, &g_aBuddy[nCntBuddy].posOld, D3DXVECTOR3(g_aBuddy[nCntBuddy].fRadius, 0.0f, g_aBuddy[nCntBuddy].fRadius), D3DXVECTOR3(-g_aBuddy[nCntBuddy].fRadius, 0.0f, -g_aBuddy[nCntBuddy].fRadius),
				&pModel->posOrigin, pModel->vtxMax, pModel->vtxMin);
		}
	}

	//当たっていたら
	if (bHit == true)
	{
		//追従カウントを加算
		g_aBuddy[nCntBuddy].nCntChase++;
	}
	else
	{
		//追従カウントを減算
		g_aBuddy[nCntBuddy].nCntChase--;

		if (g_aBuddy[nCntBuddy].nCntChase <= 0)
		{
			g_aBuddy[nCntBuddy].nCntChase = 0;
		}
	}

	//追従カウントに達していたら
	if (g_aBuddy[nCntBuddy].nCntChase > 60)
	{
		g_aBuddy[nCntBuddy].nCntChase = 60;
		g_aBuddy[nCntBuddy].bCollisionModel = false;
	}
}

//==================================================================================
//相棒のモーション処理
//==================================================================================
void SetMotisonBuddy(int nCntBuddy, int nMotionType)
{

	//if (g_aBuddy[nCntBuddy].nNowMotionNum != nMotionType)
	//{

	//	for (int nCntKey = 0; nCntKey < g_aBuddy[nCntBuddy].aMotionData[nMotionType].nNumKey; nCntKey++)
	//	{//キー数分繰り返す

	//		for (int nCntParts = 0; nCntParts < g_aBuddy[nCntBuddy].nPartsNum; nCntParts++)
	//		{//パーツ分繰り返す

	//			g_aBuddy[nCntBuddy].aModel[nCntParts].rot = g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[nCntKey].aParts[nCntParts].rot;
	//			g_aBuddy[nCntBuddy].aModel[nCntParts].pos = g_aBuddy[nCntBuddy].aModel[nCntParts].posOrigin;
	//		}

	//		g_aBuddy[nCntBuddy].nCntFrame = 0;	//フレームカウントリセット
	//	}

	//	//今のモーションの更新
	//	g_aBuddy[nCntBuddy].nNowMotionNum = nMotionType;

	//	g_aBuddy[nCntBuddy].nPatternKey = 0;	//現在のキー番号リセット
	//	g_aBuddy[nCntBuddy].nCntAllFrame = 0;	//総フレーム数リセット
	//}


	//for (int nCntParts = 0; nCntParts < g_aBuddy[nCntBuddy].nPartsNum; nCntParts++)
	//{//パーツ分繰り返す

	//	//元の向きと現在の向きで目標の向きを求める
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDest = g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot;
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].posDest = g_aBuddy[nCntBuddy].aModel[nCntParts].posOrigin + g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos;

	//	//現在と目標の差分を求める
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff = g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDest - g_aBuddy[nCntBuddy].aModel[nCntParts].rot;
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].posDiff = g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].posDest - g_aBuddy[nCntBuddy].aModel[nCntParts].pos;

	//	//角度の正規化
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.x = RotNormalize(g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.x);
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.y = RotNormalize(g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.y);
	//	g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.z = RotNormalize(g_aBuddy[nCntBuddy].aMotionData[nMotionType].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rotDiff.z);

	//}

	//今のモーションの更新
	g_aBuddy[nCntBuddy].nNowMotionNum = nMotionType;
	g_aBuddy[nCntBuddy].nPatternKey = 0;	//現在のキー番号リセット
	g_aBuddy[nCntBuddy].nCntFrame = 0;	//フレームカウントリセット
	g_aBuddy[nCntBuddy].nCntAllFrame = 0;	//総フレーム数リセット

}

//==================================================================================
//相棒のモーション更新処理
//==================================================================================
void UpdateMotionBuddy(int nCntBuddy)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	for (int nCntParts = 0; nCntParts < g_aBuddy[nCntBuddy].nPartsNum; nCntParts++)
	{//パーツ分繰り返す

		//次のキー
		int nNextKey = (g_aBuddy[nCntBuddy].nPatternKey + 1) % g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nNumKey;

		if (g_aBuddy[nCntBuddy].nPatternKey == g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nNumKey - 1 && 
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nLoop == MOTION_LOOP_OFF)
		{//ループしないとき

			nNextKey = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nNumKey - 1;
		}

		//前回と次のキーの差分取得
		float rotDiffX = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.z;


		//パーツの向きを設定
		g_aBuddy[nCntBuddy].aModel[nCntParts].rot.x =
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)g_aBuddy[nCntBuddy].nCntFrame /
			(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame
			);

		g_aBuddy[nCntBuddy].aModel[nCntParts].rot.y =
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)g_aBuddy[nCntBuddy].nCntFrame /
			(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame
			);

		g_aBuddy[nCntBuddy].aModel[nCntParts].rot.z =
			g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)g_aBuddy[nCntBuddy].nCntFrame /
			(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame
			);

		//角度の正規化
		RotNormalize(&g_aBuddy[nCntBuddy].aModel[g_aBuddy[nCntBuddy].nNowMotionNum].rot.x);
		RotNormalize(&g_aBuddy[nCntBuddy].aModel[g_aBuddy[nCntBuddy].nNowMotionNum].rot.y);
		RotNormalize(&g_aBuddy[nCntBuddy].aModel[g_aBuddy[nCntBuddy].nNowMotionNum].rot.z);


		//パーツの位置を設定
		if (nCntParts == 0 && 
			(GetMode() == MODE_TITLE || GetMode() == MODE_TUTORIAL || (GetMode() == MODE_GAME && GetGameState() != GAMESTATE_BEFOREEND)))
		{
			//目標の位置との差分を求める
			float posDiffX = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.z;

			//位置補正
			g_aBuddy[nCntBuddy].pos.x +=
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.x +
				posDiffX *
				(
				(float)g_aBuddy[nCntBuddy].nCntFrame /
				(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame);

			g_aBuddy[nCntBuddy].pos.y =
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.y +
				posDiffY *
				(
				(float)g_aBuddy[nCntBuddy].nCntFrame /
				(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame);

			g_aBuddy[nCntBuddy].pos.z +=
				g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].aParts[nCntParts].pos.z +
				posDiffZ *
				(
				(float)g_aBuddy[nCntBuddy].nCntFrame /
				(float)g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame);
		}
	}

	//フレームのカウントを加算
	g_aBuddy[nCntBuddy].nCntFrame++;
	g_aBuddy[nCntBuddy].nCntAllFrame++;

	if (g_aBuddy[nCntBuddy].nCntFrame >= g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].aKey[g_aBuddy[nCntBuddy].nPatternKey].nFrame)
	{//フレームのカウントがフレーム数に達したら

		//フレームのカウントをゼロに戻す
		g_aBuddy[nCntBuddy].nCntFrame = 0;

		//パターンNO.更新
		g_aBuddy[nCntBuddy].nPatternKey = (g_aBuddy[nCntBuddy].nPatternKey + 1) % g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nNumKey;

		if (g_aBuddy[nCntBuddy].nPatternKey == 0)
		{//キーがゼロの時

			//攻撃状態解除
			g_aBuddy[nCntBuddy].bATK = false;
			g_aBuddy[nCntBuddy].nCntAllFrame = 0;	//総フレーム数リセット

			if (g_aBuddy[nCntBuddy].aMotionData[g_aBuddy[nCntBuddy].nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//モーションの設定
				SetMotisonBuddy(nCntBuddy, PLAYERMOTION_DEF);
			}
			else
			{
				//モーションの設定
				SetMotisonBuddy(nCntBuddy, g_aBuddy[nCntBuddy].nNowMotionNum);
			}
		}
	}

}

//==================================================================================
//相棒の描画処理
//==================================================================================
void DrawBuddy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATRIX mtxnParent;			//親の計算用マトリックス

	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		if (g_aBuddy[nCntBuddy].bUse == true)
		{//使用していたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aBuddy[nCntBuddy].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBuddy[nCntBuddy].rot.y, g_aBuddy[nCntBuddy].rot.x, g_aBuddy[nCntBuddy].rot.z);
			D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].mtxWorld, &g_aBuddy[nCntBuddy].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aBuddy[nCntBuddy].pos.x, g_aBuddy[nCntBuddy].pos.y, g_aBuddy[nCntBuddy].pos.z);
			D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].mtxWorld, &g_aBuddy[nCntBuddy].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aBuddy[nCntBuddy].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			for (int nCount = 0; nCount < g_aBuddy[nCntBuddy].nPartsNum; nCount++)
			{
				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld);

				//向きを反映する
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aBuddy[nCntBuddy].aModel[nCount].rot.y, g_aBuddy[nCntBuddy].aModel[nCount].rot.x, g_aBuddy[nCntBuddy].aModel[nCount].rot.z);
				D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &mtxRot);

				//位置を反映する
				D3DXMatrixTranslation(&mtxTrans, g_aBuddy[nCntBuddy].aModel[nCount].pos.x, g_aBuddy[nCntBuddy].aModel[nCount].pos.y, g_aBuddy[nCntBuddy].aModel[nCount].pos.z);
				D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &mtxTrans);

				if (g_aBuddy[nCntBuddy].aModel[nCount].nParent == -1)
				{//NULLの場合,親

					//相棒の向きを反映する
					mtxnParent = g_aBuddy[nCntBuddy].mtxWorld;
				}
				else
				{//パーツ

					//親の向きを反映する
					mtxnParent = g_aBuddy[nCntBuddy].aModel[g_aBuddy[nCntBuddy].aModel[nCount].nParent].mtxWorld;
				}

				//自分のやつに親のワールドマトリックスを掛け算
				D3DXMatrixMultiply(&g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld, &mtxnParent);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aBuddy[nCntBuddy].aModel[nCount].mtxWorld);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aBuddy[nCntBuddy].aModel[nCount].pBuffMat->GetBufferPointer();

				if (g_aBuddy[nCntBuddy].aModel[nCount].bUse == true)
				{//パーツが使用されていたら

					//頂点数分繰り返し
					for (int nCntMat = 0; nCntMat < (int)g_aBuddy[nCntBuddy].aModel[nCount].dwNumMat; nCntMat++)
					{
						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, g_aBuddy[nCntBuddy].aModel[nCount].pTexture[nCntMat]);

						//相棒(パーツ)の描画
						g_aBuddy[nCntBuddy].aModel[nCount].pMesh->DrawSubset(nCntMat);
					}
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//相棒の情報取得
//==================================================================================
Buddy *GetBuddy(void)
{
	return &g_aBuddy[0];
}

//==================================================================================
//相棒の設定処理
//==================================================================================
void SetBuddy(int nCntBuddy)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParts = 0; nCntParts < g_aBuddy[nCntBuddy].nPartsNum; nCntParts++)
	{//パーツ分繰り返す

		//Xファイルの読み込み
		D3DXLoadMeshFromX(&g_aBuddy[nCntBuddy].aModel[nCntParts].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aBuddy[nCntBuddy].aModel[nCntParts].pBuffMat,
			NULL,
			&g_aBuddy[nCntBuddy].aModel[nCntParts].dwNumMat,
			&g_aBuddy[nCntBuddy].aModel[nCntParts].pMesh);

		D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aBuddy[nCntBuddy].aModel[nCntParts].pBuffMat->GetBufferPointer();

		//頂点数分繰り返し
		for (int nCntMat = 0; nCntMat < (int)g_aBuddy[nCntBuddy].aModel[nCntParts].dwNumMat; nCntMat++)
		{
			g_aBuddy[nCntBuddy].aModel[nCntParts].pTexture[nCntMat] = NULL;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//ファイルが存在する

				//テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aBuddy[nCntBuddy].aModel[nCntParts].pTexture[nCntMat]);
			}
		}

		if (g_aBuddy[nCntBuddy].aModel[nCntParts].nFever != 0)
		{
			//パーツを使用状態
			g_aBuddy[nCntBuddy].aModel[nCntParts].bUse = true;
		}
		else
		{
			g_aBuddy[nCntBuddy].aModel[nCntParts].bUse = false;
		}

		//モデルの全頂点チェック
		CheckVtx(g_aBuddy[nCntBuddy].aModel[nCntParts].rot.y, &g_aBuddy[nCntBuddy].aModel[nCntParts].vtxMax, &g_aBuddy[nCntBuddy].aModel[nCntParts].vtxMin, g_aBuddy[nCntBuddy].aModel[nCntParts].pMesh, g_aBuddy[nCntBuddy].aModel[nCntParts].pVtxBuff);
	}

	//影の設定処理
	if (GetMode() != MODE_TITLE)
	{
		g_aBuddy[nCntBuddy].nIdxShadow = SetShadow(g_aBuddy[nCntBuddy].fRadius * 0.5f, g_aBuddy[nCntBuddy].fRadius * 0.5f);
		SetPositionShadow(g_aBuddy[nCntBuddy].nIdxShadow, g_aBuddy[nCntBuddy].pos);
	}

	//アイコンの設定処理
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		switch (nCntBuddy)
		{
		case BUDDYTYPE_HYOROGARI:

			//アイコンの設定処理
			g_aBuddy[nCntBuddy].nIdxIcon = SetIcon(MINIMAP_ICON_CHIBI, 100.0f);
			break;

		case BUDDYTYPE_DEPPA:

			//アイコンの設定処理
			g_aBuddy[nCntBuddy].nIdxIcon = SetIcon(MINIMAP_ICON_KODEBU, 100.0f);
			break;
		}
		UpdatePositionIcon(g_aBuddy[nCntBuddy].nIdxIcon, g_aBuddy[nCntBuddy].pos);
	}
}

//==================================================================================
//相棒の読み込み処理
//==================================================================================
void ReadSetBuddy(void)
{

	//相棒数分繰り返す
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		FILE *pFile = NULL;	//ファイルポインタを宣言

		//ファイルを開く
		switch (nCntBuddy)
		{
		case 0:
			pFile = fopen(BUDDY1_TXT, "r");
			break;

		case 1:
			pFile = fopen(BUDDY2_TXT, "r");
			break;
		}


		if (pFile != NULL)
		{//ファイルが開けた場合

			char aComment[MAX_COMMENT];	//コメント、確認等

			int nCntSetParts = 0;
			int nCntFileName = 0;

			while (1)
			{//END_SCRIPTが来るまで繰り返す

				//文字列の読み込み
				fscanf(pFile, "%s", &aComment[0]);

				//モデル数の設定
				if (strcmp(aComment, "NUM_MODEL") == 0)
				{//NUM_MODELがきたら

					fscanf(pFile, "%s", &aComment[0]);	//=の分
					fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].nPartsNum);	//モデル数
				}

				while (nCntFileName != g_aBuddy[nCntBuddy].nPartsNum)
				{//モデルの数分読み込むまで繰り返し

					//文字列の読み込み
					fscanf(pFile, "%s", &aComment[0]);

					//モデル名の設定
					if (strcmp(aComment, "MODEL_FILENAME") == 0)
					{//NUM_MODELがきたら

						fscanf(pFile, "%s", &aComment[0]);	//=の分
						fscanf(pFile, "%s", &g_aBuddy[nCntBuddy].aModel[nCntFileName].acFilename[0]);	//ファイル名
						nCntFileName++;	//ファイル数加算
					}
				}

				//各パーツの設定
				if (strcmp(aComment, "CHARACTERSET") == 0)
				{//キャラクター情報の読み込みを開始

					while (strcmp(aComment, "END_CHARACTERSET") != 0)
					{//END_CHARACTERSETが来るまで繰り返し

						fscanf(pFile, "%s", &aComment[0]);	//確認する

						if (strcmp(aComment, "PARTSSET") == 0)
						{//PARTSSETでパーツ情報の読み込み開始

							while (strcmp(aComment, "END_PARTSSET") != 0)
							{//END_PARTSSETが来るまで繰り返す

								fscanf(pFile, "%s", &aComment[0]);	//確認する

								if (strcmp(aComment, "INDEX") == 0)
								{//INDEXが来たらモデル番号読み込み

									fscanf(pFile, "%s", &aComment[0]);		//=の分
									fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].nType);	//モデル種類の列挙

								}

								if (strcmp(aComment, "PARENT") == 0)
								{//PARENTが来たら親の番号読み込み

									fscanf(pFile, "%s", &aComment[0]);		//=の分
									fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].nParent);	//親の番号

								}

								if (strcmp(aComment, "POS") == 0)
								{//POSが来たら位置読み込み

									fscanf(pFile, "%s", &aComment[0]);		//=の分
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].pos.x);	//X座標
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].pos.y);	//Y座標
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].pos.z);	//Z座標

									//元の位置割り当て
									g_aBuddy[nCntBuddy].aModel[nCntSetParts].posOrigin = g_aBuddy[nCntBuddy].aModel[nCntSetParts].pos;

								}

								if (strcmp(aComment, "ROT") == 0)
								{//ROTが来たら向き読み込み

									fscanf(pFile, "%s", &aComment[0]);		//=の分
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].rot.x);	//Xの角度
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].rot.y);	//Yの角度
									fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].rot.z);	//Zの角度

								}

								if (strcmp(aComment, "WEAPON") == 0)
								{//WEAPONが来たら武器かどうか読み込み

									fscanf(pFile, "%s", &aComment[0]);		//=の分

								}

								if (strcmp(aComment, "FEVER") == 0)
								{//WEAPONが来たら武器かどうか読み込み

									fscanf(pFile, "%s", &aComment[0]);		//=の分
									fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aModel[nCntSetParts].nFever);		//常に出すかフィーバー中に出すか消すか
									
								}

							}//END_PARTSSETのかっこ

							nCntSetParts++;	//パーツのカウントを加算
						}

					}//END_CHARACTERSETのかっこ
				}

				if (strcmp(aComment, "END_SCRIPT") == 0)
				{//終了文字でループを抜ける

					break;
				}
			}

			//ファイルを閉じる
			fclose(pFile);
		}

		//相棒の配置
		SetBuddy(nCntBuddy);
	}
}

//==================================================================================
//相棒の読み込み処理
//==================================================================================
void ReadSetMotionBuddy(void)
{

	//相棒数分繰り返す
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		FILE *pFile = NULL;	//ファイルポインタを宣言

		char aComment[MAX_COMMENT];	//コメント用

		int nCntMotion = 0, nCntKey = 0, nCntParts = 0;

		//ファイルを開く
		switch (nCntBuddy)
		{
		case 0:
			pFile = fopen(BUDDY1_TXT, "r");
			break;

		case 1:
			pFile = fopen(BUDDY2_TXT, "r");
			break;
		}

		if (pFile != NULL)
		{//ファイルが開けた場合

			while (1)
			{//END_SCRIPTが来るまで繰り返す

				//文字列の読み込み
				fscanf(pFile, "%s", &aComment[0]);

				//==================================================
				//各モーションの設定
				//==================================================
				if (strcmp(aComment, "MOTIONSET") == 0)
				{//モーション情報の読み込みを開始

					while (strcmp(aComment, "END_MOTIONSET") != 0)
					{//END_MOTIONSETが来るまで繰り返し

						fscanf(pFile, "%s", &aComment[0]);	//確認する

						if (strcmp(aComment, "LOOP") == 0)
						{//LOOPが来たらループON/OFF読み込み

							fscanf(pFile, "%s", &aComment[0]);		//=の分
							fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].nLoop);	//ループ0か1
						}

						if (strcmp(aComment, "NUM_KEY") == 0)
						{//NUM_KEYが来たらキー数読み込み

							fscanf(pFile, "%s", &aComment[0]);		//=の分
							fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].nNumKey);	//キー数
						}

						//==================================================
						//各キーの設定
						//==================================================
						if (strcmp(aComment, "KEYSET") == 0)
						{//KEYSETでキー情報の読み込み開始

							while (strcmp(aComment, "END_KEYSET") != 0)
							{//END_KEYSETが来るまで繰り返し

								fscanf(pFile, "%s", &aComment[0]);	//確認する

								if (strcmp(aComment, "FRAME") == 0)
								{//FRAMEが来たら再生フレーム数読み込み

									fscanf(pFile, "%s", &aComment[0]);		//=の分
									fscanf(pFile, "%d", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//再生フレーム
								}

								//==================================================
								//パーツの設定
								//==================================================
								if (strcmp(aComment, "PARTS") == 0)
								{//PARTSでパーツ情報の読み込み開始

									while (strcmp(aComment, "END_PARTS") != 0)
									{//END_PARTSが来るまで繰り返し

										fscanf(pFile, "%s", &aComment[0]);	//確認する

										if (strcmp(aComment, "POS") == 0)
										{//POSが来たら位置読み込み

											fscanf(pFile, "%s", &aComment[0]);		//=の分
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X座標
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y座標
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z座標
										}

										if (strcmp(aComment, "ROT") == 0)
										{//ROTが来たら角度読み込み

											fscanf(pFile, "%s", &aComment[0]);		//=の分
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X角度
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y角度
											fscanf(pFile, "%f", &g_aBuddy[nCntBuddy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z角度
										}

										if (strcmp(aComment, "FEVER") == 0)
										{//ROTが来たら角度読み込み

											fscanf(pFile, "%s", &aComment[0]);		//=の分
									
										}

									}//END_PARTSのかっこ

									nCntParts++;	//パーツのカウント加算
								}


							}//END_KEYSETのかっこ

							nCntParts = 0;	//パーツのカウントリセット
							nCntKey++;	//キーのカウント加算
						}

					}//END_MOTIONSETのかっこ

					nCntKey = 0;	//キーのカウントリセット
					nCntMotion++;	//モーションのカウント加算
				}

				if (strcmp(aComment, "END_SCRIPT") == 0)
				{//終了文字でループを抜ける

					break;
				}
			}

			//ファイルを閉じる
			fclose(pFile);

			//モーションの設定
			SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
		}

	}
}

//==================================================================================
//相棒の読み込み処理
//==================================================================================
void SetBuddyDispatch(D3DXVECTOR3 pos, int nType)
{

	//群衆の情報を取得
	Crowd *pCrowd = GetCrowd();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	int nCntUse = 0;
	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; pCrowd++, nCntCrowd++)
	{
		if (pCrowd->aModel.bUse == true)
		{
			if (g_aBuddy[nType].nState != BUDDYSTATE_DISPATCH && g_aBuddy[(nType + 1) % BUDDYTYPE_MAX].nState != BUDDYSTATE_DISPATCH)
			{//どっちも派遣されていないとき

				if (nCntUse > (pCrowdInfo->nCrowdValue / 3) * 2 && pCrowd->nParent == PARENT_PLAYER)
				{//3分の２超えたら親をニートにする

					//隊列を変更する
					pPlayer->aFormation[pCrowd->aFormation.nIndex].bUse = false;

					//隊列の設定
					pCrowd->aFormation.nIndex = SetFormationPos(nType + 1);

					//親の種類設定
					pCrowd->nParent = nType + 1;
				}
			}
			else if (g_aBuddy[nType].nState != BUDDYSTATE_DISPATCH && g_aBuddy[(nType + 1) % BUDDYTYPE_MAX].nState == BUDDYSTATE_DISPATCH)
			{//今回のやつだけ派遣されていないとき

				if (nCntUse > (pCrowdInfo->nCrowdValue / 3) * 1 && nCntUse <= (pCrowdInfo->nCrowdValue / 3) * 2 && pCrowd->nParent == PARENT_PLAYER)
				{//3分の1～3分の2を親ニートにする

					//隊列を変更する
					pPlayer->aFormation[pCrowd->aFormation.nIndex].bUse = false;

					//隊列の設定
					pCrowd->aFormation.nIndex = SetFormationPos(nType + 1);

					//親の種類設定
					pCrowd->nParent = nType + 1;
				}
			}

			nCntUse++;
		}
	}

	g_aBuddy[nType].formationPos = pos;
	g_aBuddy[nType].nState = BUDDYSTATE_DISPATCH;
}

//==================================================================================
//ゲーム終了時の更新処理
//==================================================================================
void UpdateGameEndBuddy(int nCntBuddy)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//向き固定
	g_aBuddy[nCntBuddy].rot.y = 0.0f;
	g_aBuddy[nCntBuddy].fRotDiff = 0.0f;		//向きの差分
	g_aBuddy[nCntBuddy].fRotDest = g_aBuddy[nCntBuddy].rot.y;		//向きの差分

	//下に向かって走る
	g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
	g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
	g_aBuddy[nCntBuddy].bMove = true;

	//位置更新
	g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

	//慣性つける
	g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
	g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;

}

//==================================================================================
//相棒の操作(チュートリアル)
//==================================================================================
void ControllTutorialBuddy(int nCntBuddy)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	if (pPlayer->nNowMotionNum != PLAYERMOTION_ACTION && g_aBuddy[nCntBuddy].aMoveAI.bPosDest == false)
	{//アクションしていないとき && 目的地についてないとき

		if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (BUDDY_DISTANCE * BUDDY_DISTANCE))
		{//まだ規定値に着いていなかったら

			//移動量加算
			if (g_aBuddy[nCntBuddy].nState != BUDDYSTATE_RETURN)
			{//帰還状態じゃないとき

				g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
				g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * CHARA_MOVE;
			}
			else if (g_aBuddy[nCntBuddy].nState == BUDDYSTATE_RETURN)
			{//帰還状態のとき

				g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * RETURN_MOVE;
				g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * RETURN_MOVE;
			}
		}
	}

	if ((GetKeyboardPress(DIK_L) == true || GetGamepadPress(BUTTON_A, 0)) &&
		g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH &&
		pTutorial->nStep >= TUTORIAL_STEP_HYPNOSIS)
	{//Lが押された || ペンライトが使わてれいるとき

		//攻撃してる状態にする
		g_aBuddy[nCntBuddy].bATK = true;

		if (g_aBuddy[nCntBuddy].move.x <= 1.0f && g_aBuddy[nCntBuddy].move.x >= -1.0f &&
			g_aBuddy[nCntBuddy].move.z <= 1.0f && g_aBuddy[nCntBuddy].move.z >= -1.0f)
		{//止まっているときは強力

			g_aBuddy[nCntBuddy].bMove = false;
			g_aBuddy[nCntBuddy].move.x = 0.0f;
			g_aBuddy[nCntBuddy].move.z = 0.0f;

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION && g_aBuddy[nCntBuddy].bFever == false)
			{
				//モーションの設定
				SetMotisonBuddy(nCntBuddy, BUDDYMOTION_ACTION);
			}

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERACTION && g_aBuddy[nCntBuddy].bFever == true)
			{
				//モーションの設定
				SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERACTION);
			}
		}
		else
		{//動いてるときは弱

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION && g_aBuddy[nCntBuddy].bFever == false)
			{
				//モーションの設定
				SetMotisonBuddy(nCntBuddy, BUDDYMOTION_MOVEACTION);
			}

			if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_FEVERMOVEACTION && g_aBuddy[nCntBuddy].bFever == true)
			{
				//モーションの設定
				SetMotisonBuddy(nCntBuddy, BUDDYMOTION_FEVERMOVEACTION);
			}
		}
	}

	if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
	{//Lが押された && Aボタンが離された

		//攻撃状態解除
		g_aBuddy[nCntBuddy].bATK = false;

		//モーションの設定
		SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
	}

	if (g_aBuddy[nCntBuddy].aMoveAI.bPenUse == true)
	{//ペンライトが使わてれいるとき

		//攻撃してる状態にする
		g_aBuddy[nCntBuddy].bATK = true;
		g_aBuddy[nCntBuddy].move.x = 0.0f;
		g_aBuddy[nCntBuddy].move.z = 0.0f;
	}

	if (g_aBuddy[nCntBuddy].move.x >= 1.0f || g_aBuddy[nCntBuddy].move.x <= -1.0f || g_aBuddy[nCntBuddy].move.z >= 1.0f || g_aBuddy[nCntBuddy].move.z <= -1.0f)
	{//移動してるとき

		g_aBuddy[nCntBuddy].bMove = true;
	}
	else
	{//移動してないとき

		g_aBuddy[nCntBuddy].bMove = false;
	}

	//重力処理
	g_aBuddy[nCntBuddy].move.y -= 1.0f;
	g_aBuddy[nCntBuddy].move.y = 0.0f;

	if (g_aBuddy[nCntBuddy].bFever == true || (g_aBuddy[nCntBuddy].bCollisionModel == false && g_aBuddy[nCntBuddy].nState != BUDDYSTATE_DISPATCH))
	{//フィーバー中なら

		//加速
		g_aBuddy[nCntBuddy].move.x *= 1.1f;
		g_aBuddy[nCntBuddy].move.z *= 1.1f;
	}

	//位置更新
	g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

	//慣性つける
	if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_MOVEACTION)
	{
		g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
		g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;
	}
	else if (g_aBuddy[nCntBuddy].nNowMotionNum == BUDDYMOTION_MOVEACTION)
	{
		g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR_HYPNOSIS;
		g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR_HYPNOSIS;
	}
}

//==================================================================================
//相棒の更新処理
//==================================================================================
void UpdateTitleBuddy(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//タイトルの情報取得
	Title *pTitle = GetTitle();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		if (g_aBuddy[nCntBuddy].bUse == true)
		{//使用していたら

			//過去の位置保存
			g_aBuddy[nCntBuddy].posOld = g_aBuddy[nCntBuddy].pos;

			if (pTitle->nState != TITLESTATE_TKTK && pTitle->nState != TITLESTATE_FADE)
			{
				if (pTitle->nSelect == TITLESELECT_START)
				{
					g_aBuddy[BUDDYTYPE_HYOROGARI].formationPos.z = -300.0f;
					g_aBuddy[BUDDYTYPE_HYOROGARI].formationPos.x = -100.0f;
					g_aBuddy[BUDDYTYPE_DEPPA].formationPos.z = 0.0f;
					g_aBuddy[BUDDYTYPE_DEPPA].formationPos.x = 100.0f;
				}
				else
				{
					g_aBuddy[BUDDYTYPE_HYOROGARI].formationPos.z = 0.0f;
					g_aBuddy[BUDDYTYPE_HYOROGARI].formationPos.x = -100.0f;
					g_aBuddy[BUDDYTYPE_DEPPA].formationPos.z = -300.0f;
					g_aBuddy[BUDDYTYPE_DEPPA].formationPos.x = 100.0f;
				}
			}

			if (CircleRange(g_aBuddy[nCntBuddy].formationPos, g_aBuddy[nCntBuddy].pos) >= (BUDDY_DISTANCE * BUDDY_DISTANCE) && g_aBuddy[nCntBuddy].aMoveAI.bPosDest == false)
			{//まだ目的地についてないとき

				//目標の角度を求める
				g_aBuddy[nCntBuddy].fRotDest = atan2f((g_aBuddy[nCntBuddy].pos.x - g_aBuddy[nCntBuddy].formationPos.x), (g_aBuddy[nCntBuddy].pos.z - g_aBuddy[nCntBuddy].formationPos.z));

				//角度の正規化
				RotNormalize(&g_aBuddy[nCntBuddy].fRotDest);

				//目的地向かせる
				RotCorrect(nCntBuddy);

				//ニート自動移動
				g_aBuddy[nCntBuddy].move.x += sinf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI) * CHARA_MOVE;
				g_aBuddy[nCntBuddy].move.z += cosf(g_aBuddy[nCntBuddy].rot.y + D3DX_PI) * CHARA_MOVE;
			}
			else
			{
				g_aBuddy[nCntBuddy].fRotDest = 0.0f;

				//角度の正規化
				RotNormalize(&g_aBuddy[nCntBuddy].fRotDest);

				RotCorrect(nCntBuddy);
				g_aBuddy[nCntBuddy].move.x = 0.0f;
				g_aBuddy[nCntBuddy].move.z = 0.0f;

				if (pTitle->nState != TITLESTATE_TKTK)
				{//チカチカじゃないとき

					if (pTitle->nSelect == TITLESELECT_START)
					{//選択肢がスタートの時

						if (g_aBuddy[BUDDYTYPE_DEPPA].nNowMotionNum != BUDDYMOTION_DEF)
						{
							//モーションの設定
							SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_DEF);
						}

						if (g_aBuddy[BUDDYTYPE_HYOROGARI].nNowMotionNum != BUDDYMOTION_ACTION)
						{
							//モーションの設定
							SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_ACTION);
						}
					}
					else
					{//終わりの時

						if (g_aBuddy[BUDDYTYPE_HYOROGARI].nNowMotionNum != BUDDYMOTION_DEF)
						{
							//モーションの設定
							SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_DEF);
						}

						if (g_aBuddy[BUDDYTYPE_DEPPA].nNowMotionNum != BUDDYMOTION_ACTION)
						{
							//モーションの設定
							SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_ACTION);
						}
					}
				}
				else
				{
					if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_DEF)
					{
						//モーションの設定
						SetMotisonBuddy(nCntBuddy, BUDDYMOTION_DEF);
					}
				}
			}

			//位置更新
			g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

			//慣性つける
			g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
			g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;
			g_aBuddy[nCntBuddy].bMove = true;

			if (g_aBuddy[nCntBuddy].move.x <= 1.0f && g_aBuddy[nCntBuddy].move.x >= -1.0f &&
				g_aBuddy[nCntBuddy].move.z <= 1.0f && g_aBuddy[nCntBuddy].move.z >= -1.0f)
			{//止まっているときは強力

				g_aBuddy[nCntBuddy].bMove = false;
			}

			if (g_aBuddy[nCntBuddy].bMove == true)
			{//移動してるとき

				g_aBuddy[nCntBuddy].nCntWalk++;	//カウント加算

				if (g_aBuddy[nCntBuddy].nCntWalk > 15)
				{
					g_aBuddy[nCntBuddy].nCntWalk = 0;

					//歩行パーティクルの設定
					SetParticle(g_aBuddy[nCntBuddy].pos, PARTICLE_TYPE_WALK);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_WALK)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_WALK);
				}
			}

			//モーションの更新
			UpdateMotionBuddy(nCntBuddy);

			//キャラクターとの当たり判定
			CollisionCharBuddy(nCntBuddy);

			//座標制限
			LimitPosBuddy(nCntBuddy);

			//影の位置更新
			SetPositionShadow(g_aBuddy[nCntBuddy].nIdxShadow, g_aBuddy[nCntBuddy].pos);
		}
	}
}

//==================================================================================
//相棒の更新処理
//==================================================================================
void UpdateResultBuddy(void)
{
	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_ACTION)
		{

			//モーションの設定
			SetMotisonBuddy(nCntBuddy, BUDDYMOTION_ACTION);
		}

		//モーションの更新
		UpdateMotionBuddy(nCntBuddy);

		g_aBuddy[nCntBuddy].pos.y = 76.0f;
	}
}

//==================================================================================
//ランキングの更新処理
//==================================================================================
void UpdateRankingBuddy(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	for (int nCntBuddy = 0; nCntBuddy < MAX_BUDDY; nCntBuddy++)
	{
		if (g_aBuddy[nCntBuddy].bUse == true)
		{//使用していたら

			//過去の位置保存
			g_aBuddy[nCntBuddy].posOld = g_aBuddy[nCntBuddy].pos;

			//向き固定
			g_aBuddy[nCntBuddy].rot.y += 0.025f;

			RotNormalize(&g_aBuddy[nCntBuddy].rot.y);

			g_aBuddy[nCntBuddy].move.x += sinf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * 1.0f;
			g_aBuddy[nCntBuddy].move.z += cosf(D3DX_PI + g_aBuddy[nCntBuddy].rot.y) * 1.0f;

			g_aBuddy[nCntBuddy].bMove = true;

			//位置更新
			g_aBuddy[nCntBuddy].pos += g_aBuddy[nCntBuddy].move;

			//慣性つける
			g_aBuddy[nCntBuddy].move.x += (0.0f - g_aBuddy[nCntBuddy].move.x) * MOVE_FACTOR;
			g_aBuddy[nCntBuddy].move.z += (0.0f - g_aBuddy[nCntBuddy].move.z) * MOVE_FACTOR;

			if (g_aBuddy[nCntBuddy].bMove == true)
			{//移動してるとき

				g_aBuddy[nCntBuddy].nCntWalk++;	//カウント加算

				if (g_aBuddy[nCntBuddy].nCntWalk > 15)
				{
					g_aBuddy[nCntBuddy].nCntWalk = 0;

					//歩行パーティクルの設定
					SetParticle(g_aBuddy[nCntBuddy].pos, PARTICLE_TYPE_WALK);
				}

				if (g_aBuddy[nCntBuddy].nNowMotionNum != BUDDYMOTION_RANKING)
				{
					//モーションの設定
					SetMotisonBuddy(nCntBuddy, BUDDYMOTION_RANKING);
				}
			}

			//モーションの更新
			UpdateMotionBuddy(nCntBuddy);

			//座標制限
			LimitPosBuddy(nCntBuddy);

			//影の位置更新
			SetPositionShadow(g_aBuddy[nCntBuddy].nIdxShadow, g_aBuddy[nCntBuddy].pos);
		}
	}
}
