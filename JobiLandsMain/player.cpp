//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "enemy.h"
#include "meshwall.h"
#include "game.h"
#include "edit.h"
#include "tutorial.h"
#include "impactwave.h"
#include "calculation.h"
#include "dispatch.h"
#include "cutepillow.h"
#include "shadow.h"
#include "meshorbit.h"
#include "mob.h"
#include "minimap_icon.h"
#include "particle.h"
#include "debugproc.h"
#include "serif.h"
#include "sound.h"
#include "updown_fram.h"
#include "start_talk.h"
#include "manypoint.h"
#include "flag.h"


//マクロ定義
#define DMG_TIME	(10)	//ダメージ状態の時間
#define ULT_WAIT	(100)	//待ち状態
#define FLAG_POS	(150.0f)	//旗の位置
#define FLEN            (800.0f)
#define PLAYER_TXT	"data\\TEXT\\motion_set_player.txt"

//プロトタイプ宣言
void UpdateStartPlayer(void);
void UpdateGamePlayer(void);
void ControllEndPlayer(void);
void ControllPlayer(void);
void ReadSetPlayer(void);
void ReadSetMotionPlayer(void);
void UpdateMotionPlayer(void);
void LimitPosPlayer(void);
void CollisionCharPlayer(void);
void CollisionWallPlayer(void);
void UpdateStatePlayer(void);
void UpdateATKGun(void);
void UpdateATKPenLight(void);
void CollisionATKPlayer(float fDistance, float fAngle, int nValue);
void CalFormationPosPlayer(void);
int SetHypnosisWavePlayer(float fRot, float fWidthMove);

void ControllTutorialPlayer(void);

//グローバル変数宣言
Player g_aPlayer;		//プレイヤーの情報
MotionData g_aMotionData[MAX_MOTION];		//モーション用の情報
int g_nGameTimePlayer;

//==================================================================================
//プレイヤーの初期化処理
//==================================================================================
void InitPlayer(void)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各要素初期化
	if (GetMode() == MODE_GAME)
	{
		g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, -800.0f);
		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);
		g_aPlayer.bUse = true;										//使用しているか
	}
	else if (GetMode() == MODE_TITLE)
	{
		g_aPlayer.pos = D3DXVECTOR3(-150.0f, 0.0f, 0.0f);
		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);
		g_aPlayer.bUse = false;										//使用しているか
	}
	else if (GetMode() == MODE_RANKING)
	{
		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, D3DX_PI + pCamera->rot.y, 0.0f);

		//中心からD3DX_PI * -0.5fの方向に-150.0f
		g_aPlayer.pos = D3DXVECTOR3(sinf(D3DX_PI * -0.5f + g_aPlayer.rot.y) * -150.0f, 0.0f, cosf(D3DX_PI * -0.5f + g_aPlayer.rot.y) * -150.0f);
		g_aPlayer.bUse = true;										//使用しているか
	}
	else if (GetMode() == MODE_RESULT)
	{
		g_aPlayer.pos = D3DXVECTOR3(-25.0f, 143.0f, 300.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, pCamera->rot.y, 0.0f);
	}
	else
	{
		g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.posOld = g_aPlayer.pos;
		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.bUse = true;										//使用しているか
	}
	g_aPlayer.vecMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動ベクトル
	g_aPlayer.vecLine[4] = { D3DXVECTOR3(0.0f, 0.0f, 0.0f) };	//境界線ベクトル
	g_aPlayer.vecToPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//ベクトルの長さ
	g_aPlayer.fRotDiff = 0.0f;									//向きの差分
	g_aPlayer.fRotDest = g_aPlayer.rot.y;						//向きの差分
	g_aPlayer.fRot[4] = { 0.0f };								//ベクトルの向き

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);
	g_aPlayer.nIdxShadow = -1;									//影のインデックス番号
	g_aPlayer.nIdxHPGauge = -1;									//体力ゲージのインデックス番号

	for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
	{
		g_aPlayer.nIdxWave[nCntWave] = -1;						//衝撃波のインデックス番号
	}

	g_aPlayer.nIdxIcon = -1;									//ミニマップアイコンのインデックス番号
	g_aPlayer.nIdxSerif = -1;									//セリフのインデックス番号
	g_aPlayer.nIdxFlag = -1;									//旗のインデックス番号
	g_aPlayer.nState = PLAYERSTATE_NONE;						//状態
	g_aPlayer.nCntState = 0;									//状態カウント
	g_aPlayer.bDisp = true;										//描画しているか
	g_aPlayer.bFever = false;									//フィーバー状態か
	g_aPlayer.bSetTalk = false;			//会話のセットしたか


	//モーション系初期化
	g_aPlayer.nNowMotionNum = -1;		//現在のモーション番号
	g_aPlayer.nCntAllFrame = 0;			//総フレームカウント
	g_aPlayer.nCntFrame = 0;			//フレームのカウント
	g_aPlayer.nPatternKey = 0;			//何個目のキーか
	g_aPlayer.nPartsNum = 0;			//パーツ数
	g_aPlayer.bMove = false;			//移動しているか
	g_aPlayer.bJump = false;			//ジャンプしているか
	g_aPlayer.bATK = false;				//攻撃してるか

	g_aPlayer.nCntWalk = 0;				//移動のカウント
	g_aPlayer.nCntPenlight = 0;			//催眠のカウント
	g_aPlayer.nCntDis = 0;				//派遣のカウント
	g_aPlayer.nCntReturn = 0;			//帰還のカウント
	g_aPlayer.fRadius = 50.0f;			//半径

	for (int nCount = 0; nCount < MAX_MODEL; nCount++)
	{
		g_aPlayer.aModel[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//現在の位置
		g_aPlayer.aModel[nCount].posOld = g_aPlayer.aModel[nCount].pos;			//前回の位置
		g_aPlayer.aModel[nCount].posOrigin = g_aPlayer.aModel[nCount].pos;		//元の位置
		g_aPlayer.aModel[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
		g_aPlayer.aModel[nCount].rotOrigin = g_aPlayer.aModel[nCount].rot;		//元の向き
		g_aPlayer.aModel[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].OldmtxWorld);

		g_aPlayer.aModel[nCount].nIdxShadow = -1;		//影のインデックス番号
		g_aPlayer.aModel[nCount].nIdxRotate = -1;		//ぐるぐるのインデックス番号
		g_aPlayer.aModel[nCount].nIdxHypGauge = -1;		//体力ゲージのインデックス番号
		g_aPlayer.aModel[nCount].nShadow = -1;			//影を使うかどうか
		g_aPlayer.aModel[nCount].nFever = -1;				//影を使うかどうか

		g_aPlayer.aModel[nCount].nNumVtx = 0;			//頂点数
		g_aPlayer.aModel[nCount].dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aPlayer.aModel[nCount].pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aPlayer.aModel[nCount].vtxMin = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);		//モデルの最小値
		g_aPlayer.aModel[nCount].vtxMax = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);			//モデルの最大値

		g_aPlayer.aModel[nCount].nType = -1;		//読み込むモデルの種類
		g_aPlayer.aModel[nCount].nParent = -1;		//親の番号
		g_aPlayer.aModel[nCount].bUse = false;		//使用しているか
		g_aPlayer.aModel[nCount].nState = 0;		//状態

		g_aPlayer.aModel[nCount].pMesh = NULL;				//メッシュ(頂点情報)へのポインタ
		g_aPlayer.aModel[nCount].pBuffMat = NULL;			//マテリアルへのポインタ

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aPlayer.aModel[nCount].pMatData[nCntMat].MatD3D = {};
			g_aPlayer.aModel[nCount].pMatData[nCntMat].pTextureFilename = {};	//マテリアルのデータ
			g_aPlayer.aModel[nCount].acFilename[nCntMat] = {};		//ファイル名
		}
		
		g_aPlayer.aModel[nCount].dwNumMat = NULL;			//マテリアルの数

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aPlayer.aModel[nCount].pTexture[nCntTex] = {};		//テクスチャへのポインタ
		}

	}

	//隊列情報の初期化
	for (int nCntPos = 0; nCntPos < MAX_CROWD; nCntPos++)
	{
		g_aPlayer.aFormation[nCntPos].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPlayer.aFormation[nCntPos].nIndex = -1;
		g_aPlayer.aFormation[nCntPos].bUse = false;
	}

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//モーションの数繰り返す

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{//キーの数繰り返す

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{//パーツの数繰り返す

				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の向き
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向きの差分
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//目標の位置
				g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置の差分

			}

			g_aMotionData[nCntMotion].aKey[nCntKey].nFrame = 0;	//再生フレーム
		}

		g_aMotionData[nCntMotion].nNumKey = 0;				//キーの数
		g_aMotionData[nCntMotion].nLoop = MOTION_LOOP_ON;	//ループ判定
		g_aMotionData[nCntMotion].bUse = false;				//使用しているか
	}

	g_nGameTimePlayer = 0;

	//外部ファイル読み込み
	ReadSetPlayer();
	ReadSetMotionPlayer();

	//モーションの設定
	SetMotisonPlayer(PLAYERMOTION_DEF);
}

//==================================================================================
//プレイヤーの終了処理
//==================================================================================
void UninitPlayer(void)
{

	for (int nCount = 0; nCount < MAX_MODEL; nCount++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//テクスチャの破棄
			if (g_aPlayer.aModel[nCount].pTexture[nCntTex] != NULL)
			{
				g_aPlayer.aModel[nCount].pTexture[nCntTex]->Release();
				g_aPlayer.aModel[nCount].pTexture[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_aPlayer.aModel[nCount].pMesh != NULL)
		{
			g_aPlayer.aModel[nCount].pMesh->Release();
			g_aPlayer.aModel[nCount].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aPlayer.aModel[nCount].pBuffMat != NULL)
		{
			g_aPlayer.aModel[nCount].pBuffMat->Release();
			g_aPlayer.aModel[nCount].pBuffMat = NULL;
		}
	}

}

//==================================================================================
//プレイヤーの更新処理
//==================================================================================
void UpdatePlayer(void)
{

	if (GetGameState() == GAMESTATE_START)
	{
		//スタート時の更新処理
		UpdateStartPlayer();
	}
	else
	{
		//ゲーム時の更新処理
		UpdateGamePlayer();
	}
	
}

//==================================================================================
//スタート時の更新処理
//==================================================================================
void UpdateStartPlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//上下の枠情報取得
	UPDOWN_FRAM *pFram = GetUpDownFram();

	if (g_aPlayer.bUse == true)
	{//使用していたら

		if (GetUpDownFram()->bUse == false && GetUpDownFram()->bEnd == false)
		{
			//枠移動
			SetUpDownFram();
		}

		//位置は固定
		g_aPlayer.rot.y = D3DX_PI + pCamera->rot.y;
		g_aPlayer.fRotDiff = 0.0f;									//向きの差分
		g_aPlayer.fRotDest = g_aPlayer.rot.y;						//向きの差分

		if (pFram->bEnd == true)
		{//枠がついたら

			if (g_aPlayer.pos.z <= 0.0f)
			{//初期値に着いていなかったら

				g_aPlayer.move.x += sinf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.move.z += cosf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.bMove = true;
			}
			else
			{
				if (g_aPlayer.bSetTalk == false)
				{//まだセットしていなかったら

					g_aPlayer.bSetTalk = true;

					//最初の会話設定
					GetStartTalk()->bUse = true;
				}
			}
		}

		if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f &&
			g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
		{
			g_aPlayer.bMove = false;
		}

		//位置更新
		g_aPlayer.pos += g_aPlayer.move;

		//慣性つける
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;


		if (g_aPlayer.bMove == true)
		{//移動してるとき

			g_aPlayer.nCntWalk++;	//カウント加算

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//歩行パーティクルの設定
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				PlaySound(SOUND_LABEL_SE_ASIOTO);
			}


			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}

		}
		else
		{//ニュートラルモーション

			if (g_aPlayer.bATK == false && g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR)
			{
				//攻撃状態解除
				g_aPlayer.bATK = false;

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}


		//モーションの更新
		UpdateMotionPlayer();

		//座標制限
		LimitPosPlayer();

		//影の位置更新
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		//アイコンの位置更新
		UpdatePositionIcon(g_aPlayer.nIdxIcon, g_aPlayer.pos);

		//旗の位置更新
		SetPositionFlag(g_aPlayer.nIdxFlag, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + FLAG_POS, g_aPlayer.pos.z));
	}
}

//==================================================================================
//ゲーム時の更新処理
//==================================================================================
void UpdateGamePlayer(void)
{
	if (g_aPlayer.bUse == true)
	{//使用していたら

		//過去の位置保存
		g_aPlayer.posOld = g_aPlayer.pos;

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//パーツ分繰り返す

		 //パーツごとのマトリックス取得
			D3DXMATRIX mtxParts = GetParentMatrix(nCntParts);

			g_aPlayer.aModel[nCntParts].OldmtxWorld = mtxParts;
		}


		if (GetGameState() == GAMESTATE_BEFOREEND || GetGameState() == GAMESTATE_END)
		{
			//ゲーム終了時のプレイヤーの操作
			ControllEndPlayer();
		}
		else
		{
			//プレイヤーの操作
			ControllPlayer();
		}

		//隊列の位置更新
		CalFormationPosPlayer();

		//攻撃処理
		UpdateATKGun();
		UpdateATKPenLight();

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//移動してるとき

			g_aPlayer.nCntWalk++;	//カウント加算

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//歩行パーティクルの設定
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				PlaySound(SOUND_LABEL_SE_ASIOTO);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.bFever == false)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERWALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION && g_aPlayer.bFever == true)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_FEVERWALK);
			}
		}
		else
		{//ニュートラルモーション

			if (g_aPlayer.bATK == false && g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
			{
				//攻撃状態解除
				g_aPlayer.bATK = false;

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION || 
			g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION || 
			g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION || 
			g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
		{//催眠アクション中

			g_aPlayer.nCntPenlight++;

			if (g_aPlayer.nCntPenlight == 20 || g_aPlayer.nCntPenlight == 40)
			{
				if (g_aPlayer.nCntPenlight >= 40)
				{
					g_aPlayer.nCntPenlight = 0;
				}

				//催眠のホワンホワン設定
				if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION)
				{//止まって催眠

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
				{//動いて催眠
					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION)
				{//動いて催眠

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
				{//動いて催眠

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS_MOVE) / (float)40);
				}
			
				if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
				{//フィーバー中なら

				 //催眠のホワンホワン設定
					if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION)
					{//止まって催眠

						g_aPlayer.nIdxWave[1] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[2] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[3] = SetHypnosisWavePlayer(g_aPlayer.rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
					}
					else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
					{//動いて催眠

						g_aPlayer.nIdxWave[1] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[2] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[3] = SetHypnosisWavePlayer(g_aPlayer.rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
					}
				}
			}
		}
		else if (
			g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
		{//催眠アクションしてないとき

			//催眠の音を消す
			StopSound(SOUND_LABEL_BGM_HYPNOSIS);
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVECURSOR)
		{
			DISPATCH *pDispatch = GetDispatch();

			//目標の角度を求める
			g_aPlayer.fRotDest = atan2f((g_aPlayer.pos.x - pDispatch->pos.x), (g_aPlayer.pos.z - pDispatch->pos.z));
			g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

			//角度の正規化
			g_aPlayer.fRotDiff = RotNormalize(g_aPlayer.fRotDiff);

			//角度の補正をする
			g_aPlayer.rot.y += g_aPlayer.fRotDiff;

			//角度の正規化
			g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);
		}


		//軌跡の情報取得
		ORBIT *pOrbit = GetMeshOrbit();

		//パーツごとのマトリックス取得
		D3DXMATRIX mtxParts = GetParentMatrix(10);

		pOrbit->pMtxparent = &mtxParts;

		//モーションの更新
		UpdateMotionPlayer();

		//キャラクターとの当たり判定
		CollisionCharPlayer();

		//壁との当たり判定
		//CollisionWall(&g_aPlayer.pos, g_aPlayer.posOld);

		//座標制限
		LimitPosPlayer();

		//影の位置更新
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		//アイコンの位置更新
		UpdatePositionIcon(g_aPlayer.nIdxIcon, g_aPlayer.pos);

		//セリフの位置更新
		SetPositionSerif(g_aPlayer.nIdxSerif, { g_aPlayer.pos.x, g_aPlayer.pos.y + 150.0f, g_aPlayer.pos.z });

		//衝撃波の位置更新
		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
		{//移動催眠アクション中

			for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
			{
				SetPositionImpactWave(g_aPlayer.nIdxWave[nCntWave], g_aPlayer.move);
			}
		}

		//旗の位置更新
		SetPositionFlag(g_aPlayer.nIdxFlag, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + FLAG_POS, g_aPlayer.pos.z));

	}

	//状態更新
	UpdateStatePlayer();
}

//==================================================================================
//ゲーム終了時の更新処理
//==================================================================================
void ControllEndPlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	g_aPlayer.bMove = true;

	g_aPlayer.rot.y = 0.0f;

	g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
	g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
	g_aPlayer.fRotDest = pCamera->rot.y;

	//重力処理
	//g_aPlayer.move.y -= 1.0f;

	//位置更新
	g_aPlayer.pos += g_aPlayer.move;

	//慣性
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;
}

//==================================================================================
//モデルの操作
//==================================================================================
void ControllPlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//エディットの情報取得
	Edit  *pEdit = GetEdit();

	//ターゲットの情報取得
	DISPATCH *pDispatch = GetDispatch();

	if (GetGameState() == GAMESTATE_NONE && pDispatch->bUse == false && pEdit->bUse == false)
	{//ターゲット中以外

		if (g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR)
		{//移動不可モーション中は強制キャンセル

			if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
			{//←キーが押された,左移動

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//A+W,左上移動

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LRUP + pCamera->rot.y;
					pCamera->bSPush = false;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//A+S,左下移動

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LRDW + pCamera->rot.y;

					pCamera->bSPush = true;
				}
				else
				{//A,左移動

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LR + pCamera->rot.y;
					pCamera->bSPush = false;
				}

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
				{//催眠していなかったら

					g_aPlayer.bMove = true;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
			{//Dキーが押された,右移動

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//D+W,右上移動

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRUP + pCamera->rot.y;
					pCamera->bSPush = false;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//D+S,右下移動

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRDW + pCamera->rot.y;

					pCamera->bSPush = true;
				}
				else
				{//D,右移動

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LR + pCamera->rot.y;
					pCamera->bSPush = false;
				}

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
				{//催眠していなかったら

					g_aPlayer.bMove = true;
				}
			}
			else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//Wが押された、奥移動

				g_aPlayer.move.x += sinf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.move.z += cosf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.fRotDest = D3DX_PI + pCamera->rot.y;
				pCamera->bSPush = false;

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
				{//催眠していなかったら

					g_aPlayer.bMove = true;
				}
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//Sが押された、手前移動

				g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.fRotDest = pCamera->rot.y;

				pCamera->bSPush = true;

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
				{//催眠していなかったら

					g_aPlayer.bMove = true;
				}
			}
			else
			{//なんのキーも押されていない

				if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f ||
					g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
				{
					g_aPlayer.bMove = false;
					pCamera->bSPush = false;

					if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
					{//移動催眠中の時

						//モーションの設定
						SetMotisonPlayer(PLAYERMOTION_ACTION);
					}

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION && g_aPlayer.bFever == true && g_aPlayer.bATK == true)
					{//移動催眠中の時

						//モーションの設定
						SetMotisonPlayer(PLAYERMOTION_FEVERACTION);
					}
				}
			}
		}
		

		if (g_aPlayer.bATK == false && g_aPlayer.bJump == false && GetKeyboardTrigger(DIK_RETURN) == true ||
			g_aPlayer.bATK == false && g_aPlayer.bJump == false && GetGamepadTrigger(BUTTON_B, 0))
		{//ENTERが押された,,弾発射

			//抱き枕の設定処理
			SetCutepillow(g_aPlayer.pos, g_aPlayer.rot, PILLOWPARENT_PLAYER);

			if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
			{//フィーバー中なら

				//全方位抱き枕
				SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + D3DX_PI * 0.5f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
				SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + D3DX_PI * 1.0f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
				SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + -D3DX_PI * 0.5f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
			}

			//SEセット
			PlaySound(SOUND_LABEL_SE_DAKIMAKURA);
		}

		if (GetKeyboardPress(DIK_L) == true || 
			GetGamepadPress(BUTTON_A, 0))
		{//Lが押された && Aボタンが押された

			//g_aPlayer.bATK = true;

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
			{//催眠していなかったら

				//催眠SE
				PlaySound(SOUND_LABEL_BGM_HYPNOSIS);
			}

			if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f &&
				g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
			{//止まっているときは強力

			 g_aPlayer.bMove = false;
			 g_aPlayer.move.x = 0.0f;
			 g_aPlayer.move.z = 0.0f;

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.bFever == false)
				{//フィーバー中じゃなかったら

					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_ACTION);
				}

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION && g_aPlayer.bFever == true)
				{//フィーバー中だったら

					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_FEVERACTION);
				}
			}
			else
			{//動いてるときは弱

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.bFever == false)
				{//fフィーバー中じゃなかったら
					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_MOVEACTION);
				}

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION && g_aPlayer.bFever == true/* && g_aPlayer.bATK == false*/)
				{//フィーバー中だったら

					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_FEVERMOVEACTION);
				}
			}
		}

		if (GetKeyboardRelease(DIK_L) == true || 
			GetGamepadRelease(BUTTON_A, 0))
		{//Lが押された && Aボタンが離された

			//攻撃状態解除
			g_aPlayer.bATK = false;

			//モーションの設定
			SetMotisonPlayer(PLAYERMOTION_DEF);
		}
	}

	if (GetGamepadTrigger(BUTTON_RB, 0) || GetKeyboardTrigger(DIK_RSHIFT) == true)
		{//派遣の使用状況入れ替え

			SetRightDisPatch();
		}
		if (GetGamepadTrigger(BUTTON_LB, 0) || GetKeyboardTrigger(DIK_LSHIFT) == true)
		{//派遣の使用状況入れ替え

			SetLeftDisPatch();
		}

	//現在と目標の差分を求める
	g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

	//角度の正規化
	g_aPlayer.fRotDiff = RotNormalize(g_aPlayer.fRotDiff);

	//角度の補正をする
	g_aPlayer.rot.y += g_aPlayer.fRotDiff * 0.1f;

	//角度の正規化
	g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

	//重力処理
	//g_aPlayer.move.y -= 1.0f;

	if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
	{//フィーバー中なら

		//加速
		g_aPlayer.move.x *= 1.1f;
		g_aPlayer.move.z *= 1.1f;
	}

	//位置更新
	g_aPlayer.pos += g_aPlayer.move;

	//慣性つける
	if (g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION)
	{
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;
	}
	else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
	{
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR_HYPNOSIS;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR_HYPNOSIS;

		if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
		{//フィーバー中なら

			//加速
			g_aPlayer.move.x *= 1.1f;
			g_aPlayer.move.z *= 1.1f;
		}
	}

	PrintDebugProc(
		"\n------プレイヤーの操作------\n"
		"<移動> W/A/S/D\n"
		"<催眠> L\n"
		"<派遣> LT/RT\n"
		"<抱き枕> ENTER\n\n"
		"<移動量> [%f, %f, %f]\n", g_aPlayer.move.x, g_aPlayer.move.y, g_aPlayer.move.z);

}

//==================================================================================
//プレイヤーの座標制限
//==================================================================================
void LimitPosPlayer(void)
{
	if (g_aPlayer.pos.y < 0.0f)
	{//地面より下に行ったら

		//g_aPlayer.pos.y = 0.0f;

		//重力処理
		g_aPlayer.move.y = 0.0f;

		g_aPlayer.bJump = false;
	}


	//移動制限
	if (GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_END)
	{
		if (g_aPlayer.pos.x - g_aPlayer.fRadius <= -LIMIT_POS && g_aPlayer.posOld.x - g_aPlayer.fRadius >= -LIMIT_POS)
		{//今回が-1000以下, 前回が-1000以上

			g_aPlayer.pos.x = -LIMIT_POS + g_aPlayer.fRadius;
		}
		if (g_aPlayer.pos.x + g_aPlayer.fRadius >= LIMIT_POS && g_aPlayer.posOld.x + g_aPlayer.fRadius <= LIMIT_POS)
		{//今回が1000以上, 前回が1000以下

			g_aPlayer.pos.x = LIMIT_POS - g_aPlayer.fRadius;
		}
		if (g_aPlayer.pos.z + g_aPlayer.fRadius >= LIMIT_POS && g_aPlayer.posOld.z + g_aPlayer.fRadius <= LIMIT_POS)
		{//今回が1000以上, 前回が1000以下

			g_aPlayer.pos.z = LIMIT_POS - g_aPlayer.fRadius;
		}
		if (g_aPlayer.pos.z - g_aPlayer.fRadius <= -LIMIT_POS && g_aPlayer.posOld.z - g_aPlayer.fRadius >= -LIMIT_POS)
		{//今回が-1000以下, 前回が-1000以上

			g_aPlayer.pos.z = -LIMIT_POS + g_aPlayer.fRadius;
		}
	}
}

//==================================================================================
//プレイヤーの隊列計算処理
//==================================================================================
void CalFormationPosPlayer(void)
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

				g_aPlayer.aFormation[nCntCrowd + nCntWidth].pos =
					D3DXVECTOR3
					(
						g_aPlayer.pos.x + sinf(-D3DX_PI * MOVE_LR + fRot + g_aPlayer.rot.y) * (FORMATION_DISTANCE * (nCntformation + 1)),
						0.0f,
						g_aPlayer.pos.z + cosf(-D3DX_PI * MOVE_LR + fRot + g_aPlayer.rot.y) * (FORMATION_DISTANCE * (nCntformation + 1)));
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
//プレイヤーの隊列計算処理
//==================================================================================
int SetFormationPosPlayer(void)
{
	int nCntIdx = 0;

	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aPlayer.aFormation[nCntCrowd].bUse == false)
		{//使用していなかったら

			//インデックス番号を代入
			nCntIdx = nCntCrowd;

			g_aPlayer.aFormation[nCntCrowd].bUse = true;
			break;
		}
	}

	return nCntIdx;
}

//==================================================================================
//催眠中のホワンホワン設定処理
//==================================================================================
int SetHypnosisWavePlayer(float fRot, float fWidthMove)
{
	//催眠のホワンホワン
	int nIdxWave = SetImpactWave
	(
		D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + 50.0f, g_aPlayer.pos.z),
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
//ガンの攻撃更新処理
//==================================================================================
void UpdateATKGun(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	static int nCnt = 0;

	if (g_aPlayer.bATK == true)
	{//ガン発動中

		nCnt++;

		if (nCnt % 5 == 0 && nCnt <= 15)
		{
		}

		if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
		{//←キーが押された,左移動

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//A+W,左上移動

				g_aPlayer.fRotDest = D3DX_PI * MOVE_LRUP + pCamera->rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//A+S,左下移動

				g_aPlayer.fRotDest = D3DX_PI * MOVE_LRDW + pCamera->rot.y;
			}
			else
			{//A,左移動

				g_aPlayer.fRotDest = D3DX_PI * MOVE_LR + pCamera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
		{//Dキーが押された,右移動

			if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//D+W,右上移動

				g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRUP + pCamera->rot.y;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//D+S,右下移動

				g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRDW + pCamera->rot.y;
			}
			else
			{//D,右移動

				g_aPlayer.fRotDest = -D3DX_PI * MOVE_LR + pCamera->rot.y;
			}
		}
		else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
		{//Wが押された、奥移動

			g_aPlayer.fRotDest = D3DX_PI + pCamera->rot.y;
		}
		else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
		{//Sが押された、手前移動

			g_aPlayer.fRotDest = pCamera->rot.y;
		}
	}
	else
	{
		nCnt = 0;
	}
}

//==================================================================================
//攻撃処理
//==================================================================================
void UpdateATKPenLight(void)
{
	switch (g_aPlayer.nNowMotionNum)
	{
	case PLAYERMOTION_ACTION:

		//攻撃の当たり判定
		CollisionATKPlayer(PENLIGHT_RADIUS, PENLIGHT_ANGLE, STOP_HYPNOSIS);
		break;


	case PLAYERMOTION_MOVEACTION:

		//攻撃の当たり判定
		CollisionATKPlayer(PENLIGHT_RADIUS_MOVE, PENLIGHT_ANGLE_MOVE, MOVE_HYPNOSIS);
		break;

	case PLAYERMOTION_FEVERACTION:

		//攻撃の当たり判定
		CollisionATKPlayer(PENLIGHT_RADIUS, PENLIGHT_ANGLE, STOP_FEVER_HYPNOSIS);
		break;

	case PLAYERMOTION_FEVERMOVEACTION:

		//攻撃の当たり判定
		CollisionATKPlayer(PENLIGHT_RADIUS_MOVE, PENLIGHT_ANGLE_MOVE, MOVE_FEVER_HYPNOSIS);
		break;
	}
}

//==================================================================================
//プレイヤーの攻撃
//==================================================================================
void CollisionATKPlayer(float fDistance, float fAngle, int nValue)
{
	//モブの情報取得
	Mob *pMob = GetMob();

	//ペンライトの攻撃
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState != MOBSTATE_APPIEAR)
		{//モブが使用されていたら

			bool bPenLight;

			D3DXVECTOR3 pos = D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y) * -fDistance);

			//ペンライトの当たり判定
			bPenLight = CollisionFan(g_aPlayer.pos,
				D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y) * -fDistance),
				D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y) * -fDistance),
				pMob->aModel.pos, pMob->aModel.posOld);

			if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
			{//フィーバー中なら

				if (bPenLight == false)
				{//まだ当たっていなかったら

					bPenLight = CollisionFan(g_aPlayer.pos,
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y + D3DX_PI * 0.5f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y + D3DX_PI * 0.5f) * -fDistance),
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y + D3DX_PI * 0.5f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y + D3DX_PI * 0.5f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}

				if (bPenLight == false)
				{//まだ当たっていなかったら

					bPenLight = CollisionFan(g_aPlayer.pos,
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y + D3DX_PI * 1.0f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y + D3DX_PI * 1.0f) * -fDistance),
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y + D3DX_PI * 1.0f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y + D3DX_PI * 1.0f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}

				if (bPenLight == false)
				{//まだ当たっていなかったら

					bPenLight = CollisionFan(g_aPlayer.pos,
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y + -D3DX_PI * 0.5f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y + -D3DX_PI * 0.5f) * -fDistance),
						D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y + -D3DX_PI * 0.5f) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y + -D3DX_PI * 0.5f) * -fDistance),
						pMob->aModel.pos, pMob->aModel.posOld);
				}
			}

			if (bPenLight == true)
			{//扇の範囲に入ったら

				//催眠度が減少するまでのカウンターを初期化
				pMob->nSubHypCounter = HYP_CNTTIME;

				//モブのヒット処理
				HitMob(nCntMob, nValue);
			}
		}
	}
}

//==================================================================================
//壁との当たり判定
//==================================================================================
void CollisionWallPlayer(void)
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
				g_aPlayer.pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * -fLen);

			//右の頂点座標
			D3DXVECTOR3 pos1 = D3DXVECTOR3(
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen,
				g_aPlayer.pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen);

			//境界線のベクトル
			D3DXVECTOR3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			//プレイヤーの境界線のベクトル
			D3DXVECTOR3 vecLinePlayer;
			vecLinePlayer.x = g_aPlayer.pos.x - g_aPlayer.posOld.x;
			vecLinePlayer.z = g_aPlayer.pos.z - g_aPlayer.posOld.z;

			//プレイヤーと壁のベクトル
			D3DXVECTOR3 vecToPosPlayer;
			vecToPosPlayer.x = pos1.x - g_aPlayer.posOld.x;
			vecToPosPlayer.z = pos1.z - g_aPlayer.posOld.z;

			//面積の最大値
			float fMaxAreaPlayer = (vecLinePlayer.z * vecLine.x) - (vecLinePlayer.x * vecLine.z);

			//今回の面積
			float fNowAreaPlayer = (vecToPosPlayer.z * vecLine.x) - (vecToPosPlayer.x * vecLine.z);

			//割合
			float fRatePlayer = fNowAreaPlayer / fMaxAreaPlayer;

			if (fRatePlayer >= 0.0f && fRatePlayer <= 1.0f)
			{//面積の範囲内にいたら判定

				if ((vecLinePlayer.z * vecToPosPlayer.x) - (vecLinePlayer.x * vecToPosPlayer.z) > 0)
				{//壁に当たったら

					//交点からはみ出た分
					D3DXVECTOR3 CollisionPointPlayer = D3DXVECTOR3(0.0f, g_aPlayer.pos.y, 0.0f);
					CollisionPointPlayer.x = (g_aPlayer.pos.x + (vecLinePlayer.x * (fRatePlayer - 1.0f)));
					CollisionPointPlayer.z = (g_aPlayer.pos.z + (vecLinePlayer.z * (fRatePlayer - 1.0f)));

					//法線ベクトル(境界線ベクトルのXとZ反転)
					D3DXVECTOR3 vecNor = D3DXVECTOR3(vecLine.z, 0.0f, -vecLine.x);

					//ベクトルの正規化
					D3DXVec3Normalize(&vecNor, &vecNor);

					//プレイヤーの逆移動量
					D3DXVECTOR3 PlayerInverceMove;
					PlayerInverceMove.z = vecLinePlayer.z * (fRatePlayer - 1.0f);
					PlayerInverceMove.x = vecLinePlayer.x * (fRatePlayer - 1.0f);

					//内積(壁の法線とプレイヤーの逆移動量)
					float fDot = (PlayerInverceMove.x * vecNor.x) + (PlayerInverceMove.z * vecNor.z);

					//壁ずり移動量
					D3DXVECTOR3 MoveWall = vecNor * fDot;

					D3DXVECTOR3 ppp = (CollisionPointPlayer - g_aPlayer.pos);
					D3DXVECTOR3 pppp = ppp + MoveWall;

					//ぶつかった点に補正
					g_aPlayer.pos += MoveWall + (vecNor * 0.1f);

				}

			}

		}

	}
}

//==================================================================================
//弾のヒット処理
//==================================================================================
void HitPlayer(D3DXVECTOR3 move, int nDMG)
{

	////エディットの情報取得
	//Edit *pEdit = GetEdit();

	//if (g_aPlayer.bUlt == false && g_aPlayer.bUse == true && g_aPlayer.nState == PLAYERSTATE_NONE && pEdit->bUse == false)
	//{//ウルト状態じゃないとき

	//	g_aPlayer.move = move;	//移動量加算
	//	g_aPlayer.nLife -= nDMG;	//体力減らす

	//	if (g_aPlayer.nLife <= 0)
	//	{//体力がなくなった

	//		g_aPlayer.nLife = 0;
	//		//g_aPlayer.bUse = false;
	//		g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//		//サウンド停止
	//		SetGameState(GAMESTATE_END, 120);
	//	}
	//	else
	//	{//まだ体力が残ってる

	//		g_aPlayer.nCntState = DMG_TIME;	//ダメージ状態を保つ時間を与える

	//		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	//		{//パーツ分繰り返す

	//			D3DXMATERIAL *pMat;	//マテリアルデータへのポインタ

	//								//マテリアルデータへのポインタを取得
	//			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

	//			//頂点数分繰り返し
	//			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
	//			{//マテリアルを赤くする

	//				pMat[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//				pMat[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	//			}
	//		}
	//	}
	//}
}


//==================================================================================
//プレイヤーの状態更新
//==================================================================================
void UpdateStatePlayer(void)
{

	//カメラの情報取得
	Camera *pCamera = GetCamera();



	D3DXMATERIAL *pMat;	//マテリアルデータへのポインタ

	switch (g_aPlayer.nState)
	{
	case PLAYERSTATE_NONE:

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//パーツ分繰り返す

		 //マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
			{
				//元の色に戻す
				pMat[nCntMat].MatD3D.Diffuse = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Ambient = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Ambient;
			}
		}
		break;
	}
}

//==================================================================================
//キャラクターとの当たり判定
//==================================================================================
void CollisionCharPlayer(void)
{
	//敵の情報取得
	Enemy *pEnemy = GetEnemy();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true && pEnemy->bMoreSpawn == true)
		{//敵が使用されていたら

		 //キャラクター同士の当たり判定
			CollisionCharacter(
				&g_aPlayer.pos, &g_aPlayer.posOld, D3DXVECTOR3(g_aPlayer.fRadius, 0.0f, g_aPlayer.fRadius), D3DXVECTOR3(-g_aPlayer.fRadius, 0.0f, -g_aPlayer.fRadius),
				&pEnemy->pos, D3DXVECTOR3(pEnemy->fRadius, 0.0f, pEnemy->fRadius), D3DXVECTOR3(-pEnemy->fRadius, 0.0f, -pEnemy->fRadius));
		}
	}


	//モデルの情報取得
	ModelModel *pModel = GetModel();

	//大量発生の情報取得	
	ManySpawn *pManySpawn = GetManySpawn();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true && (pModel->nType < SMALL_NUM || pModel->nType >= CONE_NUM))
		{//モデルが使用されていたら

			//キャラクター同士の当たり判定
			CollisionCharacter(
				&g_aPlayer.pos, &g_aPlayer.posOld, D3DXVECTOR3(g_aPlayer.fRadius, 0.0f, g_aPlayer.fRadius), D3DXVECTOR3(-g_aPlayer.fRadius, 0.0f, -g_aPlayer.fRadius),
				&pModel->posOrigin, pModel->vtxMax, pModel->vtxMin);

			float fLength = g_aPlayer.fRadius + FLEN;

			if (CircleRange(pManySpawn->aSpawn[pModel->nType].pos, g_aPlayer.pos) <= (fLength * fLength) &&
				pManySpawn->aSpawn[pModel->nType].nMany == 1 &&
				pModel->nType < 40 && 
				pManySpawn->aSpawn[pModel->nType].bUse == false)
			{
				pManySpawn->aSpawn[pModel->nType].bUse = true;
			}
		}
	}
}

//==================================================================================
//プレイヤーのモーション処理
//==================================================================================
void SetMotisonPlayer(int nMotionType)
{

	//if (g_aPlayer.nNowMotionNum != nMotionType)
	//{

	//	for (int nCntKey = 0; nCntKey < g_aMotionData[nMotionType].nNumKey; nCntKey++)
	//	{//キー数分繰り返す

	//		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	//		{//パーツ分繰り返す

	//			g_aPlayer.aModel[nCntParts].rot = g_aMotionData[nMotionType].aKey[nCntKey].aParts[nCntParts].rot;
	//			g_aPlayer.aModel[nCntParts].pos = g_aPlayer.aModel[nCntParts].posOrigin;
	//		}

	//		g_aPlayer.nCntFrame = 0;	//フレームカウントリセット
	//	}

	//	//今のモーションの更新
	//	g_aPlayer.nNowMotionNum = nMotionType;

	//	g_aPlayer.nPatternKey = 0;	//現在のキー番号リセット
	//	g_aPlayer.nCntAllFrame = 0;	//総フレーム数リセット
	//}


	//for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	//{//パーツ分繰り返す

	//	//元の向きと現在の向きで目標の向きを求める
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDest = g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot;
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDest = g_aPlayer.aModel[nCntParts].posOrigin + g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos;

	//	//現在と目標の差分を求める
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff = g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDest - g_aPlayer.aModel[nCntParts].rot;
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDiff = g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDest - g_aPlayer.aModel[nCntParts].pos;

	//	//角度の正規化
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.x = RotNormalize(g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.x);
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.y = RotNormalize(g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.y);
	//	g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.z = RotNormalize(g_aMotionData[nMotionType].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff.z);

	//}

	//今のモーションの更新
	g_aPlayer.nNowMotionNum = nMotionType;
	g_aPlayer.nPatternKey = 0;	//現在のキー番号リセット
	g_aPlayer.nCntFrame = 0;	//フレームカウントリセット
	g_aPlayer.nCntAllFrame = 0;	//総フレーム数リセット

}

//==================================================================================
//プレイヤーのモーション更新処理
//==================================================================================
void UpdateMotionPlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	//{//パーツ分繰り返す

	//	//差分を求めて規定フレームで終わるように補正
	//	g_aPlayer.aModel[nCntParts].rot +=
	//		g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rotDiff /	//目的と現在の差分
	//		(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame;					//再生フレームで割る

	//	//位置
	//	g_aPlayer.aModel[nCntParts].pos +=
	//		g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDiff /	//目的と現在の差分
	//		(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame;					//再生フレームで割る

	//	if (g_aPlayer.aModel[nCntParts].nParent == -1)
	//	{//親の場合

	//		//マトリックスの取得
	//		D3DXMATRIX mtxwood = GetParentMatrix(nCntParts);

	//		g_aPlayer.pos.x += 
	//			sinf(g_aPlayer.rot.y) * (g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDiff.z /	//目的と現在の差分
	//			(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);			//再生フレームで割る

	//		g_aPlayer.pos.z += 
	//			cosf(g_aPlayer.rot.y) * (g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].posDiff.z /	//目的と現在の差分
	//			(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);			//再生フレームで割る
	//	}

	//	//角度の正規化
	//	g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x);
	//	g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y);
	//	g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z);

	//}

	////フレームのカウントを加算
	//g_aPlayer.nCntFrame++;
	//g_aPlayer.nCntAllFrame++;

	//if (g_aPlayer.nCntFrame >= g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame)
	//{//フレームのカウントがフレーム数に達したら

	//	//フレームのカウントをゼロに戻す
	//	g_aPlayer.nCntFrame = 0;

	//	//パターンNO.更新
	//	g_aPlayer.nPatternKey = (g_aPlayer.nPatternKey + 1) % g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

	//	if (g_aPlayer.nPatternKey == 0)
	//	{//キーがゼロの時

	//		//攻撃状態解除
	//		g_aPlayer.bATK = false;
	//		g_aPlayer.nCntAllFrame = 0;	//総フレーム数リセット

	//		if (g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
	//		{

	//			//モーションの設定
	//			SetMotisonPlayer(PLAYERMOTION_DEF);
	//		}
	//		else
	//		{
	//			//モーションの設定
	//			SetMotisonPlayer(g_aPlayer.nNowMotionNum);
	//		}

	//	}
	//	else
	//	{
	//		//モーションの設定
	//		SetMotisonPlayer(g_aPlayer.nNowMotionNum);
	//	}

	//}

	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//パーツ分繰り返す

	 //次のキー
		int nNextKey = (g_aPlayer.nPatternKey + 1) % g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey;
		int nnn;

		if (g_aPlayer.nPatternKey == g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
		{//ループしないとき

		 //nNextKey = g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		if (g_aPlayer.nPatternKey == g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_ON)
		{//ループの時

			nnn = g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		//前回と次のキーの差分取得
		float rotDiffX = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z;

		//パーツの向きを設定
		g_aPlayer.aModel[nCntParts].rot.x =
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		g_aPlayer.aModel[nCntParts].rot.y =
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		g_aPlayer.aModel[nCntParts].rot.z =
			g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		//角度の正規化
		g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x);
		g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y);
		g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z = RotNormalize(g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z);


		//パーツの位置を設定
		if (nCntParts == 0 && GetMode() != MODE_RANKING && (GetMode() == MODE_GAME && GetGameState() != GAMESTATE_BEFOREEND))
		{
			//目標の位置との差分を求める
			float posDiffX = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = g_aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z;

			//位置補正
			g_aPlayer.pos.x +=
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x +
				posDiffX *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);

			g_aPlayer.pos.y =
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y +
				posDiffY *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);

			g_aPlayer.pos.z +=
				g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z +
				posDiffZ *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);
		}
	}

	//フレームのカウントを加算
	g_aPlayer.nCntFrame++;
	g_aPlayer.nCntAllFrame++;

	if (g_aPlayer.nCntFrame >= g_aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame)
	{//フレームのカウントがフレーム数に達したら

	 //フレームのカウントをゼロに戻す
		g_aPlayer.nCntFrame = 0;

		//パターンNO.更新
		g_aPlayer.nPatternKey = (g_aPlayer.nPatternKey + 1) % g_aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

		if (g_aPlayer.nPatternKey == 0)
		{//キーがゼロの時

			//攻撃状態解除
			g_aPlayer.bATK = false;
			g_aPlayer.nCntAllFrame = 0;	//総フレーム数リセット

			if (g_aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
			else
			{
				//モーションの設定
				SetMotisonPlayer(g_aPlayer.nNowMotionNum);
			}
		}
	}
}

//==================================================================================
//プレイヤーの描画処理
//==================================================================================
void DrawPlayer(void)
{


	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATRIX mtxnParent;			//親の計算用マトリックス

	if (g_aPlayer.bDisp == true && g_aPlayer.bUse == true)
	{//使用していたら

	 //ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//向きを反映する
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//位置を反映する
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

		//現在のマテリアルを取得
		pDevice->GetMaterial(&matDef);

		for (int nCount = 0; nCount < g_aPlayer.nPartsNum; nCount++)
		{

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCount].rot.y, g_aPlayer.aModel[nCount].rot.x, g_aPlayer.aModel[nCount].rot.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCount].pos.x, g_aPlayer.aModel[nCount].pos.y, g_aPlayer.aModel[nCount].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxTrans);


			if (g_aPlayer.aModel[nCount].nParent == -1)
			{//NULLの場合,親

			 //プレイヤーの向きを反映する
				mtxnParent = g_aPlayer.mtxWorld;
			}
			else
			{//パーツ

			 //親の向きを反映する
				mtxnParent = g_aPlayer.aModel[g_aPlayer.aModel[nCount].nParent].mtxWorld;
			}

			//自分のやつに親のワールドマトリックスを掛け算
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxnParent);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCount].mtxWorld);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCount].pBuffMat->GetBufferPointer();

			if (g_aPlayer.aModel[nCount].bUse == true)
			{//パーツが使用されていたら

			 //頂点数分繰り返し
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCount].dwNumMat; nCntMat++)
				{

					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//テクスチャの設定
					pDevice->SetTexture(0, g_aPlayer.aModel[nCount].pTexture[nCntMat]);

					//プレイヤー(パーツ)の描画
					g_aPlayer.aModel[nCount].pMesh->DrawSubset(nCntMat);

				}
			}
		}

		//保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);
	}

}

//==================================================================================
//プレイヤーの情報取得
//==================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}

//==================================================================================
//プレイヤーの設定処理
//==================================================================================
void SetPlayer(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//パーツ分繰り返す

	 //Xファイルの読み込み
		D3DXLoadMeshFromX(&g_aPlayer.aModel[nCntParts].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aPlayer.aModel[nCntParts].pBuffMat,
			NULL,
			&g_aPlayer.aModel[nCntParts].dwNumMat,
			&g_aPlayer.aModel[nCntParts].pMesh);

		D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

		//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

		//頂点数分繰り返し
		for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
		{
			//マテリアルのデータ取得
			g_aPlayer.aModel[nCntParts].pMatData[nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//ファイルが存在する

			 //テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aPlayer.aModel[nCntParts].pTexture[nCntMat]);
			}
		}

		if (g_aPlayer.aModel[nCntParts].nFever != 0)
		{
			//パーツを使用状態
			g_aPlayer.aModel[nCntParts].bUse = true;
		}
		else
		{
			g_aPlayer.aModel[nCntParts].bUse = false;
		}	
		

		//モデルの全頂点チェック
		CheckVtx(g_aPlayer.aModel[nCntParts].rot.y, &g_aPlayer.aModel[nCntParts].vtxMax, &g_aPlayer.aModel[nCntParts].vtxMin, g_aPlayer.aModel[nCntParts].pMesh, g_aPlayer.aModel[nCntParts].pVtxBuff);
	}

	//影の設定
	if (GetMode() != MODE_TITLE)
	{
		g_aPlayer.nIdxShadow = SetShadow(g_aPlayer.fRadius * 0.5f, g_aPlayer.fRadius * 0.5f);
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
	}

	//アイコンの設定
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		g_aPlayer.nIdxIcon = SetIcon(MINIMAP_ICON_PLAYER, 100.0f);
		UpdatePositionIcon(g_aPlayer.nIdxIcon, g_aPlayer.pos);
	}

	//旗の設定
	g_aPlayer.nIdxFlag = SetFlag();
	SetPositionFlag(g_aPlayer.nIdxFlag, D3DXVECTOR3(g_aPlayer.pos.x, g_aPlayer.pos.y + FLAG_POS, g_aPlayer.pos.z));
}

//==================================================================================
//絶対マトリックスの取得
//==================================================================================
D3DXMATRIX GetParentMatrix(int nIdxParts)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxnParent;			//親の計算用マトリックス

									//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

	//向きを反映する
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

	//位置を反映する
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

	for (int nCount = 0; nCount < g_aPlayer.nPartsNum; nCount++)
	{
		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);

		//向きを反映する
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCount].rot.y, g_aPlayer.aModel[nCount].rot.x, g_aPlayer.aModel[nCount].rot.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxRot);

		//位置を反映する
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCount].pos.x, g_aPlayer.aModel[nCount].pos.y, g_aPlayer.aModel[nCount].pos.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxTrans);


		if (g_aPlayer.aModel[nCount].nParent == -1)
		{//NULLの場合,親

		 //プレイヤーの向きを反映する
			mtxnParent = g_aPlayer.mtxWorld;
		}
		else
		{//パーツ

		 //親の向きを反映する
			mtxnParent = g_aPlayer.aModel[g_aPlayer.aModel[nCount].nParent].mtxWorld;
		}

		//自分のやつに親のワールドマトリックスを掛け算
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxnParent);

		//ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCount].mtxWorld);
	}

	return g_aPlayer.aModel[nIdxParts].mtxWorld;
}

//==================================================================================
//プレイヤーの読み込み処理
//==================================================================================
void ReadSetPlayer(void)
{
	FILE *pFile = NULL;	//ファイルポインタを宣言

						//ファイルを開く
	pFile = fopen(PLAYER_TXT, "r");

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
				fscanf(pFile, "%d", &g_aPlayer.nPartsNum);	//モデル数
			}

			while (nCntFileName != g_aPlayer.nPartsNum)
			{//モデルの数分読み込むまで繰り返し

			 //文字列の読み込み
				fscanf(pFile, "%s", &aComment[0]);

				//モデル名の設定
				if (strcmp(aComment, "MODEL_FILENAME") == 0)
				{//NUM_MODELがきたら

					fscanf(pFile, "%s", &aComment[0]);	//=の分
					fscanf(pFile, "%s", &g_aPlayer.aModel[nCntFileName].acFilename[0]);	//ファイル名
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
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nType);	//モデル種類の列挙

							}

							if (strcmp(aComment, "PARENT") == 0)
							{//PARENTが来たら親の番号読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nParent);	//親の番号

							}

							if (strcmp(aComment, "POS") == 0)
							{//POSが来たら位置読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.x);	//X座標
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.y);	//Y座標
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.z);	//Z座標

																							//元の位置割り当て
								g_aPlayer.aModel[nCntSetParts].posOrigin = g_aPlayer.aModel[nCntSetParts].pos;

							}

							if (strcmp(aComment, "ROT") == 0)
							{//ROTが来たら向き読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.x);	//Xの角度
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.y);	//Yの角度
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.z);	//Zの角度

							}

							if (strcmp(aComment, "WEAPON") == 0)
							{//WEAPONが来たら武器かどうか読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分

							}

							if (strcmp(aComment, "FEVER") == 0)
							{
								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nFever);
								
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

	//プレイヤーの配置
	SetPlayer();

}

//==================================================================================
//プレイヤーの読み込み処理
//==================================================================================
void ReadSetMotionPlayer(void)
{

	FILE *pFile = NULL;	//ファイルポインタを宣言

	char aComment[MAX_COMMENT];	//コメント用
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0;

	//ファイルを開く
	pFile = fopen("data\\TEXT\\motion_set_player.txt", "r");

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
						fscanf(pFile, "%d", &g_aMotionData[nCntMotion].nLoop);	//ループ0か1

					}

					if (strcmp(aComment, "NUM_KEY") == 0)
					{//NUM_KEYが来たらキー数読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aMotionData[nCntMotion].nNumKey);	//キー数

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
								fscanf(pFile, "%d", &g_aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//再生フレーム

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
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X座標
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y座標
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z座標
									}

									if (strcmp(aComment, "ROT") == 0)
									{//ROTが来たら角度読み込み

										fscanf(pFile, "%s", &aComment[0]);		//=の分
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X角度
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y角度
										fscanf(pFile, "%f", &g_aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z角度
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
	}
}

//==================================================================================
//チュートリアルの更新処理
//==================================================================================
void UpdateTutorialPlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	if (g_aPlayer.bUse == true)
	{//使用していたら

		//過去の位置保存
		g_aPlayer.posOld = g_aPlayer.pos;

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//パーツ分繰り返す

			//パーツごとのマトリックス取得
			D3DXMATRIX mtxParts = GetParentMatrix(nCntParts);

			g_aPlayer.aModel[nCntParts].OldmtxWorld = mtxParts;
		}


		//プレイヤーの操作
		ControllTutorialPlayer();

		//隊列の位置更新
		CalFormationPosPlayer();

		//攻撃処理
		UpdateATKGun();
		UpdateATKPenLight();

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//移動してるとき

			g_aPlayer.nCntWalk++;	//カウント加算

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//歩行パーティクルの設定
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				PlaySound(SOUND_LABEL_SE_ASIOTO);
			}


			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.bFever == false)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERWALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION && g_aPlayer.bFever == true)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_FEVERWALK);
			}

		}
		else
		{//ニュートラルモーション

			if (g_aPlayer.bATK == false && g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
			{
				//攻撃状態解除
				g_aPlayer.bATK = false;

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION ||
			g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION ||
			g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION ||
			g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
		{//催眠アクション中

			g_aPlayer.nCntPenlight++;

			if (g_aPlayer.nCntPenlight == 20 || g_aPlayer.nCntPenlight == 40)
			{
				if (g_aPlayer.nCntPenlight >= 40)
				{
					g_aPlayer.nCntPenlight = 0;
				}

				//催眠のホワンホワン設定
				if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION)
				{//止まって催眠

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
				{//動いて催眠
					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION)
				{//動いて催眠

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
				}
				else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
				{//動いて催眠

					g_aPlayer.nIdxWave[0] = SetHypnosisWavePlayer(g_aPlayer.rot.y, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
				}

				if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
				{//フィーバー中なら

					//催眠のホワンホワン設定
					if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERACTION)
					{//止まって催眠

						g_aPlayer.nIdxWave[1] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[2] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[3] = SetHypnosisWavePlayer(g_aPlayer.rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE) * PENLIGHT_RADIUS) / (float)40);
					}
					else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_FEVERMOVEACTION)
					{//動いて催眠

						g_aPlayer.nIdxWave[1] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[2] = SetHypnosisWavePlayer(g_aPlayer.rot.y + D3DX_PI * 1.0f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
						g_aPlayer.nIdxWave[3] = SetHypnosisWavePlayer(g_aPlayer.rot.y + -D3DX_PI * 0.5f, (sinf(PENLIGHT_ANGLE_MOVE) * PENLIGHT_RADIUS) / (float)40);
					}
				}
			}
		}
		else if (
			g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
			g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
		{//催眠アクションしてないとき

		 //催眠の音を消す
			StopSound(SOUND_LABEL_BGM_HYPNOSIS);
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVECURSOR)
		{
			DISPATCH *pDispatch = GetDispatch();

			//目標の角度を求める
			g_aPlayer.fRotDest = atan2f((g_aPlayer.pos.x - pDispatch->pos.x), (g_aPlayer.pos.z - pDispatch->pos.z));
			g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

			//角度の正規化
			g_aPlayer.fRotDiff = RotNormalize(g_aPlayer.fRotDiff);

			//角度の補正をする
			g_aPlayer.rot.y += g_aPlayer.fRotDiff;

			//角度の正規化
			g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);
		}

		//軌跡の情報取得
		ORBIT *pOrbit = GetMeshOrbit();

		//パーツごとのマトリックス取得
		D3DXMATRIX mtxParts = GetParentMatrix(10);

		pOrbit->pMtxparent = &mtxParts;

		//モーションの更新
		UpdateMotionPlayer();

		//キャラクターとの当たり判定
		CollisionCharPlayer();

		//座標制限
		LimitPosPlayer();

		//影の位置更新
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);

		//アイコンの位置更新
		UpdatePositionIcon(g_aPlayer.nIdxIcon, g_aPlayer.pos);

		//セリフの位置更新
		SetPositionSerif(g_aPlayer.nIdxSerif, { g_aPlayer.pos.x, g_aPlayer.pos.y + 150.0f, g_aPlayer.pos.z });

		//衝撃波の位置更新
		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
		{//移動催眠アクション中

			for (int nCntWave = 0; nCntWave < MAX_WAVEINDEX; nCntWave++)
			{
				SetPositionImpactWave(g_aPlayer.nIdxWave[nCntWave], g_aPlayer.move);
			}
		}

		//セリフの位置更新
		SetPositionSerif(g_aPlayer.nIdxSerif, { g_aPlayer.pos.x, g_aPlayer.pos.y + 150.0f, g_aPlayer.pos.z });
	}

	//状態更新
	UpdateStatePlayer();

}

//==================================================================================
//チュートリアルの操作
//==================================================================================
void ControllTutorialPlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//エディットの情報取得
	Edit  *pEdit = GetEdit();

	//ターゲットの情報取得
	DISPATCH *pDispatch = GetDispatch();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	if (/*GetGameState() == GAMESTATE_NONE && */pDispatch->bUse == false)
	{//ターゲット中以外

		//========================================================================
		//移動のステップ以上で操作可能
		//========================================================================
		if (pTutorial->nStep >= TUTORIAL_STEP_MOVE)
		{
			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVECURSOR)
			{//移動不可モーション中は強制キャンセル

				if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
				{//←キーが押された,左移動

					if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
					{//A+W,左上移動

						g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = D3DX_PI * MOVE_LRUP + pCamera->rot.y;
					}
					else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
					{//A+S,左下移動

						g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = D3DX_PI * MOVE_LRDW + pCamera->rot.y;
					}
					else
					{//A,左移動

						g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = D3DX_PI * MOVE_LR + pCamera->rot.y;
					}

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
					{//催眠していなかったら

						g_aPlayer.bMove = true;
					}

					//左移動完了
					pTutorial->aControl.bLMove = true;
				}
				else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
				{//Dキーが押された,右移動

					if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
					{//D+W,右上移動

						g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRUP + pCamera->rot.y;
					}
					else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
					{//D+S,右下移動

						g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRDW + pCamera->rot.y;
					}
					else
					{//D,右移動

						g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
						g_aPlayer.fRotDest = -D3DX_PI * MOVE_LR + pCamera->rot.y;
					}

					//右移動完了
					pTutorial->aControl.bRMove = true;

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
					{//催眠していなかったら

						g_aPlayer.bMove = true;
					}
				}
				else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//Wが押された、奥移動

					g_aPlayer.move.x += sinf(pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI + pCamera->rot.y;

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
					{//催眠していなかったら

						g_aPlayer.bMove = true;
					}

					//奥移動完了
					pTutorial->aControl.bUPMove = true;

				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//Sが押された、手前移動

					g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = pCamera->rot.y;

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION)
					{//催眠していなかったら

						g_aPlayer.bMove = true;
					}

					//手前移動完了
					pTutorial->aControl.bDWMove = true;
				}
				else
				{//なんのキーも押されていない

					if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f ||
						g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
					{
						g_aPlayer.bMove = false;

						if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
						{//移動催眠中の時

							//モーションの設定
							SetMotisonPlayer(PLAYERMOTION_ACTION);
						}

						if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION && g_aPlayer.bFever == true && g_aPlayer.bATK == true)
						{//移動催眠中の時

							//モーションの設定
							SetMotisonPlayer(PLAYERMOTION_FEVERACTION);
						}
					}
				}
			}
		}

		//========================================================================
		//派遣のステップ以上で操作可能
		//========================================================================
		if (pTutorial->nStep >= TUTORIAL_STEP_DISPATCH && pTutorial->nStep != TUTORIAL_STEP_MORS)
		{
			if (GetGamepadTrigger(BUTTON_RB, 0) || GetKeyboardTrigger(DIK_RSHIFT) == true)
			{//派遣の使用状況入れ替え

				SetRightDisPatch();
			}
			if (GetGamepadTrigger(BUTTON_LB, 0) || GetKeyboardTrigger(DIK_LSHIFT) == true)
			{//派遣の使用状況入れ替え

				SetLeftDisPatch();
			}
		}

		//========================================================================
		//催眠のステップ以上で操作可能
		//========================================================================
		if (pTutorial->nStep >= TUTORIAL_STEP_HYPNOSIS && pTutorial->nStep != TUTORIAL_STEP_MORS)
		{
			if (GetKeyboardPress(DIK_L) == true || GetGamepadPress(BUTTON_A, 0))
			{//Lが押された && Aボタンが押された

				if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
					g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION &&
					g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION &&
					g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION)
				{//催眠していなかったら

				 //催眠SE
					PlaySound(SOUND_LABEL_BGM_HYPNOSIS);
				}

				if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f &&
					g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
				{//止まっているときは強力

					g_aPlayer.bMove = false;
					g_aPlayer.move.x = 0.0f;
					g_aPlayer.move.z = 0.0f;

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION && g_aPlayer.bFever == false)
					{//フィーバー中じゃなかったら

						//モーションの設定
						SetMotisonPlayer(PLAYERMOTION_ACTION);
					}

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERACTION && g_aPlayer.bFever == true)
					{//フィーバー中だったら

						//モーションの設定
						SetMotisonPlayer(PLAYERMOTION_FEVERACTION);
					}
				}
				else
				{//動いてるときは弱

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION && g_aPlayer.bFever == false)
					{//fフィーバー中じゃなかったら

						//モーションの設定
						SetMotisonPlayer(PLAYERMOTION_MOVEACTION);
					}

					if (g_aPlayer.nNowMotionNum != PLAYERMOTION_FEVERMOVEACTION && g_aPlayer.bFever == true)
					{//フィーバー中だったら

						//モーションの設定
						SetMotisonPlayer(PLAYERMOTION_FEVERMOVEACTION);
					}
				}
			}

			if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
			{//Lが押された && Aボタンが離された

				//攻撃状態解除
				g_aPlayer.bATK = false;

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		//========================================================================
		//洗脳のステップ以上で操作可能
		//========================================================================
		if (pTutorial->nStep >= TUTORIAL_STEP_SENNOU && pTutorial->nStep != TUTORIAL_STEP_MORS)
		{
			if (g_aPlayer.bATK == false && g_aPlayer.bJump == false && GetKeyboardTrigger(DIK_RETURN) == true ||
				g_aPlayer.bATK == false && g_aPlayer.bJump == false && GetGamepadTrigger(BUTTON_B, 0))
			{//ENTERが押された,,弾発射

				//抱き枕の設定処理
				SetCutepillow(g_aPlayer.pos, g_aPlayer.rot, PILLOWPARENT_PLAYER);

				if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
				{//フィーバー中なら

					//全方位抱き枕
					SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + D3DX_PI * 0.5f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
					SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + D3DX_PI * 1.0f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
					SetCutepillow(g_aPlayer.pos, D3DXVECTOR3(g_aPlayer.rot.x, g_aPlayer.rot.y + -D3DX_PI * 0.5f, g_aPlayer.rot.z), PILLOWPARENT_PLAYER);
				}

				//SEセット
				PlaySound(SOUND_LABEL_SE_DAKIMAKURA);
			}
		}
	}

	//現在と目標の差分を求める
	g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

	//角度の正規化
	g_aPlayer.fRotDiff = RotNormalize(g_aPlayer.fRotDiff);

	//角度の補正をする
	g_aPlayer.rot.y += g_aPlayer.fRotDiff * 0.1f;

	//角度の正規化
	g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

	//重力処理
	//g_aPlayer.move.y -= 1.0f;

	if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
	{//フィーバー中なら

		//加速
		g_aPlayer.move.x *= 1.1f;
		g_aPlayer.move.z *= 1.1f;
	}

	if (pTutorial->nStep == TUTORIAL_STEP_MORS)
	{
		g_aPlayer.move.x *= 0.8f;
		g_aPlayer.move.z *= 0.8f;
	}

	//位置更新
	g_aPlayer.pos += g_aPlayer.move;

	//慣性つける
	if (g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION)
	{
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;
	}
	else if (g_aPlayer.nNowMotionNum == PLAYERMOTION_MOVEACTION)
	{
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR_HYPNOSIS;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR_HYPNOSIS;

		if (g_aPlayer.nState == PLAYERSTATE_FEAVER && g_aPlayer.bFever == true)
		{//フィーバー中なら

		 //加速
			g_aPlayer.move.x *= 1.1f;
			g_aPlayer.move.z *= 1.1f;
		}
	}
}

//==================================================================================
//タイトルの更新処理
//==================================================================================
void UpdateTitlePlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	if (g_aPlayer.bUse == true)
	{//使用していたら

		//過去の位置保存
		g_aPlayer.posOld = g_aPlayer.pos;

		//向き固定
		g_aPlayer.rot.y += 0.025f;

		g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

		g_aPlayer.fRotDiff = 0.0f;									//向きの差分
		g_aPlayer.fRotDest = g_aPlayer.rot.y;						//向きの差分

		g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y + g_aPlayer.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y + g_aPlayer.rot.y) * 1.0f;
		g_aPlayer.bMove = true;

		//位置更新
		g_aPlayer.pos += g_aPlayer.move;

		//慣性つける
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//移動してるとき

			g_aPlayer.nCntWalk++;	//カウント加算

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//歩行パーティクルの設定
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_WALK && g_aPlayer.nNowMotionNum != PLAYERMOTION_MOVEACTION)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_WALK);
			}

		}
		else
		{//ニュートラルモーション

			if (g_aPlayer.bATK == false && g_aPlayer.nNowMotionNum != PLAYERMOTION_DEF &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHR &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNL &&
				g_aPlayer.nNowMotionNum != PLAYERMOTION_RETURNR)
			{
				//攻撃状態解除
				g_aPlayer.bATK = false;

				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		if (g_aPlayer.nNowMotionNum == PLAYERMOTION_ACTION)
		{//催眠アクション中

			g_aPlayer.nCntPenlight++;

			
		}

		//モーションの更新
		UpdateMotionPlayer();

		//座標制限
		LimitPosPlayer();

		//影の位置更新
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
	}

	//状態更新
	UpdateStatePlayer();
}

//==================================================================================
//ランキングの更新処理
//==================================================================================
void UpdateRankingPlayer(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	if (g_aPlayer.bUse == true)
	{//使用していたら

		//過去の位置保存
		g_aPlayer.posOld = g_aPlayer.pos;

		//向き固定
		g_aPlayer.rot.y += 0.025f;

		g_aPlayer.rot.y = RotNormalize(g_aPlayer.rot.y);

		g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y + g_aPlayer.rot.y) * 1.0f;
		g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y + g_aPlayer.rot.y) * 1.0f;
		g_aPlayer.bMove = true;

		//位置更新
		g_aPlayer.pos += g_aPlayer.move;

		//慣性つける
		g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
		g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//移動してるとき

			g_aPlayer.nCntWalk++;	//カウント加算

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//歩行パーティクルの設定
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);
			}

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_RANKING)
			{
				//モーションの設定
				SetMotisonPlayer(PLAYERMOTION_RANKING);
			}

		}

		//モーションの更新
		UpdateMotionPlayer();

		//座標制限
		LimitPosPlayer();

		//影の位置更新
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
	}

	//状態更新
	UpdateStatePlayer();
}

//==================================================================================
//リザルトの更新処理
//==================================================================================
void UpdateResultPlayer(void)
{
	if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION)
	{//移動催眠中の時

		//モーションの設定
		SetMotisonPlayer(PLAYERMOTION_ACTION);
	}

	//モーションの更新
	UpdateMotionPlayer();

	PrintDebugProc(
		"---------------- プレイヤー情報 ----------------\n"
		"【視点】[X：%f Y：%f Z：%f]\n",
		g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
}