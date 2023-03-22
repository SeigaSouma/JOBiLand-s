//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "main.h"
#include "particle.h"
#include "input.h"
#include "effect.h"
#include "2D_effect.h"
#include "player.h"

//マクロ定義
#define MAX_PARTICLE	(12800)	//パーティクルの最大数
#define SMOKE_LIFE		(40)	//煙の寿命
#define FIRE_LIFE		(60)	//火の寿命
#define HEAT_LIFE		(20)	//温度上昇の寿命
#define COOL_LIFE		(30)	//温度減少の寿命
#define OFFSETTING_LIFE	(60)	//打ち消しの寿命

//プロトタイプ宣言
void SmokeParticle(int nCntParticle);
void SmokeParticleRed(int nCntParticle);
void SmokeParticleYel(int nCntParticle);
void FireParticle(int nCntParticle);
void FireParticleYellow(int nCntParticle);
void OffSettingParticle(int nCntParticle);
void LandParticle(int nCntParticle);
void WalkParticle(int nCntParticle);
void PillowThrowParticle(int nCntParticle);
void HartParticle(int nCntParticle);
void TitleLogParticle(int nCntParticle);
void ManyUIParticle(int nMaxUse, int nSmallUse, int nCntParticle);
void ManyUI_MiniParticle(int nCntParticle);
void SpawnMORSParticle(int nCntParticle);
void ResultMessageParticle(int nCntParticle);
void FeverPushParticle(int nCntParticle);

//グローバル変数宣言
Particle g_aParticle[MAX_PARTICLE];			//パーティクルの情報
D3DXCOLOR g_Smokecol[MAX_PARTICLE];			//煙の色
int g_nParticleNum;

//==================================================================================
//パーティクルの初期化処理
//==================================================================================
void InitParticle(void)
{
	int nCntParticle;

	//パーティクルの情報の初期化
	for (nCntParticle =0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		g_aParticle[nCntParticle].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].posOrigin = g_aParticle[nCntParticle].pos;
		g_aParticle[nCntParticle].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aParticle[nCntParticle].fRadius = 0.0f;
		g_aParticle[nCntParticle].nLife = 0;
		g_aParticle[nCntParticle].nMaxLife = g_aParticle[nCntParticle].nLife;
		g_aParticle[nCntParticle].nType = 0;
		g_aParticle[nCntParticle].bUse = false;				//使用していない状態にする

		g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
	}
	g_nParticleNum = 0;
}

//==================================================================================
//パーティクルの終了処理
//==================================================================================
void UninitParticle(void)
{
	
}

//==================================================================================
//パーティクルの更新処理
//==================================================================================
void UpdateParticle(void)
{

	
}

//==================================================================================
//パーティクルの描画処理
//==================================================================================
void DrawParticle(void)
{
	
}

//==================================================================================
//パーティクルの設定処理
//==================================================================================
void SetParticle(D3DXVECTOR3 pos, int nType)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//パーティクルが使用されていなかったら

			g_aParticle[nCntParticle].pos = pos;
			g_aParticle[nCntParticle].posOrigin = pos;
			g_aParticle[nCntParticle].nType = nType;
			g_aParticle[nCntParticle].bUse = true;


			switch (nType)
			{
			case PARTICLE_TYPE_EXPLOSION:
				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				SmokeParticle(nCntParticle);
				SmokeParticleRed(nCntParticle);
				SmokeParticleYel(nCntParticle);
				break;

			case PARTICLE_TYPE_SMOKE:

				g_aParticle[nCntParticle].nLife = 30;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				SmokeParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_SMOKE_RED:

				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SmokeParticleRed(nCntParticle);
				break;


			case PARTICLE_TYPE_SMOKE_YEL:

				g_aParticle[nCntParticle].nLife = SMOKE_LIFE;
				g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				SmokeParticleYel(nCntParticle);
				break;
					

			case PARTICLE_TYPE_FIRE:

				g_aParticle[nCntParticle].nLife = FIRE_LIFE;
				FireParticle(nCntParticle);
				FireParticleYellow(nCntParticle);
				break;
					
			case PARTICLE_TYPE_FIRE_YEL:

				g_aParticle[nCntParticle].nLife = FIRE_LIFE;
				FireParticleYellow(nCntParticle);
				break;

			case PARTICLE_TYPE_LAND:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				LandParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_WALK:
				g_aParticle[nCntParticle].nLife = 40;
				WalkParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_PILLOWTHROW:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				PillowThrowParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_HART:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				HartParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_TITLELOG:
				g_aParticle[nCntParticle].nLife = OFFSETTING_LIFE;
				TitleLogParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_MANYTXT:
				g_aParticle[nCntParticle].nLife = 40;
				ManyUIParticle(60, 10, nCntParticle);
				break;

			case PARTICLE_TYPE_MANYTXT_MORS:
				g_aParticle[nCntParticle].nLife = 40;
				ManyUIParticle(40, 5, nCntParticle);
				break;

			case PARTICLE_TYPE_MANYTXT_MINI:
				g_aParticle[nCntParticle].nLife = 40;
				ManyUI_MiniParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_MORSSPAWN:
				g_aParticle[nCntParticle].nLife = 30;
				SpawnMORSParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_RESULTMESSAGE:
				g_aParticle[nCntParticle].nLife = 70;
				ResultMessageParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_FEVERPUSH:
				g_aParticle[nCntParticle].nLife = 40;
				FeverPushParticle(nCntParticle);
				break;

			case PARTICLE_TYPE_OFFSETTINGPARTICLE:
				g_aParticle[nCntParticle].nLife = 40;
				OffSettingParticle(nCntParticle);
				break;
			}

			//パーティクルの数加算
			g_nParticleNum++;
			g_aParticle[nCntParticle].bUse = false;

			g_aParticle[nCntParticle].nMaxLife = g_aParticle[nCntParticle].nLife;
			break;
		}

		
	}
}

//==================================================================================
//煙のパーティクル
//==================================================================================
void SmokeParticle(int nCntParticle)
{

	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 1.0f;		//移動量
		fAngle = (float)(rand() % 101 - 50) / 100.0f;	//角度

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.7f, 0.7f, 0.7f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a += 0.05f;

		g_aParticle[nCntParticle].fRadius = 20.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if(rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);

	}
}

//==================================================================================
//煙のパーティクル(赤)
//==================================================================================
void SmokeParticleRed(int nCntParticle)
{
	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 2.0f;		//移動量
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//角度

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.4f, 0.3f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a -= 0.02f;

		g_aParticle[nCntParticle].fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//煙のパーティクル(黄)
//==================================================================================
void SmokeParticleYel(int nCntParticle)
{
	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 2.0f;		//移動量
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//角度

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.8f, 0.5f, 0.0f, g_Smokecol[nCntParticle].a);
		g_Smokecol[nCntParticle].a -= 0.02f;

		g_aParticle[nCntParticle].fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//火のパーティクル
//==================================================================================
void FireParticle(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{

		fMove = (float)(rand() % 20) / 10 + 1.0f;		//移動量
		fMoveY = (float)(rand() % 60) / 10 + 10.0f;	//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].fRadius = 55.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.95f, 0.15f, 0.0f, 1.0f);

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//火のパーティクル(黄)
//==================================================================================
void FireParticleYellow(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 20) / 10 + 1.0f;		//移動量
		fMoveY = (float)(rand() % 60) / 10 + 10.0f;	//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].fRadius = 30.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//打ち消しのパーティクル
//==================================================================================
void OffSettingParticle(int nCntParticle)
{

	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{

		fMove = (float)(rand() % 10) / 1 + 3.0f;		//移動量
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//角度

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 40.0f;

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_NORMAL);
	}
}

//==================================================================================
// 着地のパーティクル
//==================================================================================
void LandParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 30; nCntUse++)
	{
		fMove = (float)(rand() % 61 - 30) / 10 + 1.0f;		//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		//出現地点のブレ
		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 10.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.9f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.8f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// 移動のパーティクル
//==================================================================================
void WalkParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 5; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 1.0f;		//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		//出現地点のブレ
		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.8f);

		g_aParticle[nCntParticle].fRadius = 5.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.98f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.92f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// 抱き枕カバー投げのパーティクル
//==================================================================================
void PillowThrowParticle(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 5; nCntUse++)
	{
		fMove = (float)(rand() % 5) / 10 + 1.0f;		//移動量
		fMoveY = (float)(rand() % 10) / 10 + 8.0f;	//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;

		//出現地点のブレ
		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.8f);

		g_aParticle[nCntParticle].fRadius = 30.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.98f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.92f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// ハートのパーティクル
//==================================================================================
void HartParticle(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 5; nCntUse++)
	{
		fMove = (float)(rand() % 5) / 10 + 1.0f;		//移動量
		fMoveY = (float)(rand() % 100) / 100 + 8.0f;	//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;

		//出現地点のブレ
		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.9f, 0.9f, 0.9f, 0.8f);

		g_aParticle[nCntParticle].fRadius = 5.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.98f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.92f;
		}

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_HART);
	}
}

//==================================================================================
// タイトルロゴ部分のパーティクル
//==================================================================================
void TitleLogParticle(int nCntParticle)
{
	float fMove;
	fMove = (float)(rand() % 20) / 10 + 4.0f;		//移動量

	for (int nCntCircle = 0; nCntCircle < 60; nCntCircle++)
	{
		float fRot = ((D3DX_PI * 2.0f) / (float)60) * nCntCircle;
		fRot -= D3DX_PI;

		if (fRot >= D3DX_PI * 0.0f && fRot < D3DX_PI * 0.5f)
		{
			fMove += 1.0f;
		}
		else if (fRot >= D3DX_PI * 0.5f && fRot < D3DX_PI * 1.0f)
		{
			fMove -= 1.0f;
		}
		else if (fRot >= -D3DX_PI * 1.0f && fRot < -D3DX_PI * 0.5f)
		{
			fMove += 1.0f;
		}
		else if (fRot >= -D3DX_PI * 0.5f && fRot <= -D3DX_PI * 0.0f)
		{
			fMove -= 1.0f;
		}

		if (fMove >= 20.0f)
		{
			fMove = 20.0f;
		}
		if (fMove <= 10.0f)
		{
			fMove = 10.0f;
		}

		for (int nCntUse = 0; nCntUse < 40; nCntUse++)
		{


			float fMooooooooove = (float)(rand() % 20) / 10 + 1.0f;		//バラバラ移動量

			if (nCntUse >= 20)
			{
				fMooooooooove = (float)(rand() % 20) / 10 + 4.0f;		//移動量
			}

			//移動量の設定
			g_aParticle[nCntParticle].move.x = sinf(fRot) * (fMove - fMooooooooove);
			g_aParticle[nCntParticle].move.z = 0.0f;
			g_aParticle[nCntParticle].move.y = cosf(fRot) * (fMove - fMooooooooove);

			//出現地点のブレ
			//g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
			//g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

			g_aParticle[nCntParticle].col = D3DXCOLOR(0.9f, 0.5f, 0.1f, 1.0f);

			g_aParticle[nCntParticle].fRadius = 10.0f;

			if (rand() % 3 == 0)
			{
				g_aParticle[nCntParticle].fRadius *= 0.95f;
			}
			else if (rand() % 3 == 1)
			{
				g_aParticle[nCntParticle].fRadius *= 0.98f;
			}
			else if (rand() % 3 == 2)
			{
				g_aParticle[nCntParticle].fRadius *= 0.92f;
			}

			//エフェクトの設定
			SetEffect_2D(g_aParticle[nCntParticle].pos,
				g_aParticle[nCntParticle].col,
				g_aParticle[nCntParticle].fRadius,
				g_aParticle[nCntParticle].nLife,
				g_aParticle[nCntParticle].move,
				MOVEEFFECT_NONE, EFFECTTYPE_JUJI);
		}
	}
}

//==================================================================================
// 大量発生のパーティクル
//==================================================================================
void ManyUIParticle(int nMaxUse, int nSmallUse, int nCntParticle)
{
	float fMove;
	fMove = (float)(rand() % 30) / 10 + 5.0f;		//移動量

	float fff = (D3DX_PI * 2.0f) / nMaxUse;

	for (int nCntCircle = 0; nCntCircle < nMaxUse; nCntCircle++)
	{
		float fRot = ((D3DX_PI * 2.0f) / (float)((nMaxUse / 2) % (nCntCircle + 1))) * nCntCircle;
		fRot = fff * nCntCircle;
		//fRot -= D3DX_PI;

		for (int nCntUse = 0; nCntUse < nSmallUse; nCntUse++)
		{
			float fMooooooooove = (float)(rand() % 31 - 15) / 10;		//バラバラ移動量

			float fRotRand = (float)(rand() % 21 - 10) / 100.0f;		//バラバラ向き

			//移動量の設定
			g_aParticle[nCntParticle].move.x = sinf(fRot + fRotRand) * (fMove + fMooooooooove);
			g_aParticle[nCntParticle].move.z = 0.0f;
			g_aParticle[nCntParticle].move.y = cosf(fRot + fRotRand) * (fMove + fMooooooooove);

			//出現地点のブレ
			//g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
			//g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.1f;

			g_aParticle[nCntParticle].col = D3DXCOLOR(0.9f, 0.5f, 0.1f, 1.0f);

			g_aParticle[nCntParticle].fRadius = 15.0f;

			if (rand() % 3 == 0)
			{
				g_aParticle[nCntParticle].fRadius *= 0.95f;
			}
			else if (rand() % 3 == 1)
			{
				g_aParticle[nCntParticle].fRadius *= 0.98f;
			}
			else if (rand() % 3 == 2)
			{
				g_aParticle[nCntParticle].fRadius *= 0.92f;
			}

			//エフェクトの設定
			SetEffect_2D(g_aParticle[nCntParticle].pos,
				g_aParticle[nCntParticle].col,
				g_aParticle[nCntParticle].fRadius,
				g_aParticle[nCntParticle].nLife,
				g_aParticle[nCntParticle].move,
				MOVEEFFECT_NONE, EFFECTTYPE_NORMAL);
		}
	}
}

//==================================================================================
// 大量発生のパーティクル(ミニ)
//==================================================================================
void ManyUI_MiniParticle(int nCntParticle)
{
	float fMove;
	fMove = (float)(rand() % 30) / 10 + 2.0f;		//移動量

	for (int nCntCircle = 0; nCntCircle < 60; nCntCircle++)
	{
		float fRot = ((D3DX_PI * 2.0f) / (float)(30 % (nCntCircle + 1))) * nCntCircle;
		fRot -= D3DX_PI;

		for (int nCntUse = 0; nCntUse < 10; nCntUse++)
		{
			float fMooooooooove = (float)(rand() % 31 - 15) / 10;		//バラバラ移動量

			float fRotRand = (float)(rand() % 21 - 10) / 100.0f;		//バラバラ向き

			//移動量の設定
			g_aParticle[nCntParticle].move.x = sinf(fRot + fRotRand) * (fMove + fMooooooooove);
			g_aParticle[nCntParticle].move.z = 0.0f;
			g_aParticle[nCntParticle].move.y = cosf(fRot + fRotRand) * (fMove + fMooooooooove);

			g_aParticle[nCntParticle].col = D3DXCOLOR(0.9f, 0.5f, 0.1f, 1.0f);

			g_aParticle[nCntParticle].fRadius = 10.0f;

			if (rand() % 3 == 0)
			{
				g_aParticle[nCntParticle].fRadius *= 0.95f;
			}
			else if (rand() % 3 == 1)
			{
				g_aParticle[nCntParticle].fRadius *= 0.98f;
			}
			else if (rand() % 3 == 2)
			{
				g_aParticle[nCntParticle].fRadius *= 0.92f;
			}

			//エフェクトの設定
			SetEffect_2D(g_aParticle[nCntParticle].pos,
				g_aParticle[nCntParticle].col,
				g_aParticle[nCntParticle].fRadius,
				g_aParticle[nCntParticle].nLife,
				g_aParticle[nCntParticle].move,
				MOVEEFFECT_NONE, EFFECTTYPE_NORMAL);
		}
	}
}

//==================================================================================
// MORS出現のパーティクル
//==================================================================================
void SpawnMORSParticle(int nCntParticle)
{
	float fMoveX, fMoveY, fMoveZ;

	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{
		fMoveX = (float)(rand() % 300) / 100 + 1.0f;		//移動量
		fMoveY = (float)(rand() % 300) / 100 + 1.0f;		//移動量
		fMoveZ = (float)(rand() % 300) / 100 + 1.0f;		//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMoveX;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMoveZ;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 629 - 314) / 100.0f) * fMoveY;

		//出現地点のブレ
		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 2.5f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 2.5f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 2.5f;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.1f, 0.4f, 0.9f, 0.8f);

		g_aParticle[nCntParticle].fRadius = 25.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.98f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.92f;
		}

		int nnnn = (rand() % 2);

		//エフェクトの設定
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_THUNDER + nnnn);
	}
}

//==================================================================================
// 結果のメッセージのパーティクル
//==================================================================================
void ResultMessageParticle(int nCntParticle)
{
	float fMove;
	fMove = (float)(rand() % 30) / 10 + 5.0f;		//移動量

	float fff = (D3DX_PI * 2.0f) / 40;

	for (int nCntCircle = 0; nCntCircle < 40; nCntCircle++)
	{
		float fRot = fff * nCntCircle;

		for (int nCntUse = 0; nCntUse < 10; nCntUse++)
		{
			float fMooooooooove = (float)(rand() % 31 - 15) / 10;		//バラバラ移動量

			float fRotRand = (float)(rand() % 21 - 10) / 100.0f;		//バラバラ向き

																		//移動量の設定
			g_aParticle[nCntParticle].move.x = sinf(fRot + fRotRand) * (fMove + fMooooooooove);
			g_aParticle[nCntParticle].move.z = 0.0f;
			g_aParticle[nCntParticle].move.y = cosf(fRot + fRotRand) * (fMove + fMooooooooove);

			g_aParticle[nCntParticle].col = D3DXCOLOR(0.9f, 0.5f, 0.1f, 1.0f);

			g_aParticle[nCntParticle].fRadius = 15.0f;

			if (rand() % 3 == 0)
			{
				g_aParticle[nCntParticle].fRadius *= 0.95f;
			}
			else if (rand() % 3 == 1)
			{
				g_aParticle[nCntParticle].fRadius *= 0.98f;
			}
			else if (rand() % 3 == 2)
			{
				g_aParticle[nCntParticle].fRadius *= 0.92f;
			}

			//エフェクトの設定
			SetEffect_2D(g_aParticle[nCntParticle].pos,
				g_aParticle[nCntParticle].col,
				g_aParticle[nCntParticle].fRadius,
				g_aParticle[nCntParticle].nLife,
				g_aParticle[nCntParticle].move,
				MOVEEFFECT_NONE, EFFECTTYPE_NORMAL);
		}
	}





	
	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 50) * 0.1f;		//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = 0.0f;

		//出現地点のブレ
		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.z * 0.1f;
		g_aParticle[nCntParticle].pos.z = 0;

		g_aParticle[nCntParticle].col = D3DXCOLOR(0.9f, 0.6f, 0.3f, 0.8f);

		g_aParticle[nCntParticle].fRadius = 200.0f;

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.98f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.92f;
		}

		//エフェクトの設定
		SetEffect_2D(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_JUJI);
	}

}

//==================================================================================
// FEVERの押せのパーティクル
//==================================================================================
void FeverPushParticle(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{

		fMove = (float)(rand() % 20) / 10 + 1.0f;		//移動量
		fMoveY = (float)(rand() % 60) / 10;	//移動量

											//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 161 - 80) / 100.0f) * -fMoveY;
		g_aParticle[nCntParticle].move.z = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.1f;
		g_aParticle[nCntParticle].pos.z = 0.0f;

		g_aParticle[nCntParticle].fRadius = 55.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.95f, 0.15f, 0.0f, 0.3f);

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.92f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.98f;
		}

		//エフェクトの設定
		SetEffect_2D(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKE);
	}


	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{

		fMove = (float)(rand() % 20) / 10 + 1.0f;		//移動量
		fMoveY = (float)(rand() % 50) / 10;	//移動量

		//移動量の設定
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 161 - 80) / 100.0f) * -fMoveY;
		g_aParticle[nCntParticle].move.z = 0.0f;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.1f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.1f;
		g_aParticle[nCntParticle].pos.z = 0.0f;

		g_aParticle[nCntParticle].fRadius = 30.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 0.2f);

		if (rand() % 3 == 0)
		{
			g_aParticle[nCntParticle].fRadius *= 0.95f;
		}
		else if (rand() % 3 == 1)
		{
			g_aParticle[nCntParticle].fRadius *= 0.92f;
		}
		else if (rand() % 3 == 2)
		{
			g_aParticle[nCntParticle].fRadius *= 0.98f;
		}

		//エフェクトの設定
		SetEffect_2D(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKE);
	}

}

//==================================================================================
//パーティクル数取得
//==================================================================================
int GetParticleNum(void)
{
	return g_nParticleNum;
}
