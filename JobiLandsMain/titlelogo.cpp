//=============================================================================
//
// �^�C�g�����S���� [titlelogo.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "titlelogo.h"
#include "shadow.h"
#include "calculation.h"

//==================================================================================
//�}�N����`
//==================================================================================
#define TITLE_LOGO_INIT_POS		(D3DXVECTOR3(0.0f,1000.0f,0.0f))		// �^�C�g���̏����ʒu
#define TITLE_LOGO_POS_DEST		(D3DXVECTOR3(0.0f,350.0f,0.0f))			// �^�C�g���̖ړI�̈ʒu
#define TITLE_LOGO_MOVE			(3.0f)									// �^�C�g���̈ړ���

//==================================================================================
// �v���g�^�C�v�錾
//==================================================================================
void SetTitleLogo(void);	// �^�C�g�����S�̐ݒ菈��

//==================================================================================
//�O���[�o���ϐ��錾
//==================================================================================
TitleLogo g_titleLogo;		//�^�C�g�����S�̏��

//==================================================================================
//�^�C�g�����S�̏���������
//==================================================================================
void InitTitleLogo(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���f�����̏�����
	ZeroMemory(&g_titleLogo.aModel, sizeof(Model));

	// ���f�����̐ݒ�
	g_titleLogo.aModel.nIdxShadow	= -1;									// �e�̃C���f�b�N�X�ԍ�
	g_titleLogo.aModel.nShadow		= 1;									// �e���g�����ǂ���
	g_titleLogo.aModel.vtxMin		= D3DXVECTOR3(10.0f, 10.0f, 10.0f);		// ���f���̍ŏ��l
	g_titleLogo.aModel.vtxMax		= D3DXVECTOR3(-10.0f, -10.0f, -10.0f);	// ���f���̍ő�l
	g_titleLogo.aModel.nParent		= -1;									// �e�̔ԍ�
	g_titleLogo.aModel.nState		= TITLE_LOGO_STATE_APPEAR;				// ���
	g_titleLogo.aModel.bUse			= false;								// �g�p��
	
	// �^�C�g�����S�̐ݒ菈��
	SetTitleLogo();
}

//==================================================================================
//�^�C�g�����S�̏I������
//==================================================================================
void UninitTitleLogo(void)
{

}

//==================================================================================
//�^�C�g�����S�̍X�V����
//==================================================================================
void UpdateTitleLogo(void)
{
	if (g_titleLogo.aModel.bUse == true)
	{//�g�p���Ă����ꍇ

		switch (g_titleLogo.aModel.nState)
		{
		case TITLE_LOGO_STATE_APPEAR:		// �o�����

			// �ړ��ʂ����Z����
			g_titleLogo.aModel.pos.y -= g_titleLogo.aModel.move.y;

			if (g_titleLogo.aModel.pos.y <= g_titleLogo.aModel.posDest.y)
			{ // �ʒu���ړI�̈ʒu��ʂ�߂����ꍇ

				// �ړI�̈ʒu�ɐݒ肷��
				g_titleLogo.aModel.pos = TITLE_LOGO_POS_DEST;

				// ��Ԃ�ʏ��Ԃɂ���
				g_titleLogo.aModel.nState = TITLE_LOGO_STATE_NONE;
			}

			break;

		case TITLE_LOGO_STATE_NONE:			// �ʏ���

			// ���ɖ���

			break;
		}
	}
}

//==================================================================================
//�^�C�g�����S�̕`�揈��
//==================================================================================
void DrawTitleLogo(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL mMatRed;			//�_���[�W�}�e���A��

	//���̏��N���A
	ZeroMemory(&mMatRed, sizeof(D3DXMATERIAL));

	mMatRed.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	mMatRed.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	// �w�ʂ̃J�����O�͂��܂���B
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	if (g_titleLogo.aModel.bUse == true)
	{//�g�p���Ă�����

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_titleLogo.aModel.mtxWorld);

		//�����𔽉f����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_titleLogo.aModel.rot.y, g_titleLogo.aModel.rot.x, g_titleLogo.aModel.rot.z);
		D3DXMatrixMultiply(&g_titleLogo.aModel.mtxWorld, &g_titleLogo.aModel.mtxWorld, &mtxRot);

		//�ʒu�𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_titleLogo.aModel.pos.x, g_titleLogo.aModel.pos.y, g_titleLogo.aModel.pos.z);
		D3DXMatrixMultiply(&g_titleLogo.aModel.mtxWorld, &g_titleLogo.aModel.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_titleLogo.aModel.mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_titleLogo.aModel.pBuffMat->GetBufferPointer();

		//���_�����J��Ԃ�
		for (int nCntMat = 0; nCntMat < (int)g_titleLogo.aModel.dwNumMat; nCntMat++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_titleLogo.aModel.pTexture[nCntMat]);

			//���f��(�p�[�c)�̕`��
			g_titleLogo.aModel.pMesh->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);

		// ���C�e�B���O��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	}

	//�f�t�H���g
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==================================================================================
// �^�C�g�����S�̐ݒ菈��
//==================================================================================
void SetTitleLogo(void)
{
	//���f���̏��擾
	Model *pModel = GetXLoadData();

	if (g_titleLogo.aModel.bUse == false)
	{ // �g�p���Ă��Ȃ������ꍇ

		//���f�����擾
		g_titleLogo.aModel = pModel[25];

		//���f���̑S���_�`�F�b�N
		CheckVtx(g_titleLogo.aModel.rot.y, &g_titleLogo.aModel.vtxMax, &g_titleLogo.aModel.vtxMin, g_titleLogo.aModel.pMesh, g_titleLogo.aModel.pVtxBuff);

		// ���̐ݒ�
		g_titleLogo.aModel.pos = TITLE_LOGO_INIT_POS;		// �ʒu
		g_titleLogo.aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ����
		g_titleLogo.aModel.move = D3DXVECTOR3(0.0f, TITLE_LOGO_MOVE, 0.0f);		// �ړ���
		g_titleLogo.aModel.posDest = TITLE_LOGO_POS_DEST;	// �ړI�̈ʒu

		g_titleLogo.aModel.bUse = true;						// �g�p��
	}
}