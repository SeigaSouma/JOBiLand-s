//=============================================================================
//
// プレイヤー処理 [player.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _PLAYER_H_		//このマクロ定義がされていなかったら
#define _PLAYER_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define JUMP		(20.0f)	//ジャンプ移動量
#define GRAVITY		(1.0f)	//重力
#define PENLIGHT_RADIUS	(300.0f)
#define PENLIGHT_RADIUS_MOVE	(200.0f)
#define PENLIGHT_ANGLE	(D3DX_PI * 0.18f)
#define PENLIGHT_ANGLE_MOVE	(D3DX_PI * 0.1f)
#define MAX_WAVEINDEX	(16)

#include "main.h"
#include "model.h"
#include "motion.h"

//モデルの種類
typedef enum
{
	PLAYERPARTS_BODY = 0,	//体
	PLAYERPARTS_HEAD,		//頭
	PLAYERPARTS_R_NINO,		//右二の腕
	PLAYERPARTS_R_UDE,		//右腕
	PLAYERPARTS_R_HAND,		//右手
	PLAYERPARTS_R_MOMO,		//右もも
	PLAYERPARTS_R_LEG,		//右ふくらはぎ
	PLAYERPARTS_R_ASIKUBI,	//右足首
	PLAYERPARTS_R_FOOT,		//右足
	PLAYERPARTS_L_NINO,		//左二の腕
	PLAYERPARTS_L_UDE,		//左腕
	PLAYERPARTS_L_HAND,		//左手
	PLAYERPARTS_L_MOMO,		//左もも
	PLAYERPARTS_L_LEG,		//左ふくらはぎ
	PLAYERPARTS_L_ASIKUBI,	//左足首
	PLAYERPARTS_L_FOOT,		//左足
	PLAYERPARTS_HANDGUN,
	PLAYERPARTS_WOODSTICK,
	PLAYERPARTS_STONE,
	PLAYERPARTS_MAX
}PLAYERPARTS;

typedef enum
{
	PLAYERMOTION_DEF = 0,	//ニュートラルモーション
	PLAYERMOTION_UP,	//打ち上げモーション
	PLAYERMOTION_DOWN,	//内下げモーション
	PLAYERMOTION_MAX
}PLAYERMOTION;

//プレイヤーの状態
typedef enum
{
	PLAYERSTATE_NONE = 0,	//何もしていない状態
	PLAYERSTATE_DMG,		//ダメージ状態
	PLAYERSTATE_INVINCIBLE,	//透明状態
	PLAYERSTATE_MAX
}PLAYERSTATE;

//プレイヤーの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 vecMove;	//移動ベクトル
	D3DXVECTOR3 vecToPos;	//ベクトルの長さ
	float fRotDiff;			//向きの差分
	float fRotDest;			//向きの差分
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxShadow;			//影のインデックス番号
	int nIdxHPGauge;		//体力ゲージのインデックス番号
	int nState;				//状態
	int nCntState;			//状態カウント
	bool bUse;				//使用しているか
	bool bDisp;				//描画しているか
	float fRadius;			//半径

	//モーション系
	bool bMove;				//移動しているか
	bool bJump;				//ジャンプしているか
	bool bATK;				//攻撃してるか
	int nCntWalk;			//移動のカウント
	int nCntPenlight;		//催眠のカウント
	int nCntDis;            //派遣のカウント
	int nCntReturn;         //帰還のカウント

	Model aModel[MAX_MODEL];	//パーツ情報
	MotionData aMotionData[MAX_MOTION];		//各モーションの情報
	Motion aMotion;			//モーションの全体情報
}Player;

//プロトタイプ宣言
void InitPlayer(void);
void UninitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);
Player *GetPlayer(void);
void SetPlayer(void);
D3DXMATRIX GetParentMatrix(int nIdxParts);
void HitPlayer(void);

#endif