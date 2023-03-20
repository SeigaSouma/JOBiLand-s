//=============================================================================
//
// �t�B�[�o�[�v�b�V������ [feverpush.cpp]
// Author : ����������
//
//=============================================================================
#include "feverpush.h"
#include "fevergauge.h"
#include "particle.h"

//�}�N����`
#define MAX_WIDTH		(320.0f)		//��
#define MAX_HEIGHT		(120.0f)		//�c
#define MIN_WIDTH		(320.0f * 0.8f)		//��
#define MIN_HEIGHT		(120.0f * 0.8f)		//�c

//�O���[�o���ϐ�
const char *c_apFilenameFeverPush[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\feverpush.png",
};
LPDIRECT3DTEXTURE9 g_apTextureFeverPush[(sizeof c_apFilenameFeverPush) / sizeof(*c_apFilenameFeverPush)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFeverPush = NULL;
FeverPush g_aFeverPush;

//==============================================================
//�t�B�[�o�[�v�b�V���̏���������
//==============================================================
void InitFeverPush(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameFeverPush) / sizeof(*c_apFilenameFeverPush); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameFeverPush[nCntTex],
			&g_apTextureFeverPush[nCntTex]);
	}

	//�ϐ�������
	g_aFeverPush.pos = D3DXVECTOR3(640.0f, 600.0f, 0.0f);	//�ʒu
	g_aFeverPush.fWidth = MIN_WIDTH;			//����
	g_aFeverPush.fHeight = MIN_HEIGHT;			//�c��
	g_aFeverPush.fWidthMove = (MAX_WIDTH * 0.035f);		//����
	g_aFeverPush.fHeightMove = (MAX_HEIGHT * 0.035f);		//�c��
	g_aFeverPush.bUse = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFeverPush,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffFeverPush->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos = D3DXVECTOR3(g_aFeverPush.pos.x - g_aFeverPush.fWidth, g_aFeverPush.pos.y - g_aFeverPush.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aFeverPush.pos.x + g_aFeverPush.fWidth, g_aFeverPush.pos.y - g_aFeverPush.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aFeverPush.pos.x - g_aFeverPush.fWidth, g_aFeverPush.pos.y + g_aFeverPush.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aFeverPush.pos.x + g_aFeverPush.fWidth, g_aFeverPush.pos.y + g_aFeverPush.fHeight, 0.0f);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFeverPush->Unlock();
}

//==============================================================
//�t�B�[�o�[�v�b�V���̉�ʂ̏I������
//==============================================================
void UninitFeverPush(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameFeverPush) / sizeof(*c_apFilenameFeverPush); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureFeverPush[nCntTex] != NULL)
		{
			g_apTextureFeverPush[nCntTex]->Release();
			g_apTextureFeverPush[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFeverPush != NULL)
	{
		g_pVtxBuffFeverPush->Release();
		g_pVtxBuffFeverPush = NULL;
	}
}

//==============================================================
//�t�B�[�o�[�v�b�V���̉�ʂ̍X�V����
//==============================================================
void UpdateFeverPush(void)
{

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffFeverPush->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aFeverPush.bUse == true)
	{//�g�p���Ă�Ƃ�

		g_aFeverPush.fWidth += g_aFeverPush.fWidthMove;
		g_aFeverPush.fHeight += g_aFeverPush.fHeightMove;

		if (g_aFeverPush.fWidth >= MAX_WIDTH)
		{//�ő�l�𒴂�����

			//�ő�l�ɌŒ�
			g_aFeverPush.fWidth = MAX_WIDTH;
			g_aFeverPush.fHeight = MAX_HEIGHT;

			//�ړ��t�]
			g_aFeverPush.fWidthMove = -(MAX_WIDTH * 0.005f);
			g_aFeverPush.fHeightMove = -(MAX_HEIGHT * 0.005f);
		}
		else if (g_aFeverPush.fWidth <= MIN_WIDTH)
		{//�ŏ��l���������

			//�ő�l�ɌŒ�
			g_aFeverPush.fWidth = MIN_WIDTH;
			g_aFeverPush.fHeight = MIN_HEIGHT;

			//�ړ��t�]
			g_aFeverPush.fWidthMove = (MAX_WIDTH * 0.025f);		//����
			g_aFeverPush.fHeightMove = (MAX_HEIGHT * 0.025f);		//�c��
		}

		//�����̃p�[�e�B�N��
		SetParticle(D3DXVECTOR3(g_aFeverPush.pos.x - 130.0f, g_aFeverPush.pos.y + 30.0f, 0.0f), PARTICLE_TYPE_FEVERPUSH);
		SetParticle(D3DXVECTOR3(g_aFeverPush.pos.x + 130.0f, g_aFeverPush.pos.y + 30.0f, 0.0f), PARTICLE_TYPE_FEVERPUSH);
	}


	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aFeverPush.pos.x - g_aFeverPush.fWidth, g_aFeverPush.pos.y - g_aFeverPush.fHeight, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aFeverPush.pos.x + g_aFeverPush.fWidth, g_aFeverPush.pos.y - g_aFeverPush.fHeight, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aFeverPush.pos.x - g_aFeverPush.fWidth, g_aFeverPush.pos.y + g_aFeverPush.fHeight, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aFeverPush.pos.x + g_aFeverPush.fWidth, g_aFeverPush.pos.y + g_aFeverPush.fHeight, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFeverPush->Unlock();
}

//==============================================================
//�t�B�[�o�[�v�b�V���̉�ʂ̕`�揈��
//==============================================================
void DrawFeverPush(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////Z�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFeverPush, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntFeverPush = 0; nCntFeverPush < 1; nCntFeverPush++)
	{
		if (g_aFeverPush.bUse == true)
		{
			//�e�N�X�`���Z�b�g
			pDevice->SetTexture(0, g_apTextureFeverPush[nCntFeverPush]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFeverPush * 4, 2);	//�v���~�e�B�u�̎��
		}
	}

	////Z�e�X�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================
//�t�B�[�o�[�v�b�V���̏��
//==============================================================
FeverPush *GetFeverPush(void)
{
	return &g_aFeverPush;
}