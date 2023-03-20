//=============================================================================
//
// �O�Տ��� [meshorbit.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "meshorbit.h"
#include "input.h"
#include "player.h"

//�}�N����`
#define WIDTH	(10)
#define HEIGHT	(1)
#define ORBIT_COUNT	(1)

//�O���[�o���ϐ��錾
const char *c_apFilenameMeshOrbit[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\gradation002.jpg",
};
LPDIRECT3DTEXTURE9 g_pTextureMeshOrbit[(sizeof c_apFilenameMeshOrbit) / sizeof(*c_apFilenameMeshOrbit)] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshOrbit = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshOrbit = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
ORBIT g_aMeshOrbit[MAX_ORBIT];						//�O�Ղ̏��
int g_nNumIndexaMeshOrbit;			//�C���f�b�N�X��
int g_nNumVertexaMeshOrbit;			//���_��
int g_nCntOrbit = 0;

//==================================================================================
//�O�Ղ̏���������
//==================================================================================
void InitMeshOrbit(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMeshOrbit) / sizeof(*c_apFilenameMeshOrbit); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMeshOrbit[nCntTex],
			&g_pTextureMeshOrbit[nCntTex]);
	}

	//�e�v�f������
	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		g_aMeshOrbit[nCntOrbit].pMtxparent = NULL;				//�e�̃}�g���b�N�X�̃|�C���^

		for (int nCntOffset = 0; nCntOffset < OFFSET_POINT; nCntOffset++)
		{
			g_aMeshOrbit[nCntOrbit].aOffSet[nCntOffset] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���[�̃I�t�Z�b�g
			g_aMeshOrbit[nCntOrbit].aCol[nCntOffset] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���[�̊�̐F

			//���[�̃��[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[nCntOffset]);
		}

		for (int nCntPoint = 0; nCntPoint < ALL_POINT; nCntPoint++)
		{
			g_aMeshOrbit[nCntOrbit].aPosPoint[nCntPoint] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�v�Z��̊e���_���W
			g_aMeshOrbit[nCntOrbit].aColPoint[nCntPoint] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�e���_�̐F
		}
	}
	g_nNumIndexaMeshOrbit = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1)) * MAX_ORBIT;
	g_nNumVertexaMeshOrbit = ((WIDTH + 1) * (HEIGHT + 1)) * MAX_ORBIT;
	g_nCntOrbit = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexaMeshOrbit,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshOrbit,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR2 textextex[100];

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		//���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//���_���W�̐ݒ�
				pVtx[0].pos = g_aMeshOrbit[nCntOrbit].aPosPoint[(nCntHeight * (WIDTH + 1)) + nCntWidth];

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

				//���_�J���[�̐ݒ�
				if (((nCntHeight * (WIDTH + 1)) + nCntWidth) < WIDTH + 1)
				{
					pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 1.0f, 0.3f);
				}
				else
				{

					pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
				}

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				(
					((float)nCntWidth / (1.0f / (float)(WIDTH + 1))) * (1.0f / (float)(WIDTH + 1)),
					((float)nCntHeight / (1.0f / (float)(HEIGHT + 1))) * (1.0f / (float)(HEIGHT + 1))
				);

				textextex[(nCntHeight * (WIDTH + 1)) + nCntWidth] = D3DXVECTOR2
				(
					((float)nCntWidth / (1.0f / (float)(WIDTH + 1))) * (1.0f / (float)(WIDTH + 1)),
					((float)nCntHeight / (1.0f / (float)(HEIGHT + 1))) * (1.0f / (float)(HEIGHT + 1))
				);

				pVtx += 1;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshOrbit->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexaMeshOrbit,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshOrbit,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshOrbit->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;
	int nVtxPoint = 0;

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
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

				//��ł�2��
				pIdx[nCntIdx + 0] = (((WIDTH + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
				pIdx[nCntIdx + 1] = ((WIDTH + 1) * (nCntHeight + 2)) + nVtxPoint;
				nCntIdx += 2;	//2�����Ă邩��
			}
		}

		nVtxPoint += (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//����̃C���f�b�N�X�������Z

	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshOrbit->Unlock();

}

//==================================================================================
//�O�Ղ̏I������
//==================================================================================
void UninitMeshOrbit(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMeshOrbit) / sizeof(*c_apFilenameMeshOrbit); nCntTex++)
	{
		if (g_pTextureMeshOrbit[nCntTex] != NULL)
		{
			g_pTextureMeshOrbit[nCntTex]->Release();
			g_pTextureMeshOrbit[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshOrbit != NULL)
	{
		g_pVtxBuffMeshOrbit->Release();
		g_pVtxBuffMeshOrbit = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffMeshOrbit != NULL)
	{
		g_pIdxBuffMeshOrbit->Release();
		g_pIdxBuffMeshOrbit = NULL;
	}

}

//==================================================================================
//�O�Ղ̍X�V����
//==================================================================================
void UpdateMeshOrbit(void)
{
	
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	D3DXVECTOR3 pos[ALL_POINT];

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshOrbit->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{
		int nCntVtx = 0;

		//���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0, nCntVtx = 0; nCntWidth < WIDTH + 1; nCntWidth++, nCntVtx++)
			{//���̒��_�����J��Ԃ�

				//���_���W�̐ݒ�
				pVtx[nCntVtx              ].pos = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth];
				pVtx[nCntVtx + (WIDTH + 1)].pos = g_aMeshOrbit[nCntOrbit].aPosPoint[(WIDTH + 1) + nCntWidth];

				pos[nCntVtx              ] = g_aMeshOrbit[nCntOrbit].aPosPoint[(WIDTH + 1) + nCntWidth];
				pos[nCntVtx + (WIDTH + 1)] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth];
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshOrbit->Unlock();
}

//==================================================================================
//�O�Ղ̕`�揈��
//==================================================================================
void DrawMeshOrbit(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	int nIdxPoint = 0;

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �w�ʂ̃J�����O�͂��܂���B
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (GetKeyboardPress(DIK_F3) == true)
	{//F9�������ꂽ
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//���[�C���[�t���[��
	}

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//���Z�����̐ݒ�
	/*pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);*/

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntOrbit = 0; nCntOrbit < MAX_ORBIT; nCntOrbit++)
	{

		//========================================================
		//�I�t�Z�b�g0�̃}�g���b�N�X�쐬
		//========================================================
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]);
		D3DXMatrixIdentity(g_aMeshOrbit[nCntOrbit].pMtxparent);
		D3DXMatrixIdentity(&mtxTrans);

		//�I�t�Z�b�g���𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_aMeshOrbit[nCntOrbit].aOffSet[0].x, g_aMeshOrbit[nCntOrbit].aOffSet[0].y, g_aMeshOrbit[nCntOrbit].aOffSet[0].z);
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0], &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0], &mtxTrans);

		*g_aMeshOrbit[nCntOrbit].pMtxparent = pPlayer->aModel[1].mtxWorld;
		//�e�̃}�g���b�N�X��������
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0], &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0], g_aMeshOrbit[nCntOrbit].pMtxparent);

		//========================================================
		//�I�t�Z�b�g0�̃}�g���b�N�X�쐬
		//========================================================
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1]);
		D3DXMatrixIdentity(&mtxTrans);

		//�I�t�Z�b�g���𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_aMeshOrbit[nCntOrbit].aOffSet[1].x, g_aMeshOrbit[nCntOrbit].aOffSet[1].y + 100.0f, g_aMeshOrbit[nCntOrbit].aOffSet[1].z);
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1], &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1], &mtxTrans);

		//�e�̃}�g���b�N�X��������
		D3DXMatrixMultiply(&g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1], &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1], g_aMeshOrbit[nCntOrbit].pMtxparent);

		if (g_nCntOrbit == ORBIT_COUNT)
		{
			//�ۑ����Ă��钸�_���W�ƒ��_�J���[�����炷(�㏑���ۑ�)
			/*for (int nCntWidth = ALL_POINT - 1; nCntWidth >= 0; nCntWidth--)
			{
				g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth + 2] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth + 0];
				g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth + 3] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth + 1];
			}*/

			for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					//g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) + nCntWidth] = g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 2) + nCntWidth];
					//g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1 + (WIDTH + 1)) + nCntWidth] = g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit + (WIDTH + 1) - 2) + nCntWidth];

					/*g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - nCntWidth] = 
						g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - nCntWidth - 1];

					g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - (WIDTH + 1) - nCntWidth] = 
						g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - (WIDTH + 1) - nCntWidth - 1];*/

					g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - nCntWidth] =
						g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - nCntWidth - 1];

					g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - (WIDTH + 1) - nCntWidth] =
						g_aMeshOrbit[nCntOrbit].aPosPoint[(g_nNumVertexaMeshOrbit - 1) - (WIDTH + 1) - nCntWidth - 1];

					/*g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 0] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 2];
					g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 1] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 3];*/
				}
			}

			/*for (int nCntWidth = g_nNumVertexaMeshOrbit - 1; nCntWidth >= 0;)
			{
				g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 0] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 2];
				g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 1] = g_aMeshOrbit[nCntOrbit].aPosPoint[nCntWidth - 3];
				nCntWidth -= 2;
			}*/

			//�ŐV�̒��_���W�ƒ��_�J���[��������
			g_aMeshOrbit[nCntOrbit].aPosPoint[WIDTH + 1] = D3DXVECTOR3(
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]._41,
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]._42,
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]._43);

			g_aMeshOrbit[nCntOrbit].aPosPoint[0] = D3DXVECTOR3(
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1]._41,
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1]._42,
				g_aMeshOrbit[nCntOrbit].amtxWorldPoint[1]._43);
		}

		//���[���h�}�g���b�N�X�̐ݒ�
		//pDevice->SetTransform(D3DTS_WORLD, &g_aMeshOrbit[nCntOrbit].amtxWorldPoint[0]);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffMeshOrbit, 0, sizeof(VERTEX_3D));

		//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetIndices(g_pIdxBuffMeshOrbit);

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureMeshOrbit[0]);

		//�|���S���̕`��
		pDevice->DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			((WIDTH + 1) * (HEIGHT + 1)),
			nIdxPoint,
			(HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1)) - 2);

		nIdxPoint += (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));	//����̃C���f�b�N�X�������Z

	}

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �O�p�`�̒��_�������v���Ȃ�Η������ƌ��Ȃ��`�悵�܂���B�i�f�t�H���g�j
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//���߂郂�[�h

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	g_nCntOrbit++;
	if (g_nCntOrbit > ORBIT_COUNT)
	{
		g_nCntOrbit = 0;
	}
}

//==================================================================================
//�O�Ղ̏��擾
//==================================================================================
ORBIT *GetMeshOrbit(void)
{
	return &g_aMeshOrbit[0];
}

//==================================================================================
//�O�Ղ̒��_���擾
//==================================================================================
LPDIRECT3DVERTEXBUFFER9 GetBuffMeshOrbit(void)
{
	return g_pVtxBuffMeshOrbit;
}