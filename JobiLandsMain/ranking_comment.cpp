//=============================================================================
//
// ランキングコメント処理 [ranking_comment.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "ranking_comment.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
#include "updown_fram.h"
#include "enemy.h"

//マクロ定義
#define MAX_TOP			(4)		//頂点数
#define TEX_V		(0.1f)		//V座標
#define SPAWNCOUNTER		(40)
#define MOVE		(5.0f)
#define WIDTH		(400.0f * 0.45f)
#define HEIGHT		(30.0f * 0.45f)
#define WINDOW_WIDTH		(640.0f)
#define WINDOW_HEIGHT		(360.0f)
#define DIS_Y		(50.0f)

//プロトタイプ宣言
void CreatVtxTextRankingComment(void);
void CreatVtxWindowRankingComment(void);
void DrawRankingCommentBuff(void);
void DrawCommentWindowBuff(void);
void UpdateTextRankingComment(void);

//グローバル変数宣言
const char *c_apFilenameRankingComment[] =		//ファイル読み込み
{
	"data\\TEXTURE\\comment_01.png",
	"data\\TEXTURE\\comment_02.png",
	"data\\TEXTURE\\comment_03.png",
	"data\\TEXTURE\\comment_04.png",
	"data\\TEXTURE\\comment_05.png",
	"data\\TEXTURE\\comment_06.png",
	"data\\TEXTURE\\comment_07.png",
	"data\\TEXTURE\\comment_08.png",
	"data\\TEXTURE\\comment_09.png",
	"data\\TEXTURE\\comment_10.png",
};

const char *c_apFilenameCommentWindow[] =		//ファイル読み込み
{
	"data\\TEXTURE\\commentwindow_01.png",
};
LPDIRECT3DTEXTURE9 g_pTextureRankingComment[(sizeof c_apFilenameRankingComment) / sizeof(*c_apFilenameRankingComment)] = {};			//テクスチャへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureCommentWindow[(sizeof c_apFilenameCommentWindow) / sizeof(*c_apFilenameCommentWindow)] = {};			//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingComment = NULL;	//頂点バッファへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCommentWindow = NULL;	//頂点バッファへのポインタ
RankingComment g_aRankingComment;		//ランキングコメントの情報

//==================================================================================
//ランキングコメントの初期化処理
//==================================================================================
void InitRankingComment(void)
{

	//各変数初期化
	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		g_aRankingComment.aInfo[nCntCommnet].bUse = false;
		g_aRankingComment.aInfo[nCntCommnet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRankingComment.aInfo[nCntCommnet].pos = D3DXVECTOR3(1080.0f, 0.0f, 0.0f);
		g_aRankingComment.aInfo[nCntCommnet].nTexV = 0;	//種類
		g_aRankingComment.aInfo[nCntCommnet].nType = 0;	//テクスチャの何分割目か
		g_aRankingComment.aInfo[nCntCommnet].nNumIndex = 0;	//インデックス番号
	}
	g_aRankingComment.nNumSpawn = 0;	//出現した回数

	//会話の頂点バッファ生成
	CreatVtxTextRankingComment();

	//ウィンドウの頂点バッファ生成
	CreatVtxWindowRankingComment();
}

//==================================================================================
//ランキングコメントの終了処理
//==================================================================================
void UninitRankingComment(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRankingComment) / sizeof(*c_apFilenameRankingComment); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureRankingComment[nCntTex] != NULL)
		{
			g_pTextureRankingComment[nCntTex]->Release();
			g_pTextureRankingComment[nCntTex] = NULL;
		}
	}

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCommentWindow) / sizeof(*c_apFilenameCommentWindow); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureCommentWindow[nCntTex] != NULL)
		{
			g_pTextureCommentWindow[nCntTex]->Release();
			g_pTextureCommentWindow[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffRankingComment != NULL)
	{
		g_pVtxBuffRankingComment->Release();
		g_pVtxBuffRankingComment = NULL;
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCommentWindow != NULL)
	{
		g_pVtxBuffCommentWindow->Release();
		g_pVtxBuffCommentWindow = NULL;
	}
}

//==================================================================================
//ランキングコメントの更新処理
//==================================================================================
void UpdateRankingComment(void)
{
	UpdateTextRankingComment();
}

//==================================================================================
//ランキングコメントのテキスト更新処理
//==================================================================================
void UpdateTextRankingComment(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingComment->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aRankingComment.aInfo[nCntCommnet].bUse == true)
		{//使用されていたら

			//上に移動させる
			g_aRankingComment.aInfo[nCntCommnet].pos.y -= 10.0f;

			//流れる処理
			if ((700.0f + (g_aRankingComment.aInfo[nCntCommnet].nNumIndex) * DIS_Y) - DIS_Y * g_aRankingComment.nNumSpawn > g_aRankingComment.aInfo[nCntCommnet].pos.y)
			{
				g_aRankingComment.aInfo[nCntCommnet].pos.y = (700.0f + (g_aRankingComment.aInfo[nCntCommnet].nNumIndex) * DIS_Y) - DIS_Y * g_aRankingComment.nNumSpawn;
			}

			if (g_aRankingComment.aInfo[nCntCommnet].pos.y <= -150.0f)
			{
				g_aRankingComment.aInfo[nCntCommnet].bUse = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRankingComment->Unlock();
}

//==================================================================================
//ランキングコメントの描画処理
//==================================================================================
void DrawRankingComment(void)
{

	//ウィンドウ描画処理
	DrawCommentWindowBuff();

	//テキスト描画処理
	DrawRankingCommentBuff();

}

//==================================================================================
//ランキングコメントのテキスト描画処理
//==================================================================================
void DrawRankingCommentBuff(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffRankingComment, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テキストの描画
	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aRankingComment.aInfo[nCntCommnet].bUse == true)
		{//使用されていたら

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureRankingComment[g_aRankingComment.aInfo[nCntCommnet].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCommnet * 4, 2);
		}
	}


	//Zテストを有効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//==================================================================================
//ランキングコメントのウィンドウ描画処理
//==================================================================================
void DrawCommentWindowBuff(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 250);

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffCommentWindow, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, g_pTextureCommentWindow[0]);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

}

//==================================================================================
//コメント設定処理
//==================================================================================
void SetRankingComment(int nTexV, int nType)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingComment->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aRankingComment.aInfo[nCntCommnet].bUse == false)
		{//使用していなかったら

			//出現高さランダム
			g_aRankingComment.aInfo[nCntCommnet].pos.y = 700.0f;

			g_aRankingComment.aInfo[nCntCommnet].nTexV = nTexV;
			g_aRankingComment.aInfo[nCntCommnet].nType = nType;

			//使用している状態にする
			g_aRankingComment.aInfo[nCntCommnet].bUse = true;

			//インデックス番号取得
			g_aRankingComment.aInfo[nCntCommnet].nNumIndex = g_aRankingComment.nNumSpawn;

			//出現した回数加算
			g_aRankingComment.nNumSpawn++;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);

			//テクスチャ座標の設定
			pVtx[0].tex = D3DXVECTOR2(0.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V);
			pVtx[1].tex = D3DXVECTOR2(1.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
			pVtx[3].tex = D3DXVECTOR2(1.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
			break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRankingComment->Unlock();
}

//==================================================================================
//テキストの頂点バッファ生成
//==================================================================================
void CreatVtxTextRankingComment(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRankingComment) / sizeof(*c_apFilenameRankingComment); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameRankingComment[nCntTex],
			&g_pTextureRankingComment[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * MAX_NICOCOME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingComment,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffRankingComment->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffRankingComment->Unlock();
}

//==================================================================================
//ウィンドウの頂点バッファ生成
//==================================================================================
void CreatVtxWindowRankingComment(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCommentWindow) / sizeof(*c_apFilenameCommentWindow); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCommentWindow[nCntTex],
			&g_pTextureCommentWindow[nCntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCommentWindow,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCommentWindow->Lock(0, 0, (void**)&pVtx, 0);
	
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(640.0f - WINDOW_WIDTH, 360.0f - WINDOW_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(640.0f + WINDOW_WIDTH, 360.0f - WINDOW_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(640.0f - WINDOW_WIDTH, 360.0f + WINDOW_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(640.0f + WINDOW_WIDTH, 360.0f + WINDOW_HEIGHT, 0.0f);

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
	g_pVtxBuffCommentWindow->Unlock();
}

//==================================================================================
//ランキングコメント情報取得
//==================================================================================
RankingComment *GetRankingComment(void)
{
	return &g_aRankingComment;
}