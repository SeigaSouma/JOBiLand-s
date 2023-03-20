//=============================================================================
//
// 敵処理 [buddy.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _BUDDY_H_		//このマクロ定義がされていなかったら
#define _BUDDY_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define JUMP		(20.0f)	//ジャンプ移動量
#define GRAVITY		(1.0f)	//重力
#define MAX_BUDDY	(2)		//相棒の最大数
#define BUDDY_DISTANCE	(60.0f)		//プレイヤーとの間隔
#define DIS_ROT		(D3DX_PI * 0.45f)

#include "main.h"
#include "model.h"
#include "crowd.h"
#include "mob.h"
#include "enemy.h"
#include "player.h"

//モデルの種類
typedef enum
{
	BUDDYTYPE_HYOROGARI = 0,
	BUDDYTYPE_DEPPA,
	BUDDYTYPE_MAX
}BUDDYTYPE;

typedef enum
{
	BUDDYMOTION_DEF = 0,	//ニュートラルモーション
	BUDDYMOTION_WALK,		//移動モーション
	BUDDYMOTION_ACTION,		//アクションモーション
	BUDDYMOTION_KYOROKYORO,	//キョロキョロモーション
	BUDDYMOTION_MOVEACTION,	//移動催眠モーション
	BUDDYMOTION_FEVERWALK,	//フィーバー中の移動モーション
	BUDDYMOTION_FEVERACTION,	  //フィーバー中のアクションモーション
	BUDDYMOTION_FEVERMOVEACTION,  //フィーバー中の移動アクションモーション
	BUDDYMOTION_RANKING,	//ランキングのモーション
	BUDDYMOTION_MAX
}BUDDYMOTION;

//プレイヤーの状態
typedef enum
{
	BUDDYSTATE_NONE = 0,	//何もしていない状態
	BUDDYSTATE_DISPATCH,	//派遣状態
	BUDDYSTATE_RETURN,		//帰還状態
	BUDDYSTATE_MAX
}BUDDYSTATE;

//移動AIの状態
typedef enum
{
	MOVEAI_NONE = 0,		//何もしていない状態
	MOVEAI_WIDTH,			//横移動
	MOVEAI_HEIGHT,			//縦移動
	MOVEAI_DIAGONAL_FORM,	//斜め移動(奥)
	MOVEAI_DIAGONAL_BACK,	//斜め移動(手前)
	MOVEAI_STOP,			//止まる(向き変更)
	MOVEAI_CHASE,			//追いかけ状態
	MOVEAI_ESCAPE,			//逃げ状態
	MOVEAI_MAX

}MOVEAISTATE;

//移動AIの構造体
typedef struct
{
	int nRotCounter;		//向き変更カウンター
	int nMoveCounter;		//移動変更カウンター
	int nRot;				//向きの+-
	int nMoveState;			//移動状態
	int nCntStop;			//止まっている状態を出やすくするカウンター
	int nChaseCounter;		//追いかけてる時間
	int nNotChaseCounter;	//追いかけない時間
	int nEscapeCounter;		//逃げる時間

	float fDistance;		//距離

	bool bPosDest;				//目的地についたか
	bool bMobDisc;				//モブを発見したかどうか
	bool bMobNum[MAX_MOB];		//何番目のモブを発見したか
	bool bChaseTime;			//追いかけるまでの時間を足すか
	bool bPenUse;				//ペンライトを使ってるか
	bool bPenRange[MAX_MOB];	//ペンライトの範囲に入っているか
	bool bEscape;				//逃げるか
	bool bEscapeNum[MAX_ENEMY];	//何番目の敵か

} MoveAI;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 formationPos;	//隊列の位置
	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	bool bUse;				//使用しているか
	bool bFever;			//フィーバー状態
	float fRotDiff;			//向きの差分
	float fRotDest;			//向きの差分
	int nIdxShadow;			//影のインデックス番号
	int nIdxIcon;			//ミニマップアイコンのインデックス番号
	int nIdxWave[MAX_WAVEINDEX];	//衝撃波のインデックス番号
	int nState;				//状態
	int nCntState;			//状態カウント
	int nCntChase;			//追い掛けカウント
	bool bCollisionModel;	//モデルとの判定を取るかどうか

	//モーション系
	int nNowMotionNum;		//現在のモーション番号
	int nCntFrame;			//フレームのカウント
	int nPatternKey;		//何個目のキーか
	int nPartsNum;			//パーツ数
	bool bMove;				//移動しているか
	bool bATK;				//攻撃してるか
	int nCntAllFrame;		//総フレームカウント

	int nCntWalk;			//移動のカウント
	int nCntPenlight;		//催眠のカウント
	float fRadius;			//半径

	float fThrowRadius;		//投げる範囲
	int nThrowCnt;			//投げるカウント

	Model aModel[MAX_MODEL];	//パーツ情報
	MotionData aMotionData[MAX_MOTION];		//モーション用の情報
	MoveAI aMoveAI;			//移動AI
	Formation aFormation[MAX_CROWD];	//隊列情報
}Buddy;

//プロトタイプ宣言
void InitBuddy(void);
void UninitBuddy(void);
void UpdateBuddy(void);
void UpdateTitleBuddy(void);
void UpdateResultBuddy(void);
void DrawBuddy(void);
Buddy *GetBuddy(void);
void SetBuddy(int nCntBuddy);
void SetBuddyDispatch(D3DXVECTOR3 pos, int nType);
void SetMotisonBuddy(int nCntBuddy, int nMotionType);
int SetFormationPosBuddy(int nCntBuddy);
void UpdateRankingBuddy(void);

#endif