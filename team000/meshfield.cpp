//=============================================================================
//
// メッシュフィールド処理 [meshfield.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"
#include "debugproc.h"
#include "mapshape.h"

//プロトタイプ宣言
void CreatMeshFieldIndex(void);
void CreatMeshFieldVeretx(void);
void ReadMeshField(void);

//グローバル変数宣言
const char *c_apFilenameMeshField[] =					//ファイル読み込み
{
	"data\\TEXTURE\\FIELD\\concreat.png",
	"data\\TEXTURE\\FIELD\\senter.png",
	"data\\TEXTURE\\FIELD\\senterlong.png",
	"data\\TEXTURE\\FIELD\\sideL.png",
	"data\\TEXTURE\\FIELD\\sideR.png",
	"data\\TEXTURE\\FIELD\\odan.png",
	"data\\TEXTURE\\FIELD\\walkroad.jpg",
	"data\\TEXTURE\\FIELD\\walkroad_01.jpg",
	"data\\TEXTURE\\FIELD\\tile_04.jpg",
	"data\\TEXTURE\\FIELD\\tile_03.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureMeshField[(sizeof c_apFilenameMeshField) / sizeof(*c_apFilenameMeshField)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;	//インデックスバッファへのポインタ
MESHFIELD g_aMeshField[MAX_FIELD];					//メッシュフィールドの情報
int g_nNumIndexaMeshField;			//インデックス数
int g_nNumVertexaMeshField;			//頂点数
int g_nCntUseField;

//==================================================================================
//メッシュフィールドの初期化処理
//==================================================================================
void InitMeshField(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMeshField) / sizeof(*c_apFilenameMeshField); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMeshField[nCntTex],
			&g_apTextureMeshField[nCntTex]);
	}

	//各要素初期化
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_aMeshField[nCntField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aMeshField[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aMeshField[nCntField].mtxWorld);
		g_aMeshField[nCntField].nNumIndex = 0;			//インデックス数
		g_aMeshField[nCntField].nNumVertex = 0;			//頂点数
		g_aMeshField[nCntField].nWidth = 0;				//横分割数
		g_aMeshField[nCntField].nHeight = 0;			//縦分割数
		g_aMeshField[nCntField].fWidthLen = 0.0f;		//横の長さ
		g_aMeshField[nCntField].fHeightLen = 0.0f;		//縦の長さ
		g_aMeshField[nCntField].bUse = false;				//使用しているか
		g_aMeshField[nCntField].nType = 0;				//種類
	}
	g_nCntUseField = 0;
	g_nNumIndexaMeshField = 0;
	g_nNumVertexaMeshField = 0;

	//セットする場所
	ReadMeshField();

	//頂点バッファの生成
	CreatMeshFieldVeretx();

	//インデックスの生成
	CreatMeshFieldIndex();
}

//==================================================================================
//メッシュフィールドの終了処理
//==================================================================================
void UninitMeshField(void)
{
	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMeshField) / sizeof(*c_apFilenameMeshField); nCntTex++)
	{
		if (g_apTextureMeshField[nCntTex] != NULL)
		{
			g_apTextureMeshField[nCntTex]->Release();
			g_apTextureMeshField[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

}

//==================================================================================
//メッシュフィールドの更新処理
//==================================================================================
void UpdateMeshField(void)
{
	PrintDebugProc(
		"床の数：%d", g_nCntUseField);
}

//==================================================================================
//メッシュフィールドの描画処理
//==================================================================================
void DrawMeshField(DRAWFIELD_TYPE nType)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	int nIdxPoint = 0;

	if (nType == DRAWFIELD_TYPE_MAP)
	{//ミニマップの時だけ


		//マップの形描画処理
		//DrawMapShape();


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
	}

#if _DEBUG
	if (GetKeyboardPress(DIK_F3) == true)
	{//F9が押された

		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//埋めるモード
	}
	else
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//埋めるモード
	}
#endif

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//使用していたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aMeshField[nCntField].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntField].rot.y, g_aMeshField[nCntField].rot.x, g_aMeshField[nCntField].rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntField].mtxWorld, &g_aMeshField[nCntField].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntField].pos.x, g_aMeshField[nCntField].pos.y, g_aMeshField[nCntField].pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntField].mtxWorld, &g_aMeshField[nCntField].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntField].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffMeshField);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureMeshField[g_aMeshField[nCntField].nType]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshField[nCntField].nNumVertex,
				nIdxPoint,
				g_aMeshField[nCntField].nNumIndex - 2);
		}

		nIdxPoint += g_aMeshField[nCntField].nNumIndex;	//今回のインデックス数を加算
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
//メッシュフィールドの設定処理
//==================================================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height, float fWidthLen, float fHeightLen, int nType)
{

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == false)
		{//使用していなかったら

			g_aMeshField[nCntField].pos = pos;	//位置
			g_aMeshField[nCntField].rot = rot;	//向き
			g_aMeshField[nCntField].nNumVertex = (Width + 1) * (Height + 1);	//頂点数
			g_aMeshField[nCntField].nNumIndex = (Height * ((Width + 1) * 2)) + (2 * (Height - 1));	//インデックス数
			g_nNumIndexaMeshField += g_aMeshField[nCntField].nNumIndex;	//インデックス数
			g_nNumVertexaMeshField += g_aMeshField[nCntField].nNumVertex;	//頂点数
			g_nCntUseField++;

			g_aMeshField[nCntField].nWidth = Width;			//横分割数
			g_aMeshField[nCntField].nHeight = Height;		//縦分割数
			g_aMeshField[nCntField].fWidthLen = fWidthLen;		//横の長さ
			g_aMeshField[nCntField].fHeightLen = fHeightLen;	//縦の長さ
			g_aMeshField[nCntField].nType = nType;			//種類
			g_aMeshField[nCntField].bUse = true;	//使用している状況にする
			break;
		}
	}

}

//==================================================================================
//メッシュフィールドの頂点バッファ作成
//==================================================================================
void CreatMeshFieldVeretx(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexaMeshField,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//頂点情報の設定
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//使用していたら

			for (int nCntHeight = 0; nCntHeight < g_aMeshField[nCntField].nHeight + 1; nCntHeight++)
			{//縦の頂点数分繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshField[nCntField].nWidth + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					//頂点座標の設定
					pVtx[0].pos = D3DXVECTOR3(
						(g_aMeshField[nCntField].fWidthLen * nCntWidth) - ((g_aMeshField[nCntField].fWidthLen * g_aMeshField[nCntField].nWidth) * 0.5f),
						0.0f,
						-((g_aMeshField[nCntField].fHeightLen * nCntHeight) - ((g_aMeshField[nCntField].fHeightLen * g_aMeshField[nCntField].nHeight) * 0.5f)));

					//法線ベクトルの設定
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					//頂点カラーの設定
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//テクスチャ座標の設定
					pVtx[0].tex = D3DXVECTOR2
					(
						((float)nCntWidth / (1.0f / (float)(g_aMeshField[nCntField].nWidth + 1))) * (1.0f / (float)(g_aMeshField[nCntField].nWidth + 1)),
						((float)nCntHeight / (1.0f / (float)(g_aMeshField[nCntField].nHeight + 1))) * (1.0f / (float)(g_aMeshField[nCntField].nHeight + 1))
					);

					pVtx += 1;
				}
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffMeshField->Unlock();
}

//==================================================================================
//メッシュフィールドのインデックス作成
//==================================================================================
void CreatMeshFieldIndex(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexaMeshField,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;
	int nVtxPoint = 0;

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//使用していたら

			//頂点番号データの設定
			for (int nCntHeight = 0; nCntHeight < g_aMeshField[nCntField].nHeight; nCntHeight++)
			{//高さの分割数分繰り返す

				for (int nCntWidth = 0; nCntWidth < g_aMeshField[nCntField].nWidth + 1; nCntWidth++)
				{//横の頂点数分繰り返す

					pIdx[nCntIdx + 0] = (nCntWidth + (g_aMeshField[nCntField].nWidth + 1) * (nCntHeight + 1)) + nVtxPoint;
					pIdx[nCntIdx + 1] = (nCntWidth + ((g_aMeshField[nCntField].nWidth + 1) * nCntHeight)) + nVtxPoint;
					nCntIdx += 2;	//2つずつ見てるから
				}

				if (nCntHeight + 1 < g_aMeshField[nCntField].nHeight)
				{//最後のちょんは打たない

					//空打ち2つ分
					pIdx[nCntIdx + 0] = (((g_aMeshField[nCntField].nWidth + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
					pIdx[nCntIdx + 1] = ((g_aMeshField[nCntField].nWidth + 1) * (nCntHeight + 2)) + nVtxPoint;
					nCntIdx += 2;	//2つずつ見てるから
				}
			}

			nVtxPoint += g_aMeshField[nCntField].nNumVertex;	//今回のインデックス数を加算
		}
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffMeshField->Unlock();
}

//==================================================================================
//メッシュフィールドの外部ファイル読み込み処理
//==================================================================================
void ReadMeshField(void)
{
	char aComment[256];	//コメント用
	int nCntMesh = 0;
	int nCntFileName = 0;

	FILE *pFile = NULL;	//ファイルポインタを宣言

	pFile = fopen("data\\TEXT\\model_set.txt", "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (1)
		{//END_SCRIPTが来るまで繰り返す

			//文字列の読み込み
			fscanf(pFile, "%s", &aComment[0]);

			//モデルの設定
			if (strcmp(aComment, "FIELDSET") == 0)
			{//モデルの読み込みを開始

				while (strcmp(aComment, "END_FIELDSET"))
				{//END_FIELDSETが来るまで繰り返し

					fscanf(pFile, "%s", &aComment[0]);	//確認する

					if (strcmp(aComment, "TEXTYPE") == 0)
					{//TEXTYPEが来たら種類読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aMeshField[nCntMesh].nType);	//モデル種類の列挙
					}

					if (strcmp(aComment, "POS") == 0)
					{//POSが来たら位置読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].pos.x);	//X座標
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].pos.y);	//Y座標
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].pos.z);	//Z座標
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROTが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].rot.x);	//Xの向き
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].rot.y);	//Yの向き
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].rot.z);	//Zの向き
						g_aMeshField[nCntMesh].rot.x = D3DXToRadian(g_aMeshField[nCntMesh].rot.x);
						g_aMeshField[nCntMesh].rot.y = D3DXToRadian(g_aMeshField[nCntMesh].rot.y);
						g_aMeshField[nCntMesh].rot.z = D3DXToRadian(g_aMeshField[nCntMesh].rot.z);
					}

					if (strcmp(aComment, "BLOCK") == 0)
					{//BLOCKが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%d", &g_aMeshField[nCntMesh].nWidth);	//横の分割数
						fscanf(pFile, "%d", &g_aMeshField[nCntMesh].nHeight);	//縦の分割数
					}

					if (strcmp(aComment, "SIZE") == 0)
					{//SIZEが来たら向き読み込み

						fscanf(pFile, "%s", &aComment[0]);		//=の分
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].fWidthLen);	//横の分割数
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].fHeightLen);	//縦の分割数
					}

				}//END_FIELDSETのかっこ

				 //モデルの番号加算
				nCntMesh++;
			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//終了文字でループを抜ける

				break;
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}

	for (int nCount = 0; nCount < nCntMesh; nCount++)
	{
		//フィールドの配置
		SetMeshField(
			g_aMeshField[nCount].pos, g_aMeshField[nCount].rot,
			g_aMeshField[nCount].nWidth, g_aMeshField[nCount].nHeight,
			g_aMeshField[nCount].fWidthLen, g_aMeshField[nCount].fHeightLen, g_aMeshField[nCount].nType);
	}
}