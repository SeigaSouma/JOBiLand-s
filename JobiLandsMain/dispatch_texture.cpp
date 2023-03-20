//=============================================================================
//
// �h���e�N�X�`������ [dispatch_texture.cpp]
// Author : ����������
//
//=============================================================================
#include "dispatch_texture.h"
#include "buddy.h"

//�}�N����`
#define NUM_TEX			(2)				//�e�N�X�`���̐�
#define MAX_WIDTH		(80.0f)		//��
#define MAX_HEIGHT		(80.0f)		//�c
#define POS_X			(310.0f)		//X�̈ʒu
#define POS_Y			(620.0f)		//Y�̈ʒu

//�O���[�o���ϐ�
const char *c_apFilenameDispatchTex[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\dispatch_gari.png",
	"data\\TEXTURE\\dispatch_tibi.png",
};
LPDIRECT3DTEXTURE9 g_apTextureDispatchTex[(sizeof c_apFilenameDispatchTex) / sizeof(*c_apFilenameDispatchTex)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDispatchTex = NULL;
DispatchTex g_aDispatchTex[NUM_TEX];

//==============================================================
//�h���e�N�X�`���̏���������
//==============================================================
void InitDispatchTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameDispatchTex) / sizeof(*c_apFilenameDispatchTex); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameDispatchTex[nCntTex],
			&g_apTextureDispatchTex[nCntTex]);
	}

	//�ϐ�������
	for (int nCntDisTex = 0; nCntDisTex < NUM_TEX; nCntDisTex++)
	{
		if (nCntDisTex == 0)
		{
			g_aDispatchTex[nCntDisTex].pos = D3DXVECTOR3(640.0f - POS_X, POS_Y, 0.0f);		//�ʒu

		}
		else if (nCntDisTex == 1)
		{
			g_aDispatchTex[nCntDisTex].pos = D3DXVECTOR3(640.0f + POS_X, POS_Y, 0.0f);		//�ʒu
		}

		g_aDispatchTex[nCntDisTex].nCntDispatch = 0;
		g_aDispatchTex[nCntDisTex].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDispatchTex,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffDispatchTex->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisTex = 0; nCntDisTex < NUM_TEX; nCntDisTex++)
	{

		pVtx[0].pos = D3DXVECTOR3(g_aDispatchTex[nCntDisTex].pos.x - MAX_WIDTH, g_aDispatchTex[nCntDisTex].pos.y - MAX_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aDispatchTex[nCntDisTex].pos.x + MAX_WIDTH, g_aDispatchTex[nCntDisTex].pos.y - MAX_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aDispatchTex[nCntDisTex].pos.x - MAX_WIDTH, g_aDispatchTex[nCntDisTex].pos.y + MAX_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aDispatchTex[nCntDisTex].pos.x + MAX_WIDTH, g_aDispatchTex[nCntDisTex].pos.y + MAX_HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDispatchTex->Unlock();
}

//==============================================================
//�h���e�N�X�`���̉�ʂ̏I������
//==============================================================
void UninitDispatchTex(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameDispatchTex) / sizeof(*c_apFilenameDispatchTex); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureDispatchTex[nCntTex] != NULL)
		{
			g_apTextureDispatchTex[nCntTex]->Release();
			g_apTextureDispatchTex[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffDispatchTex != NULL)
	{
		g_pVtxBuffDispatchTex->Release();
		g_pVtxBuffDispatchTex = NULL;
	}
}

//==============================================================
//�h���e�N�X�`���̉�ʂ̍X�V����
//==============================================================
void UpdateDispatchTex(void)
{
	VERTEX_2D *pVtx;

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffDispatchTex->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDisTex = 0; nCntDisTex < NUM_TEX; nCntDisTex++, pBuddy++)
	{
		if (pBuddy->nState == BUDDYSTATE_DISPATCH)
		{
			g_aDispatchTex[nCntDisTex].nCntDispatch = 1;		//�h��
		}
		else
		{
			g_aDispatchTex[nCntDisTex].nCntDispatch = 0;		//�h��
		}

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f + g_aDispatchTex[nCntDisTex].nCntDispatch * 0.5f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.5f + g_aDispatchTex[nCntDisTex].nCntDispatch * 0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f + g_aDispatchTex[nCntDisTex].nCntDispatch * 0.5f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f + g_aDispatchTex[nCntDisTex].nCntDispatch * 0.5f, 1.0f);
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDispatchTex->Unlock();
}

//==============================================================
//�h���e�N�X�`���̉�ʂ̕`�揈��
//==============================================================
void DrawDispatchTex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	////Z�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffDispatchTex, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntDispatchTex = 0; nCntDispatchTex < NUM_TEX; nCntDispatchTex++)
	{
		//�e�N�X�`���Z�b�g
		pDevice->SetTexture(0, g_apTextureDispatchTex[nCntDispatchTex]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntDispatchTex * 4, 2);	//�v���~�e�B�u�̎��
	}

	////Z�e�X�g��L���ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//==============================================================
//�h���e�N�X�`���̏��
//==============================================================
DispatchTex *GetDispatchTex(void)
{
	return &g_aDispatchTex[0];
}