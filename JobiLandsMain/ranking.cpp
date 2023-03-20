//=============================================================================
//
// �����L���O���� [ranking.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "ranking.h"
#include "input.h"
#include "fade.h"
#include "game.h"
#include "light.h"
#include "camera.h"
#include "shadow.h"
#include "edit.h"
#include "meshfield.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "particle.h"
#include "effect.h"
#include "2D_effect.h"
#include "player.h"
#include "sound.h"
#include "sound.h"

//�}�N����`
#define DEFAULT_SCORE	(100)
#define MIN_RANKING		(5)			//�����L���O���̍ŏ��l
#define NUM_PLACE		(4)			//�X�R�A�̌���
#define SET_TIME		(80)
#define CHANGE_TIME		(60 * 25)
#define START_X			(580.0f)
#define START_TEXT_X	(640.0f)
#define WIDTH			(60.0f * 1.5f)
#define HEIGHT			(40.0f * 1.5f)
#define DIS_X			(-42.0f * 1.9f)
#define DIS_Y			(130.0f)
//#define TEXT_WIDTH		(64.0f * 5.0f)
#define TEXT_WIDTH		(640.0f)
#define TEXT_HEIGHT		(360.0f)
#define FILE_RANK		"data\\TEXT\\realtime_ranking.bin"		//�����L���O�t�@�C��

//�v���g�^�C�v�錾
void SetRanking(int nCntRank);
void SaveDataRanking(void);
void LoadDataRanking(void);
void CalRanking(void);
void SetProvisionRanking(void);
void UpdateNewRecordRanking(void);

void CreatRankingBuff(void);
void CreatRankingText(void);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureRanking = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRanking = NULL;		//���_�o�b�t�@�̃|�C���^

const char *c_apFilenameRankingText[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\ranking_02.png",
};
LPDIRECT3DTEXTURE9 g_pTextureRankingText[(sizeof c_apFilenameRankingText) / sizeof(*c_apFilenameRankingText)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingText = NULL;		//���_�o�b�t�@�̃|�C���^

Ranking g_Ranking;	//�����L���O�̏��
RankingText g_aRankingText;		//�e�L�X�g�̏��

//==============================================================
//�����L���O�̏���������
//==============================================================
void InitRanking(int nOldMode)
{

	//�e��ϐ�������
	for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
	{
		g_Ranking.aEachRealRanking[nCntRank].pos = D3DXVECTOR3(START_X, 100.0f + DIS_Y * nCntRank, 0.0f);
		g_Ranking.aEachRealRanking[nCntRank].posDest = D3DXVECTOR3(START_X, 100.0f + DIS_Y * nCntRank, 0.0f);	//�ڕW�̈ʒu
		g_Ranking.aEachRealRanking[nCntRank].nScore = 0;		//���ꂼ��̃X�R�A
		g_Ranking.aEachRealRanking[nCntRank].nDigit = 0;		//����
		g_Ranking.aEachRealRanking[nCntRank].fWidth = WIDTH;
		g_Ranking.aEachRealRanking[nCntRank].fHeight = HEIGHT;
		g_Ranking.aEachRealRanking[nCntRank].bUse = false;		//�g�p���Ă��邩

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos =
				D3DXVECTOR3(
					g_Ranking.aEachRealRanking[nCntRank].pos.x + (nCntScore * DIS_X),
					g_Ranking.aEachRealRanking[nCntRank].pos.y,
					0.0f);
		}
	}

	for (int nCntAllRank = 0; nCntAllRank < 512; nCntAllRank++)
	{//�S�Ẵ����L���O�f�[�^������

		g_Ranking.nScore[nCntAllRank] = 0;
	}
	g_Ranking.nNumRanking = 0;	//�����L���O�̐�
	g_Ranking.nChangeTime = 0;	//�����J�ڂ̃J�E���^�[
	g_Ranking.nCntSetCounter = 0;	//�Z�b�g����܂ł̃J�E���^�[
	g_Ranking.nCntUse = 0;		//�g�p�J�E���g

	//����̃X�R�A�擾
	g_Ranking.nNewRecordNum = -1;	//�j���[���R�[�h�̔ԍ�
	g_Ranking.bNewRecord = false;	//�j���[���R�[�h�̔���
	g_Ranking.nOldMode = nOldMode;		//�J�ڑO�̃��[�h

	//���[�h����
	LoadDataRanking();

	if (g_Ranking.nOldMode == MODE_RESULT)
	{//�����J�ڂł͍X�V���Ȃ�

		//�v�Z����
		CalRanking();

		//�Z�[�u
		SaveDataRanking();
	}

	//���_�o�b�t�@����
	CreatRankingBuff();
	CreatRankingText();

	for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
	{
		SetRanking(nCntRank);
	}

	//�e�̏���������
	InitShadow();

	//���f���̏���������
	InitModel();

	//�G�f�B�b�g�̏�����
	InitEdit();

	//�v���C���[�̏���������
	InitPlayer();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//2D�G�t�F�N�g�̏���������
	InitEffect_2D();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�J��������������
	InitCamera();

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM_RANKING);
}

//==============================================================
//�����L���O�̏I������
//==============================================================
void UninitRanking(void)
{

	//�T�E���h�̒�~
	StopSound();

	//�e�̏I������
	UninitShadow();

	//���f���̏I������
	UninitModel();

	//�G�f�B�b�g�̏�����
	UninitEdit();

	//�v���C���[�̏I������
	UninitPlayer();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���h�[���̏I������
	UninitMeshDome();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//2D�G�t�F�N�g�̏I������
	UninitEffect_2D();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	//�e�N�X�`���̔j��
	if (g_pTextureRanking != NULL)
	{
		g_pTextureRanking->Release();
		g_pTextureRanking = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRanking != NULL)
	{
		g_pVtxBuffRanking->Release();
		g_pVtxBuffRanking = NULL;
	}

	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRankingText) / sizeof(*c_apFilenameRankingText); nCntTex++)
	{
		if (g_pTextureRankingText[nCntTex] != NULL)
		{
			g_pTextureRankingText[nCntTex]->Release();
			g_pTextureRankingText[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRankingText != NULL)
	{
		g_pVtxBuffRankingText->Release();
		g_pVtxBuffRankingText = NULL;
	}

}

//==============================================================
//�����L���O�̍X�V����
//==============================================================
void UpdateRanking(void)
{
	//�Z�b�g����܂ł̃J�E���^�[���Z
	g_Ranking.nCntSetCounter++;

	if (SET_TIME <= g_Ranking.nCntSetCounter)
	{//�K��l�𒴂�����

		//�����L���O��������ݒ�
		SetRankingPos();
	}

	if (g_Ranking.nOldMode == MODE_RESULT && g_Ranking.bNewRecord == true &&
		SET_TIME < g_Ranking.nCntSetCounter && g_Ranking.nCntUse >= MIN_RANKING)
	{//�J�ڑO�����U���g && �j���[���R�[�h�̎�

		//�j���[���R�[�h���̍X�V����
		UpdateNewRecordRanking();
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0) || CHANGE_TIME <= g_Ranking.nChangeTime)
	{//Enter�������ꂽ

		if (g_Ranking.nCntUse >= MIN_RANKING)
		{//�S���o�Ă�����

			SetFade(MODE_TITLE);
		}
		else
		{
			SetProvisionRanking();
		}
	}

	for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
	{
		//�����̃e�N�X�`���ݒ�
		SetRanking(nCntRank);
	}
}

//==============================================================
//�����L���O�̋K��l�ɐݒ菈��
//==============================================================
void SetProvisionRanking(void)
{
	for (int nCntRank = 0; nCntRank < MIN_RANKING - g_Ranking.nChangeTime; nCntRank++)
	{
		//�ݒ菈��
		SetRankingPos();

		//�ʒu�␳
		g_Ranking.aEachRealRanking[nCntRank].pos.y = g_Ranking.aEachRealRanking[nCntRank].posDest.y;

		//�����␳
		g_Ranking.aEachRealRanking[nCntRank].fWidth = WIDTH;
		g_Ranking.aEachRealRanking[nCntRank].fHeight = HEIGHT;
	}
}

//==============================================================
//�����L���O�̃j���[���R�[�h���X�V����
//==============================================================
void UpdateNewRecordRanking(void)
{

	static int nMove = 1;	//�ړ�����

	//�j���[���R�[�h�̃����L���O���g�k����
	g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth += (WIDTH * 0.02f) * nMove;
	g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fHeight += (HEIGHT * 0.02f) * nMove;

	if (WIDTH * 1.5f < g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth)
	{//�K��l�𒴂�����

		g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth = WIDTH * 1.5f;
		g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fHeight = HEIGHT * 1.5f;
		nMove *= -1;
	}
	else if (WIDTH > g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth)
	{//�K��l�����������

		g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fWidth = WIDTH;
		g_Ranking.aEachRealRanking[g_Ranking.nNewRecordNum].fHeight = HEIGHT;
		nMove *= -1;
	}

}

//==============================================================
//�����L���O�̕`�揈��
//==============================================================
void DrawRanking(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{
		//���f���̕`�揈��
		DrawModel(0);

		//�G�f�B�b�g�̏�����
		DrawEdit();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//���b�V���t�B�[���h�̕`�揈��
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���h�[���̕`�揈��
		DrawMeshDome();

		//�e�̕`�揈��
		DrawShadow();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();
	}
	else if (nType == DRAWTYPE_UI)
	{
		//2D�G�t�F�N�g�̕`�揈��
		DrawEffect_2D();

		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffRankingText, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureRankingText[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffRanking, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
		{
			if (g_Ranking.aEachRealRanking[nCntRank].bUse == true)
			{//�g�p����Ă�����

				for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
				{
					if (g_Ranking.aEachRealRanking[nCntRank].nDigit > nCntScore)
					{//�P�^���\��

						//�e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_pTextureRanking);

						//�|���S���̕`��
						pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntScore * 4) + (nCntRank * NUM_PLACE * 4), 2);
					}
				}
			}
		}
	}
}

//==============================================================
//�����L���O�̃e�N�X�`���ݒ菈��
//==============================================================
void SetRanking(int nCntRank)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntRank * NUM_PLACE;

	int nCount = 0, nValue = g_Ranking.nScore[nCntRank];
	do
	{//0�ɂȂ�܂ŃJ�E���g�i�߂�

		nValue /= 10;
		nCount++;

	} while (nValue > 0);

	//�P�^����o��
	g_Ranking.aEachRealRanking[nCntRank].nDigit = nCount;

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

		aTexU[nCntTex] = g_Ranking.nScore[nCntRank] % nDigit / (nDigit / 10);
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
	g_pVtxBuffRanking->Unlock();
}

//==============================================================
//�����L���O�̐ݒ菈��
//==============================================================
void SetRankingPos(void)
{
	for (int nCntRank = MIN_RANKING - 1; nCntRank >= 0; nCntRank--)
	{
		if (g_Ranking.aEachRealRanking[nCntRank].bUse == false)
		{//�g�p����Ă��Ȃ�������

			g_Ranking.aEachRealRanking[nCntRank].pos = D3DXVECTOR3(START_X, 100.0f + DIS_Y * nCntRank, 0.0f);
			g_Ranking.aEachRealRanking[nCntRank].fWidth = WIDTH * 10.0f;
			g_Ranking.aEachRealRanking[nCntRank].fHeight = HEIGHT * 10.0f;
			g_Ranking.aEachRealRanking[nCntRank].bUse = true;

			//�Z�b�g����Ԋu���Z�b�g
			g_Ranking.nCntSetCounter = 0;

			//�g�p���Ă�J�E���g���Z
			g_Ranking.nCntUse++;

			for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
			{//�������ʒu�ݒ�

				g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos =
					D3DXVECTOR3(
						g_Ranking.aEachRealRanking[nCntRank].pos.x + (nCntScore * DIS_X),
						g_Ranking.aEachRealRanking[nCntRank].pos.y,
						0.0f);
			}

			//SE���Z�b�g
			PlaySound(SOUND_LABEL_SE_IDOU);

			break;
		}
	}
}

//==============================================================
//���[�h����
//==============================================================
void LoadDataRanking(void)
{
	FILE *pFile;

	//�t�@�C�����J��
	pFile = fopen(FILE_RANK, "rb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		//�t�@�C���ɐ��l��ǂݍ���
		fread(&g_Ranking.nNumRanking, sizeof(int), 1, pFile);
		fread(&g_Ranking.nScore[0], sizeof(int), g_Ranking.nNumRanking, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
	else
	{//�t�@�C�����J���Ȃ������ꍇ

		g_Ranking.nNumRanking = MIN_RANKING;
		for (int nCount = 0; nCount < g_Ranking.nNumRanking; nCount++)
		{//�S���[��

			g_Ranking.nScore[nCount] = 200 - (nCount * 10);
		}

		SaveDataRanking();

	}
}

//==============================================================
//�v�Z����
//==============================================================
void CalRanking(void)
{

	//�����L���O�̐����Z
	g_Ranking.nNumRanking++;

	//�~������
	for (int nCntData = 0; nCntData < g_Ranking.nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < g_Ranking.nNumRanking; nCntSort++)
		{
			if (g_Ranking.nScore[nCntData] < g_Ranking.nScore[nCntSort])
			{//�v�f1���v�f2���傫��������

				int nTemp = g_Ranking.nScore[nCntData];
				g_Ranking.nScore[nCntData] = g_Ranking.nScore[nCntSort];
				g_Ranking.nScore[nCntSort] = nTemp;

			}
		}
	}

	//����̐����擾
	int NowData = 0;

	//�����L���O�ő�l��������̂�������������
	if (g_Ranking.nScore[g_Ranking.nNumRanking - 1] < NowData)
	{//����ւ�

		g_Ranking.nScore[g_Ranking.nNumRanking - 1] = NowData;	//�ŏ��l�ƍ���̃f�[�^����ւ�
	}

	if (g_Ranking.nScore[MIN_RANKING - 1] < NowData)
	{//����ւ�

		g_Ranking.nNewRecordNum = MIN_RANKING - 1;			//�j���[���R�[�h�̗v�f�ԍ�
		g_Ranking.bNewRecord = true;	//�t���OON
	}

	//�~������
	for (int nCntData = 0; nCntData < g_Ranking.nNumRanking - 1; nCntData++)
	{
		for (int nCntSort = nCntData + 1; nCntSort < g_Ranking.nNumRanking; nCntSort++)
		{
			if (g_Ranking.nScore[nCntData] < g_Ranking.nScore[nCntSort])
			{//�v�f1���v�f2���傫��������

				int nTemp = g_Ranking.nScore[nCntData];
				g_Ranking.nScore[nCntData] = g_Ranking.nScore[nCntSort];
				g_Ranking.nScore[nCntSort] = nTemp;
			}
		}
	}

	if (g_Ranking.bNewRecord == true)
	{//�j���[���R�[�h�̎�

		for (int nCount = MIN_RANKING; nCount > 0; nCount--)
		{//�v�f�ԍ��L�����Ĕ�����

			if (g_Ranking.nScore[nCount - 1] == g_Ranking.nThisScore)
			{
				g_Ranking.nNewRecordNum = nCount - 1;
				break;
			}
		}
	}

}

//==============================================================
//�Z�[�u����
//==============================================================
void SaveDataRanking(void)
{
	FILE *pFile;

	//�t�@�C�����J��
	pFile = fopen(FILE_RANK, "wb");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		//�t�@�C���ɐ��l�������o��
		fwrite(&g_Ranking.nNumRanking, sizeof(int), 1, pFile);
		fwrite(&g_Ranking.nScore[0], sizeof(int), g_Ranking.nNumRanking, pFile);

		//�t�@�C�������
		fclose(pFile);
	}
}

//==============================================================
//�����L���O�̒��_�o�b�t�@����
//==============================================================
void CreatRankingBuff(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_05.png",
		&g_pTextureRanking);


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE * MIN_RANKING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRanking,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRanking->Lock(0, 0, (void**)&pVtx, 0);

	int aTexU[NUM_PLACE];

	for (int nCntRank = 0; nCntRank < MIN_RANKING; nCntRank++)
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

			aTexU[nCntTex] = g_Ranking.aEachRealRanking[nCntRank].nScore % nDigit / (nDigit / 10);
			nDigit /= 10;
		}

		for (int nCntScore = 0; nCntScore < NUM_PLACE; nCntScore++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x - g_Ranking.aEachRealRanking[nCntRank].fWidth, g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y - g_Ranking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x + g_Ranking.aEachRealRanking[nCntRank].fWidth, g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y - g_Ranking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x - g_Ranking.aEachRealRanking[nCntRank].fWidth, g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y + g_Ranking.aEachRealRanking[nCntRank].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.x + g_Ranking.aEachRealRanking[nCntRank].fWidth, g_Ranking.aEachRealRanking[nCntRank].aScoreNum[nCntScore].pos.y + g_Ranking.aEachRealRanking[nCntRank].fHeight, 0.0f);

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
	g_pVtxBuffRanking->Unlock();
}

//==============================================================
//�����L���O(�e�L�X�g)�̒��_�o�b�t�@����
//==============================================================
void CreatRankingText(void)
{
	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRankingText) / sizeof(*c_apFilenameRankingText); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameRankingText[nCntTex],
			&g_pTextureRankingText[nCntTex]);
	}

	//�e��ϐ�������
	ZeroMemory(&g_aRankingText, sizeof(RankingText));
	ZeroMemory(&g_aRankingText.aUI, sizeof(UI_2D));
	g_aRankingText.aUI.pos = D3DXVECTOR3(START_TEXT_X, 360.0f, 0.0f);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingText,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingText->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aRankingText.aUI.pos.x - TEXT_WIDTH, g_aRankingText.aUI.pos.y - TEXT_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aRankingText.aUI.pos.x + TEXT_WIDTH, g_aRankingText.aUI.pos.y - TEXT_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aRankingText.aUI.pos.x - TEXT_WIDTH, g_aRankingText.aUI.pos.y + TEXT_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aRankingText.aUI.pos.x + TEXT_WIDTH, g_aRankingText.aUI.pos.y + TEXT_HEIGHT, 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingText->Unlock();
}

//==============================================================
//�����L���O�̏��擾
//==============================================================
Ranking *GetRanking(void)
{
	return &g_Ranking;
}