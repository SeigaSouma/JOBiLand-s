//=============================================================================
//
// セリフ処理 [serif.h]
// Author : 大原怜将
//
//=============================================================================
#ifndef _SERIF_H_		//このマクロ定義がされていなかったら
#define _SERIF_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_SERIF	(1024)	//セリフの最大数

//誰のセリフ？
typedef enum
{
	SERIFSTATE_DISPATCH = 0,    //派遣時のセリフ
	SERIFSTATE_RETURN,          //帰還時のセリフ
	SERIFSTATE_ENEMYNONE,		//敵の普通のセリフ
	SERIFSTATE_ENEMYDISC,		//敵発見時のセリフ
	SERIFSTATE_MOB_BOCCHI,		//ぼっちのセリフ
	SERIFSTATE_MOB_YAKUZA,		//やくざのセリフ
	SERIFSTATE_MOB_WORK,		//会社員のセリフ
	SERIFSTATE_MAX
} SERIFSTATE;

//敵（うろちょろ）
typedef enum
{
	ENEMYSERIFSTATE_SEEK = 0,	//ニートはどこだ
	ENEMYSERIFSTATE_ERADIC,		//ニート撲滅
	ENEMYSERIFSTATE_AKIBA,		//秋葉原守らないと
	ENEMYSERIFSTATE_ZINKEN,		//ニートに人権ない
	ENEMYSERIFSTATE_WORK,		//働こうと思わないの
	ENEMYSERIFSTATE_OBUTSU,		//汚物は消毒だー！
}ENEMYSERIFSTATE;

//敵（発見時）
typedef enum
{
	DISCSERIFSTATE_BIKKURI = 0,		//！
	DISCSERIFSTATE_NEET,			//ニート発見
	DISCSERIFSTATE_MADAMADA,		//まだまだだね
	DISCSERIFSTATE_TAIDA,			//怠惰デスねぇ
	DISCSERIFSTATE_HANZAI,			//バレなきゃ犯罪じゃない
}ENEMYDISCSERIFSTATE;

//ぼっち
typedef enum
{
	BOCCHISERIFSTATE_CLUB = 0,	//ぼっち（部活）
	BOCCHISERIFSTATE_REAL,		//ぼっち（現実は怖い）
	BOCCHISERIFSTATE_EEE,		//ぼっち（え、）
	BOCCHISERIFSTATE_SORRY,		//ぼっち（イキってすみません）
	BOCCHISERIFSTATE_WORK,		//ぼっち（働いたら負け）
}BOCCHISERIFSTATE;

//やくざ
typedef enum
{
	YAKUZASERIFSTATE_BOSS,			//やくざ（ボスと呼べ）
	YAKUZASERIFSTATE_RAPPER,		//やくざ（ヤクザかラッパー）
	YAKUZASERIFSTATE_KOYUBI,		//やくざ（小指ない）
	YAKUZASERIFSTATE_TOKYO,			//やくざ（東京湾）
	YAKUZASERIFSTATE_HEKE,			//やくざ（ヘケ）
}YAKUZASERIFSTATE;

//会社員
typedef enum
{
	WORKSERIFSTATE_BLACK,		//会社員（会社やめたい）
	WORKSERIFSTATE_STATION,		//会社員（電車遅延）
	WORKSERIFSTATE_ANIME,		//会社員（アニメ消化）
	WORKSERIFSTATE_KEEP,		//会社員（２０連勤）
}WORKSERIFSTATE;

typedef struct
{
	D3DXVECTOR3 pos;    //位置
	D3DXVECTOR3 rot;    //向き
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	float fWidth;	    //横幅
	float fHeight;		//縦幅
	int nType;          //種類
	int nLife;          //寿命
	int nBocchiSelect;		//ぼっちちゃんのセリフの選択
	int nYakuzaSelect;		//やくざのセリフの選択
	int nWorkSelect;		//会社員のセリフの選択

	int nEnemyNoneSelect;	//敵の普通のセリフの選択
	int nEnemyDiscSelect;	//敵の発見時のセリフの選択
	bool bUse;          //使用しているかどうか
	bool bDisp;         //描画するかどうか

} Serif;

//プロトタイプ宣言
void InitSerif(void);
void UninitSerif(void);
void UpdateSerif(void);
void DrawSerif(void);
int SetSerif(float fWidth, float fHeight, int nLife, int nType, D3DXVECTOR3 pos);
void SetPositionSerif(int nIdxGauge, D3DXVECTOR3 pos);
Serif *GetSerif(void);
//void GetSerifUse(int nIdxGauge, bool bUse);

#endif
