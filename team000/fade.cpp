//==============================================================
//
//DirectX[fade.cpp]
//Author:����������
//
//==============================================================
#include"fade.h"

//�}�N����`
#define WIDTH	(960.0f)
#define HEIGHT	(360.0f)
#define WIDTH_MOVE	(640.0f * 0.05f)
#define HEIGHT_MOVE	(360.0f * 0.05f)
#define MOVE	(35.0f)
#define POS_X_DEST	(SCREEN_WIDTH + WIDTH)

//�O���[�o���ϐ�
const char *c_apFilenameFade[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\fade_01.png",
	"data\\TEXTURE\\fade_02.png",
	"data\\TEXTURE\\fade_03.png",
};

LPDIRECT3DTEXTURE9 g_pTextureFade[(sizeof c_apFilenameFade) / sizeof(*c_apFilenameFade)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFade = NULL;		//���_�o�b�t�@�ւ̃|�C���^
MODE g_modeNext;									//���̉�ʁi���[�h�j
Fade g_aFade;

//==============================================================
//�t�F�[�h�̏���������
//==============================================================
void InitFade(MODE modeNext)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameFade) / sizeof(*c_apFilenameFade); CntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameFade[CntTex],
			&g_pTextureFade[CntTex]);
	}


	for (int nCntFade = 0; nCntFade < FADE_VTX_MAX; nCntFade++)
	{
		g_aFade.fWidth[nCntFade] = WIDTH;		//��
		g_aFade.fHeight[nCntFade] = HEIGHT;		//����
	}
	g_aFade.pos = D3DXVECTOR3(320.0f, 360.0f, 0.0f);	//�ʒu
	g_aFade.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		//�F
	g_aFade.nState = FADE_IN;			//���
	g_aFade.nCntZoom = 0;		//�傫���Ȃ鎞��
	g_aFade.nCntVoice = 0;		//�{�C�X�̃J�E���^�[
	g_aFade.nTexType = rand() % 3;		//�e�N�X�`���̎��
	g_modeNext = modeNext;		//���̉�ʁi���[�h�j��ݒ�

								//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * FADE_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFade,
		NULL);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFade = 0; nCntFade < FADE_VTX_MAX; nCntFade++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aFade.pos.x - g_aFade.fWidth[nCntFade], g_aFade.pos.y - g_aFade.fHeight[nCntFade], 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFade.pos.x + g_aFade.fWidth[nCntFade], g_aFade.pos.y - g_aFade.fHeight[nCntFade], 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFade.pos.x - g_aFade.fWidth[nCntFade], g_aFade.pos.y + g_aFade.fHeight[nCntFade], 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFade.pos.x + g_aFade.fWidth[nCntFade], g_aFade.pos.y + g_aFade.fHeight[nCntFade], 0.0f);

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
	g_pVtxBuffFade->Unlock();

	//���[�h�̐ݒ�
	SetMode(g_modeNext);
}

//==============================================================
//�t�F�[�h�̏I������
//==============================================================
void UninitFade(void)
{
	//�e�N�X�`���̔j��
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameFade) / sizeof(*c_apFilenameFade); CntTex++)
	{
		if (g_pTextureFade[CntTex] != NULL)
		{
			g_pTextureFade[CntTex]->Release();
			g_pTextureFade[CntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFade != NULL)
	{
		g_pVtxBuffFade->Release();
		g_pVtxBuffFade = NULL;
	}
}

//==============================================================
//�t�F�[�h�̍X�V����
//==============================================================
void UpdateFade(void)
{
	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFade->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aFade.nState != FADE_NONE)
	{
		if (g_aFade.nState == FADE_IN)
		{//�t�F�[�h�C�����

		 //�|���S���������������Ă���
			g_aFade.pos.x += MOVE;

			if (g_aFade.pos.x >= POS_X_DEST)
			{
				//�����l�ŌŒ�
				g_aFade.pos.x = POS_X_DEST;

				//�ʏ��Ԃ�
				g_aFade.nState = FADE_NONE;
			}
		}
		else if (g_aFade.nState == FADE_OUT)
		{//�t�F�[�h�A�E�g���

		 //�|���S�������������Ă���
			g_aFade.pos.x -= MOVE;
			//g_aFade.fWidth[FADE_VTX_SHAPE] -= WIDTH_MOVE;
			//g_aFade.fHeight[FADE_VTX_SHAPE] -= HEIGHT_MOVE;

			if (g_aFade.pos.x <= 320.0f)
			{
				//0.0f�ŌŒ�
				g_aFade.pos.x = 320.0f;

				//�t�F�[�h�C����Ԃ�
				g_aFade.nState = FADE_IN;

				//���[�h�ݒ�i���̉�ʂɈڍs�j
				SetMode(g_modeNext);
			}
		}
		else if (g_aFade.nState == FADE_VOICEOUT)
		{//�t�F�[�h�A�E�g���

			g_aFade.nCntVoice++;

			if (g_aFade.nCntVoice >= 100)
			{
				//�|���S�������������Ă���
				g_aFade.pos.x -= MOVE;
			}

			if (g_aFade.pos.x <= 320.0f)
			{
				//0.0f�ŌŒ�
				g_aFade.pos.x = 320.0f;

				//�t�F�[�h�C����Ԃ�
				g_aFade.nState = FADE_IN;

				g_aFade.nCntVoice = 0;

				//���[�h�ݒ�i���̉�ʂɈڍs�j
				SetMode(g_modeNext);
			}
		}
	}

	//���_�J���[�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aFade.pos.x - g_aFade.fWidth[FADE_VTX_FADE], g_aFade.pos.y - g_aFade.fHeight[FADE_VTX_FADE], 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aFade.pos.x + g_aFade.fWidth[FADE_VTX_FADE], g_aFade.pos.y - g_aFade.fHeight[FADE_VTX_FADE], 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aFade.pos.x - g_aFade.fWidth[FADE_VTX_FADE], g_aFade.pos.y + g_aFade.fHeight[FADE_VTX_FADE], 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aFade.pos.x + g_aFade.fWidth[FADE_VTX_FADE], g_aFade.pos.y + g_aFade.fHeight[FADE_VTX_FADE], 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFade->Unlock();
}

//==============================================================
//�t�F�[�h�̕`�揈��
//==============================================================
void DrawFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

									//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFade, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntFade = 0; nCntFade < FADE_VTX_MAX; nCntFade++)
	{
		//�e�N�X�`���Z�b�g
		pDevice->SetTexture(0, g_pTextureFade[g_aFade.nTexType]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFade * 4, 2);	//�v���~�e�B�u�̎��
	}

}

//==============================================================
//�t�F�[�h�̐ݒ菈��
//==============================================================
void SetFade(MODE modeNext)
{
	if (g_aFade.nState == FADE_NONE)
	{
		g_aFade.nState = FADE_OUT;										//�t�F�[�h�A�E�g���
		g_modeNext = modeNext;									//���̉�ʁi���[�h�j��ݒ�

																//�����l�ŌŒ�
		g_aFade.pos.x = POS_X_DEST;

		g_aFade.nTexType = rand() % 3;		//�e�N�X�`���̎��

	}
}

//==============================================================
//�t�F�[�h�̐ݒ菈��
//==============================================================
void SetVoiceFade(MODE modeNext)
{
	if (g_aFade.nState == FADE_NONE)
	{
		g_aFade.nState = FADE_VOICEOUT;										//�t�F�[�h�A�E�g���
		g_modeNext = modeNext;									//���̉�ʁi���[�h�j��ݒ�
																//�����l�ŌŒ�
		g_aFade.pos.x = POS_X_DEST;
		g_aFade.nTexType = rand() % 3;		//�e�N�X�`���̎��
	}
}

//==============================================================
//�t�F�[�h�̎擾����
//==============================================================
FADE GetFade(void)
{
	return g_aFade.nState;
}