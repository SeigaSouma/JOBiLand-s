//=============================================================================
//
// セリフ処理 [serif.h]
// Author : 大原怜将
//
//=============================================================================
#include "main.h"
#include "serif.h"
#include "player.h"
#include "camera.h"
#include "mob.h"
#include "enemy.h"

//マクロ定義
#define MAX_WIDTH		(70.0f)		//横幅
#define MAX_HEIGHT		(60.0f)		//縦幅
#define MAX_TOP			(4)
#define MAX_HYP			(10)		//催眠度の最大値
#define MOVE_TIME		(20)
#define NUM_ENEMYSERIF	(6)			//敵の普通のセリフの数
#define NUM_DISCSERIF	(5)			//敵の発見時のセリフの数
#define NUM_BOCCHISERIF	(5)			//ぼっちのセリフの数
#define NUM_YAKUZASERIF	(5)			//やくざのセリフの数
#define NUM_WORKSERIF	(4)			//会社員のセリフの数

//プロトタイプ宣言
//void GaugeDecrement(int nCntGauge);
//void ChangeColorSerif(int nCntGauge);
void SelectBocchiSerif(int nCntSerif);
void SelectYakuzaSerif(int nCntSerif);
void SelectWorkSerif(int nCntSerif);
void SelectEnemyDiscSerif(int nCntSerif);
void SelectEnemyNoneSerif(int nCntSerif);

//グローバル変数宣言
const char *c_apFilenameSerif[] =		//ファイル読み込み
{
	"data\\TEXTURE\\serif000.png",		//派遣
	"data\\TEXTURE\\serif001.png",		//帰還
	"data\\TEXTURE\\serif002.png",		//敵（！）
	"data\\TEXTURE\\serif019.png",		//敵（ニート発見！）
	"data\\TEXTURE\\serif020.png",		//敵（まだまだだね）
	"data\\TEXTURE\\serif021.png",		//敵（怠惰）
	"data\\TEXTURE\\serif022.png",		//敵（バレなきゃ犯罪じゃない）
	"data\\TEXTURE\\serif003.png",		//メイト
	"data\\TEXTURE\\serif004.png",		//ぼっち（部活）
	"data\\TEXTURE\\serif005.png",		//ぼっち（現実）
	"data\\TEXTURE\\serif010.png",		//ぼっち（え、）
	"data\\TEXTURE\\serif011.png",		//ぼっち（イキって）
	"data\\TEXTURE\\serif012.png",		//ぼっち（働いたら負け）
	"data\\TEXTURE\\serif006.png",		//ヤクザ（ボス）
	"data\\TEXTURE\\serif013.png",		//ヤクザ（ヤクザかラッパー）
	"data\\TEXTURE\\serif014.png",		//ヤクザ（小指ない）
	"data\\TEXTURE\\serif015.png",		//ヤクザ（東京湾）
	"data\\TEXTURE\\serif016.png",		//ヤクザ（heke）
	"data\\TEXTURE\\serif008.png",		//会社員（会社）
	"data\\TEXTURE\\serif009.png",		//会社員（電車遅延）
	"data\\TEXTURE\\serif017.png",		//会社員（アニメ消化）
	"data\\TEXTURE\\serif018.png",		//会社員（２０連勤…）
	"data\\TEXTURE\\serif023.png",		//敵（ニートはどこだ）
	"data\\TEXTURE\\serif024.png",		//敵（ニート撲滅）
	"data\\TEXTURE\\serif025.png",		//敵（秋葉原守らないと）
	"data\\TEXTURE\\serif026.png",		//敵（ニートに人権ない）
	"data\\TEXTURE\\serif027.png",		//敵（働こうと思わないの）
	"data\\TEXTURE\\serif028.png",		//敵（汚物は消毒だー！）
};
LPDIRECT3DTEXTURE9 g_pTextureSerif[(sizeof c_apFilenameSerif) / sizeof(*c_apFilenameSerif)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSerif = NULL;	//頂点バッファへのポインタ
Serif g_aSerif[MAX_SERIF];				//HPゲージの情報
bool g_bUseSerif;		//セリフを初期化したか

//==================================================================================
//セリフの初期化処理
//==================================================================================
void InitSerif(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSerif) / sizeof(*c_apFilenameSerif); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameSerif[nCntTex],
			&g_pTextureSerif[nCntTex]);
	}


	//各要素初期化
	for (int nCntGauge = 0; nCntGauge < MAX_SERIF; nCntGauge++)
	{
		g_aSerif[nCntGauge].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSerif[nCntGauge].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//ワールドマトリックスの初期化
		D3DXMatrixIdentity(&g_aSerif[nCntGauge].mtxWorld);
		g_aSerif[nCntGauge].fWidth = 0.0f;
		g_aSerif[nCntGauge].fHeight = 0.0f;
		g_aSerif[nCntGauge].nType = SERIFSTATE_DISPATCH;
		g_aSerif[nCntGauge].nLife = 0;
		g_aSerif[nCntGauge].nBocchiSelect = 0;			//ぼっちちゃんのセリフの選択
		g_aSerif[nCntGauge].nYakuzaSelect = 0;			//やくざのセリフの選択
		g_aSerif[nCntGauge].nWorkSelect = 0;			//会社員のセリフの選択
		g_aSerif[nCntGauge].nEnemyDiscSelect = 0;		//敵の発見時のセリフの選択
		g_aSerif[nCntGauge].nEnemyNoneSelect = 0;		//敵の普通のセリフの選択
		g_aSerif[nCntGauge].bUse = false;
		g_aSerif[nCntGauge].bDisp = false;

	}

	g_bUseSerif = false;		//セリフを初期化したか

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_SERIF,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSerif,
		NULL);

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSerif->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_SERIF; nCntGauge++)
	{
		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


		pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);


		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;
		
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSerif->Unlock();
}

//==================================================================================
//セリフの終了処理
//==================================================================================
void UninitSerif(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSerif) / sizeof(*c_apFilenameSerif); nCntTex++)
	{
		//テクスチャの破棄
		if (g_pTextureSerif[nCntTex] != NULL)
		{
			g_pTextureSerif[nCntTex]->Release();
			g_pTextureSerif[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffSerif != NULL)
	{
		g_pVtxBuffSerif->Release();
		g_pVtxBuffSerif = NULL;
	}
}

//==================================================================================
//セリフの更新処理
//==================================================================================
void UpdateSerif(void)
{

	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSerif->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_SERIF; nCntGauge++)
	{
		if (g_aSerif[nCntGauge].bUse == true && g_aSerif[nCntGauge].bDisp == true)
		{//使用されていたら

			if (g_aSerif[nCntGauge].fWidth <= MAX_WIDTH && g_aSerif[nCntGauge].fHeight <= MAX_HEIGHT && g_aSerif[nCntGauge].nLife > 0)
			{//横幅が50以下かつ縦幅が25以下かつ寿命が0より大きい時

				g_aSerif[nCntGauge].fWidth += MAX_WIDTH / (float)MOVE_TIME;     //横幅を加算
				g_aSerif[nCntGauge].fHeight += MAX_HEIGHT / (float)MOVE_TIME;   //縦幅を加算
			}
			else
			{
				int nn = 0;
			}
			
			if (g_aSerif[nCntGauge].nLife > 0)
			{//寿命が0より多きとき

				g_aSerif[nCntGauge].nLife--;    //寿命を減らす
			}
			

			if (g_aSerif[nCntGauge].nLife <= 0)
			{//寿命が0のとき

				g_aSerif[nCntGauge].fWidth -= (MAX_WIDTH / (float)MOVE_TIME) * 0.5f;    //横幅を減らす
				g_aSerif[nCntGauge].fHeight -= (MAX_HEIGHT / (float)MOVE_TIME) * 0.5f;  //縦幅を減らす

				if (g_aSerif[nCntGauge].fWidth <= 0.0f && g_aSerif[nCntGauge].fHeight <= 0.0f)
				{//横幅も縦幅も0以下になったら

					//敵の情報取得
					Enemy *pEnemy = GetEnemy();

					//モブの情報取得
					Mob *pMob = GetMob();

					//プレイヤーの情報取得
					Player *pPlayer = GetPlayer();

					g_aSerif[nCntGauge].bUse = false;     //使用していない状態にする
					g_aSerif[nCntGauge].bDisp = false;    //描画しない
					g_bUseSerif = false;

					//敵のインデックス初期化
					if (g_bUseSerif == false)
					{
						for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
						{
							if (nCntGauge == pEnemy->nIdxSerif && pEnemy->bUse == true && pEnemy->bMoreSpawn == true)
							{
								pEnemy->nIdxSerif = -1;
								g_bUseSerif = true;

								break;
							}
						}
					}
					
					//モブのインデックス初期化
					if (g_bUseSerif == false)
					{
						for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
						{
							if (nCntGauge == pMob->aModel.nIdxSerif && pMob->aModel.bUse == true)
							{
								pMob->aModel.nIdxSerif = -1;
								g_bUseSerif = true;

								break;

							}
						}
					}

					//プレイヤーのインデックス初期化
					if (g_bUseSerif == false)
					{
						if (nCntGauge == pPlayer->nIdxSerif && pPlayer->bUse == true)
						{
							pPlayer->nIdxSerif = -1;
							g_bUseSerif = true;
						}
					}

					g_bUseSerif = false;
				}
			}

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);
		}

		if ((g_aSerif[nCntGauge].bDisp == false && g_aSerif[nCntGauge].bUse == true && g_aSerif[nCntGauge].nType == SERIFSTATE_DISPATCH) ||
			(g_aSerif[nCntGauge].bDisp == false && g_aSerif[nCntGauge].bUse == true && g_aSerif[nCntGauge].nType == SERIFSTATE_RETURN))
		{//使用してたら && 派遣 ||
		 //使用してたら && 帰還

			g_aSerif[nCntGauge].bUse = false;			//使用してない状態にする

		}

		pVtx += MAX_TOP;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSerif->Unlock();
}

//==================================================================================
//セリフの位置更新
//==================================================================================
void SetPositionSerif(int nIdxGauge, D3DXVECTOR3 pos)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	if (g_aSerif[nIdxGauge].bUse == true && g_aSerif[nIdxGauge].bDisp == true)
	{//使用されていたら

		g_aSerif[nIdxGauge].pos = pos;
		g_aSerif[nIdxGauge].pos.y = pos.y + 15.0f;
	}
}

//==================================================================================
//セリフの描画処理
//==================================================================================
void DrawSerif(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//計算用マトリックス宣言
	D3DXMATRIX mtxView;		//ビューマトリックス取得用

	// ライティングを無効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//アルファテストを有効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_SERIF; nCntGauge++)
	{
		if (g_aSerif[nCntGauge].bUse == true && g_aSerif[nCntGauge].bDisp == true)
		{//使用されていたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aSerif[nCntGauge].mtxWorld);

			//ビューマトリックスを取得
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//ポリゴンをカメラに対して正面に向ける
			D3DXMatrixInverse(&g_aSerif[nCntGauge].mtxWorld, NULL, &mtxView);	//逆配列を求める

			//上で消える
			g_aSerif[nCntGauge].mtxWorld._21 = 0.0f;
			g_aSerif[nCntGauge].mtxWorld._23 = 0.0f;
			g_aSerif[nCntGauge].mtxWorld._24 = 0.0f;

			g_aSerif[nCntGauge].mtxWorld._41 = 0.0f;
			g_aSerif[nCntGauge].mtxWorld._42 = 0.0f;
			g_aSerif[nCntGauge].mtxWorld._43 = 0.0f;

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aSerif[nCntGauge].pos.x, g_aSerif[nCntGauge].pos.y, g_aSerif[nCntGauge].pos.z);
			D3DXMatrixMultiply(&g_aSerif[nCntGauge].mtxWorld, &g_aSerif[nCntGauge].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aSerif[nCntGauge].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffSerif, 0, sizeof(VERTEX_3D));

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			switch (g_aSerif[nCntGauge].nType)
			{
			case SERIFSTATE_DISPATCH:		//派遣

				pDevice->SetTexture(0, g_pTextureSerif[0]);
				break;

			case SERIFSTATE_RETURN:			//帰還

				pDevice->SetTexture(0, g_pTextureSerif[1]);
				break;

			case SERIFSTATE_ENEMYNONE:		//敵（うろちょろ）

				//敵のセリフの選択処理
				SelectEnemyNoneSerif(nCntGauge);

				break;

			case SERIFSTATE_ENEMYDISC:		//敵（発見時）

				//敵のセリフの選択処理
				SelectEnemyDiscSerif(nCntGauge);
				break;

			case SERIFSTATE_MOB_BOCCHI:			//ぼっち

				//モブのセリフの選択処理
				SelectBocchiSerif(nCntGauge);

				break;

			case SERIFSTATE_MOB_YAKUZA:			//やくざ

				//モブのセリフの選択処理
				SelectYakuzaSerif(nCntGauge);

				break;

			case SERIFSTATE_MOB_WORK:			//会社員

				//モブのセリフの選択処理
				SelectWorkSerif(nCntGauge);

				break;
			}
			
			//HPゲージの描画
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntGauge * 4), 2);
		}
	}

	//アルファテストを無効にする
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==================================================================================
//セリフの設定処理
//==================================================================================
int SetSerif(float fWidth, float fHeight, int nLife, int nType, D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	//頂点情報へのポインタ

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffSerif->Lock(0, 0, (void**)&pVtx, 0);

	int nCntGauge = -1;
	int nn = 0;

	//１つ使用してたら消す
	if (nType == SERIFSTATE_DISPATCH || nType == SERIFSTATE_RETURN)
	{
		for (int nCount = 0; nCount < MAX_SERIF; nCount++)
		{
			if ((g_aSerif[nCount].bUse == true && g_aSerif[nCount].nType == SERIFSTATE_DISPATCH) ||
				(g_aSerif[nCount].bUse == true && g_aSerif[nCount].nType == SERIFSTATE_RETURN))
			{//使用してたら && 派遣 ||
			 //使用してたら && 帰還

				g_aSerif[nCount].bDisp = false;			//描画してない状態にする

			}

			if (g_aSerif[nCount].bUse == false)
			{
				pPlayer->nIdxSerif = -1;		//プレイヤーのインデックス初期化
			}
		}
	}

	for (int nCount = 0; nCount < MAX_SERIF; nCount++)
	{
		if (g_aSerif[nCount].bUse == false)
		{//使用していなかったら

			nCntGauge = nCount;						//インデックス番号取得
			g_aSerif[nCntGauge].bUse = true;		//使用している状態にする
			g_aSerif[nCntGauge].bDisp = true;		//描画している状態にする

			g_aSerif[nCntGauge].fHeight = fHeight;
			g_aSerif[nCntGauge].fWidth = fWidth;
			g_aSerif[nCntGauge].nLife = nLife;

			if (g_aSerif[nCntGauge].nLife != 150)
			{

				g_aSerif[nCntGauge].bUse = true;		//使用している状態にする
			}
			g_aSerif[nCntGauge].nType = nType;
			g_aSerif[nCntGauge].pos = pos;

			//頂点座標の設定
			pVtx[0].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);

			//セリフの種類決める
			if (g_aSerif[nCount].nType == SERIFSTATE_ENEMYNONE)
			{//敵の普通のセリフだったら

				g_aSerif[nCntGauge].nEnemyNoneSelect = rand() % NUM_ENEMYSERIF;
			}
			if (g_aSerif[nCount].nType == SERIFSTATE_ENEMYDISC)
			{//敵の発見時のセリフだったら

				g_aSerif[nCntGauge].nEnemyDiscSelect = rand() % NUM_DISCSERIF;
			}
			else if (g_aSerif[nCount].nType == SERIFSTATE_MOB_BOCCHI)
			{//ぼっちのセリフだったら

				g_aSerif[nCntGauge].nBocchiSelect = rand() % NUM_BOCCHISERIF;
			}
			else if (g_aSerif[nCount].nType == SERIFSTATE_MOB_YAKUZA)
			{//やくざのセリフだったら

				g_aSerif[nCntGauge].nYakuzaSelect = rand() % NUM_YAKUZASERIF;
			}
			else if (g_aSerif[nCount].nType == SERIFSTATE_MOB_WORK)
			{//会社員のセリフだったら

				g_aSerif[nCntGauge].nWorkSelect = rand() % NUM_WORKSERIF;
			}
			
			break;
		}

		pVtx += MAX_TOP;
	}

	if (nCntGauge == -1)
	{
		int nn = 0;
	}

	//頂点バッファをアンロックする
	g_pVtxBuffSerif->Unlock();

	return nCntGauge;
}

//==================================================================================
//敵の普通のセリフの選択処理
//==================================================================================
void SelectEnemyNoneSerif(int nCntSerif)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャ設定
	switch (g_aSerif[nCntSerif].nEnemyNoneSelect)
	{
	case ENEMYSERIFSTATE_SEEK:		//ニートはどこだ

		pDevice->SetTexture(0, g_pTextureSerif[22]);

		break;

	case ENEMYSERIFSTATE_ERADIC:	//ニート撲滅

		pDevice->SetTexture(0, g_pTextureSerif[23]);

		break;

	case ENEMYSERIFSTATE_AKIBA:		//秋葉原まもらないと

		pDevice->SetTexture(0, g_pTextureSerif[24]);

		break;

	case ENEMYSERIFSTATE_ZINKEN:	//ニートに人権ない

		pDevice->SetTexture(0, g_pTextureSerif[25]);

		break;

	case ENEMYSERIFSTATE_WORK:		//働こうと思わないの

		pDevice->SetTexture(0, g_pTextureSerif[26]);

		break;

	case ENEMYSERIFSTATE_OBUTSU:	//汚物は消毒だー！

		pDevice->SetTexture(0, g_pTextureSerif[27]);

		break;
	}
}

//==================================================================================
//敵の発見時のセリフの選択処理
//==================================================================================
void SelectEnemyDiscSerif(int nCntSerif)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャ設定
	switch (g_aSerif[nCntSerif].nEnemyDiscSelect)
	{
	case DISCSERIFSTATE_BIKKURI:		//！

		pDevice->SetTexture(0, g_pTextureSerif[2]);

		break;

	case DISCSERIFSTATE_NEET:			//ニート発見

		pDevice->SetTexture(0, g_pTextureSerif[3]);

		break;

	case DISCSERIFSTATE_MADAMADA:		//まだまだだね

		pDevice->SetTexture(0, g_pTextureSerif[4]);

		break;

	case DISCSERIFSTATE_TAIDA:			//怠惰デスねぇ

		pDevice->SetTexture(0, g_pTextureSerif[5]);

		break;

	case DISCSERIFSTATE_HANZAI:		//バレなきゃ犯罪じゃない

		pDevice->SetTexture(0, g_pTextureSerif[6]);

		break;
	}
}

//==================================================================================
//ぼっちのセリフの選択処理
//==================================================================================
void SelectBocchiSerif(int nCntSerif)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ぼっちちゃん
	switch (g_aSerif[nCntSerif].nBocchiSelect)
	{

	case BOCCHISERIFSTATE_CLUB:			//ぼっち（部活）

		pDevice->SetTexture(0, g_pTextureSerif[8]);

		break;

	case BOCCHISERIFSTATE_REAL:			//ぼっち（現実）

		pDevice->SetTexture(0, g_pTextureSerif[9]);

		break;

	case BOCCHISERIFSTATE_EEE:			//ぼっち（え、）

		pDevice->SetTexture(0, g_pTextureSerif[10]);

		break;

	case BOCCHISERIFSTATE_SORRY:		//ぼっち（イキってすみません）

		pDevice->SetTexture(0, g_pTextureSerif[11]);

		break;

	case BOCCHISERIFSTATE_WORK:			//ぼっち（働いたら負け）

		pDevice->SetTexture(0, g_pTextureSerif[12]);

		break;
	}
}

//==================================================================================
//やくざのセリフの選択処理
//==================================================================================
void SelectYakuzaSerif(int nCntSerif)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//やくざ
	switch (g_aSerif[nCntSerif].nYakuzaSelect)
	{
	case YAKUZASERIFSTATE_BOSS:			//やくざ（ボス）

		pDevice->SetTexture(0, g_pTextureSerif[13]);

		break;

	case YAKUZASERIFSTATE_RAPPER:		//やくざ（ヤクザかラッパー）

		pDevice->SetTexture(0, g_pTextureSerif[14]);

		break;

	case YAKUZASERIFSTATE_KOYUBI:		//やくざ（小指ない）

		pDevice->SetTexture(0, g_pTextureSerif[15]);

		break;

	case YAKUZASERIFSTATE_TOKYO:		//やくざ（東京湾）

		pDevice->SetTexture(0, g_pTextureSerif[16]);

		break;

	case YAKUZASERIFSTATE_HEKE:			//やくざ（ヘケ）

		pDevice->SetTexture(0, g_pTextureSerif[17]);

		break;
	}
}

//==================================================================================
//会社員のセリフの選択処理
//==================================================================================
void SelectWorkSerif(int nCntSerif)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//会社員
	switch (g_aSerif[nCntSerif].nWorkSelect)
	{

	case WORKSERIFSTATE_BLACK:			//会社員（会社やめたい）

		pDevice->SetTexture(0, g_pTextureSerif[18]);

		break;

	case WORKSERIFSTATE_STATION:		//会社員（電車遅延）

		pDevice->SetTexture(0, g_pTextureSerif[19]);

		break;

	case WORKSERIFSTATE_ANIME:			//会社員（アニメ消化）

		pDevice->SetTexture(0, g_pTextureSerif[20]);

		break;

	case WORKSERIFSTATE_KEEP:			//会社員（２０連勤）

		pDevice->SetTexture(0, g_pTextureSerif[21]);

		break;
	}
}

//==================================================================================
//セリフの情報取得
//==================================================================================
Serif *GetSerif(void)
{
	return &g_aSerif[0];

	//大原は神
	//↑それな
	//↑は？
	//↑何言ってんだ
	///大原全角使うな殺すぞby okusada
	//草
	//バグってた
	//だまれ
}
