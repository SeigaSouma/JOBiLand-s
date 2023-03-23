//==============================================================
//
//DirectX[tutorial.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _TUTORIAL_H_				//このマクロ定義がされていなかったら
#define _TUTORIAL_H_				//2重インクルード防止のマクロを定義する

#include"main.h"

//タイトルの状態
typedef enum
{
	TUTORIALSTATE_APPEAR = 0,			//出現状態(点滅)
	TUTORIALSTATE_NORMAL,				//通常状態
	TUTORIALSTATE_WAIT,				//出現待ち状態(表示なし)
	TUTORIALSTATE_MAX
} TUTORIALSTATE;

typedef enum
{
	TUTORIAL_STEP_START = 0,
	TUTORIAL_STEP_MOVE,
	TUTORIAL_STEP_DISPATCH,
	TUTORIAL_STEP_RETURN,
	TUTORIAL_STEP_HYPNOSIS,
	TUTORIAL_STEP_SENNOU,
	TUTORIAL_STEP_USEFEVER,
	TUTORIAL_STEP_HYPNOSISFEVER,
	TUTORIAL_STEP_MORS,
	TUTORIAL_STEP_END,
	TUTORIAL_STEP_MAX
}TUTORIAL_STEP;

//チュートリアル構造体の定義
typedef struct
{
	bool bUPMove;		//上移動
	bool bLMove;		//左移動
	bool bRMove;		//右移動
	bool bDWMove;		//下移動
	bool bSetMob;		//モブ配置したか
	bool bDispatchR;	//右派遣
	bool bDispatchL;	//左派遣
	bool bReturnL;		//左帰還
	bool bReturnR;		//右帰還
	bool bHypnosis;		//催眠度MAX
	bool bCutePillow;	//抱き枕カバー被せた
	bool bFeverUse;		//フィーバー状態にしたか
	int nCntHypnosisFever;	//フィーバー時にモブ捕まえた数
	bool bSetMobFever;		//フィーバー時にモブ配置したか

	bool bSetEnemy;		//敵配置したか
	bool bReleaseCrowd;	//洗脳解除

}TutorialControl;

typedef struct
{
	int nStep;				//ステップ
	TutorialControl aControl;	//チュートリアルの操作状況
} Tutorial;

//プロトタイプ宣言
void InitTutorial(void);
void UninitTutorial(void);
void UpdateTutorial(void);
void DrawTutorial(void);

#endif
