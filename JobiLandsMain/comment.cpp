//=============================================================================
//
// �R�����g���� [comment.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "ranking.h"
#include "comment.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
#include "ranking_comment.h"

//�}�N����`
#define MAX_TOP			(4)		//���_��
#define TEX_V		(0.1f)		//V���W
#define SPAWNCOUNTER		(40)
#define MOVE		(5.0f)
#define WIDTH		(400.0f)
#define HEIGHT		(30.0f)

//�v���g�^�C�v�錾
void UpdateManySpawnComment(void);
void UpdateRankingSpawnComment(void);

//�O���[�o���ϐ��錾
const char *c_apFilenameCommnet[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\comment_01.png",
	"data\\TEXTURE\\comment_02.png",
	"data\\TEXTURE\\comment_03.png",
	"data\\TEXTURE\\comment_04.png",
	"data\\TEXTURE\\comment_05.png",
	"data\\TEXTURE\\comment_06.png",
	"data\\TEXTURE\\comment_07.png",
	"data\\TEXTURE\\comment_08.png",
	"data\\TEXTURE\\comment_09.png",
	"data\\TEXTURE\\comment_10.png",
};
LPDIRECT3DTEXTURE9 g_pTextureCommnet[(sizeof c_apFilenameCommnet) / sizeof(*c_apFilenameCommnet)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCommnet = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Commnet g_aCommnet;						//�R�����g�̏��

//==================================================================================
//�R�����g�̏���������
//==================================================================================
void InitCommnet(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCommnet) / sizeof(*c_apFilenameCommnet); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCommnet[nCntTex],
			&g_pTextureCommnet[nCntTex]);
	}

	//�R�����g�̏��̏�����
	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		g_aCommnet.aInfo[nCntCommnet].pos = D3DXVECTOR3(640.0f, 500.0f, 0.0f);
		g_aCommnet.aInfo[nCntCommnet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
		g_aCommnet.aInfo[nCntCommnet].nType = 0;	//���
		g_aCommnet.aInfo[nCntCommnet].nTexV = 0;	//�e�N�X�`���̉������ڂ�
		g_aCommnet.aInfo[nCntCommnet].bUse = false;	//�g�p���Ă��邩
	}

	//�R�����g�̏�����
	g_aCommnet.nCntSpawn = 0;
	g_aCommnet.nNumSpawn = 0;
	g_aCommnet.bUse = false;    //�g�p���Ă��邩

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * MAX_NICOCOME,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffCommnet,
								NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCommnet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCommnet->Unlock();
}

//==================================================================================
//�R�����g�̏I������
//==================================================================================
void UninitCommnet(void)
{

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCommnet) / sizeof(*c_apFilenameCommnet); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureCommnet[nCntTex] != NULL)
		{
			g_pTextureCommnet[nCntTex]->Release();
			g_pTextureCommnet[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCommnet != NULL)
	{
		g_pVtxBuffCommnet->Release();
		g_pVtxBuffCommnet = NULL;
	}
}

//==================================================================================
//�R�����g�̍X�V����
//==================================================================================
void UpdateCommnet(void)
{

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCommnet->Lock(0, 0, (void**)&pVtx, 0);

	if (GetMode() == MODE_GAME)
	{
		//�o���܂ł̍X�V����
		UpdateManySpawnComment();
	}
	else
	{
		UpdateRankingSpawnComment();
	}

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aCommnet.aInfo[nCntCommnet].bUse == true)
		{//�g�p����Ă�����

			//�ړ�
			g_aCommnet.aInfo[nCntCommnet].pos.x -= g_aCommnet.aInfo[nCntCommnet].move.x;

			if (g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH <= 0.0f)
			{//��ʊO�ɏo����

				//�g�p���Ă��Ȃ���Ԃɂ���
				g_aCommnet.aInfo[nCntCommnet].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
		}

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCommnet->Unlock();
}

//==================================================================================
//��ʔ����̃R�����g�X�V����
//==================================================================================
void UpdateManySpawnComment(void)
{
	if (g_aCommnet.bUse == true)
	{//�g�p����Ă�����

		//��ʔ����̃R�����g�J�E���g���Z
		g_aCommnet.nCntSpawn++;

		//�o������܂ł̃J�E���^�[�ݒ�
		int nSpawnCounter = SPAWNCOUNTER - (g_aCommnet.nNumSpawn * 10);

		if (nSpawnCounter <= 4)
		{
			nSpawnCounter = 4;
		}

		if (g_aCommnet.nCntSpawn >= nSpawnCounter)
		{//�o���܂ł̃J�E���g�ɒB������

			//��ʔ����̃R�����g�J�E���g���Z�b�g
			g_aCommnet.nCntSpawn = 0;

			//�񐔉��Z
			g_aCommnet.nNumSpawn++;

			//�R�����g�̐ݒ�
			SetComment();
		}
	}
}

//==================================================================================
//�����L���O�̃R�����g�X�V����
//==================================================================================
void UpdateRankingSpawnComment(void)
{

	//�����L���O�̏��擾
	Ranking *pRanking = GetRanking();

	if (g_aCommnet.bUse == true)
	{//�g�p����Ă�����

		//�o������܂ł̃J�E���^�[�ݒ�
		int nSpawnCounter = SPAWNCOUNTER;

		if (pRanking->nOldMode == MODE_TITLE)
		{//�����L���O�̑J�ڑO�����U���g�̎�

			nSpawnCounter = 80;
		}

		//��ʔ����̃R�����g�J�E���g���Z
		g_aCommnet.nCntSpawn++;

		if (g_aCommnet.nCntSpawn >= nSpawnCounter)
		{//�o���܂ł̃J�E���g�ɒB������

			//��ʔ����̃R�����g�J�E���g���Z�b�g
			g_aCommnet.nCntSpawn = 0;

			//�񐔉��Z
			g_aCommnet.nNumSpawn++;

			//�R�����g�̐ݒ�
			SetComment();
		}
	}
}

//==================================================================================
//�R�����g�̕`�揈��
//==================================================================================
void DrawCommnet(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCommnet, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�L�X�g�̕`��
	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aCommnet.aInfo[nCntCommnet].bUse == true)
		{//�g�p����Ă�����

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureCommnet[g_aCommnet.aInfo[nCntCommnet].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCommnet * 4, 2);
		}
	}

}

//==================================================================================
//�R�����g���擾
//==================================================================================
void SetComment(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//�����L���O�̏��擾
	Ranking *pRanking = GetRanking();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCommnet->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aCommnet.bUse == false)
	{//�g�p����Ă��Ȃ�������

		//��ʔ����̃R�����g���g�p��Ԃɂ���
		g_aCommnet.bUse = true;
	}

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aCommnet.aInfo[nCntCommnet].bUse == false)
		{//�g�p���Ă��Ȃ�������

			//�o���������_��
			g_aCommnet.aInfo[nCntCommnet].pos.x = SCREEN_WIDTH + WIDTH;
			g_aCommnet.aInfo[nCntCommnet].pos.x += (rand() % 6) * 10.0f;

			if (GetMode() == MODE_RANKING)
			{
				g_aCommnet.aInfo[nCntCommnet].pos.x -= (SCREEN_WIDTH * 0.3f);
			}

			//�o�����������_��
			g_aCommnet.aInfo[nCntCommnet].pos.y = SCREEN_HEIGHT * 0.5f;
			g_aCommnet.aInfo[nCntCommnet].pos.y += (rand() % 7 - 3) * 80.0f;
			g_aCommnet.aInfo[nCntCommnet].pos.y += (rand() % 7 - 3) * 20.0f;

			//�ړ��ʃ����_��
			g_aCommnet.aInfo[nCntCommnet].move.x = (float)(rand() % 80) * 0.1f + MOVE;

			//��ރ����_��
			if (GetMode() != MODE_RANKING)
			{//�����L���O�ȊO

				g_aCommnet.aInfo[nCntCommnet].nType = rand() % 4;
			}
			else
			{//�����L���O�̂Ƃ�

				if (pRanking->nOldMode == MODE_TITLE)
				{//�^�C�g�����烉���L���O�������Ƃ�

					g_aCommnet.aInfo[nCntCommnet].nType = rand() % 2 + 8;
				}
				else
				{//���U���g���烉���L���O�ɂ������Ƃ�

					g_aCommnet.aInfo[nCntCommnet].nType = rand() % 2 + 4;

					if (pRanking->bNewRecord == true && rand() % 2 == 0)
					{//�j���[���R�[�h����������

						g_aCommnet.aInfo[nCntCommnet].nType = 6;
					}
					else if (pRanking->bNewRecord == false && rand() % 2 == 0)
					{//�����L���O�O

						g_aCommnet.aInfo[nCntCommnet].nType = 7;
					}
				}
			}

			//�e�N�X�`���̉������ڂ�
			g_aCommnet.aInfo[nCntCommnet].nTexV = rand() % 10;

			//�g�p���Ă����Ԃɂ���
			g_aCommnet.aInfo[nCntCommnet].bUse = true;

			if (GetMode() == MODE_RANKING)
			{
				SetRankingComment(g_aCommnet.aInfo[nCntCommnet].nTexV, g_aCommnet.aInfo[nCntCommnet].nType);
			}


			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x - WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aCommnet.aInfo[nCntCommnet].pos.x + WIDTH, g_aCommnet.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V);
			pVtx[1].tex = D3DXVECTOR2(1.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
			pVtx[3].tex = D3DXVECTOR2(1.0f, g_aCommnet.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCommnet->Unlock();
}

//==================================================================================
//�R�����g���擾
//==================================================================================
Commnet *GetCommnet(void)
{
	return &g_aCommnet;
}