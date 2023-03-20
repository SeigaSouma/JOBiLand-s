//==============================================================
//
//DirectX[main.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _MAIN_H_		//このマクロ定義がされていなかったら
#define _MAIN_H_		//2重インクルード防止のマクロを定義する

#include<windows.h>
#include<time.h>
#include"d3dx9.h"							//描画処理に必要
#define DIRECTINPUT_VERSION		(0x0800)	//ビルド時の警告対処用マクロ
#include"dinput.h"							//入力処理に必要
#include"xaudio2.h"							//サウンド処理に必要
#include<stdio.h>

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")		//描画処理に必要
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")	//DirectXコンポーネント(部品)使用に必要
#pragma comment(lib,"winmm.lib")	//システム時刻取得に必要
#pragma comment(lib,"dinput8.lib")	//入力処理に必要

//マクロ定義
#define SCREEN_WIDTH		(1280)															//ウインドウの幅
#define SCREEN_HEIGHT		(720)															//ウインドウの高さ
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//頂点フォーマット2D
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//頂点フォーマット3D
#define FVF_VERTEX_3D_MULTI	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)		//頂点フォーマット3D_MULTI
#define MOVE_LRUP			(0.75f)															//斜め上移動
#define MOVE_LR				(0.5f)															//水平移動
#define MOVE_LRDW			(0.25f)															//斜め上移動

#define MAX_MAT				(512)		//マテリアルサイズ
#define MAX_TX				(96)		//テクスチャサイズ
#define LIMIT_POS			(5000.0f)	//大人の箱庭
#define CHARA_MOVE			(1.7f)		//キャラクターの移動速度
#define MOVE_FACTOR			(0.25f)		//キャラクターの移動補正係数
#define MOVE_FACTOR_HYPNOSIS	(0.5f)		//キャラクターの移動補正係数

//画面(モード)の種類
typedef enum
{
	MODE_TITLE = 0,	//タイトル画面
	MODE_TUTORIAL,	//チュートリアル画面
	MODE_GAME,		//ゲーム画面
	MODE_RESULT,	//リザルト画面
	MODE_RANKING,	//ランキング画面
	MODE_LOGO,		//チームロゴ画面
	MODE_MAX
} MODE;

typedef enum
{
	DRAWTYPE_MAIN = 0,
	DRAWTYPE_MAP,
	DRAWTYPE_UI,
	DRAWTYPE_MAX
}DRAWTYPE;

//頂点情報[2D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	float rhw;				//座標変換用係数(1.0fで固定)
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
} VERTEX_2D;

//頂点情報[3D]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
} VERTEX_3D;

//頂点情報[3D_MULTI]の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;		//頂点座標
	D3DXVECTOR3 nor;		//法線ベクトル
	D3DCOLOR col;			//頂点カラー
	D3DXVECTOR2 tex;		//テクスチャ座標
	D3DXVECTOR2 texM;		//マルチテクスチャ座標
} VERTEX_3D_MULTI;

//モデルの構造体を定義
typedef struct
{

	D3DXVECTOR3 pos;		//現在の位置
	D3DXVECTOR3 posOld;		//前回の位置
	D3DXVECTOR3 posOrigin;	//元の位置
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotOrigin;	//元の向き
	D3DXVECTOR3 move;		//移動量
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	D3DXMATRIX OldmtxWorld;	//ワールドマトリックス

	int nIdxShadow;			//影のインデックス番号
	int nIdxRotate;			//ぐるぐるのインデックス番号
	int nIdxHypGauge;		//体力ゲージのインデックス番号
	int nIdxSerif;			//セリフのインデックス番号
	int nShadow;			//影を使うかどうか
	int nFever;				//影を使うかどうか

	int nNumVtx;			//頂点数
	DWORD dwSizeFVF;		//頂点フォーマットのサイズ
	BYTE *pVtxBuff;			//頂点バッファのポインタ
	D3DXVECTOR3 vtxMin;		//モデルの最小値
	D3DXVECTOR3	vtxMax;		//モデルの最大値

	int nType;			//読み込むモデルの種類
	int nParent;		//親の番号
	bool bUse;			//使用しているか
	int nState;			//状態
	

	LPD3DXMESH pMesh;			//メッシュ(頂点情報)へのポインタ
	LPD3DXBUFFER pBuffMat;		//マテリアルへのポインタ
	D3DXMATERIAL pMatData[MAX_MAT];	//マテリアルのデータ
	DWORD dwNumMat;			//マテリアルの数
	LPDIRECT3DTEXTURE9 pTexture[MAX_TX];	//テクスチャへのポインタ
	char acFilename[512];	//ファイル名
}Model;

//2DUI用の構造体を定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	D3DXCOLOR col;		//頂点カラー
	float fWidth;		//幅
	float fHeight;		//高さ
	int nPatternAnim;	//アニメーションパターンNo
	int nCntAnim;		//アニメーションカウンター
	D3DXVECTOR3 rot;	//向き
	float fAngle;		//角度
	float fLength;		//対角線の長さ
	int nCount;			//雑カウント
	bool bUse;			//使用しているか
	float aTexU;		//テクスチャ座標U[スクロール用]
	float aTexV;		//テクスチャ座標V[スクロール用]

} UI_2D;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
LPD3DXFONT GetFont(void);
int GetFPS(void);

#endif