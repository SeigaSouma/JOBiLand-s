//=============================================================================
//
// 抱き枕カバー処理 [meshcylinder.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "cutepillow.h"
#include "input.h"
#include "debugproc.h"
#include "calculation.h"
#include "mob.h"
#include "rotate.h"
#include "hypnosis_gauge.h"
#include "crowd.h"
#include "shadow.h"
#include "minimap_icon.h"
#include "particle.h"
#include "sound.h"

//マクロ定義
#define POS_Cutepillow	(50.0f * 0.7f)
#define POS_Cutepillow_Y	(30.0f * 0.7f)
#define MOVE		(8.0f)
#define WIDTH		(8)
#define HEIGHT		(8)

//プロトタイプ宣言
void UpdatePillowThrow(int nCntPillow, D3DXVECTOR3 *pos, float fRot);
void CollisionMobPillow(int nCntPillow);
void HomingLenPillow(int nCntPillow);

//グローバル変数宣言
const char *c_apFilenameCutePillow[] =					//ファイル読み込み
{
	"data\\TEXTURE\\p_kurokami.png",
	"data\\TEXTURE\\p_bobchan.png",
	"data\\TEXTURE\\p_tuin.png",
	"data\\TEXTURE\\p_seifuku.png",
	"data\\TEXTURE\\p_one.png",
	"data\\TEXTURE\\p_kode.png",
	"data\\TEXTURE\\p_goldtuin.png",
};
LPDIRECT3DTEXTURE9 g_apTextureCutePillow[(sizeof c_apFilenameCutePillow) / sizeof(*c_apFilenameCutePillow)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCutePillow = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffCutePillow = NULL;	//インデックスバッファへのポインタ
Cutepillow g_aCutePillow[MAX_CUTEPILLOW];			//抱き枕カバーの情報
int g_nNumIndexCutePillow;			//インデックス数
int g_nNumVertexCutePillow;			//頂点数
float g_fHigh = 9.5f;
float g_fMove = 4.4f;
float g_fHeight = 0.07f;
float g_fRot = 0.3f;

//==================================================================================
//抱き枕カバーの初期化処理
//==================================================================================
void InitCutepillow(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCutePillow) / sizeof(*c_apFilenameCutePillow); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCutePillow[nCntTex],
			&g_apTextureCutePillow[nCntTex]);
	}

	//各要素初期化
	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		g_aCutePillow[nCntPillow].pos = D3DXVECTOR3(0.0f, -(POS_Cutepillow_Y * HEIGHT), 0.0f);		//位置
		g_aCutePillow[nCntPillow].posOld = D3DXVECTOR3(0.0f, -(POS_Cutepillow_Y * HEIGHT), 0.0f);		//位置
		g_aCutePillow[nCntPillow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_aCutePillow[nCntPillow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

		//ワールドマトリックス
		D3DXMatrixIdentity(&g_aCutePillow[nCntPillow].mtxWorld);
		g_aCutePillow[nCntPillow].nIdxShadow = -1;			//影のインデックス番号
		g_aCutePillow[nCntPillow].nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));			//インデックス数
		g_aCutePillow[nCntPillow].nNumVertex = (HEIGHT + 1) * (WIDTH + 1);			//頂点数
		g_aCutePillow[nCntPillow].nState = 0;				//状態
		g_aCutePillow[nCntPillow].nType = 0;				//種類
		g_aCutePillow[nCntPillow].nParent = 0;			//親
		g_aCutePillow[nCntPillow].bUse = false;				//使用しているか
		g_aCutePillow[nCntPillow].fWidth = POS_Cutepillow;			//横
		g_aCutePillow[nCntPillow].fHeight = POS_Cutepillow_Y;			//縦
	}
	g_fHigh = 9.5f;
	g_fMove = 4.4f;
	g_fHeight = 0.07f;
	g_fRot = 0.3f;

	g_nNumIndexCutePillow = ((HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1))) * MAX_CUTEPILLOW;		//インデックス数
	g_nNumVertexCutePillow = ((HEIGHT + 1) * (WIDTH + 1)) * MAX_CUTEPILLOW;		//頂点数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexCutePillow,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCutePillow,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCutePillow->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[MAX_CUTEPILLOW];		//計算用の座標
	float fRot = (D3DX_PI * 2) / (float)(WIDTH);	//1つごとの角度を求める
	D3DXVECTOR3 NormalizeNor[MAX_CUTEPILLOW];

	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			float fWidth = POS_Cutepillow;
			if (nCntHeight == 0)
			{
				fWidth *= 0.5f;
			}

			pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * fWidth,
				(POS_Cutepillow_Y * HEIGHT) - (POS_Cutepillow_Y * HEIGHT) - ((POS_Cutepillow_Y * nCntHeight)),
				cosf(nCntWidth % WIDTH * fRot) * fWidth
			);

			pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * fWidth,
				(POS_Cutepillow_Y * HEIGHT) - (POS_Cutepillow_Y * HEIGHT) - ((POS_Cutepillow_Y * (nCntHeight + 1))),
				cosf(nCntWidth % WIDTH * fRot) * fWidth
			);

			//各頂点から原点を引く
			NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aCutePillow[0].pos;
			NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aCutePillow[0].pos;

			//出た向きの値を正規化する
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))], &NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);
		}
	}

	//頂点情報の設定
	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//頂点座標の設定
				pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//法線ベクトルの設定
				pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2
				(
					nCntWidth * (1.0f / (float)(WIDTH)),
					nCntHeight * (1.0f / (float)(HEIGHT))
				);

				pVtx += 1;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCutePillow->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexCutePillow,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffCutePillow,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffCutePillow->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;
	int nVtxPoint = 0;

	//頂点番号データの設定
	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//高さの分割数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				pIdx[nCntIdx + 1] = (nCntWidth + (WIDTH + 1) * (nCntHeight + 1)) + nVtxPoint;
				pIdx[nCntIdx + 0] = (nCntWidth + ((WIDTH + 1) * nCntHeight)) + nVtxPoint;

				nCntIdx += 2;	//2つずつ見てるから
			}

			if (nCntHeight + 1 < HEIGHT)
			{//最後のちょんは打たない

				pIdx[nCntIdx + 1] = (((WIDTH + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
				pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 2)) + nVtxPoint;

				nCntIdx += 2;	//2つずつ見てるから
			}
		}

		nVtxPoint += g_aCutePillow[nCntPillow].nNumVertex;	//今回のインデックス数を加算
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffCutePillow->Unlock();

}

//==================================================================================
//抱き枕カバーの終了処理
//==================================================================================
void UninitCutepillow(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCutePillow) / sizeof(*c_apFilenameCutePillow); nCntTex++)
	{
		if (g_apTextureCutePillow[nCntTex] != NULL)
		{
			g_apTextureCutePillow[nCntTex]->Release();
			g_apTextureCutePillow[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffCutePillow != NULL)
	{
		g_pVtxBuffCutePillow->Release();
		g_pVtxBuffCutePillow = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffCutePillow != NULL)
	{
		g_pIdxBuffCutePillow->Release();
		g_pIdxBuffCutePillow = NULL;
	}

}

//==================================================================================
//抱き枕カバーの更新処理
//==================================================================================
void UpdateCutepillow(void)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffCutePillow->Lock(0, 0, (void**)&pVtx, 0);

	//影の情報取得
	SHADOW *pShadow = GetShadow();

	D3DXVECTOR3 pos[MAX_CUTEPILLOW];		//計算用の座標
	float fRot = (D3DX_PI * 2) / (float)(WIDTH);	//1つごとの角度を求める
	D3DXVECTOR3 NormalizeNor[MAX_CUTEPILLOW];

	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		if (g_aCutePillow[nCntPillow].bUse == true)
		{
			//前回の位置更新
			g_aCutePillow[nCntPillow].posOld = g_aCutePillow[nCntPillow].pos;

			//投げの更新処理
			UpdatePillowThrow(nCntPillow, &pos[0], fRot);

			//弾の位置の更新
			g_aCutePillow[nCntPillow].pos += g_aCutePillow[nCntPillow].move;

			//群衆との当たり判定
			CollisionMobPillow(nCntPillow);

			//地面に付いたら消す
			if (g_aCutePillow[nCntPillow].pos.y <= 0.0f && g_aCutePillow[nCntPillow].nState == CUTEPILLOW_STATE_FALL)
			{
				g_aCutePillow[nCntPillow].bUse = false;

				//影も消す
				pShadow[g_aCutePillow[nCntPillow].nIdxShadow].bUse = false;
			}

			for (int nCntHeight = 0, nCntVtx = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					//頂点座標の設定
					pVtx[nCntVtx].pos = pos[nCntVtx];
					nCntVtx++;
				}
			}

			//影の位置更新
			SetPositionShadow(g_aCutePillow[nCntPillow].nIdxShadow, g_aCutePillow[nCntPillow].pos);
		}

		//頂点数分加算
		pVtx += g_aCutePillow[nCntPillow].nNumVertex;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffCutePillow->Unlock();
}

//==================================================================================
//抱き枕カバーの投げた処理
//==================================================================================
void UpdatePillowThrow(int nCntPillow, D3DXVECTOR3 *pos, float fRot)
{

	//伸ばす処理
	if (g_aCutePillow[nCntPillow].move.y >= 0.0f)
	{//上昇中

		g_aCutePillow[nCntPillow].move.y -= 0.5f;
		g_aCutePillow[nCntPillow].fHeight = POS_Cutepillow_Y * g_fHeight;
		g_aCutePillow[nCntPillow].rot.y += g_fRot;
		HomingLenPillow(nCntPillow);
	}
	else
	{//下降中

		g_aCutePillow[nCntPillow].nState = CUTEPILLOW_STATE_FALL;
		g_aCutePillow[nCntPillow].move.y -= 0.005f;
		g_aCutePillow[nCntPillow].move.x *= 0.96f;
		g_aCutePillow[nCntPillow].move.z *= 0.96f;
		g_aCutePillow[nCntPillow].fHeight += (POS_Cutepillow_Y - g_aCutePillow[nCntPillow].fHeight) * 0.025f;
		g_aCutePillow[nCntPillow].rot.y += g_fRot * 0.1f;
	}

	//角度の正規化
	RotNormalize(&g_aCutePillow[nCntPillow].rot.y);

	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//縦の頂点数分繰り返す

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//横の頂点数分繰り返す

			float fWidth = g_aCutePillow[nCntPillow].fWidth;
			float fHeight = g_aCutePillow[nCntPillow].fHeight;

			if (nCntHeight == 0)
			{
				fWidth *= 0.5f;
			}

			pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * fWidth,
				(POS_Cutepillow_Y * HEIGHT) - (fHeight * HEIGHT) - ((fHeight * nCntHeight)),
				cosf(nCntWidth % WIDTH * fRot) * fWidth
			);

			pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * fWidth,
				(POS_Cutepillow_Y * HEIGHT) - (fHeight * HEIGHT) - ((fHeight * (nCntHeight + 1))),
				cosf(nCntWidth % WIDTH * fRot) * fWidth
			);
		}
	}

}

//==================================================================================
//抱き枕カバーの投げた処理
//==================================================================================
void HomingLenPillow(int nCntPillow)
{

	//モブの情報取得
	Mob *pMob = GetMob();

	float MaxLength = 9999999.9f;	//長さの最大値
	int nIdxCrawl = -1;	//現在の巡回位置番号

	int nn = 0;
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++, nn++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS)
		{//モブが使用されていたら

			//抱き枕カバーとモブの距離
			float fNowLength =
				(pMob->aModel.pos.x - g_aCutePillow[nCntPillow].pos.x) * (pMob->aModel.pos.x - g_aCutePillow[nCntPillow].pos.x)
				+ (pMob->aModel.pos.z - g_aCutePillow[nCntPillow].pos.z) * (pMob->aModel.pos.z - g_aCutePillow[nCntPillow].pos.z);

			fNowLength *= 0.5f;

			//最大値が今回の長さより大きかったら
			if (MaxLength >= fNowLength)
			{
				MaxLength = fNowLength;	//最大値入れ替え
				nIdxCrawl = nCntMob;	//番号入れ替え
			}
		}
	}

	pMob -= MAX_MOB;

	int nAA = pMob[nIdxCrawl].aModel.nIdxRotate;

	if (CircleRange(pMob[nIdxCrawl].aModel.pos, g_aCutePillow[nCntPillow].pos) <= (60.0f * 60.0f))
	{//規定値に着いていたら

		g_aCutePillow[nCntPillow].move.x *= 0.9f;
		g_aCutePillow[nCntPillow].move.z *= 0.9f;
	}
}

//==================================================================================
//抱き枕カバーとモブの当たり判定
//==================================================================================
void CollisionMobPillow(int nCntPillow)
{
	//モブの情報取得
	Mob *pMob = GetMob();
	Rotate *pRotate = GetRotate();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();
	SHADOW *pShadow = GetShadow();
	SwitchSE *pSwitchSE = GetSwitchSE();

	//アイコンの情報取得
	MINIICON *pMiniIcon = GetMiniIcon();

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS)
		{//群衆が使用されていたら && 催眠状態

			//抱き枕カバーとモブの距離
			float fLength = pMob->aModel.vtxMax.x + g_aCutePillow[nCntPillow].fWidth;

			if (SphereRange(g_aCutePillow[nCntPillow].pos, pMob->aModel.pos) <= (fLength * fLength) &&
				pMob->aModel.pos.y + pMob->aModel.vtxMax.y >= g_aCutePillow[nCntPillow].pos.y)
			{//円の中に入ったら && 被ったら

				//群衆の設定
				SetCrowd(pMob->aModel.pos, pMob->aModel.nType, pMob->nTexType, g_aCutePillow[nCntPillow].nParent);
				pMob->aModel.bUse = false;
				StopSound(SOUND_LABEL_SE_PIYOPIYO);
				pSwitchSE->bUseRotate = false;


				//ぐるぐる削除
				pRotate[pMob->aModel.nIdxRotate].bUse = false;
				pRotate[pMob->aModel.nIdxRotate].bDisp = false;
				pMob->aModel.nIdxRotate = -1;

				//催眠度ゲージ削除
				pHypnosis[pMob->aModel.nIdxHypGauge].bUse = false;
				pMob->aModel.nIdxHypGauge = -1;

				//影削除
				pShadow[pMob->aModel.nIdxShadow].bUse = false;

				//ミニマップアイコンの削除
				pMiniIcon[pMob->nIdxIcon].bUse = false;
			}
		}
	}
}

//==================================================================================
//抱き枕カバーの描画処理
//==================================================================================
void DrawCutepillow(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	// ライティングを無効にする
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntPillow = 0, nIdxPoint = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		if (g_aCutePillow[nCntPillow].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aCutePillow[nCntPillow].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCutePillow[nCntPillow].rot.y, g_aCutePillow[nCntPillow].rot.x, g_aCutePillow[nCntPillow].rot.z);
			D3DXMatrixMultiply(&g_aCutePillow[nCntPillow].mtxWorld, &g_aCutePillow[nCntPillow].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aCutePillow[nCntPillow].pos.x, g_aCutePillow[nCntPillow].pos.y, g_aCutePillow[nCntPillow].pos.z);
			D3DXMatrixMultiply(&g_aCutePillow[nCntPillow].mtxWorld, &g_aCutePillow[nCntPillow].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aCutePillow[nCntPillow].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffCutePillow, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffCutePillow);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureCutePillow[g_aCutePillow[nCntPillow].nType]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aCutePillow[nCntPillow].nNumVertex,
				nIdxPoint,
				g_aCutePillow[nCntPillow].nNumIndex - 2);
		}

		//インデックス数加算
		nIdxPoint += g_aCutePillow[nCntPillow].nNumIndex;
	}
	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==================================================================================
//抱き枕カバーの設定処理
//==================================================================================
void SetCutepillow(D3DXVECTOR3 pos, D3DXVECTOR3 rot ,int nParent)
{
	
	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		if (g_aCutePillow[nCntPillow].bUse == false)
		{//使用されていなかったら


			g_aCutePillow[nCntPillow].bUse = true;
			g_aCutePillow[nCntPillow].pos = pos;
			g_aCutePillow[nCntPillow].pos.y = pos.y - POS_Cutepillow_Y;
			g_aCutePillow[nCntPillow].posOld = pos;
			g_aCutePillow[nCntPillow].move = D3DXVECTOR3(sinf(D3DX_PI + rot.y) * g_fMove, g_fHigh, cosf(D3DX_PI + rot.y) * g_fMove);
			g_aCutePillow[nCntPillow].nState = CUTEPILLOW_STATE_TROW;
			g_aCutePillow[nCntPillow].nType = rand() % ((sizeof c_apFilenameCutePillow) / sizeof(*c_apFilenameCutePillow));
			g_aCutePillow[nCntPillow].nParent = nParent;

			//影を設定
			g_aCutePillow[nCntPillow].nIdxShadow = SetShadow(POS_Cutepillow, POS_Cutepillow);

			//影の位置更新
			SetPositionShadow(g_aCutePillow[nCntPillow].nIdxShadow, g_aCutePillow[nCntPillow].pos);

			//発射パーティクルの設定
			SetParticle(D3DXVECTOR3(g_aCutePillow[nCntPillow].pos.x, g_aCutePillow[nCntPillow].pos.y + POS_Cutepillow * (HEIGHT - 5), g_aCutePillow[nCntPillow].pos.z), PARTICLE_TYPE_PILLOWTHROW);
			break;
		}
	}
}

//==================================================================================
//抱き枕カバーの情報取得
//==================================================================================
Cutepillow *GetCutepillow(void)
{
	return &g_aCutePillow[0];
}

