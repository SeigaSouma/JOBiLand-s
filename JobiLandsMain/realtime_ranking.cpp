//=============================================================================
//
// ���A���^�C�������L���O���� [realtime_ranking.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include"realtime_ranking.h"
#include"input.h"
#include"fade.h"
#include"crowd.h"
#include"game.h"

//�}�N����`
#define DEFAULT_SCORE	(100)
#define MAX_PATTERN_LIVE	(2)
#define MIN_RANKING		(5)			//�����L���O���̍ŏ��l
#define NUM_PLACE		(4)			//�X�R�A�̌���
#define WIDTH			(60.0f * 0.45f)
#define HEIGHT			(40.0f * 0.45f)
#define DIS_X			(-30.0f)
#define TEXT_WIDTH			(350.0f * 0.5f)
#define TEXT_HEIGHT			(35.0f * 0.5f)
#define FILE_RANK		"data\\TEXT\\realtime_ranking.bin"		//�����L���O�t�@�C��

//�v���g�^�C�v�錾
void SetRealTimeRanking(int nCntRank);
void SaveDataRealTimeRanking(void);
void LoadDataRealTimeRanking(void);
void CalRealTimeRanking(void);
void CalNowRealTimeRanking(void);
void UpdateRealTimeRankingTop(void);
void UpdateRealTimeRankingLive(void);

void CreatRealTimeRankingBuff(void);
void CreatRealTimeText(void);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureRealTimeRanking = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRealTimeRanking = NULL;		//���_�o�b�t�@�̃|�C���^

const char *c_apFilenameRealTimeText[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\neet_vision1.png",
	"data\\TEXTURE\\nextranknum_01.png",
	"data\\TEXTURE\\nowranking_01.png",
	"data\\TEXTURE\\ranktop_01.png",
	"data\\TEXTURE\\neet_vision2.png",
};
LPDIRECT3DTEXTURE9 g_pTextureRealTimeText[(sizeof c_apFilenameRealTimeText) / sizeof(*c_apFilenameRealTimeText)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRealTimeText = NULL;		//���_�o�b�t�@�̃|�C���^

RealTimeRanking g_RealTimeRanking;	//�����L���O�̏��
RealTimeText g_aRealTimeText[REALRANKINGTEXT_VTX_MAX];		//�e�L�X�g�̏��

//==============================================================
//���A���^�C�������L���O�̏���������
//==============================================================
void InitRealTimeRanking(void)
{

	//�e��ϐ�������
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].pos = D3DXVECTOR3(200.0f, 50.0f, 0.0f);
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].pos = D3DXVECTOR3(200.0f, 100.0f, 0.0f);

	for (int nCntRank = 0; nCntRank < REALRANKING_TYPE_MAX; nCntRank++)
	{
		g_RealTimeRanking.aEachRealRanking[nCntRank].nScore = 3;		//���ꂼ��̃X�R�A
		g_RealTimeRanking.aEachRealRanking[nCntRank].nDigit = 0;		//����
		g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth = WIDTH;
		g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight = HEIGHT;

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			if (nCntRank == REALRANKING_TYPE_RANKING)
			{
				g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos =
					D3DXVECTOR3(
						-(2 * DIS_X * 0.9f) + g_RealTimeRanking.aEachRealRanking[nCntRank].pos.x + (nCntScore * DIS_X * 0.9f),
						g_RealTimeRanking.aEachRealRanking[nCntRank].pos.y,
						0.0f);
			}
			else
			{
				g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos =
					D3DXVECTOR3(
						-(2 * DIS_X * 0.9f) + g_RealTimeRanking.aEachRealRanking[nCntRank].pos.x + (nCntScore * DIS_X * 0.9f),
						g_RealTimeRanking.aEachRealRanking[nCntRank].pos.y,
						0.0f);
			}

		}
	}

	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].fWidth = WIDTH * 0.95f;
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].fHeight = HEIGHT * 0.95f;
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].fWidth = WIDTH * 0.95f;
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].fHeight = HEIGHT * 0.95f;

	for (int nCntAllRank = 0; nCntAllRank < 512; nCntAllRank++)
	{//�S�Ẵ����L���O�f�[�^������

		g_RealTimeRanking.nScore[nCntAllRank] = 0;
	}
	g_RealTimeRanking.nNumRanking = 0;	//�����L���O�̐�
	g_RealTimeRanking.nNowScore = 0;		//���݂̃X�R�A
	g_RealTimeRanking.bTop = false;			//1�ʂ��ǂ���

	//���[�h����
	LoadDataRealTimeRanking();

	//�v�Z����
	CalRealTimeRanking();

	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].nScore = g_RealTimeRanking.nScore[g_RealTimeRanking.nNumRanking - 1];		//���ꂼ��̃X�R�A

	g_RealTimeRanking.nNowRank = g_RealTimeRanking.nNumRanking + 1;	//���݂̏���


	CreatRealTimeRankingBuff();
	CreatRealTimeText();

}

//==============================================================
//���A���^�C�������L���O�̏I������
//==============================================================
void UninitRealTimeRanking(void)
{

	//�e�N�X�`���̔j��
	if (g_pTextureRealTimeRanking != NULL)
	{
		g_pTextureRealTimeRanking->Release();
		g_pTextureRealTimeRanking = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRealTimeRanking != NULL)
	{
		g_pVtxBuffRealTimeRanking->Release();
		g_pVtxBuffRealTimeRanking = NULL;
	}

	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRealTimeText) / sizeof(*c_apFilenameRealTimeText); nCntTex++)
	{
		if (g_pTextureRealTimeText[nCntTex] != NULL)
		{
			g_pTextureRealTimeText[nCntTex]->Release();
			g_pTextureRealTimeText[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRealTimeText != NULL)
	{
		g_pVtxBuffRealTimeText->Release();
		g_pVtxBuffRealTimeText = NULL;
	}

}

//==============================================================
//���A���^�C�������L���O�̍X�V����
//==============================================================
void UpdateRealTimeRanking(void)
{

	//�Q�O�̐l���擾
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//���݂̃X�R�A
	g_RealTimeRanking.nNowScore = 3 + pCrowdInfo->nCrowdValue;

	//���݂̏��ʌv�Z����
	CalNowRealTimeRanking();

	//�L�^�X�V���̏���
	UpdateRealTimeRankingTop();

	//���C�u�A�j���[�V����
	UpdateRealTimeRankingLive();

	for (int nCntRank = 0; nCntRank < REALRANKING_TYPE_MAX; nCntRank++)
	{
		//�����̃e�N�X�`���ݒ�
		SetRealTimeRanking(nCntRank);
	}
}

//==============================================================
//���A���^�C�������L���O�̋L�^�X�V������
//==============================================================
void UpdateRealTimeRankingTop(void)
{

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRealTimeText->Lock(0, 0, (void**)&pVtx, 0);

	static int nMove = 1;

	if (g_RealTimeRanking.bTop == true)
	{//�L�^�X�V����������

		g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth += (TEXT_WIDTH * 0.025f) * nMove;
		g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight += (TEXT_HEIGHT * 0.025f) * nMove;

		if (g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth >= TEXT_WIDTH * 1.5f)
		{
			g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth = TEXT_WIDTH * 1.5f;
			g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight = TEXT_HEIGHT * 1.5f;
			nMove *= -1;
		}
		else if (g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth <= TEXT_WIDTH)
		{
			g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth = TEXT_WIDTH;
			g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight = TEXT_HEIGHT;
			nMove *= -1;
		}
	}
	else
	{
		g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth = TEXT_WIDTH;
		g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight = TEXT_HEIGHT;
	}

	//���_���W�̐ݒ�
	pVtx[REALRANKINGTEXT_VTX_TOP * 4 + 0].pos = D3DXVECTOR3(g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.x - g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth, g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.y - g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_TOP * 4 + 1].pos = D3DXVECTOR3(g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.x + g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth, g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.y - g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_TOP * 4 + 2].pos = D3DXVECTOR3(g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.x - g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth, g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.y + g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_TOP * 4 + 3].pos = D3DXVECTOR3(g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.x + g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fWidth, g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos.y + g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.fHeight, 0.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRealTimeText->Unlock();
}

//==============================================================
//���A���^�C�������L���O�̃��C�u�_�ŏ���
//==============================================================
void UpdateRealTimeRankingLive(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRealTimeText->Lock(0, 0, (void**)&pVtx, 0);

	g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nCntAnim++;	//�J�E���^�[�����Z

	if ((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nCntAnim % 60) == 0)
	{//�K��J�E���g�i�񂾂�

		//�p�^�[��No.���X�V
		g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim = (g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim + 1) % MAX_PATTERN_LIVE;
		g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nCntAnim = 0;
	}

	//���_���W�̐ݒ�
	pVtx[REALRANKINGTEXT_VTX_FADE * 4 + 0].tex = D3DXVECTOR2((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim % MAX_PATTERN_LIVE) * 0.5f, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_FADE * 4 + 1].tex = D3DXVECTOR2((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim % MAX_PATTERN_LIVE + 1) * 0.5f, 0.0f);
	pVtx[REALRANKINGTEXT_VTX_FADE * 4 + 2].tex = D3DXVECTOR2((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim % MAX_PATTERN_LIVE) * 0.5f, 1.0f);
	pVtx[REALRANKINGTEXT_VTX_FADE * 4 + 3].tex = D3DXVECTOR2((g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.nPatternAnim % MAX_PATTERN_LIVE + 1) * 0.5f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRealTimeText->Unlock();
}

//==============================================================
//���A���^�C�������L���O�̕`�揈��
//==============================================================
void DrawRealTimeRanking(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRealTimeText, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRank = 0; nCntRank < REALRANKINGTEXT_VTX_GIZA; nCntRank++)
	{
		if (nCntRank == REALRANKINGTEXT_VTX_TOP && g_RealTimeRanking.bTop == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRealTimeText[nCntRank]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
		}
		else if (nCntRank != REALRANKINGTEXT_VTX_TOP)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRealTimeText[nCntRank]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntRank * 4, 2);
		}
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRealTimeRanking, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntRank = 0; nCntRank < g_RealTimeRanking.nNumRanking; nCntRank++)
	{
		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			if (g_RealTimeRanking.aEachRealRanking[nCntRank].nDigit > nCntScore)
			{//�P�^���\��

				if (nCntRank == REALRANKING_TYPE_RANKING && g_RealTimeRanking.bTop == false)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pTextureRealTimeRanking);

					//�|���S���̕`��
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntScore * 4) + (nCntRank * NUM_PLACE * 4), 2);
				}
				else if (nCntRank != REALRANKING_TYPE_RANKING)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pTextureRealTimeRanking);

					//�|���S���̕`��
					pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntScore * 4) + (nCntRank * NUM_PLACE * 4), 2);
				}
			}
		}
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRealTimeText, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_RealTimeRanking.bTop == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRealTimeText[REALRANKINGTEXT_VTX_TOP]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, REALRANKINGTEXT_VTX_TOP * 4, 2);
	}

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRealTimeText, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureRealTimeText[REALRANKINGTEXT_VTX_GIZA]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, REALRANKINGTEXT_VTX_GIZA * 4, 2);
	

}

//==============================================================
//���A���^�C�������L���O�̐ݒ菈��
//==============================================================
void SetRealTimeRanking(int nCntRank)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRealTimeRanking->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntRank * NUM_PLACE;

	int nCount = 0, nValue = g_RealTimeRanking.aEachRealRanking[nCntRank].nScore;
	do
	{//0�ɂȂ�܂ŃJ�E���g�i�߂�

		nValue /= 10;
		nCount++;

	} while (nValue > 0);

	//�P�^����o��
	g_RealTimeRanking.aEachRealRanking[nCntRank].nDigit = nCount;

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

		aTexU[nCntTex] = g_RealTimeRanking.aEachRealRanking[nCntRank].nScore % nDigit / (nDigit / 10);
		nDigit /= 10;
	}

	for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(aTexU[nCntScore] * 0.1f + 0.1f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRealTimeRanking->Unlock();
}

//==============================================================
//���A���^�C�������L���O�̌��݂̏��ʌv�Z����
//==============================================================
void CalNowRealTimeRanking(void)
{
	int nCntAllRank = 0;
	for (nCntAllRank = 0; nCntAllRank < g_RealTimeRanking.nNumRanking; nCntAllRank++)
	{//�S�Ẵ����L���O�f�[�^

		if (g_RealTimeRanking.nScore[nCntAllRank] <= g_RealTimeRanking.nNowScore)
		{//���݂̃����L���O�̕����傫��������

			//���ʂ�ݒ�
			g_RealTimeRanking.nNowRank = nCntAllRank + 1;

			if (nCntAllRank <= 0)
			{//0�ȉ��̎�
				
				//1�ʂ��ǂ����̔���
				g_RealTimeRanking.bTop = true;
			}
			else
			{
				g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].nScore = g_RealTimeRanking.nScore[nCntAllRank - 1];

				//1�ʂ��ǂ����̔���
				g_RealTimeRanking.bTop = false;
			}
			break;
		}
	}
	

	if (g_RealTimeRanking.nScore[nCntAllRank - 1] > g_RealTimeRanking.nNowScore)
	{//���݂̃����L���O��1���������݂̃X�R�A���Ⴂ

		//�����L���O��1������
		g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].nScore = g_RealTimeRanking.nScore[nCntAllRank - 1];
		g_RealTimeRanking.nNowRank = nCntAllRank + 1;
	}

	//���݂̏���
	g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].nScore = g_RealTimeRanking.nNowRank;
}

//==============================================================
//���[�h����
//==============================================================
void LoadDataRealTimeRanking(void)
{
	FILE *pFile;

	//�t�@�C�����J��
	pFile = fopen(FILE_RANK, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		//�t�@�C���ɐ��l��ǂݍ���
		fread(&g_RealTimeRanking.nNumRanking, sizeof(int), 1, pFile);
		fread(&g_RealTimeRanking.nScore[0], sizeof(int), g_RealTimeRanking.nNumRanking, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ

		g_RealTimeRanking.nNumRanking = MIN_RANKING;
		for (int nCount = 0; nCount < g_RealTimeRanking.nNumRanking; nCount++)
		{//�S���[��

			g_RealTimeRanking.nScore[nCount] = 200 - (nCount * 10);
		}

		SaveDataRealTimeRanking();

	}
}

//==============================================================
//�v�Z����
//==============================================================
void CalRealTimeRanking(void)
{

	//�~������
	for (int nCntData = 0; nCntData < g_RealTimeRanking.nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < g_RealTimeRanking.nNumRanking; nCntSort++)
		{
			if (g_RealTimeRanking.nScore[nCntData] < g_RealTimeRanking.nScore[nCntSort])
			{//�v�f1���v�f2���傫��������

				int nTemp = g_RealTimeRanking.nScore[nCntData];
				g_RealTimeRanking.nScore[nCntData] = g_RealTimeRanking.nScore[nCntSort];
				g_RealTimeRanking.nScore[nCntSort] = nTemp;
			}
		}
	}
}

//==============================================================
//�Z�[�u����
//==============================================================
void SaveDataRealTimeRanking(void)
{
	FILE *pFile;

	//�t�@�C�����J��
	pFile = fopen(FILE_RANK, "wb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		//�t�@�C���ɐ��l�������o��
		fwrite(&g_RealTimeRanking.nNumRanking, sizeof(int), 1, pFile);
		fwrite(&g_RealTimeRanking.nScore[0], sizeof(int), g_RealTimeRanking.nNumRanking, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}

//==============================================================
//���A���^�C�������L���O�̒��_�o�b�t�@����
//==============================================================
void CreatRealTimeRankingBuff(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_04.png",
		&g_pTextureRealTimeRanking);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE * REALRANKING_TYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRealTimeRanking,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRealTimeRanking->Lock(0, 0, (void**)&pVtx, 0);

	int aTexU[NUM_PLACE];

	for (int nCntRank = 0; nCntRank < REALRANKING_TYPE_MAX; nCntRank++)
	{
		int nDigit = 1;		// aTexU�v�Z�p

							//�v�Z�p����o��
		for (int nCntDigit = 0; nCntDigit < NUM_PLACE; nCntDigit++)
		{
			nDigit *= 10;
		}

		//�e�N�X�`�����W�ɑ������
		for (int nCntTex = 0; nCntTex < NUM_PLACE; nCntTex++)
		{//�������ݒ�

			aTexU[nCntTex] = g_RealTimeRanking.aEachRealRanking[nCntRank].nScore % nDigit / (nDigit / 10);
			nDigit /= 10;
		}

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x - g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth, g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y - g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x + g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth, g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y - g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x - g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth, g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y + g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x + g_RealTimeRanking.aEachRealRanking[nCntRank].fWidth, g_RealTimeRanking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y + g_RealTimeRanking.aEachRealRanking[nCntRank].fHeight, 0.0f);

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
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRealTimeRanking->Unlock();
}

//==============================================================
//���A���^�C�������L���O(�e�L�X�g)�̒��_�o�b�t�@����
//==============================================================
void CreatRealTimeText(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRealTimeText) / sizeof(*c_apFilenameRealTimeText); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameRealTimeText[nCntTex],
			&g_pTextureRealTimeText[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * REALRANKINGTEXT_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRealTimeText,
		NULL);

	for (int nCntRank = 0; nCntRank < REALRANKINGTEXT_VTX_MAX; nCntRank++)
	{
		//UI���̏�����
		ZeroMemory(&g_aRealTimeText[nCntRank].aUI, sizeof(UI_2D));
	}

	for (int nCntRank = 0; nCntRank < REALRANKINGTEXT_VTX_MAX; nCntRank++)
	{
		g_aRealTimeText[nCntRank].aUI.fWidth = TEXT_WIDTH;
		g_aRealTimeText[nCntRank].aUI.fHeight = TEXT_HEIGHT;
	}

	//�t�F�[�h��
	g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.pos = D3DXVECTOR3(
		165.0f,
		75.0f,
		0.0f);

	//���̏��ʂ̐l��
	g_aRealTimeText[REALRANKINGTEXT_VTX_NEXTNUM].aUI.pos = D3DXVECTOR3(
		195.0f,
		g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].pos.y,
		0.0f);

	//���݂̏���&���݂̐l��
	g_aRealTimeText[REALRANKINGTEXT_VTX_NOW].aUI.pos = D3DXVECTOR3(
		255.0f,
		g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKNUM].pos.y,
		0.0f);

	g_aRealTimeText[REALRANKINGTEXT_VTX_TOP].aUI.pos = D3DXVECTOR3(
		200.0f,
		g_RealTimeRanking.aEachRealRanking[REALRANKING_TYPE_RANKING].pos.y,
		0.0f);

	//�M�U�M�U�m�C�Y
	g_aRealTimeText[REALRANKINGTEXT_VTX_GIZA].aUI.pos = g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.pos;

	g_aRealTimeText[REALRANKINGTEXT_VTX_NOW].aUI.fWidth = TEXT_WIDTH;
	g_aRealTimeText[REALRANKINGTEXT_VTX_NOW].aUI.fHeight = TEXT_HEIGHT;

	g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.fWidth = 165.0f;
	g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.fHeight = TEXT_HEIGHT * 6.0f;

	g_aRealTimeText[REALRANKINGTEXT_VTX_GIZA].aUI.fWidth = g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.fWidth;
	g_aRealTimeText[REALRANKINGTEXT_VTX_GIZA].aUI.fHeight = g_aRealTimeText[REALRANKINGTEXT_VTX_FADE].aUI.fHeight;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRealTimeText->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRank = 0; nCntRank < REALRANKINGTEXT_VTX_MAX; nCntRank++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aRealTimeText[nCntRank].aUI.pos.x - g_aRealTimeText[nCntRank].aUI.fWidth, g_aRealTimeText[nCntRank].aUI.pos.y - g_aRealTimeText[nCntRank].aUI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRealTimeText[nCntRank].aUI.pos.x + g_aRealTimeText[nCntRank].aUI.fWidth, g_aRealTimeText[nCntRank].aUI.pos.y - g_aRealTimeText[nCntRank].aUI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRealTimeText[nCntRank].aUI.pos.x - g_aRealTimeText[nCntRank].aUI.fWidth, g_aRealTimeText[nCntRank].aUI.pos.y + g_aRealTimeText[nCntRank].aUI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRealTimeText[nCntRank].aUI.pos.x + g_aRealTimeText[nCntRank].aUI.fWidth, g_aRealTimeText[nCntRank].aUI.pos.y + g_aRealTimeText[nCntRank].aUI.fHeight, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		if (nCntRank != REALRANKINGTEXT_VTX_GIZA)
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);
		}

		//�e�N�X�`�����W�̐ݒ�
		if (nCntRank == REALRANKINGTEXT_VTX_FADE)
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRealTimeText->Unlock();
}