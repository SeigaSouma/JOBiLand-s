//=============================================================================
//
// �Ռ��g���� [scorecircle.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "scorecircle.h"
#include "impactwave.h"
#include "player.h"
#include "input.h"

//�}�N����`
#define POS_SCORECIRCLE_Y	(0.0f)
#define WIDTH				(32)	//������
#define HEIGHT				(1)		//��������

//�O���[�o���ϐ��錾
const char *c_apFilenameScoreCircle[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\gradation001.jpg",
	"data\\TEXTURE\\gradation000.jpg",
	"data\\TEXTURE\\gradation003.jpg",
	"data\\TEXTURE\\gradation004.jpg",
	"data\\TEXTURE\\gradation005.jpg",
	"data\\TEXTURE\\gradation006.jpg",
	"data\\TEXTURE\\gradation007.jpg",
	"data\\TEXTURE\\gradation008.jpg",
	"data\\TEXTURE\\gradation009.jpg",
	"data\\TEXTURE\\gradation011.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureScoreCircle[(sizeof c_apFilenameScoreCircle) / sizeof(*c_apFilenameScoreCircle)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScoreCircle = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffScoreCircle = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
SCORECIRCLE g_aScoreCircle[MAX_SCORECIRCLE];		//�Ռ��g�̏��
int g_nNumVertxScoreCircle;
int g_nNumIndexScoreCircle;

//==================================================================================
//�Ռ��g�̏���������
//==================================================================================
void InitScoreCircle(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameScoreCircle) / sizeof(*c_apFilenameScoreCircle); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameScoreCircle[nCntTex],
			&g_apTextureScoreCircle[nCntTex]);
	}

	//�e�v�f������
	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		g_aScoreCircle[nCntWave].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�N�_�̈ʒu
		g_aScoreCircle[nCntWave].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aScoreCircle[nCntWave].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ�
		g_aScoreCircle[nCntWave].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

		for (int nCntNor = 0; nCntNor < 256; nCntNor++)
		{
			g_aScoreCircle[nCntWave].nor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�@���̌���
		}
		g_aScoreCircle[nCntWave].col = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);			//�F
		g_aScoreCircle[nCntWave].colOrigin = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);	//���̐F

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aScoreCircle[nCntWave].mtxWorld);
		g_aScoreCircle[nCntWave].fOutWidth = 100.0f;		//����(�O)
		g_aScoreCircle[nCntWave].fInWidth = 0.0f;			//��(��)
		g_aScoreCircle[nCntWave].fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);		//1������������̊p�x����
		g_aScoreCircle[nCntWave].fHeight = 0.0f;
		g_aScoreCircle[nCntWave].fMove = 0.0f;			//�L���鑬�x
		g_aScoreCircle[nCntWave].nLife = 0;				//����
		g_aScoreCircle[nCntWave].nMaxLife = 0;			//�ő����
		g_aScoreCircle[nCntWave].nTexType = 0;			//�e�N�X�`�����
		g_aScoreCircle[nCntWave].bUse = false;				//�g�p���Ă��邩
	}
	g_nNumIndexScoreCircle = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//�C���f�b�N�X��
	g_nNumVertxScoreCircle = (HEIGHT + 1) * (WIDTH + 1);	//���_��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_nNumVertxScoreCircle * MAX_SCORECIRCLE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScoreCircle,
		NULL);

	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreCircle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[512];		//�v�Z�p�̍��W

	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//��
				pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * g_aScoreCircle[nCntWave].fOutWidth,
					POS_SCORECIRCLE_Y,
					cosf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * g_aScoreCircle[nCntWave].fOutWidth
				);

				//��
				pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * g_aScoreCircle[nCntWave].fInWidth,
					POS_SCORECIRCLE_Y,
					cosf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * g_aScoreCircle[nCntWave].fInWidth
				);

				//�e���_���猴�_������
				g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aScoreCircle[nCntWave].pos;
				g_aScoreCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aScoreCircle[nCntWave].pos;

				//�o�������̒l�𐳋K������
				D3DXVec3Normalize(&g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
				D3DXVec3Normalize(&g_aScoreCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))], &g_aScoreCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

			}
		}
	}

	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		//���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//���_���W�̐ݒ�
				pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//���_�J���[�̐ݒ�
				pVtx[0].col = g_aScoreCircle[nCntWave].col;

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				(
					nCntWidth * (1.0f / (float)(WIDTH / WIDTH)),
					nCntHeight * (1.0f / (float)(HEIGHT))
				);

				pVtx += 1;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScoreCircle->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (g_nNumIndexScoreCircle * MAX_SCORECIRCLE),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffScoreCircle,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffScoreCircle->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0, nVtxPoint = 0, nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		//���_�ԍ��f�[�^�̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//�����̕��������J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				pIdx[nCntIdx + 0] = (nCntWidth + (WIDTH + 1) * (nCntHeight + 1)) + nVtxPoint;
				pIdx[nCntIdx + 1] = (nCntWidth + ((WIDTH + 1) * nCntHeight)) + nVtxPoint;

				nCntIdx += 2;	//2�����Ă邩��
			}

			if (nCntHeight + 1 < HEIGHT)
			{//�Ō�̂����͑ł��Ȃ�

				pIdx[nCntIdx + 0] = (((WIDTH + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
				pIdx[nCntIdx + 1] = ((WIDTH + 1) * (nCntHeight + 2)) + nVtxPoint;

				nCntIdx += 2;	//2�����Ă邩��
			}
		}

		nVtxPoint += g_nNumVertxScoreCircle;	//���_��������
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffScoreCircle->Unlock();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�X�R�A�̉~�ݒ�
	SetScoreCircle(D3DXVECTOR3(-200.0f, pPlayer->pos.y + 70.0f, pPlayer->pos.z - 2.02f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * 0.5f), D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 100.0f, 0.0f, 0);
	SetScoreCircle(D3DXVECTOR3(-200.0f, pPlayer->pos.y + 70.0f, pPlayer->pos.z - 1.01f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * 0.5f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f), 200.0f, 0.0f, 0);
	SetScoreCircle(D3DXVECTOR3(-200.0f, pPlayer->pos.y + 70.0f, pPlayer->pos.z), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * 0.5f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f), 300.0f, 0.0f, 0);
}

//==================================================================================
//�Ռ��g�̏I������
//==================================================================================
void UninitScoreCircle(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameScoreCircle) / sizeof(*c_apFilenameScoreCircle); nCntTex++)
	{
		if (g_apTextureScoreCircle[nCntTex] != NULL)
		{
			g_apTextureScoreCircle[nCntTex]->Release();
			g_apTextureScoreCircle[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffScoreCircle != NULL)
	{
		g_pVtxBuffScoreCircle->Release();
		g_pVtxBuffScoreCircle = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffScoreCircle != NULL)
	{
		g_pIdxBuffScoreCircle->Release();
		g_pIdxBuffScoreCircle = NULL;
	}

}

//==================================================================================
//�Ռ��g�̍X�V����
//==================================================================================
void UpdateScoreCircle(void)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	int nCheck = 0;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreCircle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		if (g_aScoreCircle[nCntWave].bUse == true)
		{//�g�p����Ă�����

			//����̈ʒu�܂Ői�߂�
			pVtx += g_nNumVertxScoreCircle * nCntWave;
			nCheck += g_nNumVertxScoreCircle * nCntWave;

			g_aScoreCircle[nCntWave].pos += g_aScoreCircle[nCntWave].move;

			//�L���Ă���
			g_aScoreCircle[nCntWave].fOutWidth += g_aScoreCircle[nCntWave].fMove;

			//���_���̍X�V
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * (g_aScoreCircle[nCntWave].fOutWidth - nCntHeight * g_aScoreCircle[nCntWave].fInWidth),
						POS_SCORECIRCLE_Y + nCntHeight * g_aScoreCircle[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * (g_aScoreCircle[nCntWave].fOutWidth - nCntHeight * g_aScoreCircle[nCntWave].fInWidth)
					);

					//���_�J���[�̐ݒ�
					pVtx[0].col = g_aScoreCircle[nCntWave].col;

					pVtx += 1;
					nCheck += 1;
				}
			}

			//�i�߂����߂�
			pVtx -= (g_nNumVertxScoreCircle * nCntWave + g_nNumVertxScoreCircle);
			nCheck -= (g_nNumVertxScoreCircle * nCntWave + g_nNumVertxScoreCircle);

			//�����X�V
			//g_aScoreCircle[nCntWave].nLife--;

			//�s�����x�X�V
			//g_aScoreCircle[nCntWave].col.a = g_aScoreCircle[nCntWave].colOrigin.a * (float)g_aScoreCircle[nCntWave].nLife / (float)g_aScoreCircle[nCntWave].nMaxLife;

			//if (g_aScoreCircle[nCntWave].nLife <= 0)
			//{//�������s����

			//	g_aScoreCircle[nCntWave].bUse = false;
			//}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScoreCircle->Unlock();
}

//==================================================================================
//�Ռ��g�̈ʒu�ݒ菈��
//==================================================================================
void SetPositionScoreCircle(int nIdxWave, D3DXVECTOR3 move)
{
	/*if (g_aScoreCircle[nIdxWave].bUse == true)
	{
		g_aScoreCircle[nIdxWave].pos.x += move.x;
		g_aScoreCircle[nIdxWave].pos.z += move.z;
	}*/
}

//==================================================================================
//�Ռ��g�̕`�揈��
//==================================================================================
void DrawScoreCircle(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// �w�ʂ̃J�����O�͂��܂���B
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	for (int nCntWave = 0, nIdxPoint = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		if (g_aScoreCircle[nCntWave].bUse == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aScoreCircle[nCntWave].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aScoreCircle[nCntWave].rot.y, g_aScoreCircle[nCntWave].rot.x, g_aScoreCircle[nCntWave].rot.z);
			D3DXMatrixMultiply(&g_aScoreCircle[nCntWave].mtxWorld, &g_aScoreCircle[nCntWave].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aScoreCircle[nCntWave].pos.x, g_aScoreCircle[nCntWave].pos.y, g_aScoreCircle[nCntWave].pos.z);
			D3DXMatrixMultiply(&g_aScoreCircle[nCntWave].mtxWorld, &g_aScoreCircle[nCntWave].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aScoreCircle[nCntWave].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffScoreCircle, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffScoreCircle);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureScoreCircle[g_aScoreCircle[nCntWave].nTexType]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_nNumVertxScoreCircle/* * nCntWave*/,
				nIdxPoint,
				(g_nNumIndexScoreCircle - 2));

		}

		//����̃C���f�b�N�X�������Z
		nIdxPoint += g_nNumIndexScoreCircle;
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �O�p�`�̒��_�������v���Ȃ�Η������ƌ��Ȃ��`�悵�܂���B�i�f�t�H���g�j
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

//==================================================================================
//�Ռ��g�̐ݒ菈��
//==================================================================================
void SetScoreCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nTexType)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffScoreCircle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posMath[512];		//�v�Z�p�̍��W

	for (int nCntWave = 0; nCntWave < MAX_SCORECIRCLE; nCntWave++)
	{
		if (g_aScoreCircle[nCntWave].bUse == false)
		{//�g�p���Ă��Ȃ�������

			//����̈ʒu�܂Ői�߂�
			pVtx += g_nNumVertxScoreCircle * nCntWave;

			//�e�v�f�ݒ�
			g_aScoreCircle[nCntWave].pos = pos;
			g_aScoreCircle[nCntWave].rot = rot;
			g_aScoreCircle[nCntWave].col = col;
			g_aScoreCircle[nCntWave].colOrigin = col;
			g_aScoreCircle[nCntWave].fOutWidth = fWidth;
			g_aScoreCircle[nCntWave].fInWidth = g_aScoreCircle[nCntWave].fOutWidth;
			g_aScoreCircle[nCntWave].fHeight = fHeight;
			g_aScoreCircle[nCntWave].nTexType = nTexType;
			g_aScoreCircle[nCntWave].bUse = true;

			//���_���̐ݒ�
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * (g_aScoreCircle[nCntWave].fOutWidth - nCntHeight * g_aScoreCircle[nCntWave].fInWidth),
						POS_SCORECIRCLE_Y + nCntHeight * g_aScoreCircle[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aScoreCircle[nCntWave].fRotWidth) * (g_aScoreCircle[nCntWave].fOutWidth - nCntHeight * g_aScoreCircle[nCntWave].fInWidth)
					);

					//���_���W�̐ݒ�
					pVtx[0].pos = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))];

					//�e���_���猴�_������
					g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aScoreCircle[nCntWave].pos;

					//�o�������̒l�𐳋K������
					D3DXVec3Normalize(&g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);

					//�@���x�N�g���̐ݒ�
					pVtx[0].nor = g_aScoreCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

					pVtx += 1;
				}
			}

			int nCntIdx = g_nNumIndexScoreCircle * nCntWave;
			int nVtxPoint = g_nNumVertxScoreCircle * nCntWave;	//���_��������

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffScoreCircle->Unlock();
}