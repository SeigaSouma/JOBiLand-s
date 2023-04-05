//==============================================================
//
//DirectX[resultscore.cpp]
//Author:����������
//
//==============================================================
#include"main.h"
#include"resultscore.h"
#include"fade.h"
#include "sound.h"

//�}�N����`
#define NUM_PLACE		(2)			//�X�R�A�̌���
#define WIDTH	(100.0f)
#define HEIGHT	(100.0f)

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureResultScore = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultScore = NULL;		//���_�o�b�t�@�̃|�C���^
int g_nResultScore = 0;										//�X�R�A�̒l

ResultScore g_aResultScore[NUM_PLACE];		//�X�R�A���

//==============================================================
//���U���g�X�R�A�̏���������
//==============================================================
void InitResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int aTexU[NUM_PLACE];

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_05.png",
		&g_pTextureResultScore);

	for (int nCntRS = 0; nCntRS < NUM_PLACE; nCntRS++)
	{
		g_aResultScore[nCntRS].pos = {480.0f + nCntRS * 100.0f,300.0f,0.0f};		//�ʒu������������
		g_aResultScore[nCntRS].move = { 0.0f,0.0f,0.0f };	//�ړ��ʂ�����������
		g_aResultScore[nCntRS].fHeight = HEIGHT * 10.0f;			//����
		g_aResultScore[nCntRS].fWidth = WIDTH * 10.0f;				//��
		g_nResultScore = 0;									//�l������������
		g_aResultScore[nCntRS].bUse = false;				//�g�p����Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultScore,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	aTexU[0] = g_nResultScore % 100 / 10;
	aTexU[1] = g_nResultScore % 10 / 1;

	for (int nCntRS = 0; nCntRS < NUM_PLACE; nCntRS++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x - g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y - g_aResultScore[nCntRS].fHeight,0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x + g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y - g_aResultScore[nCntRS].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x - g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y + g_aResultScore[nCntRS].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x + g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y + g_aResultScore[nCntRS].fHeight, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;											//���_�f�[�^�̃|�C���^���S���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}

//==============================================================
//���U���g�X�R�A�̏I������
//==============================================================
void UninitResultScore(void)
{

	StopSound();

	//�e�N�X�`���̔j��
	if (g_pTextureResultScore != NULL)
	{
		g_pTextureResultScore->Release();
		g_pTextureResultScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultScore != NULL)
	{
		g_pVtxBuffResultScore->Release();
		g_pVtxBuffResultScore = NULL;
	}
}

//==============================================================
//���U���g�X�R�A�̍X�V����
//==============================================================
void UpdateResultScore(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	if (GetFade() == FADE_NONE)
	{
		for (int nCntRS = 0; nCntRS < NUM_PLACE; nCntRS++)
		{
			if (g_aResultScore[nCntRS].bUse == false)
			{
				//SE���Z�b�g
				//PlaySound(SOUND_LABEL_SE_IDOU);
			}

			g_aResultScore[nCntRS].bUse = true;		//�g�p����Ă���

			//�ړI�̈ʒu�Ɍ����킹�鏈��
			g_aResultScore[nCntRS].pos.y += ((SCREEN_HEIGHT / 2 - 200) - g_aResultScore[nCntRS].pos.y) * 0.25f;
			g_aResultScore[nCntRS].fHeight += (HEIGHT - g_aResultScore[nCntRS].fHeight) * 0.25f;
			g_aResultScore[nCntRS].fWidth += (WIDTH - g_aResultScore[nCntRS].fWidth) * 0.25f;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x - g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y - g_aResultScore[nCntRS].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x + g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y - g_aResultScore[nCntRS].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x - g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y + g_aResultScore[nCntRS].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResultScore[nCntRS].pos.x + g_aResultScore[nCntRS].fWidth, g_aResultScore[nCntRS].pos.y + g_aResultScore[nCntRS].fHeight, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}

//==============================================================
//���U���g�X�R�A�̕`�揈��
//==============================================================
void DrawResultScore(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureResultScore);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (g_aResultScore[nCntScore].bUse == true)
		{//�g�p����Ă���Ƃ�

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);

		}
	}
}

//==============================================================
//���U���g�X�R�A�̐ݒ菈��
//==============================================================
void SetResultScore(int nScore)
{
	int aTexU[NUM_PLACE];		//�e���̐������i�[
	int nCntScore;
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	g_nResultScore = nScore;

	aTexU[0] = g_nResultScore % 100 / 10;
	aTexU[1] = g_nResultScore % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;											//���_�f�[�^�̃|�C���^���S���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}

//==============================================================
//���U���g�X�R�A�̉��Z����
//==============================================================
void AddResultScore(int nValue)
{
	int aTexU[NUM_PLACE];		//�e���̐������i�[
	int nCntScore;
	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	g_nResultScore += nValue;

	aTexU[0] = g_nResultScore % 100 / 10;
	aTexU[1] = g_nResultScore % 10 / 1;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;											//���_�f�[�^�̃|�C���^���S���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultScore->Unlock();
}

//==============================================================
//���U���g�X�R�A�̎擾
//==============================================================
int GetResultScore(void)
{
	return g_nResultScore;
}