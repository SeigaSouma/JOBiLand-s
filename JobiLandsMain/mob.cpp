//=============================================================================
//
// モブ処理 [mob.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "mob.h"
#include "crowd.h"
#include "input.h"
#include "game.h"
#include "player.h"
#include "calculation.h"
#include "debugproc.h"
#include "rotate.h"
#include "hypnosis_gauge.h"
#include "calculation.h"
#include "shadow.h"
#include "buddy.h"
#include "minimap_icon.h"
#include "debugproc.h"
#include "iventsign.h"
#include "EventWindow.h"
#include "camera.h"
#include "manyUI.h"
#include "manypoint.h"
#include "eventstamp.h"
#include "tutorial.h"
#include "sound.h"
#include "comment.h"
#include "iventradar.h"
#include "endmessage.h"
#include "endcountdown.h"
#include "player.h"
#include "fenish.h"


//マクロ定義
#define MOVE		(0.9f)
#define DISTANCE	(60.0f)	//間隔
#define DISTANCE_LENGTH	(200.0f)	//間隔
#define MOB_NUMBER		(33)		//モブのモデルの番号
#define HYPSUB_INTER	(80)		//催眠度ゲージ減少間隔
#define HYPADD_INTER	(40)		//催眠度ゲージ増加間隔
#define ROT_CNT			(50)		//rotの切り替え時間
#define ROT_ROUND		(0.0025f)	//傾ける角度
#define NORMAL_SPAWN	(30)		//通常スポーンの間隔
#define MAX_MOVE		(451)		//モブの移動時間の最大値
#define MIN_MOVE		(100)		//モブの移動時間の最小値
#define NUM_BILL		(51)		//建物の数
#define MOVE_APPIEAR	(0.7f)		//出現時の移動量
#define MOVE_MANY_APPIEAR	(2.1f)	//フィーバー出現時の移動量
#define START_MOB		(200)		//開始時のモブの数
#define MAX_SPAWN_MOB	(700)		//場に出る最大モブ数
#define STOP_SPAWN_TIME	(70)		//大量発生開始から終了までの時間	現在35秒	60*75	60*175に終了
#define MANY_TIME		(30)		//大量発生時のスポーン間隔
#define SPWAN_DISTANCE	(100.0f)	//大量発生時のスポーン距離
//#define WAVE_CHANGE_TIME	(60 * 3)	//ウェーブ発生場所の変更時間
#define WAVE_SPAWN		(5)			//ウェーブ発生の間隔
#define WAVE_START		(60 * 3)	//ウェーブ発生スタート時間
#define WAVE_STOP		(60 * 5)	//ウェーブ終了時間
#define FLEN            (450.0f)     //

//プロトタイプ宣言
void ControllMob(int nCntMob);
void UpdateStateMob(int nCntMob);
void LimitPosMob(int nCntMob);
void CollisionModelMob(int nCntMob);
void ReadSetMobPos(void);
void UpdateCrawlMob(int nCntMob);
void UpdateSpawnMob(int nCntMob);
void ManySpawnMob(int nCntMob);
void ManySpawnSetPos(void);
void MobSerifSet(int nCntMob);

//グローバル変数宣言
const char *c_apFilenameMob[] =					//ファイル読み込み
{
	"data\\TEXTURE\\crowd02.jpg",
	"data\\TEXTURE\\crowd05.jpg",
	"data\\TEXTURE\\crowd04.jpg",
	"data\\TEXTURE\\crowd03.jpg",
	"data\\TEXTURE\\crowd07.jpg",
	"data\\TEXTURE\\crowd08.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureMob[(sizeof c_apFilenameMob) / sizeof(*c_apFilenameMob)] = {};	//テクスチャのポインタ
Mob g_aMob[MAX_MOB];		//モブの情報
ManySpawn g_aManySpawn;		//大量発生の情報
SwitchSpawn g_aUseSpawn;
WaveSpawn g_aWaveSpawn;		//通常発生（ウェーブ）の情報
SwitchSE g_aSwitchSE;		//SE使用状況

int g_nSpawnCounter;		//スポーンする時間
int g_nManySpawnCounter;	//大量発生でスポーンする時間
int g_nNumMob;
bool g_bManySpawn;
int g_nGameTime;		//ゲーム時間
int g_nMobSerifTime;		//セリフ設定する時間

//==================================================================================
//モブの初期化処理
//==================================================================================
void InitMob(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMob) / sizeof(*c_apFilenameMob); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMob[nCntTex],
			&g_apTextureMob[nCntTex]);
	}

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//各要素初期化
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		g_aMob[nCntMob].nHypnosis = 0;			//催眠度
		g_aMob[nCntMob].nSubHypCounter = HYP_CNTTIME;		//催眠度減少するまでのカウンター
		g_aMob[nCntMob].nSubHypInter = 0;		//催眠度を減らす間隔
		g_aMob[nCntMob].nAddHypInter = HYPADD_INTER;		//催眠度を増やす間隔
		g_aMob[nCntMob].nSwitchCounter = 0;		//rotの切り替え時間
		g_aMob[nCntMob].nTurnSwitchX = 1;		//rot.xの+-切り替え
		g_aMob[nCntMob].nTurnSwitchZ = 1;		//rot.zの+-切り替え
		g_aMob[nCntMob].nSpawnCounter = 0;		//モブのスポーンする時間
		g_aMob[nCntMob].nSpawnNumber = 0;		//モブが一度にスポーンする数
		g_aMob[nCntMob].nSpawnType = 0;			//スポーン地点の種類
		g_aMob[nCntMob].nMoveCounter = 0;		//モブの移動のカウンター
		g_aMob[nCntMob].nIdxIcon = 0;			//ミニマップアイコンのインデックス番号
		g_aMob[nCntMob].nTexType = 0;			//テクスチャタイプ

		g_aMob[nCntMob].fAlpha = 0.4f;				//透明度
		g_aMob[nCntMob].fRandAlpha = (float)(rand() % 51 + 13) * 0.0001f;			//透明度の増加数


		g_aMob[nCntMob].aModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aMob[nCntMob].aModel.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aMob[nCntMob].aModel.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
		g_aMob[nCntMob].aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aMob[nCntMob].aModel.rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
		g_aMob[nCntMob].aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMob[nCntMob].aModel.mtxWorld);
		D3DXMatrixIdentity(&g_aMob[nCntMob].aModel.OldmtxWorld);

		g_aMob[nCntMob].aModel.nIdxShadow = -1;			//影のインデックス番号
		g_aMob[nCntMob].aModel.nIdxRotate = -1;	//ぐるぐるのインデックス番号
		g_aMob[nCntMob].aModel.nIdxHypGauge = -1;		//体力ゲージのインデックス番号
		g_aMob[nCntMob].aModel.nIdxSerif = -1;			//セリフのインデックス番号
		g_aMob[nCntMob].aModel.nShadow = 1;			//影を使うかどうか

		g_aMob[nCntMob].aModel.nNumVtx = 0;			//頂点数
		g_aMob[nCntMob].aModel.dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aMob[nCntMob].aModel.pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aMob[nCntMob].aModel.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
		g_aMob[nCntMob].aModel.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

		g_aMob[nCntMob].aModel.nType = 0;			//読み込むモデルの種類
		g_aMob[nCntMob].aModel.nParent = -1;		//親の番号
		g_aMob[nCntMob].aModel.bUse = false;			//使用しているか
		g_aMob[nCntMob].aModel.nState = 0;			//状態

		g_aMob[nCntMob].aModel.pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
		g_aMob[nCntMob].aModel.pBuffMat = NULL;		//マテリアルへのポインタ

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aMob[nCntMob].aModel.pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
			g_aMob[nCntMob].aModel.pMatData[nCntMat].pTextureFilename = NULL;	//マテリアルのデータ
			g_aMob[nCntMob].aModel.acFilename[nCntMat] = NULL;	//ファイル名
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aMob[nCntMob].aModel.pTexture[nCntTex] = NULL;	//テクスチャへのポインタ
		}
		g_aMob[nCntMob].aModel.dwNumMat = NULL;			//マテリアルの数
	}

	//大量発生情報の初期化
	g_aManySpawn.nSpawnCounter = 0;		//大量発生するまでの時間
	g_aManySpawn.nStopTime = 0;			//大量発生終了までの時間
	g_aManySpawn.nBillNumber = rand() % NUM_BILL;	//建物の場所
	g_aManySpawn.nCounter = 0;
	g_aManySpawn.bFirst = false;		//1番目のスポーン
	g_aManySpawn.bSecond = false;		//2番目のスポーン
	g_aManySpawn.bThird = false;		//3番目のスポーン

	//ウェーブ発生の初期化
	g_aWaveSpawn.nStartSpawn = 0;			//発生開始時間
	g_aWaveSpawn.nStopSpawn = 0;			//発生終了時間
	g_aWaveSpawn.nPosChangeCounter = 0;		//発生位置変えるまでの時間
	g_aWaveSpawn.nSpawnCounter = 0;			//発生するまでの時間
	g_aWaveSpawn.nBillNumber = rand() % NUM_BILL;	//建物の場所

	//スポーン情報
	for (int nCntSpawn = 0; nCntSpawn < MAX_SPAWN; nCntSpawn++)
	{
		g_aManySpawn.aSpawn[nCntSpawn].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//スポーン地点
		g_aManySpawn.aSpawn[nCntSpawn].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//スポーン地点の向き
		g_aManySpawn.aSpawn[nCntSpawn].nType = 0;				//スポーンする地点の種類
		g_aManySpawn.aSpawn[nCntSpawn].nMany = 0;				//大量発生するかの判定
		g_aManySpawn.aSpawn[nCntSpawn].bUse = false;            //建物が大量発生地点かの判定
	}

	g_nSpawnCounter = 0;		//スポーンする時間
	g_nManySpawnCounter = 0;
	g_nNumMob = 0;
	g_aUseSpawn.bUseSpawn = false;	//大量発生してるか
	g_nGameTime = 0;	//ゲーム時間
	g_nMobSerifTime = 0;	//セリフの設定する時間
	g_aSwitchSE.bUseRotate = false;		//ぐるぐるSE使用してるか

#if _DEBUG

	//大量発生しない
	g_bManySpawn = false;

#else NDEBUG

	//大量発生する
	g_bManySpawn = true;

#endif

	//モブのスポーン位置の読み込み
	ReadSetMobPos();

	if (GetMode() == MODE_GAME)
	{
		while (1)
		{//大量発生する場所を決める

			g_aManySpawn.nBillNumber = rand() % NUM_BILL;

			if (g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nMany == 1)
			{//大量発生する位置だったら

				break;
			}
		}

		//開始時のモブを設定
		for (int nCntMob = 0; nCntMob < START_MOB; nCntMob++)
		{
			int nCntNumber = rand() % NUM_BILL;		//建物の種類

			//モブの設定
			SetMob(g_aManySpawn.aSpawn[nCntNumber].pos, g_aManySpawn.aSpawn[nCntNumber].rot, g_aManySpawn.aSpawn[nCntNumber].nType, rand() % 3 + MOB_NUMBER);
		}
	}
}

//==================================================================================
//モブの終了処理
//==================================================================================
void UninitMob(void)
{

	StopSound();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMob) / sizeof(*c_apFilenameMob); nCntTex++)
	{
		if (g_apTextureMob[nCntTex] != NULL)
		{
			g_apTextureMob[nCntTex]->Release();
			g_apTextureMob[nCntTex] = NULL;
		}
	}

}

//==================================================================================
//モブの更新処理
//==================================================================================
void UpdateMob(void)
{
	//モデルの情報取得
	ModelModel *pModel = GetModel();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();
	Rotate *pRotate = GetRotate();

	if (GetGameState() == GAMESTATE_NONE)
	{
		//スポーンする時間を増加する
		g_nSpawnCounter++;
		g_nManySpawnCounter++;
		g_aWaveSpawn.nPosChangeCounter++;
		g_aWaveSpawn.nSpawnCounter++;
		g_aWaveSpawn.nStartSpawn++;

		g_nGameTime++;

		if (g_bManySpawn == true)
		{//ONの時

			//大量発生するまでの時間
			g_aManySpawn.nSpawnCounter++;

			PrintDebugProc("[F9]：警備強化・大量発生[ON]\n");
		}
		else
		{
			PrintDebugProc("[F9]：警備強化・大量発生[OFF]\n");
		}
	}

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F9) == true)
	{//F9が押された

		//大量発生切り替え
		g_bManySpawn = g_bManySpawn ? false : true;
	}
#endif

	int nUse = 0;
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		if (g_aMob[nCntMob].aModel.bUse == true)
		{//使用していたら

			//使ってる数加算
			nUse++;

			//セリフ設定時間加算
			g_nMobSerifTime++;

			//過去の位置保存
			g_aMob[nCntMob].aModel.posOld = g_aMob[nCntMob].aModel.pos;

			//モブの操作
			ControllMob(nCntMob);

			//モブ同士の当たり判定
			CollisionModelMob(nCntMob);

			//座標制限
			LimitPosMob(nCntMob);

			//状態更新
			UpdateStateMob(nCntMob);

			//催眠度ゲージを使用している状態にする
			/*if (g_aMob[nCntMob].aModel.nIdxHypGauge >= MAX_MOB)
			{
			int a = 0;
			}*/

			//ぐるぐるの位置更新
			SetPositionRotate(g_aMob[nCntMob].aModel.nIdxRotate, g_aMob[nCntMob].aModel.pos);

			//催眠度ゲージの位置更新
			SetPositionHypnosisGauge(g_aMob[nCntMob].aModel.nIdxHypGauge, g_aMob[nCntMob].aModel.pos, g_aMob[nCntMob].nHypnosis);

			//影の位置更新
			SetPositionShadow(g_aMob[nCntMob].aModel.nIdxShadow, g_aMob[nCntMob].aModel.pos);

			//アイコンの位置更新
			UpdatePositionIcon(g_aMob[nCntMob].nIdxIcon, g_aMob[nCntMob].aModel.pos);

			//セリフの設定処理
			MobSerifSet(nCntMob);

			if (g_aMob[nCntMob].aModel.nState != MOBSTATE_APPIEAR)
			{
				//セリフの位置更新
				SetPositionSerif(g_aMob[nCntMob].aModel.nIdxSerif, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			}

			if (g_aMob[nCntMob].aModel.nState != MOBSTATE_HYPNOSIS && g_aMob[nCntMob].aModel.nState != MOBSTATE_KEEP)
			{//催眠状態、催眠かけてる最中以外の時

				//ぐるぐる削除
				pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = false;
			}
		}

		//モブの発生
		UpdateSpawnMob(nCntMob);
	}

	g_nNumMob = nUse;

	PrintDebugProc("モブの数%d\n", g_nNumMob);

	PrintDebugProc("大量発生の位置 [X:%f Y:%f Z:%f]\n",
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x, g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y, g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z);

	if (g_aUseSpawn.bUseSpawn == true)
	{
		PrintDebugProc("<<<<<大量発生中>>>>>\n");
	}

	//ゲーム時間
	PrintDebugProc("ゲーム時間：%d\n", g_nGameTime / 60);

	//ゲーム終了30秒前
	if (g_nGameTime == GAMETIME - (60 * 30))
	{
		SetEndmessage({ 1280.0f * 0.5f, 300.0f, 0.0f }, 150);
	}

	//ゲーム終了5秒前
	if (g_nGameTime == GAMETIME - (60 * 5))
	{
		SetEndCountDown(0);
	}

	//ゲーム終了
	if (g_nGameTime == GAMETIME)
	{
		SetFinish(0);
	}
}

//==================================================================================
//モブの操作
//==================================================================================
void ControllMob(int nCntMob)
{

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//重力処理
	g_aMob[nCntMob].aModel.move.y -= GRAVITY;

	//位置更新
	g_aMob[nCntMob].aModel.pos += g_aMob[nCntMob].aModel.move;

	//慣性
	g_aMob[nCntMob].aModel.move.x += (0.0f - g_aMob[nCntMob].aModel.move.x) * 0.15f;
	g_aMob[nCntMob].aModel.move.z += (0.0f - g_aMob[nCntMob].aModel.move.z) * 0.15f;
}

//==================================================================================
//モブの座標制限
//==================================================================================
void LimitPosMob(int nCntMob)
{

	if (g_aMob[nCntMob].aModel.pos.y < 0.0f)
	{//地面より下に行ったら

		g_aMob[nCntMob].aModel.pos.y = 0.0f;

		//重力処理
		g_aMob[nCntMob].aModel.move.y = 0.0f;
	}

	//移動制限
	if (g_aMob[nCntMob].aModel.pos.x + g_aMob[nCntMob].aModel.vtxMin.x < -LIMIT_POS) { g_aMob[nCntMob].aModel.pos.x = -LIMIT_POS - g_aMob[nCntMob].aModel.vtxMin.x; }
	if (g_aMob[nCntMob].aModel.pos.x + g_aMob[nCntMob].aModel.vtxMax.x > LIMIT_POS) { g_aMob[nCntMob].aModel.pos.x = LIMIT_POS - g_aMob[nCntMob].aModel.vtxMax.x; }
	if (g_aMob[nCntMob].aModel.pos.z + g_aMob[nCntMob].aModel.vtxMax.z > LIMIT_POS) { g_aMob[nCntMob].aModel.pos.z = LIMIT_POS - g_aMob[nCntMob].aModel.vtxMax.z; }
	if (g_aMob[nCntMob].aModel.pos.z + g_aMob[nCntMob].aModel.vtxMin.z < -LIMIT_POS) { g_aMob[nCntMob].aModel.pos.z = -LIMIT_POS - g_aMob[nCntMob].aModel.vtxMin.z; }

}

//==================================================================================
//モブの発生処理
//==================================================================================
void UpdateSpawnMob(int nCntMob)
{
	//イベントサインの情報取得
	IventSign *pIventSign = GetIventSign();

	//大量発生地点の情報取得
	MANYPOINT *pManyPoint = GetManyPoint();

	//イベントスタンプの情報取得
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//コメントの情報取得
	Commnet *pComment = GetCommnet();

	if (g_aManySpawn.nStopTime < STOP_SPAWN_TIME)
	{//一定時間経過したら大量発生

		if (g_nManySpawnCounter >= MANY_TIME)
		{//スポーンの速さ

			if (g_aManySpawn.nSpawnCounter >= SECOND_SPAWNMOB - 330 && g_aManySpawn.bSecond == false)
			{//一定時間経過したら

				//コメントを使用する状態にする
				pComment->bUse = true;
			}
			else if (g_aManySpawn.nSpawnCounter >= FIRST_SPAWNMOB - 330 && g_aManySpawn.bFirst == false)
			{//一定時間経過したら

				//コメントを使用する状態にする
				pComment->bUse = true;
			}
			else if (g_aManySpawn.nSpawnCounter >= THIRD_SPAWNMOB - 330 && g_aManySpawn.bThird == false)
			{//一定時間経過したら

				//コメントを使用する状態にする
				pComment->bUse = true;
			}

			if (g_aManySpawn.nSpawnCounter >= SECOND_SPAWNMOB && g_aManySpawn.bSecond == false)
			{//一定時間経過したら

				//大量発生のスポーン
				ManySpawnMob(nCntMob);

				//コメントを消す
				pComment->bUse = false;
			}
			else if (g_aManySpawn.nSpawnCounter >= FIRST_SPAWNMOB && g_aManySpawn.bFirst == false)
			{//一定時間経過したら

				//大量発生のスポーン
				ManySpawnMob(nCntMob);

				//コメントを消す
				pComment->bUse = false;
			}
			else if (g_aManySpawn.nSpawnCounter >= THIRD_SPAWNMOB && g_aManySpawn.bThird == false)
			{//一定時間経過したら

				//大量発生のスポーン
				ManySpawnMob(nCntMob);

				//コメントを消す
				pComment->bUse = false;
			}
		}
	}
	else if (g_aManySpawn.nStopTime >= STOP_SPAWN_TIME)
	{//一定時間経過したら初期化

		//何回目の大量発生か
		if (g_aManySpawn.nSpawnCounter >= (THIRD_SPAWNMOB + STOP_SPAWN_TIME) && g_aManySpawn.bThird == false)
		{//3番目

			g_aManySpawn.bThird = true;			//3番目のスポーン

			//大量発生位置設定
			ManySpawnSetPos();

		}
		else if (g_aManySpawn.nSpawnCounter >= (SECOND_SPAWNMOB + STOP_SPAWN_TIME) && g_aManySpawn.bSecond == false)
		{//2番目

			g_aManySpawn.bSecond = true;		//2番目のスポーン

			//大量発生位置設定
			ManySpawnSetPos();

		}
		else if (g_aManySpawn.nSpawnCounter >= (FIRST_SPAWNMOB + STOP_SPAWN_TIME) && g_aManySpawn.bFirst == false)
		{//1番目

			g_aManySpawn.bFirst = true;			//1番目のスポーン

			//大量発生位置設定
			ManySpawnSetPos();

		}
	}

	//通常スポーン
	if (g_nSpawnCounter >= NORMAL_SPAWN && g_aMob[nCntMob].aModel.bUse == false)
	{//一定時間経った時

		if (g_nNumMob <= MAX_SPAWN_MOB)
		{//モブ700以下
			int nBillNumber = rand() % NUM_BILL;		//建物の種類

			//モブの配置
			SetMob(g_aManySpawn.aSpawn[nBillNumber].pos, g_aManySpawn.aSpawn[nBillNumber].rot, g_aManySpawn.aSpawn[nBillNumber].nType, rand() % 3 + MOB_NUMBER);
		}

		//スポーンする時間を初期化する
		g_nSpawnCounter = 0;
	}

	//ウェーブスポーン
	if (g_aWaveSpawn.nStartSpawn > WAVE_START && g_aMob[nCntMob].aModel.bUse == false)
	{//発生開始

		g_aWaveSpawn.nStopSpawn++;		//終了までの時間加算

		if (g_aWaveSpawn.nSpawnCounter >= WAVE_SPAWN)
		{//一定時間経った時

			if (g_nNumMob <= MAX_SPAWN_MOB)
			{//モブ700以下

				//モブの配置
				SetMob(g_aManySpawn.aSpawn[g_aWaveSpawn.nBillNumber].pos, g_aManySpawn.aSpawn[g_aWaveSpawn.nBillNumber].rot, g_aManySpawn.aSpawn[g_aWaveSpawn.nBillNumber].nType, rand() % 3 + MOB_NUMBER);
			}

			//スポーンする時間を初期化する
			g_aWaveSpawn.nSpawnCounter = 0;

			if (g_aWaveSpawn.nStopSpawn > WAVE_STOP * MAX_MOB)
			{//終了時間

				g_aWaveSpawn.nBillNumber = rand() % NUM_BILL;		//建物の種類

				//初期化
				g_aWaveSpawn.nStartSpawn = 0;
				g_aWaveSpawn.nStopSpawn = 0;
			}
		}
	}
}

//==================================================================================
//大量発生のスポーン処理
//==================================================================================
void ManySpawnMob(int nCntMob)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//イベントサインの情報取得
	IventSign *pIventSign = GetIventSign();

	//大量発生地点の情報取得
	MANYPOINT *pManyPoint = GetManyPoint();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//モブの配置

	//元の位置
	SetMob(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos, g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot, g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType, rand() % 3 + MOB_NUMBER);
	
	//横1
	SetMob(
		D3DXVECTOR3(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x + sinf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * -SPWAN_DISTANCE,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z + cosf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * -SPWAN_DISTANCE),
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot,
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType,
		rand() % 3 + MOB_NUMBER);

	//横2
	SetMob(
		D3DXVECTOR3(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x + sinf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * (-SPWAN_DISTANCE * 2.0f),
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z + cosf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * (-SPWAN_DISTANCE * 2.0f)),
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot,
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType,
		rand() % 3 + MOB_NUMBER);

	//横3
	SetMob(
		D3DXVECTOR3(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x + sinf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * SPWAN_DISTANCE,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z + cosf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * SPWAN_DISTANCE),
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot,
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType,
		rand() % 3 + MOB_NUMBER);

	//横4
	SetMob(
		D3DXVECTOR3(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.x + sinf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * (SPWAN_DISTANCE * 2.0f),
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.y,
			g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos.z + cosf(D3DX_PI * 0.5f + g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot.y) * (SPWAN_DISTANCE * 2.0f)),
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot,
		g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nType,
		rand() % 3 + MOB_NUMBER);

	g_nManySpawnCounter = 0;		//スポーンする速さ初期化

	if (g_aManySpawn.nStopTime == 0)
	{//大量発生の終了時間が0の時

		//大量発生地点を目標の場所として渡す
		pCamera->aMany.posRDest = g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos;
		pCamera->aMany.rotVDest = g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].rot;
		pCamera->nState = CAMERASTATE_MANYSPAWN;

		if (pPlayer->nNowMotionNum == PLAYERMOTION_ACTION || pPlayer->nNowMotionNum == PLAYERMOTION_MOVEACTION)
		{//催眠アクションしていたら

			//催眠の音を消す
			StopSound(SOUND_LABEL_BGM_HYPNOSIS);
		}

		//イベントサインON
		pIventSign->bUse = true;

		//大量発生地点ON
		pManyPoint->bUse = true;

		//UIの設定
		SetManyUI(0);

		//イベントサインの設定
		SetIventSign(g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].pos);
	}

	//大量発生の時間を足す
	g_aManySpawn.nStopTime++;

	g_aUseSpawn.bUseSpawn = true;	//大量発生してる状態にする
}

//==================================================================================
//大量発生の位置設定
//==================================================================================
void ManySpawnSetPos(void)
{
	//イベントサインの情報取得
	IventSign *pIventSign = GetIventSign();

	//大量発生地点の情報取得
	MANYPOINT *pManyPoint = GetManyPoint();

	//イベントスタンプの情報取得
	EVENTSTAMP *pEventStamp = GetEventStamp();

	while (1)
	{
		g_aManySpawn.nBillNumber = rand() % NUM_BILL; 		//大量発生する場所

		if (g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nMany == 1 && g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].bUse == false)
		{//大量発生する位置だったら

			break;
		}

		else if (g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nMany == 1 && g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].bUse == true)
		{
			g_aManySpawn.nCounter++;

			if (g_aManySpawn.nCounter >= NUM_BILL - 1)
			{
				for (int nCount = 0; nCount < NUM_BILL; nCount++)
				{
					g_aManySpawn.aSpawn[nCount].bUse = false;
				}

				while (1)
				{//大量発生する場所を決める

					g_aManySpawn.nBillNumber = rand() % NUM_BILL;

					if (g_aManySpawn.aSpawn[g_aManySpawn.nBillNumber].nMany == 1)
					{//大量発生する位置だったら

						break;
					}
				}

				g_aManySpawn.nCounter = 0;

				break;
			}
		}
	}

	//大量発生終了時間の初期化
	g_aManySpawn.nStopTime = 0;

	g_aUseSpawn.bUseSpawn = false;	//大量発生しない状態にする

	//イベントサインOFF
	pIventSign->bUse = false;

	//大量発生地点OFF
	pManyPoint->bUse = false;

	//イベントスタンプ消す
	pEventStamp[EVENTTYPE_MANYMOB].bUse = false;
}

//==================================================================================
//モブの状態更新処理
//==================================================================================
void UpdateStateMob(int nCntMob)
{
	Rotate *pRotate = GetRotate();
	Player *pPlayer = GetPlayer();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	switch (g_aMob[nCntMob].aModel.nState)
	{
	case MOBSTATE_NONE:		//通常状態

		if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_USEFEVER)
		{
			//うろちょろ処理
			UpdateCrawlMob(nCntMob);
		}
		else if(GetMode() == MODE_GAME)
		{
			//うろちょろ処理
			UpdateCrawlMob(nCntMob);
		}

		//催眠度を増やす間隔を初期化
		g_aMob[nCntMob].nAddHypInter = 0;

		if (g_aMob[nCntMob].aModel.rot.x >= 0.01f || g_aMob[nCntMob].aModel.rot.x <= -0.01f ||
			g_aMob[nCntMob].aModel.rot.z >= 0.01f || g_aMob[nCntMob].aModel.rot.z <= -0.01f)
		{//モブのrotを戻す

			g_aMob[nCntMob].aModel.rot.x = 0.0f;
			g_aMob[nCntMob].aModel.rot.z = 0.0f;
		}

		//催眠度減少するまでのカウンターを減らす
		g_aMob[nCntMob].nSubHypCounter--;

		//催眠度を減らす間隔
		g_aMob[nCntMob].nSubHypInter++;

		//一定間隔でゲージを減らす
		if (g_aMob[nCntMob].nHypnosis > 0 && g_aMob[nCntMob].nSubHypCounter <= 0 && (g_aMob[nCntMob].nSubHypInter % HYPSUB_INTER) == 0)
		{
			//催眠度ゲージを減らす
			g_aMob[nCntMob].nHypnosis--;
		}

		if (g_aMob[nCntMob].nHypnosis <= 0)
		{//催眠度ゲージが０の時

		    //ぐるぐるを使用していない状態にする
			pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = false;

			//催眠度ゲージを使用していない状態にする
			pHypnosis[g_aMob[nCntMob].aModel.nIdxHypGauge].bDisp = false;
		}

		break;

	case MOBSTATE_DMG:			//ダメージ状態

								//催眠度を０にする
		g_aMob[nCntMob].nHypnosis = 0;

		break;

	case MOBSTATE_KEEP:		//催眠にかけられている状態


		pHypnosis[g_aMob[nCntMob].aModel.nIdxHypGauge].bDisp = true;

		//rot切り替えの時間を増やす
		g_aMob[nCntMob].nSwitchCounter++;

		//催眠度減少カウンターを減らす
		g_aMob[nCntMob].nSubHypCounter--;
		g_aMob[nCntMob].nSubHypInter++;

		if (g_aMob[nCntMob].nSubHypCounter <= 0 && (g_aMob[nCntMob].nSubHypInter % HYPSUB_INTER) == 0)
		{//催眠度0、減少するまでのカウンターが0、一定時間経った時

			//催眠度ゲージを減らす
			g_aMob[nCntMob].nHypnosis--;

			if (g_aMob[nCntMob].nHypnosis <= 0)
			{
				g_aMob[nCntMob].nHypnosis = 0;
			}

			g_aMob[nCntMob].nSubHypInter = 0;
			g_aMob[nCntMob].nAddHypInter = 0;
		}

		if (g_aMob[nCntMob].nHypnosis <= 0 && g_aMob[nCntMob].nAddHypInter <= 0)
		{
			//通常状態にする
			g_aMob[nCntMob].aModel.nState = MOBSTATE_NONE;
		}

		if (g_aMob[nCntMob].nSwitchCounter == ROT_CNT / 2)
		{//一定時間経ったらrot+-切り替え
			g_aMob[nCntMob].nTurnSwitchX *= -1;
		}

		if (g_aMob[nCntMob].nSwitchCounter == ROT_CNT)
		{//一定時間経ったらrot+-切り替え
			g_aMob[nCntMob].nTurnSwitchZ *= -1;

			g_aMob[nCntMob].nSwitchCounter = 0;
		}

		//モブを傾けて回す
		g_aMob[nCntMob].aModel.rot.x += ROT_ROUND * g_aMob[nCntMob].nTurnSwitchX;
		g_aMob[nCntMob].aModel.rot.z += ROT_ROUND * g_aMob[nCntMob].nTurnSwitchZ;

		g_aMob[nCntMob].aModel.rot.x = RotNormalize(g_aMob[nCntMob].aModel.rot.x);
		g_aMob[nCntMob].aModel.rot.z = RotNormalize(g_aMob[nCntMob].aModel.rot.z);


		//if (pPlayer->nNowMotionNum != PLAYERMOTION_ACTION && g_aMob[nCntMob].nHypnosis != 10)
		//{//プレイヤーが催眠をやめたとき && 催眠度が10じゃないとき

		//    //催眠度が減少するまでのカウンターを初期化
		//	g_aMob[nCntMob].nSubHypCounter = HYP_CNTTIME;

		//	g_aMob[nCntMob].aModel.nState = MOBSTATE_NONE;		//通常状態にする
		//}
		break;

	case MOBSTATE_HYPNOSIS:		//催眠状態

		if (g_aMob[nCntMob].aModel.rot.x >= 0.01f || g_aMob[nCntMob].aModel.rot.x <= -0.01f ||
			g_aMob[nCntMob].aModel.rot.z >= 0.01f || g_aMob[nCntMob].aModel.rot.z <= -0.01f)
		{//モブが傾いてる場合

		    //モブのrotを戻す
			g_aMob[nCntMob].aModel.rot.x = 0.0f;
			g_aMob[nCntMob].aModel.rot.z = 0.0f;
		}

		//催眠度減少カウンターを減らす
		g_aMob[nCntMob].nSubHypCounter--;
		g_aMob[nCntMob].nSubHypInter++;

		if (g_aMob[nCntMob].nHypnosis > 0 && g_aMob[nCntMob].nSubHypCounter <= 0 && (g_aMob[nCntMob].nSubHypInter % HYPSUB_INTER) == 0)
		{//催眠度0、減少するまでのカウンターが0、一定時間経った時

		    //催眠度ゲージを減らす
			g_aMob[nCntMob].nHypnosis--;
			g_aMob[nCntMob].nSubHypInter = 0;

			//通常状態にする
			g_aMob[nCntMob].aModel.nState = MOBSTATE_NONE;
		}

		break;

	case MOBSTATE_WEAR:			//抱き枕カバー被っている状態

		break;

	case MOBSTATE_APPIEAR:		//出現状態

		//モブの移動
		if (g_aUseSpawn.bUseSpawn == false)
		{
			g_aMob[nCntMob].aModel.pos.x = g_aMob[nCntMob].aModel.pos.x + sinf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * MOVE_APPIEAR;
			g_aMob[nCntMob].aModel.pos.z = g_aMob[nCntMob].aModel.pos.z + cosf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * MOVE_APPIEAR;
		}
		else if (g_aUseSpawn.bUseSpawn == true)
		{//大量発生の時

			g_aMob[nCntMob].aModel.pos.x = g_aMob[nCntMob].aModel.pos.x + sinf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * MOVE_MANY_APPIEAR;
			g_aMob[nCntMob].aModel.pos.z = g_aMob[nCntMob].aModel.pos.z + cosf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * MOVE_MANY_APPIEAR;
		}

		if (g_aMob[nCntMob].fAlpha < 1.0f)
		{//透明度が1.0未満の時

		    //透明度を上げる
			g_aMob[nCntMob].fAlpha += g_aMob[nCntMob].fRandAlpha;

		}
		else if (g_aMob[nCntMob].fAlpha >= 1.0f)
		{//透明度が1.0以上の場合

			g_aMob[nCntMob].fAlpha = 0.4f;	//透明度初期化

			g_aMob[nCntMob].aModel.rot.y = RotNormalize(g_aMob[nCntMob].aModel.rot.y);		//角度の正規化
			g_aMob[nCntMob].aModel.rot.y += D3DXToRadian(90.0f);	//向きを９０度傾ける
			g_aMob[nCntMob].aModel.rot.y = RotNormalize(g_aMob[nCntMob].aModel.rot.y);		//角度の正規化

			g_aMob[nCntMob].aModel.nState = MOBSTATE_NONE;			//通常状態
		}

		//透明度の設定
		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aMob[nCntMob].aModel.pMatData[nCntMat].MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aMob[nCntMob].fAlpha);
			g_aMob[nCntMob].aModel.pMatData[nCntMat].MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aMob[nCntMob].fAlpha);
		}

		break;
	}

}

//==================================================================================
//モブの行動更新処理
//==================================================================================
void UpdateActionMob(int nCntMob)
{

}

//==================================================================================
//モブのうろちょろ処理
//==================================================================================
void UpdateCrawlMob(int nCntMob)
{
	int nMoveTime = 0;		//移動の目標時間

							//モブの移動のカウンターを増やす
	g_aMob[nCntMob].nMoveCounter++;

	//移動の目標時間を決める
	nMoveTime = rand() % MAX_MOVE + MIN_MOVE;

	if ((g_aMob[nCntMob].nMoveCounter % nMoveTime) == 0)
	{//一定時間経ったら

	 //移動の向き切り替え
		g_aMob[nCntMob].aModel.rot.y += D3DX_PI;

		//角度の正規化
		g_aMob[nCntMob].aModel.rot.y = RotNormalize(g_aMob[nCntMob].aModel.rot.y);
	}

	//モブの移動
	g_aMob[nCntMob].aModel.pos.x = g_aMob[nCntMob].aModel.pos.x + sinf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * 1.0f;
	g_aMob[nCntMob].aModel.pos.z = g_aMob[nCntMob].aModel.pos.z + cosf(D3DX_PI + g_aMob[nCntMob].aModel.rot.y) * 1.0f;
}

//==================================================================================
//モブのセリフ設定処理
//==================================================================================
void MobSerifSet(int nCntMob)
{
	if (g_nMobSerifTime >= 2000 && g_aMob[nCntMob].aModel.nState != MOBSTATE_APPIEAR && g_aMob[nCntMob].aModel.nIdxSerif == -1)
	{//一定時間経ったら

		switch (g_aMob[nCntMob].nTexType)
		{
		case 0:		//会社員

			//セリフの設定
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_WORK, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		//case 1:		//ジャイアン？

		//	//セリフの設定
		//	g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_WORK, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
		//	break;

		case 2:			//やくざ

			//セリフの設定
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_YAKUZA, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		case 3:			//会社員

			//セリフの設定
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_WORK, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		case 4:			//ぼっち

			//セリフの設定
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_BOCCHI, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		case 5:			//ぼっち地雷

			//セリフの設定
			g_aMob[nCntMob].aModel.nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_MOB_BOCCHI, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });
			break;

		}
		

		//セリフの位置更新
		SetPositionSerif(g_aMob[nCntMob].aModel.nIdxSerif, { g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y + 110.0f, g_aMob[nCntMob].aModel.pos.z });

		//時間初期化
		g_nMobSerifTime = 0;
	}
}

//==================================================================================
//モブのヒット処理
//==================================================================================
void HitMob(int nCntMob, int nValue)
{
	//情報取得
	Rotate *pRotate = GetRotate();
	Player *pPlayer = GetPlayer();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//催眠状態じゃないとき
	if (g_aMob[nCntMob].aModel.nState != MOBSTATE_HYPNOSIS)
	{
		//とどまらせる
		g_aMob[nCntMob].aModel.nState = MOBSTATE_KEEP;
	}

	//催眠度増加カウンターを増やす
	g_aMob[nCntMob].nAddHypInter += nValue;

	//催眠度増加
	if (g_aMob[nCntMob].nAddHypInter >= HYPADD_INTER)
	{//催眠度増加カウンターが規定値を超えたら

		//催眠度増加カウンターリセット
		g_aMob[nCntMob].nAddHypInter = 0;

		//催眠度を増やす
		if (g_aMob[nCntMob].nHypnosis < 10)
		{//催眠度が10未満の時

			//催眠度上昇
			g_aMob[nCntMob].nHypnosis++;
		}
	}

	//ぐるぐる追加
	if (pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp == false && g_aMob[nCntMob].nHypnosis >= 10 && g_aSwitchSE.bUseRotate == false)
	{
		pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = true;

		//SEをセット
		PlaySound(SOUND_LABEL_SE_PIYOPIYO);

		g_aSwitchSE.bUseRotate = true;		//SE使用してる状態にする
	}

	//催眠度が最大
	if (g_aMob[nCntMob].nHypnosis >= 10)
	{
		//催眠状態にする
		g_aMob[nCntMob].aModel.nState = MOBSTATE_HYPNOSIS;
	    
		//チュートリアルの時だけ
		if (GetMode() == MODE_TUTORIAL)
		{
			//催眠完了状態
			pTutorial->aControl.bHypnosis = true;
		}
	}
}

//==================================================================================
//オブジェクトとの当たり判定
//==================================================================================
void CollisionModelMob(int nCntMob)
{

	//モデルの情報取得
	ModelModel *pModel = GetModel();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true)
		{//モデルが使用されていたら

			//キャラクター同士の当たり判定
			CollisionCharacter(
				&g_aMob[nCntMob].aModel.pos, &g_aMob[nCntMob].aModel.posOld, g_aMob[nCntMob].aModel.vtxMax, g_aMob[nCntMob].aModel.vtxMin,
				&pModel->pos, pModel->vtxMax, pModel->vtxMin);
		}
	}
}

//==================================================================================
//モブの描画処理
//==================================================================================
void DrawMob(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATERIAL mMatRed;			//ダメージマテリアル

									//他の情報クリア
	ZeroMemory(&mMatRed, sizeof(D3DXMATERIAL));
	mMatRed.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	mMatRed.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		if (g_aMob[nCntMob].aModel.bUse == true)
		{//使用していたら

		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMob[nCntMob].aModel.mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMob[nCntMob].aModel.rot.y, g_aMob[nCntMob].aModel.rot.x, g_aMob[nCntMob].aModel.rot.z);
			D3DXMatrixMultiply(&g_aMob[nCntMob].aModel.mtxWorld, &g_aMob[nCntMob].aModel.mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aMob[nCntMob].aModel.pos.x, g_aMob[nCntMob].aModel.pos.y, g_aMob[nCntMob].aModel.pos.z);
			D3DXMatrixMultiply(&g_aMob[nCntMob].aModel.mtxWorld, &g_aMob[nCntMob].aModel.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMob[nCntMob].aModel.mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aMob[nCntMob].aModel.pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aMob[nCntMob].aModel.dwNumMat; nCntMat++)
			{
				switch (g_aMob[nCntMob].aModel.nState)
				{
				case MOBSTATE_APPIEAR:		//出現状態

					pDevice->SetMaterial(&g_aMob[nCntMob].aModel.pMatData[nCntMat].MatD3D);
					break;

				default:
					//マテリアルの設定
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
					break;
				}

				//テクスチャの設定
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					pDevice->SetTexture(0, g_apTextureMob[g_aMob[nCntMob].nTexType]);
				}
				else
				{
					pDevice->SetTexture(0, NULL);
				}

				

				//モブ(パーツ)の描画
				g_aMob[nCntMob].aModel.pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//モブの設定処理
//==================================================================================
void SetMob(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nSpawnType, int nModelType)
{
	//モデルの情報取得
	Model *pModel = GetXLoadData();
	Rotate *pRotate = GetRotate();
	Player *pPlayer = GetPlayer();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		if (g_aMob[nCntMob].aModel.bUse == false)
		{//使用していなかったら

			//モデル情報取得
			g_aMob[nCntMob].aModel = pModel[nModelType];

			g_aMob[nCntMob].nSpawnType = nSpawnType;	//出現する建物の種類
			g_aMob[nCntMob].aModel.nType = nModelType;	//モブの種類
			g_aMob[nCntMob].nTexType = ((nModelType - MOB_NUMBER) * 2) + (rand() % 2);	//テクスチャタイプ

			g_aMob[nCntMob].aModel.pos = pos;	//位置
			g_aMob[nCntMob].aModel.rot = rot;	//向き
			g_aMob[nCntMob].aModel.rot.y += D3DX_PI;
			g_aMob[nCntMob].aModel.nState = MOBSTATE_APPIEAR;	//出現状態にする
			g_aMob[nCntMob].aModel.bUse = true;					//使用している状況にする
			g_aMob[nCntMob].nHypnosis = 0;
			g_aMob[nCntMob].fRandAlpha = (float)(rand() % 51 + 13) * 0.0001f;		//透明度の増加数

			if (g_aUseSpawn.bUseSpawn == true)
			{//大量発生中

				//倍
				g_aMob[nCntMob].fRandAlpha *= 3.0f;
			}

			//モデルの全頂点チェック
			CheckVtx(g_aMob[nCntMob].aModel.rot.y, &g_aMob[nCntMob].aModel.vtxMax, &g_aMob[nCntMob].aModel.vtxMin, g_aMob[nCntMob].aModel.pMesh, g_aMob[nCntMob].aModel.pVtxBuff);

			//催眠度ゲージの設定
			g_aMob[nCntMob].aModel.nIdxHypGauge = SetHypnosisGauge(25.0f, 5.0f, 10);

			//催眠度ゲージの位置更新
			SetPositionHypnosisGauge(g_aMob[nCntMob].aModel.nIdxHypGauge, g_aMob[nCntMob].aModel.pos, g_aMob[nCntMob].nHypnosis);

			//ぐるぐるを設定
			g_aMob[nCntMob].aModel.nIdxRotate = SetRotate();

			//影を設定
			g_aMob[nCntMob].aModel.nIdxShadow = SetShadow(g_aMob[nCntMob].aModel.vtxMax.x * 1.5f, g_aMob[nCntMob].aModel.vtxMax.z * 1.5f);

			//影の位置更新
			SetPositionShadow(g_aMob[nCntMob].aModel.nIdxShadow, g_aMob[nCntMob].aModel.pos);

			//アイコンの設定処理
			g_aMob[nCntMob].nIdxIcon = SetIcon(MINIMAP_ICON_MOB, 40.0f);
			UpdatePositionIcon(g_aMob[nCntMob].nIdxIcon, g_aMob[nCntMob].aModel.pos);

			//ぐるぐるを使用していない状態にする
			pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = false;

			//催眠度ゲージを使用していない状態にする
			pHypnosis[g_aMob[nCntMob].aModel.nIdxHypGauge].bDisp = false;

			break;
		}
	}
}

//==================================================================================
//モブのスポーン位置の読み込み処理
//==================================================================================
void ReadSetMobPos(void)
{

	char aComment[MAX_COMMENT];	//コメント用
	int nCntSpawn = 0;
	int nCntFileName = 0;

	FILE *pFile = NULL;	//ファイルポインタを宣言

						//ファイルを開く
	pFile = fopen("data\\TEXT\\mob_set.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{//END_SCRIPTが来るまで繰り返す

		 //文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//モデルの設定
			if (strcmp(aComment, "MOBSET") == 0)
			{//モデルの読み込みを開始

				while (strcmp(aComment, "END_MOBSET"))
				{//END_ENEMYSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "MANY") == 0)
					{//MANYが来たら種類読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aManySpawn.aSpawn[nCntSpawn].nMany);		//大量発生使用の判定
					}

					if (strcmp(aComment, "TYPE") == 0)
					{//TYPEが来たら種類読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						g_aManySpawn.aSpawn[nCntSpawn].nType = nCntSpawn;
					}

					if (strcmp(aComment, "POS") == 0)
					{//POSが来たら位置読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].pos.x);	//X座標
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].pos.y);	//Y座標
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].pos.z);	//Z座標
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROTが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].rot.x);	//Xの向き
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].rot.y);	//Yの向き
						fscanf(pFile, "%f", &g_aManySpawn.aSpawn[nCntSpawn].rot.z);	//Zの向き

						g_aManySpawn.aSpawn[nCntSpawn].rot = D3DXToRadian(g_aManySpawn.aSpawn[nCntSpawn].rot);
					}

				}//END_MOBSETのかっこ

				 //スポーン地点の番号加算
				nCntSpawn++;
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
//モブのチュートリアル更新処理
//==================================================================================
void UpdateTutorialMob(void)
{
	//モデルの情報取得
	ModelModel *pModel = GetModel();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();
	Rotate *pRotate = GetRotate();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	if (pTutorial->nStep >= TUTORIAL_STEP_USEFEVER)
	{
		if (pTutorial->aControl.bSetMobFever == false)
		{//まだフィーバー時のモブを設置していなかったら

			//開始時のモブを設定
			for (int nCntMob = 0; nCntMob < START_MOB; nCntMob++)
			{
				int nCntNumber = rand() % NUM_BILL;		//建物の種類

				//モブの設定
				SetMob(g_aManySpawn.aSpawn[nCntNumber].pos, g_aManySpawn.aSpawn[nCntNumber].rot, g_aManySpawn.aSpawn[nCntNumber].nType, rand() % 3 + MOB_NUMBER);
			}

			//設置状態にする
			pTutorial->aControl.bSetMobFever = true;
		}

		//スポーンする時間を増加する
		g_nSpawnCounter++;
	}

	if (pTutorial->nStep >= TUTORIAL_STEP_HYPNOSIS && pTutorial->aControl.bSetMob == false)
	{//催眠のステップ以上 && モブが5人以下

		for (int nCntMob = 0; nCntMob < 5; nCntMob++)
		{
			int nCntNumber = rand() % NUM_BILL;		//建物の種類

			//モブの設定
			SetMob(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, nCntMob * (D3DX_PI / 5.0f), 0.0f), g_aManySpawn.aSpawn[nCntNumber].nType, rand() % 3 + MOB_NUMBER);
		}

		//モブ配置完了
		pTutorial->aControl.bSetMob = true;
	}

	int nUse = 0;
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++)
	{
		if (g_aMob[nCntMob].aModel.bUse == true)
		{//使用していたら

			//使ってる数加算
			nUse++;

			//過去の位置保存
			g_aMob[nCntMob].aModel.posOld = g_aMob[nCntMob].aModel.pos;

			//モブの操作
			ControllMob(nCntMob);

			//モブ同士の当たり判定
			CollisionModelMob(nCntMob);

			//座標制限
			LimitPosMob(nCntMob);

			//状態更新
			UpdateStateMob(nCntMob);

			//ぐるぐるの位置更新
			SetPositionRotate(g_aMob[nCntMob].aModel.nIdxRotate, g_aMob[nCntMob].aModel.pos);

			//催眠度ゲージの位置更新
			SetPositionHypnosisGauge(g_aMob[nCntMob].aModel.nIdxHypGauge, g_aMob[nCntMob].aModel.pos, g_aMob[nCntMob].nHypnosis);

			//影の位置更新
			SetPositionShadow(g_aMob[nCntMob].aModel.nIdxShadow, g_aMob[nCntMob].aModel.pos);

			//アイコンの位置更新
			UpdatePositionIcon(g_aMob[nCntMob].nIdxIcon, g_aMob[nCntMob].aModel.pos);

			if (g_aMob[nCntMob].aModel.nState != MOBSTATE_HYPNOSIS && g_aMob[nCntMob].aModel.nState != MOBSTATE_KEEP)
			{//催眠状態、催眠かけてる最中以外の時

				//ぐるぐる削除
				pRotate[g_aMob[nCntMob].aModel.nIdxRotate].bDisp = false;
			}

		}
		else if (g_aMob[nCntMob].aModel.bUse == false)
		{//使用していなかったら

			//モブの発生
			UpdateSpawnMob(nCntMob);
		}
	}

	g_nNumMob = nUse;
}

//==================================================================================
//モブの情報取得
//==================================================================================
Mob *GetMob(void)
{
	return &g_aMob[0];
}

//==================================================================================
//大量発生地点の情報取得
//==================================================================================
ManySpawn *GetManySpawn(void)
{
	return &g_aManySpawn;
}

//==================================================================================
//大量発生地点の情報取得
//==================================================================================
SwitchSpawn *GetSwitchSpawn(void)
{
	return &g_aUseSpawn;
}

//==================================================================================
//SEの情報取得
//==================================================================================
SwitchSE *GetSwitchSE(void)
{
	return &g_aSwitchSE;
}