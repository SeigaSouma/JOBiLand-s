//=============================================================================
//
// �G�t�F�N�g���� [2D_effect.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "2D_effect.h"
#include "effect.h"
#include "input.h"
#include "particle.h"
#include "camera.h"

//�}�N����`
#define MAX_TOP			(4)		//���_��
#define MAX_EFFECT_2D		(12800)	//�G�t�F�N�g�̍ő吔
#define EFFECT_2DSIZE1		(0.97f)
#define EFFECT_2DSIZE2		(0.98f)
#define EFFECT_2DSIZE3		(0.99f)


//�v���g�^�C�v�錾
void UpdateSubEffect_2D(int nCntEffect);
void UpdateSuperSubEffect_2D(int nCntEffect);
void UpdateAddEffect_2D(int nCntEffect);

//�O���[�o���ϐ��錾
const char *c_apFilenameEffect_2D[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\effect002.jpg",
	"data\\TEXTURE\\smoke_05.tga",
	"data\\TEXTURE\\effect000.png",
	"data\\TEXTURE\\hart_01.png",
	"data\\TEXTURE\\effect001.png",
};

LPDIRECT3DTEXTURE9 g_pTextureEffect_2D[(sizeof c_apFilenameEffect_2D) / sizeof(*c_apFilenameEffect_2D)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect_2D = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Effect_2D g_aEffect_2D[MAX_EFFECT_2D];						//�G�t�F�N�g�̏��
int g_Effect_2DNum;									//�G�t�F�N�g�̐�
bool g_bEffect_2DUse;

//==================================================================================
//�G�t�F�N�g�̏���������
//==================================================================================
void InitEffect_2D(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameEffect_2D) / sizeof(*c_apFilenameEffect_2D); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEffect_2D[nCntTex],
			&g_pTextureEffect_2D[nCntTex]);

	}


	//�G�t�F�N�g�̏��̏�����
	for (int nCntEffect =0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		g_aEffect_2D[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
		g_aEffect_2D[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
		g_aEffect_2D[nCntEffect].mtxWorld = {};	//���[���h�}�g���b�N�X
		g_aEffect_2D[nCntEffect].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F
		g_aEffect_2D[nCntEffect].fRadius = 0.0f;		//���a
		g_aEffect_2D[nCntEffect].fMaxRadius = 0.0f;	//�ő唼�a
		g_aEffect_2D[nCntEffect].nLife = EFFECT_2D_LIFE;			//����
		g_aEffect_2D[nCntEffect].nMaxLife = EFFECT_2D_LIFE;		//�ő����(�Œ�)
		g_aEffect_2D[nCntEffect].moveType = 0;		//�ړ��̎��
		g_aEffect_2D[nCntEffect].nType = 0;			//���
		g_aEffect_2D[nCntEffect].bUse = false;			//�g�p���Ă��邩�ǂ���
		g_aEffect_2D[nCntEffect].bAddAlpha = false;		//���Z�����̔���
	}
	g_Effect_2DNum = 0;
	g_bEffect_2DUse = true;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * MAX_EFFECT_2D,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffEffect_2D,
								NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect_2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aEffect_2D[nCntEffect].col;
		pVtx[1].col = g_aEffect_2D[nCntEffect].col;
		pVtx[2].col = g_aEffect_2D[nCntEffect].col;
		pVtx[3].col = g_aEffect_2D[nCntEffect].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect_2D->Unlock();
}

//==================================================================================
//�G�t�F�N�g�̏I������
//==================================================================================
void UninitEffect_2D(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameEffect_2D) / sizeof(*c_apFilenameEffect_2D); nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureEffect_2D[nCount] != NULL)
		{
			g_pTextureEffect_2D[nCount]->Release();
			g_pTextureEffect_2D[nCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect_2D != NULL)
	{
		g_pVtxBuffEffect_2D->Release();
		g_pVtxBuffEffect_2D = NULL;
	}
}

//==================================================================================
//�G�t�F�N�g�̍X�V����
//==================================================================================
void UpdateEffect_2D(void)
{

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect_2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		if (g_aEffect_2D[nCntEffect].bUse == true)
		{//�G�t�F�N�g���g�p����Ă�����

			//���_���W�̍X�V(����������)
			if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK)
			{//�����������ꍇ

				if (((float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife) <= 0.5f)
				{//�ő�̗͂̔����ȉ�

					g_aEffect_2D[nCntEffect].pos += g_aEffect_2D[nCntEffect].move * (((float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife));
				}
				else
				{//��������

					g_aEffect_2D[nCntEffect].pos += g_aEffect_2D[nCntEffect].move * (((float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife));
				}
			}
			else 
			{//�����ȊO

				//�ʒu�X�V
				g_aEffect_2D[nCntEffect].pos += g_aEffect_2D[nCntEffect].move;
			}
			

			//���������Ă��������̏ꍇ
			if (g_aEffect_2D[nCntEffect].moveType == MOVEEFFECT_SUB)
			{//�G�t�F�N�g�����������Ă���

				if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_NORMAL || g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKE ||
					g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK || g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_BLACK)
				{
					//�k������
					UpdateSubEffect_2D(nCntEffect);
				}
			}
			else if (g_aEffect_2D[nCntEffect].moveType == MOVEEFFECT_SUPERSUB)
			{//�G�t�F�N�g�����������Ă���

				UpdateSuperSubEffect_2D(nCntEffect);
			}
			else if (g_aEffect_2D[nCntEffect].moveType == MOVEEFFECT_ADD)
			{//�G�t�F�N�g��傫�����Ă���

				//�g�又��
				UpdateAddEffect_2D(nCntEffect);
			}
			else if (g_aEffect_2D[nCntEffect].moveType == MOVEEFFECT_NONE)
			{//�ω��Ȃ�

			}


			//�����̍X�V
			g_aEffect_2D[nCntEffect].nLife--;

			//�s�����x�̍X�V
			g_aEffect_2D[nCntEffect].col.a = (float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife;

			if (g_aEffect_2D[nCntEffect].nLife <= 0)
			{//�������s������

				g_aEffect_2D[nCntEffect].bUse = false;
				g_aEffect_2D[nCntEffect].bAddAlpha = false;
				g_Effect_2DNum--;
			}

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect_2D[nCntEffect].col;
			pVtx[1].col = g_aEffect_2D[nCntEffect].col;
			pVtx[2].col = g_aEffect_2D[nCntEffect].col;
			pVtx[3].col = g_aEffect_2D[nCntEffect].col;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);

		}

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect_2D->Unlock();
}

//==================================================================================
//�G�t�F�N�g�̏k������
//==================================================================================
void UpdateSubEffect_2D(int nCntEffect)
{

	int nEffect_2DType = rand() % 3;

	if (nEffect_2DType == 0)
	{
		g_aEffect_2D[nCntEffect].fRadius *= EFFECT_2DSIZE1;
	}
	else if (nEffect_2DType == 1)
	{
		g_aEffect_2D[nCntEffect].fRadius *= EFFECT_2DSIZE2;
	}
	else if (nEffect_2DType == 2)
	{
		g_aEffect_2D[nCntEffect].fRadius *= EFFECT_2DSIZE3;
	}

	//g_aEffect_2D[nCntEffect].fRadius = g_aEffect_2D[nCntEffect].fMaxRadius * (float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife;

}

//==================================================================================
//�G�t�F�N�g�̏k������
//==================================================================================
void UpdateSuperSubEffect_2D(int nCntEffect)
{

	g_aEffect_2D[nCntEffect].fRadius = g_aEffect_2D[nCntEffect].fMaxRadius * (float)g_aEffect_2D[nCntEffect].nLife / (float)g_aEffect_2D[nCntEffect].nMaxLife;
}

//==================================================================================
//�G�t�F�N�g�̊g�又��
//==================================================================================
void UpdateAddEffect_2D(int nCntEffect)
{

	int nEffect_2DType = rand() % 3;

	if (nEffect_2DType == 0)
	{
		g_aEffect_2D[nCntEffect].fRadius += 1.8f;
	}
	else if (nEffect_2DType == 1)
	{
		g_aEffect_2D[nCntEffect].fRadius += 0.8f;
	}
	else if (nEffect_2DType == 2)
	{
		g_aEffect_2D[nCntEffect].fRadius += 0.0f;
	}

}

//==================================================================================
//�G�t�F�N�g�̕`�揈��
//==================================================================================
void DrawEffect_2D(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�|���S���̕`��
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		if (g_aEffect_2D[nCntEffect].bUse == true && g_bEffect_2DUse == true)
		{//�G�t�F�N�g���g�p����Ă���

			//���u�����f�B���O�����Z�����ɐݒ�
			if (g_aEffect_2D[nCntEffect].bAddAlpha == false)
			{
				pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
				pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			}

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffEffect_2D, 0, sizeof(VERTEX_2D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_2D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEffect_2D[g_aEffect_2D[nCntEffect].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * MAX_TOP, 2);

			//���u�����f�B���O�����ɖ߂�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		}
	}
}

//==================================================================================
//�G�t�F�N�g�̐ݒ菈��
//==================================================================================
void SetEffect_2D(D3DXVECTOR3 pos, D3DXCOLOR col, float fRadius, int nLife, D3DXVECTOR3 move, int moveType, int nType)
{

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect_2D->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT_2D; nCntEffect++)
	{
		if (g_aEffect_2D[nCntEffect].bUse == false)
		{//�G�t�F�N�g���g�p����Ă��Ȃ�������

			g_aEffect_2D[nCntEffect].pos = pos;
			g_aEffect_2D[nCntEffect].move = move;
			g_aEffect_2D[nCntEffect].col = col;
			g_aEffect_2D[nCntEffect].fRadius = fRadius;
			g_aEffect_2D[nCntEffect].fMaxRadius = fRadius;
			g_aEffect_2D[nCntEffect].nLife = nLife;
			g_aEffect_2D[nCntEffect].nMaxLife = nLife;
			g_aEffect_2D[nCntEffect].moveType = moveType;
			g_aEffect_2D[nCntEffect].nType = nType;
			g_aEffect_2D[nCntEffect].bUse = true;		//�g�p���Ă����ԂɕύX
			g_Effect_2DNum++;

			if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_NORMAL)
			{//�ʏ�̎�

				g_aEffect_2D[nCntEffect].bAddAlpha = false;
			}
			else if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKE)
			{//���̃G�t�F�N�g

				g_aEffect_2D[nCntEffect].bAddAlpha = false;
			}
			else if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_SMOKEBLACK)
			{//�����̃G�t�F�N�g

				g_aEffect_2D[nCntEffect].bAddAlpha = true;
			}
			else if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_BLACK)
			{//���̃G�t�F�N�g

				g_aEffect_2D[nCntEffect].bAddAlpha = true;
			}
			else if (g_aEffect_2D[nCntEffect].nType == EFFECTTYPE_JUJI)
			{//�\���̃G�t�F�N�g

				g_aEffect_2D[nCntEffect].bAddAlpha = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + -g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + -g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect_2D[nCntEffect].pos.x + +g_aEffect_2D[nCntEffect].fRadius, g_aEffect_2D[nCntEffect].pos.y + +g_aEffect_2D[nCntEffect].fRadius, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aEffect_2D[nCntEffect].col;
			pVtx[1].col = g_aEffect_2D[nCntEffect].col;
			pVtx[2].col = g_aEffect_2D[nCntEffect].col;
			pVtx[3].col = g_aEffect_2D[nCntEffect].col;

			break;
		}

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect_2D->Unlock();
}

//==================================================================================
//�G�t�F�N�g�̐��擾
//==================================================================================
int GetEffect_2DNum(void)
{
	return g_Effect_2DNum;
}

//==================================================================================
//�G�t�F�N�g���g�p���邩
//==================================================================================
bool GetEffect_2DUse(void)
{
	return g_bEffect_2DUse;
}