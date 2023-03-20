//==============================================================
//
//遷移しないフェード[nofade.cpp]
//Author:大原怜将
//
//==============================================================
#include "nofade.h"
#include "game.h"

//マクロ定義
#define DEF_POSX_UP			(0.0f)		//pos.xの上の初期値
#define DEF_POSX_DOWN		(1280.0f)	//pos.xの下の初期値
#define DEF_POSY_UP			(0.0f)		//pos.yの上の初期値
#define DEF_POSY_DOWN		(720.0f)	//pos.yの下の初期値
#define DEF_POS_Z			(0.0f)		//pos.zの初期値
#define COL_R				(0.0f)		//赤色の数値
#define COL_G				(0.0f)		//緑色の数値
#define COL_B				(0.0f)		//青色の数値
#define COL_A				(1.0f)		//不透明度の数値
#define TEX_LEFT_X			(0.0f)		//テクスチャの左xの初期値
#define TEX_RIGHT_X			(1.0f)		//テクスチャの右xの初期値
#define TEX_UP_Y			(0.0f)		//テクスチャの上yの初期値
#define TEX_DOWN_Y			(1.0f)		//テクスチャの下yの初期値

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNoFade = NULL;		//頂点バッファへのポインタ
NOFADE g_Nofade;										//フェードの状態

int g_uuu;

//==============================================================
//フェードの初期化処理
//==============================================================
void InitNoFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	g_Nofade.nState = NOFADE_OUT;	//フェードイン状態に
	g_Nofade.bUse = false;          //使用していない状態にする
	g_Nofade.colorNoFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		//黒いポリゴン（不透明）にしておく
	g_Nofade.nCounter = 0;
	g_uuu = 0;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNoFade,
		NULL);

	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNoFade->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(DEF_POSX_UP, DEF_POSY_UP, DEF_POS_Z);
	pVtx[1].pos = D3DXVECTOR3(DEF_POSX_DOWN, DEF_POSY_UP, DEF_POS_Z);
	pVtx[2].pos = D3DXVECTOR3(DEF_POSX_UP, DEF_POSY_DOWN, DEF_POS_Z);
	pVtx[3].pos = D3DXVECTOR3(DEF_POSX_DOWN, DEF_POSY_DOWN, DEF_POS_Z);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(COL_R, COL_G, COL_B, COL_A);
	pVtx[1].col = D3DXCOLOR(COL_R, COL_G, COL_B, COL_A);
	pVtx[2].col = D3DXCOLOR(COL_R, COL_G, COL_B, COL_A);
	pVtx[3].col = D3DXCOLOR(COL_R, COL_G, COL_B, COL_A);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(TEX_LEFT_X, TEX_UP_Y);
	pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT_X, TEX_UP_Y);
	pVtx[2].tex = D3DXVECTOR2(TEX_LEFT_X, TEX_DOWN_Y);
	pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT_X, TEX_DOWN_Y);

	//頂点バッファをアンロックする
	g_pVtxBuffNoFade->Unlock();
}

//==============================================================
//フェードの終了処理
//==============================================================
void UninitNoFade(void)
{
	//頂点バッファの破棄
	if (g_pVtxBuffNoFade != NULL)
	{
		g_pVtxBuffNoFade->Release();
		g_pVtxBuffNoFade = NULL;
	}
}

//==============================================================
//フェードの更新処理
//==============================================================
void UpdateNoFade(void)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffNoFade->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Nofade.bUse == true)
	{
		g_Nofade.nCounter--;

		if (g_Nofade.nCounter <= 0)
		{
			if (g_Nofade.nState == NOFADE_IN)
			{//フェードイン状態
				g_Nofade.colorNoFade.a -= 0.025f;		//ポリゴンを透明にしていく

				if (g_Nofade.colorNoFade.a <= 0.0f)
				{
					g_Nofade.colorNoFade.a = 0.0f;
					g_Nofade.nCounter = 0;
					g_Nofade.bUse = false;              //使用していない状態にする
				}
			}
			else if (g_Nofade.nState == NOFADE_OUT)
			{//フェードアウト状態

				g_Nofade.colorNoFade.a += 0.025f;		//ポリゴンを不透明にしていく

				if (g_Nofade.colorNoFade.a >= 1.0f)
				{
					g_Nofade.colorNoFade.a = 1.0f;
					g_Nofade.nState = NOFADE_IN;		//フェードイン状態に
					//SetGameState(GAMESTATE_BEFOREEND, 0);
				}
			}
		}
		


		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_Nofade.colorNoFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_Nofade.colorNoFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_Nofade.colorNoFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_Nofade.colorNoFade.a);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffNoFade->Unlock();
}

//==============================================================
//フェードの描画処理
//==============================================================
void DrawNoFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffNoFade, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Nofade.bUse == true)
	{//使用されていたら

		//テクスチャの設定
		pDevice->SetTexture(0, NULL);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			0,												//プリミティブ（ポリゴンの数）
			2);												//描画するプリミティブ数
	}
}

//==============================================================
//フェードの設定処理
//==============================================================
void SetNoFade(int nCounter)
{
	if (g_Nofade.bUse == false)
	{//使用されていなかったら

		g_Nofade.nState = NOFADE_OUT;							//フェードアウト状態
		g_Nofade.bUse = true;                                   //使用している状態にする
		g_Nofade.colorNoFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//黒いポリゴン（透明）にしておく
		g_Nofade.nCounter = nCounter;
	}
}

//==============================================================
//フェードの取得処理
//==============================================================
NOFADE GetNoFade(void)
{
	return g_Nofade;
}