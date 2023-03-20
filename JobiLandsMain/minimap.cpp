//=============================================================================
//
// �|���S������ [minimap.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "minimap.h"
#include "mapshape.h"

//�}�N����`
#define POS_POLYGON	(1000.0f)
#define MAX_TOP		(4)
#define MAX_BILDING	(256)

//�O���[�o���ϐ��錾
const char *c_apFilenameMiniMap[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\window1.png",
};
LPDIRECT3DTEXTURE9 g_apTextureMiniMap[(sizeof c_apFilenameMiniMap) / sizeof(*c_apFilenameMiniMap)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniMap = NULL;			//���_�o�b�t�@�ւ̃|�C���^
MINIMAP g_aMinimap[MAX_BILDING];			//�~�j�}�b�v�̏��

//==================================================================================
//�~�j�}�b�v�̏���������
//==================================================================================
void InitMinimap(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMiniMap) / sizeof(*c_apFilenameMiniMap); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMiniMap[nCntTex],
			&g_apTextureMiniMap[nCntTex]);
	}

	//�e�v�f������
	for (int nCnt = 0; nCnt < MAX_BILDING; nCnt++)
	{
		g_aMinimap[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aMinimap[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aMinimap[nCnt].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//�ő�T�C�Y
		g_aMinimap[nCnt].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//�ŏ��T�C�Y

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMinimap[nCnt].mtxWorld);
		g_aMinimap[nCnt].bUse = false;				//�g�p����Ă��邩�ǂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_BILDING,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiniMap,
		NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILDING; nCnt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMin.x, 0.0f, g_aMinimap[nCnt].vtxMax.z);
		pVtx[1].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMax.x, 0.0f, g_aMinimap[nCnt].vtxMax.z);
		pVtx[2].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMin.x, 0.0f, g_aMinimap[nCnt].vtxMin.z);
		pVtx[3].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMax.x, 0.0f, g_aMinimap[nCnt].vtxMin.z);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		/*pVtx[0].col =
			pVtx[1].col =
			pVtx[2].col =*/
			pVtx[3].col = D3DXCOLOR(1.0f, 0.3f, 0.3f, 1.0f);

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
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMiniMap->Unlock();

	//		SetMiniMap(D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f), D3DXVECTOR3(100.0f, 0.0f, 100.0f));
			/*SetMiniMap(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(1000.0f, 0.0f, 1000.0f), -D3DXVECTOR3(200.0f, 0.0f, 200.0f));*/

}

//==================================================================================
//�~�j�}�b�v�̏I������
//==================================================================================
void UninitMinimap(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMiniMap) / sizeof(*c_apFilenameMiniMap); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureMiniMap[nCntTex] != NULL)
		{
			g_apTextureMiniMap[nCntTex]->Release();
			g_apTextureMiniMap[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMiniMap != NULL)
	{
		g_pVtxBuffMiniMap->Release();
		g_pVtxBuffMiniMap = NULL;
	}
}

//==================================================================================
//�~�j�}�b�v�̍X�V����
//==================================================================================
void UpdateMinimap(void)
{

}

//==================================================================================
//�~�j�}�b�v�̕`�揈��
//==================================================================================
void DrawMinimap(void)
{

	//�}�b�v�̌`�`�揈��
	//DrawMapShape();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

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

	for (int nCnt = 0; nCnt < MAX_BILDING; nCnt++)
	{
		if (g_aMinimap[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMinimap[nCnt].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMinimap[nCnt].rot.y, g_aMinimap[nCnt].rot.x, g_aMinimap[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aMinimap[nCnt].mtxWorld, &g_aMinimap[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aMinimap[nCnt].pos.x, g_aMinimap[nCnt].pos.y, g_aMinimap[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aMinimap[nCnt].mtxWorld, &g_aMinimap[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMinimap[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMiniMap, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMiniMap[0]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==================================================================================
//�~�j�}�b�v�̔z�u����
//==================================================================================
void SetMiniMap(D3DXVECTOR3 pos, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin)
{

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffMiniMap->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_BILDING; nCnt++)
	{
		if (g_aMinimap[nCnt].bUse == false)
		{//�g�p����Ă��Ȃ���Ԃɂ���

			g_aMinimap[nCnt].pos = pos;
			g_aMinimap[nCnt].vtxMax = vtxMax;
			g_aMinimap[nCnt].vtxMin = vtxMin;
			g_aMinimap[nCnt].bUse = true;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMin.x, 0.0f, g_aMinimap[nCnt].vtxMax.z);
			pVtx[1].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMax.x, 0.0f, g_aMinimap[nCnt].vtxMax.z);
			pVtx[2].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMin.x, 0.0f, g_aMinimap[nCnt].vtxMin.z);
			pVtx[3].pos = D3DXVECTOR3(g_aMinimap[nCnt].vtxMax.x, 0.0f, g_aMinimap[nCnt].vtxMin.z);

		/*	pVtx[0].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.5f, 0.5f, 1.0f);*/

			break;
		}
		
		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMiniMap->Unlock();
}
