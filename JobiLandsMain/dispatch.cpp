//=============================================================================
//
// 派遣処理 [dispatch.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "dispatch.h"
#include "camera.h"
#include "player.h"
#include "input.h"
#include "buddy.h"
#include "crowd.h"
#include "tutorial.h"
#include "serif.h"
#include "sound.h"

//マクロ定義
#define POS_DISPATCH	(1000.0f)
#define POS_DISPATCH_X	(100.0f)
#define POS_DISPATCH_Z	(100.0f)
#define MOVE			(2.8f)
#define WIDTH			(1)
#define HEIGHT			(1)

//グローバル変数宣言
const char *c_apFilenameDisPatch[] =	//ファイル読み込み
{
	"data\\TEXTURE\\target.png",
};
LPDIRECT3DTEXTURE9 g_apTextureDisPatch[(sizeof c_apFilenameDisPatch) / sizeof(*c_apFilenameDisPatch)] = {};	//テクスチャのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDisPatch = NULL;	//頂点バッファへのポインタ
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffDisPatch = NULL;	//インデックスバッファへのポインタ
DISPATCH g_aDisPatch[MAX_DISPACTH];		//派遣の情報
int g_nNumIndexaDisPatch;			//インデックス数
int g_nNumVertexaDisPatch;			//頂点数

//==================================================================================
//派遣の初期化処理
//==================================================================================
void InitDisPatch(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//テクスチャデータの配列分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameDisPatch) / sizeof(*c_apFilenameDisPatch); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameDisPatch[nCntTex],
			&g_apTextureDisPatch[nCntTex]);
	}

	//各要素初期化
	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		g_aDisPatch[nCntDis].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		g_aDisPatch[nCntDis].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
		g_aDisPatch[nCntDis].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

		//ワールドマトリックス
		D3DXMatrixIdentity(&g_aDisPatch[nCntDis].mtxWorld);
		g_aDisPatch[nCntDis].bUse = false;				//使用しているか
		g_aDisPatch[nCntDis].nType = -1;				//種類
	}

	g_nNumIndexaDisPatch = ((HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1))) * MAX_DISPACTH;	//インデックス数
	g_nNumVertexaDisPatch = ((WIDTH + 1) * (HEIGHT + 1)) * MAX_DISPACTH;	//頂点数

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexaDisPatch,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDisPatch,
		NULL);

	VERTEX_3D *pVtx;			//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	g_pVtxBuffDisPatch->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		//頂点情報の設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//縦の頂点数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				//頂点座標の設定
				pVtx[0].pos = D3DXVECTOR3(
					(POS_DISPATCH_X * nCntWidth) - ((POS_DISPATCH_X * WIDTH) * 0.5f),
					0.0f,
					-((POS_DISPATCH_Z * nCntHeight) - ((POS_DISPATCH_Z * HEIGHT) * 0.5f)));

				//法線ベクトルの設定
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//頂点カラーの設定
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//テクスチャ座標の設定
				pVtx[0].tex = D3DXVECTOR2
				(
					((float)nCntWidth / (1.0f / (float)(WIDTH + 1))) * (1.0f / (float)(WIDTH + 1)),
					((float)nCntHeight / (1.0f / (float)(HEIGHT + 1))) * (1.0f / (float)(HEIGHT + 1))
				);

				pVtx += 1;
			}
		}
	}

	//頂点バッファをアンロックする
	g_pVtxBuffDisPatch->Unlock();

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexaDisPatch,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffDisPatch,
		NULL);

	WORD *pIdx;	//インデックス情報へのポインタ

	//インデックスバッファをロックし、頂点番号データへのポインタを取得
	g_pIdxBuffDisPatch->Lock(0, 0, (void**)&pIdx, 0);

	//インデックスバッファの要素番号
	int nCntIdx = 0;
	int nVtxPoint = 0;

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		//頂点番号データの設定
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//高さの分割数分繰り返す

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//横の頂点数分繰り返す

				pIdx[nCntIdx + 0] = (nCntWidth + (WIDTH + 1) * (nCntHeight + 1)) + nVtxPoint;
				pIdx[nCntIdx + 1] = (nCntWidth + ((WIDTH + 1) * nCntHeight)) + nVtxPoint;
				nCntIdx += 2;	//2つずつ見てるから
			}

			if (nCntHeight + 1 < HEIGHT)
			{//最後のちょんは打たない

				//空打ち2つ分
				pIdx[nCntIdx + 0] = (((WIDTH + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
				pIdx[nCntIdx + 1] = ((WIDTH + 1) * (nCntHeight + 2)) + nVtxPoint;
				nCntIdx += 2;	//2つずつ見てるから
			}
		}

		nVtxPoint += ((WIDTH + 1) * (HEIGHT + 1));	//今回のインデックス数を加算
	}

	//インデックスバッファをアンロックする
	g_pIdxBuffDisPatch->Unlock();

}

//==================================================================================
//派遣の終了処理
//==================================================================================
void UninitDisPatch(void)
{
	//ボイス終了
	StopSound();

	//テクスチャの破棄
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameDisPatch) / sizeof(*c_apFilenameDisPatch); nCntTex++)
	{
		if (g_apTextureDisPatch[nCntTex] != NULL)
		{
			g_apTextureDisPatch[nCntTex]->Release();
			g_apTextureDisPatch[nCntTex] = NULL;
		}
	}

	//頂点バッファの破棄
	if (g_pVtxBuffDisPatch != NULL)
	{
		g_pVtxBuffDisPatch->Release();
		g_pVtxBuffDisPatch = NULL;
	}

	//頂点インデックスの破棄
	if (g_pIdxBuffDisPatch != NULL)
	{
		g_pIdxBuffDisPatch->Release();
		g_pIdxBuffDisPatch = NULL;
	}

}

//==================================================================================
//派遣の更新処理
//==================================================================================
void UpdateDisPatch(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		if (g_aDisPatch[nCntDis].bUse == true)
		{//使用していたら

			if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
			{//←キーが押された,左移動

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//A+W,左上移動

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//A+S,左下移動

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
				}
				else
				{//A,左移動

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
			{//Dキーが押された,右移動

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//D+W,右上移動

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//D+S,右下移動

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
				}
				else
				{//D,右移動

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//Wが押された、奥移動

				g_aDisPatch[nCntDis].move.x += sinf(pCamera->rot.y) * MOVE;
				g_aDisPatch[nCntDis].move.z += cosf(pCamera->rot.y) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//Sが押された、手前移動

				g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI + pCamera->rot.y) * MOVE;
				g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI + pCamera->rot.y) * MOVE;
			}

			//位置更新
			g_aDisPatch[nCntDis].pos += g_aDisPatch[nCntDis].move;

			//慣性つける
			g_aDisPatch[nCntDis].move.x += (0.0f - g_aDisPatch[nCntDis].move.x) * 0.3f;
			g_aDisPatch[nCntDis].move.z += (0.0f - g_aDisPatch[nCntDis].move.z) * 0.3f;

			if (GetGamepadRelease(BUTTON_LB + g_aDisPatch[nCntDis].nType, 0) || GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
			{//派遣の使用状況入れ替え

				Player *pPlayer = GetPlayer();

				if (g_aDisPatch[nCntDis].nType == 0)
				{
					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_DISPATCHL);

					//セリフをセット
					pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

					//ボイスセット
					PlaySound(SOUND_LABEL_SE_YATTYAE);
				}
				else if (g_aDisPatch[nCntDis].nType == 1)
				{
					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_DISPATCHR);

					//セリフをセット
					pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

					//ボイスセット
					PlaySound(SOUND_LABEL_SE_YATTYAE);
				}

				

				//派遣する
				SetBuddyDispatch(g_aDisPatch[nCntDis].pos, g_aDisPatch[nCntDis].nType);
				g_aDisPatch[nCntDis].bUse = false;
				g_aDisPatch[nCntDis].nType = -1;
			}

		}
	}
}

//==================================================================================
//派遣の描画処理
//==================================================================================
void DrawDisPatch(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	int nIdxPoint = 0;

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		if (g_aDisPatch[nCntDis].bUse == true)
		{//使用していたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aDisPatch[nCntDis].mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aDisPatch[nCntDis].rot.y, g_aDisPatch[nCntDis].rot.x, g_aDisPatch[nCntDis].rot.z);
			D3DXMatrixMultiply(&g_aDisPatch[nCntDis].mtxWorld, &g_aDisPatch[nCntDis].mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aDisPatch[nCntDis].pos.x, g_aDisPatch[nCntDis].pos.y, g_aDisPatch[nCntDis].pos.z);
			D3DXMatrixMultiply(&g_aDisPatch[nCntDis].mtxWorld, &g_aDisPatch[nCntDis].mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aDisPatch[nCntDis].mtxWorld);

			//頂点バッファをデータストリームに設定
			pDevice->SetStreamSource(0, g_pVtxBuffDisPatch, 0, sizeof(VERTEX_3D));

			//インデックスバッファをデータストリームに設定
			pDevice->SetIndices(g_pIdxBuffDisPatch);

			//頂点フォーマットの設定
			pDevice->SetFVF(FVF_VERTEX_3D);

			//テクスチャの設定
			pDevice->SetTexture(0, g_apTextureDisPatch[0]);

			//ポリゴンの描画
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				((WIDTH + 1) * (HEIGHT + 1)),
				nIdxPoint,
				((HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1))) - 2);
		}

		nIdxPoint += ((HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1)));	//今回のインデックス数を加算
	}
}

//==================================================================================
//派遣の設定処理(BUDDYTYPE_DEPPA
//==================================================================================
void SetRightDisPatch(void)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();
	pBuddy += DISPATCH_TYPE_RIGHT;
	//BUDDYTYPE_DEPPA

	//群衆の情報を取得
	Crowd *pCrowd = GetCrowd();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//派遣ターゲットの出現
	if (g_aDisPatch[0].nType != DISPATCH_TYPE_LEFT && pBuddy->nState != BUDDYSTATE_DISPATCH)
	{//もう一方が使われてないとき

	    //モーションをセット
		if (pPlayer->nNowMotionNum != PLAYERMOTION_MOVECURSOR)
		{
			SetMotisonPlayer(PLAYERMOTION_MOVECURSOR);
		}

		pPlayer->bMove = false;

		//使用状況入れ替え
		g_aDisPatch[0].bUse = true;
		g_aDisPatch[0].nType = DISPATCH_TYPE_RIGHT;
		//g_aDisPatch[nType].bUse = g_aDisPatch[nType].bUse ? false : true;
		g_aDisPatch[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDisPatch[0].pos = D3DXVECTOR3(pPlayer->pos.x - sinf(pPlayer->rot.y + D3DX_PI * 0.2f) * 200.0f, 0.5f, pPlayer->pos.z - cosf(pPlayer->rot.y + D3DX_PI * 0.2f) * 200.0f);
	}

	//========================================================================
	//帰還のステップ以上で操作可能
	//========================================================================
	if (GetMode() == MODE_GAME || (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_RETURN))
	{
		//呼び戻し
		if (pBuddy->nState == BUDDYSTATE_DISPATCH)
		{//派遣状態の時

			Player *pPlayer = GetPlayer();

			//右帰還完了
			pTutorial->aControl.bReturnR = true;

			//モーションをセット
			SetMotisonPlayer(PLAYERMOTION_RETURNR);
			pPlayer->bMove = false;

			//セリフの設定
			pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_RETURN, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

			//通常状態に戻す
			pBuddy->nState = BUDDYSTATE_RETURN;

			//PlaySound(SOUND_LABEL_SE_ORANGEKUN);

			for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++, pCrowd++)
			{
				if (pCrowd->aModel.bUse == true && pCrowd->nParent == PARENT_CHIBI)
				{//ガリが親の時

					//隊列を変更する
					pBuddy->aFormation[pCrowd->aFormation.nIndex].bUse = false;

					//隊列の位置設定
					pCrowd->aFormation.nIndex = SetFormationPos(PARENT_PLAYER);

					//親をプレイヤーにする
					pCrowd->nParent = PARENT_PLAYER;

					//帰還状態にする
					pCrowd->nState = CROWDSTATE_RETURN;
				}
			}
		}
	}
}

//==================================================================================
//派遣の設定処理(BUDDYTYPE_HYOROGARI
//==================================================================================
void SetLeftDisPatch(void)
{
	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//相棒の情報取得
	Buddy *pBuddy = GetBuddy();
	pBuddy += DISPATCH_TYPE_LEFT;

	//群衆の情報を取得
	Crowd *pCrowd = GetCrowd();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	if (g_aDisPatch[0].nType != DISPATCH_TYPE_RIGHT && pBuddy->nState != BUDDYSTATE_DISPATCH)
	{//もう一方が使われてないとき
		
	    //モーションをセット
		if (pPlayer->nNowMotionNum != PLAYERMOTION_MOVECURSOR)
		{
			SetMotisonPlayer(PLAYERMOTION_MOVECURSOR);
		}
		

		pPlayer->bMove = false;

		//使用状況入れ替え
		g_aDisPatch[0].bUse = true;
		g_aDisPatch[0].nType = DISPATCH_TYPE_LEFT;
		g_aDisPatch[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDisPatch[0].pos = D3DXVECTOR3(pPlayer->pos.x - sinf(pPlayer->rot.y + D3DX_PI * -0.2f) * 200.0f, 0.5f, pPlayer->pos.z - cosf(pPlayer->rot.y + D3DX_PI * -0.2f) * 200.0f);
	}

	//========================================================================
	//帰還のステップ以上で操作可能
	//========================================================================
	if (GetMode() == MODE_GAME || (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_RETURN))
	{
		//呼び戻し
		if (pBuddy->nState == BUDDYSTATE_DISPATCH)
		{//派遣状態の時

			Player *pPlayer = GetPlayer();

			//左帰還完了
			pTutorial->aControl.bReturnL = true;

			//通常状態にする
			SetMotisonPlayer(PLAYERMOTION_RETURNL);
			pPlayer->bMove = false;

			//セリフの設定
			pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_RETURN, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

			//通常状態に戻す
			pBuddy->nState = BUDDYSTATE_RETURN;

			//PlaySound(SOUND_LABEL_SE_ONMINO);

			for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++, pCrowd++)
			{
				if (pCrowd->aModel.bUse == true && pCrowd->nParent == PARENT_GARI)
				{//ガリが親の時

					//隊列を変更する
					pBuddy->aFormation[pCrowd->aFormation.nIndex].bUse = false;

					//隊列の位置設定
					pCrowd->aFormation.nIndex = SetFormationPos(PARENT_PLAYER);

					//親をプレイヤーにする
					pCrowd->nParent = PARENT_PLAYER;

					//帰還状態にする
					pCrowd->nState = CROWDSTATE_RETURN;
				}
			}
		}
	}
}

//==================================================================================
//派遣のチュートリアル更新処理
//==================================================================================
void UpdateTutorialDisPatch(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		if (g_aDisPatch[nCntDis].bUse == true)
		{//使用していたら

			if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
			{//←キーが押された,左移動

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//A+W,左上移動

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//A+S,左下移動

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
				}
				else
				{//A,左移動

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
			{//Dキーが押された,右移動

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//D+W,右上移動

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//D+S,右下移動

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
				}
				else
				{//D,右移動

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//Wが押された、奥移動

				g_aDisPatch[nCntDis].move.x += sinf(pCamera->rot.y) * MOVE;
				g_aDisPatch[nCntDis].move.z += cosf(pCamera->rot.y) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//Sが押された、手前移動

				g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI + pCamera->rot.y) * MOVE;
				g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI + pCamera->rot.y) * MOVE;
			}

			//位置更新
			g_aDisPatch[nCntDis].pos += g_aDisPatch[nCntDis].move;

			//慣性つける
			g_aDisPatch[nCntDis].move.x += (0.0f - g_aDisPatch[nCntDis].move.x) * 0.3f;
			g_aDisPatch[nCntDis].move.z += (0.0f - g_aDisPatch[nCntDis].move.z) * 0.3f;

			if (GetGamepadRelease(BUTTON_LB + g_aDisPatch[nCntDis].nType, 0) || GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
			{//派遣の使用状況入れ替え

				if (g_aDisPatch[nCntDis].nType == 0)
				{
					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_DISPATCHL);
					
					//セリフをセット
					pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

					//SEをセット
					PlaySound(SOUND_LABEL_SE_YATTYAE);

					//左派遣完了
					pTutorial->aControl.bDispatchL = true;
				}
				else if (g_aDisPatch[nCntDis].nType == 1)
				{
					//モーションの設定
					SetMotisonPlayer(PLAYERMOTION_DISPATCHR);

					//セリフをセット
					pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

					//SEをセット
					PlaySound(SOUND_LABEL_SE_YATTYAE);

					//右派遣完了
					pTutorial->aControl.bDispatchR = true;
				}

				//派遣する
				SetBuddyDispatch(g_aDisPatch[nCntDis].pos, g_aDisPatch[nCntDis].nType);
				g_aDisPatch[nCntDis].bUse = false;
				g_aDisPatch[nCntDis].nType = -1;
			}
		}
	}
}

//==================================================================================
//派遣の情報取得
//==================================================================================
DISPATCH *GetDispatch(void)
{
	return &g_aDisPatch[0];
}

