//=============================================================================
//
// ���S���� [logo.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "logo.h"
#include "fade.h"

#define MIN_ALPHA	(0.4f)		//�s�����x�ŏ��l
#define MAX_ALPHA	(1.0f)		//�s�����x�ő�l
#define CHANGE_TIME	(60 * 3)	//�؂�ւ��^�C�}�[
#define LOG_WIDTH	(640.0f)
#define LOG_HEIGHT	(LOG_WIDTH * 0.2f)
#define LOG_MINI_WIDTH	(150.0f * 0.5f)
#define LOG_MINI_HEIGHT	(200.0f * 0.5f)

//�O���[�o���ϐ��錾
const char *c_apFilenameLogo[] =					//�t�@�C���ǂݍ���
{
	NULL,
	"data\\TEXTURE\\teamLog_01.png",
};
LPDIRECT3DTEXTURE9 g_pTextureLogo[(sizeof c_apFilenameLogo) / sizeof(*c_apFilenameLogo)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLogo = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Logo g_Logo[LOGOVTX_MAX];

//==================================================================================
//���S�̏���������
//==================================================================================
void InitLogo(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameLogo) / sizeof(*c_apFilenameLogo); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameLogo[nCntTex],
			&g_pTextureLogo[nCntTex]);
	}

	//�e��ϐ�������
	for (int nCntLogo = 0; nCntLogo < LOGOVTX_MAX; nCntLogo++)
	{
		g_Logo[nCntLogo].nCntChange = 0;	//�؂�ւ�鎞��
		g_Logo[nCntLogo].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//�F
		g_Logo[nCntLogo].pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//�ʒu
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * LOGOVTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLogo,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffLogo->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCntLogo = 0; nCntLogo < LOGOVTX_MAX; nCntLogo++)
	{
		//���_���W�̐ݒ�
		switch (nCntLogo)
		{
		case LOGOVTX_WHITE:
			pVtx[0].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x - 640.0f, g_Logo[nCntLogo].pos.y - 360.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x + 640.0f, g_Logo[nCntLogo].pos.y - 360.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x - 640.0f, g_Logo[nCntLogo].pos.y + 360.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x + 640.0f, g_Logo[nCntLogo].pos.y + 360.0f, 0.0f);
			break;

		case LOGOVTX_LOG:
			pVtx[0].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x - LOG_WIDTH, g_Logo[nCntLogo].pos.y - LOG_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x + LOG_WIDTH, g_Logo[nCntLogo].pos.y - LOG_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x - LOG_WIDTH, g_Logo[nCntLogo].pos.y + LOG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Logo[nCntLogo].pos.x + LOG_WIDTH, g_Logo[nCntLogo].pos.y + LOG_HEIGHT, 0.0f);
			break;
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_Logo[nCntLogo].col;
		pVtx[1].col = g_Logo[nCntLogo].col;
		pVtx[2].col = g_Logo[nCntLogo].col;
		pVtx[3].col = g_Logo[nCntLogo].col;

		//rhw�̐ݒ�
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLogo->Unlock();
}

//==================================================================================
//���S�̏I������
//==================================================================================
void UninitLogo(void)
{

	for (int nCntLogo = 0; nCntLogo < (sizeof c_apFilenameLogo) / sizeof(*c_apFilenameLogo); nCntLogo++)
	{
		if (g_pTextureLogo[nCntLogo] != NULL)
		{
			g_pTextureLogo[nCntLogo]->Release();
			g_pTextureLogo[nCntLogo] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffLogo != NULL)
	{
		g_pVtxBuffLogo->Release();
		g_pVtxBuffLogo = NULL;
	}

}

//==================================================================================
//���S�̍X�V����
//==================================================================================
void UpdateLogo(void)
{
	if (GetFade() == FADE_NONE)
	{//�������Ă��Ȃ��Ƃ�

		//�����J�ڂ̃J�E���^�[���Z
		g_Logo[0].nCntChange++;

		if (GetKeyboardTrigger(DIK_RETURN) == true ||
			GetGamepadTrigger(BUTTON_A, 0) ||
			GetGamepadTrigger(BUTTON_START, 0) || g_Logo[0].nCntChange >= CHANGE_TIME)
		{//Enter�������ꂽ

			SetFade(MODE_TITLE);
		}
	}
}

//==================================================================================
//���S�̕`�揈��
//==================================================================================
void DrawLogo(int nType)
{
	if (nType == DRAWTYPE_UI)
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffLogo, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntLogo = 0; nCntLogo < LOGOVTX_MAX; nCntLogo++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureLogo[nCntLogo]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLogo * 4, 2);
		}
	}
}

//==================================================================================
//���S�̏��擾
//==================================================================================
Logo *GetLogo(void)
{
	return &g_Logo[0];
}