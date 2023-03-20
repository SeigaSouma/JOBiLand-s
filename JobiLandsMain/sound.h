//=============================================================================
//
// サウンド処理 [sound.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//*****************************************************************************
// サウンド一覧
//*****************************************************************************
typedef enum
{
	SOUND_LABEL_BGM_TITLE = 0,	// タイトル
	SOUND_LABEL_BGM_GAME,		// ゲームBGM
	SOUND_LABEL_BGM_FEVER,   // フィーバー突入時
	SOUND_LABEL_SE_PIYOPIYO,	// 頭にひよこ
	SOUND_LABEL_SE_GET,	 // 見つかった！！
	SOUND_LABEL_SE_HYPNOSISEND,	// 催眠完了
	SOUND_LABEL_SE_ASIOTO,	// 足音
	SOUND_LABEL_SE_KEIBIKYOUKA,	// 警備強化
	SOUND_LABEL_SE_DAKIMAKURA,  //抱き枕発射
	SOUND_LABEL_SE_AMURO,	// アムロ行きま～す
	SOUND_LABEL_SE_DHUHUHUHU,	// デュフフフ
	SOUND_LABEL_SE_ONMINO,	// 御身の前に
	SOUND_LABEL_SE_ORANGEKUN,	// 戻ってまいりました
	SOUND_LABEL_SE_SYUTUGEKI,	// 出撃でござる
	SOUND_LABEL_SE_YATTYAE,	// やっちゃえ
	SOUND_LABEL_SE_CLICK,   //これは使わないよ！！
	SOUND_LABEL_SE_WHEEL,   //これは使わないよ！！
	SOUND_LABEL_BGM_HYPNOSIS,   //催眠中
	SOUND_LABEL_SE_EVENTTEXT,   //イベントテキスト
	SOUND_LABEL_SE_WATERGUN,   //これは使わないよ！！
	SOUND_LABEL_SE_SHIBUKI,   // しぶき
	SOUND_LABEL_SE_KYUWAWA,   // きゅわわ～ン
	SOUND_LABEL_SE_DOGAN,   // ドガーン
	SOUND_LABEL_SE_GEKIATSU,   // 激アツキター！
	SOUND_LABEL_SE_UC,   // ユニコーーーーン！！！
	SOUND_LABEL_SE_MODORU,   // もどるお
	SOUND_LABEL_SE_SECURITY,   // 警備強化きたお
	SOUND_LABEL_SE_THEWORLD,   // ザ・ワールド
	SOUND_LABEL_SE_MANYPOS,   // 大量発生地点
	SOUND_LABEL_SE_DHUFU,	// デュフ単発
	SOUND_LABEL_SE_CUTIN,	// カットイン
	SOUND_LABEL_SE_BACHIBACHI,	// 警備強化バチバチ
	SOUND_LABEL_BGM_TUTORIAL,	// チュートリアルのBGM
	SOUND_LABEL_BGM_RESULT,	// リザルトのBGM
	SOUND_LABEL_BGM_RANKING,	// ランキングのBGM
	SOUND_LABEL_SE_IDOU,	// ランキング、リザルトとかの数字が移動するときの音
	SOUND_LABEL_SE_KOKE,	// ｺｹｺｯｺｰ
	SOUND_LABEL_SE_TUTORIALWINDOW,	// チュートリアルウィンドウ設置
	SOUND_LABEL_SE_ENDCOUNTDOWN,	// カウントダウン
	SOUND_LABEL_SE_FINISH,	// 終了
	SOUND_LABEL_SE_TUTORIALCLEAR,	//  チュートリアルのステップクリア
	SOUND_LABEL_SE_GAMECLEAR,	//  ゲームクリア
	SOUND_LABEL_SE_GAMEFAILED,	//  ゲームクリアじゃない
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);
void VolumeChange(float fVolume);
int GetVolume(void);

#endif
