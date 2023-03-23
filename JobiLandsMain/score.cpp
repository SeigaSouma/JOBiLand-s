//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include"score.h"
#include"input.h"
#include"fade.h"
#include"game.h"

//�}�N����`
#define NUM_PLACE		(4)			//�X�R�A�̌���
#define WIDTH			(60.0f)
#define HEIGHT			(40.0f)
#define DIS_X			(-70.0f)

//�v���g�^�C�v�錾
void CreatScoreBuff(void);
void SetScore(void);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;		//���_�o�b�t�@�̃|�C���^

Score g_Score;	//�����L���O�̏��

//==============================================================
//�X�R�A�̏���������
//==============================================================
void InitScore(void)
{

	//�e��ϐ�������
	g_Score.pos = D3DXVECTOR3(640.0f, 50.0f, 0.0f);
	g_Score.nScore = 0;		//���ꂼ��̃X�R�A
	g_Score.nDigit = 0;		//����
	g_Score.fWidth = WIDTH;
	g_Score.fHeight = HEIGHT;

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		g_Score.aScoreNum[nCntScore].pos =
			D3DXVECTOR3(
				(g_Score.pos.x + (nCntScore * DIS_X)) - ((DIS_X * NUM_PLACE) * 0.5f),
				g_Score.pos.y,
				0.0f);
	}

	//�X�R�A�̒��_�o�b�t�@����
	CreatScoreBuff();

}

//==============================================================
//�X�R�A�̏I������
//==============================================================
void UninitScore(void)
{

	//�e�N�X�`���̔j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}


}

//==============================================================
//�X�R�A�̍X�V����
//==============================================================
void UpdateScore(void)
{
	//���݂̃X�R�A
	g_Score.nScore = 333;

	//�����̃e�N�X�`���ݒ�
	SetScore();
}

//==============================================================
//�X�R�A�̕`�揈��
//==============================================================
void DrawScore(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffScore, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		if (g_Score.nDigit > nCntScore)
		{//�P�^���\��

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureScore);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntScore * 4, 2);
		}
	}

}

//==============================================================
//�X�R�A�̐ݒ菈��
//==============================================================
void SetScore(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int nCount = 0, nValue = g_Score.nScore;
	do
	{//0�ɂȂ�܂ŃJ�E���g�i�߂�

		nValue /= 10;
		nCount++;

	} while (nValue > 0);

	//�P�^����o��
	g_Score.nDigit = nCount;

	int aTexU[NUM_PLACE];
	int nDigit = 1;		// aTexU�v�Z�p

	//�v�Z�p����o��
	for (int nCntDigit = 0; nCntDigit < NUM_PLACE; nCntDigit++)
	{
		nDigit *= 10;
	}
	
	//�e�N�X�`�����W�ɑ������
	for (int nCntTex = NUM_PLACE - 1; nCntTex >= 0; nCntTex--)
	{//�������ݒ�

		aTexU[nCntTex] = g_Score.nScore % nDigit / (nDigit / 10);
		nDigit /= 10;
	}

	for (int nCntScore = 0; nCntScore < g_Score.nDigit; nCntScore++)
	{
		g_Score.aScoreNum[nCntScore].pos =
			D3DXVECTOR3(
			(g_Score.pos.x + (nCntScore * DIS_X)) - ((DIS_X * g_Score.nDigit) * 0.5f),
				g_Score.pos.y,
				0.0f);
	}

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x - g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y - g_Score.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x + g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y - g_Score.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x - g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y + g_Score.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x + g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y + g_Score.fHeight, 0.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}

//==============================================================
//�X�R�A�̒��_�o�b�t�@����
//==============================================================
void CreatScoreBuff(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_04.png",
		&g_pTextureScore);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	int aTexU[NUM_PLACE];
	int nDigit = 1;		// aTexU�v�Z�p

	//�v�Z�p����o��
	for (int nCntDigit = 0; nCntDigit < NUM_PLACE; nCntDigit++)
	{
		nDigit *= 10;
	}

	//�e�N�X�`�����W�ɑ������
	for (int nCntTex = 0; nCntTex < NUM_PLACE; nCntTex++)
	{//�������ݒ�

		aTexU[nCntTex] = g_Score.nScore % nDigit / (nDigit / 10);
		nDigit /= 10;
	}

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x - g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y - g_Score.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x + g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y - g_Score.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x - g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y + g_Score.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Score.aScoreNum[nCntScore].pos.x + g_Score.fWidth, g_Score.aScoreNum[nCntScore].pos.y + g_Score.fHeight, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScore->Unlock();
}
