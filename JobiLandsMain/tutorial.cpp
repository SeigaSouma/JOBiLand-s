#include "title.h"
#include "main.h"
#include "game.h"
#include "sound.h"
#include "player.h"
#include "input.h"
#include "fade.h"
#include "tutorial.h"

//マクロ定義
#define NUM_TUTORIAL  (4)  //キーの最大数
#define MOVE_COL (0.01f) //色の移動量
//#define MOVE_ENT (0.1f)  //エンター押した後の移動

//チュートリアル構造体
typedef struct
{
	bool bUse;
	D3DXCOLOR col;
}TUTORIAL;

//グローバル変数宣言
LPDIRECT3DTEXTURE9 g_apTextureTutorial[NUM_TUTORIAL] = {};		//テクスチャへ(4枚分)のポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorial = NULL;  //頂点バッファへのポインタ
bool g_Kcheck = false; //キーが押されたか確認する
int g_Tutorial;		   //チュートリアルの画像をチェック
TUTORIAL g_aTutorial[NUM_TUTORIAL];
float g_Movcol;		//色の移動量
					//float g_Moventer;	//エンター押した後の移動量

					//======================
					//チュートリアル画面の初期化処理
					//======================
void InitTutorial(void)
{
	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorial3d1.png",
		&g_apTextureTutorial[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\tutorial3d2.png",
		&g_apTextureTutorial[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enter.png",
		&g_apTextureTutorial[2]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\back1.png",
		&g_apTextureTutorial[3]);

	//キーチェックを初期化
	g_Kcheck = false;

	//チュートリアルを初期化
	g_Tutorial = 0;

	//初期化(色の移動量)
	g_Movcol = MOVE_COL;
	/*g_Moventer = MOVE_ENT;*/

	for (nCnt = 0; nCnt < NUM_TUTORIAL; nCnt++)
	{
		//使用しているかどうか初期化
		g_aTutorial[nCnt].bUse = false;

		//色を初期化
		g_aTutorial[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TUTORIAL,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorial,
		NULL);

	VERTEX_2D *pVtx;    //頂点情報へのポインタ

						//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);

	for (nCnt = 0; nCnt < NUM_TUTORIAL; nCnt++)
	{
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1280.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 720.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1280.0f, 720.0f, 0.0f);

		if (nCnt == 2)
		{
			pVtx[0].pos = D3DXVECTOR3(1050.0f, 620.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(1280.0f, 620.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(1050.0f, 710.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(1280.0f, 710.0f, 0.0f);
		}

		if (nCnt == 3)
		{
			pVtx[0].pos = D3DXVECTOR3(100.0f, 620.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(300.0f, 620.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(100.0f, 710.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(300.0f, 710.0f, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;   //頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffTutorial->Unlock();

	//サウンドの再生
	PlaySound(SOUND_LABEL_BGM_TUTORIAL);
}

//============================
//チュートリアルの終了処理
//============================
void UninitTutorial(void)
{
	int nCnt;

	//サウンドの停止
	StopSound();

	for (nCnt = 0; nCnt < NUM_TUTORIAL; nCnt++)
	{
		if (g_apTextureTutorial[nCnt] != NULL)
		{
			g_apTextureTutorial[nCnt]->Release();
			g_apTextureTutorial[nCnt] = NULL;
		}

	}

	//頂点バッファの破棄
	if (g_pVtxBuffTutorial != NULL)
	{
		g_pVtxBuffTutorial->Release();
		g_pVtxBuffTutorial = NULL;
	}
}

//===============================
//チュートリアルの更新処理
//===============================
void UpdateTutorial(void)
{
	FADE pFade;
	pFade = GetFade();
	int nCnt = 0;

	if (g_Tutorial == 0)
	{
		g_aTutorial[0].bUse = true;

		g_aTutorial[2].bUse = true;
	}

	else
	{
		g_aTutorial[1].bUse = true;

		g_aTutorial[2].bUse = true;

		g_aTutorial[3].bUse = true;
	}

	//次のページへ移動する処理
	if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0) == true)
	{
		if (g_Tutorial == 0)
		{

			g_Tutorial = 1;

			g_aTutorial[1].bUse = true;

			g_aTutorial[3].bUse = true;

			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_ENTER);

			/*g_Movcol = g_Moventer;*/
		}

		//ゲーム画面へ移行する処理
		else if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0) == true)
		{//決定(ENTERキー)が押された
		 //モード設定(ゲーム画面に移行)

			if (g_Kcheck == false && g_Tutorial == 1)
			{
				g_Kcheck = true;

				//サウンドの再生
				PlaySound(SOUND_LABEL_SE_ENTER);

				/*g_Movcol = g_Moventer;*/

				SetFade(MODE_GAME);
			}
		}
	}

	//前のページへ戻る処理
	if (GetKeyboardTrigger(DIK_BACK) == true || GetGamepadTrigger(BUTTON_B, 0) == true)
	{
		if (g_Tutorial == 1)
		{
			g_Tutorial = 0;

			g_aTutorial[1].bUse = false;

			g_aTutorial[3].bUse = false;

			//サウンドの再生
			PlaySound(SOUND_LABEL_SE_ENTER);

			/*g_Movcol = g_Moventer;*/
		}
	}
}

//=================================
//チュートリアルの描画処理
//=================================
void DrawTutorial(void)
{
	int nCnt;

	LPDIRECT3DDEVICE9 pDevice;

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTutorial, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (nCnt = 0; nCnt < NUM_TUTORIAL; nCnt++)
	{
		if (g_aTutorial[nCnt].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureTutorial[nCnt]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2); //プリミティブの種類
		}
	}
}

////==================
////点滅処理(ENTER)
////==================
//void ColEntTutorial(int nCnt)
//{
//
//	VERTEX_2D *pVtx;    //頂点情報へのポインタ
//
//						//頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);
//
//	pVtx += 8;
//
//	if (g_aTutorial[nCnt].col.a < 0.0f || g_aTutorial[nCnt].col.a > 1.0f)
//	{
//		g_Movcol *= -1.0f;
//	}
//
//	g_aTutorial[nCnt].col.a -= g_Movcol;
//
//	//頂点カラーの設定
//	pVtx[0].col = g_aTutorial[nCnt].col;
//	pVtx[1].col = g_aTutorial[nCnt].col;
//	pVtx[2].col = g_aTutorial[nCnt].col;
//	pVtx[3].col = g_aTutorial[nCnt].col;
//
//	//頂点バッファをアンロックする
//	g_pVtxBuffTutorial->Unlock();
//
//}
//
////==================
////点滅処理(BACK)
////==================
//void ColBackTutorial(int nCnt)
//{
//	VERTEX_2D *pVtx;    //頂点情報へのポインタ
//
//						//頂点バッファをロックし、頂点情報へのポインタを取得
//	g_pVtxBuffTutorial->Lock(0, 0, (void**)&pVtx, 0);
//
//	pVtx += 12;
//
//	if (g_aTutorial[nCnt].col.a < 0.0f || g_aTutorial[nCnt].col.a > 1.0f)
//	{
//		g_Movcol *= -1.0f;
//	}
//
//	g_aTutorial[nCnt].col.a -= g_Movcol;
//
//	//頂点カラーの設定
//	pVtx[0].col = g_aTutorial[nCnt].col;
//	pVtx[1].col = g_aTutorial[nCnt].col;
//	pVtx[2].col = g_aTutorial[nCnt].col;
//	pVtx[3].col = g_aTutorial[nCnt].col;
//
//	//頂点バッファをアンロックする
//	g_pVtxBuffTutorial->Unlock();
//}