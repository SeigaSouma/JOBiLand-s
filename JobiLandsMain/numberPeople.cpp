//==============================================================
//
//表示する人数[numberPeople.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "numberPeople.h"
#include "powergage.h"

//マクロ定義
#define NUM_PEOPLE	(2)			//人数の種類
#define MAX_NUMBER	(4)			//桁数
#define DIS_X		(45.0f)		//人数の間隔
#define WIDTH		(35.0f)		//数字の幅
#define HEIGHT		(30.0f)		//数字の高さ
#define MOVE_V		(0.1f)		//Vの分割数

//プロトタイプ宣言
void ScrollNumberPeople(int nCntPeople);

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTexturePeople[NUM_PEOPLE] = {};	//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPeople = NULL;		//頂点バッファへのポインタ
People g_aPeople[NUM_PEOPLE];							//人数の情報

//==============================================================
//人数の初期化処理
//==============================================================
void InitPeople(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D *pVtx;								//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_045.png",
		&g_pTexturePeople[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_055.png",
		&g_pTexturePeople[1]);

	//初期化
	for (int nCntPeople = 0; nCntPeople < PEOPLESTATE_MAX; nCntPeople++)
	{
		g_aPeople[nCntPeople].nPeople = 0;							//人数
		g_aPeople[nCntPeople].bUse = true;							//使用していない状態にする

		for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			g_aPeople[PEOPLESTATE_NEET].pos[nCntNumber] = D3DXVECTOR3(365.0f + (DIS_X * nCntNumber), 27.0f, 0.0f);	//位置を初期化する
			g_aPeople[PEOPLESTATE_ENEMY].pos[nCntNumber] = D3DXVECTOR3(780.0f + (DIS_X * nCntNumber), 27.0f, 0.0f);	//位置を初期化する
			g_aPeople[nCntPeople].aDestTexV[nCntNumber] = 0.0f;		//目標のV座標
			g_aPeople[nCntPeople].aTexV[nCntNumber] = 0.0f;			//現在のV座標
		}
	}


	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PEOPLESTATE_MAX * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPeople,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPeople->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPeople = 0; nCntPeople < PEOPLESTATE_MAX; nCntPeople++)
	{
		//数字のテクスチャ設定
		g_aPeople[nCntPeople].aTexV[0] = (g_aPeople[nCntPeople].nPeople % 10000 / 1000) * 0.1f;
		g_aPeople[nCntPeople].aTexV[1] = (g_aPeople[nCntPeople].nPeople % 1000 / 100) * 0.1f;
		g_aPeople[nCntPeople].aTexV[2] = (g_aPeople[nCntPeople].nPeople % 100 / 10) * 0.1f;
		g_aPeople[nCntPeople].aTexV[3] = (g_aPeople[nCntPeople].nPeople % 10 / 1) * 0.1f;

		for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aPeople[nCntPeople].pos[nCntNumber].x - WIDTH, g_aPeople[nCntPeople].pos[nCntNumber].y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPeople[nCntPeople].pos[nCntNumber].x + WIDTH, g_aPeople[nCntPeople].pos[nCntNumber].y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPeople[nCntPeople].pos[nCntNumber].x - WIDTH, g_aPeople[nCntPeople].pos[nCntNumber].y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPeople[nCntPeople].pos[nCntNumber].x + WIDTH, g_aPeople[nCntPeople].pos[nCntNumber].y + HEIGHT, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPeople[nCntPeople].aTexV[nCntNumber]);
			pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPeople[nCntPeople].aTexV[nCntNumber]);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPeople[nCntPeople].aTexV[nCntNumber] + 0.1f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPeople[nCntPeople].aTexV[nCntNumber] + 0.1f);

			pVtx += 4;		//頂点データのポインタを４つ分集める
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPeople->Unlock();
}

//==============================================================
//人数の終了処理
//==============================================================
void UninitPeople(void)
{
	int nCntPeople;

	//テクスチャの破棄
	for (nCntPeople = 0; nCntPeople < NUM_PEOPLE; nCntPeople++)
	{
		if (g_pTexturePeople[nCntPeople] != NULL)
		{
			g_pTexturePeople[nCntPeople]->Release();
			g_pTexturePeople[nCntPeople] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffPeople != NULL)
	{
		g_pVtxBuffPeople->Release();
		g_pVtxBuffPeople = NULL;
	}
}

//==============================================================
//人数の更新処理
//==============================================================
void UpdatePeople(void)
{
	PowerGage *pPowerGage = GetPowerGage();
	pPowerGage++;	//１つ分飛ばす

	for (int nCntPeople = 0; nCntPeople < PEOPLESTATE_MAX; nCntPeople++, pPowerGage++)
	{
		//人数代入
		g_aPeople[nCntPeople].nPeople = pPowerGage->population;

		//回転処理
		ScrollNumberPeople(nCntPeople);
	}
}

//==============================================================
//人数の回転処理
//==============================================================
void ScrollNumberPeople(int nCntPeople)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffPeople->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntPeople * MAX_NUMBER;

	//目標のテクスチャ座標設定
	g_aPeople[nCntPeople].aDestTexV[0] = (g_aPeople[nCntPeople].nPeople % 10000 / 1000) * 0.1f;
	g_aPeople[nCntPeople].aDestTexV[1] = (g_aPeople[nCntPeople].nPeople % 1000 / 100) * 0.1f;
	g_aPeople[nCntPeople].aDestTexV[2] = (g_aPeople[nCntPeople].nPeople % 100 / 10) * 0.1f;
	g_aPeople[nCntPeople].aDestTexV[3] = (g_aPeople[nCntPeople].nPeople % 10 / 1) * 0.1f;

	//数字の補正
	//for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	//{
	//	if (g_aPeople[nCntPeople].aTexV[nCntNumber] <= g_aPeople[nCntPeople].aDestTexV[nCntNumber])
	//	{//目標の値の方が大きい時

	//		//数字分補正
	//		g_aPeople[nCntPeople].aTexV[nCntNumber] += 9.0f;
	//	}
	//}

	//補正していく
	g_aPeople[nCntPeople].aTexV[0] += (g_aPeople[nCntPeople].aDestTexV[0] - g_aPeople[nCntPeople].aTexV[0]) * 0.15f;
	g_aPeople[nCntPeople].aTexV[1] += (g_aPeople[nCntPeople].aDestTexV[1] - g_aPeople[nCntPeople].aTexV[1]) * 0.15f;
	g_aPeople[nCntPeople].aTexV[2] += (g_aPeople[nCntPeople].aDestTexV[2] - g_aPeople[nCntPeople].aTexV[2]) * 0.15f;
	g_aPeople[nCntPeople].aTexV[3] += (g_aPeople[nCntPeople].aDestTexV[3] - g_aPeople[nCntPeople].aTexV[3]) * 0.15f;

	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPeople[nCntPeople].aTexV[nCntNumber]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPeople[nCntPeople].aTexV[nCntNumber]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPeople[nCntPeople].aTexV[nCntNumber] + 0.1f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPeople[nCntPeople].aTexV[nCntNumber] + 0.1f);

		pVtx += 4;				//頂点データのポインタを４つ分集める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffPeople->Unlock();
}


//==============================================================
//人数の描画処理
//==============================================================
void DrawPeople(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	int nCntPeople;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffPeople, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumber = 0; nCntNumber < PEOPLESTATE_MAX; nCntNumber++)
	{
		if (g_aPeople[nCntNumber].bUse == true)
		{
			for (nCntPeople = 0; nCntPeople < MAX_NUMBER; nCntPeople++)
			{
				if (nCntNumber == PEOPLESTATE_ENEMY)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_pTexturePeople[PEOPLESTATE_ENEMY]);
				}
				else if (nCntNumber == PEOPLESTATE_NEET)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_pTexturePeople[PEOPLESTATE_NEET]);
				}
				

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
					(4 * nCntPeople) + (4 * nCntNumber * MAX_NUMBER),			//プリミティブ（ポリゴンの数）
					2);												//描画するプリミティブ数
			}
		}
	}
}