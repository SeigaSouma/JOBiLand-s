//==========================================
//
//カットイン処理
//Author:日野澤匠泉 [cutin.h]
//
//==========================================

#ifndef _CUTIN_H_	//この定義がされていなかったら
#define _CUTIN_H_	//二重インクルード防止のマクロ定義をする

//カットイン構造体
typedef struct
{
	D3DXVECTOR3 move;	//移動量
	D3DXVECTOR3 pos;	//位置
	D3DXVECTOR3 rot;	//向き

	D3DXCOLOR col;		//色

	float fHeight;		//高さ
	float fWidth;		//幅

	int nType;		//種類

	bool bUse;		//使用されているかどうか

}CUTIN;

//マクロ定義
#define MAX_TEX	(2)			//テクスチャ最大数
#define IMAGE	(0)			//画像
#define SENTENCE	(1)		//文字

#define TEXT_X_CUTIN	(640.0f)	//幅		
#define TEXT_Y_CUTIN	(350.0f)	//高さ

#define TEXT_X_TEXT		(300.0f)	//幅
#define TEXT_Y_TEXT		(300.0f)	//高さ


//プロトタイプ宣言
void InitCutin(void);
void UninitCutin(void);
void UpdateCutin(void);
void DrawCutin(void);
void SetCutin(D3DXVECTOR3 pos, int nType, float fHeight, float fWidth);
CUTIN *GetCutin(void);

#endif