//=============================================================================
//
// モデル処理 [model.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _MODEL_H_		//このマクロ定義がされていなかったら
#define _MODEL_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define MAX_MODEL	(312)	//モデルの最大数

#include "main.h"

//列挙型定義
//なんのモデルか
typedef enum
{
	MODELTYPE_TREE_NORMAL = 0,
	MODELTYPE_TREE_SMALL,
	MODELTYPE_TREE_BIG,
	MODELTYPE_TREE_BW_NORMAL,
	MODELTYPE_TREE_BW_SMALL,
	MODELTYPE_TREE_BW_BIG,
	MODELTYPE_TREE_APLLE_NORMAL,
	MODELTYPE_TREE_APLLE_SMALL,
	MODELTYPE_TREE_APLLE_BIG,
	MODELTYPE_ENEMYCOR,
	MODELTYPE_STONE000,
	MODELTYPE_STONE001,
	MODELTYPE_STONE002,
	MODELTYPE_STONE003,
	MODELTYPE_TREE_BIIIIIG,
	MODELTYPE_CAR,
	MODELTYPE_SHIELD,
	MODELTYPE_WAGON,
	MODELTYPE_WOODENEMY,
	MODELTYPE_SHINBOKU,
	MODELTYPE_MARK,
	MODELTYPE_MAX

}MODELTYPE;

typedef struct
{
	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 posOrigin;	//元の位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotOrigin;	//元の向き
	D3DXVECTOR3 move;		//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nIdxShadow;			//影のインデックス番号
	int nShadow;	//影を使うかどうか

	int nNumVtx;			//頂点数
	D3DXVECTOR3 vtxMin;		//モデルの最小値
	D3DXVECTOR3	vtxMax;		//モデルの最大値

	int nState;			//状態
	int nCntState;		//状態カウンター
	int nType;			//読み込むモデルの種類
	int nParent;		//親の番号
	int nWeapon;		//武器かどうか
	bool bUse;			//使用しているか
	bool bDisp;
	
	bool bSkelton;	//透明になるかどうか

	Model aXLoadMeshData;	//Xファイルで読み込んだデータ
}ModelModel;

//プロトタイプ宣言
void InitModel(void);
void UninitModel(void);
void UpdateModel(void);
void DrawModel(int nType);
ModelModel *GetModel(void);
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nShadow);
int GetModelFileNum(void);
Model *GetXLoadData(void);
int GetModelNum(void);

#endif