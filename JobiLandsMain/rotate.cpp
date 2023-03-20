//==============================================================
//
//���邮�鏈��[rotate.h]
//Author:����������
//
//==============================================================
#include"rotate.h"
#include"mob.h"
#include"crowd.h"
#include"calculation.h"
#include "sound.h"

//�}�N����`
#define POS_X				(10.0f)		//���邮��̑傫��X
#define POS_Y				(5.0f)		//���邮��̑傫��Y

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureRotate = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRotate = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Rotate g_aRotate[MAX_MOB];							//���邮��̏��

//==============================================================
//���邮��̏���������
//==============================================================
void InitRotate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\rotate000.png",
		&g_pTextureRotate);


	//�ϐ��̏�����
	for (int nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
		{
			g_aRotate[nCntRotate].pos[nCntNumber] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu�̏�����
		}

		g_aRotate[nCntRotate].rot[0] = D3DXVECTOR3(0.0f, 0.0f * D3DX_PI, 0.0f);			//�P�ڂ̌����̏�����
		g_aRotate[nCntRotate].rot[1] = D3DXVECTOR3(0.0f, 0.8f * D3DX_PI, 0.0f);			//�Q�ڂ̌����̏�����
		g_aRotate[nCntRotate].rot[2] = D3DXVECTOR3(0.0f, 1.4f * D3DX_PI, 0.0f);			//�R�ڂ̌����̏�����

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aRotate[nCntRotate].mtxWorld);
		g_aRotate[nCntRotate].bUse = false;												//�g�p���Ă��Ȃ���Ԃɂ���
		g_aRotate[nCntRotate].bDisp = false;											//�`�悵�Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_MOB * NUM_ROTATE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRotate,
		NULL);

	VERTEX_3D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffRotate->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
		{

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-POS_X, POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(POS_X, POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-POS_X, -POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(POS_X, -POS_Y, 0.0f);

			//�@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

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

			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffRotate->Unlock();
}

//==============================================================
//���邮��̏I������
//==============================================================
void UnInitRotate(void)
{
	StopSound();

	//�e�N�X�`���̔j��
	if (g_pTextureRotate != NULL)
	{
		g_pTextureRotate->Release();
		g_pTextureRotate = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffRotate != NULL)
	{
		g_pVtxBuffRotate->Release();
		g_pVtxBuffRotate = NULL;
	}
}

//==============================================================
//���邮��̍X�V����
//==============================================================
void UpdateRotate(void)
{
	for (int nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		if (g_aRotate[nCntRotate].bUse == true && g_aRotate[nCntRotate].bDisp == true)
		{//���邮�邪�g�p����Ă���Ƃ�

			for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
			{
				//��]������
				g_aRotate[nCntRotate].rot[nCntNumber].y += 0.05f;

				RotNormalize(&g_aRotate[nCntRotate].rot[nCntNumber].y);
			}
		}
	}
}

//==============================================================
//���邮��̕`�揈��
//==============================================================
void DrawRotate(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		if (g_aRotate[nCntRotate].bUse == true && g_aRotate[nCntRotate].bDisp == true)
		{
			for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
			{

				//���[���h�}�g���b�N�X��������
				D3DXMatrixIdentity(&g_aRotate[nCntRotate].mtxWorld);

				//�r���[�}�g���b�N�X���擾
				pDevice->GetTransform(D3DTS_VIEW, &mtxView);

				//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
				D3DXMatrixInverse(&g_aRotate[nCntRotate].mtxWorld, NULL, &mtxView);		//�t�s������߂�
				g_aRotate[nCntRotate].mtxWorld._41 = 0.0f;
				g_aRotate[nCntRotate].mtxWorld._42 = 0.0f;
				g_aRotate[nCntRotate].mtxWorld._43 = 0.0f;

				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, g_aRotate[nCntRotate].pos[nCntNumber].x,
					g_aRotate[nCntRotate].pos[nCntNumber].y,
					g_aRotate[nCntRotate].pos[nCntNumber].z);

				D3DXMatrixMultiply(&g_aRotate[nCntRotate].mtxWorld,
					&g_aRotate[nCntRotate].mtxWorld, &mtxTrans);

				//���[���h�}�g���b�N�X�̐ݒ�
				pDevice->SetTransform(D3DTS_WORLD, &g_aRotate[nCntRotate].mtxWorld);

				//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
				pDevice->SetStreamSource(0, g_pVtxBuffRotate, 0, sizeof(VERTEX_3D));

				//���_�t�H�[�}�b�g�̐ݒ�
				pDevice->SetFVF(FVF_VERTEX_3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_pTextureRotate);

				//���邮��̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
					(4 * nCntRotate * 3) + (nCntNumber * 4),		//�v���~�e�B�u�i���邮��̐��j
					2);												//�`�悷��v���~�e�B�u��
			}
		}
	}

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//==============================================================
//���邮��̈ʒu�X�V����
//==============================================================
void SetPositionRotate(int nIdxRotate, D3DXVECTOR3 pos)
{
	if (nIdxRotate >= 0 && nIdxRotate < MAX_MOB)
	{
		for (int nCntNumber = 0; nCntNumber < NUM_ROTATE; nCntNumber++)
		{
			//�w�肵�����邮��̈ʒu��ݒ�
			g_aRotate[nIdxRotate].pos[nCntNumber].x = pos.x + sinf(g_aRotate[nIdxRotate].rot[nCntNumber].y) * 25.0f;
			g_aRotate[nIdxRotate].pos[nCntNumber].y = pos.y + 60.0f;
			g_aRotate[nIdxRotate].pos[nCntNumber].z = pos.z + cosf(g_aRotate[nIdxRotate].rot[nCntNumber].y) * 25.0f;
		}
	}
	else
	{
		int nn = 0;
	}
}

//==============================================================
//���邮��̐ݒ菈��
//==============================================================
int SetRotate()
{
	int nCntRotate;
	int nCounter = -1;

	for (nCntRotate = 0; nCntRotate < MAX_MOB; nCntRotate++)
	{
		if (g_aRotate[nCntRotate].bUse == false)
		{
			nCounter = nCntRotate;

			g_aRotate[nCntRotate].bUse = true;
			g_aRotate[nCntRotate].bDisp = false;

			break;
		}
	}

	return nCounter;
}

//==============================================================
//���邮��̏��擾
//==============================================================
Rotate *GetRotate(void)
{
	return &g_aRotate[0];
}
