//==============================================================
//
//�G�̑O�ɏo����̌`[search_fan.cpp]
//Author:����������
//
//==============================================================
#include"search_fan.h"
#include"enemy.h"
#include"calculation.h"

//�}�N����`
#define WIDTH_VTX		(10)	//���̒��_��
#define HEIGHT_VTX		(1)		//�c�̒��_��
#define FAN_RANGE		(-D3DX_PI * 0.25f)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureSearchFan = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSearchFan = NULL;		//���_�o�b�t�@�ւ̃|�C���^
SearchFan g_aSearchFan[MAX_ENEMY];			//��̏��

int g_nNumVtx;		//�o�[�e�b�N�X��

//==============================================================
//��̏���������
//==============================================================
void InitSearchFan(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\searchfan000.png",
		&g_pTextureSearchFan);

	//������
	for (int nCntFan = 0; nCntFan < MAX_ENEMY; nCntFan++)
	{
		g_aSearchFan[nCntFan].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSearchFan[nCntFan].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSearchFan[nCntFan].mtxWorld = {};		//���[���h�}�g���b�N�X
		g_aSearchFan[nCntFan].bUse = false;
		g_aSearchFan[nCntFan].bDisp = false;
	}

	g_nNumVtx = WIDTH_VTX;		//���_��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVtx * MAX_ENEMY,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSearchFan,
		NULL);

	VERTEX_3D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSearchFan->Lock(0, 0, (void**)&pVtx, 0);

	float fRot = (D3DX_PI * 0.5f) / (float)WIDTH_VTX;
	D3DXVECTOR3 pos[128];

	for (int nCntFan = 0; nCntFan < MAX_ENEMY; nCntFan++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(
			0.0f,
			0.0f,
			0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.7f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);

		pVtx += 1;

		for (int nCntWidth = 1; nCntWidth < WIDTH_VTX - 1; nCntWidth++)
		{//��

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(
				sinf(nCntWidth * fRot + FAN_RANGE) * -SEARCH_RADIUS * 2,
				0.0f,
				cosf(nCntWidth * fRot + FAN_RANGE) * -SEARCH_RADIUS * 2);

			pos[0] = D3DXVECTOR3(
				sinf(nCntWidth * fRot + FAN_RANGE) * -SEARCH_RADIUS * 2,
				0.0f,
				cosf(nCntWidth * fRot + FAN_RANGE) * -SEARCH_RADIUS * 2);

			//�@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.2f);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 1.0f);

			pVtx += 1;

		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSearchFan->Unlock();
}

//==============================================================
//��̏I������
//==============================================================
void UnInitSearchFan(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureSearchFan != NULL)
	{
		g_pTextureSearchFan->Release();
		g_pTextureSearchFan = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSearchFan != NULL)
	{
		g_pVtxBuffSearchFan->Release();
		g_pVtxBuffSearchFan = NULL;
	}
}

//==============================================================
//��̍X�V����
//==============================================================
void UpdateSearchFan(void)
{
	
}

//==============================================================
//��̕`�揈��
//==============================================================
void DrawSearchFan(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;						//�r���[�}�g���b�N�X�擾�p

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntFan = 0; nCntFan < MAX_ENEMY; nCntFan++)
	{
		if (g_aSearchFan[nCntFan].bUse == true && g_aSearchFan[nCntFan].bDisp == true)
		{
			//���[���h�}�g���b�N�X��������
			D3DXMatrixIdentity(&g_aSearchFan[nCntFan].mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aSearchFan[nCntFan].rot.y, g_aSearchFan[nCntFan].rot.x, g_aSearchFan[nCntFan].rot.z);
			D3DXMatrixMultiply(&g_aSearchFan[nCntFan].mtxWorld, &g_aSearchFan[nCntFan].mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aSearchFan[nCntFan].pos.x, g_aSearchFan[nCntFan].pos.y, g_aSearchFan[nCntFan].pos.z);
			D3DXMatrixMultiply(&g_aSearchFan[nCntFan].mtxWorld, &g_aSearchFan[nCntFan].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSearchFan[nCntFan].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffSearchFan, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureSearchFan);

			//��̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLEFAN,		//�v���~�e�B�u�̎��
				g_nNumVtx * nCntFan,						//�v���~�e�B�u
				8);									//�`�悷��v���~�e�B�u��
		}
	}

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==============================================================
//��̈ʒu�X�V����
//==============================================================
void SetPositionSearchFan(int nIdxSearchFan, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{

	g_aSearchFan[nIdxSearchFan].rot.y = rot.y;

	RotNormalize(&g_aSearchFan[nIdxSearchFan].rot.y);

	//�w�肵����̈ʒu��ݒ�
	g_aSearchFan[nIdxSearchFan].pos.x = pos.x;
	g_aSearchFan[nIdxSearchFan].pos.y = pos.y + 0.5f;
	g_aSearchFan[nIdxSearchFan].pos.z = pos.z;


}

//==============================================================
//��̐ݒ菈��
//==============================================================
int SetSearchFan()
{
	int nCntFan;
	int nCounter = -1;

	for (nCntFan = 0; nCntFan < MAX_ENEMY; nCntFan++)
	{
		if (g_aSearchFan[nCntFan].bUse == false)
		{
			nCounter = nCntFan;

			g_aSearchFan[nCntFan].bUse = true;
			g_aSearchFan[nCntFan].bDisp = true;

			break;
		}
	}

	return nCounter;
}

//==============================================================
//��̏��擾
//==============================================================
SearchFan *GetSearchFan(void)
{
	return &g_aSearchFan[0];
}
