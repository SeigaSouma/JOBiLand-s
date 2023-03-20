//=============================================================================
//
// 群衆処理 [crowd.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _CROWD_H_		//このマクロ定義がされていなかったら
#define _CROWD_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_CROWD	(1600)	//モデルの最大数
#define FORMATION_DISTANCE	(60.0f)	//間隔

#include "main.h"
#include "model.h"

typedef enum
{
	CROWDSTATE_NONE = 0,	//何もしていない状態
	CROWDSTATE_JOIN,		//参加状態
	CROWDSTATE_CHASE,		//追い掛け状態
	CROWDSTATE_FADE,		//消えていく状態
	CROWDSTATE_WAIT,        //射撃待ち状態
	CROWDSTATE_RETURN,		//帰る状態
	CROWDSTATE_MAX
}CROWDSTATE;

typedef enum
{
	CROWDTARGET_PLAYER = 0,
	CROWDTARGET_SHINBOKU,
	CROWDTARGET_MAX
}CROWDTARGET;

typedef enum
{
	PARENT_PLAYER = 0,		//プレイヤー
	PARENT_GARI,			//ヒョロガリ陰キャ
	PARENT_CHIBI,			//クソ眼鏡チビ
	PARENT_MAX
}PARENT;

//群衆の構造体
typedef struct
{
	int nCrowdValue;        //敵の総数
	int nCrowdCnter;		//総数比較
	int nCrowdKeep;			//保存用
	int nParentPlayerNum;	//親がプレイヤーの数
	int nCroedTutorial;		//チュートリアル保存用
}CrowdInfo;

//群衆の数
typedef struct
{
	int nNum;			//群衆の数
	int nNumOld;		//過去の群衆の数

}CrowdNum;

typedef struct
{
	D3DXVECTOR3 pos;	//隊列の位置
	int nIndex;			//隊列のインデックス番号
	bool bUse;			//隊列を使用しているか
}Formation;

typedef struct
{
	int nState;				//状態
	int nCntChase;			//追い掛けるカウント
	float fFadeAlpha;		//フェードの不透明度
	D3DXVECTOR3 formationPos;	//隊列の位置
	D3DXVECTOR3 ParentPos;	//親の位置
	D3DXVECTOR3 ParentRot;	//親の向き
	float fThrowRadius;		//投げる範囲
	int nThrowCnt;			//投げるカウント
	int nType;				//種類
	int nParent;			//親
	int nTexType;			//テクスチャタイプ

	bool bFever;			//フィーバー状態か

	Model aModel;			//モデルの構造体
	Formation aFormation;	//隊列の情報
}Crowd;

//プロトタイプ宣言
void InitCrowd(void);
void UninitCrowd(void);
void UpdateCrowd(void);
void DrawCrowd(void);
Crowd *GetCrowd(void);
CrowdInfo *GetCrowdInfo(void);
CrowdNum *GetCrowdNum(void);
void SetCrowd(D3DXVECTOR3 pos, int nType, int nTexType, int nParent);
void SetCrowdResult(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexType, int nParent);
int SetFormationPos(int nParent);
void UpdateResultCrowd(void);

#endif