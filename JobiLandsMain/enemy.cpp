//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "enemy.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "calculation.h"
#include "search_circle.h"
#include "crowd.h"
#include "shadow.h"
#include "minimap_icon.h"
#include "watergun.h"
#include "effect.h"
#include "particle.h"
#include "EventWindow.h"
#include "search_fan.h"
#include "tutorial.h"
#include "buddy.h"
#include "dispatch.h"
#include "sound.h"
#include "eventstamp.h"
#include "serif.h"
#include "sound.h"

//マクロ定義
#define MOVE			(0.4f)
#define CRAWL_MOVE		(0.1f)		//巡回の移動量
#define MAX_LENGTH		(100.0f)	//元の距離
#define ROT_MOVE		(0.03f)		//回転移動量
#define DMG_TIME		(20)		//ダメージ状態の時間
#define SEARCH_INTERVAL	(90)
#define ATK_INTERVAL	(100)
#define LIFE			(2)
#define CHASE_RADIUS	(240.0f)
//#define SPWAN_TIME      (4500)    //警備強化状態に入るまでの時間
#define FIRST_SPAWN		(60 * 85)	//1番目の警備強化
#define SECOND_SPAWN	(60 * 185)	//2番目の警備強化
//#define THIRD_SPAWN		(60 * 280)	//3番目の警備強化
#define STRENG_TIME     (60 * 40)   //警備強化状態の時間
//#define STRENG_TIME     (60 * 30)   //警備強化状態の時間
#define STOP_MANYTIME		(60 * 35)	//大量発生時の警備終了時間
#define SOUNDCOOLTIME   (10)            //サウンドのクールタイム
#define ENEMY_TXT		"data\\TEXT\\motion_set_enemy.txt"
#define ENEMYSET_TXT	"data\\TEXT\\model_set.txt"

//プロトタイプ宣言
void XFileLoadEnemy(void);
void ReadSetMotionEnemy(void);

void ControllEnemy(int nCntEnemy);
void UpdateStateEnemy(int nCntEnemy);
void UpdateActionEnemy(int nCntEnemy);
void UpdateMotionEnemy(int nCntEnemy);
void LimitPosEnemy(int nCntEnemy);
void RotFacePlayerEnemy(int nCntEnemy);
void UpdateCrawlEnemy(int nCntEnemy);
void ATKEnemy(int nCntEnemy);
void CollisionModelEnemy(int nCntEnemy);
void ShotBulletEnemy(int nCntEnemy, D3DXVECTOR3 pos);
void MoreSpawnEnemy(void);
void EnemySerifSet(int nCntEnemy);
void UpdateGameEndEnemy(int nCntEnemy);
void ManyMoreSpawnEnemy(void);

//グローバル変数宣言
Enemy g_aEnemy[MAX_ENEMY];				//敵の情報
Model g_aXLoadMeshEnemy[MAX_MODEL];		//Xファイルのデータ
EnemyInfo g_aEnemyInfo;	//情報
MoreEnemy g_MoreEnemy;  //警備強化
int g_nCntWaterBullet = 0;
bool g_bManyMORS;
int g_nEnemySerifTime;		//セリフの設定時間
int g_SoundCoolTime;        //水鉄砲のSEのクールタイム

//==================================================================================
//敵の初期化処理
//==================================================================================
void InitEnemy(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//各要素初期化
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		//敵の構造体
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aEnemy[nCntEnemy].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//現在の位置
		g_aEnemy[nCntEnemy].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

																		//ワールドマトリックス
		D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);
		g_aEnemy[nCntEnemy].bUse = false;				//使用しているか
		g_aEnemy[nCntEnemy].bMoreSpawn = false;        //警備強化しているかどうか
		g_aEnemy[nCntEnemy].nState = 0;				//状態
		g_aEnemy[nCntEnemy].nIdxShadow = -1;			//影のインデックス番号
		g_aEnemy[nCntEnemy].nIdxSearchRadius = -1;	//探索範囲円のインデックス番号
		g_aEnemy[nCntEnemy].nIdxIcon = -1;			//ミニマップアイコンのインデックス番号
		g_aEnemy[nCntEnemy].nIdxSearchFan = -1;		//扇のインデックス番号
		g_aEnemy[nCntEnemy].nIdxSerif = -1;			//セリフのインデックス番号
		g_aEnemy[nCntEnemy].nCntState = 0;			//状態カウンター
		g_aEnemy[nCntEnemy].nCntAction = 0;			//行動カウンター
		g_aEnemy[nCntEnemy].nATKInterval = 0;		//攻撃の間隔
		g_aEnemy[nCntEnemy].nCntATK = 0;			//攻撃の間隔カウント
		g_aEnemy[nCntEnemy].nCntSearchCircle = 0;	//探索範囲円のカウント
		g_aEnemy[nCntEnemy].fChaseRadius = 0.0f;		//追い掛け範囲
		g_aEnemy[nCntEnemy].fSearchRadius = 0.0f;	//探索範囲
		g_aEnemy[nCntEnemy].fSearchLength = 0.0f;	//探索の長さ
		g_aEnemy[nCntEnemy].fSearchRot = 0.0f;		//探索の向き
		g_aEnemy[nCntEnemy].fRadius = 0.0f;			//探索の向き

		g_aEnemy[nCntEnemy].nNumPosCrawl = 0;		//巡回する地点の番号
		g_aEnemy[nCntEnemy].nNumMaxCrawl = 0;		//巡回する地点の最大数
		g_aEnemy[nCntEnemy].MassGeneration = 0;     //大量発生時に出る敵
		g_aEnemy[nCntEnemy].nCntWait = 0;           //出現待ちのカウント

													//巡回ポイント
		for (int nCntCrawl = 0; nCntCrawl < MAX_POSCRAWL; nCntCrawl++)
		{
			g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		//モーション系
		g_aEnemy[nCntEnemy].nNowMotionNum = 0;		//現在のモーション番号
		g_aEnemy[nCntEnemy].nCntFrame = 0;			//フレームのカウント
		g_aEnemy[nCntEnemy].nPatternKey = 0;		//何個目のキーか
		g_aEnemy[nCntEnemy].nPartsNum = 0;			//パーツ数
		g_aEnemy[nCntEnemy].nCntAllFrame = 0;		//総フレームカウント

													//モデル構造体の初期化
		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			g_aEnemy[nCntEnemy].aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
			g_aEnemy[nCntEnemy].aModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
			g_aEnemy[nCntEnemy].aModel[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
			g_aEnemy[nCntEnemy].aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
			g_aEnemy[nCntEnemy].aModel[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
			g_aEnemy[nCntEnemy].aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

																							//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntModel].mtxWorld);
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntModel].OldmtxWorld);

			g_aEnemy[nCntEnemy].aModel[nCntModel].nIdxShadow = -1;			//影のインデックス番号
			g_aEnemy[nCntEnemy].aModel[nCntModel].nIdxRotate = -1;	//ぐるぐるのインデックス番号
			g_aEnemy[nCntEnemy].aModel[nCntModel].nIdxHypGauge = -1;		//体力ゲージのインデックス番号
			g_aEnemy[nCntEnemy].aModel[nCntModel].nShadow = 1;			//影を使うかどうか

			g_aEnemy[nCntEnemy].aModel[nCntModel].nNumVtx = 0;			//頂点数
			g_aEnemy[nCntEnemy].aModel[nCntModel].dwSizeFVF = NULL;		//頂点フォーマットのサイズ
			g_aEnemy[nCntEnemy].aModel[nCntModel].pVtxBuff = NULL;			//頂点バッファのポインタ
			g_aEnemy[nCntEnemy].aModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
			g_aEnemy[nCntEnemy].aModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

			g_aEnemy[nCntEnemy].aModel[nCntModel].nType = 0;			//読み込むモデルの種類
			g_aEnemy[nCntEnemy].aModel[nCntModel].nParent = -1;		//親の番号
			g_aEnemy[nCntEnemy].aModel[nCntModel].bUse = false;			//使用しているか
			g_aEnemy[nCntEnemy].aModel[nCntModel].nState = 0;			//状態

			for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
			{
				g_aEnemy[nCntEnemy].aModel[nCntModel].pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
				g_aEnemy[nCntEnemy].aModel[nCntModel].pMatData[nCntMat].pTextureFilename = NULL;	//マテリアルのデータ
				g_aEnemy[nCntEnemy].aModel[nCntModel].acFilename[nCntMat] = NULL;	//ファイル名
			}

			for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
			{
				g_aEnemy[nCntEnemy].aModel[nCntModel].pTexture[nCntTex] = NULL;	//テクスチャへのポインタ
			}

			g_aEnemy[nCntEnemy].aModel[nCntModel].pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
			g_aEnemy[nCntEnemy].aModel[nCntModel].pBuffMat = NULL;		//マテリアルへのポインタ
			g_aEnemy[nCntEnemy].aModel[nCntModel].dwNumMat = NULL;			//マテリアルの数
		}
	}

	//モデルデータ
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aXLoadMeshEnemy[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aXLoadMeshEnemy[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aXLoadMeshEnemy[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
		g_aXLoadMeshEnemy[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aXLoadMeshEnemy[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
		g_aXLoadMeshEnemy[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

																				//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aXLoadMeshEnemy[nCntModel].mtxWorld);
		D3DXMatrixIdentity(&g_aXLoadMeshEnemy[nCntModel].OldmtxWorld);
		g_aXLoadMeshEnemy[nCntModel].nIdxShadow = -1;			//影のインデックス番号
		g_aXLoadMeshEnemy[nCntModel].nIdxRotate = -1;	//ぐるぐるのインデックス番号
		g_aXLoadMeshEnemy[nCntModel].nIdxHypGauge = -1;		//体力ゲージのインデックス番号
		g_aXLoadMeshEnemy[nCntModel].nShadow = 1;			//影を使うかどうか

		g_aXLoadMeshEnemy[nCntModel].nNumVtx = 0;			//頂点数
		g_aXLoadMeshEnemy[nCntModel].dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aXLoadMeshEnemy[nCntModel].pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aXLoadMeshEnemy[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
		g_aXLoadMeshEnemy[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

		g_aXLoadMeshEnemy[nCntModel].nType = 0;			//読み込むモデルの種類
		g_aXLoadMeshEnemy[nCntModel].nParent = -1;		//親の番号
		g_aXLoadMeshEnemy[nCntModel].bUse = false;			//使用しているか
		g_aXLoadMeshEnemy[nCntModel].nState = 0;			//状態

		g_aXLoadMeshEnemy[nCntModel].pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
		g_aXLoadMeshEnemy[nCntModel].pBuffMat = NULL;		//マテリアルへのポインタ
		g_aXLoadMeshEnemy[nCntModel].dwNumMat = NULL;		//マテリアルの数

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aXLoadMeshEnemy[nCntModel].pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
			g_aXLoadMeshEnemy[nCntModel].pMatData[nCntMat].pTextureFilename = NULL;	//マテリアルのデータ
			g_aXLoadMeshEnemy[nCntModel].acFilename[nCntMat] = NULL;	//ファイル名
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aXLoadMeshEnemy[nCntModel].pTexture[nCntTex] = NULL;	//テクスチャへのポインタ
		}
	}

	//敵の情報
	g_aEnemyInfo.nNumEnemy = 0;			//敵の数

	//警備強化情報
	g_MoreEnemy;

	g_MoreEnemy.MoreSecurityCounter = 0;		//警備強化するまでの時間
	g_MoreEnemy.SpawnCounter = STRENG_TIME;     //警備強化終了までの時間
	g_MoreEnemy.ManySpawnCounter = STOP_MANYTIME;		//大量発生時の警備強化終了時間
	g_MoreEnemy.bMoreSpawn = false;             //警備強化中かどうか
	g_MoreEnemy.bManyMoreSpawn = false;		//大量発生時に警備強化中かどうか
	g_MoreEnemy.bFirst = false;				//1番目
	g_MoreEnemy.bSecond = false;			//2番目
	g_MoreEnemy.bManyFirst = false;			//1番目の大量発生時の警備強化
	g_MoreEnemy.bManySecond = false;		//2番目の大量発生時の警備強化
	g_MoreEnemy.bManyThird = false;			//3番目の大量発生時の警備強化

	g_nCntWaterBullet = 0;   //弾を発射するカウント
	g_nEnemySerifTime = 0;	//敵のセリフ設定時間

#if _DEBUG

	g_bManyMORS = false;	//警備強化するかどうか

#else NDEBUG

	g_bManyMORS = true;	//警備強化するかどうか

#endif

	//外部ファイル読み込み
	ReadSetMotionEnemy();

	if (GetMode() == MODE_TUTORIAL)
	{
		ReadSetEnemy();
	}

}

//==================================================================================
//敵の終了処理
//==================================================================================
void UninitEnemy(void)
{
	//SE終了
	StopSound();

	for (int nCntParts = 0; nCntParts < MAX_MODEL; nCntParts++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//テクスチャの破棄
			if (g_aXLoadMeshEnemy[nCntParts].pTexture[nCntTex] != NULL)
			{
				g_aXLoadMeshEnemy[nCntParts].pTexture[nCntTex]->Release();
				g_aXLoadMeshEnemy[nCntParts].pTexture[nCntTex] = NULL;
			}
		}

		//メッシュの破棄
		if (g_aXLoadMeshEnemy[nCntParts].pMesh != NULL)
		{
			g_aXLoadMeshEnemy[nCntParts].pMesh->Release();
			g_aXLoadMeshEnemy[nCntParts].pMesh = NULL;
		}

		//マテリアルの破棄
		if (g_aXLoadMeshEnemy[nCntParts].pBuffMat != NULL)
		{
			g_aXLoadMeshEnemy[nCntParts].pBuffMat->Release();
			g_aXLoadMeshEnemy[nCntParts].pBuffMat = NULL;
		}
	}
}

//==================================================================================
//敵の更新処理
//==================================================================================
void UpdateEnemy(void)
{
	//プレイヤーの情報取得
	ModelModel *pModel = GetModel();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F9) == true)
	{//F9が押された

	 //大量発生切り替え
		g_bManyMORS = g_bManyMORS ? false : true;
	}
#endif

	if (GetGameState() == GAMESTATE_NONE)
	{
		if (/*g_MoreEnemy.bMoreSpawn == false && */g_bManyMORS == true)
		{//使用していなかったら

			g_MoreEnemy.MoreSecurityCounter++;      //警備強化するまでの時間を加算
		}
	}

	if (g_MoreEnemy.bMoreSpawn == true)
	{//使用していたら

		g_MoreEnemy.SpawnCounter--;
	}

	if (g_MoreEnemy.bManyMoreSpawn == true)
	{//大量発生時に警備強化してたら
		g_MoreEnemy.ManySpawnCounter--;		//大量発生時の警備強化終了時間
	}

	//警備強化
	MoreSpawnEnemy();

	//大量発生時の警備強化
	ManyMoreSpawnEnemy();

	g_SoundCoolTime--;

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bMoreSpawn == true)
		{//使用していたら

			//過去の位置保存
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			if (g_aEnemy[nCntEnemy].nCntSearchCircle >= SEARCH_INTERVAL - 1 && GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_END)
			{//規定値超えたら

				g_aEnemy[nCntEnemy].nIdxSearchRadius = SetSearchCircle(
					D3DXVECTOR3(
						g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 10.0f, g_aEnemy[nCntEnemy].pos.z),
					g_aEnemy[nCntEnemy].rot,
					D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.5f),
					10.0f, 0.0f,
					(int)(SEARCH_RADIUS * 0.5f), 2.0f,
					SEARCH_CIRCLE_TYPE_ORANGE2);
			}

			//探索範囲円のカウント加算
			g_aEnemy[nCntEnemy].nCntSearchCircle = (g_aEnemy[nCntEnemy].nCntSearchCircle + 1) % SEARCH_INTERVAL;

			if (GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_AFTEREND && GetGameState() != GAMESTATE_END)
			{//終了前じゃなかったら

				if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT || g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_WAIT || g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_LOST)
				{//モーションが射撃、敵の状態が出現待ちまたは、消滅待ち以外の時

					//敵の操作
					ControllEnemy(nCntEnemy);
				}

				//敵同士の当たり判定
				CollisionModelEnemy(nCntEnemy);
			}
			else if(GetGameState() == GAMESTATE_BEFOREEND || GetGameState() == GAMESTATE_END)
			{
				//終了前処理
				UpdateGameEndEnemy(nCntEnemy);
			}

			//座標制限
			LimitPosEnemy(nCntEnemy);

			if (GetGameState() != GAMESTATE_BEFOREEND && GetGameState() != GAMESTATE_AFTEREND && GetGameState() != GAMESTATE_END)
			{//終了前じゃなかったら

				//状態更新
				UpdateStateEnemy(nCntEnemy);
			}

			//行動更新処理
			UpdateActionEnemy(nCntEnemy);

			//モーション更新処理
			UpdateMotionEnemy(nCntEnemy);

			//影の位置更新
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);

			//探索円の位置更新
			SetPositionSearchCircle(g_aEnemy[nCntEnemy].nIdxSearchRadius, g_aEnemy[nCntEnemy].pos);

			//アイコンの位置更新
			UpdatePositionIcon(g_aEnemy[nCntEnemy].nIdxIcon, g_aEnemy[nCntEnemy].pos);

			//扇の形の位置更新
			SetPositionSearchFan(g_aEnemy[nCntEnemy].nIdxSearchFan, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);

			//敵のセリフ設定処理
			EnemySerifSet(nCntEnemy);

			//セリフの位置更新
			SetPositionSerif(g_aEnemy[nCntEnemy].nIdxSerif, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 150.0f, g_aEnemy[nCntEnemy].pos.z });
		}
	}
}

//==================================================================================
//敵の操作
//==================================================================================
void ControllEnemy(int nCntEnemy)
{

	//重力処理
	g_aEnemy[nCntEnemy].move.y -= GRAVITY;

	//位置更新
	g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
	g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * 0.1f;
	g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z) * 0.1f;
}

//==================================================================================
//敵の攻撃処理
//==================================================================================
void ATKEnemy(int nCntEnemy)
{

	//プレイヤーの情報取得
	//Player *pPlayer = GetPlayer();

	//攻撃間隔カウント加算
	g_aEnemy[nCntEnemy].nCntATK++;

	if (g_aEnemy[nCntEnemy].nCntATK >= g_aEnemy[nCntEnemy].nATKInterval && g_aEnemy[nCntEnemy].nCntATK <= g_aEnemy[nCntEnemy].nATKInterval + 5)
	{
		if (g_aEnemy[nCntEnemy].nCntATK >= g_aEnemy[nCntEnemy].nATKInterval + 5)
		{
			//カウントリセット
			g_aEnemy[nCntEnemy].nCntATK = 0;
		}

		g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 3.0f;
		g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 3.0f;

		//敵とプレイヤーの距離
		//float fLength = pPlayer->fRadius + g_aEnemy[nCntEnemy].aModel.vtxMax.x;

		//if (SphereRange(g_aEnemy[nCntEnemy].pos, pPlayer->pos) <= (fLength * fLength))
		//{//球の中に入ったら

		//	//プレイヤーのヒット処理
		//	HitPlayer(D3DXVECTOR3(sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 15.0f, 10.0f, cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 15.0f), 1);
		//}
	}
}

//==================================================================================
//プレイヤーの方を見る処理
//==================================================================================
void RotFacePlayerEnemy(int nCntEnemy)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//敵とプレイヤーの距離
	float fLength = pPlayer->fRadius + g_aEnemy[nCntEnemy].fChaseRadius;

	float fRotDest = g_aEnemy[nCntEnemy].rot.y;	//目標の角度
	float fRotDiff = 0.0f;	//現在と目標の差分

							//目標の角度を求める
	fRotDest = atan2f((g_aEnemy[nCntEnemy].pos.x - pPlayer->pos.x), (g_aEnemy[nCntEnemy].pos.z - pPlayer->pos.z));
	fRotDiff = fRotDest - g_aEnemy[nCntEnemy].rot.y;

	//角度の正規化
	RotNormalize(&fRotDiff);

	//角度の補正をする
	g_aEnemy[nCntEnemy].rot.y += fRotDiff * 0.15f;

	//角度の正規化
	RotNormalize(&g_aEnemy[nCntEnemy].rot.y);

	//向いてる方向にダッシュ
	g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * MOVE;
	g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * MOVE;
}

//==================================================================================
//敵の巡回処理
//==================================================================================
void UpdateCrawlEnemy(int nCntEnemy)
{

	float fRotDest = g_aEnemy[nCntEnemy].rot.y;	//目標の角度
	float fRotDiff = 0.0f;	//現在と目標の差分

							//目標の角度を求める
	fRotDest = atan2f(
		(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].PosCrawl[g_aEnemy[nCntEnemy].nNumPosCrawl].x),
		(g_aEnemy[nCntEnemy].pos.z - g_aEnemy[nCntEnemy].PosCrawl[g_aEnemy[nCntEnemy].nNumPosCrawl].z));

	//目標との差分
	fRotDiff = fRotDest - g_aEnemy[nCntEnemy].rot.y;

	//角度の正規化
	RotNormalize(&fRotDiff);

	//角度の補正をする
	g_aEnemy[nCntEnemy].rot.y += fRotDiff * 0.2f;

	//角度の正規化
	RotNormalize(&g_aEnemy[nCntEnemy].rot.y);

	if (CircleRange(g_aEnemy[nCntEnemy].PosCrawl[g_aEnemy[nCntEnemy].nNumPosCrawl], g_aEnemy[nCntEnemy].pos) >= (60.0f * 60.0f))
	{//まだ規定値に着いていなかったら

	 //向いてる方向にダッシュ
		g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * CRAWL_MOVE;
		g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * CRAWL_MOVE;
	}
	else
	{//規定値に着いたら

	 //巡回地点更新
		g_aEnemy[nCntEnemy].nNumPosCrawl = (g_aEnemy[nCntEnemy].nNumPosCrawl + 1) % g_aEnemy[nCntEnemy].nNumMaxCrawl;
	}
}

//==================================================================================
//敵の座標制限
//==================================================================================
void LimitPosEnemy(int nCntEnemy)
{

	if (g_aEnemy[nCntEnemy].pos.y < 0.0f)
	{//地面より下に行ったら

		g_aEnemy[nCntEnemy].pos.y = 0.0f;

		//重力処理
		g_aEnemy[nCntEnemy].move.y = 0.0f;
	}

	//移動制限
	if (g_aEnemy[nCntEnemy].pos.x + -g_aEnemy[nCntEnemy].fRadius < -LIMIT_POS) { g_aEnemy[nCntEnemy].pos.x = -LIMIT_POS - -g_aEnemy[nCntEnemy].fRadius; }
	if (g_aEnemy[nCntEnemy].pos.x + g_aEnemy[nCntEnemy].fRadius > LIMIT_POS) { g_aEnemy[nCntEnemy].pos.x = LIMIT_POS - g_aEnemy[nCntEnemy].fRadius; }
	if (g_aEnemy[nCntEnemy].pos.z + g_aEnemy[nCntEnemy].fRadius > LIMIT_POS) { g_aEnemy[nCntEnemy].pos.z = LIMIT_POS - g_aEnemy[nCntEnemy].fRadius; }
	if (g_aEnemy[nCntEnemy].pos.z + -g_aEnemy[nCntEnemy].fRadius < -LIMIT_POS) { g_aEnemy[nCntEnemy].pos.z = -LIMIT_POS - -g_aEnemy[nCntEnemy].fRadius; }

}

//==================================================================================
//弾のヒット処理
//==================================================================================
void HitEnemy(int nCntEnemy, D3DXVECTOR3 move, int nDMG)
{


}

//==================================================================================
//敵の状態更新処理
//==================================================================================
void UpdateStateEnemy(int nCntEnemy)
{

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//扇の当たり判定の情報を取得
	SearchFan *pSearchFan = GetSearchFan();

	//イベントスタンプの情報取得
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//敵とプレイヤーの距離
	float fLength = pPlayer->fRadius + g_aEnemy[nCntEnemy].fChaseRadius;

	switch (g_aEnemy[nCntEnemy].nState)
	{
	case ENEMYSTATE_NONE:

		if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT)
		{//モーションが射撃以外の時

			UpdateCrawlEnemy(nCntEnemy);
		}

		bool bRange;

		//探索範囲の判定
		bRange = CollisionFan(g_aEnemy[nCntEnemy].pos,
			D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + sinf(D3DX_PI * 0.25f + g_aEnemy[nCntEnemy].rot.y) * -g_aEnemy[nCntEnemy].fChaseRadius, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z + cosf(D3DX_PI * 0.25f + g_aEnemy[nCntEnemy].rot.y) * -g_aEnemy[nCntEnemy].fChaseRadius),
			D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + sinf(-D3DX_PI * 0.25f + g_aEnemy[nCntEnemy].rot.y) * -g_aEnemy[nCntEnemy].fChaseRadius, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z + cosf(-D3DX_PI * 0.25f + g_aEnemy[nCntEnemy].rot.y) * -g_aEnemy[nCntEnemy].fChaseRadius),
			pPlayer->pos, pPlayer->posOld);

		if (bRange == true)
		{//扇の中に入ったら

			g_aEnemy[nCntEnemy].nState = ENEMYSTATE_CHASE;

			//モーションの設定
			SetMotionEnemy(nCntEnemy, ENEMYMOTION_DASH);

			//SEセット
			PlaySound(SOUND_LABEL_SE_GET);

			if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
			{
				//セリフの設定
				g_aEnemy[nCntEnemy].nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_ENEMYDISC, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 150.0f, g_aEnemy[nCntEnemy].pos.z });
			}

			//セリフの位置更新
			SetPositionSerif(g_aEnemy[nCntEnemy].nIdxSerif, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 150.0f, g_aEnemy[nCntEnemy].pos.z });
		}

		break;

	case ENEMYSTATE_CHASE:

		if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT)
		{//モーションが射撃以外の時

		 //プレイヤーの方を見る処理
			RotFacePlayerEnemy(nCntEnemy);

			if (CircleRange(g_aEnemy[nCntEnemy].pos, pPlayer->pos) >= (fLength * fLength))
			{//探索サークルの中に入ったら

				float MaxLength = 9999999.9f;	//長さの最大値
				int nIdxCrawl = g_aEnemy[nCntEnemy].nNumPosCrawl;	//現在の巡回位置番号

				for (int nCntCrawl = 0; nCntCrawl < g_aEnemy[nCntEnemy].nNumMaxCrawl; nCntCrawl++)
				{
					float fNowLength =
						(g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].x) * (g_aEnemy[nCntEnemy].pos.x - g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].x)
						+ (g_aEnemy[nCntEnemy].pos.z - g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].z) * (g_aEnemy[nCntEnemy].pos.z - g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].z);

					//最大値が今回の長さより大きかったら
					if (MaxLength >= fNowLength)
					{
						MaxLength = fNowLength;	//最大値入れ替え
						nIdxCrawl = nCntCrawl;	//番号入れ替え
					}
				}

				//一番近い地点の番号にする
				g_aEnemy[nCntEnemy].nNumPosCrawl = nIdxCrawl;

				g_aEnemy[nCntEnemy].nState = ENEMYSTATE_NONE;

				//モーションの設定
				SetMotionEnemy(nCntEnemy, ENEMYMOTION_DEF);
			}
		}
		break;

	case ENEMYSTATE_WAIT:

		if (g_aEnemy[nCntEnemy].nCntAllFrame < 150)
		{
			SetParticle(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 50.0f, g_aEnemy[nCntEnemy].pos.z), PARTICLE_TYPE_MORSSPAWN);
		}

		if (g_aEnemy[nCntEnemy].nCntAllFrame == 230)
		{//フレーム数が30になったら

			//敵を探索状態にする
			g_aEnemy[nCntEnemy].nState = ENEMYSTATE_NONE;

			//モーションの設定
			SetMotionEnemy(nCntEnemy, ENEMYMOTION_DEF);

			//扇の当たり判定の描画を出す
			pSearchFan[g_aEnemy[nCntEnemy].nIdxSearchFan].bDisp = true;
		}
		break;

	case ENEMYSTATE_LOST:

		//影の情報取得
		SHADOW *pShadow = GetShadow();

		//マップアイコンの情報取得
		MINIICON *pMiniIcon = GetMiniIcon();

		if (g_aEnemy[nCntEnemy].nCntAllFrame < 60)
		{
			SetParticle(D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 50.0f, g_aEnemy[nCntEnemy].pos.z), PARTICLE_TYPE_MORSSPAWN);
		}

		if (g_aEnemy[nCntEnemy].nCntAllFrame == 90)
		{//フレーム数が60になったら

		 //出現していない状態にする
			g_aEnemy[nCntEnemy].bMoreSpawn = false;

			//影を使用していない状態にする
			pShadow[g_aEnemy[nCntEnemy].nIdxShadow].bUse = false;

			//扇の当たり判定の描画を消す
			pSearchFan[g_aEnemy[nCntEnemy].nIdxSearchFan].bDisp = false;

			////扇の形の位置更新
			//SetPositionSearchFan(g_aEnemy[nCount].nIdxSearchFan, g_aEnemy[nCount].pos, g_aEnemy[nCount].rot);

			//マップアイコンを使用していない状態にする
			pMiniIcon[g_aEnemy[nCntEnemy].nIdxIcon].bUse = false;

			//警備強化の時間を戻す
			if (g_MoreEnemy.SpawnCounter <= 0)
			{
				//警備強化状態を解く
				g_MoreEnemy.bMoreSpawn = false;
				g_MoreEnemy.SpawnCounter = STRENG_TIME;
			}
			if (g_MoreEnemy.ManySpawnCounter <= 0)
			{
				//警備強化状態を解く
				g_MoreEnemy.bManyMoreSpawn = false;
				g_MoreEnemy.ManySpawnCounter = STOP_MANYTIME;		//大量発生時の警備強化終了時間
			}

			//イベントスタンプ消す
			pEventStamp[EVENTTYPE_MORS].bUse = false;

			//警備強化するまでの時間を0にする
			//g_MoreEnemy.MoreSecurityCounter = 0;
		}
		break;
	}
}

//==================================================================================
//敵の行動更新処理
//==================================================================================
void UpdateActionEnemy(int nCntEnemy)
{

}

//==================================================================================
//敵のセリフ設定処理
//==================================================================================
void EnemySerifSet(int nCntEnemy)
{
	if (g_nEnemySerifTime >= 432 && GetMode() == MODE_GAME || g_nEnemySerifTime >= 432 && GetMode() == MODE_TUTORIAL)
	{//一定時間経ったら

		 //セリフの設定
		g_aEnemy[nCntEnemy].nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_ENEMYNONE, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 110.0f, g_aEnemy[nCntEnemy].pos.z });

		//セリフの位置更新
		SetPositionSerif(g_aEnemy[nCntEnemy].nIdxSerif, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 110.0f, g_aEnemy[nCntEnemy].pos.z });

		//時間初期化
		g_nEnemySerifTime = 0;
	}
	else
	{
		//セリフ設定時間加算
		g_nEnemySerifTime++;
	}
}

//==================================================================================
//敵との当たり判定
//==================================================================================
void CollisionModelEnemy(int nCntEnemy)
{
	//敵の情報取得
	Enemy *pEnemy = GetEnemy();

	if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_WAIT)
	{
		for (int nCount = 0; nCount < MAX_ENEMY; nCount++, pEnemy++)
		{
			if (pEnemy->bUse == true)
			{//敵が使用されていたら

			 //キャラクター同士の当たり判定
				CollisionCharacter(
					&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, D3DXVECTOR3(g_aEnemy[nCntEnemy].fRadius, 0.0f, g_aEnemy[nCntEnemy].fRadius), D3DXVECTOR3(-g_aEnemy[nCntEnemy].fRadius, 0.0f, -g_aEnemy[nCntEnemy].fRadius),
					&pEnemy->pos, D3DXVECTOR3(pEnemy->fRadius, 0.0f, pEnemy->fRadius), D3DXVECTOR3(-pEnemy->fRadius, 0.0f, -pEnemy->fRadius));
			}
		}

		float nnn = D3DX_PI * 0.5f;

		//モデルの情報取得
		ModelModel *pModel = GetModel();

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
		{
			if (pModel->bUse == true && pModel->nType < SMALL_NUM)
			{//モデルが使用されていたら

			 //キャラクター同士の当たり判定
				CollisionCharacter(
					&g_aEnemy[nCntEnemy].pos, &g_aEnemy[nCntEnemy].posOld, D3DXVECTOR3(g_aEnemy[nCntEnemy].fRadius, 0.0f, g_aEnemy[nCntEnemy].fRadius), D3DXVECTOR3(-g_aEnemy[nCntEnemy].fRadius, 0.0f, -g_aEnemy[nCntEnemy].fRadius),
					&pModel->pos, pModel->vtxMax, pModel->vtxMin);
			}
		}

		//群衆の情報取得
		Crowd *pCrowd = GetCrowd();

		if (g_aEnemy[nCntEnemy].nState != ENEMYSTATE_LOST)
		{
			int nnnn = 0;
			for (int nCount = 0; nCount < MAX_CROWD; nCount++, pCrowd++)
			{
				if (pCrowd->aModel.bUse == true)
				{//群衆が使用されていたら

					//敵と群衆の距離
					float fLength = pCrowd->aModel.vtxMax.x + SEARCH_RADIUS;

					if (SphereRange(g_aEnemy[nCntEnemy].pos, pCrowd->aModel.pos) <= (fLength * fLength))
					{//球の中に入ったら

						nnnn++;

						if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT)
						{
							//モーションの設定
							SetMotionEnemy(nCntEnemy, ENEMYMOTION_SHOT);
						}

						if (pCrowd->nState != CROWDSTATE_WAIT && pCrowd->nState != CROWDSTATE_FADE)
						{
							//射撃待ち状態にする
							pCrowd->nState = CROWDSTATE_WAIT;
							pCrowd->nCntChase = 0;
						}

						if (pCrowd->nState == CROWDSTATE_WAIT)
						{
							//射撃エフェクト
							ShotBulletEnemy(nCntEnemy, pCrowd->aModel.pos);
						}
					}
				}
			}

			if (nnnn == 0 && g_aEnemy[nCntEnemy].nNowMotionNum == ENEMYMOTION_SHOT)
			{//球の中に群衆が入ってなくて敵のモーションが射撃モーション中だったら

				g_aEnemy[nCntEnemy].nCntState = ENEMYSTATE_NONE;

				//モーションの設定
				SetMotionEnemy(nCntEnemy, ENEMYMOTION_DEF);
			}
		}
	}
}

//==================================================================================
//水鉄砲発射処理
//==================================================================================
void ShotBulletEnemy(int nCntEnemy, D3DXVECTOR3 pos)
{
	//群衆の情報を取得
	Crowd *pCrowd = GetCrowd();

	if (g_aEnemy[nCntEnemy].nNowMotionNum == ENEMYMOTION_SHOT)
	{
		float fRotDest = g_aEnemy[nCntEnemy].rot.y;	//目標の角度
		float fRotDiff = 0.0f;	//現在と目標の差分

								//目標の角度を求める
		fRotDest = atan2f(
			(g_aEnemy[nCntEnemy].pos.x - pos.x),
			(g_aEnemy[nCntEnemy].pos.z - pos.z));

		//目標との差分
		fRotDiff = fRotDest - g_aEnemy[nCntEnemy].rot.y;

		//角度の正規化
		RotNormalize(&fRotDiff);

		//角度の補正をする
		g_aEnemy[nCntEnemy].rot.y += fRotDiff * 1.0f;

		//角度の正規化
		RotNormalize(&g_aEnemy[nCntEnemy].rot.y);

		if (g_aEnemy[nCntEnemy].nCntAllFrame == 15)
		{//カウントが15に達したら

			float fMove, fMoveY, ffffff;

			for (int nCntUse = 0; nCntUse < 40; nCntUse++)
			{
				fMove = (float)(rand() % 50) / 10 + 1.0f;		//移動量
				fMoveY = (float)(rand() % 11 - 5) / 10;	//移動量

				D3DXVECTOR3 move;

				//移動量の設定
				move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
				move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
				ffffff = (float)(rand() % 51 - 25) / 100.0f;

				float fRadius = 10.0f;

				if (rand() % 3 == 0)
				{
					fRadius *= 0.95f;
				}
				else if (rand() % 3 == 1)
				{
					fRadius *= 0.98f;
				}
				else if (rand() % 3 == 2)
				{
					fRadius *= 0.92f;
				}

				//エフェクトの設定
				SetWatergun({ g_aEnemy[nCntEnemy].pos.x + sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 0.0f, g_aEnemy[nCntEnemy].pos.y + 75.0f, g_aEnemy[nCntEnemy].pos.z + cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 35.0f },
				{ 0.5f , 0.5f , 1.0f , 1.0f },
					fRadius,
					30,
					D3DXVECTOR3(sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y + ffffff) * fMove, fMoveY, cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y + ffffff) * fMove),
					MOVEEFFECT_SUB, WATERBULLETTYPE_BLACK);
			}

			if (GetMode() != MODE_RESULT && g_SoundCoolTime <= 0)
			{//サウンドのクールタイムが終わっていたら

				//SEをセット
				PlaySound(SOUND_LABEL_SE_SHIBUKI);

				//クールタイム設定
				g_SoundCoolTime = SOUNDCOOLTIME;
			}
		}

	}
}

//==================================================================================
//敵の警備強化処理
//==================================================================================
void MoreSpawnEnemy(void)
{
	//扇の当たり判定の情報を取得
	SearchFan *pSearchFan = GetSearchFan();

	int nMoreCount = 0;		//ウィンドウ表示時間
	int nSpawnTime = 0;		//スポーン時間

							//時間設定
	if (g_MoreEnemy.bFirst == false)
	{//1番目
		nMoreCount = FIRST_SPAWN - 250;		//ウィンドウ時間
		nSpawnTime = FIRST_SPAWN;			//スポーン時間
	}
	else if (g_MoreEnemy.bSecond == false)
	{//2番目
		nMoreCount = SECOND_SPAWN - 250;	//ウィンドウ時間
		nSpawnTime = SECOND_SPAWN;			//スポーン時間
	}
	//else if (g_MoreEnemy.bThird == false)
	//{//3番目
	//	nMoreCount = THIRD_SPAWN - 250;	//ウィンドウ時間
	//	nSpawnTime = THIRD_SPAWN;			//スポーン時間
	//}
	else
	{
		nMoreCount = 10000000;
		nSpawnTime = 10000000;
	}

	if (g_MoreEnemy.MoreSecurityCounter == nMoreCount)
	{
		//イベントウィンドウの設定
		SetEventWindow();

		PlaySound(SOUND_LABEL_SE_KEIBIKYOUKA);
	}


	if (g_MoreEnemy.MoreSecurityCounter >= nSpawnTime)
	{//警備強化するまでの時間が3000になったら

		if (g_MoreEnemy.bFirst == false)
		{//1番目
			g_MoreEnemy.bFirst = true;		//使用した
		}
		else if (g_MoreEnemy.bSecond == false)
		{//2番目
			g_MoreEnemy.bSecond = true;		//使用した
		}
		//else if (g_MoreEnemy.bThird == false)
		//{//3番目
		//	g_MoreEnemy.bThird = true;		//使用した
		//}

		//g_MoreEnemy.MoreSecurityCounter = 0;

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			if (g_aEnemy[nCount].bMoreSpawn == false)
			{//使用していない状態だったら

				if (g_aEnemy[nCount].MassGeneration == 1)
				{//MassGenerationが1だったら

					//敵を出現待ち状態にする
					g_aEnemy[nCount].nState = ENEMYSTATE_WAIT;

					//モーションの設定
					SetMotionEnemy(nCount, ENEMYMOTION_WAIT);

					//出現している状態にする
					g_aEnemy[nCount].bMoreSpawn = true;

					//警備強化状態にする
					g_MoreEnemy.bMoreSpawn = true;

					//原点に戻す
					g_aEnemy[nCount].pos = g_aEnemy[nCount].posOrigin;

					//巡回地点のリセット
					g_aEnemy[nCount].nNumPosCrawl = 0;

					//影を設定
					g_aEnemy[nCount].nIdxShadow = SetShadow(25.0f, 25.0f);

					//影の位置更新
					SetPositionShadow(g_aEnemy[nCount].nIdxShadow, g_aEnemy[nCount].pos);

					//アイコンの設定処理
					g_aEnemy[nCount].nIdxIcon = SetIcon(MINIMAP_ICON_ENEMY, 80.0f);
					UpdatePositionIcon(g_aEnemy[nCount].nIdxIcon, g_aEnemy[nCount].pos);
				}
			}
		}
	}

	//影の情報取得
	SHADOW *pShadow = GetShadow();

	//マップアイコンの情報取得
	MINIICON *pMiniIcon = GetMiniIcon();

	if (g_MoreEnemy.SpawnCounter <= 0 && g_MoreEnemy.bMoreSpawn == true)
	{//カウントが0なったら

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			if (g_aEnemy[nCount].MassGeneration == 1)
			{
				if (g_aEnemy[nCount].nNowMotionNum != ENEMYMOTION_LOST)
				{

					SetMotionEnemy(nCount, ENEMYMOTION_LOST);
				}

				g_aEnemy[nCount].nState = ENEMYSTATE_LOST;
			}
		}
	}
}

//==================================================================================
//敵の大量発生時の警備強化処理
//==================================================================================
void ManyMoreSpawnEnemy(void)
{
	//扇の当たり判定の情報を取得
	SearchFan *pSearchFan = GetSearchFan();
	int nSpawnTime = 0;		//スポーン時間

	//時間設定
	if (g_MoreEnemy.bManyFirst == false)
	{//1番目
		nSpawnTime = FIRST_SPAWNMOB;			//スポーン時間
	}
	else if (g_MoreEnemy.bManySecond == false)
	{//2番目
		nSpawnTime = SECOND_SPAWNMOB;			//スポーン時間
	}
	else if (g_MoreEnemy.bManyThird == false)
	{//3番目
		nSpawnTime = THIRD_SPAWNMOB;			//スポーン時間
	}
	else
	{
		nSpawnTime = 10000000;
	}

	if (g_MoreEnemy.MoreSecurityCounter >= nSpawnTime)
	{//警備強化するまでの時間が3000になったら

		if (g_MoreEnemy.bManyFirst == false)
		{//1番目
			g_MoreEnemy.bManyFirst = true;		//使用した
		}
		else if (g_MoreEnemy.bManySecond == false)
		{//2番目
			g_MoreEnemy.bManySecond = true;		//使用した
		}
		else if (g_MoreEnemy.bManyThird == false)
		{//3番目
			g_MoreEnemy.bManyThird = true;		//使用した
		}

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			if (g_aEnemy[nCount].bMoreSpawn == false)
			{//使用していない状態だったら

				if (g_aEnemy[nCount].MassGeneration == 2)
				{//MassGenerationが2だったら

					//敵を出現待ち状態にする
					g_aEnemy[nCount].nState = ENEMYSTATE_WAIT;

					//モーションの設定
					SetMotionEnemy(nCount, ENEMYMOTION_WAIT);

					//出現している状態にする
					g_aEnemy[nCount].bMoreSpawn = true;

					//警備強化状態にする
					g_MoreEnemy.bManyMoreSpawn = true;

					//原点に戻す
					g_aEnemy[nCount].pos = g_aEnemy[nCount].posOrigin;

					//巡回地点のリセット
					g_aEnemy[nCount].nNumPosCrawl = 0;

					//影を設定
					g_aEnemy[nCount].nIdxShadow = SetShadow(25.0f, 25.0f);

					//影の位置更新
					SetPositionShadow(g_aEnemy[nCount].nIdxShadow, g_aEnemy[nCount].pos);

					//アイコンの設定処理
					g_aEnemy[nCount].nIdxIcon = SetIcon(MINIMAP_ICON_ENEMY, 80.0f);
					UpdatePositionIcon(g_aEnemy[nCount].nIdxIcon, g_aEnemy[nCount].pos);
				}
			}
		}
	}

	//影の情報取得
	SHADOW *pShadow = GetShadow();

	//マップアイコンの情報取得
	MINIICON *pMiniIcon = GetMiniIcon();

	if (g_MoreEnemy.ManySpawnCounter <= 0 && g_MoreEnemy.bManyMoreSpawn == true)
	{//カウントが0なったら

		for (int nCount = 0; nCount < MAX_ENEMY; nCount++)
		{
			if (g_aEnemy[nCount].MassGeneration == 2)
			{
				if (g_aEnemy[nCount].nNowMotionNum != ENEMYMOTION_LOST)
				{

					SetMotionEnemy(nCount, ENEMYMOTION_LOST);
				}

				g_aEnemy[nCount].nState = ENEMYSTATE_LOST;
			}
		}
	}
}

//==================================================================================
//敵の描画処理
//==================================================================================
void DrawEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxnParent;			//親の計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATERIAL mMatRed;			//ダメージマテリアル

	//他の情報クリア
	ZeroMemory(&mMatRed, sizeof(D3DXMATERIAL));
	mMatRed.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	mMatRed.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bMoreSpawn == true)
		{//使用していたら

		 //ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].rot.y, g_aEnemy[nCntEnemy].rot.x, g_aEnemy[nCntEnemy].rot.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y, g_aEnemy[nCntEnemy].pos.z);
			D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].mtxWorld, &g_aEnemy[nCntEnemy].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			for (int nCntParts = 0; nCntParts < g_aEnemy[nCntEnemy].nPartsNum; nCntParts++)
			{

				//ワールドマトリックスの初期化
				D3DXMatrixIdentity(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				//向きを反映する
				D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x, g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxRot);

				//位置を反映する
				D3DXMatrixTranslation(&mtxTrans, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.x, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.y, g_aEnemy[nCntEnemy].aModel[nCntParts].pos.z);
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxTrans);


				if (g_aEnemy[nCntEnemy].aModel[nCntParts].nParent == -1)
				{//NULLの場合,親

				 //プレイヤーの向きを反映する
					mtxnParent = g_aEnemy[nCntEnemy].mtxWorld;
				}
				else
				{//パーツ

				 //親の向きを反映する
					mtxnParent = g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].aModel[nCntParts].nParent].mtxWorld;
				}

				//自分のやつに親のワールドマトリックスを掛け算
				D3DXMatrixMultiply(&g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld, &mtxnParent);

				//ワールドマトリックスの設定
				pDevice->SetTransform(D3DTS_WORLD, &g_aEnemy[nCntEnemy].aModel[nCntParts].mtxWorld);

				//マテリアルデータへのポインタを取得
				pMat = (D3DXMATERIAL*)g_aEnemy[nCntEnemy].aModel[nCntParts].pBuffMat->GetBufferPointer();

				if (g_aEnemy[nCntEnemy].aModel[nCntParts].bUse == true)
				{//パーツが使用されていたら

				 //頂点数分繰り返し
					for (int nCntMat = 0; nCntMat < (int)g_aEnemy[nCntEnemy].aModel[nCntParts].dwNumMat; nCntMat++)
					{

						//マテリアルの設定
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//テクスチャの設定
						pDevice->SetTexture(0, g_aEnemy[nCntEnemy].aModel[nCntParts].pTexture[nCntMat]);

						//プレイヤー(パーツ)の描画
						g_aEnemy[nCntEnemy].aModel[nCntParts].pMesh->DrawSubset(nCntMat);
					}
				}
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//敵のモーション処理
//==================================================================================
void SetMotionEnemy(int nCntEnemy, int nMotionType)
{

	//今のモーションの更新
	g_aEnemy[nCntEnemy].nNowMotionNum = nMotionType;
	g_aEnemy[nCntEnemy].nPatternKey = 0;	//現在のキー番号リセット
	g_aEnemy[nCntEnemy].nCntFrame = 0;	//フレームカウントリセット
	g_aEnemy[nCntEnemy].nCntAllFrame = 0;	//総フレーム数リセット
}

//==================================================================================
//敵のモーション更新処理
//==================================================================================
void UpdateMotionEnemy(int nCntEnemy)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	for (int nCntParts = 0; nCntParts < g_aEnemy[nCntEnemy].nPartsNum; nCntParts++)
	{//パーツ分繰り返す

	 //次のキー
		int nNextKey = (g_aEnemy[nCntEnemy].nPatternKey + 1) % g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].nNumKey;

		//前回と次のキーの差分取得
		float rotDiffX = g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.z;

		//パーツの向きを設定
		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.x =
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)g_aEnemy[nCntEnemy].nCntFrame /
				(float)g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].nFrame
				);

		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.y =
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)g_aEnemy[nCntEnemy].nCntFrame /
				(float)g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].nFrame
				);

		g_aEnemy[nCntEnemy].aModel[nCntParts].rot.z =
			g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)g_aEnemy[nCntEnemy].nCntFrame /
				(float)g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].nFrame
				);

		//角度の正規化
		RotNormalize(&g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].nNowMotionNum].rot.x);
		RotNormalize(&g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].nNowMotionNum].rot.y);
		RotNormalize(&g_aEnemy[nCntEnemy].aModel[g_aEnemy[nCntEnemy].nNowMotionNum].rot.z);

	}

	//フレームのカウントを加算
	g_aEnemy[nCntEnemy].nCntFrame++;
	g_aEnemy[nCntEnemy].nCntAllFrame++;

	if (g_aEnemy[nCntEnemy].nCntFrame >= g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].aKey[g_aEnemy[nCntEnemy].nPatternKey].nFrame)
	{//フレームのカウントがフレーム数に達したら

	 //フレームのカウントをゼロに戻す
		g_aEnemy[nCntEnemy].nCntFrame = 0;

		//パターンNO.更新
		g_aEnemy[nCntEnemy].nPatternKey = (g_aEnemy[nCntEnemy].nPatternKey + 1) % g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].nNumKey;

		if (g_aEnemy[nCntEnemy].nPatternKey == 0)
		{//キーがゼロの時

		 //攻撃状態解除
			g_aEnemy[nCntEnemy].nCntAllFrame = 0;	//総フレーム数リセット

			if (g_aEnemy[nCntEnemy].aMotionData[g_aEnemy[nCntEnemy].nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//モーションの設定
				SetMotionEnemy(nCntEnemy, ENEMYMOTION_DEF);
			}
			else
			{
				//モーションの設定
				SetMotionEnemy(nCntEnemy, g_aEnemy[nCntEnemy].nNowMotionNum);
			}
		}
	}
}

//==================================================================================
//敵の設定処理
//==================================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//モデルの情報取得
	Model *pModel = GetXLoadData();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//扇の当たり判定の情報を取得
	SearchFan *pSearchFan = GetSearchFan();

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false)
		{//使用していなかったら

			for (int nCntFileNum = 0; nCntFileNum < g_aEnemy[nCntEnemy].nPartsNum; nCntFileNum++)
			{//パーツ数分読み込む

			 //モデル情報取得
				g_aEnemy[nCntEnemy].aModel[nCntFileNum] = g_aXLoadMeshEnemy[nCntFileNum];
			}

			//モデル情報取得
			g_aEnemy[nCntEnemy].pos = pos;	//位置
			g_aEnemy[nCntEnemy].posOrigin = g_aEnemy[nCntEnemy].pos;
			g_aEnemy[nCntEnemy].rot = rot;	//向き

			g_aEnemy[nCntEnemy].bMoreSpawn = false;	//使用している状況にする

													//扇の形の設定
			g_aEnemy[nCntEnemy].nIdxSearchFan = SetSearchFan();

			//扇の形の位置更新
			SetPositionSearchFan(g_aEnemy[nCntEnemy].nIdxSearchFan, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);

			//見えなくする
			pSearchFan[g_aEnemy[nCntEnemy].nIdxSearchFan].bDisp = false;

			if (g_aEnemy[nCntEnemy].MassGeneration != 1 && g_aEnemy[nCntEnemy].MassGeneration != 2)
			{
				g_aEnemy[nCntEnemy].bMoreSpawn = true;	//使用している状況にする

														//影を設定
				g_aEnemy[nCntEnemy].nIdxShadow = SetShadow(25.0f, 25.0f);

				//影の位置更新
				SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);

				//アイコンの設定処理
				g_aEnemy[nCntEnemy].nIdxIcon = SetIcon(MINIMAP_ICON_ENEMY, 80.0f);
				UpdatePositionIcon(g_aEnemy[nCntEnemy].nIdxIcon, g_aEnemy[nCntEnemy].pos);

				//見えるようにする
				pSearchFan[g_aEnemy[nCntEnemy].nIdxSearchFan].bDisp = true;
			}

			g_aEnemy[nCntEnemy].bUse = true;     //使用している状況にする
			g_aEnemy[nCntEnemy].nATKInterval = ATK_INTERVAL;		//攻撃間隔
			g_aEnemy[nCntEnemy].nState = ENEMYSTATE_WAIT;

			g_aEnemy[nCntEnemy].nCntState = 0;
			g_aEnemy[nCntEnemy].nCntAction = 200;
			g_aEnemy[nCntEnemy].fChaseRadius = CHASE_RADIUS;	//追い掛け範囲の設定
			g_aEnemy[nCntEnemy].fSearchRadius = SEARCH_RADIUS;	//探索範囲の設定
			g_aEnemy[nCntEnemy].fSearchRot = 0.0f;	//探索の向き
			g_aEnemy[nCntEnemy].fSearchLength = 100.0f + (float)(rand() % 10) * 10;	//探索の長さ
			g_aEnemy[nCntEnemy].nCntATK = 0;	//攻撃カウントリセット

			for (int nCntParts = 0; nCntParts < g_aEnemy[nCntEnemy].nPartsNum; nCntParts++)
			{
				g_aEnemy[nCntEnemy].aModel[nCntParts].bUse = true;
			}
			g_aEnemyInfo.nNumEnemy++;	//敵の数加算

			//モーションの設定
			SetMotionEnemy(nCntEnemy, ENEMYSTATE_WAIT);

			break;
		}
	}
}

//==================================================================================
//敵のXファイル読み込み処理
//==================================================================================
void XFileLoadEnemy(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntFileNum = 0; nCntFileNum < g_aEnemy[0].nPartsNum; nCntFileNum++)
	{//パーツ数分読み込む

	 //Xファイルの読み込み
		D3DXLoadMeshFromX(&g_aXLoadMeshEnemy[nCntFileNum].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aXLoadMeshEnemy[nCntFileNum].pBuffMat,
			NULL,
			&g_aXLoadMeshEnemy[nCntFileNum].dwNumMat,
			&g_aXLoadMeshEnemy[nCntFileNum].pMesh);

		D3DXMATERIAL *pMat;		//マテリアルデータへのポインタ

								//マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)g_aXLoadMeshEnemy[nCntFileNum].pBuffMat->GetBufferPointer();

		//頂点数分繰り返し
		for (int nCntMat = 0; nCntMat < (int)g_aXLoadMeshEnemy[nCntFileNum].dwNumMat; nCntMat++)
		{
			g_aXLoadMeshEnemy[nCntFileNum].pTexture[nCntMat] = NULL;

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//ファイルが存在する

			 //テクスチャの読み込み
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aXLoadMeshEnemy[nCntFileNum].pTexture[nCntMat]);
			}
		}

		CheckVtx(g_aXLoadMeshEnemy[nCntFileNum].rot.y, &g_aXLoadMeshEnemy[nCntFileNum].vtxMax, &g_aXLoadMeshEnemy[nCntFileNum].vtxMin, g_aXLoadMeshEnemy[nCntFileNum].pMesh, g_aXLoadMeshEnemy[nCntFileNum].pVtxBuff);

	}
}

//==================================================================================
//敵の読み込み処理
//==================================================================================
void ReadSetEnemy(void)
{
	FILE *pFile = NULL;	//ファイルポインタを宣言

						//ファイルを開く
	pFile = fopen(ENEMYSET_TXT, "r");

	char aComment[MAX_COMMENT];	//コメント、確認等
	int nCntEnemy = 0;

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{//END_SCRIPTが来るまで繰り返す

		 //文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//敵の設定
			if (strcmp(aComment, "ENEMYSET") == 0)
			{//モデルの読み込みを開始

			 //巡回地点のカウント
				int nCntCrawl = 0;

				while (strcmp(aComment, "END_ENEMYSET"))
				{//END_ENEMYSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "POS") == 0)
					{//POSが来たら位置読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.x);	//X座標
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.y);	//Y座標
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].pos.z);	//Z座標
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROTが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.x);	//Xの向き
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.y);	//Yの向き
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].rot.z);	//Zの向き
					}

					if (strcmp(aComment, "CRAWLPOS") == 0)
					{//CRAWLPOSが来たら巡回地点読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].x);	//X座標
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].y);	//Y座標
						fscanf(pFile, "%f", &g_aEnemy[nCntEnemy].PosCrawl[nCntCrawl].z);	//Z座標

																							//巡回地点の番号加算
						nCntCrawl++;
					}

					if (strcmp(aComment, "SPAWN") == 0)
					{
						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aEnemy[nCntEnemy].MassGeneration);   //常に巡回しているか大量発生時に巡回するか
					}

					if (strcmp(aComment, "LIFE") == 0)
					{//LIFEが来たら体力読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
					}

				}//END_ENEMYSETのかっこ

				 //巡回地点の最大数
				g_aEnemy[nCntEnemy].nNumMaxCrawl = nCntCrawl;

				//敵の番号加算
				nCntEnemy++;

			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//終了文字でループを抜ける

				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	//Xファイルの読み込み
	XFileLoadEnemy();

	if (GetMode() == MODE_GAME)
	{
		//敵の配置
		for (int nCount = 0; nCount < nCntEnemy; nCount++)
		{
			SetEnemy(g_aEnemy[nCount].pos, g_aEnemy[nCount].rot);
		}
	}
}

//==================================================================================
//敵の読み込み処理
//==================================================================================
void ReadSetMotionEnemy(void)
{

	FILE *pFile = NULL;	//ファイルポインタを宣言

	char aComment[MAX_COMMENT];	//コメント用
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0, nCntFileName = 0, nCntSetParts = 0;

	//ファイルを開く
	pFile = fopen(ENEMY_TXT, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{//END_SCRIPTが来るまで繰り返す

		 //文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//==================================================
			//階層構造の設定
			//==================================================
			//モデル数の設定
			if (strcmp(aComment, "NUM_MODEL") == 0)
			{//NUM_MODELがきたら

				fscanf(pFile, "%s", &aComment[0]);	//=の分
				fscanf(pFile, "%d", &g_aEnemy[0].nPartsNum);	//モデル数
			}

			while (nCntFileName != g_aEnemy[0].nPartsNum)
			{//モデルの数分読み込むまで繰り返し

			 //文字列の読み込み
				fscanf(pFile, "%s", &aComment[0]);

				//モデル名の設定
				if (strcmp(aComment, "MODEL_FILENAME") == 0)
				{//NUM_MODELがきたら

					fscanf(pFile, "%s", &aComment[0]);	//=の分
					fscanf(pFile, "%s", &g_aXLoadMeshEnemy[nCntFileName].acFilename[0]);	//ファイル名
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
								fscanf(pFile, "%d", &g_aXLoadMeshEnemy[nCntSetParts].nType);	//モデル種類の列挙

							}

							if (strcmp(aComment, "PARENT") == 0)
							{//PARENTが来たら親の番号読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%d", &g_aXLoadMeshEnemy[nCntSetParts].nParent);	//親の番号

							}

							if (strcmp(aComment, "POS") == 0)
							{//POSが来たら位置読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].pos.x);	//X座標
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].pos.y);	//Y座標
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].pos.z);	//Z座標

																								//元の位置割り当て
								g_aXLoadMeshEnemy[nCntSetParts].posOrigin = g_aXLoadMeshEnemy[nCntSetParts].pos;

							}

							if (strcmp(aComment, "ROT") == 0)
							{//ROTが来たら向き読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].rot.x);	//Xの角度
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].rot.y);	//Yの角度
								fscanf(pFile, "%f", &g_aXLoadMeshEnemy[nCntSetParts].rot.z);	//Zの角度

								g_aXLoadMeshEnemy[nCntSetParts].rot = D3DXToRadian(g_aXLoadMeshEnemy[nCntSetParts].rot);

							}

							if (strcmp(aComment, "WEAPON") == 0)
							{//WEAPONが来たら武器かどうか読み込み

								fscanf(pFile, "%s", &aComment[0]);		//=の分

							}

						}//END_PARTSSETのかっこ

						nCntSetParts++;	//パーツのカウントを加算
					}

				}//END_CHARACTERSETのかっこ

			}

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
						fscanf(pFile, "%d", &g_aEnemy[0].aMotionData[nCntMotion].nLoop);	//ループ0か1

					}

					if (strcmp(aComment, "NUM_KEY") == 0)
					{//NUM_KEYが来たらキー数読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aEnemy[0].aMotionData[nCntMotion].nNumKey);	//キー数

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
								fscanf(pFile, "%d", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//再生フレーム

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
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X座標
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y座標
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z座標
									}

									if (strcmp(aComment, "ROT") == 0)
									{//ROTが来たら角度読み込み

										fscanf(pFile, "%s", &aComment[0]);		//=の分
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X角度
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y角度
										fscanf(pFile, "%f", &g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z角度
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

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].nPartsNum = g_aEnemy[0].nPartsNum;

		for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
		{//モーションの数繰り返す

			g_aEnemy[nCntEnemy].aMotionData[nCntMotion].nLoop = g_aEnemy[0].aMotionData[nCntMotion].nLoop;
			g_aEnemy[nCntEnemy].aMotionData[nCntMotion].nNumKey = g_aEnemy[0].aMotionData[nCntMotion].nNumKey;

			for (int nCntKey = 0; nCntKey < g_aEnemy[0].aMotionData[nCntMotion].nNumKey; nCntKey++)
			{//キーの数繰り返す

				g_aEnemy[nCntEnemy].aMotionData[nCntMotion].aKey[nCntKey].nFrame = g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].nFrame;

				for (int nCntParts = 0; nCntParts < g_aEnemy[0].nPartsNum; nCntParts++)
				{//パーツ分繰り返す

					g_aEnemy[nCntEnemy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot;
					g_aEnemy[nCntEnemy].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = g_aEnemy[0].aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos;
				}
			}
		}
	}
}

//==================================================================================
//敵の終了時更新処理
//==================================================================================
void UpdateGameEndEnemy(int nCntEnemy)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//向き固定
	g_aEnemy[nCntEnemy].rot.y = 0.0f;

	if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_DASH)
	{
		//モーションの設定
		SetMotionEnemy(nCntEnemy, ENEMYMOTION_DASH);
	}

	//向いてる方向にダッシュ
	g_aEnemy[nCntEnemy].move.x += sinf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 1.7f;
	g_aEnemy[nCntEnemy].move.z += cosf(D3DX_PI + g_aEnemy[nCntEnemy].rot.y) * 1.7f;

	//位置更新
	g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;

	//慣性つける
	g_aEnemy[nCntEnemy].move.x += (0.0f - g_aEnemy[nCntEnemy].move.x) * MOVE_FACTOR;
	g_aEnemy[nCntEnemy].move.z += (0.0f - g_aEnemy[nCntEnemy].move.z) * MOVE_FACTOR;
}

//==================================================================================
//敵のチュートリアル更新処理
//==================================================================================
void UpdateTutorialEnemy(void)
{
	//プレイヤーの情報取得
	ModelModel *pModel = GetModel();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();

	//群衆の情報取得
	Crowd *pCrowd = GetCrowd();

	//警備強化
	MoreSpawnEnemy();

	if (pTutorial->nStep >= TUTORIAL_STEP_MORS && pTutorial->aControl.bSetEnemy == false)
	{//催眠のステップ以上 && モブが0

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			//巡回地点の最大数
			g_aEnemy[nCnt].nNumMaxCrawl = 1;
			g_aEnemy[nCnt].PosCrawl[0] = pPlayer->pos;

			//みんな通常スポーン
			g_aEnemy[nCnt].MassGeneration = 99;

			//敵を出現待ち状態にする
			g_aEnemy[nCnt].nState = ENEMYSTATE_WAIT;

			//モーションの設定
			SetMotionEnemy(nCnt, ENEMYMOTION_WAIT);

			g_aEnemy[nCnt].bMoreSpawn = true;	//使用している状況にする
		}

		SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * 1.0f) * 150.0f, pPlayer->pos.y, pPlayer->pos.z + cosf(D3DX_PI * 1.0f) * 150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * 0.5f) * 150.0f, pPlayer->pos.y, pPlayer->pos.z + cosf(D3DX_PI * 0.5f) * 150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * 0.0f) * 150.0f, pPlayer->pos.y, pPlayer->pos.z + cosf(D3DX_PI * 0.0f) * 150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		SetEnemy(D3DXVECTOR3(pPlayer->pos.x + sinf(D3DX_PI * -0.5f) * 150.0f, pPlayer->pos.y, pPlayer->pos.z + cosf(D3DX_PI * -0.5f) * 150.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		//敵配置完了
		pTutorial->aControl.bSetEnemy = true;

		//プレイヤー止める
		pPlayer->bATK = false;
		pPlayer->bMove = false;
		SetMotisonPlayer(PLAYERMOTION_DEF);
		pBuddy[BUDDYTYPE_HYOROGARI].bATK = false;
		pBuddy[BUDDYTYPE_DEPPA].bATK = false;

		if (pBuddy[BUDDYTYPE_HYOROGARI].nState == BUDDYSTATE_DISPATCH)
		{//派遣状態なら

		 //呼び戻し
			SetLeftDisPatch();
			pBuddy[BUDDYTYPE_HYOROGARI].nState = BUDDYSTATE_NONE;
			SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_DEF);
		}

		if (pBuddy[BUDDYTYPE_DEPPA].nState == BUDDYSTATE_DISPATCH)
		{//派遣状態なら

			//呼び戻し
			SetRightDisPatch();
			pBuddy[BUDDYTYPE_DEPPA].nState = BUDDYSTATE_NONE;
			SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_DEF);
		}
	}

	if (pTutorial->nStep == TUTORIAL_STEP_END)
	{//催眠のステップ以上 && モブが0

		for (int nCnt = 0; nCnt < 4; nCnt++)
		{
			if (g_aEnemy[nCnt].nState != ENEMYSTATE_LOST)
			{
				g_aEnemy[nCnt].nState = ENEMYSTATE_LOST;
			}

			if (g_aEnemy[nCnt].nNowMotionNum != ENEMYMOTION_LOST)
			{

				SetMotionEnemy(nCnt, ENEMYMOTION_LOST);
			}
		}
	}

	for (int nCnt = 0; nCnt < 4; nCnt++)
	{
		g_aEnemy[nCnt].PosCrawl[0] = pPlayer->pos;
	}


	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bMoreSpawn == true)
		{//使用していたら

		 //過去の位置保存
			g_aEnemy[nCntEnemy].posOld = g_aEnemy[nCntEnemy].pos;

			if (g_aEnemy[nCntEnemy].nCntSearchCircle >= SEARCH_INTERVAL - 1)
			{//規定値超えたら

				g_aEnemy[nCntEnemy].nIdxSearchRadius = SetSearchCircle(
					D3DXVECTOR3(
						g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 10.0f, g_aEnemy[nCntEnemy].pos.z),
					g_aEnemy[nCntEnemy].rot,
					D3DXCOLOR(1.0f, 0.2f, 0.2f, 0.5f),
					10.0f, 0.0f,
					(int)(SEARCH_RADIUS * 0.5f), 2.0f,
					SEARCH_CIRCLE_TYPE_ORANGE2);
			}

			//探索範囲円のカウント加算
			g_aEnemy[nCntEnemy].nCntSearchCircle = (g_aEnemy[nCntEnemy].nCntSearchCircle + 1) % SEARCH_INTERVAL;

			if (g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_SHOT || g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_WAIT || g_aEnemy[nCntEnemy].nNowMotionNum != ENEMYMOTION_LOST)
			{//モーションが射撃、敵の状態が出現待ちまたは、消滅待ち以外の時

			 //敵の操作
				ControllEnemy(nCntEnemy);
			}

			//敵同士の当たり判定
			CollisionModelEnemy(nCntEnemy);

			//座標制限
			LimitPosEnemy(nCntEnemy);

			//状態更新
			UpdateStateEnemy(nCntEnemy);

			//行動更新処理
			UpdateActionEnemy(nCntEnemy);

			//モーション更新処理
			UpdateMotionEnemy(nCntEnemy);

			//影の位置更新
			SetPositionShadow(g_aEnemy[nCntEnemy].nIdxShadow, g_aEnemy[nCntEnemy].pos);

			//探索円の位置更新
			SetPositionSearchCircle(g_aEnemy[nCntEnemy].nIdxSearchRadius, g_aEnemy[nCntEnemy].pos);

			//アイコンの位置更新
			UpdatePositionIcon(g_aEnemy[nCntEnemy].nIdxIcon, g_aEnemy[nCntEnemy].pos);

			//扇の形の位置更新
			SetPositionSearchFan(g_aEnemy[nCntEnemy].nIdxSearchFan, g_aEnemy[nCntEnemy].pos, g_aEnemy[nCntEnemy].rot);

			//セリフのセット
			EnemySerifSet(nCntEnemy);

			//セリフの位置更新
			SetPositionSerif(g_aEnemy[nCntEnemy].nIdxSerif, { g_aEnemy[nCntEnemy].pos.x, g_aEnemy[nCntEnemy].pos.y + 150.0f, g_aEnemy[nCntEnemy].pos.z });
		}
	}
}

//==================================================================================
//敵の情報取得
//==================================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];
}

//==================================================================================
//敵の情報取得
//==================================================================================
EnemyInfo *GetEnemyInfo(void)
{
	return &g_aEnemyInfo;
}
