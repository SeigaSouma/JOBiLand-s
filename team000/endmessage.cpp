//==============================================================
//
//終わる前に出すメッセージ[endmessage.cpp]
//Author:大原怜将
//
//==============================================================
#include "endmessage.h"
#include "input.h"
#include "sound.h"

//マクロ定義
#define NUMBER_WIDTH		(300.0f * 1.5f)	//幅
#define NUMBER_HEIGHT		(185.0f * 1.5f)	//高さ
#define COUNTZOOM           (15)

////グローバル変数宣言g_aEndmessage
//const char *c_apFilenameEndmessage[] =	//ファイル読み込み
//{
//	"data\\TEXTURE\\radar1.png",	//大量発生なし
//	//"data\\TEXTURE\\radar2.png",	//大量発生中
//};

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureEndmessage = NULL;	//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEndmessage = NULL;		//頂点バッファへのポインタ
Endmessage g_aEndmessage;

//==============================================================
//終わる少し前に出すメッセージの初期化処理
//==============================================================
void InitEndmessage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D *pVtx;								//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\endmessage.png",
		&g_pTextureEndmessage);

	
	//初期化
	g_aEndmessage.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置を初期化する
	g_aEndmessage.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   //色を初期化する
	g_aEndmessage.nCounter = 0;                          //カウンターを初期化
	g_aEndmessage.nCntZoom = 0;                          //カウンターを初期化
	g_aEndmessage.fWidth = 0.0f;				 //幅を初期化する
	g_aEndmessage.fHeight = 0.0f;			     //高さを初期化する
	g_aEndmessage.bUse = false;                          //使用していない状態にする

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEndmessage,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEndmessage->Lock(0, 0, (void**)&pVtx, 0);


	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aEndmessage.pos.x - NUMBER_WIDTH, g_aEndmessage.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEndmessage.pos.x + NUMBER_WIDTH, g_aEndmessage.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEndmessage.pos.x - NUMBER_WIDTH, g_aEndmessage.pos.y + NUMBER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEndmessage.pos.x + NUMBER_WIDTH, g_aEndmessage.pos.y + NUMBER_HEIGHT, 0.0f);

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

	//頂点バッファをアンロックする
	g_pVtxBuffEndmessage->Unlock();
}

//==============================================================
//終わる少し前に出すメッセージの終了処理
//==============================================================
void UninitEndmessage(void)
{
	if (g_pTextureEndmessage != NULL)
	{
		g_pTextureEndmessage->Release();
		g_pTextureEndmessage = NULL;
	}


	//頂点バッファの破棄
	if (g_pVtxBuffEndmessage != NULL)
	{
		g_pVtxBuffEndmessage->Release();
		g_pVtxBuffEndmessage = NULL;
	}
}

//==============================================================
//終わる少し前に出すメッセージの更新処理
//==============================================================
void UpdateEndmessage(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEndmessage->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aEndmessage.bUse == true)
	{//使用しているとき

		g_aEndmessage.nCounter--;    //カウンターを減算

		if (g_aEndmessage.nCounter <= 0)
		{//カウンターが0以下のとき

			g_aEndmessage.col.a -= 0.08f;

			if (g_aEndmessage.col.a <= 0.0f)
			{
				g_aEndmessage.bUse = false;    //使用していない状態にする

				g_aEndmessage.col.a = 0.0f;    //透明度を0にする

				g_aEndmessage.nCntZoom = 0;

				g_aEndmessage.fHeight = 0.0f;
				g_aEndmessage.fWidth = 0.0f;
			}
		}

		g_aEndmessage.col.a += 0.005f;
		g_aEndmessage.fHeight += NUMBER_HEIGHT * 0.005f;
		g_aEndmessage.fWidth += NUMBER_WIDTH * 0.005f;

		if (g_aEndmessage.col.a >= 0.6f)
		{//透明度が0.5以上のとき

			g_aEndmessage.col.a = 0.6f;      //透明度を0.5にする

			
				g_aEndmessage.nCntZoom++;
			
			
			//if (g_aEndmessage.nCntZoom <= COUNTZOOM)
			{
				g_aEndmessage.fHeight += NUMBER_HEIGHT * 0.3f;
				g_aEndmessage.fWidth += NUMBER_WIDTH * 0.3f;
			}
		}

		/*if (g_aEndmessage.nCntZoom >= COUNTZOOM && g_aEndmessage.fWidth >= -20.0f && g_aEndmessage.fHeight >= -20.0f)
		{
				g_aEndmessage.fHeight -= NUMBER_HEIGHT * 0.01f;
				g_aEndmessage.fWidth -= NUMBER_WIDTH * 0.01f;
		}*/



		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEndmessage.pos.x - NUMBER_WIDTH - g_aEndmessage.fWidth, g_aEndmessage.pos.y - NUMBER_HEIGHT - g_aEndmessage.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEndmessage.pos.x + NUMBER_WIDTH + g_aEndmessage.fWidth, g_aEndmessage.pos.y - NUMBER_HEIGHT - g_aEndmessage.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEndmessage.pos.x - NUMBER_WIDTH - g_aEndmessage.fWidth, g_aEndmessage.pos.y + NUMBER_HEIGHT + g_aEndmessage.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEndmessage.pos.x + NUMBER_WIDTH + g_aEndmessage.fWidth, g_aEndmessage.pos.y + NUMBER_HEIGHT + g_aEndmessage.fHeight, 0.0f);

		//頂点カラー
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndmessage.col.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndmessage.col.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndmessage.col.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndmessage.col.a);

		pVtx += 4;     //頂点データのポインタを４つ分集める
	}
	
	

	//頂点バッファをアンロックする
	g_pVtxBuffEndmessage->Unlock();
}

//==============================================================
//終わる少し前に出すメッセージの描画処理
//==============================================================
void DrawEndmessage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ

													//アルファテストを有効にする
	/*pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);*/

	////Zテストを無効にする
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEndmessage, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aEndmessage.bUse == true)
	{
		//テクスチャの設定
		pDevice->SetTexture(0, g_pTextureEndmessage);

		//ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
			0,			                                    //プリミティブ（ポリゴンの数）
			2);												//描画するプリミティブ数
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
//終わる少し前に出すメッセージの情報
//==============================================================
void SetEndmessage(D3DXVECTOR3 pos, int nCounter)
{
	if (g_aEndmessage.bUse == false)
	{//使用していなかったら

		//使用している状態にする
		g_aEndmessage.bUse = true;

		g_aEndmessage.pos = pos;
		g_aEndmessage.nCounter = nCounter;

		//ｺｹｺｯｺｰ
		PlaySound(SOUND_LABEL_SE_KOKE);
	}
}

//==============================================================
//終わる少し前に出すメッセージの情報
//==============================================================
Endmessage *GetEndmessage(void)
{
	return &g_aEndmessage;
}