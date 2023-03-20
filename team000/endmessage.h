//==============================================================
//
//終わる前に出すメッセージ[endmessage.h]
//Author:大原怜将
//
//==============================================================
#ifndef _ENDMESSAGE_H_				//このマクロ定義がされていなかったら
#define _ENDMESSAGE_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//人
typedef enum
{
	MESSAGETYPE_NONE = 0,	//普通に出るやつ
	MESSAGETYPE_BIG,		//後ろに出るやつ
	MESSAGETYPE_MAX
}MESSAGETYPE;

//ミニマップフレームの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXCOLOR col;          //色
	MESSAGETYPE ntype;        //種類
	int nCounter;           //文字が消えるまでのカウント
	int nCntZoom;           //文字が大きくなるカウント
	float fWidth;			//横幅
	float fHeight;			//縦幅
	bool bUse;              //使用しているかどうか
}Endmessage;


//プロトタイプ宣言
void InitEndmessage(void);
void UninitEndmessage(void);
void UpdateEndmessage(void);
void DrawEndmessage(void);
Endmessage *GetEndmessage(void);


void SetEndmessage(D3DXVECTOR3 pos,int nCounter);


#endif