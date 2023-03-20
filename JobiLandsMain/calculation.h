//=============================================================================
//
// メイン処理 [calculation.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _CALCULATION_H_	//このマクロ定義がされていなかったら
#define _CALCULATION_H_	//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_COMMENT	(512)	//コメントの最大数
#define MOVE_LOG	(64)	//ロゴの最大数

//UI拡縮のモード
typedef enum
{
	MOVELOG_TYPE_ADD = 0,	//拡大
	MOVELOG_TYPE_REMOVE,	//縮小
	MOVELOG_TYPE_SAME,		//揃える
	MOVELOG_TYPE_MAX
}MOVELOG_TYPE;

//ロゴ拡縮の構造体
typedef struct
{
	int nCnt;	//ロゴ拡縮のカウント
	int nPattern;	//ロゴ拡縮のパターン
	float Length;	//ロゴ拡縮の長さ
	bool bUse;		//使用しているか
}MOVELOG;

//プロトタイプ宣言
void InitCalculastion(void);
void RotNormalize(float *fRot);	//角度の正規化(-3.14 || 3.14超えたら補正)
float CircleRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);	//1と2の当たり判定(円)
float SphereRange(D3DXVECTOR3 pos1, D3DXVECTOR3 pos2);	//1と2の当たり判定(球)
void CheckVtx(float RotY, D3DXVECTOR3 *pVtxMax, D3DXVECTOR3 *pVtxMin, LPD3DXMESH pMesh, BYTE *pVtxBuff);	//全頂点チェック
void CollisionCharacter(D3DXVECTOR3 *MainPos, D3DXVECTOR3 *MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 *TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin);		//ぶつかったら補正
bool bHitCharacter(D3DXVECTOR3 *MainPos, D3DXVECTOR3 *MainPosOld, D3DXVECTOR3 MainVtxMax, D3DXVECTOR3 MainVtxMin, D3DXVECTOR3 *TargetPos, D3DXVECTOR3 TargetVtxMax, D3DXVECTOR3 TargetVtxMin);			//ぶつかったかチェック
void CollisionLimitLine(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);	//壁との当たり判定
int SetMoveLog(void);	//ロゴ拡縮する時のインデックス取得
float MoveLog(int nIdxMoveLog, float pos, int nCntElapsed);	//pVtx[?].posの長さになるやつ
void ResetMoveLog(int nIdxMoveLog);		//もう一回拡縮したいとき
bool CollisionFan(D3DXVECTOR3 posChar, D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 MainPos, D3DXVECTOR3 MainPosOld);
float LineOuterProduct(D3DXVECTOR3 posLeft, D3DXVECTOR3 posRight, D3DXVECTOR3 pos);					// 外積の左右判断処理

#endif