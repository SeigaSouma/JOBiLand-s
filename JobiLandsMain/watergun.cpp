//=============================================================================
//
// 水鉄砲の弾処理 [watergun.cpp]
// Author : 大原怜将
//
//=============================================================================
#include "main.h"
#include "watergun.h"
#include "effect.h"
#include "input.h"
#include "particle.h"
#include "camera.h"
#include "crowd.h"
#include "enemy.h"
#include "calculation.h"
#include "player.h"

//マクロ定義
#define MAX_TOP			(4)		//頂点数
#define MAX_EFFECT		(1280)	//エフェクトの最大数
#define EFFECTSIZE1		(0.97f)
#define EFFECTSIZE2		(0.98f)
#define EFFECTSIZE3		(0.99f)


//プロトタイプ宣言
void UpdateSubWatergun(int nCntWatergun);
void UpdateSuperSubWatergun(int nCntWatergun);
void UpdateAddWatergun(int nCntWatergun);
void CollisionWaterBullet(int nCntWatergun, D3DXVECTOR3 pos, float fWidth, float fHeight);

//グローバル変数宣言
const char *c_apFilenameWatergun[] =		//ファイル読み込み
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect002.jpg",
	"data\\TEXTURE\\smoke_05.tga",
	"data\\TEXTURE\\effect000.png",
	"data\\TEXTURE\\hart_01.png",
};

LPDIRECT3DTEXTURE9 g_pTextureWatergun[(sizeof c_apFilenameWatergun) / sizeof(*c_apFilenameWatergun)] = {};	//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWatergun = NULL;	//頂点バッファへのポインタ
Watergun g_aWatergun[MAX_EFFECT];						//エフェクトの情報
int g_WatergunNum;									//エフェクトの数
bool g_bWatergunUse;

//==================================================================================
//エフェクトの初期化処理
//==================================================================================
void InitWatergun(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameWatergun) / sizeof(*c_apFilenameWatergun); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameWatergun[nCntTex],
			&g_pTextureWatergun[nCntTex]);

	}

	//エフェクトの情報の初期化
	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		g_aWatergun[nCntWatergun].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWatergun[nCntWatergun].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWatergun[nCntWatergun].mtxWorld = {};
		g_aWatergun[nCntWatergun].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aWatergun[nCntWatergun].fRadius = 0.0f;
		g_aWatergun[nCntWatergun].fMaxRadius = 0.0f;		//最大半径
		g_aWatergun[nCntWatergun].nLife = WATERBULLET_LIFE;
		g_aWatergun[nCntWatergun].nMaxLife = g_aWatergun[nCntWatergun].nLife;	//最大寿命
		g_aWatergun[nCntWatergun].moveType = 0;				//移動の種類
		g_aWatergun[nCntWatergun].nType = 0;
		g_aWatergun[nCntWatergun].bUse = false;				//使用していない状態にする
		g_aWatergun[nCntWatergun].bAddAlpha = false;
	}
	g_WatergunNum = 0;
	g_bWatergunUse = true;

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWatergun,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

								//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWatergun->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//頂点カラーの設定
		pVtx[0].col = g_aWatergun[nCntWatergun].col;
		pVtx[1].col = g_aWatergun[nCntWatergun].col;
		pVtx[2].col = g_aWatergun[nCntWatergun].col;
		pVtx[3].col = g_aWatergun[nCntWatergun].col;

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWatergun->Unlock();
}

//==================================================================================
//エフェクトの終了処理
//==================================================================================
void UninitWatergun(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameWatergun) / sizeof(*c_apFilenameWatergun); nCount++)
	{
		//テクスチャの破棄
		if (g_pTextureWatergun[nCount] != NULL)
		{
			g_pTextureWatergun[nCount]->Release();
			g_pTextureWatergun[nCount] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffWatergun != NULL)
	{
		g_pVtxBuffWatergun->Release();
		g_pVtxBuffWatergun = NULL;
	}
}

//==================================================================================
//エフェクトの更新処理
//==================================================================================
void UpdateWatergun(void)
{
	Enemy *pEnemy = GetEnemy();
	Crowd *pCrowd = GetCrowd();

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWatergun->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//F5が押された,エフェクト使用切り替え

		g_bWatergunUse = g_bWatergunUse ? false : true;
	}

	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		if (g_aWatergun[nCntWatergun].bUse == true)
		{//エフェクトが使用されていたら

		 //頂点座標の更新(小さくする)
			if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKEBLACK)
			{//黒煙だった場合

				if (((float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife) <= 0.5f)
				{//最大体力の半分以下

					g_aWatergun[nCntWatergun].pos += g_aWatergun[nCntWatergun].move * (((float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife));
				}
				else
				{//半分より上

					g_aWatergun[nCntWatergun].pos += g_aWatergun[nCntWatergun].move * (((float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife));
				}
			}
			else
			{//黒煙以外

			 //位置更新
				g_aWatergun[nCntWatergun].pos += g_aWatergun[nCntWatergun].move;
			}

			//群衆と敵の弾の当たり判定
			CollisionWaterBullet(nCntWatergun, pCrowd->aModel.pos, 50.0f, 50.0f);

			//小さくしていく処理の場合
			if (g_aWatergun[nCntWatergun].moveType == MOVEEFFECT_SUB)
			{//エフェクトを小さくしていく

				if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_NORMAL || g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKE ||
					g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKEBLACK || g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_BLACK)
				{
					//縮小処理
					UpdateSubWatergun(nCntWatergun);
				}
			}
			else if (g_aWatergun[nCntWatergun].moveType == MOVEEFFECT_SUPERSUB)
			{//エフェクトを小さくしていく

				UpdateSuperSubWatergun(nCntWatergun);
			}
			else
			{//エフェクトを大きくしていく

			 //拡大処理
				UpdateAddWatergun(nCntWatergun);
			}


			//寿命の更新
			g_aWatergun[nCntWatergun].nLife--;

			//不透明度の更新
			g_aWatergun[nCntWatergun].col.a = (float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife;

			if (g_aWatergun[nCntWatergun].nLife <= 0)
			{//寿命が尽きたら

				g_aWatergun[nCntWatergun].bUse = false;
				g_aWatergun[nCntWatergun].bAddAlpha = false;
				g_WatergunNum--;
			}
			
			//頂点カラーの設定
			pVtx[0].col = g_aWatergun[nCntWatergun].col;
			pVtx[1].col = g_aWatergun[nCntWatergun].col;
			pVtx[2].col = g_aWatergun[nCntWatergun].col;
			pVtx[3].col = g_aWatergun[nCntWatergun].col;

			//頂点座標の更新(小さくする)
			pVtx[0].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);

		}

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffWatergun->Unlock();
}

//==================================================================================
//エフェクトの縮小処理
//==================================================================================
void UpdateSubWatergun(int nCntWatergun)
{

	int nWatergunType = rand() % 3;

	if (nWatergunType == 0)
	{
		g_aWatergun[nCntWatergun].fRadius *= EFFECTSIZE1;
	}
	else if (nWatergunType == 1)
	{
		g_aWatergun[nCntWatergun].fRadius *= EFFECTSIZE2;
	}
	else if (nWatergunType == 2)
	{
		g_aWatergun[nCntWatergun].fRadius *= EFFECTSIZE3;
	}

	//g_aWatergun[nCntWatergun].fRadius = g_aWatergun[nCntWatergun].fMaxRadius * (float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife;

}

//==================================================================================
//エフェクトの縮小処理
//==================================================================================
void UpdateSuperSubWatergun(int nCntWatergun)
{

	g_aWatergun[nCntWatergun].fRadius = g_aWatergun[nCntWatergun].fMaxRadius * (float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife;
}

//==================================================================================
//エフェクトの拡大処理
//==================================================================================
void UpdateAddWatergun(int nCntWatergun)
{

	int nWatergunType = rand() % 3;

	if (nWatergunType == 0)
	{
		g_aWatergun[nCntWatergun].fRadius += 1.8f;
	}
	else if (nWatergunType == 1)
	{
		g_aWatergun[nCntWatergun].fRadius += 0.8f;
	}
	else if (nWatergunType == 2)
	{
		g_aWatergun[nCntWatergun].fRadius += 0.0f;
	}

}

//==================================================================================
//エフェクトの描画処理
//==================================================================================
void DrawWatergun(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

							// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//ポリゴンの描画
	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		if (g_aWatergun[nCntWatergun].bUse == true && g_bWatergunUse == true)
		{//エフェクトが使用されている

		 //αブレンディングを加算合成に設定
			if (g_aWatergun[nCntWatergun].bAddAlpha == false)
			{
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aWatergun[nCntWatergun].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aWatergun[nCntWatergun].mtxWorld, NULL, &mtxView);	//逆配列を求める
			g_aWatergun[nCntWatergun].mtxWorld._41 = 0.0f;
			g_aWatergun[nCntWatergun].mtxWorld._42 = 0.0f;
			g_aWatergun[nCntWatergun].mtxWorld._43 = 0.0f;

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aWatergun[nCntWatergun].pos.x, g_aWatergun[nCntWatergun].pos.y, g_aWatergun[nCntWatergun].pos.z);
			D3DXMatrixMultiply(&g_aWatergun[nCntWatergun].mtxWorld, &g_aWatergun[nCntWatergun].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aWatergun[nCntWatergun].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffWatergun, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_pTextureWatergun[g_aWatergun[nCntWatergun].nType]);

			//ビルボードの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWatergun * MAX_TOP, 2);

			//αブレンディングを元に戻す
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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

//==================================================================================
//エフェクトの設定処理
//==================================================================================
void SetWatergun(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffWatergun->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		if (g_aWatergun[nCntWatergun].bUse == false)
		{//エフェクトが使用されていなかったら

			g_aWatergun[nCntWatergun].pos = pos;
			g_aWatergun[nCntWatergun].move = move;
			g_aWatergun[nCntWatergun].col = col;
			g_aWatergun[nCntWatergun].fRadius = fRadius;
			g_aWatergun[nCntWatergun].fMaxRadius = fRadius;
			g_aWatergun[nCntWatergun].nLife = nLife;
			g_aWatergun[nCntWatergun].nMaxLife = nLife;
			g_aWatergun[nCntWatergun].moveType = moveType;
			g_aWatergun[nCntWatergun].nType = nType;
			g_aWatergun[nCntWatergun].bUse = true;		//使用している状態に変更
			g_WatergunNum++;

			//if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_NORMAL)
			//{//通常の時

			//	g_aWatergun[nCntWatergun].bAddAlpha = false;
			//}
			//else if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKE)
			//{//煙のエフェクト

			//	g_aWatergun[nCntWatergun].bAddAlpha = false;
			//}
			//else if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKEBLACK)
			//{//黒煙のエフェクト

			//	g_aWatergun[nCntWatergun].bAddAlpha = true;
			//}
			//else if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_BLACK)
			//{//黒のエフェクト

			//	g_aWatergun[nCntWatergun].bAddAlpha = true;
			//}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);

			//頂点カラーの設定
			pVtx[0].col = g_aWatergun[nCntWatergun].col;
			pVtx[1].col = g_aWatergun[nCntWatergun].col;
			pVtx[2].col = g_aWatergun[nCntWatergun].col;
			pVtx[3].col = g_aWatergun[nCntWatergun].col;

			break;
		}

		pVtx += MAX_TOP;	//頂点データのポインタを頂点数分進める
	}
	//頂点バッファをアンロックする
	g_pVtxBuffWatergun->Unlock();
}

//==================================================================================
//水鉄砲の弾の当たり判定
//==================================================================================
void CollisionWaterBullet(int nCntWatergun,  D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//群衆の情報取得
	Crowd *pCrowd = GetCrowd();

	//敵の情報取得
	Enemy *pEnemy = GetEnemy();

	//プレイヤーの情報を取得
	Player *pPlayer = GetPlayer();

	for (int nCount = 0; nCount < MAX_CROWD; nCount++, pCrowd++)
	{
		if (pCrowd->aModel.bUse == true && pCrowd->nState != CROWDSTATE_FADE)
		{//群衆が使用されていたら

		    //敵と群衆の距離
			float fLength = pCrowd->aModel.vtxMax.x + g_aWatergun[nCntWatergun].fRadius;

			if (CircleRange(g_aWatergun[nCntWatergun].pos, pCrowd->aModel.pos) <= (fLength * fLength))
			{//円の中に入ったら

			    //群衆を消す
				pCrowd->nState = CROWDSTATE_FADE;
			}
		}
	}
}

//==================================================================================
//エフェクトの数取得
//==================================================================================
int GetWatergunNum(void)
{
	return g_WatergunNum;
}

//==================================================================================
//エフェクトを使用するか
//==================================================================================
bool GetWatergunUse(void)
{
	return g_bWatergunUse;
}