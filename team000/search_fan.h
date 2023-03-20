//==============================================================
//
//敵の前に出す扇[search_fan.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _SEARCH_FAN_H_		//このマクロ定義がされていなかったら
#define _SEARCH_FAN_H_		//2重インクルード防止のマクロを定義する

#include"main.h"

//扇の構造体の定義
typedef struct
{
	D3DXVECTOR3 pos;				//位置
	D3DXVECTOR3 rot;				//向き
	D3DXMATRIX mtxWorld;			//ワールドマトリックス

	bool bUse;						//扇の形を使用しているか
	bool bDisp;						//扇の形を描画しているか
} SearchFan;

//プロトタイプ宣言
void InitSearchFan(void);
void UnInitSearchFan(void);
void UpdateSearchFan(void);
void DrawSearchFan(void);
void SetPositionSearchFan(int nIdxSearchFan, D3DXVECTOR3 pos, D3DXVECTOR3 rot);
int SetSearchFan(void);
SearchFan *GetSearchFan(void);

#endif
