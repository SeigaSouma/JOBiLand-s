//=============================================================================
//
// ���˕����� [launch.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "launch.h"
#include "input.h"
#include "model.h"
#include "game.h"
#include "edit.h"
#include "calculation.h"
#include "shadow.h"
#include "particle.h"
#include "debugproc.h"
#include "sound.h"

//�}�N����`
#define LAUNCH_GRAVITY	(-0.3f)									// ���˕��̏d��
#define LAUNCH_FLY		(-13.0f)										// ���˕��̔�Ԑ���
#define LAUNCH_POS		(D3DXVECTOR3(230.0f, 150.0f, 0.0f))		// ���˕��̈ʒu

//�O���[�o���ϐ��錾
Launch g_aLaunch[MAX_LAUNCH];		//���˕��̏��

//==================================================================================
//���˕��̏���������
//==================================================================================
void InitLaunch(void)
{
	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{ // ���̏�����

		// ���f�����̏�����
		ZeroMemory(&g_aLaunch[nCntLaunch].modelData, sizeof(Model));

		// ���f�����̐ݒ�
		g_aLaunch[nCntLaunch].modelData.nIdxShadow = -1;								// �e�̃C���f�b�N�X�ԍ�
		g_aLaunch[nCntLaunch].modelData.nShadow = 1;									// �e���g�����ǂ���
		g_aLaunch[nCntLaunch].modelData.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		// ���f���̍ŏ��l
		g_aLaunch[nCntLaunch].modelData.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);	// ���f���̍ő�l
		g_aLaunch[nCntLaunch].modelData.nParent = -1;									// �e�̔ԍ�
		g_aLaunch[nCntLaunch].modelData.bUse = false;									// �g�p��

		// ���̏�����
		g_aLaunch[nCntLaunch].fGravity = 0.0f;			// �d��
	}
}

//==================================================================================
//���˕��̏I������
//==================================================================================
void UninitLaunch(void)
{

}

//==================================================================================
//���˕��̍X�V����
//==================================================================================
void UpdateLaunch(void)
{
	if (GetKeyboardTrigger(DIK_0) == true)
	{ // 0�L�[���������ꍇ

		// ���˕��̐ݒ菈��
		SetLaunch();
	}

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{
		if (g_aLaunch[nCntLaunch].modelData.bUse == true)
		{ // �g�p���Ă���ꍇ
			
			// �d�͂�������
			g_aLaunch[nCntLaunch].fGravity += LAUNCH_GRAVITY;

			// �d�͂��X�V����
			g_aLaunch[nCntLaunch].modelData.move.y = g_aLaunch[nCntLaunch].fGravity;

			// �ʒu���X�V����
			g_aLaunch[nCntLaunch].modelData.pos += g_aLaunch[nCntLaunch].modelData.move;

			if (g_aLaunch[nCntLaunch].modelData.pos.y <= 0.0f)
			{ // �ʒu����萔�ȉ��ɂȂ��

				// �ʒu��␳����
				g_aLaunch[nCntLaunch].modelData.pos.y = 0.0f;

				g_aLaunch[nCntLaunch].modelData.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
		}
	}
}

//==================================================================================
//���˕��̕`�揈��
//==================================================================================
void DrawLaunch(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxnParent;			//�e�̌v�Z�p�}�g���b�N�X

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{
		if (g_aLaunch[nCntLaunch].modelData.bUse == true)
		{//�g�p���Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aLaunch[nCntLaunch].modelData.mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aLaunch[nCntLaunch].modelData.rot.y, g_aLaunch[nCntLaunch].modelData.rot.x, g_aLaunch[nCntLaunch].modelData.rot.z);
			D3DXMatrixMultiply(&g_aLaunch[nCntLaunch].modelData.mtxWorld, &g_aLaunch[nCntLaunch].modelData.mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aLaunch[nCntLaunch].modelData.pos.x, g_aLaunch[nCntLaunch].modelData.pos.y, g_aLaunch[nCntLaunch].modelData.pos.z);
			D3DXMatrixMultiply(&g_aLaunch[nCntLaunch].modelData.mtxWorld, &g_aLaunch[nCntLaunch].modelData.mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aLaunch[nCntLaunch].modelData.mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aLaunch[nCntLaunch].modelData.pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aLaunch[nCntLaunch].modelData.dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aLaunch[nCntLaunch].modelData.pTexture[nCntMat]);

				//���˕�(�p�[�c)�̕`��
				g_aLaunch[nCntLaunch].modelData.pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//���˕��̐ݒ菈��
//==================================================================================
void SetLaunch(void)
{
	Model *pModel = GetXLoadData();		// ���f���̏��

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{//�p�[�c���J��Ԃ�

		if (g_aLaunch[nCntLaunch].modelData.bUse == false)
		{ // �g�p���Ă��Ȃ��ꍇ

			g_aLaunch[nCntLaunch].modelData.nType = LAUNCHTYPE_GOOD;					// �ǂ���

			switch (g_aLaunch[nCntLaunch].modelData.nType)
			{
			case LAUNCHTYPE_GOOD:	// �ǂ��z

				// ���f�������擾����
				g_aLaunch[nCntLaunch].modelData = pModel[7];

				break;				// �����o��

			case LAUNCHTYPE_EVIL:	// �����z

				// ���f�������擾����
				g_aLaunch[nCntLaunch].modelData = pModel[0];

				break;				// �����o��
			}

			// ���̏�����
			g_aLaunch[nCntLaunch].fGravity = 0.0f;			// �d��

			// ���˕��̐ݒ�
			g_aLaunch[nCntLaunch].modelData.pos = LAUNCH_POS;							// �ʒu
			g_aLaunch[nCntLaunch].modelData.move = D3DXVECTOR3(LAUNCH_FLY, 0.0f, 0.0f);	// �ړ���
			g_aLaunch[nCntLaunch].modelData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
			g_aLaunch[nCntLaunch].modelData.bUse = true;								// �g�p��

			break;		// �����o��
		}
	}
}

//==================================================================================
//���˕��̏��擾
//==================================================================================
Launch *GetLaunch(void)
{
	return &g_aLaunch[0];
}