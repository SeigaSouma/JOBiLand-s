//=============================================================================
//
// エフェクト処理 [2D_effect.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "2D_effect.h"
#include "effect.h"
#include "input.h"
#include "particle.h"
#include "camera.h"

//マクロ定義
#define MAX_TOP			(4)		//頂点数
#define MAX_EFFECT_2D		(12800)	//エフェクトの最大数
#define EFFECT_2DSIZE1		(0.97f)
#define EFFECT_2DSIZE2		(0.98f)
#define EFFECT_2DSIZE3		(0.99f)


//プロトタイプ宣言
void UpdateSubEffect_2D(int nCntEffect);
void UpdateSuperSubEffect_2D(int nCntEffect);
void UpdateAddEffect_2D(int nCntEffect);

//グローバル変数宣言
const char *c_apFilenameEffect_2D[] =		//ファイル読み込み
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect002.jpg",
	"data\\TEXTURE\\smoke_05.tga",
	"data\\TEXTURE\\effect000.png",
	"data\\TEXTURE\\hart_01.png",
	"data\\TEXTURE\\effect001.png",
};

LPDIRECT3DTEXTURE9 g_pTextureEffect_2D[(sizeof c_apFilenameEffect_2D) / sizeof(*c_apFilenameEffect_2D)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect_2D = NULL;	//頂点バッファへのポインタ
Effect_2D g_aEffect_2D[MAX_EFFECT_2D];						//エフェクトの情報
int g_Effect_2DNum;									//エフェクトの数
bool g_bEffect_2DUse;

//==================================================================================
//エフェクトの初期化処理
//==================================================================================
void InitEffect_2D(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameEffect_2D) / sizeof(*c_apFilenameEffect_2D); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEffect_2D[nCntTex],
			&g_pTextureEffect_2D[nCntTex]);

	}


	//エフェクトの情報の初期化
	for (int nCntEffect =0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		g_aEffect_2D[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
		g_aEffect_2D[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
		g_aEffect_2D[nCntEffect].mtxWorld = {};	//ワールドマトリックス
		g_aEffect_2D[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//色
		g_aEffect_2D[nCntEffect].fRadius = 0.0f;		//半径
		g_aEffect_2D[nCntEffect].fMaxRadius = 0.0f;	//最大半径
		g_aEffect_2D[nCntEffect].nLife = EFFECT_2D_LIFE;			//寿命
		g_aEffect_2D[nCntEffect].nMaxLife = EFFECT_2D_LIFE;		//最大寿命(固定)
		g_aEffect_2D[nCntEffect].moveType = 0;		//移動の種類
		g_aEffect_2D[nCntEffect].nType = 0;			//種類
		g_aEffect_2D[nCntEffect].bUse = false;			//使用しているかどうか
		g_aEffect_2D[nCntEffect].bAddAlpha = false;		//加算合成の判定
	}
	g_Effect_2DNum = 0;
	g_bEffect_2DUse = true;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * MAX_EFFECT_2D,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffEffect_2D,
								NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect_2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);

		//rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[0].col = g_aEffect_2D[nCntEffect].col;
		pVtx[1].col = g_aEffect_2D[nCntEffect].col;
		pVtx[2].col = g_aEffect_2D[nCntEffect].col;
		pVtx[3].col = g_aEffect_2D[nCntEffect].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect_2D->Unlock();
}

//==================================================================================
//エフェクトの終了処理
//==================================================================================
void UninitEffect_2D(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameEffect_2D) / sizeof(*c_apFilenameEffect_2D); nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureEffect_2D[nCount] != NULL)
		{
			g_pTextureEffect_2D[nCount]->Release();
			g_pTextureEffect_2D[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffEffect_2D != NULL)
	{
		g_pVtxBuffEffect_2D->Release();
		g_pVtxBuffEffect_2D = NULL;
	}
}

//==================================================================================
//エフェクトの更新処理
//==================================================================================
void UpdateEffect_2D(void)
{

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect_2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		if (g_aEffect_2D[nCntEffect].bUse == true)
		{//エフェクトが使用されていたら

			//頂点座標の更新(小さくする)
			if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK)
			{//黒煙だった場合

				if (((float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife) <= 0.5f)
				{//最大体力の半分以下

					g_aEffect_2D[nCntEffect].pos += g_aEffect_2D[nCntEffect].move * (((float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife));
				}
				else
				{//半分より上

					g_aEffect_2D[nCntEffect].pos += g_aEffect_2D[nCntEffect].move * (((float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife));
				}
			}
			else 
			{//黒煙以外

				//位置更新
				g_aEffect_2D[nCntEffect].pos += g_aEffect_2D[nCntEffect].move;
			}
			

			//小さくしていく処理の場合
			if (g_aEffect_2D[nCntEffect].moveType == MOVEEFFECT_SUB)
			{//エフェクトを小さくしていく

				if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_NORMAL || g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKE ||
					g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK || g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_BLACK)
				{
					//縮小処理
					UpdateSubEffect_2D(nCntEffect);
				}
			}
			else if (g_aEffect_2D[nCntEffect].moveType == MOVEEFFECT_SUPERSUB)
			{//エフェクトを小さくしていく

				UpdateSuperSubEffect_2D(nCntEffect);
			}
			else if (g_aEffect_2D[nCntEffect].moveType == MOVEEFFECT_ADD)
			{//エフェクトを大きくしていく

				//拡大処理
				UpdateAddEffect_2D(nCntEffect);
			}
			else if (g_aEffect_2D[nCntEffect].moveType == MOVEEFFECT_NONE)
			{//変化なし

			}


			//寿命の更新
			g_aEffect_2D[nCntEffect].nLife--;

			//不透明度の更新
			g_aEffect_2D[nCntEffect].col.a = (float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife;

			if (g_aEffect_2D[nCntEffect].nLife <= 0)
			{//寿命が尽きたら

				g_aEffect_2D[nCntEffect].bUse = false;
				g_aEffect_2D[nCntEffect].bAddAlpha = false;
				g_Effect_2DNum--;
			}

			//頂点カラーの設定
			pVtx[0].col = g_aEffect_2D[nCntEffect].col;
			pVtx[1].col = g_aEffect_2D[nCntEffect].col;
			pVtx[2].col = g_aEffect_2D[nCntEffect].col;
			pVtx[3].col = g_aEffect_2D[nCntEffect].col;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);

		}

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffEffect_2D->Unlock();
}

//==================================================================================
//エフェクトの縮小処理
//==================================================================================
void UpdateSubEffect_2D(int nCntEffect)
{

	int nEffect_2DType = rand() % 3;

	if (nEffect_2DType == 0)
	{
		g_aEffect_2D[nCntEffect].fRadius *= EFFECT_2DSIZE1;
	}
	else if (nEffect_2DType == 1)
	{
		g_aEffect_2D[nCntEffect].fRadius *= EFFECT_2DSIZE2;
	}
	else if (nEffect_2DType == 2)
	{
		g_aEffect_2D[nCntEffect].fRadius *= EFFECT_2DSIZE3;
	}

	//g_aEffect_2D[nCntEffect].fRadius = g_aEffect_2D[nCntEffect].fMaxRadius * (float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife;

}

//==================================================================================
//エフェクトの縮小処理
//==================================================================================
void UpdateSuperSubEffect_2D(int nCntEffect)
{

	g_aEffect_2D[nCntEffect].fRadius = g_aEffect_2D[nCntEffect].fMaxRadius * (float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife;
}

//==================================================================================
//エフェクトの拡大処理
//==================================================================================
void UpdateAddEffect_2D(int nCntEffect)
{

	int nEffect_2DType = rand() % 3;

	if (nEffect_2DType == 0)
	{
		g_aEffect_2D[nCntEffect].fRadius += 1.8f;
	}
	else if (nEffect_2DType == 1)
	{
		g_aEffect_2D[nCntEffect].fRadius += 0.8f;
	}
	else if (nEffect_2DType == 2)
	{
		g_aEffect_2D[nCntEffect].fRadius += 0.0f;
	}

}

//==================================================================================
//エフェクトの描画処理
//==================================================================================
void DrawEffect_2D(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ポリゴンの描画
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		if (g_aEffect_2D[nCntEffect].bUse == true && g_bEffect_2DUse == true)
		{//エフェクトが使用されている

			//αブレンディングを加算合成に設定
			if (g_aEffect_2D[nCntEffect].bAddAlpha == false)
			{
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffEffect_2D, 0, sizeof(VERTEX_2D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_2D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureEffect_2D[g_aEffect_2D[nCntEffect].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * MAX_TOP, 2);

			//αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//==================================================================================
//エフェクトの設定処理
//==================================================================================
void SetEffect_2D(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType)
{

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffEffect_2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		if (g_aEffect_2D[nCntEffect].bUse == false)
		{//エフェクトが使用されていなかったら

			g_aEffect_2D[nCntEffect].pos = pos;
			g_aEffect_2D[nCntEffect].move = move;
			g_aEffect_2D[nCntEffect].col = col;
			g_aEffect_2D[nCntEffect].fRadius = fRadius;
			g_aEffect_2D[nCntEffect].fMaxRadius = fRadius;
			g_aEffect_2D[nCntEffect].nLife = nLife;
			g_aEffect_2D[nCntEffect].nMaxLife = nLife;
			g_aEffect_2D[nCntEffect].moveType = moveType;
			g_aEffect_2D[nCntEffect].nType = nType;
			g_aEffect_2D[nCntEffect].bUse = true;		//使用している状態に変更
			g_Effect_2DNum++;

			if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_NORMAL)
			{//通常の時

				g_aEffect_2D[nCntEffect].bAddAlpha = false;
			}
			else if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKE)
			{//煙のエフェクト

				g_aEffect_2D[nCntEffect].bAddAlpha = false;
			}
			else if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK)
			{//黒煙のエフェクト

				g_aEffect_2D[nCntEffect].bAddAlpha = true;
			}
			else if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_BLACK)
			{//黒のエフェクト

				g_aEffect_2D[nCntEffect].bAddAlpha = true;
			}
			else if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_JUJI)
			{//十字のエフェクト

				g_aEffect_2D[nCntEffect].bAddAlpha = false;
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aEffect_2D[nCntEffect].col;
			pVtx[1].col = g_aEffect_2D[nCntEffect].col;
			pVtx[2].col = g_aEffect_2D[nCntEffect].col;
			pVtx[3].col = g_aEffect_2D[nCntEffect].col;

			break;
		}

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffEffect_2D->Unlock();
}

//==================================================================================
//エフェクトの数取得
//==================================================================================
int GetEffect_2DNum(void)
{
	return g_Effect_2DNum;
}

//==================================================================================
//エフェクトを使用するか
//==================================================================================
bool GetEffect_2DUse(void)
{
	return g_bEffect_2DUse;
}