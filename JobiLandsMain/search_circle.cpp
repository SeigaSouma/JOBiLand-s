//=============================================================================
//
// �T���͈͏��� [search_circle.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "search_circle.h"
#include "input.h"
#include "calculation.h"
#include "crowd.h"

//�}�N����`
#define POS_SEARCH_CIRCLE_Y	(0.0f)
#define WIDTH				(32)	//������
#define HEIGHT				(1)		//��������

//�v���g�^�C�v�錾
void CollisionSearchCircle(void);

//�O���[�o���ϐ��錾
const char *c_apFilenameSearchCircle[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\gradation001.jpg",
	"data\\TEXTURE\\gradation000.jpg",
	"data\\TEXTURE\\gradation003.jpg",
	"data\\TEXTURE\\gradation004.jpg",
	"data\\TEXTURE\\gradation005.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureSearchCircle[(sizeof c_apFilenameSearchCircle) / sizeof(*c_apFilenameSearchCircle)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSearchCircle = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffSearchCircle = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
SEARCH_CIRCLE g_aSearchCircle[MAX_SEARCH_CIRCLE];		//�T���͈͂̏��
int g_nNumVertxSearchCircle;
int g_nNumIndexSearchCircle;

//==================================================================================
//�T���͈͂̏���������
//==================================================================================
void InitSearchCircle(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSearchCircle) / sizeof(*c_apFilenameSearchCircle); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameSearchCircle[nCntTex],
			&g_apTextureSearchCircle[nCntTex]);
	}
	
	//�e�v�f������
	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		g_aSearchCircle[nCntWave].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSearchCircle[nCntWave].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSearchCircle[nCntWave].nor[nCntWave] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�@���̌���
		g_aSearchCircle[nCntWave].col = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);
		g_aSearchCircle[nCntWave].colOrigin = g_aSearchCircle[nCntWave].col;

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aSearchCircle[nCntWave].mtxWorld);

		g_aSearchCircle[nCntWave].fOutWidth = 100.0f;
		g_aSearchCircle[nCntWave].fInWidth = 0.0f;
		g_aSearchCircle[nCntWave].fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);		//1���Ƃ̊p�x�����߂�
		g_aSearchCircle[nCntWave].fHeight = 0.0f;
		g_aSearchCircle[nCntWave].fMove = 0.0f;
		g_aSearchCircle[nCntWave].nLife = 0;		//����
		g_aSearchCircle[nCntWave].nMaxLife = 0;		//�ő����
		g_aSearchCircle[nCntWave].nTexType = 0;		//�e�N�X�`�����
		g_aSearchCircle[nCntWave].bUse = false;
	}
	g_nNumIndexSearchCircle = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//�C���f�b�N�X��
	g_nNumVertxSearchCircle = (HEIGHT + 1) * (WIDTH + 1);		//���_��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_nNumVertxSearchCircle * MAX_SEARCH_CIRCLE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSearchCircle,
		NULL);

	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSearchCircle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[MAX_SEARCH_CIRCLE];		//�v�Z�p�̍��W

	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//��
				pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * g_aSearchCircle[nCntWave].fOutWidth,
					POS_SEARCH_CIRCLE_Y,
					cosf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * g_aSearchCircle[nCntWave].fOutWidth
				);

				//��
				pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * g_aSearchCircle[nCntWave].fInWidth,
					POS_SEARCH_CIRCLE_Y,
					cosf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * g_aSearchCircle[nCntWave].fInWidth
				);

				//�e���_���猴�_������
				g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aSearchCircle[nCntWave].pos;
				g_aSearchCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aSearchCircle[nCntWave].pos;

				//�o�������̒l�𐳋K������
				D3DXVec3Normalize(&g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
				D3DXVec3Normalize(&g_aSearchCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))], &g_aSearchCircle[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

			}
		}
	}

	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		//���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//���_���W�̐ݒ�
				pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//���_�J���[�̐ݒ�
				pVtx[0].col = g_aSearchCircle[nCntWave].col;

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
	g_pVtxBuffSearchCircle->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (g_nNumIndexSearchCircle * MAX_SEARCH_CIRCLE),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffSearchCircle,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffSearchCircle->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0, nVtxPoint = 0, nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
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

		nVtxPoint += g_nNumVertxSearchCircle;	//���_��������
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffSearchCircle->Unlock();

}

//==================================================================================
//�T���͈͂̏I������
//==================================================================================
void UninitSearchCircle(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSearchCircle) / sizeof(*c_apFilenameSearchCircle); nCntTex++)
	{
		if (g_apTextureSearchCircle[nCntTex] != NULL)
		{
			g_apTextureSearchCircle[nCntTex]->Release();
			g_apTextureSearchCircle[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSearchCircle != NULL)
	{
		g_pVtxBuffSearchCircle->Release();
		g_pVtxBuffSearchCircle = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffSearchCircle != NULL)
	{
		g_pIdxBuffSearchCircle->Release();
		g_pIdxBuffSearchCircle = NULL;
	}

}

//==================================================================================
//�T���͈͂̍X�V����
//==================================================================================
void UpdateSearchCircle(void)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSearchCircle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		if (g_aSearchCircle[nCntWave].bUse == true)
		{//�g�p����Ă�����

			//����̈ʒu�܂Ői�߂�
			pVtx += g_nNumVertxSearchCircle * nCntWave;

			//�L���Ă���
			g_aSearchCircle[nCntWave].fOutWidth += g_aSearchCircle[nCntWave].fMove;

			//���_���̍X�V
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * (g_aSearchCircle[nCntWave].fOutWidth - nCntHeight * g_aSearchCircle[nCntWave].fInWidth),
						POS_SEARCH_CIRCLE_Y + nCntHeight * g_aSearchCircle[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * (g_aSearchCircle[nCntWave].fOutWidth - nCntHeight * g_aSearchCircle[nCntWave].fInWidth)
					);

					//���_�J���[�̐ݒ�
					pVtx[0].col = g_aSearchCircle[nCntWave].col;

					pVtx += 1;
				}
			}

			//�i�߂����߂�
			pVtx -= (g_nNumVertxSearchCircle * nCntWave + g_nNumVertxSearchCircle);

			//�����X�V
			g_aSearchCircle[nCntWave].nLife--;

			//�s�����x�X�V
			g_aSearchCircle[nCntWave].col.a = g_aSearchCircle[nCntWave].colOrigin.a * (float)g_aSearchCircle[nCntWave].nLife / (float)g_aSearchCircle[nCntWave].nMaxLife;

			if (g_aSearchCircle[nCntWave].nLife <= 0)
			{//�������s����

				g_aSearchCircle[nCntWave].bUse = false;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSearchCircle->Unlock();
}

//==================================================================================
//�e�̈ʒu�ݒ菈��
//==================================================================================
void SetPositionSearchCircle(int nIdxSearchRadius, D3DXVECTOR3 pos)
{
	if (g_aSearchCircle[nIdxSearchRadius].bUse == true)
	{
		g_aSearchCircle[nIdxSearchRadius].pos = D3DXVECTOR3(pos.x, 1.2f, pos.z);
	}
}

//==================================================================================
//�e�̈ʒu�ݒ菈��
//==================================================================================
void CollisionSearchCircle(void)
{

	////�Q�O�̏��擾
	//Crowd *pCrowd = GetCrowd();

	//for (int nCount = 0; nCount < MAX_CROWD; nCount++, pCrowd++)
	//{
	//	if (pCrowd->aModel.bUse == true)
	//	{//�Q�O���g�p����Ă�����

	//	 //�L�����N�^�[���m�̓����蔻��
	//	 /*CollisionCharacter(
	//	 &g_aCrowd[nCntCrowd].aModel.pos, &g_aCrowd[nCntCrowd].aModel.posOld, g_aCrowd[nCntCrowd].aModel.vtxMax, g_aCrowd[nCntCrowd].aModel.vtxMin,
	//	 &pCrowd->aModel.pos, pCrowd->aModel.vtxMax, pCrowd->aModel.vtxMin);*/
	//	}
	//}

	////�G�ƃv���C���[�̋���
	//float fLength = pCrowd->aModel.vtxMax.x + fRadius;

	//if (SphereRange(D3DXVECTOR3(), pCrowd->aModel.pos) <= (fLength * fLength))
	//{//���̒��ɓ�������


	//}
}

//==================================================================================
//�T���͈͂̕`�揈��
//==================================================================================
void DrawSearchCircle(void)
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

	for (int nCntWave = 0, nIdxPoint = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		if (g_aSearchCircle[nCntWave].bUse == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSearchCircle[nCntWave].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSearchCircle[nCntWave].rot.y, g_aSearchCircle[nCntWave].rot.x, g_aSearchCircle[nCntWave].rot.z);
			D3DXMatrixMultiply(&g_aSearchCircle[nCntWave].mtxWorld, &g_aSearchCircle[nCntWave].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aSearchCircle[nCntWave].pos.x, g_aSearchCircle[nCntWave].pos.y, g_aSearchCircle[nCntWave].pos.z);
			D3DXMatrixMultiply(&g_aSearchCircle[nCntWave].mtxWorld, &g_aSearchCircle[nCntWave].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSearchCircle[nCntWave].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffSearchCircle, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffSearchCircle);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureSearchCircle[g_aSearchCircle[nCntWave].nTexType]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_nNumVertxSearchCircle/* * nCntWave*/,
				nIdxPoint,
				(g_nNumIndexSearchCircle - 2));

		}

		//����̃C���f�b�N�X�������Z
		nIdxPoint += g_nNumIndexSearchCircle;
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
//�T���͈͂̐ݒ菈��
//==================================================================================
int SetSearchCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fMove, int nTexType)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSearchCircle->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posMath[MAX_SEARCH_CIRCLE];		//�v�Z�p�̍��W
	int nCntShadow = -1;

	for (int nCntWave = 0; nCntWave < MAX_SEARCH_CIRCLE; nCntWave++)
	{
		if (g_aSearchCircle[nCntWave].bUse == false)
		{//�g�p���Ă��Ȃ�������

			nCntShadow = nCntWave;			//�C���f�b�N�X�ԍ��擾

			//����̈ʒu�܂Ői�߂�
			pVtx += g_nNumVertxSearchCircle * nCntWave;

			//�e�v�f�ݒ�
			g_aSearchCircle[nCntWave].pos = pos;
			g_aSearchCircle[nCntWave].pos.y = 1.2f;
			g_aSearchCircle[nCntWave].rot = rot;
			g_aSearchCircle[nCntWave].col = col;
			g_aSearchCircle[nCntWave].colOrigin = col;
			g_aSearchCircle[nCntWave].nLife = nLife;
			g_aSearchCircle[nCntWave].nMaxLife = nLife;
			g_aSearchCircle[nCntWave].fOutWidth = fWidth;
			g_aSearchCircle[nCntWave].fInWidth = g_aSearchCircle[nCntWave].fOutWidth;
			g_aSearchCircle[nCntWave].fHeight = fHeight;
			g_aSearchCircle[nCntWave].fMove = fMove;
			g_aSearchCircle[nCntWave].nTexType = nTexType;
			g_aSearchCircle[nCntWave].bUse = true;

			//���_���̐ݒ�
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

				 //posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				 //(
				 //	sinf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * (g_aSearchCircle[nCntWave].fOutWidth - nCntHeight * g_aSearchCircle[nCntWave].fInWidth),
				 //	POS_IMPACTWAVE_Y + nCntHeight * 1000.0f,
				 //	cosf(nCntWidth % WIDTH * g_aSearchCircle[nCntWave].fRotWidth) * (g_aSearchCircle[nCntWave].fOutWidth - nCntHeight * g_aSearchCircle[nCntWave].fInWidth)
				 //);

				 ////���_���W�̐ݒ�
				 //pVtx[0].pos = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))];

					//�e���_���猴�_������
					g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aSearchCircle[nCntWave].pos;

					//�o�������̒l�𐳋K������
					D3DXVec3Normalize(&g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);

					//�@���x�N�g���̐ݒ�
					pVtx[0].nor = g_aSearchCircle[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

					pVtx += 1;
				}
			}

			int nCntIdx = g_nNumIndexSearchCircle * nCntWave;
			int nVtxPoint = g_nNumVertxSearchCircle * nCntWave;	//���_��������

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSearchCircle->Unlock();

	return nCntShadow;

}