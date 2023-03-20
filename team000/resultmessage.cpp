//=============================================================================
//
// 結果のメッセージ処理 [resultmessage.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "input.h"
#include "light.h"
#include "resultmessage.h"
#include "2D_effect.h"
#include "particle.h"
#include "sound.h"

#define NUM_PLACE	(2)
#define MOVE_TIME	(20)
#define TEXT_X	(360.0f * 1.2f)
#define TEXT_Y	(130.0f * 1.2f)

//グローバル変数宣言
const char *c_apFilenameResultMessage[] =	//ファイル読み込み
{
	"data\\TEXTURE\\resultclear_01.png",	//反逆
	"data\\TEXTURE\\resultclear_02.png",	//成功
	"data\\TEXTURE\\resultfailed_01.png",	//反逆
	"data\\TEXTURE\\resultfailed_02.png",	//失敗
};

LPDIRECT3DTEXTURE9 g_pTextureResultMessage[(sizeof c_apFilenameResultMessage) / sizeof(*c_apFilenameResultMessage)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultMessage = NULL;		//頂点バッファへのポインタ
RESULT_MESSAGE g_aResultMessage[NUM_PLACE];					//文章
int g_nCntAfterResultMessage;

//=============================================================================
//結果のメッセージ初期化処理
//=============================================================================					   
void InitResultMessage(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャの数分読み込む
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameResultMessage) / sizeof(*c_apFilenameResultMessage); CntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameResultMessage[CntTex],
			&g_pTextureResultMessage[CntTex]);
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultMessage,
		NULL);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		g_aResultMessage[nCntTxt].pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//位置
		g_aResultMessage[nCntTxt].posOrigin = g_aResultMessage[nCntTxt].pos;	//位置
		g_aResultMessage[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色
		g_aResultMessage[nCntTxt].nType = 0;			//種類
		g_aResultMessage[nCntTxt].fWidth = 0.0f;		//幅
		g_aResultMessage[nCntTxt].fHeight = 0.0f;		//高さ
		g_aResultMessage[nCntTxt].nCntZoom = 0;		//大きくなる時間
		g_aResultMessage[nCntTxt].bUse = false;			//使用しているか
	}
	g_nCntAfterResultMessage = 0;

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - TEXT_X, g_aResultMessage[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + TEXT_X, g_aResultMessage[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - TEXT_X, g_aResultMessage[nCntTxt].pos.y + TEXT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + TEXT_X, g_aResultMessage[nCntTxt].pos.y + TEXT_Y, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラー
		pVtx[0].col = g_aResultMessage[nCntTxt].col;
		pVtx[1].col = g_aResultMessage[nCntTxt].col;
		pVtx[2].col = g_aResultMessage[nCntTxt].col;
		pVtx[3].col = g_aResultMessage[nCntTxt].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultMessage->Unlock();
}

//=============================================================================
//結果のメッセージ終了処理
//=============================================================================
void UninitResultMessage(void)
{

	//SEを終了
	StopSound();

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameResultMessage) / sizeof(*c_apFilenameResultMessage); CntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureResultMessage[CntTex] != NULL)
		{
			g_pTextureResultMessage[CntTex]->Release();
			g_pTextureResultMessage[CntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffResultMessage != NULL)
	{
		g_pVtxBuffResultMessage->Release();
		g_pVtxBuffResultMessage = NULL;
	}
}

//=============================================================================
//結果のメッセージ更新処理
//=============================================================================
void UpdateResultMessage(void)
{

	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aResultMessage[nCntTxt].bUse == true)
		{//使用されていたら

			//拡大の時間加算
			g_aResultMessage[nCntTxt].nCntZoom++;

			//補正して移動
			g_aResultMessage[nCntTxt].pos.x += (g_aResultMessage[nCntTxt].posOrigin.x - g_aResultMessage[nCntTxt].pos.x) * 0.25f;
			g_aResultMessage[nCntTxt].pos.y += (g_aResultMessage[nCntTxt].posOrigin.y - g_aResultMessage[nCntTxt].pos.y) * 0.05f;
			g_aResultMessage[nCntTxt].col.a += (1.0f - g_aResultMessage[nCntTxt].col.a) * 0.05f;

			if (g_aResultMessage[nCntTxt].nCntZoom == 10 && nCntTxt == 0 && g_aResultMessage[nCntTxt].nType == RESULTTYPE_CLEAR)
			{//着きそうなときにパーティクル

				SetParticle(g_aResultMessage[nCntTxt].posOrigin, PARTICLE_TYPE_RESULTMESSAGE);

				//クリア音
				PlaySound(SOUND_LABEL_SE_GAMECLEAR);
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y - g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y - g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y + g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y + g_aResultMessage[nCntTxt].fHeight, 0.0f);

			//頂点カラー
			pVtx[0].col = g_aResultMessage[nCntTxt].col;
			pVtx[1].col = g_aResultMessage[nCntTxt].col;
			pVtx[2].col = g_aResultMessage[nCntTxt].col;
			pVtx[3].col = g_aResultMessage[nCntTxt].col;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultMessage->Unlock();

}
//=============================================================================
//結果のメッセージ描画処理
//=============================================================================
void DrawResultMessage(void)
{
	//デバイスのポインタ取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, g_pVtxBuffResultMessage, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aResultMessage[nCntTxt].bUse == true)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureResultMessage[(g_aResultMessage[nCntTxt].nType * NUM_PLACE) + nCntTxt]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTxt * 4, 2);
		}
	}

	if (g_aResultMessage[0].bUse == true)
	{
		//2Dエフェクトの描画処理
		DrawEffect_2D();
	}

}

//=============================================================================
//結果のメッセージ設定処理
//=============================================================================
void SetResultMessage(int nType)
{
	VERTEX_2D *pVtx;

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aResultMessage[nCntTxt].bUse == false)
		{//使用されていなかったら

			g_aResultMessage[nCntTxt].bUse = true;

			if (nType == RESULTTYPE_CLEAR)
			{
				g_aResultMessage[nCntTxt].pos = D3DXVECTOR3(-600.0f + nCntTxt * 1880.0f, 360.0f, 0.0f);	//位置
				g_aResultMessage[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色

			}
			else
			{
				g_aResultMessage[nCntTxt].pos = D3DXVECTOR3(640.0f, 400.0f, 0.0f);	//位置
				g_aResultMessage[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		//色

				//げーむおーばー音
				PlaySound(SOUND_LABEL_SE_GAMEFAILED);
			}
			g_aResultMessage[nCntTxt].fWidth = TEXT_X;
			g_aResultMessage[nCntTxt].fHeight = TEXT_Y;
			g_aResultMessage[nCntTxt].nCntZoom = 0;
			g_nCntAfterResultMessage = 0;
			g_aResultMessage[nCntTxt].nType = nType;	//イベントの種類

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y - g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y - g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y + g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y + g_aResultMessage[nCntTxt].fHeight, 0.0f);

			//頂点カラー
			pVtx[0].col = g_aResultMessage[nCntTxt].col;
			pVtx[1].col = g_aResultMessage[nCntTxt].col;
			pVtx[2].col = g_aResultMessage[nCntTxt].col;
			pVtx[3].col = g_aResultMessage[nCntTxt].col;
			//break;
		}

		pVtx += 4;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffResultMessage->Unlock();
}

//=============================================================================
//結果のメッセージ情報取得
//=============================================================================
RESULT_MESSAGE *GetResultMessage(void)
{
	return &g_aResultMessage[0];
}