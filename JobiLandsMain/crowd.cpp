//=============================================================================
//
// 群衆処理 [crowd.h]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "crowd.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "calculation.h"
#include "mob.h"
#include "hypnosis_gauge.h"
#include "rotate.h"
#include "shadow.h"
#include "cutepillow.h"
#include "buddy.h"
#include "particle.h"
#include "buddy.h"
#include "tutorial.h"
#include "sound.h"
#include "resultscore.h"

//マクロ定義
#define CHASE_CNT	(60)	//追従までのカウント
#define THROW_COOLTIME	(90)	//投げたときのクールタイム
#define RETURN_MOVE		(2.5f)	//帰還時の移動量
#define HART_COOLTIME	(10)	//ハートのクールタイム

//プロトタイプ宣言
void ControllCrowd(int nCntCrowd);
void UpdateStateCrowd(int nCntCrowd);
void LimitPosCrowd(int nCntCrowd);
void CollisionModelCrowd(int nCntCrowd);
void ThrowPillowCrowd(int nCntCrowd);

//グローバル変数宣言
const char *c_apFilenameCrowd[] =					//ファイル読み込み
{
	"data\\TEXTURE\\crowd02.jpg",
	"data\\TEXTURE\\crowd05.jpg",
	"data\\TEXTURE\\crowd04.jpg",
	"data\\TEXTURE\\crowd03.jpg",
	"data\\TEXTURE\\crowd07.jpg",
	"data\\TEXTURE\\crowd08.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureCrowd[(sizeof c_apFilenameCrowd) / sizeof(*c_apFilenameCrowd)] = {};	//テクスチャのポインタ
Crowd g_aCrowd[MAX_CROWD];		//群衆の情報
CrowdInfo g_aCrowdInfo;         //情報
CrowdNum g_aCrowdNum;			//群衆の数の情報
D3DXVECTOR3 FormationPos[MAX_CROWD];
int g_nHartSoundCooltime;

//==================================================================================
//群衆の初期化処理
//==================================================================================
void InitCrowd(void)
{

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCrowd) / sizeof(*c_apFilenameCrowd); nCntTex++)
	{
		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCrowd[nCntTex],
			&g_apTextureCrowd[nCntTex]);
	}

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//各要素初期化
	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		//群衆構造体の初期化
		g_aCrowd[nCntCrowd].nState = CROWDSTATE_NONE;	//状態
		g_aCrowd[nCntCrowd].nCntChase = 0;			//追い掛けるカウント
		g_aCrowd[nCntCrowd].fFadeAlpha = 1.0f;		//フェードの不透明度
		g_aCrowd[nCntCrowd].formationPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//隊列の位置
		g_aCrowd[nCntCrowd].ParentPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//親の位置
		g_aCrowd[nCntCrowd].ParentRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//親の向き
		g_aCrowd[nCntCrowd].fThrowRadius = 240.0f;		//投げる範囲
		g_aCrowd[nCntCrowd].nThrowCnt = 0;				//投げるカウント
		g_aCrowd[nCntCrowd].nType = 0;				//種類
		g_aCrowd[nCntCrowd].nParent = 0;			//親
		g_aCrowd[nCntCrowd].nTexType = 0;			//テクスチャタイプ
		g_aCrowd[nCntCrowd].bFever = false;			//フィーバー状態か

		//モデル構造体の初期化
		g_aCrowd[nCntCrowd].aModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//現在の位置
		g_aCrowd[nCntCrowd].aModel.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
		g_aCrowd[nCntCrowd].aModel.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の位置
		g_aCrowd[nCntCrowd].aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		g_aCrowd[nCntCrowd].aModel.rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//元の向き
		g_aCrowd[nCntCrowd].aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量

		//ワールドマトリックス
		D3DXMatrixIdentity(&g_aCrowd[nCntCrowd].aModel.mtxWorld);
		D3DXMatrixIdentity(&g_aCrowd[nCntCrowd].aModel.OldmtxWorld);

		g_aCrowd[nCntCrowd].aModel.nIdxShadow = -1;			//影のインデックス番号
		g_aCrowd[nCntCrowd].aModel.nIdxRotate = -1;			//ぐるぐるのインデックス番号
		g_aCrowd[nCntCrowd].aModel.nIdxHypGauge = -1;		//体力ゲージのインデックス番号
		g_aCrowd[nCntCrowd].aModel.nShadow = 1;			//影を使うかどうか

		g_aCrowd[nCntCrowd].aModel.nNumVtx = 0;			//頂点数
		g_aCrowd[nCntCrowd].aModel.dwSizeFVF = NULL;		//頂点フォーマットのサイズ
		g_aCrowd[nCntCrowd].aModel.pVtxBuff = NULL;			//頂点バッファのポインタ
		g_aCrowd[nCntCrowd].aModel.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//モデルの最小値
		g_aCrowd[nCntCrowd].aModel.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//モデルの最大値

		g_aCrowd[nCntCrowd].aModel.nType = 0;			//読み込むモデルの種類
		g_aCrowd[nCntCrowd].aModel.nParent = -1;		//親の番号
		g_aCrowd[nCntCrowd].aModel.bUse = false;			//使用しているか
		g_aCrowd[nCntCrowd].aModel.nState = 0;			//状態

		g_aCrowd[nCntCrowd].aModel.pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
		g_aCrowd[nCntCrowd].aModel.pBuffMat = NULL;		//マテリアルへのポインタ

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aCrowd[nCntCrowd].aModel.pMatData[nCntMat].MatD3D = {};	//マテリアルのデータ
			g_aCrowd[nCntCrowd].aModel.pMatData[nCntMat].pTextureFilename = NULL;	//マテリアルのデータ
			g_aCrowd[nCntCrowd].aModel.acFilename[nCntMat] = NULL;	//ファイル名
		}
		g_aCrowd[nCntCrowd].aModel.dwNumMat = NULL;			//マテリアルの数

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aCrowd[nCntCrowd].aModel.pTexture[nCntTex] = NULL;	//テクスチャへのポインタ
		}
		
		//隊列の情報初期化
		g_aCrowd[nCntCrowd].aFormation.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//隊列の位置
		g_aCrowd[nCntCrowd].aFormation.nIndex = -1;			//隊列のインデックス番号
		g_aCrowd[nCntCrowd].aFormation.bUse = false;			//隊列を使用しているか
	}

	//群衆情報の初期化
	//g_aCrowdInfo.nCrowdValue = 0;        //敵の総数
	g_aCrowdInfo.nParentPlayerNum = 0;	//親がプレイヤーの数
	g_aCrowdInfo.nCrowdCnter = 0;		//カウンター初期化
	g_aCrowdInfo.nCrowdKeep = 0;
	g_aCrowdInfo.nCroedTutorial = 0;	//チュートリアル保存用

	//群衆の数の情報
	g_aCrowdNum.nNum = 0;			//群衆の数
	g_aCrowdNum.nNumOld = 0;		//過去の群衆の数

	//ハートパーティクルのクールダウン
	g_nHartSoundCooltime = 0;

	//隊列の位置初期化
	for (int nCntCrowd = 0; nCntCrowd < MAX_MOB; nCntCrowd++)
	{
		FormationPos[nCntCrowd] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

}

//==================================================================================
//群衆の終了処理
//==================================================================================
void UninitCrowd(void)
{
	StopSound();

	//ファイル数分繰り返す
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCrowd) / sizeof(*c_apFilenameCrowd); nCntTex++)
	{
		if (g_apTextureCrowd[nCntTex] != NULL)
		{
			g_apTextureCrowd[nCntTex]->Release();
			g_apTextureCrowd[nCntTex] = NULL;
		}
	}
}

//==================================================================================
//群衆の更新処理
//==================================================================================
void UpdateCrowd(void)
{
	//モデルの情報取得
	ModelModel *pModel = GetModel();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//バディーの情報取得
	Buddy *pBuddy = GetBuddy();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	//目標の人数の情報取得
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

#if _DEBUG
	if (GetKeyboardPress(DIK_G) == true)
	{//Yキーが押された,視点上移動

		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33, 0, PARENT_PLAYER);
		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33, 0, PARENT_PLAYER);
		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33, 0, PARENT_PLAYER);
		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33, 0, PARENT_PLAYER);
		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33,0, PARENT_PLAYER);
	
	}

	if (GetKeyboardTrigger(DIK_F) == true)
	{//Yキーが押された,視点上移動

		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33,0, PARENT_PLAYER);
	}
#endif

	int nCntUse = 0, nCntPlayer = 0, nCntDebu = 0, nCntGari = 0;
	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aCrowd[nCntCrowd].aModel.bUse == true)
		{//使用していたら

			//過去の位置保存
			g_aCrowd[nCntCrowd].aModel.posOld = g_aCrowd[nCntCrowd].aModel.pos;

			//過去の群衆の数保存
			g_aCrowdNum.nNumOld = g_aCrowdNum.nNum;

			if (g_aCrowd[nCntCrowd].nState != CROWDSTATE_FADE)
			{
				//派遣時
				if (g_aCrowd[nCntCrowd].nParent == PARENT_PLAYER)
				{//プレイヤー

					//pPlayer->aFormation[g_aCrowd[nCntCrowd].aFormation.nIndex].bUse = false;

					//隊列の位置
					g_aCrowd[nCntCrowd].formationPos = pPlayer->aFormation[nCntPlayer].pos;

					nCntPlayer++;
				}

				if (g_aCrowd[nCntCrowd].nParent == PARENT_GARI)
				{//ヒョロガリメガネ陰キャ

					//隊列の位置
					//g_aCrowd[nCntCrowd].formationPos = pBuddy[BUDDYTYPE_HYOROGARI].aFormation[g_aCrowd[nCntCrowd].aFormation.nIndex].pos;
					g_aCrowd[nCntCrowd].formationPos = pBuddy[BUDDYTYPE_HYOROGARI].aFormation[nCntGari].pos;

					nCntGari++;
				}

				if (g_aCrowd[nCntCrowd].nParent == PARENT_CHIBI)
				{//クソチビ
					
					//隊列の位置
					//g_aCrowd[nCntCrowd].formationPos = pBuddy[BUDDYTYPE_DEPPA].aFormation[g_aCrowd[nCntCrowd].aFormation.nIndex].pos;
					g_aCrowd[nCntCrowd].formationPos = pBuddy[BUDDYTYPE_DEPPA].aFormation[nCntDebu].pos;

					nCntDebu++;
				}

				//使用のカウント加算
				nCntUse++;

				if (g_aCrowd[nCntCrowd].nState != CROWDSTATE_WAIT)
				{
					//群衆の操作
					ControllCrowd(nCntCrowd);

					//群衆同士の当たり判定
					CollisionModelCrowd(nCntCrowd);

					//群衆のカバー投げ処理
					ThrowPillowCrowd(nCntCrowd);

					//座標制限
					LimitPosCrowd(nCntCrowd);

					//追従カウントに達していたら
					if (g_aCrowd[nCntCrowd].nCntChase >= CHASE_CNT)
					{
						g_aCrowd[nCntCrowd].nCntChase = CHASE_CNT;
						g_aCrowd[nCntCrowd].nState = CROWDSTATE_CHASE;
					}
				}
			}

			//状態更新
			UpdateStateCrowd(nCntCrowd);

			//影の位置更新
			SetPositionShadow(g_aCrowd[nCntCrowd].aModel.nIdxShadow, g_aCrowd[nCntCrowd].aModel.pos);
		}
	}

	//総数加算
	g_aCrowdInfo.nCrowdValue = nCntUse;

	if (GetMode() == MODE_TUTORIAL && pTutorial->nStep < TUTORIAL_STEP_HYPNOSISFEVER)
	{
		g_aCrowdInfo.nCroedTutorial = nCntUse;
	}

	//目標の人数
	if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_HYPNOSISFEVER)
	{
		pGetnTargetNop->nCntPeople = 10 - g_aCrowdInfo.nCrowdValue + g_aCrowdInfo.nCroedTutorial;    //目標の人数を減らす
	}
	else
	{
		pGetnTargetNop->nCntPeople = TARGETHITO - g_aCrowdInfo.nCrowdValue - 3;    //目標の人数を減らす
	}

	//保存
	g_aCrowdInfo.nCrowdKeep = g_aCrowdInfo.nCrowdValue;

	//親がプレイヤーの数代入
	g_aCrowdInfo.nParentPlayerNum = nCntPlayer;

	//クールタイム減少
	g_nHartSoundCooltime--;

	if (g_nHartSoundCooltime <= 0)
	{//0を下回ったら

		//固定
		g_nHartSoundCooltime = 0;
	}

	if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_MORS && g_aCrowdInfo.nCrowdValue == 0)
	{//モードがチュートリアル && MORSのステップ以上 && モブが0

		//催眠完了状態
		pTutorial->aControl.bReleaseCrowd = true;
	}
}

//==================================================================================
//群衆の操作
//==================================================================================
void ControllCrowd(int nCntCrowd)
{

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//バディーの情報取得
	Buddy *pBuddy = GetBuddy();

	//重力処理
	g_aCrowd[nCntCrowd].aModel.move.y -= GRAVITY;

	float fRotDest = g_aCrowd[nCntCrowd].aModel.rot.y;	//目標の角度
	float fRotDiff = 0.0f;	//現在と目標の差分

	//目標の角度を求める
	fRotDest = atan2f((g_aCrowd[nCntCrowd].aModel.pos.x - g_aCrowd[nCntCrowd].formationPos.x), (g_aCrowd[nCntCrowd].aModel.pos.z - g_aCrowd[nCntCrowd].formationPos.z));
	fRotDiff = fRotDest - g_aCrowd[nCntCrowd].aModel.rot.y;

	//角度の正規化
	RotNormalize(&fRotDiff);

	//角度の補正をする
	g_aCrowd[nCntCrowd].aModel.rot.y += fRotDiff * 0.2f;

	//角度の正規化
	RotNormalize(&g_aCrowd[nCntCrowd].aModel.rot.y);

	if (CircleRange(g_aCrowd[nCntCrowd].formationPos, g_aCrowd[nCntCrowd].aModel.pos) >= (50.0f * 50.0f) && g_aCrowd[nCntCrowd].nState != CROWDSTATE_JOIN)
	{//まだ規定値に着いていなかったら && 何もない状態

		if (g_aCrowd[nCntCrowd].nState != CROWDSTATE_RETURN)
		{
			g_aCrowd[nCntCrowd].aModel.move.x += sinf(D3DX_PI + g_aCrowd[nCntCrowd].aModel.rot.y) * CHARA_MOVE;
			g_aCrowd[nCntCrowd].aModel.move.z += cosf(D3DX_PI + g_aCrowd[nCntCrowd].aModel.rot.y) * CHARA_MOVE;
		}
		else if (g_aCrowd[nCntCrowd].nState == CROWDSTATE_RETURN)
		{
			g_aCrowd[nCntCrowd].aModel.move.x += sinf(D3DX_PI + g_aCrowd[nCntCrowd].aModel.rot.y) * RETURN_MOVE;
			g_aCrowd[nCntCrowd].aModel.move.z += cosf(D3DX_PI + g_aCrowd[nCntCrowd].aModel.rot.y) * RETURN_MOVE;
		}
	}

	if (g_aCrowd[nCntCrowd].bFever == true)
	{//フィーバー中なら

		//加速
		g_aCrowd[nCntCrowd].aModel.move.x *= 1.1f;
		g_aCrowd[nCntCrowd].aModel.move.z *= 1.1f;
	}

	//位置更新
	g_aCrowd[nCntCrowd].aModel.pos += g_aCrowd[nCntCrowd].aModel.move;
	//CollisionModelCrowd(nCntCrowd);

	//慣性
	g_aCrowd[nCntCrowd].aModel.move.x += (0.0f - g_aCrowd[nCntCrowd].aModel.move.x) * MOVE_FACTOR;
	g_aCrowd[nCntCrowd].aModel.move.z += (0.0f - g_aCrowd[nCntCrowd].aModel.move.z) * MOVE_FACTOR;

	if (CircleRange(g_aCrowd[nCntCrowd].formationPos, g_aCrowd[nCntCrowd].aModel.pos) <= (50.0f * 50.0f) && g_aCrowd[nCntCrowd].nState == CROWDSTATE_CHASE)
	{//規定値に着いてる && 追い掛け状態

		//なにもしてない状態にする
		g_aCrowd[nCntCrowd].nState = CROWDSTATE_NONE;
		g_aCrowd[nCntCrowd].nCntChase = 0;
	}
}

//==================================================================================
//群衆の座標制限
//==================================================================================
void LimitPosCrowd(int nCntCrowd)
{

	if (g_aCrowd[nCntCrowd].aModel.pos.y < 0.0f)
	{//地面より下に行ったら

		g_aCrowd[nCntCrowd].aModel.pos.y = 0.0f;

		if (g_aCrowd[nCntCrowd].nState == CROWDSTATE_JOIN)
		{
			g_aCrowd[nCntCrowd].nState = CROWDSTATE_NONE;
		}

		//重力処理
		g_aCrowd[nCntCrowd].aModel.move.y = 0.0f;
	}

	//移動制限
	if (g_aCrowd[nCntCrowd].aModel.pos.x + g_aCrowd[nCntCrowd].aModel.vtxMin.x < -LIMIT_POS) { g_aCrowd[nCntCrowd].aModel.pos.x = -LIMIT_POS - g_aCrowd[nCntCrowd].aModel.vtxMin.x; }
	if (g_aCrowd[nCntCrowd].aModel.pos.x + g_aCrowd[nCntCrowd].aModel.vtxMax.x > LIMIT_POS) { g_aCrowd[nCntCrowd].aModel.pos.x = LIMIT_POS - g_aCrowd[nCntCrowd].aModel.vtxMax.x; }
	if (g_aCrowd[nCntCrowd].aModel.pos.z + g_aCrowd[nCntCrowd].aModel.vtxMax.z > LIMIT_POS) { g_aCrowd[nCntCrowd].aModel.pos.z = LIMIT_POS - g_aCrowd[nCntCrowd].aModel.vtxMax.z; }
	if (g_aCrowd[nCntCrowd].aModel.pos.z + g_aCrowd[nCntCrowd].aModel.vtxMin.z < -LIMIT_POS) { g_aCrowd[nCntCrowd].aModel.pos.z = -LIMIT_POS - g_aCrowd[nCntCrowd].aModel.vtxMin.z; }

}

//==================================================================================
//群衆の状態更新処理
//==================================================================================
void UpdateStateCrowd(int nCntCrowd)
{

	//影の情報取得
	SHADOW *pShadow = GetShadow();

	//目標の人数の情報取得
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

	switch (g_aCrowd[nCntCrowd].nState)
	{
	case CROWDSTATE_NONE:
		break;

	case CROWDSTATE_JOIN:
		break;

	case CROWDSTATE_FADE:
		//CrowdInfo *pCrowdInfo = GetCrowdInfo();

		//消していく
		g_aCrowd[nCntCrowd].fFadeAlpha -= 0.01f;
		g_aCrowd[nCntCrowd].nState = CROWDSTATE_FADE;

		if (g_aCrowd[nCntCrowd].fFadeAlpha <= 0.0f)
		{
			//消していく
			g_aCrowd[nCntCrowd].aModel.bUse = false;

			//影も消す
			pShadow[g_aCrowd[nCntCrowd].aModel.nIdxShadow].bUse = false;

			//目標の人数を増やす
			g_aCrowdInfo.nCrowdValue++;
			
		}
		break;

	case CROWDSTATE_WAIT:

		//射撃待ち状態にする
		g_aCrowd[nCntCrowd].nState = CROWDSTATE_WAIT;

		g_aCrowd[nCntCrowd].nCntChase = 0;

		//移動量を0にする
		g_aCrowd[nCntCrowd].aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;

	case CROWDSTATE_RETURN:

		if (CircleRange(g_aCrowd[nCntCrowd].formationPos, g_aCrowd[nCntCrowd].aModel.pos) <= (50.0f * 50.0f))
		{//規定値に着いてる

			g_aCrowd[nCntCrowd].nState = CROWDSTATE_NONE;
		}
		break;
	}
	
}

//==================================================================================
//群衆の抱き枕カバー投げ処理
//==================================================================================
void ThrowPillowCrowd(int nCntCrowd)
{
	//モブの情報取得
	Mob *pMob = GetMob();

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS)
		{//使用されている && 催眠状態

			if (CircleRange(g_aCrowd[nCntCrowd].aModel.pos, pMob->aModel.pos) <= (g_aCrowd[nCntCrowd].fThrowRadius * g_aCrowd[nCntCrowd].fThrowRadius))
			{//探索サークルの中に入ったら

				if (g_aCrowd[nCntCrowd].nThrowCnt >= 1)
				{//クールタイム中だったら

					g_aCrowd[nCntCrowd].nThrowCnt = (g_aCrowd[nCntCrowd].nThrowCnt + 1) % THROW_COOLTIME;
				}

				if (g_aCrowd[nCntCrowd].nThrowCnt <= 0)
				{
					//抱き枕カバーの向きを求める
					float fRotDest = atan2f((g_aCrowd[nCntCrowd].aModel.pos.x - pMob->aModel.pos.x), (g_aCrowd[nCntCrowd].aModel.pos.z - pMob->aModel.pos.z));

					//抱き枕の設定処理
					SetCutepillow(g_aCrowd[nCntCrowd].aModel.pos, D3DXVECTOR3(0.0f, fRotDest, 0.0f), g_aCrowd[nCntCrowd].nParent);

					//クールタイム設定
					g_aCrowd[nCntCrowd].nThrowCnt = 1;
				}
				break;
			}
		}
	}
}

//==================================================================================
//群衆の行動更新処理
//==================================================================================
void UpdateActionCrowd(int nCntCrowd)
{
	
}

//==================================================================================
//オブジェクトとの当たり判定
//==================================================================================
void CollisionModelCrowd(int nCntCrowd)
{

	//モデルの情報取得
	ModelModel *pModel = GetModel();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	bool bHit = false;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true && g_aCrowd[nCntCrowd].nState != CROWDSTATE_CHASE && pModel->nType < SMALL_NUM)
		{//モデルが使用されていたら && 追い掛け状態じゃなかったら

			//当たっていたら
			if (bHitCharacter(
				&g_aCrowd[nCntCrowd].aModel.pos, &g_aCrowd[nCntCrowd].aModel.posOld, g_aCrowd[nCntCrowd].aModel.vtxMax, g_aCrowd[nCntCrowd].aModel.vtxMin,
				&pModel->pos, pModel->vtxMax, pModel->vtxMin) == true)
			{
				bHit = true;
			}

			//キャラクター同士の当たり判定
			CollisionCharacter(
				&g_aCrowd[nCntCrowd].aModel.pos, &g_aCrowd[nCntCrowd].aModel.posOld, g_aCrowd[nCntCrowd].aModel.vtxMax, g_aCrowd[nCntCrowd].aModel.vtxMin,
				&pModel->pos, pModel->vtxMax, pModel->vtxMin);
		}
	}

	//当たっていたら
	if (bHit == true)
	{
		//追従カウントを加算
		g_aCrowd[nCntCrowd].nCntChase++;
	}
	else
	{
		//追従カウントを減算
		g_aCrowd[nCntCrowd].nCntChase--;

		if (g_aCrowd[nCntCrowd].nCntChase <= 0)
		{
			g_aCrowd[nCntCrowd].nCntChase = 0;
		}
	}

}

//==================================================================================
//群衆の描画処理
//==================================================================================
void DrawCrowd(void)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス宣言
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	//D3DXMATERIAL mMatRed;			//ダメージマテリアル
	D3DXMATERIAL mFade;				//フェードマテリアル
	D3DXMATERIAL mDefalt;			//デフォルトマテリアル

	//他の情報クリア
	ZeroMemory(&mDefalt, sizeof(D3DXMATERIAL));
	mDefalt.MatD3D.Diffuse = D3DXCOLOR(0.59f, 0.40f, 0.94f, 1.0f);
	mDefalt.MatD3D.Ambient = D3DXCOLOR(0.59f, 0.40f, 0.94f, 1.0f);


	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aCrowd[nCntCrowd].aModel.bUse == true)
		{//使用していたら

			//ワールドマトリックスの初期化
			D3DXMatrixIdentity(&g_aCrowd[nCntCrowd].aModel.mtxWorld);

			//向きを反映する
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCrowd[nCntCrowd].aModel.rot.y, g_aCrowd[nCntCrowd].aModel.rot.x, g_aCrowd[nCntCrowd].aModel.rot.z);
			D3DXMatrixMultiply(&g_aCrowd[nCntCrowd].aModel.mtxWorld, &g_aCrowd[nCntCrowd].aModel.mtxWorld, &mtxRot);

			//位置を反映する
			D3DXMatrixTranslation(&mtxTrans, g_aCrowd[nCntCrowd].aModel.pos.x, g_aCrowd[nCntCrowd].aModel.pos.y, g_aCrowd[nCntCrowd].aModel.pos.z);
			D3DXMatrixMultiply(&g_aCrowd[nCntCrowd].aModel.mtxWorld, &g_aCrowd[nCntCrowd].aModel.mtxWorld, &mtxTrans);

			//ワールドマトリックスの設定
			pDevice->SetTransform(D3DTS_WORLD, &g_aCrowd[nCntCrowd].aModel.mtxWorld);

			//現在のマテリアルを取得
			pDevice->GetMaterial(&matDef);

			//マテリアルデータへのポインタを取得
			pMat = (D3DXMATERIAL*)g_aCrowd[nCntCrowd].aModel.pBuffMat->GetBufferPointer();

			//頂点数分繰り返し
			for (int nCntMat = 0; nCntMat < (int)g_aCrowd[nCntCrowd].aModel.dwNumMat; nCntMat++)
			{
				if (GetMode() != MODE_RESULT)
				{

					//マテリアルの設定
					switch (g_aCrowd[nCntCrowd].nState)
					{
					case CROWDSTATE_FADE:
						mFade.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, g_aCrowd[nCntCrowd].fFadeAlpha);
						mFade.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, g_aCrowd[nCntCrowd].fFadeAlpha);
						pDevice->SetMaterial(&mFade.MatD3D);
						break;

					default:
						pDevice->SetMaterial(&mDefalt.MatD3D);
						//pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
						break;
					}
				}
				else
				{
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}

				//テクスチャの設定
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//テクスチャの設定
					pDevice->SetTexture(0, g_apTextureCrowd[g_aCrowd[nCntCrowd].nTexType]);
					//pDevice->SetTexture(0, NULL);
				}
				else
				{
					pDevice->SetTexture(0, NULL);
				}

				//群衆(パーツ)の描画
				g_aCrowd[nCntCrowd].aModel.pMesh->DrawSubset(nCntMat);
			}

			//保存していたマテリアルを戻す
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//群衆の位置設定
//==================================================================================
int SetFormationPos(int nParent)
{
	int nIndex = -1;

	//隊列の位置設定
	switch (nParent)
	{
	case PARENT_PLAYER:
		nIndex = SetFormationPosPlayer();
		break;

	case PARENT_GARI:
		nIndex = SetFormationPosBuddy(BUDDYTYPE_HYOROGARI);
		break;

	case PARENT_CHIBI:
		nIndex = SetFormationPosBuddy(BUDDYTYPE_DEPPA);
		break;
	}

	return nIndex;
}

//==================================================================================
//群衆の設定処理
//==================================================================================
void SetCrowd (D3DXVECTOR3 pos, int nType, int nTexType, int nParent)
{
	//モデルの情報取得
	Model *pModel = GetXLoadData();

	//目標の人数の情報取得
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aCrowd[nCntCrowd].aModel.bUse == false)
		{//使用していなかったら

			//モデル情報取得
			g_aCrowd[nCntCrowd].aModel = pModel[nType];
			g_aCrowd[nCntCrowd].aModel.pos = pos;	//位置
			g_aCrowd[nCntCrowd].nState = CROWDSTATE_JOIN;	//状態
			g_aCrowd[nCntCrowd].aModel.move.y += 10.0f;
			g_aCrowd[nCntCrowd].aModel.bUse = true;	//使用している状況にする
			g_aCrowd[nCntCrowd].fFadeAlpha = 1.0f;	//フェードの不透明度
			g_aCrowd[nCntCrowd].nThrowCnt = 0;			//投げるカウント
			g_aCrowd[nCntCrowd].nParent = nParent;			//投げるカウント
			g_aCrowd[nCntCrowd].nTexType = nTexType;	//テクスチャタイプ

			//pGetnTargetNop->nCntPeople--;    //目標の人数を減らす

			g_aCrowdNum.nNum++;		//群衆の数追加

			if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_SENNOU)
			{//モードがチュートリアル && 洗脳のステップ以上

				//洗脳完了状態
				pTutorial->aControl.bCutePillow = true;

				if (pTutorial->nStep >= TUTORIAL_STEP_HYPNOSISFEVER)
				{//フィーバー洗脳のステップ以上

				 //フィーバー時に捕まえた人数加算
					pTutorial->aControl.nCntHypnosisFever++;
				}
			}

			if (pPlayer->bFever == true)
			{//フィーバー中の時

				g_aCrowd[nCntCrowd].bFever = true;
			}

			//モデルの全頂点チェック
			CheckVtx(g_aCrowd[nCntCrowd].aModel.rot.y, &g_aCrowd[nCntCrowd].aModel.vtxMax, &g_aCrowd[nCntCrowd].aModel.vtxMin, g_aCrowd[nCntCrowd].aModel.pMesh, g_aCrowd[nCntCrowd].aModel.pVtxBuff);

			//隊列の位置設定
			g_aCrowd[nCntCrowd].aFormation.nIndex = SetFormationPos(nParent);

			//影を設定
			g_aCrowd[nCntCrowd].aModel.nIdxShadow = SetShadow(g_aCrowd[nCntCrowd].aModel.vtxMax.x * 1.5f, g_aCrowd[nCntCrowd].aModel.vtxMax.z * 1.5f);

			//影の位置更新
			SetPositionShadow(g_aCrowd[nCntCrowd].aModel.nIdxShadow, g_aCrowd[nCntCrowd].aModel.pos);

			if (GetMode() != MODE_RESULT && g_nHartSoundCooltime <= 0)
			{//サウンドのクールタイムが終わっていたら

				//ハートのパーティクルをセット
				SetParticle(D3DXVECTOR3(g_aCrowd[nCntCrowd].aModel.pos.x, g_aCrowd[nCntCrowd].aModel.pos.y, g_aCrowd[nCntCrowd].aModel.pos.z), PARTICLE_TYPE_HART);

				//SEをセット
				PlaySound(SOUND_LABEL_SE_HYPNOSISEND);

				//クールタイム設定
				g_nHartSoundCooltime = HART_COOLTIME;
			}
			break;
		}
	}
}

//==================================================================================
//群衆の設定処理
//==================================================================================
void SetCrowdResult(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexType, int nParent)
{
	//モデルの情報取得
	Model *pModel = GetXLoadData();

	//目標の人数の情報取得
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//プレイヤーの情報取得
	Player *pPlayer = GetPlayer();

	//チュートリアルの情報取得
	Tutorial *pTutorial = GetTutorial();

	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aCrowd[nCntCrowd].aModel.bUse == false)
		{//使用していなかったら

		 //モデル情報取得
			g_aCrowd[nCntCrowd].aModel = pModel[nType];

			g_aCrowd[nCntCrowd].aModel.rot = rot;
			g_aCrowd[nCntCrowd].aModel.pos = pos;	//位置
			g_aCrowd[nCntCrowd].nState = CROWDSTATE_JOIN;	//状態

			g_aCrowd[nCntCrowd].aModel.move.y = (float)((rand() % 19 - 9)) * 0.01f;

			g_aCrowd[nCntCrowd].aModel.bUse = true;			//使用している状況にする
			g_aCrowd[nCntCrowd].fFadeAlpha = 1.0f;			//フェードの不透明度
			g_aCrowd[nCntCrowd].nThrowCnt = 0;				//投げるカウント
			g_aCrowd[nCntCrowd].nParent = nParent;			//投げるカウント
			g_aCrowd[nCntCrowd].nTexType = nTexType;		//テクスチャタイプ

			//pGetnTargetNop->nCntPeople--;    //目標の人数を減らす

			g_aCrowdNum.nNum++;		//群衆の数追加

			if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_SENNOU)
			{//モードがチュートリアル && 洗脳のステップ以上

				//洗脳完了状態
				pTutorial->aControl.bCutePillow = true;
			}

			if (pPlayer->bFever == true)
			{//フィーバー中の時

				g_aCrowd[nCntCrowd].bFever = true;
			}

			//モデルの全頂点チェック
			CheckVtx(g_aCrowd[nCntCrowd].aModel.rot.y, &g_aCrowd[nCntCrowd].aModel.vtxMax, &g_aCrowd[nCntCrowd].aModel.vtxMin, g_aCrowd[nCntCrowd].aModel.pMesh, g_aCrowd[nCntCrowd].aModel.pVtxBuff);

			//隊列の位置設定
			g_aCrowd[nCntCrowd].aFormation.nIndex = SetFormationPos(nParent);

			//影を設定
			g_aCrowd[nCntCrowd].aModel.nIdxShadow = SetShadow(g_aCrowd[nCntCrowd].aModel.vtxMax.x * 1.5f, g_aCrowd[nCntCrowd].aModel.vtxMax.z * 1.5f);

			//影の位置更新
			SetPositionShadow(g_aCrowd[nCntCrowd].aModel.nIdxShadow, g_aCrowd[nCntCrowd].aModel.pos);
			break;
		}
	}
}


//==================================================================================
//群衆の情報取得
//==================================================================================
Crowd *GetCrowd(void)
{
	return &g_aCrowd[0];
}

//==================================================================================
//群衆の情報取得
//==================================================================================
CrowdInfo *GetCrowdInfo(void)
{
	return &g_aCrowdInfo;
}

//==================================================================================
//群衆の数の情報取得
//==================================================================================
CrowdNum *GetCrowdNum(void)
{
	return &g_aCrowdNum;
}

//==================================================================================
//リザルト時の群衆の更新処理
//==================================================================================
void UpdateResultCrowd(void)
{
	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		//重力加速度
		g_aCrowd[nCntCrowd].aModel.move.y -= 0.1f;

		//重力
		g_aCrowd[nCntCrowd].aModel.pos.y += g_aCrowd[nCntCrowd].aModel.move.y;

		if (g_aCrowd[nCntCrowd].aModel.pos.y <= 0)
		{//0以下の時

			g_aCrowd[nCntCrowd].aModel.move.y *= -1;

			g_aCrowd[nCntCrowd].aModel.move.x = ((float)(rand() % 629 - 314) / 100.0f) * 5.0f;
		}

		g_aCrowd[nCntCrowd].aModel.pos.x += g_aCrowd[nCntCrowd].aModel.move.x;

		if (g_aCrowd[nCntCrowd].aModel.pos.x <= -900.0f)
		{
			g_aCrowd[nCntCrowd].aModel.pos.x = -900.0f;
		}
		else if (g_aCrowd[nCntCrowd].aModel.pos.x >= 900.0f)
		{
			g_aCrowd[nCntCrowd].aModel.pos.x = 900.0f;
		}

	}
}
