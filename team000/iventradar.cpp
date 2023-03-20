//==============================================================
//
//イベントサインの下地[IventRadar.cpp]
//Author:大原怜将
//
//==============================================================
#include "iventradar.h"
#include "mob.h"
#include "comment.h"

//マクロ定義
#define NUM_RADAR			(2)			//人数の種類
#define NUMBER_WIDTH		(250.0f)	//幅
#define NUMBER_HEIGHT		(250.0f)	//高さ

////グローバル変数宣言
//const char *c_apFilenameIventRadar[] =	//ファイル読み込み
//{
//	"data\\TEXTURE\\radar1.png",	//大量発生なし
//	//"data\\TEXTURE\\radar2.png",	//大量発生中
//};

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureIventRadar[NUM_RADAR] = {};	//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIventRadar = NULL;		//頂点バッファへのポインタ
IventRadar g_aIventRadar[NUM_RADAR];						//人数の情報

//==============================================================
//人数の初期化処理
//==============================================================
void InitIventRadar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D *pVtx;								//頂点情報へのポインタ
	
	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\radar2.png",
		&g_pTextureIventRadar[0]);

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\radar1.png",
		&g_pTextureIventRadar[1]);

	//初期化
	for (int nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		g_aIventRadar[nCntIventRadar].pos = D3DXVECTOR3(1380.0f, 590.0f, 0.0f);	//位置を初期化する
		g_aIventRadar[nCntIventRadar].col = { 1.0f, 1.0f, 1.0f, 1.0f };     //色の初期化
		g_aIventRadar[nCntIventRadar].fWidth = NUMBER_WIDTH;				//幅を初期化する
		g_aIventRadar[nCntIventRadar].fHeight = NUMBER_HEIGHT;				//高さを初期化する
		g_aIventRadar[nCntIventRadar].bUse = true;							//使用していない状態にする

		if (nCntIventRadar == 0)
		{//大量発生中

			g_aIventRadar[nCntIventRadar].type = RADARTYPE_MANYPEOPLE;
		}
		
		if (nCntIventRadar == 1)
		{//大量発生なし

			g_aIventRadar[nCntIventRadar].type = RADARTYPE_FEWPEOPLE;
		}
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RADAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffIventRadar,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffIventRadar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		if (nCntIventRadar == 0)
		{//大量発生中

		    //頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
		}
		
		if (nCntIventRadar == 1)
		{//大量発生なし

		    //頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
		}

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//頂点データのポインタを４つ分集める
		
	}

	//頂点バッファをアンロックする
	g_pVtxBuffIventRadar->Unlock();
}

//==============================================================
//人数の終了処理
//==============================================================
void UninitIventRadar(void)
{
	int nCntIventRadar;

	//テクスチャの破棄
	for (nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		if (g_pTextureIventRadar[nCntIventRadar] != NULL)
		{
			g_pTextureIventRadar[nCntIventRadar]->Release();
			g_pTextureIventRadar[nCntIventRadar] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffIventRadar != NULL)
	{
		g_pVtxBuffIventRadar->Release();
		g_pVtxBuffIventRadar = NULL;
	}
}

//==============================================================
//人数の更新処理
//==============================================================
void UpdateIventRadar(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	SwitchSpawn *pSwitchSpawn =  GetSwitchSpawn();    //大量発生中かどうかの情報を取得

	Commnet *pCommnet = GetCommnet();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffIventRadar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		//g_aIventRadar[nCntIventRadar].pos = D3DXVECTOR3(1160.0f, 590.0f, 0.0f);	//位置

		if (g_aIventRadar[nCntIventRadar].type == RADARTYPE_MANYPEOPLE)
		{//大量発生なし

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
		}

		if (g_aIventRadar[nCntIventRadar].type == RADARTYPE_FEWPEOPLE)
		{//大量発生中
			
			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
		}

		if (pSwitchSpawn->bUseSpawn == false && pCommnet->bUse == false)
		{//大量発生していないとき

			float fDest = 1380.0f;	//目標の角度
			float fDiff = fDest - g_aIventRadar[nCntIventRadar].pos.x;	//現在と目標の差分

			D3DXVECTOR3 move;

			

			if (g_aIventRadar[RADARTYPE_FEWPEOPLE].col.a >= 1.0f)
			{//透明度が1以上だったら

				g_aIventRadar[nCntIventRadar].pos.x += fDiff * 0.15f;
			}

			if (g_aIventRadar[nCntIventRadar].type == RADARTYPE_FEWPEOPLE)
			{//大量発生していないときのレーダーだったら

				g_aIventRadar[nCntIventRadar].col.a += 0.015f;      //透明度を下げる

				if (g_aIventRadar[nCntIventRadar].col.a >= 1.0f)
				{//透明度が1以上だったら

					g_aIventRadar[nCntIventRadar].col.a = 1.0f;    //1に戻す
				}

				//頂点カラー
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
			}
		}

		if (pCommnet->bUse == true)
		{//大量発生しているとき

			float fDest = 1160.0f;	//目標の角度
			float fDiff = fDest - g_aIventRadar[nCntIventRadar].pos.x;	//現在と目標の差分
			g_aIventRadar[nCntIventRadar].pos.x += fDiff * 0.15f;

			D3DXVECTOR3 move;

			//if (fDiff >= 0)
			//{//差分が
			//	move.x = 5.0f;

			//	g_aIventRadar[nCntIventRadar].pos.x -= move.x;
			//}
			
			if (g_aIventRadar[nCntIventRadar].type == RADARTYPE_FEWPEOPLE)
			{//大量発生していないときのレーダーだったら

				if (fDiff <= 0)
				{
					g_aIventRadar[nCntIventRadar].col.a -= 0.015f;     //透明度を上げる

					if (g_aIventRadar[nCntIventRadar].col.a <= 0.0f)
					{//透明度が0以下だったら

						g_aIventRadar[nCntIventRadar].col.a = 0.0f;    //0に戻す
					}

					//頂点カラー
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
				}
			}
			
		}

		pVtx += 4;     //頂点データのポインタを４つ分集める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffIventRadar->Unlock();
}

//==============================================================
//人数の描画処理
//==============================================================
void DrawIventRadar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	////Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffIventRadar, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		if (g_aIventRadar[nCntIventRadar].bUse == true)
		{//使用されていたら

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureIventRadar[nCntIventRadar]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
				4 * nCntIventRadar,			                    //プリミティブ（ポリゴンの数）
				2);												//描画するプリミティブ数
		}
	}

	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==============================================================
//レーダーの情報
//==============================================================
IventRadar *GetIventRadar(void)
{
	return &g_aIventRadar[0];
}