//==============================================================
//
//表示の背景[ground.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "ground.h"
#include "numberPeople.h"

//マクロ定義
#define MAX_GROUND			(2)			//種類
#define MAX_NUMBER			(4)			//桁数
#define NUMBER_WIDTH		(45.0f)		//表示背景の幅
#define NUMBER_HEIGHTU		(68.0f)		//人数の上高さ
#define NUMBER_HEIGHTD		(63.0f)		//人数の下高さ
#define NEET_WIDTHL			(310.0f)	//ニートの人数の左の位置
#define NEET_WIDTHR			(240.0f)	//ニートの人数の右の位置
#define ENEMY_WIDTHL		(105.0f)	//敵の人数の左の位置
#define ENEMY_WIDTHR		(175.0f)	//敵の人数の右の位置

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureGround[MAX_GROUND] = {};				//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGround = NULL;		//頂点バッファへのポインタ
Ground g_aGround[MAX_GROUND];							//表示背景の情報

//==============================================================
//表示背景の初期化処理
//==============================================================
void InitGround(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ground002.png",
		&g_pTextureGround[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ground001.png",
		&g_pTextureGround[1]);

	VERTEX_2D *pVtx;								//頂点情報へのポインタ

	for (int nCntGround = 0; nCntGround < MAX_GROUND; nCntGround++)
	{
		g_aGround[nCntGround].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置を初期化する
		g_aGround[nCntGround].fWidth = NUMBER_WIDTH;				//幅を初期化する
		g_aGround[nCntGround].fHeight = NUMBER_HEIGHTD;				//高さを初期化する
		g_aGround[nCntGround].bUse = true;							//使用していない状態にする
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUMBER * MAX_GROUND,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGround,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffGround->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPeople = 0; nCntPeople < MAX_GROUND; nCntPeople++)
	{
		for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (nCntPeople == 0)
			{//ニート

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 - NEET_WIDTHL), g_aGround[nCntPeople].fHeight - NUMBER_HEIGHTU, 0.0f);
				pVtx[1].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 - NEET_WIDTHR), g_aGround[nCntPeople].fHeight - NUMBER_HEIGHTU, 0.0f);
				pVtx[2].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 - NEET_WIDTHL), g_aGround[nCntPeople].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 - NEET_WIDTHR), g_aGround[nCntPeople].fHeight, 0.0f);
			}
			else if (nCntPeople == 1)
			{//敵

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 + ENEMY_WIDTHL), g_aGround[nCntPeople].fHeight - NUMBER_HEIGHTU, 0.0f);
				pVtx[1].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 + ENEMY_WIDTHR), g_aGround[nCntPeople].fHeight - NUMBER_HEIGHTU, 0.0f);
				pVtx[2].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 + ENEMY_WIDTHL), g_aGround[nCntPeople].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 + ENEMY_WIDTHR), g_aGround[nCntPeople].fHeight, 0.0f);
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

			pVtx += 4;											//頂点データのポインタを４つ分集める
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffGround->Unlock();
}

//==============================================================
//表示背景の終了処理
//==============================================================
void UninitGround(void)
{
	for (int nCntGround = 0; nCntGround < MAX_GROUND; nCntGround++)
	{
		if (g_pTextureGround[nCntGround] != NULL)
		{
			g_pTextureGround[nCntGround]->Release();
			g_pTextureGround[nCntGround] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffGround != NULL)
	{
		g_pVtxBuffGround->Release();
		g_pVtxBuffGround = NULL;
	}
}

//==============================================================
//表示背景の更新処理
//==============================================================
void UpdateGround(void)
{
	
}

//==============================================================
//表示背景の描画処理
//==============================================================
void DrawGround(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	int nCntGround;

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffGround, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumber = 0; nCntNumber < MAX_GROUND; nCntNumber++)
	{
		if (g_aGround[nCntNumber].bUse == true)
		{
			for (nCntGround = 0; nCntGround < MAX_NUMBER; nCntGround++)
			{
				if (nCntNumber == PEOPLESTATE_ENEMY)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_pTextureGround[PEOPLESTATE_ENEMY]);
				}
				else if (nCntNumber == PEOPLESTATE_NEET)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_pTextureGround[PEOPLESTATE_NEET]);
				}

				//ポリゴンの描画
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
					(4 * nCntGround) + (4 * nCntNumber * MAX_NUMBER),			//プリミティブ（ポリゴンの数）
					2);												//描画するプリミティブ数
			}
		}
	}
}