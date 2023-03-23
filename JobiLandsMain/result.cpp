//==============================================================
//
//リザルト画面処理[result.cpp]
//Author:hinosawa
//
//==============================================================
#include"result.h"
#include"sound.h"
#include"input.h"
#include"resultscore.h"
#include"score.h"
#include"light.h"
#include"camera.h"
#include"shadow.h"
#include"model.h"
#include"edit.h"
#include"player.h"
#include"meshfield.h"
#include"meshcylinder.h"
#include"meshdome.h"
#include"fade.h"

//マクロ定義
#define NUM_RESULT		(3)		//背景の数
#define CHANGE_TIME		(60 * 30)
#define COL_R			(255)	//赤色の数値
#define COL_G			(255)	//緑色の数値
#define COL_B			(255)	//青色の数値
#define COL_A			(255)	//不透明度の数値
#define RESULT_TIME		(800)	//リザルト画面表示時間

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureResult[NUM_RESULT] = {};	//テクスチャ(3等分)へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		//頂点バッファへのポインタ
int g_nCounterResult;
int g_nCounterRS;

//==============================================================
//リザルト画面の初期化処理
//==============================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	int nCntResult;

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\p_one.png",
		&g_pTextureResult[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score.jpg",
		&g_pTextureResult[1]);

	g_nCounterResult = 0;		//カウンターの初期化
	g_nCounterRS = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(COL_R, COL_G, COL_B, COL_A);
		pVtx[1].col = D3DCOLOR_RGBA(COL_R, COL_G, COL_B, COL_A);
		pVtx[2].col = D3DCOLOR_RGBA(COL_R, COL_G, COL_B, COL_A);
		pVtx[3].col = D3DCOLOR_RGBA(COL_R, COL_G, COL_B, COL_A);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//頂点データのポインタを４つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResult->Unlock();

	//タイトルのカメラ初期化処理
	InitResultCamera();

	//影の初期化処理
	InitShadow();

	//モデルの初期化処理
	InitModel();

	//エディットの初期化
	InitEdit();

	//プレイヤーの初期化処理
	InitPlayer();

	//メッシュフィールドの初期化処理
	InitMeshField();

	//メッシュシリンダーの初期化処理
	InitMeshCylinder();

	//メッシュドームの初期化処理
	InitMeshDome();

	//スコアの初期化処理
	InitResultScore();

	Score *pSc = GetScore();

	SetResultScore(pSc->nScore);

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM_RESULT);

}

//==============================================================
//リザルト画面の終了処理
//==============================================================
void UninitResult(void)
{
	//サウンドの停止
	StopSound();

	//影の終了処理
	UninitShadow();

	//モデルの終了処理
	UninitModel();

	//エディットの終了処理
	UninitEdit();

	//プレイヤーの終了処理
	UninitPlayer();

	//メッシュフィールドの終了処理
	UninitMeshField();

	//メッシュシリンダーの終了処理
	UninitMeshCylinder();

	//メッシュドームの終了処理
	UninitMeshDome();

	//スコアの終了処理
	UninitResultScore();

	for (int nCntGame = 0; nCntGame < NUM_RESULT; nCntGame++)
	{
		//テクスチャの破棄
		if (g_pTextureResult[nCntGame] != NULL)
		{
			g_pTextureResult[nCntGame]->Release();
			g_pTextureResult[nCntGame] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//==============================================================
//リザルト画面の更新処理
//==============================================================
void UpdateResult(void)
{
	//自動遷移のカウント加算
	g_nCounterResult++;
	g_nCounterRS++;

	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0) || CHANGE_TIME <= g_nCounterResult)
	{//Enterが押された

		//モード設定(ゲーム画面に移行)
		SetFade(MODE_TITLE);				//フェードアウト
	}

	if (g_nCounterRS >= 60)
	{

		//スコアリザルトの更新処理
		UpdateResultScore();
	}
}

//==============================================================
//リザルト画面の描画処理
//==============================================================
void DrawResult(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{
		//影の描画処理
		DrawShadow();

		//モデルの描画処理
		DrawModel(0);

		//エディットの描画処理
		DrawEdit();

		//プレイヤーの描画処理
		DrawPlayer();

		//メッシュフィールドの描画処理
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//メッシュシリンダーの描画処理
		DrawMeshCylinder();

		//メッシュドームの描画処理
		DrawMeshDome();
	}

	if (nType == DRAWTYPE_UI)
	{
		//スコアの描画処理
		DrawResultScore();

		LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

		//頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

		//頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureResult[0]);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			0,												//プリミティブ（ポリゴンの数）
			2);												//描画するプリミティブ数
	}
}