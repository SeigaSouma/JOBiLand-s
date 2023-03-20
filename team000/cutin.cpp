//=========================================================================
//
//�J�b�g�C������
//Author:�����V����	[cutin.cpp]
//
//=========================================================================

#include "main.h"
#include "input.h"
#include "cutin.h"
#include "sound.h"

//�֐��錾
void UpdateSentence(void);
void MoveSentence(void);

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9	g_pTextureCutin[MAX_TEX] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9		g_pVtxBuffCutin = NULL;			//���_�̃o�b�t�@
CUTIN g_aCutin[MAX_TEX];		//�J�b�g�C�����

int g_nCnter;		//�J�E���^�[

//�e�N�X�`���t�@�C����
const char *c_apFilenameCutin[] =
{
	"data/TEXTURE/cutin.png",
	"data/TEXTURE/nitya.png"
};

//=========================================================================
//�J�b�g�C���̏���������
//=========================================================================
void InitCutin(void)
{

	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCutin[nCntCutin],
			&g_pTextureCutin[nCntCutin]);
	}

	//�e��ϐ�������
	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		g_aCutin[nCntCutin].pos = { 0.0f,0.0f,0.0f };
		g_aCutin[nCntCutin].rot = { 0.0f,0.0f,0.0f };
		g_aCutin[nCntCutin].move = { 1.5f,0.0f,0.0f };

		g_aCutin[nCntCutin].col = { 1.0f,1.0f,1.0f,1.0f };

		g_aCutin[nCntCutin].fHeight = 0.0f;
		g_aCutin[nCntCutin].fWidth = 0.0f;

		g_nCnter = 0;

		g_aCutin[nCntCutin].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCutin,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCutin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		switch (g_aCutin[nCntCutin].nType)
		{
		case 0:

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - TEXT_X_CUTIN, g_aCutin[nCntCutin].pos.y - TEXT_Y_CUTIN, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + TEXT_X_CUTIN, g_aCutin[nCntCutin].pos.y - TEXT_Y_CUTIN, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - TEXT_X_CUTIN, g_aCutin[nCntCutin].pos.y + TEXT_Y_CUTIN, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + TEXT_X_CUTIN, g_aCutin[nCntCutin].pos.y + TEXT_Y_CUTIN, 0.0f);

			break;

		case 1:

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - TEXT_X_TEXT, g_aCutin[nCntCutin].pos.y - TEXT_Y_TEXT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + TEXT_X_TEXT, g_aCutin[nCntCutin].pos.y - TEXT_Y_TEXT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - TEXT_X_TEXT, g_aCutin[nCntCutin].pos.y + TEXT_Y_TEXT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + TEXT_X_TEXT, g_aCutin[nCntCutin].pos.y + TEXT_Y_TEXT, 0.0f);

			break;
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCutin->Unlock();

}

//=========================================================================
//�J�b�g�C���̏I������
//=========================================================================
void UninitCutin(void)
{
	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureCutin[nCntCutin] != NULL)
		{
			g_pTextureCutin[nCntCutin]->Release();
			g_pTextureCutin[nCntCutin] = NULL;
		}
	}

	//���_�o�b�t�@�̏���
	if (g_pVtxBuffCutin != NULL)
	{
		g_pVtxBuffCutin->Release();
		g_pVtxBuffCutin = NULL;
	}
}


//=========================================================================
//�J�b�g�C���̍X�V����
//=========================================================================
void UpdateCutin(void)
{
	
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCutin->Lock(0, 0, (void**)&pVtx, 0);

		if (g_aCutin[IMAGE].bUse == true)
		{//�g�p����Ă�����

			//�ړI�n�Ɍ������Đi�ޏ���
			g_aCutin[IMAGE].pos.x += ((SCREEN_WIDTH / 2) - g_aCutin[IMAGE].pos.x) * 0.25f;
			g_aCutin[IMAGE].fHeight += ((TEXT_Y_CUTIN) - g_aCutin[IMAGE].fHeight) * 0.25f;
			g_aCutin[IMAGE].fWidth += ((TEXT_X_CUTIN)- g_aCutin[IMAGE].fWidth) * 0.25f;

			//���_���W�̐ݒ�
			pVtx[IMAGE * 4 + 0].pos = D3DXVECTOR3(g_aCutin[IMAGE].pos.x - g_aCutin[IMAGE].fWidth, g_aCutin[IMAGE].pos.y - g_aCutin[IMAGE].fHeight, 0.0f);
			pVtx[IMAGE * 4 + 1].pos = D3DXVECTOR3(g_aCutin[IMAGE].pos.x + g_aCutin[IMAGE].fWidth, g_aCutin[IMAGE].pos.y - g_aCutin[IMAGE].fHeight, 0.0f);
			pVtx[IMAGE * 4 + 2].pos = D3DXVECTOR3(g_aCutin[IMAGE].pos.x - g_aCutin[IMAGE].fWidth, g_aCutin[IMAGE].pos.y + g_aCutin[IMAGE].fHeight, 0.0f);
			pVtx[IMAGE * 4 + 3].pos = D3DXVECTOR3(g_aCutin[IMAGE].pos.x + g_aCutin[IMAGE].fWidth, g_aCutin[IMAGE].pos.y + g_aCutin[IMAGE].fHeight, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[IMAGE * 4 + 0].col = g_aCutin[IMAGE].col;
			pVtx[IMAGE * 4 + 1].col = g_aCutin[IMAGE].col;
			pVtx[IMAGE * 4 + 2].col = g_aCutin[IMAGE].col;
			pVtx[IMAGE * 4 + 3].col = g_aCutin[IMAGE].col;
		}

		//���_�o�b�t�@���A�����b�N����
		g_pVtxBuffCutin->Unlock();

		//������̍X�V
		UpdateSentence();

}

//=========================================================================
//���͂̍X�V����
//=========================================================================
void UpdateSentence(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCutin->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aCutin[SENTENCE].bUse == true)
	{//�g�p����Ă�����

		//��������������鏈��
		MoveSentence();

		//�ړI�̈ʒu�Ɍ����킹�鏈��
		g_aCutin[SENTENCE].col.a += ((1.0f) - g_aCutin[SENTENCE].col.a) * 0.03f;
		g_aCutin[SENTENCE].pos.y += ((220.0f) - g_aCutin[SENTENCE].pos.y) * 0.01f;

		//���_���W�̐ݒ�
		pVtx[SENTENCE * 4 + 0].pos = D3DXVECTOR3(g_aCutin[SENTENCE].pos.x - g_aCutin[SENTENCE].fWidth, g_aCutin[SENTENCE].pos.y - g_aCutin[SENTENCE].fHeight, 0.0f);
		pVtx[SENTENCE * 4 + 1].pos = D3DXVECTOR3(g_aCutin[SENTENCE].pos.x + g_aCutin[SENTENCE].fWidth, g_aCutin[SENTENCE].pos.y - g_aCutin[SENTENCE].fHeight, 0.0f);
		pVtx[SENTENCE * 4 + 2].pos = D3DXVECTOR3(g_aCutin[SENTENCE].pos.x - g_aCutin[SENTENCE].fWidth, g_aCutin[SENTENCE].pos.y + g_aCutin[SENTENCE].fHeight, 0.0f);
		pVtx[SENTENCE * 4 + 3].pos = D3DXVECTOR3(g_aCutin[SENTENCE].pos.x + g_aCutin[SENTENCE].fWidth, g_aCutin[SENTENCE].pos.y + g_aCutin[SENTENCE].fHeight, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[SENTENCE * 4 + 0].col = g_aCutin[SENTENCE].col;
		pVtx[SENTENCE * 4 + 1].col = g_aCutin[SENTENCE].col;
		pVtx[SENTENCE * 4 + 2].col = g_aCutin[SENTENCE].col;
		pVtx[SENTENCE * 4 + 3].col = g_aCutin[SENTENCE].col;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCutin->Unlock();

}

//=========================================================================
//���͈ړ��̍X�V����
//=========================================================================
void MoveSentence(void)
{
	//�J�E���^�[���Z
	g_nCnter++;

	//**********************************
	//���������鏈��
	//**********************************
	if (g_aCutin[SENTENCE].pos.x > 1010.0f)
	{// [X] 1020����

		g_aCutin[SENTENCE].pos.x = 1010.0f;
		g_aCutin[SENTENCE].move.x *= -1;

	}

	else if (g_aCutin[SENTENCE].pos.x < 990.0f)
	{// [X] 980��艺

		g_aCutin[SENTENCE].pos.x = 990.0f;
		g_aCutin[SENTENCE].move.x *= -1;

	}

	//�ʒu�̍X�V
	g_aCutin[SENTENCE].pos.x += g_aCutin[SENTENCE].move.x;

	if (g_nCnter >= 80)
	{//80�ȏゾ������

		//���l���Z
		g_aCutin[SENTENCE].col.a -= 0.1f;
		g_aCutin[IMAGE].col.a -= 0.1f;

		for (int nCnt = 0; nCnt < MAX_TEX; nCnt++)
		{
			if (g_aCutin[nCnt].col.a <= 0.0f)
			{//���l��0.0f�ȉ��̂Ƃ�

				g_aCutin[SENTENCE].bUse = false;	//�g�p����Ă��Ȃ���Ԃɂ���
				g_aCutin[IMAGE].bUse = false;	//�g�p����Ă��Ȃ���Ԃɂ���
				g_nCnter = 0;		//�J�E���^�[������

				//�t�B�[�o�[��
				PlaySound(SOUND_LABEL_BGM_FEVER);
			}
		}
	}
}

//=========================================================================
//�J�b�g�C���̕`�揈��
//=========================================================================
void DrawCutin(void)
{

	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCutin, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		if (g_aCutin[nCntCutin].bUse == true)
		{//�g�p����Ă�����

			switch (g_aCutin[nCntCutin].nType)
			{
			case 0:

				//�J�b�g�C���̕`��
				pDevice->SetTexture(0, g_pTextureCutin[0]);

				break;

			case 1:

				//�J�b�g�C���̕`��
				pDevice->SetTexture(0, g_pTextureCutin[1]);

				break;
			}

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCutin * 4, 2);		//�v���~�e�B�u�̎��
		}

	}
}

//=========================================================================
//�J�b�g�C���̐ݒ菈��
//=========================================================================
void SetCutin(D3DXVECTOR3 pos, int nType, float fHeight,float fWidth)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffCutin->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCutin = 0; nCntCutin < MAX_TEX; nCntCutin++)
	{
		if (g_aCutin[nCntCutin].bUse == false)
		{//�J�b�g�C�����g�p����Ă��Ȃ���

			g_aCutin[nCntCutin].pos = pos;
			g_aCutin[nCntCutin].nType = nType;
			g_aCutin[nCntCutin].fHeight = fHeight;
			g_aCutin[nCntCutin].fWidth = fWidth;

			//�g�p����Ă����Ԃɂ���
			g_aCutin[nCntCutin].bUse = true;

			g_aCutin[SENTENCE].col.a = 0.0f;
			g_aCutin[IMAGE].col.a = 1.0f;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - g_aCutin[nCntCutin].fWidth, g_aCutin[nCntCutin].pos.y - g_aCutin[nCntCutin].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + g_aCutin[nCntCutin].fWidth, g_aCutin[nCntCutin].pos.y - g_aCutin[nCntCutin].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x - g_aCutin[nCntCutin].fWidth, g_aCutin[nCntCutin].pos.y + g_aCutin[nCntCutin].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCutin[nCntCutin].pos.x + g_aCutin[nCntCutin].fWidth, g_aCutin[nCntCutin].pos.y + g_aCutin[nCntCutin].fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCutin->Unlock();
}

//=========================================================================
//�J�b�g�C���̏��擾
//=========================================================================
CUTIN *GetCutin(void)
{
	return &g_aCutin[0];
}