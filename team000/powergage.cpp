//==============================================================
//
//DirectX[powergage.cpp]
//Author:日野澤匠泉
//
//==============================================================

#include "powergage.h"
#include "input.h"
#include "debugproc.h"
#include "crowd.h"
#include "game.h"

//マクロ定義
#define MAX_TEX		(4)		//テクスチャ最大数
#define SIZE		(150)	//サイズ
#define LENGTH		(600.0f)	//ゲージの長さ

//グローバル変数
LPDIRECT3DTEXTURE9 g_apTexturePowerGage[MAX_TEX] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPowerGage = NULL;

PowerGage g_aPowerGage[MAX_TEX];	//勢力ゲージの情報

//==============================================================
//勢力ゲージの初期化処理
//==============================================================
void InitPowerGage(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\powergauge.png",
		&g_apTexturePowerGage[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_apTexturePowerGage[1]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		NULL,
		&g_apTexturePowerGage[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\powergauge_02.png",
		&g_apTexturePowerGage[3]);


	//それぞれの初期位置
	g_aPowerGage[POWERGAGE_VTX_FRAM].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 80.0f, 0.0f);
	g_aPowerGage[POWERGAGE_VTX_NEET].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x - (LENGTH * 0.5f), g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y, 0.0f);
	g_aPowerGage[POWERGAGE_VTX_ENEMY].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x + (LENGTH * 0.5f), g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y, 0.0f);
	g_aPowerGage[POWERGAGE_VTX_TEX].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 80.0f, 0.0f);

	//仮の人数手打ちしたよん
	g_aPowerGage[POWERGAGE_VTX_NEET].population = 0;
	g_aPowerGage[POWERGAGE_VTX_ENEMY].population = TARGETHITO;

	//各種変数初期化
	for (int nCntPowerGage = 0; nCntPowerGage < MAX_TEX; nCntPowerGage++)
	{
		g_aPowerGage[nCntPowerGage].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aPowerGage[nCntPowerGage].result = 0.0f;		//比率計算結果
		g_aPowerGage[nCntPowerGage].bUse = true;
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPowerGage,
		NULL);

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPowerGage->Lock(0, 0, (void**)&pVtx, 0);

	//**********************************************************
	//淵
	//**********************************************************
	//位置を更新
	pVtx[0].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x - (LENGTH * 0.5f), g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y - SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x + (LENGTH * 0.5f), g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y - SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x - (LENGTH * 0.5f), g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y + SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x + (LENGTH * 0.5f), g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y + SIZE, 0.0f);

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

	//**********************************************************
	//味方のゲージ
	//**********************************************************
	//位置を更新
	pVtx[0].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_NEET].pos.x, g_aPowerGage[POWERGAGE_VTX_NEET].pos.y - SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_NEET].pos.x + LENGTH, g_aPowerGage[POWERGAGE_VTX_NEET].pos.y - SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_NEET].pos.x, g_aPowerGage[POWERGAGE_VTX_NEET].pos.y + SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_NEET].pos.x + LENGTH, g_aPowerGage[POWERGAGE_VTX_NEET].pos.y + SIZE, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//**********************************************************
	//敵のゲージ
	//**********************************************************
	//位置を更新
	pVtx[0].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.x - LENGTH, g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.y - SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.x, g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.y - SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.x - LENGTH, g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.y + SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.x, g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.y + SIZE, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//位置を更新
	pVtx[0].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x - (LENGTH * 0.5f) - 30.0f, g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y - SIZE, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x + (LENGTH * 0.5f) + 30.0f, g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y - SIZE, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x - (LENGTH * 0.5f) - 30.0f, g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y + SIZE, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_FRAM].pos.x + (LENGTH * 0.5f) + 30.0f, g_aPowerGage[POWERGAGE_VTX_FRAM].pos.y + SIZE, 0.0f);

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
	g_pVtxBuffPowerGage->Unlock();
}

//==============================================================
//勢力ゲージの画面の終了処理
//==============================================================
void UninitPowerGage(void)
{
	for (int nCntPowerGage = 0; nCntPowerGage < MAX_TEX; nCntPowerGage++)
	{
		//テクスチャの破棄
		if (g_apTexturePowerGage[nCntPowerGage] != NULL)
		{
			g_apTexturePowerGage[nCntPowerGage]->Release();
			g_apTexturePowerGage[nCntPowerGage] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPowerGage != NULL)
	{
		g_pVtxBuffPowerGage->Release();
		g_pVtxBuffPowerGage = NULL;
	}

}

//==============================================================
//勢力ゲージの画面の更新処理
//==============================================================
void UpdatePowerGage(void)
{
	VERTEX_2D *pVtx;
	float fCurrentGage;		//現在のゲージの量
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//目標の人数の情報取得
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

	g_aPowerGage[POWERGAGE_VTX_NEET].population = 3 + pCrowdInfo->nCrowdValue;
	g_aPowerGage[POWERGAGE_VTX_ENEMY].population = pGetnTargetNop->nCntPeople;

	//0人以下は0固定
	if (g_aPowerGage[POWERGAGE_VTX_NEET].population < 0)
	{
		g_aPowerGage[POWERGAGE_VTX_NEET].population = 0;
	}
	if (g_aPowerGage[POWERGAGE_VTX_ENEMY].population < 0)
	{
		g_aPowerGage[POWERGAGE_VTX_ENEMY].population = 0;
	}

	//ニートの比率を出す処理
	g_aPowerGage[POWERGAGE_VTX_NEET].result = (float)g_aPowerGage[POWERGAGE_VTX_ENEMY].population /
		(float)(g_aPowerGage[POWERGAGE_VTX_ENEMY].population + g_aPowerGage[POWERGAGE_VTX_NEET].population);

	//全体の長さ - ニートの長さ
	fCurrentGage = LENGTH * g_aPowerGage[POWERGAGE_VTX_NEET].result;
	float fNeetLength = LENGTH - fCurrentGage;

	//頂点バッファをロックし頂点情報へのポインタを取得
	g_pVtxBuffPowerGage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPowerGage = 0; nCntPowerGage < MAX_TEX; nCntPowerGage++)
	{
		g_aPowerGage[nCntPowerGage].bUse = true;	//使用されている状態

		if (g_aPowerGage[nCntPowerGage].bUse == true)
		{//使用されていたら

			//位置を更新
			pVtx[POWERGAGE_VTX_ENEMY * 4 + 0].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.x - fCurrentGage, g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.y - SIZE, 0.0f);
			pVtx[POWERGAGE_VTX_ENEMY * 4 + 1].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.x, g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.y - SIZE, 0.0f);
			pVtx[POWERGAGE_VTX_ENEMY * 4 + 2].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.x - fCurrentGage, g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.y + SIZE, 0.0f);
			pVtx[POWERGAGE_VTX_ENEMY * 4 + 3].pos = D3DXVECTOR3(g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.x, g_aPowerGage[POWERGAGE_VTX_ENEMY].pos.y + SIZE, 0.0f);
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPowerGage->Unlock();

	//デバッグ表示
	PrintDebugProc("ニート:%d  敵:%d\n",g_aPowerGage[POWERGAGE_VTX_NEET].population, g_aPowerGage[POWERGAGE_VTX_ENEMY].population);
}

//==============================================================
//勢力ゲージの画面の描画処理
//==============================================================
void DrawPowerGage(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();			

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPowerGage, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntPowerGage = 0; nCntPowerGage < MAX_TEX; nCntPowerGage++)
	{
		if (g_aPowerGage[nCntPowerGage].bUse == true)
		{//使用されている時

			switch (nCntPowerGage)
			{
			case POWERGAGE_VTX_FRAM:

				//アルファテストを有効にする
				pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
				pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
				pDevice->SetRenderState(D3DRS_ALPHAREF, 250);

				//αブレンディングを加算合成に設定
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				break;

			case POWERGAGE_VTX_NEET:

				//Zテストを無効にする
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
				break;

			case POWERGAGE_VTX_ENEMY:

				//Zテストを無効にする
				pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
				break;
			}

			//テクスチャセット
			pDevice->SetTexture(0, g_apTexturePowerGage[nCntPowerGage]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntPowerGage * 4, 2);	//プリミティブの種類

			//αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

			//アルファテストを無効にする
			pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
			pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
			pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

			//Zテストを有効にする
			pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
			pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
		}
	}
}

//==============================================================
//勢力ゲージの情報
//==============================================================
PowerGage *GetPowerGage(void)
{
	return &g_aPowerGage[0];
}