//=============================================================================
//
// ��ʔ����n�_���� [manypoint.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "manypoint.h"
#include "input.h"
#include "mob.h"

//�}�N����`
#define POS_MANYPOINT_Y	(1000.0f)
#define MOVE		(0.0f)
#define WIDTH		(16)
#define HEIGHT		(1)

//�O���[�o���ϐ��錾
const char *c_apFilenameManyPoint[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\gradation007.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureManyPoint[(sizeof c_apFilenameManyPoint) / sizeof(*c_apFilenameManyPoint)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManyPoint = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffManyPoint = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MANYPOINT g_aManyPoint;									//��ʔ����n�_�̏��

//==================================================================================
//��ʔ����n�_�̏���������
//==================================================================================
void InitManyPoint(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameManyPoint) / sizeof(*c_apFilenameManyPoint); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameManyPoint[nCntTex],
			&g_apTextureManyPoint[nCntTex]);
	}

	//�e�v�f������
	g_aManyPoint.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	g_aManyPoint.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aManyPoint.mtxWorld);
	g_aManyPoint.nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));			//�C���f�b�N�X��
	g_aManyPoint.nNumVertex = (HEIGHT + 1) * (WIDTH + 1);			//���_��
	g_aManyPoint.bUse = false;				//�g�p���Ă��邩

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_aManyPoint.nNumVertex,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffManyPoint,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffManyPoint->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[128];		//�v�Z�p�̍��W
	float fRot = (D3DX_PI * 2) / (float)(WIDTH);	//1���Ƃ̊p�x�����߂�

	D3DXVECTOR3 posTest = D3DXVECTOR3(1000.0f, 500.0f, 2000.0f);
	D3DXVECTOR3 NormalizeNor[128];

	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * POS_MANYPOINT,
				(POS_MANYPOINT_Y * HEIGHT) - ((POS_MANYPOINT_Y * (nCntHeight + 1))),
				cosf(nCntWidth % WIDTH * fRot) * POS_MANYPOINT
			);

			pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * POS_MANYPOINT,
				(POS_MANYPOINT_Y * HEIGHT) - ((POS_MANYPOINT_Y * nCntHeight)),
				cosf(nCntWidth % WIDTH * fRot) * POS_MANYPOINT
			);

			//�e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aManyPoint.pos;
			NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aManyPoint.pos;

			//�o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))],				&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))],	&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);

		}
	}

	//���_���̐ݒ�
	for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			//���_���W�̐ݒ�
			pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

			//�@���x�N�g���̐ݒ�
			pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))];

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.2f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2
			(
				nCntWidth * (1.0f / (float)(WIDTH)),
				nCntHeight * (1.0f / (float)(HEIGHT))
			);

			pVtx += 1;
		}

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffManyPoint->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_aManyPoint.nNumIndex,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffManyPoint,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffManyPoint->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;

	//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//�����̕��������J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			pIdx[nCntIdx + 0] = nCntWidth + (WIDTH + 1) * (nCntHeight + 1);
			pIdx[nCntIdx + 1] = nCntWidth + ((WIDTH + 1) * nCntHeight);
			
			nCntIdx += 2;	//2�����Ă邩��
		}

		if (nCntHeight + 1 < HEIGHT)
		{//�Ō�̂����͑ł��Ȃ�

			pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 1)) - 1;
			pIdx[nCntIdx + 1] = (WIDTH + 1) * (nCntHeight + 2);

			nCntIdx += 2;	//2�����Ă邩��
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffManyPoint->Unlock();
}

//==================================================================================
//��ʔ����n�_�̏I������
//==================================================================================
void UninitManyPoint(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameManyPoint) / sizeof(*c_apFilenameManyPoint); nCntTex++)
	{
		if (g_apTextureManyPoint[nCntTex] != NULL)
		{
			g_apTextureManyPoint[nCntTex]->Release();
			g_apTextureManyPoint[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffManyPoint != NULL)
	{
		g_pVtxBuffManyPoint->Release();
		g_pVtxBuffManyPoint = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffManyPoint != NULL)
	{
		g_pIdxBuffManyPoint->Release();
		g_pIdxBuffManyPoint = NULL;
	}

}

//==================================================================================
//��ʔ����n�_�̍X�V����
//==================================================================================
void UpdateManyPoint(void)
{
	ManySpawn *pManySpawn = GetManySpawn();
	static float fAlpha = 0.2f;
	static int fMove = 1;

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffManyPoint->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aManyPoint.bUse == true)
	{//�g�p����Ă�����

		//�s�����x���Z
		fAlpha += 0.01f * fMove;

		if (fAlpha > 0.6f)
		{
			//�s�����x���Z
			fAlpha = 0.6f;
			fMove *= -1;
		}
		else if (fAlpha < 0.2f)
		{
			//�s�����x���Z
			fAlpha = 0.2f;
			fMove *= -1;
		}
		

		//��ʔ����n�_�Ɉړ�
		g_aManyPoint.pos = D3DXVECTOR3(
			pManySpawn->aSpawn[pManySpawn->nBillNumber].pos.x,
			0.0f,
			pManySpawn->aSpawn[pManySpawn->nBillNumber].pos.z);

		//���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, fAlpha);

				pVtx += 1;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffManyPoint->Unlock();
}

//==================================================================================
//��ʔ����n�_�̕`�揈��
//==================================================================================
void DrawManyPoint(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �w�ʂ̃J�����O�͂��܂���B
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	if (g_aManyPoint.bUse == true)
	{//�g�p����Ă�����

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aManyPoint.mtxWorld);

		//�����𔽉f����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aManyPoint.rot.y, g_aManyPoint.rot.x, g_aManyPoint.rot.z);
		D3DXMatrixMultiply(&g_aManyPoint.mtxWorld, &g_aManyPoint.mtxWorld, &mtxRot);

		//�ʒu�𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_aManyPoint.pos.x, g_aManyPoint.pos.y, g_aManyPoint.pos.z);
		D3DXMatrixMultiply(&g_aManyPoint.mtxWorld, &g_aManyPoint.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aManyPoint.mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffManyPoint, 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffManyPoint);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_apTextureManyPoint[0]);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			g_aManyPoint.nNumVertex,
			0,
			g_aManyPoint.nNumIndex - 2);
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	//�f�t�H���g
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

//==================================================================================
//��ʔ����n�_�̏��擾
//==================================================================================
MANYPOINT *GetManyPoint(void)
{
	return &g_aManyPoint;
}
