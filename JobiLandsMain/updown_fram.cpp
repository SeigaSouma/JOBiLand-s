//=============================================================================
//
// 上下の枠処理 [updown_fram.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input.h"
#include "updown_fram.h"
#include "sound.h"

#define MOVE_TIME	(20)
#define POS_X		(640.0f)
#define POS_Y		(50.0f)

//プロトタイプ宣言
void UpdateInUpDownFram(void);
void UpdateOutUpDownFram(void);

//グローバル変数宣言
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUpDownFram = NULL;		//頂点バッファへのポインタ
UPDOWN_FRAM g_aUpDownFram;		//上下の枠

//=============================================================================
//上下の枠初期化処理
//=============================================================================					   
void InitUpDownFram(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * FRAMTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUpDownFram,
		NULL);

	//各要素初期化
	g_aUpDownFram.pos[FRAMTYPE_UP] = D3DXVECTOR3(POS_X, -POS_Y, 0.0f);	//位置
	g_aUpDownFram.pos[FRAMTYPE_DOWN] = D3DXVECTOR3(POS_X, SCREEN_HEIGHT + POS_Y, 0.0f);	//位置
	g_aUpDownFram.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//色
	g_aUpDownFram.nState = UPDOWN_FRAM_STATE_IN;	//入ってく状態
	g_aUpDownFram.bUse = false;			//使用しているか
	g_aUpDownFram.bEnd = false;			//到着しているか

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUpDownFram->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFram = 0; nCntFram < FRAMTYPE_MAX; nCntFram++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = g_aUpDownFram.col;
		pVtx[1].col = g_aUpDownFram.col;
		pVtx[2].col = g_aUpDownFram.col;
		pVtx[3].col = g_aUpDownFram.col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffUpDownFram->Unlock();
}

//=============================================================================
//上下の枠終了処理
//=============================================================================
void UninitUpDownFram(void)
{

	//頂点バッファの破棄
	if (g_pVtxBuffUpDownFram != NULL)
	{
		g_pVtxBuffUpDownFram->Release();
		g_pVtxBuffUpDownFram = NULL;
	}
}

//=============================================================================
//上下の枠更新処理
//=============================================================================
void UpdateUpDownFram(void)
{

	switch (g_aUpDownFram.nState)
	{
	case UPDOWN_FRAM_STATE_IN:

		//上下の枠進んでく処理
		UpdateInUpDownFram();
		break;

	case UPDOWN_FRAM_STATE_OUT:

		//上下の枠はけてく処理
		UpdateOutUpDownFram();
		break;
	}

}

//=============================================================================
//上下の枠進んでく処理
//=============================================================================
void UpdateInUpDownFram(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUpDownFram->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aUpDownFram.bUse == true)
	{//使用されていたら

		//位置更新
		if (g_aUpDownFram.pos[FRAMTYPE_UP].y >= POS_Y)
		{//規定値を超えたら

			g_aUpDownFram.pos[FRAMTYPE_UP].y = POS_Y;
			g_aUpDownFram.pos[FRAMTYPE_DOWN].y = SCREEN_HEIGHT - POS_Y;
			g_aUpDownFram.bEnd = true;
		}
		else
		{
			//上下ではさむ
			g_aUpDownFram.pos[FRAMTYPE_UP].y += 2.0f;
			g_aUpDownFram.pos[FRAMTYPE_DOWN].y -= 2.0f;
		}

		//不透明度更新
		if (g_aUpDownFram.col.a >= 1.0f)
		{
			g_aUpDownFram.col.a = 1.0f;
		}
		else
		{
			g_aUpDownFram.col.a += 0.025f;
		}

		for (int nCntFram = 0; nCntFram < FRAMTYPE_MAX; nCntFram++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);

			//頂点カラー
			pVtx[0].col = g_aUpDownFram.col;
			pVtx[1].col = g_aUpDownFram.col;
			pVtx[2].col = g_aUpDownFram.col;
			pVtx[3].col = g_aUpDownFram.col;

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffUpDownFram->Unlock();
}

//=============================================================================
//上下の枠はけてく処理
//=============================================================================
void UpdateOutUpDownFram(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUpDownFram->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aUpDownFram.bUse == true)
	{//使用されていたら

	 //位置更新
		if (g_aUpDownFram.pos[FRAMTYPE_UP].y <= -POS_Y)
		{//規定値を超えたら

			g_aUpDownFram.pos[FRAMTYPE_UP].y = -POS_Y;
			g_aUpDownFram.pos[FRAMTYPE_DOWN].y = SCREEN_HEIGHT + POS_Y;
		}
		else
		{
			//上下消えてく
			g_aUpDownFram.pos[FRAMTYPE_UP].y -= 2.0f;
			g_aUpDownFram.pos[FRAMTYPE_DOWN].y += 2.0f;
		}

		//不透明度更新
		if (g_aUpDownFram.col.a <= 0.0f)
		{
			g_aUpDownFram.col.a = 0.0f;
			g_aUpDownFram.bUse = false;
		}
		else
		{
			g_aUpDownFram.col.a -= 0.025f;
		}

		for (int nCntFram = 0; nCntFram < FRAMTYPE_MAX; nCntFram++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);

			//頂点カラー
			pVtx[0].col = g_aUpDownFram.col;
			pVtx[1].col = g_aUpDownFram.col;
			pVtx[2].col = g_aUpDownFram.col;
			pVtx[3].col = g_aUpDownFram.col;

			pVtx += 4;
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffUpDownFram->Unlock();
}

//=============================================================================
//上下の枠描画処理
//=============================================================================
void DrawUpDownFram(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffUpDownFram, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aUpDownFram.bUse == true)
	{
		for (int nCntFram = 0; nCntFram < FRAMTYPE_MAX; nCntFram++)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, NULL);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFram * 4, 2);
		}
	}
}

//=============================================================================
//上下の枠設定処理
//=============================================================================
void SetUpDownFram(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffUpDownFram->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aUpDownFram.bUse == false)
	{//使用されていなかったら

		g_aUpDownFram.pos[FRAMTYPE_UP] = D3DXVECTOR3(POS_X, -POS_Y, 0.0f);	//位置
		g_aUpDownFram.pos[FRAMTYPE_DOWN] = D3DXVECTOR3(POS_X, SCREEN_HEIGHT + POS_Y, 0.0f);	//位置
		g_aUpDownFram.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//色
		g_aUpDownFram.nState = UPDOWN_FRAM_STATE_IN;	//入ってく状態
		g_aUpDownFram.bUse = true;	//使用している状態にする
		g_aUpDownFram.bEnd = false;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffUpDownFram->Unlock();
}

//=============================================================================
//上下の枠情報取得
//=============================================================================
UPDOWN_FRAM *GetUpDownFram(void)
{
	return &g_aUpDownFram;
}