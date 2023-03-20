//=============================================================================
//
// 最初の会話処理 [ranking_comment.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _RANKING_COMMENT_H_		//このマクロ定義がされていなかったら
#define _RANKING_COMMENT_H_		//二重インクルード防止のマクロを定義する

#include "main.h"
#include "comment.h"


//構造体の定義
typedef struct
{
	int nNumSpawn;	//出現した回数
	CommnetInfo aInfo[MAX_NICOCOME];
}RankingComment;

//プロトタイプ宣言
void InitRankingComment(void);
void UninitRankingComment(void);
void UpdateRankingComment(void);
void DrawRankingComment(void);
void SetRankingComment(int nTexV, int nType);
void SetTartgetWindow(void);
RankingComment *GetRankingComment(void);

#endif