//=============================================================================
//
// 敵処理 [enemy.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _ENEMY_H_		//このマクロ定義がされていなかったら
#define _ENEMY_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_ENEMY	(32)	//モデルの最大数
#define TARGETNUM_ENEMY	(60)	//倒す目標数
#define MAX_POSCRAWL	(32)	//巡回ポイントの最大数
#define SEARCH_RADIUS	(120.0f)

#include "main.h"
#include "model.h"

typedef enum
{
	ENEMYSTATE_NONE = 0,	//何もしていない状態
	ENEMYSTATE_SEARCH,		//探索状態
	ENEMYSTATE_CHASE,		//追い状態
	ENEMYSTATE_WAIT,        //出現待ち状態
	ENEMYSTATE_LOST,        //消滅待ち状態
	ENEMYSTATE_MAX
}ENEMYSTATE;

typedef enum
{
	ENEMYMOTION_DEF = 0,	//ニュートラルモーション
	ENEMYMOTION_DASH,		//ダッシュ
	ENEMYMOTION_SHOT,       //射撃
	ENEMYMOTION_WAIT,       //出現待ち
	ENEMYMOTION_LOST,       //消滅待ち
	ENEMYMOTION_MAX
}ENEMYMOTION;

typedef enum
{
	ENEMYTARGET_PLAYER = 0,
	ENEMYTARGET_SHINBOKU,
	ENEMYTARGET_MAX
}ENEMYTARGET;

//敵の構造体
typedef struct
{
	int nNumEnemy;			//敵の数
}EnemyInfo;

typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posOrigin;	//現在の位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているか
	bool bMoreSpawn;        //警備強化しているかどうか
	int nState;				//状態
	int nIdxShadow;			//影のインデックス番号
	int nIdxSearchRadius;	//探索範囲円のインデックス番号
	int nIdxIcon;			//ミニマップアイコンのインデックス番号
	int nIdxSearchFan;		//扇のインデックス番号
	int nIdxSerif;			//セリフのインデックス番号
	int nCntState;			//状態カウンター
	int nCntAction;			//行動カウンター
	int nATKInterval;		//攻撃の間隔
	int nCntATK;			//攻撃の間隔カウント
	int nCntSearchCircle;	//探索範囲円のカウント
	float fChaseRadius;		//追い掛け範囲
	float fSearchRadius;	//探索範囲
	float fSearchLength;	//探索の長さ
	float fSearchRot;		//探索の向き
	float fRadius;			//探索の向き
	D3DXVECTOR3 PosCrawl[MAX_POSCRAWL];	//巡回ポイント
	int nNumPosCrawl;		//巡回する地点の番号
	int nNumMaxCrawl;		//巡回する地点の最大数
	int MassGeneration;     //大量発生時に出る敵
	int nCntWait;           //出現待ちのカウント

	//モーション系
	int nNowMotionNum;		//現在のモーション番号
	int nCntFrame;			//フレームのカウント
	int nPatternKey;		//何個目のキーか
	int nPartsNum;			//パーツ数
	int nCntAllFrame;		//総フレームカウント

	Model aModel[MAX_MODEL];	//モデルの構造体
	MotionData aMotionData[MAX_MOTION];		//モーション用の情報
}Enemy;

typedef struct
{
	int MoreSecurityCounter;    //警備強化するまでの時間
	int SpawnCounter;           //警備強化終了までの時間
	int ManySpawnCounter;		//大量発生時の警備強化終了までの時間
	bool bMoreSpawn;            //警備強化中かどうか
	bool bManyMoreSpawn;		//大量発生時に警備強化中かどうか
	bool bFirst;				//1番目のスポーン
	bool bSecond;				//2番目のスポーン
	bool bThird;				//3番目のスポーン
	bool bManyFirst;			//1番目の大量発生時のスポーン
	bool bManySecond;			//2番目の大量発生時のスポーン
	bool bManyThird;			//3番目の大量発生時のスポーン

} MoreEnemy;

//プロトタイプ宣言
void InitEnemy(void);
void UninitEnemy(void);
void UpdateEnemy(void);
void UpdateTutorialEnemy(void);
void DrawEnemy(void);
Enemy *GetEnemy(void);
EnemyInfo *GetEnemyInfo(void);
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
void HitEnemy(int nCntEnemy, D3DXVECTOR3 move, int nDMG);
void SetMotionEnemy(int nCntEnemy, int nMotionType);
void ReadSetEnemy(void);

#endif