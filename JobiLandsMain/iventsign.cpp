//=========================================================================
//
//�C�x���g�T�C������
//Author:�����V����	[iventsign.cpp]
//
//=========================================================================
#include "iventsign.h"
#include "player.h"

//�O���[�o���ϐ��錾
LPD3DXMESH g_pMeshIventSign = NULL;				//���b�V��(���_���)�ւ̃|�C���^
LPD3DXBUFFER g_pBuffMatIventSign = NULL;		//�}�e���A���̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureIventSign = NULL;	//�e�N�X�`���ւ̃|�C���^
DWORD g_dwNumMatIventSign = 0;					//�}�e���A���̐�
D3DXMATRIX g_mtxWorldIventSign;					//���[���h�}�g���b�N�X

IventSign g_IventSign;				//�C�x���g�T�C���̏����擾

//=========================================================================
//�C�x���g�T�C���̏���������
//=========================================================================
void InitIventSign(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//�f�o�C�X�̎擾
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	//�e��ϐ�������
	g_IventSign.pos = D3DXVECTOR3(0.0f, -100.0f, 0.0f);			//�ʒu�̏�����
	g_IventSign.Iventpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�C�x���g�ʒu
	g_IventSign.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�����̏�����
	g_IventSign.Dest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̈ړ�����
	g_IventSign.bUse = false;			//�g�p���Ă��邩�ǂ���

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX("data\\MODEL\\sign.x",
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&g_pBuffMatIventSign,
		NULL,
		&g_dwNumMatIventSign,
		&g_pMeshIventSign);

	//�}�e���A�����ɑ΂���|�C���^���擾
	pMat = (D3DXMATERIAL*)g_pBuffMatIventSign->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)g_dwNumMatIventSign; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{//�e�N�X�`���t�@�C�������݂���

		 //�e�N�X�`���̓ǂݍ���
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&g_pTextureIventSign);
		}
	}
}

//=========================================================================
//�C�x���g�T�C���̏I������
//=========================================================================
void UninitIventSign(void)
{
	//���b�V���̔j��
	if (g_pMeshIventSign != NULL)
	{
		g_pMeshIventSign->Release();
		g_pMeshIventSign = NULL;
	}

	//�}�e���A���̔j��
	if (g_pBuffMatIventSign != NULL)
	{
		g_pBuffMatIventSign->Release();
		g_pBuffMatIventSign = NULL;
	}

	if (g_pTextureIventSign != NULL)
	{
		g_pTextureIventSign->Release();
		g_pTextureIventSign = NULL;
	}
}

//=========================================================================
//�C�x���g�T�C���̍X�V����
//=========================================================================
void UpdateIventSign(void)
{
	//�v���C���[���擾
	Player *pPlayer = GetPlayer();

	//�ړI�̋����Z�o
	g_IventSign.Dest.x = g_IventSign.Iventpos.x - pPlayer->pos.x;
	g_IventSign.Dest.z = g_IventSign.Iventpos.z - pPlayer->pos.z;

	/*g_IventSign.pos.x = pPlayer->pos.x;
	g_IventSign.pos.z = pPlayer->pos.z;*/

	g_IventSign.rot.y = atan2f(g_IventSign.Dest.x, g_IventSign.Dest.z);		//�ڕW�̈ړ�����(�p�x)
}

//=========================================================================
//�C�x���g�T�C���̕`�揈��
//=========================================================================
void DrawIventSign(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p
	D3DMATERIAL9 matDef;		//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;			//�}�e���A���f�[�^�ւ̃|�C���^
	
	// ���C�e�B���O�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	if (g_IventSign.bUse == true)
	{//�g�p���Ă���Ƃ�

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_mtxWorldIventSign);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_IventSign.rot.y, g_IventSign.rot.x, g_IventSign.rot.z);
		D3DXMatrixMultiply(&g_mtxWorldIventSign, &g_mtxWorldIventSign, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, g_IventSign.pos.x, g_IventSign.pos.y, g_IventSign.pos.z);
		D3DXMatrixMultiply(&g_mtxWorldIventSign, &g_mtxWorldIventSign, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_mtxWorldIventSign);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_pBuffMatIventSign->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)g_dwNumMatIventSign; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureIventSign);

			//���f��(�p�[�c)�̕`��
			g_pMeshIventSign->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
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

//=========================================================================
//�C�x���g�T�C���̎擾
//=========================================================================
void SetIventSign(D3DXVECTOR3 pos)
{
	//�v���C���[���擾
	Player *pPlayer = GetPlayer();

	g_IventSign.Iventpos = pos;		//�ʒu

	//�ړI�̋����Z�o
	g_IventSign.Dest.x = g_IventSign.Iventpos.x - pPlayer->pos.x;
	g_IventSign.Dest.z = g_IventSign.Iventpos.z - pPlayer->pos.z;

	g_IventSign.rot.y = atan2f(g_IventSign.Dest.x, g_IventSign.Dest.z);		//�ڕW�̈ړ�����(�p�x)
}

//=========================================================================
//�C�x���g�T�C���̎擾
//=========================================================================
IventSign *GetIventSign(void)
{
	return &g_IventSign;
}
