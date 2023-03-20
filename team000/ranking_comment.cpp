//=============================================================================
//
// �����L���O�R�����g���� [ranking_comment.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "ranking_comment.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
#include "updown_fram.h"
#include "enemy.h"

//�}�N����`
#define MAX_TOP			(4)		//���_��
#define TEX_V		(0.1f)		//V���W
#define SPAWNCOUNTER		(40)
#define MOVE		(5.0f)
#define WIDTH		(400.0f * 0.45f)
#define HEIGHT		(30.0f * 0.45f)
#define WINDOW_WIDTH		(640.0f)
#define WINDOW_HEIGHT		(360.0f)
#define DIS_Y		(50.0f)

//�v���g�^�C�v�錾
void CreatVtxTextRankingComment(void);
void CreatVtxWindowRankingComment(void);
void DrawRankingCommentBuff(void);
void DrawCommentWindowBuff(void);
void UpdateTextRankingComment(void);

//�O���[�o���ϐ��錾
const char *c_apFilenameRankingComment[] =		//�t�@�C���ǂݍ���
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

const char *c_apFilenameCommentWindow[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\commentwindow_01.png",
};
LPDIRECT3DTEXTURE9 g_pTextureRankingComment[(sizeof c_apFilenameRankingComment) / sizeof(*c_apFilenameRankingComment)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureCommentWindow[(sizeof c_apFilenameCommentWindow) / sizeof(*c_apFilenameCommentWindow)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRankingComment = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCommentWindow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
RankingComment g_aRankingComment;		//�����L���O�R�����g�̏��

//==================================================================================
//�����L���O�R�����g�̏���������
//==================================================================================
void InitRankingComment(void)
{

	//�e�ϐ�������
	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		g_aRankingComment.aInfo[nCntCommnet].bUse = false;
		g_aRankingComment.aInfo[nCntCommnet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aRankingComment.aInfo[nCntCommnet].pos = D3DXVECTOR3(1080.0f, 0.0f, 0.0f);
		g_aRankingComment.aInfo[nCntCommnet].nTexV = 0;	//���
		g_aRankingComment.aInfo[nCntCommnet].nType = 0;	//�e�N�X�`���̉������ڂ�
		g_aRankingComment.aInfo[nCntCommnet].nNumIndex = 0;	//�C���f�b�N�X�ԍ�
	}
	g_aRankingComment.nNumSpawn = 0;	//�o��������

	//��b�̒��_�o�b�t�@����
	CreatVtxTextRankingComment();

	//�E�B���h�E�̒��_�o�b�t�@����
	CreatVtxWindowRankingComment();
}

//==================================================================================
//�����L���O�R�����g�̏I������
//==================================================================================
void UninitRankingComment(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRankingComment) / sizeof(*c_apFilenameRankingComment); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureRankingComment[nCntTex] != NULL)
		{
			g_pTextureRankingComment[nCntTex]->Release();
			g_pTextureRankingComment[nCntTex] = NULL;
		}
	}

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCommentWindow) / sizeof(*c_apFilenameCommentWindow); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureCommentWindow[nCntTex] != NULL)
		{
			g_pTextureCommentWindow[nCntTex]->Release();
			g_pTextureCommentWindow[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRankingComment != NULL)
	{
		g_pVtxBuffRankingComment->Release();
		g_pVtxBuffRankingComment = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCommentWindow != NULL)
	{
		g_pVtxBuffCommentWindow->Release();
		g_pVtxBuffCommentWindow = NULL;
	}
}

//==================================================================================
//�����L���O�R�����g�̍X�V����
//==================================================================================
void UpdateRankingComment(void)
{
	UpdateTextRankingComment();
}

//==================================================================================
//�����L���O�R�����g�̃e�L�X�g�X�V����
//==================================================================================
void UpdateTextRankingComment(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingComment->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aRankingComment.aInfo[nCntCommnet].bUse == true)
		{//�g�p����Ă�����

			//��Ɉړ�������
			g_aRankingComment.aInfo[nCntCommnet].pos.y -= 10.0f;

			//����鏈��
			if ((700.0f + (g_aRankingComment.aInfo[nCntCommnet].nNumIndex) * DIS_Y) - DIS_Y * g_aRankingComment.nNumSpawn > g_aRankingComment.aInfo[nCntCommnet].pos.y)
			{
				g_aRankingComment.aInfo[nCntCommnet].pos.y = (700.0f + (g_aRankingComment.aInfo[nCntCommnet].nNumIndex) * DIS_Y) - DIS_Y * g_aRankingComment.nNumSpawn;
			}

			if (g_aRankingComment.aInfo[nCntCommnet].pos.y <= -150.0f)
			{
				g_aRankingComment.aInfo[nCntCommnet].bUse = false;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingComment->Unlock();
}

//==================================================================================
//�����L���O�R�����g�̕`�揈��
//==================================================================================
void DrawRankingComment(void)
{

	//�E�B���h�E�`�揈��
	DrawCommentWindowBuff();

	//�e�L�X�g�`�揈��
	DrawRankingCommentBuff();

}

//==================================================================================
//�����L���O�R�����g�̃e�L�X�g�`�揈��
//==================================================================================
void DrawRankingCommentBuff(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffRankingComment, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�L�X�g�̕`��
	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aRankingComment.aInfo[nCntCommnet].bUse == true)
		{//�g�p����Ă�����

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureRankingComment[g_aRankingComment.aInfo[nCntCommnet].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntCommnet * 4, 2);
		}
	}


	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

}

//==================================================================================
//�����L���O�R�����g�̃E�B���h�E�`�揈��
//==================================================================================
void DrawCommentWindowBuff(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 250);

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffCommentWindow, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureCommentWindow[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

}

//==================================================================================
//�R�����g�ݒ菈��
//==================================================================================
void SetRankingComment(int nTexV, int nType)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingComment->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		if (g_aRankingComment.aInfo[nCntCommnet].bUse == false)
		{//�g�p���Ă��Ȃ�������

			//�o�����������_��
			g_aRankingComment.aInfo[nCntCommnet].pos.y = 700.0f;

			g_aRankingComment.aInfo[nCntCommnet].nTexV = nTexV;
			g_aRankingComment.aInfo[nCntCommnet].nType = nType;

			//�g�p���Ă����Ԃɂ���
			g_aRankingComment.aInfo[nCntCommnet].bUse = true;

			//�C���f�b�N�X�ԍ��擾
			g_aRankingComment.aInfo[nCntCommnet].nNumIndex = g_aRankingComment.nNumSpawn;

			//�o�������񐔉��Z
			g_aRankingComment.nNumSpawn++;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V);
			pVtx[1].tex = D3DXVECTOR2(1.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
			pVtx[3].tex = D3DXVECTOR2(1.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingComment->Unlock();
}

//==================================================================================
//�e�L�X�g�̒��_�o�b�t�@����
//==================================================================================
void CreatVtxTextRankingComment(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameRankingComment) / sizeof(*c_apFilenameRankingComment); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameRankingComment[nCntTex],
			&g_pTextureRankingComment[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * MAX_NICOCOME,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRankingComment,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRankingComment->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntCommnet = 0; nCntCommnet < MAX_NICOCOME; nCntCommnet++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y - HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x - WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aRankingComment.aInfo[nCntCommnet].pos.x + WIDTH, g_aRankingComment.aInfo[nCntCommnet].pos.y + HEIGHT, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aRankingComment.aInfo[nCntCommnet].nTexV * TEX_V + TEX_V);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRankingComment->Unlock();
}

//==================================================================================
//�E�B���h�E�̒��_�o�b�t�@����
//==================================================================================
void CreatVtxWindowRankingComment(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCommentWindow) / sizeof(*c_apFilenameCommentWindow); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCommentWindow[nCntTex],
			&g_pTextureCommentWindow[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCommentWindow,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCommentWindow->Lock(0, 0, (void**)&pVtx, 0);
	
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(640.0f - WINDOW_WIDTH, 360.0f - WINDOW_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(640.0f + WINDOW_WIDTH, 360.0f - WINDOW_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(640.0f - WINDOW_WIDTH, 360.0f + WINDOW_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(640.0f + WINDOW_WIDTH, 360.0f + WINDOW_HEIGHT, 0.0f);

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
	g_pVtxBuffCommentWindow->Unlock();
}

//==================================================================================
//�����L���O�R�����g���擾
//==================================================================================
RankingComment *GetRankingComment(void)
{
	return &g_aRankingComment;
}