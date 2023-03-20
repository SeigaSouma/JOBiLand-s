//=============================================================================
//
// モブ処理 [mob.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MOB_H_		//このマクロ定義がされていなかったら
#define _MOB_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_MOB			(1024)		//モデルの最大数
#define HYP_CNTTIME		(180)		//催眠度が減少するまでの時間
#define MAX_SPAWN		(64)		//スポーン地点の最大数
#define STOP_HYPNOSIS	(15)	//止まってる時の催眠度
#define MOVE_HYPNOSIS	(8)	//動いてる時の催眠度
#define FIRST_SPAWNMOB	(60 * 30)	//1番目の大量発生
#define SECOND_SPAWNMOB	(60 * 140)	//2番目の大量発生
#define THIRD_SPAWNMOB	(60 * 200)	//3番目の大量発生
#define STOP_FEVER_HYPNOSIS	(20)	//フィーバー時の催眠度
#define MOVE_FEVER_HYPNOSIS	(15)	//フィーバー時の催眠度

#include "main.h"
#include "model.h"
#include "serif.h"

typedef enum
{
	MOBSTATE_APPIEAR = 0,	//何もしていない状態
	MOBSTATE_DMG,		//ダメージ状態
	MOBSTATE_KEEP,		//催眠にかけられている状態
	MOBSTATE_HYPNOSIS,	//催眠状態
	MOBSTATE_WEAR,		//抱き枕カバー被ってる状態
	MOBSTATE_NONE,
}MOBSTATE;

typedef enum
{
	MOBACTION_NONE = 0,	//何もしていない状態
	MOBACTION_SEARCH,		//探索状態
	MOBACTION_CHASE,		//追い状態
	MOBACTION_WAIT,		//待ち状態
	MOBACTION_MAX
}MOBACTION;

typedef enum
{
	MOBTARGET_PLAYER = 0,
	MOBTARGET_SHINBOKU,
	MOBTARGET_MAX
}MOBTARGET;

//SE使用状況
typedef struct
{
	bool bUseRotate;		//SE使用してるか
} SwitchSE;


//スポーン地点の構造体
typedef struct
{
	bool bUseSpawn;
} SwitchSpawn;

//スポーン地点の構造体
typedef struct
{
	D3DXVECTOR3 pos;		//スポーン地点
	D3DXVECTOR3 rot;		//スポーン地点の向き
	int nType;				//スポーンする地点の種類
	int nMany;				//大量発生するかの判定
	bool bUse;              //その建物が大量発生地点か判定
} Spawn;

//大量発生の構造体
typedef struct
{
	int nSpawnCounter;	//大量発生するまでの時間
	int nStopTime;		//大量発生終了までの時間
	int nBillNumber;	//建物の場所
	int nCounter;

	bool bFirst;		//1番目のスポーン
	bool bSecond;		//2番目のスポーン
	bool bThird;		//3番目のスポーン

	Spawn aSpawn[MAX_SPAWN];	//スポーン情報
} ManySpawn;

//通常発生の構造体（ウェーブ）
typedef struct
{
	int nStartSpawn;		//発生開始時間
	int nStopSpawn;			//発生終了時間
	int nSpawnCounter;		//発生する間隔
	int nPosChangeCounter;	//発生する場所変えるまでの時間
	int nBillNumber;		//建物の場所

} WaveSpawn;

//群衆の構造体
typedef struct
{
	int nHypnosis;			//催眠度
	int nSubHypCounter;		//催眠度減少するまでのカウンター
	int nSubHypInter;		//催眠度を減らす間隔
	int nAddHypInter;		//催眠度を増やす間隔
	int nSwitchCounter;		//rotの切り替え時間
	int nTurnSwitchX;		//rot.xの+-切り替え
	int nTurnSwitchZ;		//rot.zの+-切り替え
	int nSpawnCounter;		//モブのスポーンする時間
	int nSpawnNumber;		//モブが一度にスポーンする数
	int nSpawnType;			//スポーン地点の種類
	int nMoveCounter;		//モブの移動のカウンター
	int nIdxIcon;			//ミニマップアイコンのインデックス番号
	int nTexType;			//テクスチャタイプ

	float fAlpha;				//透明度
	float fRandAlpha;			//透明度の増加数

	Model aModel;					//モデルの構造体
}Mob;

//プロトタイプ宣言
void InitMob(void);
void UninitMob(void);
void UpdateMob(void);
void UpdateTutorialMob(void);
void DrawMob(void);
Mob *GetMob(void);
ManySpawn *GetManySpawn(void);
SwitchSpawn *GetSwitchSpawn(void);
SwitchSE *GetSwitchSE(void);
void SetMob(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nSpawnType, int nModelType);
void HitMob(int nCntMob, int nValue);

#endif