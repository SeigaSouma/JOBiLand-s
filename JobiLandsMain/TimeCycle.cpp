//<====================================================
//
//タイムサイクル処理(TimeCycle.cpp)
//Author takumi hinosawa
//
//<====================================================
#include "TimeCycle.h"
#include "Input.h"
#include "game.h"
#include "nofade.h"

//マクロ定義
#define SIZE (175.0f)		//画像サイズ

//グローバル変数宣言
LPDIRECT3DTEXTURE9	g_pTextureTimeCycle = NULL;			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffTimeCycle = NULL;		//頂点のバッファ
TIMECYCLE g_TimeCycle;

//=====================================
//タイムサイクルの初期化処理
//=====================================					   
void InitTimeCycle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//デバイスの取得

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Time_C001.png",
		&g_pTextureTimeCycle);

	//各種変数初期化
	g_TimeCycle.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SIZE * 0.5f, 0.0f);		//位置の初期化
	g_TimeCycle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//向きを初期化する(Z値を使用する)
													
	g_TimeCycle.fAngle = atan2f(SIZE, SIZE);						//対角線の角度を算出する
	g_TimeCycle.fLength = sqrtf(SIZE * SIZE + SIZE * SIZE)* 0.5f;	//対角線の長さを算出する

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeCycle,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimeCycle->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + (-D3DX_PI + g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[0].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + (-D3DX_PI + g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + (D3DX_PI - g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[1].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + (D3DX_PI - g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z - g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[2].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z - g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[3].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定 
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	g_pVtxBuffTimeCycle->Unlock();
}

//<=====================================
//タイムサイクルの終了処理
//<=====================================
void UninitTimeCycle(void)
{
	//テクスチャの破棄
	if (g_pTextureTimeCycle != NULL)
	{
		g_pTextureTimeCycle->Release();
		g_pTextureTimeCycle = NULL;
	}
	//頂点バッファの処理
	if (g_pVtxBuffTimeCycle != NULL)
	{
		g_pVtxBuffTimeCycle->Release();
		g_pVtxBuffTimeCycle = NULL;
	}
}

//<=====================================
//タイムサイクルの更新処理
//<=====================================
void UpdateTimeCycle(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffTimeCycle->Lock(0, 0, (void**)&pVtx, 0);

	if (GetMode() == MODE_GAME && GetGameState() == GAMESTATE_NONE)
	{
		//タイムサイクル処理
		g_TimeCycle.rot.z -= D3DX_PI / (float)GAMETIME;

		if (g_TimeCycle.rot.z <= -D3DX_PI)
		{//一回転したら回転が終わる処理

			g_TimeCycle.rot.z = -D3DX_PI;
			//SetGameState(GAMESTATE_END, 60);
			//SetGameState(GAMESTATE_GAMEOVER, 0);
			SetNoFade(80);
			SetGameState(GAMESTATE_AFTEREND, 0);

			
		}
	}

	//頂点座標の設定
	pVtx[0].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + (-D3DX_PI + g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[0].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + (-D3DX_PI + g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + (D3DX_PI - g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[1].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + (D3DX_PI - g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z - g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[2].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z - g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[3].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[3].pos.z = 0.0f;

	//頂点バッファをアンロックする
	g_pVtxBuffTimeCycle->Unlock();
}

//<=====================================
//タイムサイクルの描画処理
//<=====================================
void DrawTimeCycle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ取得

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffTimeCycle, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//プレイヤーの描画処理
	pDevice->SetTexture(0, g_pTextureTimeCycle);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//プリミティブの種類

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
