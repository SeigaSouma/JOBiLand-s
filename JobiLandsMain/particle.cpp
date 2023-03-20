//=============================================================================
//
// �p�[�e�B�N������ [particle.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "particle.h"
#include "input.h"
#include "effect.h"
#include "2D_effect.h"
#include "player.h"

//�}�N����`
#define MAX_PARTICLE	(12800)	//�p�[�e�B�N���̍ő吔
#define SMOKE_LIFE		(40)	//���̎���
#define FIRE_LIFE		(60)	//�΂̎���
#define HEAT_LIFE		(20)	//���x�㏸�̎���
#define COOL_LIFE		(30)	//���x�����̎���
#define OFFSETTING_LIFE	(60)	//�ł������̎���

//�v���g�^�C�v�錾
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

//�O���[�o���ϐ��錾
Particle g_aParticle[MAX_PARTICLE];			//�p�[�e�B�N���̏��
D3DXCOLOR g_Smokecol[MAX_PARTICLE];			//���̐F
int g_nParticleNum;

//==================================================================================
//�p�[�e�B�N���̏���������
//==================================================================================
void InitParticle(void)
{
	int nCntParticle;

	//�p�[�e�B�N���̏��̏�����
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
		g_aParticle[nCntParticle].bUse = false;				//�g�p���Ă��Ȃ���Ԃɂ���

		g_Smokecol[nCntParticle] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		
	}
	g_nParticleNum = 0;
}

//==================================================================================
//�p�[�e�B�N���̏I������
//==================================================================================
void UninitParticle(void)
{
	
}

//==================================================================================
//�p�[�e�B�N���̍X�V����
//==================================================================================
void UpdateParticle(void)
{

	
}

//==================================================================================
//�p�[�e�B�N���̕`�揈��
//==================================================================================
void DrawParticle(void)
{
	
}

//==================================================================================
//�p�[�e�B�N���̐ݒ菈��
//==================================================================================
void SetParticle(D3DXVECTOR3 pos, int nType)
{

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE; nCntParticle++)
	{
		if (g_aParticle[nCntParticle].bUse == false)
		{//�p�[�e�B�N�����g�p����Ă��Ȃ�������

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

			//�p�[�e�B�N���̐����Z
			g_nParticleNum++;
			g_aParticle[nCntParticle].bUse = false;

			g_aParticle[nCntParticle].nMaxLife = g_aParticle[nCntParticle].nLife;
			break;
		}

		
	}
}

//==================================================================================
//���̃p�[�e�B�N��
//==================================================================================
void SmokeParticle(int nCntParticle)
{

	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 3; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 1.0f;		//�ړ���
		fAngle = (float)(rand() % 101 - 50) / 100.0f;	//�p�x

		//�ړ��ʂ̐ݒ�
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);

	}
}

//==================================================================================
//���̃p�[�e�B�N��(��)
//==================================================================================
void SmokeParticleRed(int nCntParticle)
{
	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 2.0f;		//�ړ���
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//�p�x

		//�ړ��ʂ̐ݒ�
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//���̃p�[�e�B�N��(��)
//==================================================================================
void SmokeParticleYel(int nCntParticle)
{
	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 2.0f;		//�ړ���
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//�p�x

		//�ړ��ʂ̐ݒ�
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//�΂̃p�[�e�B�N��
//==================================================================================
void FireParticle(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{

		fMove = (float)(rand() % 20) / 10 + 1.0f;		//�ړ���
		fMoveY = (float)(rand() % 60) / 10 + 10.0f;	//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].fRadius = 55.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(0.95f, 0.15f, 0.0f, 1.0f);

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//�΂̃p�[�e�B�N��(��)
//==================================================================================
void FireParticleYellow(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{
		fMove = (float)(rand() % 20) / 10 + 1.0f;		//�ړ���
		fMoveY = (float)(rand() % 60) / 10 + 10.0f;	//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].pos.x += g_aParticle[nCntParticle].move.x * 0.3f;
		g_aParticle[nCntParticle].pos.y += g_aParticle[nCntParticle].move.y * 0.3f;
		g_aParticle[nCntParticle].pos.z += g_aParticle[nCntParticle].move.z * 0.3f;

		g_aParticle[nCntParticle].fRadius = 30.0f;
		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f);

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
//�ł������̃p�[�e�B�N��
//==================================================================================
void OffSettingParticle(int nCntParticle)
{

	float fAngle, fMove;

	for (int nCntUse = 0; nCntUse < 10; nCntUse++)
	{

		fMove = (float)(rand() % 10) / 1 + 3.0f;		//�ړ���
		fAngle = (float)(rand() % 629 - 314) / 100.0f;	//�p�x

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;

		g_aParticle[nCntParticle].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

		g_aParticle[nCntParticle].fRadius = 40.0f;

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_NORMAL);
	}
}

//==================================================================================
// ���n�̃p�[�e�B�N��
//==================================================================================
void LandParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 30; nCntUse++)
	{
		fMove = (float)(rand() % 61 - 30) / 10 + 1.0f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		//�o���n�_�̃u��
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// �ړ��̃p�[�e�B�N��
//==================================================================================
void WalkParticle(int nCntParticle)
{
	float fMove;

	for (int nCntUse = 0; nCntUse < 5; nCntUse++)
	{
		fMove = (float)(rand() % 10) / 10 + 1.0f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = 0.0f;

		//�o���n�_�̃u��
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// �������J�o�[�����̃p�[�e�B�N��
//==================================================================================
void PillowThrowParticle(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 5; nCntUse++)
	{
		fMove = (float)(rand() % 5) / 10 + 1.0f;		//�ړ���
		fMoveY = (float)(rand() % 10) / 10 + 8.0f;	//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;

		//�o���n�_�̃u��
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_SUB, EFFECTTYPE_SMOKEBLACK);
	}
}

//==================================================================================
// �n�[�g�̃p�[�e�B�N��
//==================================================================================
void HartParticle(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 5; nCntUse++)
	{
		fMove = (float)(rand() % 5) / 10 + 1.0f;		//�ړ���
		fMoveY = (float)(rand() % 100) / 100 + 8.0f;	//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 314) / 100.0f) * fMoveY;

		//�o���n�_�̃u��
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_HART);
	}
}

//==================================================================================
// �^�C�g�����S�����̃p�[�e�B�N��
//==================================================================================
void TitleLogParticle(int nCntParticle)
{
	float fMove;
	fMove = (float)(rand() % 20) / 10 + 4.0f;		//�ړ���

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


			float fMooooooooove = (float)(rand() % 20) / 10 + 1.0f;		//�o���o���ړ���

			if (nCntUse >= 20)
			{
				fMooooooooove = (float)(rand() % 20) / 10 + 4.0f;		//�ړ���
			}

			//�ړ��ʂ̐ݒ�
			g_aParticle[nCntParticle].move.x = sinf(fRot) * (fMove - fMooooooooove);
			g_aParticle[nCntParticle].move.z = 0.0f;
			g_aParticle[nCntParticle].move.y = cosf(fRot) * (fMove - fMooooooooove);

			//�o���n�_�̃u��
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

			//�G�t�F�N�g�̐ݒ�
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
// ��ʔ����̃p�[�e�B�N��
//==================================================================================
void ManyUIParticle(int nMaxUse, int nSmallUse, int nCntParticle)
{
	float fMove;
	fMove = (float)(rand() % 30) / 10 + 5.0f;		//�ړ���

	float fff = (D3DX_PI * 2.0f) / nMaxUse;

	for (int nCntCircle = 0; nCntCircle < nMaxUse; nCntCircle++)
	{
		float fRot = ((D3DX_PI * 2.0f) / (float)((nMaxUse / 2) % (nCntCircle + 1))) * nCntCircle;
		fRot = fff * nCntCircle;
		//fRot -= D3DX_PI;

		for (int nCntUse = 0; nCntUse < nSmallUse; nCntUse++)
		{
			float fMooooooooove = (float)(rand() % 31 - 15) / 10;		//�o���o���ړ���

			float fRotRand = (float)(rand() % 21 - 10) / 100.0f;		//�o���o������

			//�ړ��ʂ̐ݒ�
			g_aParticle[nCntParticle].move.x = sinf(fRot + fRotRand) * (fMove + fMooooooooove);
			g_aParticle[nCntParticle].move.z = 0.0f;
			g_aParticle[nCntParticle].move.y = cosf(fRot + fRotRand) * (fMove + fMooooooooove);

			//�o���n�_�̃u��
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

			//�G�t�F�N�g�̐ݒ�
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
// ��ʔ����̃p�[�e�B�N��(�~�j)
//==================================================================================
void ManyUI_MiniParticle(int nCntParticle)
{
	float fMove;
	fMove = (float)(rand() % 30) / 10 + 2.0f;		//�ړ���

	for (int nCntCircle = 0; nCntCircle < 60; nCntCircle++)
	{
		float fRot = ((D3DX_PI * 2.0f) / (float)(30 % (nCntCircle + 1))) * nCntCircle;
		fRot -= D3DX_PI;

		for (int nCntUse = 0; nCntUse < 10; nCntUse++)
		{
			float fMooooooooove = (float)(rand() % 31 - 15) / 10;		//�o���o���ړ���

			float fRotRand = (float)(rand() % 21 - 10) / 100.0f;		//�o���o������

			//�ړ��ʂ̐ݒ�
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

			//�G�t�F�N�g�̐ݒ�
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
// MORS�o���̃p�[�e�B�N��
//==================================================================================
void SpawnMORSParticle(int nCntParticle)
{
	float fMoveX, fMoveY, fMoveZ;

	for (int nCntUse = 0; nCntUse < 1; nCntUse++)
	{
		fMoveX = (float)(rand() % 300) / 100 + 1.0f;		//�ړ���
		fMoveY = (float)(rand() % 300) / 100 + 1.0f;		//�ړ���
		fMoveZ = (float)(rand() % 300) / 100 + 1.0f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMoveX;
		g_aParticle[nCntParticle].move.z = cosf((float)(rand() % 629 - 314) / 100.0f) * fMoveZ;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 629 - 314) / 100.0f) * fMoveY;

		//�o���n�_�̃u��
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_THUNDER + nnnn);
	}
}

//==================================================================================
// ���ʂ̃��b�Z�[�W�̃p�[�e�B�N��
//==================================================================================
void ResultMessageParticle(int nCntParticle)
{
	float fMove;
	fMove = (float)(rand() % 30) / 10 + 5.0f;		//�ړ���

	float fff = (D3DX_PI * 2.0f) / 40;

	for (int nCntCircle = 0; nCntCircle < 40; nCntCircle++)
	{
		float fRot = fff * nCntCircle;

		for (int nCntUse = 0; nCntUse < 10; nCntUse++)
		{
			float fMooooooooove = (float)(rand() % 31 - 15) / 10;		//�o���o���ړ���

			float fRotRand = (float)(rand() % 21 - 10) / 100.0f;		//�o���o������

																		//�ړ��ʂ̐ݒ�
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

			//�G�t�F�N�g�̐ݒ�
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
		fMove = (float)(rand() % 50) * 0.1f;		//�ړ���

		//�ړ��ʂ̐ݒ�
		g_aParticle[nCntParticle].move.x = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.y = sinf((float)(rand() % 629 - 314) / 100.0f) * fMove;
		g_aParticle[nCntParticle].move.z = 0.0f;

		//�o���n�_�̃u��
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect_2D(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_JUJI);
	}

}

//==================================================================================
// FEVER�̉����̃p�[�e�B�N��
//==================================================================================
void FeverPushParticle(int nCntParticle)
{
	float fMove, fMoveY;

	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{

		fMove = (float)(rand() % 20) / 10 + 1.0f;		//�ړ���
		fMoveY = (float)(rand() % 60) / 10;	//�ړ���

											//�ړ��ʂ̐ݒ�
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect_2D(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKE);
	}


	for (int nCntUse = 0; nCntUse < 2; nCntUse++)
	{

		fMove = (float)(rand() % 20) / 10 + 1.0f;		//�ړ���
		fMoveY = (float)(rand() % 50) / 10;	//�ړ���

		//�ړ��ʂ̐ݒ�
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

		//�G�t�F�N�g�̐ݒ�
		SetEffect_2D(g_aParticle[nCntParticle].pos,
			g_aParticle[nCntParticle].col,
			g_aParticle[nCntParticle].fRadius,
			g_aParticle[nCntParticle].nLife,
			g_aParticle[nCntParticle].move,
			MOVEEFFECT_ADD, EFFECTTYPE_SMOKE);
	}

}

//==================================================================================
//�p�[�e�B�N�����擾
//==================================================================================
int GetParticleNum(void)
{
	return g_nParticleNum;
}
