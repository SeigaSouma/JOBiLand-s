//=============================================================================
//
// ミニマップアイコン [minimap_icon.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "player.h"
#include "minimap_icon.h"
#include "mapshape.h"

//マクロ定義
#define SIZE	(80.0f)	//画像サイズ
#define MAX_CHR	(1280)		//キャラクター数

//グローバル変数宣言
const char *c_apFilenameMiniIcon[] =					//ファイル読み込み
{
	"data\\TEXTURE\\000.png",
	"data\\TEXTURE\\001.png",
	"data\\TEXTURE\\002.png",
	"data\\TEXTURE\\003.png",
	"data\\TEXTURE\\004.png",
};

LPDIRECT3DTEXTURE9 g_apTextureMiniIcon[(sizeof c_apFilenameMiniIcon) / sizeof(*c_apFilenameMiniIcon)] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniIcon = NULL;
MINIICON g_aMiniIcon[MAX_CHR];		//アイコンの情報

//==================================================================================
//アイコンの初期化処理
//==================================================================================
void InitMiniIcon(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMiniIcon) / sizeof(*c_apFilenameMiniIcon); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMiniIcon[nCntTex],
			&g_apTextureMiniIcon[nCntTex]);
	}

	//各変数の初期化
	for (int nCnt = 0; nCnt < MAX_CHR; nCnt++)
	{
		g_aMiniIcon[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aMiniIcon[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMiniIcon[nCnt].mtxWorld);
		g_aMiniIcon[nCnt].nType = 0;				//アイコンのタイプ
		g_aMiniIcon[nCnt].bUse = false;				//使用されているかどうか
	}

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_CHR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiniIcon,
		NULL);

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMiniIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_CHR; nCnt++)
	{
		//位置を更新
		pVtx[0].pos = D3DXVECTOR3(- SIZE, 0.0f, + SIZE);
		pVtx[1].pos = D3DXVECTOR3(+ SIZE, 0.0f, + SIZE);
		pVtx[2].pos = D3DXVECTOR3(- SIZE, 0.0f, - SIZE);
		pVtx[3].pos = D3DXVECTOR3(+ SIZE, 0.0f, - SIZE);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

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

		pVtx += 4;		//頂点データのポインタを4つ分進める

	}
	//頂点バッファをアンロックする
	g_pVtxBuffMiniIcon->Unlock();
}

//==================================================================================
//アイコンの終了処理
//==================================================================================
void UninitMiniIcon(void)
{
	for (int nCnt = 0; nCnt < (sizeof c_apFilenameMiniIcon) / sizeof(*c_apFilenameMiniIcon); nCnt++)
	{
		//テクスチャの破棄
		if (g_apTextureMiniIcon[nCnt] != NULL)
		{
			g_apTextureMiniIcon[nCnt]->Release();
			g_apTextureMiniIcon[nCnt] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMiniIcon != NULL)
	{
		g_pVtxBuffMiniIcon->Release();
		g_pVtxBuffMiniIcon = NULL;
	}
}

//==================================================================================
//アイコンの更新処理
//==================================================================================
void UpdateMiniIcon(void)
{

}

//==================================================================================
//アイコンの位置更新処理
//==================================================================================
void UpdatePositionIcon(int nIdxIcon, D3DXVECTOR3 pos)
{
	if (g_aMiniIcon[nIdxIcon].bUse == true)
	{//使用されていたら

		//アイコンをくっつける
		g_aMiniIcon[nIdxIcon].pos = pos;
	}
}

//==================================================================================
//アイコンの位置更新処理
//==================================================================================
int SetIcon(int nType, float size)
{
	int nCntShadow = -1;

	VERTEX_3D *pVtx;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMiniIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_CHR; nCount++)
	{
		if (g_aMiniIcon[nCount].bUse == false)
		{//使用していなかったら

			//位置を更新
			pVtx[0].pos = D3DXVECTOR3(-size, 0.0f, +size);
			pVtx[1].pos = D3DXVECTOR3(+size, 0.0f, +size);
			pVtx[2].pos = D3DXVECTOR3(-size, 0.0f, -size);
			pVtx[3].pos = D3DXVECTOR3(+size, 0.0f, -size);

			g_aMiniIcon[nCount].nType = nType;
			nCntShadow = nCount;	//インデックス番号取得
			g_aMiniIcon[nCount].bUse = true;	//使用している状況にする
			break;
		}

		pVtx += 4;		//頂点データのポインタを4つ分進める
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMiniIcon->Unlock();

	return nCntShadow;
}

//==================================================================================
//モブの描画処理
//==================================================================================
void DrawMiniIcon(void)
{

	//マップの形描画処理
	//DrawMapShape();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言

	//ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

#if 1
	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
#else
	//Zテストを無効にする
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//常に描画する
#endif

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCnt = 0; nCnt < MAX_CHR; nCnt++)
	{
		if (g_aMiniIcon[nCnt].bUse == true)
		{
			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMiniIcon[nCnt].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMiniIcon[nCnt].rot.y, g_aMiniIcon[nCnt].rot.x, g_aMiniIcon[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aMiniIcon[nCnt].mtxWorld, &g_aMiniIcon[nCnt].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aMiniIcon[nCnt].pos.x, g_aMiniIcon[nCnt].pos.y, g_aMiniIcon[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aMiniIcon[nCnt].mtxWorld, &g_aMiniIcon[nCnt].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMiniIcon[nCnt].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMiniIcon, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMiniIcon[g_aMiniIcon[nCnt].nType]);

			//ポリゴンの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
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
//アイコンの情報取得
//==================================================================================
MINIICON *GetMiniIcon(void)
{
	return &g_aMiniIcon[0];
}