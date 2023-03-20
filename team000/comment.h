//=============================================================================
//
// コメント処理 [comment.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _COMMENT_H_		//このマクロ定義がされていなかったら
#define _COMMENT_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_NICOCOME	(256)	//ニココメのさいだいすう

//構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 move;	//移動量
	int nType;	//種類
	int nTexV;	//テクスチャの何分割目か
	int nNumIndex;	//インデックス番号
	bool bUse;	//使用しているか
}CommnetInfo;

typedef struct
{
	bool bUse;		//使用しているか
	int nCntSpawn;	//出現までのカウンター
	int nNumSpawn;	//出現した回数
	CommnetInfo aInfo[MAX_NICOCOME];
}Commnet;

//プロトタイプ宣言
void InitCommnet(void);
void UninitCommnet(void);
void UpdateCommnet(void);
void DrawCommnet(void);
void SetComment(void);
Commnet *GetCommnet(void);

#endif