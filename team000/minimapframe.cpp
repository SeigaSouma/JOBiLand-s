//==============================================================
//
//イベントサインの下地[minimapframe.cpp]
//Author:大原怜将
//
//==============================================================
#include "minimapframe.h"

//マクロ定義
#define NUM_RADAR			(2)			//ミニマップフレームの種類
#define NUMBER_WIDTH		(183.0f)	//幅
#define NUMBER_HEIGHT		(185.0f)	//高さ

////グローバル変数宣言g_aMinimapframe
//const char *c_apFilenameminimapframe[] =	//ファイル読み込み
//{
//	"data\\TEXTURE\\radar1.png",	//大量発生なし
//	//"data\\TEXTURE\\radar2.png",	//大量発生中
//};

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureminimapframe = NULL;	//テクスチャ1へのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffminimapframe = NULL;		//頂点バッファへのポインタ
Minimapframe g_aMinimapframe;

//==============================================================
//ミニマップフレームの初期化処理
//==============================================================
void InitMinimapframe(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
	VERTEX_2D *pVtx;								//頂点情報へのポインタ

	//テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\minimapframe1.png",
		&g_pTextureminimapframe);

	//初期化
	
	g_aMinimapframe.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //位置を初期化する
	g_aMinimapframe.fWidth = NUMBER_WIDTH;				   //幅を初期化する
	g_aMinimapframe.fHeight = NUMBER_HEIGHT;			   //高さを初期化する
	
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffminimapframe,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffminimapframe->Lock(0, 0, (void**)&pVtx, 0);

	
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aMinimapframe.pos.x - NUMBER_WIDTH, g_aMinimapframe.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aMinimapframe.pos.x + NUMBER_WIDTH, g_aMinimapframe.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aMinimapframe.pos.x - NUMBER_WIDTH, g_aMinimapframe.pos.y + NUMBER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aMinimapframe.pos.x + NUMBER_WIDTH, g_aMinimapframe.pos.y + NUMBER_HEIGHT, 0.0f);

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
	g_pVtxBuffminimapframe->Unlock();
}

//==============================================================
//ミニマップフレームの終了処理
//==============================================================
void UninitMinimapframe(void)
{
	if (g_pTextureminimapframe != NULL)
	{
		g_pTextureminimapframe->Release();
		g_pTextureminimapframe = NULL;
	}
	

	//頂点バッファの破棄
	if (g_pVtxBuffminimapframe != NULL)
	{
		g_pVtxBuffminimapframe->Release();
		g_pVtxBuffminimapframe = NULL;
	}
}

//==============================================================
//ミニマップフレームの更新処理
//==============================================================
void UpdateMinimapframe(void)
{
	VERTEX_2D *pVtx;				//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffminimapframe->Lock(0, 0, (void**)&pVtx, 0);

	g_aMinimapframe.pos = D3DXVECTOR3(119.0f, 720.0f - 145.0f, 0.0f);	//位置

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(g_aMinimapframe.pos.x - NUMBER_WIDTH, g_aMinimapframe.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aMinimapframe.pos.x + NUMBER_WIDTH, g_aMinimapframe.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aMinimapframe.pos.x - NUMBER_WIDTH, g_aMinimapframe.pos.y + NUMBER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aMinimapframe.pos.x + NUMBER_WIDTH, g_aMinimapframe.pos.y + NUMBER_HEIGHT, 0.0f);
	
	pVtx += 4;     //頂点データのポインタを４つ分集める
	
	//頂点バッファをアンロックする
	g_pVtxBuffminimapframe->Unlock();
}

//==============================================================
//ミニマップフレームの描画処理
//==============================================================
void DrawMinimapframe(void)
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
	pDevice->SetStreamSource(0, g_pVtxBuffminimapframe, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureminimapframe);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,			                                    //プリミティブ（ポリゴンの数）
		2);												//描画するプリミティブ数
	
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
Minimapframe *GetMinimapframe(void)
{
	return &g_aMinimapframe;
}