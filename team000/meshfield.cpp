//=============================================================================
//
// ���b�V���t�B�[���h���� [meshfield.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "meshfield.h"
#include "input.h"
#include "debugproc.h"
#include "mapshape.h"

//�v���g�^�C�v�錾
void CreatMeshFieldIndex(void);
void CreatMeshFieldVeretx(void);
void ReadMeshField(void);

//�O���[�o���ϐ��錾
const char *c_apFilenameMeshField[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\FIELD\\concreat.png",
	"data\\TEXTURE\\FIELD\\senter.png",
	"data\\TEXTURE\\FIELD\\senterlong.png",
	"data\\TEXTURE\\FIELD\\sideL.png",
	"data\\TEXTURE\\FIELD\\sideR.png",
	"data\\TEXTURE\\FIELD\\odan.png",
	"data\\TEXTURE\\FIELD\\walkroad.jpg",
	"data\\TEXTURE\\FIELD\\walkroad_01.jpg",
	"data\\TEXTURE\\FIELD\\tile_04.jpg",
	"data\\TEXTURE\\FIELD\\tile_03.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureMeshField[(sizeof c_apFilenameMeshField) / sizeof(*c_apFilenameMeshField)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMeshField = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffMeshField = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
MESHFIELD g_aMeshField[MAX_FIELD];					//���b�V���t�B�[���h�̏��
int g_nNumIndexaMeshField;			//�C���f�b�N�X��
int g_nNumVertexaMeshField;			//���_��
int g_nCntUseField;

//==================================================================================
//���b�V���t�B�[���h�̏���������
//==================================================================================
void InitMeshField(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMeshField) / sizeof(*c_apFilenameMeshField); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMeshField[nCntTex],
			&g_apTextureMeshField[nCntTex]);
	}

	//�e�v�f������
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		g_aMeshField[nCntField].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aMeshField[nCntField].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMeshField[nCntField].mtxWorld);
		g_aMeshField[nCntField].nNumIndex = 0;			//�C���f�b�N�X��
		g_aMeshField[nCntField].nNumVertex = 0;			//���_��
		g_aMeshField[nCntField].nWidth = 0;				//��������
		g_aMeshField[nCntField].nHeight = 0;			//�c������
		g_aMeshField[nCntField].fWidthLen = 0.0f;		//���̒���
		g_aMeshField[nCntField].fHeightLen = 0.0f;		//�c�̒���
		g_aMeshField[nCntField].bUse = false;				//�g�p���Ă��邩
		g_aMeshField[nCntField].nType = 0;				//���
	}
	g_nCntUseField = 0;
	g_nNumIndexaMeshField = 0;
	g_nNumVertexaMeshField = 0;

	//�Z�b�g����ꏊ
	ReadMeshField();

	//���_�o�b�t�@�̐���
	CreatMeshFieldVeretx();

	//�C���f�b�N�X�̐���
	CreatMeshFieldIndex();
}

//==================================================================================
//���b�V���t�B�[���h�̏I������
//==================================================================================
void UninitMeshField(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMeshField) / sizeof(*c_apFilenameMeshField); nCntTex++)
	{
		if (g_apTextureMeshField[nCntTex] != NULL)
		{
			g_apTextureMeshField[nCntTex]->Release();
			g_apTextureMeshField[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMeshField != NULL)
	{
		g_pVtxBuffMeshField->Release();
		g_pVtxBuffMeshField = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffMeshField != NULL)
	{
		g_pIdxBuffMeshField->Release();
		g_pIdxBuffMeshField = NULL;
	}

}

//==================================================================================
//���b�V���t�B�[���h�̍X�V����
//==================================================================================
void UpdateMeshField(void)
{
	PrintDebugProc(
		"���̐��F%d", g_nCntUseField);
}

//==================================================================================
//���b�V���t�B�[���h�̕`�揈��
//==================================================================================
void DrawMeshField(DRAWFIELD_TYPE nType)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	int nIdxPoint = 0;

	if (nType == DRAWFIELD_TYPE_MAP)
	{//�~�j�}�b�v�̎�����


		//�}�b�v�̌`�`�揈��
		//DrawMapShape();


		//���C�e�B���O�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
#if 1
		//Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��
#else
		//Z�e�X�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��
#endif
		//�A���t�@�e�X�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	}

#if _DEBUG
	if (GetKeyboardPress(DIK_F3) == true)
	{//F9�������ꂽ

		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//���߂郂�[�h
	}
	else
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);	//���߂郂�[�h
	}
#endif

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//�g�p���Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMeshField[nCntField].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMeshField[nCntField].rot.y, g_aMeshField[nCntField].rot.x, g_aMeshField[nCntField].rot.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntField].mtxWorld, &g_aMeshField[nCntField].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aMeshField[nCntField].pos.x, g_aMeshField[nCntField].pos.y, g_aMeshField[nCntField].pos.z);
			D3DXMatrixMultiply(&g_aMeshField[nCntField].mtxWorld, &g_aMeshField[nCntField].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMeshField[nCntField].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMeshField, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffMeshField);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMeshField[g_aMeshField[nCntField].nType]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aMeshField[nCntField].nNumVertex,
				nIdxPoint,
				g_aMeshField[nCntField].nNumIndex - 2);
		}

		nIdxPoint += g_aMeshField[nCntField].nNumIndex;	//����̃C���f�b�N�X�������Z
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==================================================================================
//���b�V���t�B�[���h�̐ݒ菈��
//==================================================================================
void SetMeshField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int Width, int Height, float fWidthLen, float fHeightLen, int nType)
{

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == false)
		{//�g�p���Ă��Ȃ�������

			g_aMeshField[nCntField].pos = pos;	//�ʒu
			g_aMeshField[nCntField].rot = rot;	//����
			g_aMeshField[nCntField].nNumVertex = (Width + 1) * (Height + 1);	//���_��
			g_aMeshField[nCntField].nNumIndex = (Height * ((Width + 1) * 2)) + (2 * (Height - 1));	//�C���f�b�N�X��
			g_nNumIndexaMeshField += g_aMeshField[nCntField].nNumIndex;	//�C���f�b�N�X��
			g_nNumVertexaMeshField += g_aMeshField[nCntField].nNumVertex;	//���_��
			g_nCntUseField++;

			g_aMeshField[nCntField].nWidth = Width;			//��������
			g_aMeshField[nCntField].nHeight = Height;		//�c������
			g_aMeshField[nCntField].fWidthLen = fWidthLen;		//���̒���
			g_aMeshField[nCntField].fHeightLen = fHeightLen;	//�c�̒���
			g_aMeshField[nCntField].nType = nType;			//���
			g_aMeshField[nCntField].bUse = true;	//�g�p���Ă���󋵂ɂ���
			break;
		}
	}

}

//==================================================================================
//���b�V���t�B�[���h�̒��_�o�b�t�@�쐬
//==================================================================================
void CreatMeshFieldVeretx(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexaMeshField,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMeshField,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMeshField->Lock(0, 0, (void**)&pVtx, 0);

	//���_���̐ݒ�
	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//�g�p���Ă�����

			for (int nCntHeight = 0; nCntHeight < g_aMeshField[nCntField].nHeight + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshField[nCntField].nWidth + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					//���_���W�̐ݒ�
					pVtx[0].pos = D3DXVECTOR3(
						(g_aMeshField[nCntField].fWidthLen * nCntWidth) - ((g_aMeshField[nCntField].fWidthLen * g_aMeshField[nCntField].nWidth) * 0.5f),
						0.0f,
						-((g_aMeshField[nCntField].fHeightLen * nCntHeight) - ((g_aMeshField[nCntField].fHeightLen * g_aMeshField[nCntField].nHeight) * 0.5f)));

					//�@���x�N�g���̐ݒ�
					pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

					//���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

					//�e�N�X�`�����W�̐ݒ�
					pVtx[0].tex = D3DXVECTOR2
					(
						((float)nCntWidth / (1.0f / (float)(g_aMeshField[nCntField].nWidth + 1))) * (1.0f / (float)(g_aMeshField[nCntField].nWidth + 1)),
						((float)nCntHeight / (1.0f / (float)(g_aMeshField[nCntField].nHeight + 1))) * (1.0f / (float)(g_aMeshField[nCntField].nHeight + 1))
					);

					pVtx += 1;
				}
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMeshField->Unlock();
}

//==================================================================================
//���b�V���t�B�[���h�̃C���f�b�N�X�쐬
//==================================================================================
void CreatMeshFieldIndex(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexaMeshField,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffMeshField,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffMeshField->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;
	int nVtxPoint = 0;

	for (int nCntField = 0; nCntField < MAX_FIELD; nCntField++)
	{
		if (g_aMeshField[nCntField].bUse == true)
		{//�g�p���Ă�����

			//���_�ԍ��f�[�^�̐ݒ�
			for (int nCntHeight = 0; nCntHeight < g_aMeshField[nCntField].nHeight; nCntHeight++)
			{//�����̕��������J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < g_aMeshField[nCntField].nWidth + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					pIdx[nCntIdx + 0] = (nCntWidth + (g_aMeshField[nCntField].nWidth + 1) * (nCntHeight + 1)) + nVtxPoint;
					pIdx[nCntIdx + 1] = (nCntWidth + ((g_aMeshField[nCntField].nWidth + 1) * nCntHeight)) + nVtxPoint;
					nCntIdx += 2;	//2�����Ă邩��
				}

				if (nCntHeight + 1 < g_aMeshField[nCntField].nHeight)
				{//�Ō�̂����͑ł��Ȃ�

					//��ł�2��
					pIdx[nCntIdx + 0] = (((g_aMeshField[nCntField].nWidth + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
					pIdx[nCntIdx + 1] = ((g_aMeshField[nCntField].nWidth + 1) * (nCntHeight + 2)) + nVtxPoint;
					nCntIdx += 2;	//2�����Ă邩��
				}
			}

			nVtxPoint += g_aMeshField[nCntField].nNumVertex;	//����̃C���f�b�N�X�������Z
		}
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffMeshField->Unlock();
}

//==================================================================================
//���b�V���t�B�[���h�̊O���t�@�C���ǂݍ��ݏ���
//==================================================================================
void ReadMeshField(void)
{
	char aComment[256];	//�R�����g�p
	int nCntMesh = 0;
	int nCntFileName = 0;

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	pFile = fopen("data\\TEXT\\model_set.txt", "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

			//������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//���f���̐ݒ�
			if (strcmp(aComment, "FIELDSET") == 0)
			{//���f���̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_FIELDSET"))
				{//END_FIELDSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "TEXTYPE") == 0)
					{//TEXTYPE���������ޓǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aMeshField[nCntMesh].nType);	//���f����ނ̗�
					}

					if (strcmp(aComment, "POS") == 0)
					{//POS��������ʒu�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].pos.x);	//X���W
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].pos.y);	//Y���W
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].pos.z);	//Z���W
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROT������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].rot.x);	//X�̌���
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].rot.y);	//Y�̌���
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].rot.z);	//Z�̌���
						g_aMeshField[nCntMesh].rot.x = D3DXToRadian(g_aMeshField[nCntMesh].rot.x);
						g_aMeshField[nCntMesh].rot.y = D3DXToRadian(g_aMeshField[nCntMesh].rot.y);
						g_aMeshField[nCntMesh].rot.z = D3DXToRadian(g_aMeshField[nCntMesh].rot.z);
					}

					if (strcmp(aComment, "BLOCK") == 0)
					{//BLOCK������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aMeshField[nCntMesh].nWidth);	//���̕�����
						fscanf(pFile, "%d", &g_aMeshField[nCntMesh].nHeight);	//�c�̕�����
					}

					if (strcmp(aComment, "SIZE") == 0)
					{//SIZE������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].fWidthLen);	//���̕�����
						fscanf(pFile, "%f", &g_aMeshField[nCntMesh].fHeightLen);	//�c�̕�����
					}

				}//END_FIELDSET�̂�����

				 //���f���̔ԍ����Z
				nCntMesh++;
			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//�I�������Ń��[�v�𔲂���

				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	for (int nCount = 0; nCount < nCntMesh; nCount++)
	{
		//�t�B�[���h�̔z�u
		SetMeshField(
			g_aMeshField[nCount].pos, g_aMeshField[nCount].rot,
			g_aMeshField[nCount].nWidth, g_aMeshField[nCount].nHeight,
			g_aMeshField[nCount].fWidthLen, g_aMeshField[nCount].fHeightLen, g_aMeshField[nCount].nType);
	}
}