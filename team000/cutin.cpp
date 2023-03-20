//=========================================================================
//
//カットイン処理
//Author:日野澤匠泉	[cutin.cpp]
//
//=========================================================================

#include "main.h"
#include "input.h"
#include "cutin.h"
#include "sound.h"

//関数宣言
void UpdateSentence(void);
void MoveSentence(void);

//グローバル変数宣言
LPDIRECT3DTEXTURE9	g_pTextureCutin[MAX_TEX] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffCutin = NULL;			//頂点のバッファ
CUTIN g_aCutin[MAX_TEX];		//カットイン情報

int g_nCnter;		//カウンター

//テクスチャファイル名
const char *c_apFilenameCutin[] =
{
	"data/TEXTURE/cutin.png",
	"data/TEXTURE/nitya.png"
};

//=========================================================================
//カットインの初期化処理
//=========================================================================
void InitCutin(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスのポインタ

	//テクスチャの読み込み
	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCutin[nCntCutin],
			&g_pTextureCutin[nCntCutin]);
	}

	//各種変数初期化
	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		g_aCutin[nCntCutin].pos = { 0.0f,0.0f,0.0f };
		g_aCutin[nCntCutin].rot = { 0.0f,0.0f,0.0f };
		g_aCutin[nCntCutin].move = { 1.5f,0.0f,0.0f };

		g_aCutin[nCntCutin].col = { 1.0f,1.0f,1.0f,1.0f };

		g_aCutin[nCntCutin].fHeight = 0.0f;
		g_aCutin[nCntCutin].fWidth = 0.0f;

		g_nCnter = 0;

		g_aCutin[nCntCutin].bUse = false;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCutin,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCutin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		switch (g_aCutin[nCntCutin].nType)
		{
		case 0:

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - TEXT_X_CUTIN, g_aCutin[nCntCutin].pos.y - TEXT_Y_CUTIN, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + TEXT_X_CUTIN, g_aCutin[nCntCutin].pos.y - TEXT_Y_CUTIN, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - TEXT_X_CUTIN, g_aCutin[nCntCutin].pos.y + TEXT_Y_CUTIN, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + TEXT_X_CUTIN, g_aCutin[nCntCutin].pos.y + TEXT_Y_CUTIN, 0.0f);

			break;

		case 1:

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - TEXT_X_TEXT, g_aCutin[nCntCutin].pos.y - TEXT_Y_TEXT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + TEXT_X_TEXT, g_aCutin[nCntCutin].pos.y - TEXT_Y_TEXT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - TEXT_X_TEXT, g_aCutin[nCntCutin].pos.y + TEXT_Y_TEXT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + TEXT_X_TEXT, g_aCutin[nCntCutin].pos.y + TEXT_Y_TEXT, 0.0f);

			break;
		}

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

		pVtx += 4;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffCutin->Unlock();

}

//=========================================================================
//カットインの終了処理
//=========================================================================
void UninitCutin(void)
{
	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		//テクスチャの破棄
		if (g_pTextureCutin[nCntCutin] != NULL)
		{
			g_pTextureCutin[nCntCutin]->Release();
			g_pTextureCutin[nCntCutin] = NULL;
		}
	}

	//頂点バッファの処理
	if (g_pVtxBuffCutin != NULL)
	{
		g_pVtxBuffCutin->Release();
		g_pVtxBuffCutin = NULL;
	}
}


//=========================================================================
//カットインの更新処理
//=========================================================================
void UpdateCutin(void)
{
	
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCutin->Lock(0, 0, (void**)&pVtx, 0);

		if (g_aCutin[IMAGE].bUse == true)
		{//使用されていたら

			//目的地に向かって進む処理
			g_aCutin[IMAGE].pos.x += ((SCREEN_WIDTH / 2) - g_aCutin[IMAGE].pos.x) * 0.25f;
			g_aCutin[IMAGE].fHeight += ((TEXT_Y_CUTIN) - g_aCutin[IMAGE].fHeight) * 0.25f;
			g_aCutin[IMAGE].fWidth += ((TEXT_X_CUTIN)- g_aCutin[IMAGE].fWidth) * 0.25f;

			//頂点座標の設定
			pVtx[IMAGE * 4 + 0].pos = D3DXVECTOR3(g_aCutin[IMAGE].pos.x - g_aCutin[IMAGE].fWidth, g_aCutin[IMAGE].pos.y - g_aCutin[IMAGE].fHeight, 0.0f);
			pVtx[IMAGE * 4 + 1].pos = D3DXVECTOR3(g_aCutin[IMAGE].pos.x + g_aCutin[IMAGE].fWidth, g_aCutin[IMAGE].pos.y - g_aCutin[IMAGE].fHeight, 0.0f);
			pVtx[IMAGE * 4 + 2].pos = D3DXVECTOR3(g_aCutin[IMAGE].pos.x - g_aCutin[IMAGE].fWidth, g_aCutin[IMAGE].pos.y + g_aCutin[IMAGE].fHeight, 0.0f);
			pVtx[IMAGE * 4 + 3].pos = D3DXVECTOR3(g_aCutin[IMAGE].pos.x + g_aCutin[IMAGE].fWidth, g_aCutin[IMAGE].pos.y + g_aCutin[IMAGE].fHeight, 0.0f);

			//頂点カラーの設定
			pVtx[IMAGE * 4 + 0].col = g_aCutin[IMAGE].col;
			pVtx[IMAGE * 4 + 1].col = g_aCutin[IMAGE].col;
			pVtx[IMAGE * 4 + 2].col = g_aCutin[IMAGE].col;
			pVtx[IMAGE * 4 + 3].col = g_aCutin[IMAGE].col;
		}

		//頂点バッファをアンロックする
		g_pVtxBuffCutin->Unlock();

		//文字列の更新
		UpdateSentence();

}

//=========================================================================
//文章の更新処理
//=========================================================================
void UpdateSentence(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCutin->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aCutin[SENTENCE].bUse == true)
	{//使用されていたら

		//文字を微動させる処理
		MoveSentence();

		//目的の位置に向かわせる処理
		g_aCutin[SENTENCE].col.a += ((1.0f) - g_aCutin[SENTENCE].col.a) * 0.03f;
		g_aCutin[SENTENCE].pos.y += ((220.0f) - g_aCutin[SENTENCE].pos.y) * 0.01f;

		//頂点座標の設定
		pVtx[SENTENCE * 4 + 0].pos = D3DXVECTOR3(g_aCutin[SENTENCE].pos.x - g_aCutin[SENTENCE].fWidth, g_aCutin[SENTENCE].pos.y - g_aCutin[SENTENCE].fHeight, 0.0f);
		pVtx[SENTENCE * 4 + 1].pos = D3DXVECTOR3(g_aCutin[SENTENCE].pos.x + g_aCutin[SENTENCE].fWidth, g_aCutin[SENTENCE].pos.y - g_aCutin[SENTENCE].fHeight, 0.0f);
		pVtx[SENTENCE * 4 + 2].pos = D3DXVECTOR3(g_aCutin[SENTENCE].pos.x - g_aCutin[SENTENCE].fWidth, g_aCutin[SENTENCE].pos.y + g_aCutin[SENTENCE].fHeight, 0.0f);
		pVtx[SENTENCE * 4 + 3].pos = D3DXVECTOR3(g_aCutin[SENTENCE].pos.x + g_aCutin[SENTENCE].fWidth, g_aCutin[SENTENCE].pos.y + g_aCutin[SENTENCE].fHeight, 0.0f);

		//頂点カラーの設定
		pVtx[SENTENCE * 4 + 0].col = g_aCutin[SENTENCE].col;
		pVtx[SENTENCE * 4 + 1].col = g_aCutin[SENTENCE].col;
		pVtx[SENTENCE * 4 + 2].col = g_aCutin[SENTENCE].col;
		pVtx[SENTENCE * 4 + 3].col = g_aCutin[SENTENCE].col;

	}

	//頂点バッファをアンロックする
	g_pVtxBuffCutin->Unlock();

}

//=========================================================================
//文章移動の更新処理
//=========================================================================
void MoveSentence(void)
{
	//カウンター加算
	g_nCnter++;

	//**********************************
	//微動させる処理
	//**********************************
	if (g_aCutin[SENTENCE].pos.x > 1010.0f)
	{// [X] 1020より上

		g_aCutin[SENTENCE].pos.x = 1010.0f;
		g_aCutin[SENTENCE].move.x *= -1;

	}

	else if (g_aCutin[SENTENCE].pos.x < 990.0f)
	{// [X] 980より下

		g_aCutin[SENTENCE].pos.x = 990.0f;
		g_aCutin[SENTENCE].move.x *= -1;

	}

	//位置の更新
	g_aCutin[SENTENCE].pos.x += g_aCutin[SENTENCE].move.x;

	if (g_nCnter >= 80)
	{//80以上だった時

		//α値減算
		g_aCutin[SENTENCE].col.a -= 0.1f;
		g_aCutin[IMAGE].col.a -= 0.1f;

		for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
		{
			if (g_aCutin[nCnt].col.a <= 0.0f)
			{//α値が0.0f以下のとき

				g_aCutin[SENTENCE].bUse = false;	//使用されていない状態にする
				g_aCutin[IMAGE].bUse = false;	//使用されていない状態にする
				g_nCnter = 0;		//カウンター初期化

				//フィーバー音
				PlaySound(SOUND_LABEL_BGM_FEVER);
			}
		}
	}
}

//=========================================================================
//カットインの描画処理
//=========================================================================
void DrawCutin(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//デバイスのポインタ
	pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCutin, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		if (g_aCutin[nCntCutin].bUse == true)
		{//使用されていたら

			switch (g_aCutin[nCntCutin].nType)
			{
			case 0:

				//カットインの描画
				pDevice->SetTexture(0, g_pTextureCutin[0]);

				break;

			case 1:

				//カットインの描画
				pDevice->SetTexture(0, g_pTextureCutin[1]);

				break;
			}

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCutin * 4, 2);		//プリミティブの種類
		}

	}
}

//=========================================================================
//カットインの設定処理
//=========================================================================
void SetCutin(D3DXVECTOR3 pos, int nType, float fHeight,float fWidth)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffCutin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		if (g_aCutin[nCntCutin].bUse == false)
		{//カットインが使用されていない時

			g_aCutin[nCntCutin].pos = pos;
			g_aCutin[nCntCutin].nType = nType;
			g_aCutin[nCntCutin].fHeight = fHeight;
			g_aCutin[nCntCutin].fWidth = fWidth;

			//使用されている状態にする
			g_aCutin[nCntCutin].bUse = true;

			g_aCutin[SENTENCE].col.a = 0.0f;
			g_aCutin[IMAGE].col.a = 1.0f;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - g_aCutin[nCntCutin].fWidth, g_aCutin[nCntCutin].pos.y - g_aCutin[nCntCutin].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + g_aCutin[nCntCutin].fWidth, g_aCutin[nCntCutin].pos.y - g_aCutin[nCntCutin].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - g_aCutin[nCntCutin].fWidth, g_aCutin[nCntCutin].pos.y + g_aCutin[nCntCutin].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + g_aCutin[nCntCutin].fWidth, g_aCutin[nCntCutin].pos.y + g_aCutin[nCntCutin].fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}


	//頂点バッファをアンロックする
	g_pVtxBuffCutin->Unlock();
}

//=========================================================================
//カットインの情報取得
//=========================================================================
CUTIN *GetCutin(void)
{
	return &g_aCutin[0];
}