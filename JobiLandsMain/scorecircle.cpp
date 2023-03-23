//=============================================================================
//
// �Ռ��g���� [impactwave.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "impactwave.h"
#include "input.h"

//�}�N����`
#define POS_IMPACTWAVE_Y	(0.0f)
#define WIDTH				(32)	//������
#define HEIGHT				(1)		//��������

//�O���[�o���ϐ��錾
const char *c_apFilenameImpactWave[] =	//�t�@�C���ǂݍ���
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
LPDIRECT3DTEXTURE9 g_apTextureImpactWave[(sizeof c_apFilenameImpactWave) / sizeof(*c_apFilenameImpactWave)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffImpactWave = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffImpactWave = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
IMPACTWAVE g_aImpactWave[MAX_IMPACTWAVE];		//�Ռ��g�̏��
int g_nNumVertx;
int g_nNumIndex;

//==================================================================================
//�Ռ��g�̏���������
//==================================================================================
void InitImpactWave(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameImpactWave) / sizeof(*c_apFilenameImpactWave); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameImpactWave[nCntTex],
			&g_apTextureImpactWave[nCntTex]);
	}

	//�e�v�f������
	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		g_aImpactWave[nCntWave].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�N�_�̈ʒu
		g_aImpactWave[nCntWave].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aImpactWave[nCntWave].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ�
		g_aImpactWave[nCntWave].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

		for (int nCntNor = 0; nCntNor < MAX_IMPACTWAVE; nCntNor++)
		{
			g_aImpactWave[nCntWave].nor[nCntNor] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�@���̌���
		}
		g_aImpactWave[nCntWave].col = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);			//�F
		g_aImpactWave[nCntWave].colOrigin = D3DXCOLOR(0.6f, 0.6f, 0.8f, 1.0f);	//���̐F

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aImpactWave[nCntWave].mtxWorld);
		g_aImpactWave[nCntWave].fOutWidth = 100.0f;		//����(�O)
		g_aImpactWave[nCntWave].fInWidth = 0.0f;			//��(��)
		g_aImpactWave[nCntWave].fRotWidth = (D3DX_PI * 2) / (float)(WIDTH);		//1������������̊p�x����
		g_aImpactWave[nCntWave].fHeight = 0.0f;
		g_aImpactWave[nCntWave].fMove = 0.0f;			//�L���鑬�x
		g_aImpactWave[nCntWave].nLife = 0;				//����
		g_aImpactWave[nCntWave].nMaxLife = 0;			//�ő����
		g_aImpactWave[nCntWave].nTexType = 0;			//�e�N�X�`�����
		g_aImpactWave[nCntWave].bUse = false;				//�g�p���Ă��邩
	}
	g_nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//�C���f�b�N�X��
	g_nNumVertx = (HEIGHT + 1) * (WIDTH + 1);	//���_��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (g_nNumVertx * MAX_IMPACTWAVE),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffImpactWave,
		NULL);

	VERTEX_3D *pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffImpactWave->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[MAX_IMPACTWAVE];		//�v�Z�p�̍��W

	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//��
				pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * g_aImpactWave[nCntWave].fOutWidth,
					POS_IMPACTWAVE_Y,
					cosf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * g_aImpactWave[nCntWave].fOutWidth
				);

				//��
				pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
				(
					sinf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * g_aImpactWave[nCntWave].fInWidth,
					POS_IMPACTWAVE_Y,
					cosf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * g_aImpactWave[nCntWave].fInWidth
				);

				//�e���_���猴�_������
				g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aImpactWave[nCntWave].pos;
				g_aImpactWave[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aImpactWave[nCntWave].pos;

				//�o�������̒l�𐳋K������
				D3DXVec3Normalize(&g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
				D3DXVec3Normalize(&g_aImpactWave[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))], &g_aImpactWave[nCntWave].nor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

			}
		}
	}

	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		//���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//���_���W�̐ݒ�
				pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//���_�J���[�̐ݒ�
				pVtx[0].col = g_aImpactWave[nCntWave].col;

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
	g_pVtxBuffImpactWave->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (g_nNumIndex * MAX_IMPACTWAVE),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffImpactWave,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffImpactWave->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCntIdx = 0, nVtxPoint = 0, nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
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

		nVtxPoint += g_nNumVertx;	//���_��������
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffImpactWave->Unlock();

}

//==================================================================================
//�Ռ��g�̏I������
//==================================================================================
void UninitImpactWave(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameImpactWave) / sizeof(*c_apFilenameImpactWave); nCntTex++)
	{
		if (g_apTextureImpactWave[nCntTex] != NULL)
		{
			g_apTextureImpactWave[nCntTex]->Release();
			g_apTextureImpactWave[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffImpactWave != NULL)
	{
		g_pVtxBuffImpactWave->Release();
		g_pVtxBuffImpactWave = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffImpactWave != NULL)
	{
		g_pIdxBuffImpactWave->Release();
		g_pIdxBuffImpactWave = NULL;
	}

}

//==================================================================================
//�Ռ��g�̍X�V����
//==================================================================================
void UpdateImpactWave(void)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	int nCheck = 0;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffImpactWave->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		if (g_aImpactWave[nCntWave].bUse == true)
		{//�g�p����Ă�����

			//����̈ʒu�܂Ői�߂�
			pVtx += g_nNumVertx * nCntWave;
			nCheck += g_nNumVertx * nCntWave;

			g_aImpactWave[nCntWave].pos += g_aImpactWave[nCntWave].move;

			//�L���Ă���
			g_aImpactWave[nCntWave].fOutWidth += g_aImpactWave[nCntWave].fMove;

			//���_���̍X�V
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * (g_aImpactWave[nCntWave].fOutWidth - nCntHeight * g_aImpactWave[nCntWave].fInWidth),
						POS_IMPACTWAVE_Y + nCntHeight * g_aImpactWave[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * (g_aImpactWave[nCntWave].fOutWidth - nCntHeight * g_aImpactWave[nCntWave].fInWidth)
					);

					//���_�J���[�̐ݒ�
					pVtx[0].col = g_aImpactWave[nCntWave].col;

					pVtx += 1;
					nCheck += 1;
				}
			}

			//�i�߂����߂�
			pVtx -= (g_nNumVertx * nCntWave + g_nNumVertx);
			nCheck -= (g_nNumVertx * nCntWave + g_nNumVertx);

			//�����X�V
			g_aImpactWave[nCntWave].nLife--;

			//�s�����x�X�V
			g_aImpactWave[nCntWave].col.a = g_aImpactWave[nCntWave].colOrigin.a * (float)g_aImpactWave[nCntWave].nLife / (float)g_aImpactWave[nCntWave].nMaxLife;

			if (g_aImpactWave[nCntWave].nLife <= 0)
			{//�������s����

				g_aImpactWave[nCntWave].bUse = false;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffImpactWave->Unlock();
}

//==================================================================================
//�Ռ��g�̈ʒu�ݒ菈��
//==================================================================================
void SetPositionImpactWave(int nIdxWave, D3DXVECTOR3 move)
{
	if (g_aImpactWave[nIdxWave].bUse == true)
	{
		g_aImpactWave[nIdxWave].pos.x += move.x;
		g_aImpactWave[nIdxWave].pos.z += move.z;
	}
}

//==================================================================================
//�Ռ��g�̕`�揈��
//==================================================================================
void DrawImpactWave(void)
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

	for (int nCntWave = 0, nIdxPoint = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		if (g_aImpactWave[nCntWave].bUse == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aImpactWave[nCntWave].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aImpactWave[nCntWave].rot.y, g_aImpactWave[nCntWave].rot.x, g_aImpactWave[nCntWave].rot.z);
			D3DXMatrixMultiply(&g_aImpactWave[nCntWave].mtxWorld, &g_aImpactWave[nCntWave].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aImpactWave[nCntWave].pos.x, g_aImpactWave[nCntWave].pos.y, g_aImpactWave[nCntWave].pos.z);
			D3DXMatrixMultiply(&g_aImpactWave[nCntWave].mtxWorld, &g_aImpactWave[nCntWave].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aImpactWave[nCntWave].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffImpactWave, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffImpactWave);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureImpactWave[g_aImpactWave[nCntWave].nTexType]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_nNumVertx/* * nCntWave*/,
				nIdxPoint,
				(g_nNumIndex - 2));

		}

		//����̃C���f�b�N�X�������Z
		nIdxPoint += g_nNumIndex;
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
int SetImpactWave(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fMove, int nTexType)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffImpactWave->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 posMath[MAX_IMPACTWAVE];		//�v�Z�p�̍��W

	int nCnt = -1;
	for (int nCntWave = 0; nCntWave < MAX_IMPACTWAVE; nCntWave++)
	{
		if (g_aImpactWave[nCntWave].bUse == false)
		{//�g�p���Ă��Ȃ�������

			//����̈ʒu�܂Ői�߂�
			pVtx += g_nNumVertx * nCntWave;

			//�e�v�f�ݒ�
			g_aImpactWave[nCntWave].pos = pos;
			g_aImpactWave[nCntWave].move.x = sinf(D3DX_PI + rot.y) * move.x;
			g_aImpactWave[nCntWave].move.z = cosf(D3DX_PI + rot.y) * move.z;
			g_aImpactWave[nCntWave].rot = rot;
			g_aImpactWave[nCntWave].col = col;
			g_aImpactWave[nCntWave].colOrigin = col;
			g_aImpactWave[nCntWave].nLife = nLife;
			g_aImpactWave[nCntWave].nMaxLife = nLife;
			g_aImpactWave[nCntWave].fOutWidth = fWidth;
			g_aImpactWave[nCntWave].fInWidth = g_aImpactWave[nCntWave].fOutWidth;
			g_aImpactWave[nCntWave].fHeight = fHeight;
			g_aImpactWave[nCntWave].fMove = fMove;
			g_aImpactWave[nCntWave].nTexType = nTexType;
			g_aImpactWave[nCntWave].bUse = true;

			//���_���̐ݒ�
			for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
					(
						sinf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * (g_aImpactWave[nCntWave].fOutWidth - nCntHeight * g_aImpactWave[nCntWave].fInWidth),
						POS_IMPACTWAVE_Y + nCntHeight * g_aImpactWave[nCntWave].fHeight,
						cosf(nCntWidth % WIDTH * g_aImpactWave[nCntWave].fRotWidth) * (g_aImpactWave[nCntWave].fOutWidth - nCntHeight * g_aImpactWave[nCntWave].fInWidth)
					);

					//���_���W�̐ݒ�
					pVtx[0].pos = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))];

					//�e���_���猴�_������
					g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))] = posMath[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aImpactWave[nCntWave].pos;

					//�o�������̒l�𐳋K������
					D3DXVec3Normalize(&g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))], &g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))]);

					//�@���x�N�g���̐ݒ�
					pVtx[0].nor = g_aImpactWave[nCntWave].nor[nCntWidth + (nCntHeight * (WIDTH + 1))];

					pVtx += 1;
				}
			}

			int nCntIdx = g_nNumIndex * nCntWave;
			int nVtxPoint = g_nNumVertx * nCntWave;	//���_��������
			nCnt = nCntWave;

			break;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffImpactWave->Unlock();

	return nCnt;
}