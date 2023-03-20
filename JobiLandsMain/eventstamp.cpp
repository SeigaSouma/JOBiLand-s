//=============================================================================
//
// イベントスタンプ処理 [EventStamp.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "eventstamp.h"
#include "input.h"
#include "particle.h"
#include "sound.h"

#define WINDOW_X	(100.0f)
#define WINDOW_Y	(60.0f)
#define POS_Y		(100.0f)
#define DIS_Y		(140.0f)

//グローバル変数宣言
const char *c_apFilenameEventStamp[] =	//ファイル読み込み
{
	"data\\TEXTURE\\tairyo_05.png",		//大量発生
	"data\\TEXTURE\\security_05.png",	//警備強化
};

LPDIRECT3DTEXTURE9 g_pTextureEventStamp[(sizeof c_apFilenameEventStamp) / sizeof(*c_apFilenameEventStamp)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEventStamp = NULL;		//頂点バッファへのポインタ
EVENTSTAMP g_EventStamp[MAX_EVENTSTAMP];			//文章

//=============================================================================
//イベントスタンプの初期化処理
//=============================================================================					   
void InitEventStamp(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameEventStamp) / sizeof(*c_apFilenameEventStamp); CntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEventStamp[CntTex],
			&g_pTextureEventStamp[CntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EVENTSTAMP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEventStamp,
		NULL);

	//各変数初期化
	for (int nCntStamp = 0; nCntStamp < MAX_EVENTSTAMP; nCntStamp++)
	{
		g_EventStamp[nCntStamp].bUse = false;		//使用しているか
		g_EventStamp[nCntStamp].nType = 0;		//種類

		//UI情報
		g_EventStamp[nCntStamp].aUI.pos = D3DXVECTOR3(1100.0f, POS_Y, 0.0f);	//頂点座標
		g_EventStamp[nCntStamp].aUI.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//頂点カラー
		g_EventStamp[nCntStamp].aUI.fWidth = WINDOW_X;		//幅
		g_EventStamp[nCntStamp].aUI.fHeight = WINDOW_Y;		//高さ
		g_EventStamp[nCntStamp].aUI.nPatternAnim = 0;	//アニメーションパターンNo
		g_EventStamp[nCntStamp].aUI.nCntAnim = 0;		//アニメーションカウンター
		g_EventStamp[nCntStamp].aUI.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
		g_EventStamp[nCntStamp].aUI.fAngle = 0.0f;		//角度
		g_EventStamp[nCntStamp].aUI.fLength = 0.0f;		//対角線の長さ
		g_EventStamp[nCntStamp].aUI.nCount = 0;			//雑カウント
		g_EventStamp[nCntStamp].aUI.bUse = false;			//使用しているか
		g_EventStamp[nCntStamp].aUI.aTexU = 0.0f;		//テクスチャ座標U[スクロール用]
		g_EventStamp[nCntStamp].aUI.aTexV = 0.0f;		//テクスチャ座標V[スクロール用]
	}

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEventStamp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStamp = 0; nCntStamp < MAX_EVENTSTAMP; nCntStamp++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x - g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y - g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x + g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y - g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x - g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y + g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x + g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y + g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = g_EventStamp[nCntStamp].aUI.col;
		pVtx[1].col = g_EventStamp[nCntStamp].aUI.col;
		pVtx[2].col = g_EventStamp[nCntStamp].aUI.col;
		pVtx[3].col = g_EventStamp[nCntStamp].aUI.col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEventStamp->Unlock();
}

//=============================================================================
//イベントスタンプの終了処理
//=============================================================================
void UninitEventStamp(void)
{
	StopSound();

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameEventStamp) / sizeof(*c_apFilenameEventStamp); CntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureEventStamp[CntTex] != NULL)
		{
			g_pTextureEventStamp[CntTex]->Release();
			g_pTextureEventStamp[CntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEventStamp != NULL)
	{
		g_pVtxBuffEventStamp->Release();
		g_pVtxBuffEventStamp = NULL;
	}
}

//=============================================================================
//イベントスタンプの更新処理
//=============================================================================
void UpdateEventStamp(void)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEventStamp->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntStamp = 0, nCntUse = 0; nCntStamp < MAX_EVENTSTAMP; nCntStamp++)
	{
		if (g_EventStamp[nCntStamp].bUse == true)
		{//使用されていたら

			//位置更新
			g_EventStamp[nCntStamp].aUI.pos.y += (POS_Y + (nCntUse * DIS_Y) - g_EventStamp[nCntStamp].aUI.pos.y) * 0.25f;
			g_EventStamp[nCntStamp].aUI.fWidth += (WINDOW_X - g_EventStamp[nCntStamp].aUI.fWidth) * 0.25f;
			g_EventStamp[nCntStamp].aUI.fHeight += (WINDOW_Y - g_EventStamp[nCntStamp].aUI.fHeight) * 0.25f;

			//カウント加算
			g_EventStamp[nCntStamp].aUI.nCount++;

			if (g_EventStamp[nCntStamp].aUI.nCount == 10)
			{
				SetParticle(g_EventStamp[nCntStamp].aUI.pos, PARTICLE_TYPE_MANYTXT_MINI);

				PlaySound(SOUND_LABEL_SE_EVENTTEXT);
			}
			else if (g_EventStamp[nCntStamp].aUI.nCount > 10)
			{
				g_EventStamp[nCntStamp].aUI.nCount = 11;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x - g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y - g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x + g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y - g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x - g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y + g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x + g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y + g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
		
			nCntUse++;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEventStamp->Unlock();

}

//=============================================================================
//イベントスタンプの描画処理
//=============================================================================
void DrawEventStamp(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffEventStamp, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntStamp = 0; nCntStamp < MAX_EVENTSTAMP; nCntStamp++)
	{
		if (g_EventStamp[nCntStamp].bUse == true)
		{//使用されていたら

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEventStamp[g_EventStamp[nCntStamp].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStamp * 4, 2);
		}
	}
}

//=============================================================================
//イベントスタンプの設定処理
//=============================================================================
void SetEventStamp(int nType)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEventStamp->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_EventStamp[nType].bUse == false)
	{//使用されていなかったら

		pVtx += 4 * nType;

		g_EventStamp[nType].bUse = true;
		g_EventStamp[nType].nType = nType;
		g_EventStamp[nType].aUI.pos = D3DXVECTOR3(1100.0f, 300.0f, 0.0f);
		g_EventStamp[nType].aUI.fWidth = WINDOW_X * 10.0f;
		g_EventStamp[nType].aUI.fHeight = WINDOW_Y * 10.0f;
		g_EventStamp[nType].aUI.nCount = 0;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_EventStamp[nType].aUI.pos.x - g_EventStamp[nType].aUI.fWidth, g_EventStamp[nType].aUI.pos.y - g_EventStamp[nType].aUI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_EventStamp[nType].aUI.pos.x + g_EventStamp[nType].aUI.fWidth, g_EventStamp[nType].aUI.pos.y - g_EventStamp[nType].aUI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_EventStamp[nType].aUI.pos.x - g_EventStamp[nType].aUI.fWidth, g_EventStamp[nType].aUI.pos.y + g_EventStamp[nType].aUI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_EventStamp[nType].aUI.pos.x + g_EventStamp[nType].aUI.fWidth, g_EventStamp[nType].aUI.pos.y + g_EventStamp[nType].aUI.fHeight, 0.0f);
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEventStamp->Unlock();
}

//=============================================================================
//イベントスタンプの情報取得
//=============================================================================
EVENTSTAMP *GetEventStamp(void)
{
	return &g_EventStamp[0];
}