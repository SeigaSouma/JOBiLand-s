//=============================================================================
//
// 抱き枕カバー処理 [cutepillow.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _CUTEPILLOW_H_		//このマクロ定義がされていなかったら
#define _CUTEPILLOW_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_CUTEPILLOW	(512)	//弾の最大数

//列挙型定義
typedef enum
{
	CUTEPILLOW_STATE_TROW = 0,	//投げ状態
	CUTEPILLOW_STATE_FALL,		//降下状態
}CUTEPILLOW_STATE;

//列挙型定義
typedef enum
{
	PILLOWPARENT_PLAYER = 0,		//プレイヤー
	PILLOWPARENT_GARI,			//がり
	PILLOWPARENT_CHIBI,			//チビ
	PILLOWPARENT_MAX
}PILLOWPARENT;

//抱き枕カバー構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 posOld;		//位置
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxShadow;			//影のインデックス番号
	int nNumIndex;			//インデックス数
	int nNumVertex;			//頂点数
	int nState;				//状態
	int nType;				//種類
	int nParent;			//親
	bool bUse;				//使用しているか
	float fWidth;			//横
	float fHeight;			//縦
}Cutepillow;

//プロトタイプ宣言
void InitCutepillow(void);
void UninitCutepillow(void);
void UpdateCutepillow(void);
void DrawCutepillow(void);
void SetCutepillow(D3DXVECTOR3 pos, D3DXVECTOR3 rot ,int nParent);
Cutepillow *GetCutepillow(void);

#endif