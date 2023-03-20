//=============================================================================
//
// ���S�C�̒e���� [watergun.cpp]
// Author : �匴�叫
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

//�}�N����`
#define MAX_TOP			(4)		//���_��
#define MAX_EFFECT		(1280)	//�G�t�F�N�g�̍ő吔
#define EFFECTSIZE1		(0.97f)
#define EFFECTSIZE2		(0.98f)
#define EFFECTSIZE3		(0.99f)


//�v���g�^�C�v�錾
void UpdateSubWatergun(int nCntWatergun);
void UpdateSuperSubWatergun(int nCntWatergun);
void UpdateAddWatergun(int nCntWatergun);
void CollisionWaterBullet(int nCntWatergun, D3DXVECTOR3 pos, float fWidth, float fHeight);

//�O���[�o���ϐ��錾
const char *c_apFilenameWatergun[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect002.jpg",
	"data\\TEXTURE\\smoke_05.tga",
	"data\\TEXTURE\\effect000.png",
	"data\\TEXTURE\\hart_01.png",
};

LPDIRECT3DTEXTURE9 g_pTextureWatergun[(sizeof c_apFilenameWatergun) / sizeof(*c_apFilenameWatergun)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffWatergun = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Watergun g_aWatergun[MAX_EFFECT];						//�G�t�F�N�g�̏��
int g_WatergunNum;									//�G�t�F�N�g�̐�
bool g_bWatergunUse;

//==================================================================================
//�G�t�F�N�g�̏���������
//==================================================================================
void InitWatergun(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameWatergun) / sizeof(*c_apFilenameWatergun); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameWatergun[nCntTex],
			&g_pTextureWatergun[nCntTex]);

	}

	//�G�t�F�N�g�̏��̏�����
	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		g_aWatergun[nCntWatergun].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWatergun[nCntWatergun].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aWatergun[nCntWatergun].mtxWorld = {};
		g_aWatergun[nCntWatergun].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		g_aWatergun[nCntWatergun].fRadius = 0.0f;
		g_aWatergun[nCntWatergun].fMaxRadius = 0.0f;		//�ő唼�a
		g_aWatergun[nCntWatergun].nLife = WATERBULLET_LIFE;
		g_aWatergun[nCntWatergun].nMaxLife = g_aWatergun[nCntWatergun].nLife;	//�ő����
		g_aWatergun[nCntWatergun].moveType = 0;				//�ړ��̎��
		g_aWatergun[nCntWatergun].nType = 0;
		g_aWatergun[nCntWatergun].bUse = false;				//�g�p���Ă��Ȃ���Ԃɂ���
		g_aWatergun[nCntWatergun].bAddAlpha = false;
	}
	g_WatergunNum = 0;
	g_bWatergunUse = true;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_EFFECT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffWatergun,
		NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWatergun->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aWatergun[nCntWatergun].col;
		pVtx[1].col = g_aWatergun[nCntWatergun].col;
		pVtx[2].col = g_aWatergun[nCntWatergun].col;
		pVtx[3].col = g_aWatergun[nCntWatergun].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWatergun->Unlock();
}

//==================================================================================
//�G�t�F�N�g�̏I������
//==================================================================================
void UninitWatergun(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameWatergun) / sizeof(*c_apFilenameWatergun); nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureWatergun[nCount] != NULL)
		{
			g_pTextureWatergun[nCount]->Release();
			g_pTextureWatergun[nCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffWatergun != NULL)
	{
		g_pVtxBuffWatergun->Release();
		g_pVtxBuffWatergun = NULL;
	}
}

//==================================================================================
//�G�t�F�N�g�̍X�V����
//==================================================================================
void UpdateWatergun(void)
{
	Enemy *pEnemy = GetEnemy();
	Crowd *pCrowd = GetCrowd();

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWatergun->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_F5) == true)
	{//F5�������ꂽ,�G�t�F�N�g�g�p�؂�ւ�

		g_bWatergunUse = g_bWatergunUse ? false : true;
	}

	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		if (g_aWatergun[nCntWatergun].bUse == true)
		{//�G�t�F�N�g���g�p����Ă�����

		 //���_���W�̍X�V(����������)
			if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKEBLACK)
			{//�����������ꍇ

				if (((float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife) <= 0.5f)
				{//�ő�̗͂̔����ȉ�

					g_aWatergun[nCntWatergun].pos += g_aWatergun[nCntWatergun].move * (((float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife));
				}
				else
				{//��������

					g_aWatergun[nCntWatergun].pos += g_aWatergun[nCntWatergun].move * (((float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife));
				}
			}
			else
			{//�����ȊO

			 //�ʒu�X�V
				g_aWatergun[nCntWatergun].pos += g_aWatergun[nCntWatergun].move;
			}

			//�Q�O�ƓG�̒e�̓����蔻��
			CollisionWaterBullet(nCntWatergun, pCrowd->aModel.pos, 50.0f, 50.0f);

			//���������Ă��������̏ꍇ
			if (g_aWatergun[nCntWatergun].moveType == MOVEEFFECT_SUB)
			{//�G�t�F�N�g�����������Ă���

				if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_NORMAL || g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKE ||
					g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKEBLACK || g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_BLACK)
				{
					//�k������
					UpdateSubWatergun(nCntWatergun);
				}
			}
			else if (g_aWatergun[nCntWatergun].moveType == MOVEEFFECT_SUPERSUB)
			{//�G�t�F�N�g�����������Ă���

				UpdateSuperSubWatergun(nCntWatergun);
			}
			else
			{//�G�t�F�N�g��傫�����Ă���

			 //�g�又��
				UpdateAddWatergun(nCntWatergun);
			}


			//�����̍X�V
			g_aWatergun[nCntWatergun].nLife--;

			//�s�����x�̍X�V
			g_aWatergun[nCntWatergun].col.a = (float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife;

			if (g_aWatergun[nCntWatergun].nLife <= 0)
			{//�������s������

				g_aWatergun[nCntWatergun].bUse = false;
				g_aWatergun[nCntWatergun].bAddAlpha = false;
				g_WatergunNum--;
			}
			
			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aWatergun[nCntWatergun].col;
			pVtx[1].col = g_aWatergun[nCntWatergun].col;
			pVtx[2].col = g_aWatergun[nCntWatergun].col;
			pVtx[3].col = g_aWatergun[nCntWatergun].col;

			//���_���W�̍X�V(����������)
			pVtx[0].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);

		}

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWatergun->Unlock();
}

//==================================================================================
//�G�t�F�N�g�̏k������
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
//�G�t�F�N�g�̏k������
//==================================================================================
void UpdateSuperSubWatergun(int nCntWatergun)
{

	g_aWatergun[nCntWatergun].fRadius = g_aWatergun[nCntWatergun].fMaxRadius * (float)g_aWatergun[nCntWatergun].nLife / (float)g_aWatergun[nCntWatergun].nMaxLife;
}

//==================================================================================
//�G�t�F�N�g�̊g�又��
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
//�G�t�F�N�g�̕`�揈��
//==================================================================================
void DrawWatergun(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

							// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//�|���S���̕`��
	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		if (g_aWatergun[nCntWatergun].bUse == true && g_bWatergunUse == true)
		{//�G�t�F�N�g���g�p����Ă���

		 //���u�����f�B���O�����Z�����ɐݒ�
			if (g_aWatergun[nCntWatergun].bAddAlpha == false)
			{
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aWatergun[nCntWatergun].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aWatergun[nCntWatergun].mtxWorld, NULL, &mtxView);	//�t�z������߂�
			g_aWatergun[nCntWatergun].mtxWorld._41 = 0.0f;
			g_aWatergun[nCntWatergun].mtxWorld._42 = 0.0f;
			g_aWatergun[nCntWatergun].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aWatergun[nCntWatergun].pos.x, g_aWatergun[nCntWatergun].pos.y, g_aWatergun[nCntWatergun].pos.z);
			D3DXMatrixMultiply(&g_aWatergun[nCntWatergun].mtxWorld, &g_aWatergun[nCntWatergun].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aWatergun[nCntWatergun].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffWatergun, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureWatergun[g_aWatergun[nCntWatergun].nType]);

			//�r���{�[�h�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWatergun * MAX_TOP, 2);

			//���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		}

	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==================================================================================
//�G�t�F�N�g�̐ݒ菈��
//==================================================================================
void SetWatergun(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffWatergun->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWatergun = 0; nCntWatergun < MAX_EFFECT; nCntWatergun++)
	{
		if (g_aWatergun[nCntWatergun].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�������

			g_aWatergun[nCntWatergun].pos = pos;
			g_aWatergun[nCntWatergun].move = move;
			g_aWatergun[nCntWatergun].col = col;
			g_aWatergun[nCntWatergun].fRadius = fRadius;
			g_aWatergun[nCntWatergun].fMaxRadius = fRadius;
			g_aWatergun[nCntWatergun].nLife = nLife;
			g_aWatergun[nCntWatergun].nMaxLife = nLife;
			g_aWatergun[nCntWatergun].moveType = moveType;
			g_aWatergun[nCntWatergun].nType = nType;
			g_aWatergun[nCntWatergun].bUse = true;		//�g�p���Ă����ԂɕύX
			g_WatergunNum++;

			//if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_NORMAL)
			//{//�ʏ�̎�

			//	g_aWatergun[nCntWatergun].bAddAlpha = false;
			//}
			//else if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKE)
			//{//���̃G�t�F�N�g

			//	g_aWatergun[nCntWatergun].bAddAlpha = false;
			//}
			//else if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_SMOKEBLACK)
			//{//�����̃G�t�F�N�g

			//	g_aWatergun[nCntWatergun].bAddAlpha = true;
			//}
			//else if (g_aWatergun[nCntWatergun].nType == WATERBULLETTYPE_BLACK)
			//{//���̃G�t�F�N�g

			//	g_aWatergun[nCntWatergun].bAddAlpha = true;
			//}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, +g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(+g_aWatergun[nCntWatergun].fRadius, -g_aWatergun[nCntWatergun].fRadius, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aWatergun[nCntWatergun].col;
			pVtx[1].col = g_aWatergun[nCntWatergun].col;
			pVtx[2].col = g_aWatergun[nCntWatergun].col;
			pVtx[3].col = g_aWatergun[nCntWatergun].col;

			break;
		}

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffWatergun->Unlock();
}

//==================================================================================
//���S�C�̒e�̓����蔻��
//==================================================================================
void CollisionWaterBullet(int nCntWatergun,  D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	//�Q�O�̏��擾
	Crowd *pCrowd = GetCrowd();

	//�G�̏��擾
	Enemy *pEnemy = GetEnemy();

	//�v���C���[�̏����擾
	Player *pPlayer = GetPlayer();

	for (int nCount = 0; nCount < MAX_CROWD; nCount++, pCrowd++)
	{
		if (pCrowd->aModel.bUse == true && pCrowd->nState != CROWDSTATE_FADE)
		{//�Q�O���g�p����Ă�����

		    //�G�ƌQ�O�̋���
			float fLength = pCrowd->aModel.vtxMax.x + g_aWatergun[nCntWatergun].fRadius;

			if (CircleRange(g_aWatergun[nCntWatergun].pos, pCrowd->aModel.pos) <= (fLength * fLength))
			{//�~�̒��ɓ�������

			    //�Q�O������
				pCrowd->nState = CROWDSTATE_FADE;
			}
		}
	}
}

//==================================================================================
//�G�t�F�N�g�̐��擾
//==================================================================================
int GetWatergunNum(void)
{
	return g_WatergunNum;
}

//==================================================================================
//�G�t�F�N�g���g�p���邩
//==================================================================================
bool GetWatergunUse(void)
{
	return g_bWatergunUse;
}