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
#include "player.h"

//�}�N����`
#define LAUNCH_GRAVITY			(-0.3f)									// ���˕��̏d��
#define LAUNCH_FLY				(-13.0f)								// ���˕��̔�Ԑ���
#define LAUNCH_POS				(D3DXVECTOR3(600.0f, 150.0f, 0.0f))		// ���˕��̈ʒu
#define LAUNCH_RETURN_POS		(130.0f)								// ���˕��̒��˕Ԃ��\���W
#define LAUNCH_LEVEL			(4)										// ���˕��̃��x��
#define LAUNCH_NUM_RANGE		(3)										// ���˕��͈̔͂̐�

// �]���֌W�̃}�N����`
#define LAUNCH_GOOD_RANGE		(LAUNCH_RETURN_POS)						// ���˕��͈̔�(�Œ�]��)
#define LAUNCH_GREAT_RANGE		(8.0f)									// ���˕��͈̔�(���]��)
#define LAUNCH_PERFECT_RANGE	(-100.0f)								// ���˕��͈̔�(�ō��]��)
#define LAUNCH_GOOD_SCORE		(100)									// ���˕��͈̔�(�Œ�]��)

// ���˕Ԃ��֌W�̃}�N����`
#define LAUNCH_RETURN_GOOD		(D3DXVECTOR3(400.0f,0.0f,0.0f))			// �ǂ����̍s����
#define LAUNCH_RETURN_EVIL		(D3DXVECTOR3(700.0f,0.0f,0.0f))			// �������̍s����

// �v���g�^�C�v�錾
void FlyLaunch(Launch *pLaunch);				// ���˕��̔�ԏ���
void ReturnLaunch(Launch *pLaunch);				// ���˕��̒��˕Ԃ�����
void DistanceReturnLaunch(Launch *pLaunch);		// ���˕��̋������Z����
void LaunchReturnRange(Launch *pLaunch);		// ���˕��͈̔͑��菈��

//�O���[�o���ϐ��錾
Launch g_aLaunch[MAX_LAUNCH];		//���˕��̏��

// ���˕��̃��x���ݒ�
Launch_Info g_aLaunchInfo[LAUNCH_LEVEL] =
{
	{ 0, 0.015f, -0.1f},
	{ 1, 0.022f,-0.15f},
	{ 2, 0.025f,-0.2f},
	{ 3, 0.03f,-0.3f},
};

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
		g_aLaunch[nCntLaunch].modelData.nType = LAUNCHTYPE_GOOD;						// ���
		g_aLaunch[nCntLaunch].modelData.nState = LAUNCHSTATE_FLY;						// ���
		g_aLaunch[nCntLaunch].modelData.bUse = false;									// �g�p��

		// ���̏�����
		g_aLaunch[nCntLaunch].fGravity = 0.0f;			// �d��
		g_aLaunch[nCntLaunch].nScore = 0;				// �X�R�A
		g_aLaunch[nCntLaunch].fSpeed = 0.0f;			// �X�s�[�h
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
		SetLaunch(0);
	}

	if (GetKeyboardTrigger(DIK_9) == true)
	{ // 0�L�[���������ꍇ

		// ���˕��̐ݒ菈��
		SetLaunch(1);
	}

	if (GetKeyboardTrigger(DIK_8) == true)
	{ // 0�L�[���������ꍇ

		// ���˕��̐ݒ菈��
		SetLaunch(2);
	}

	if (GetKeyboardTrigger(DIK_7) == true)
	{ // 0�L�[���������ꍇ

		// ���˕��̐ݒ菈��
		SetLaunch(3);
	}

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{
		if (g_aLaunch[nCntLaunch].modelData.bUse == true)
		{ // �g�p���Ă���ꍇ

			// �O��̈ʒu���L�^����
			g_aLaunch[nCntLaunch].modelData.posOld = g_aLaunch[nCntLaunch].modelData.pos;

			switch (g_aLaunch[nCntLaunch].modelData.nState)
			{
			case LAUNCHSTATE_FLY:		// ��ԏ��

				// �d�͂�������
				g_aLaunch[nCntLaunch].fGravity += g_aLaunchInfo[g_aLaunch[nCntLaunch].nLevel].fGravity;

				// ���˕��̔�ԏ���
				FlyLaunch(&g_aLaunch[nCntLaunch]);

				if (g_aLaunch[nCntLaunch].modelData.pos.x <= LAUNCH_RETURN_POS)
				{ // �ʒu�������߂����ꍇ

					// ���˕Ԃ��\��Ԃɂ���
					g_aLaunch[nCntLaunch].modelData.nState = LAUNCHSTATE_RETURN_POSSIBLE;
				}

				break;					// �����o��

			case LAUNCHSTATE_RETURN_POSSIBLE:		// ���˕Ԃ��\���

				// �d�͂�������
				g_aLaunch[nCntLaunch].fGravity += g_aLaunchInfo[g_aLaunch[nCntLaunch].nLevel].fGravity;

				// ���˕��̔�ԏ���
				FlyLaunch(&g_aLaunch[nCntLaunch]);

				// ���˕��̒��˕Ԃ�����
				ReturnLaunch(&g_aLaunch[nCntLaunch]);

				break;					// �����o��

			case LAUNCHSTATE_RETURN:	// ���˕Ԃ����

				if (g_aLaunch[nCntLaunch].modelData.nType == LAUNCHTYPE_GOOD)
				{ // �ǂ����������ꍇ

					// �d�͂�������
					g_aLaunch[nCntLaunch].fGravity += g_aLaunchInfo[g_aLaunch[nCntLaunch].nLevel].fGravity;
				}

				// ���˕��̔�ԏ���
				FlyLaunch(&g_aLaunch[nCntLaunch]);

				break;					// �����o��
			}

			if (g_aLaunch[nCntLaunch].modelData.pos.y <= 0.0f)
			{ // �ʒu����萔�ȉ��ɂȂ��

				// �ʒu��␳����
				g_aLaunch[nCntLaunch].modelData.pos.y = 0.0f;

				// �ړ��ʂ�����������
				g_aLaunch[nCntLaunch].modelData.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

				// �g�p���Ȃ�
				g_aLaunch[nCntLaunch].modelData.bUse = false;
			}
		}
	}

	// ���˕��̏�Ԃ̃f�o�b�O�\��
	PrintDebugProc("���˕��̏�ԁF[%d]\n", g_aLaunch[0].modelData.nState);
	PrintDebugProc("���˕��͈̔́F[%d]\n", g_aLaunch[0].nScore);
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
void SetLaunch(int nLevel)
{
	Model *pModel = GetXLoadData();		// ���f���̏��
	Player *pPlayer = GetPlayer();		// �v���C���[�̏����擾����
	D3DXVECTOR3 distance;				// ����
	int nType;							// �����_���ŎZ�o������

	for (int nCntLaunch = 0; nCntLaunch < MAX_LAUNCH; nCntLaunch++)
	{//�p�[�c���J��Ԃ�

		if (g_aLaunch[nCntLaunch].modelData.bUse == false)
		{ // �g�p���Ă��Ȃ��ꍇ

			// ���̐ݒ�
			g_aLaunch[nCntLaunch].nScore = 0;								// �X�R�A
			g_aLaunch[nCntLaunch].nLevel = nLevel;							// ���x��
			g_aLaunch[nCntLaunch].fSpeed = g_aLaunchInfo[nLevel].fFrame;	// �X�s�[�h
			nType = rand() % 2;												// �ǂ���

			switch (nType)
			{
			case LAUNCHTYPE_GOOD:	// �ǂ��z

				// ���f�������擾����
				g_aLaunch[nCntLaunch].modelData = pModel[7];

				break;				// �����o��

			case LAUNCHTYPE_EVIL:	// �����z

				// ���f�������擾����
				g_aLaunch[nCntLaunch].modelData = pModel[2];

				break;				// �����o��
			}

			// ���̏�����
			g_aLaunch[nCntLaunch].modelData.pos = LAUNCH_POS;	// �ʒu
			g_aLaunch[nCntLaunch].modelData.posOld = g_aLaunch[nCntLaunch].modelData.pos;		// �O��̈ʒu
			g_aLaunch[nCntLaunch].modelData.nType = nType;		// ���
			g_aLaunch[nCntLaunch].modelData.nState = LAUNCHSTATE_FLY;		// ���

			// �����𑪂�
			distance.x = (pPlayer->pos.x - g_aLaunch[nCntLaunch].modelData.pos.x);
			distance.y = (pPlayer->pos.y - g_aLaunch[nCntLaunch].modelData.pos.y);

			// �ړ��ʂ����߂�
			distance.x *= g_aLaunch[nCntLaunch].fSpeed;

			g_aLaunch[nCntLaunch].fGravity = 0.0f;				// �d��

			// ���˕��̐ݒ�
			g_aLaunch[nCntLaunch].modelData.move = D3DXVECTOR3(distance.x, 0.0f, 0.0f);	// �ړ���
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

//==================================================================================
// ���˕��̃��x���̎擾����
//==================================================================================
Launch_Info *GetLaundhLevel(void)
{
	// ���˕��̃��x����Ԃ�
	return &g_aLaunchInfo[0];
}

//==================================================================================
// ���˕��̔�ԏ���
//==================================================================================
void FlyLaunch(Launch *pLaunch)
{
	// �d�͂��X�V����
	pLaunch->modelData.move.y = pLaunch->fGravity;

	// �ʒu���X�V����
	pLaunch->modelData.pos += pLaunch->modelData.move;
}

//==================================================================================
// ���˕��̒��˕Ԃ�����
//==================================================================================
void ReturnLaunch(Launch *pLaunch)
{
	Player *pPlayer = GetPlayer();		// �v���C���[�̏����擾����

	if (GetKeyboardTrigger(DIK_W) == true)
	{ // ENTER�L�[���������ꍇ

		// �d�͂�����������
		pLaunch->fGravity = 4.0f;

		// ���˕Ԃ��Ԃɂ���
		pLaunch->modelData.nState = LAUNCHSTATE_RETURN;

		// ���˕��̋������Z����
		DistanceReturnLaunch(pLaunch);

		// ���˕��͈̔͑��菈��
		LaunchReturnRange(pLaunch);
	}
}

//==================================================================================
// ���˕��̋������Z����
//==================================================================================
void DistanceReturnLaunch(Launch *pLaunch)
{
	D3DXVECTOR3 distance;

	if (pLaunch->modelData.nType == LAUNCHTYPE_GOOD)
	{ // �ǂ����������ꍇ

		// �ړ��ʂ�ݒ肷��
		pLaunch->modelData.move = D3DXVECTOR3(-LAUNCH_FLY, 0.0f, 0.0f);
	}
	else if (pLaunch->modelData.nType == LAUNCHTYPE_EVIL)
	{ // �������������ꍇ

		// �����𑪂�
		distance.x = (LAUNCH_RETURN_EVIL.x - pLaunch->modelData.pos.x);
		distance.y = (LAUNCH_RETURN_EVIL.y - pLaunch->modelData.pos.y);

		// �ړ��ʂ����߂�
		distance.x *= 0.01f;
		distance.y *= -0.05f;

		// ���˕��̐ݒ�
		pLaunch->modelData.move = D3DXVECTOR3(distance.x, 0.0f, 0.0f);	// �ړ���
		pLaunch->fGravity = distance.y;									// �d��
	}
}

//==================================================================================
// ���˕��͈̔͑��菈��
//==================================================================================
void LaunchReturnRange(Launch *pLaunch)
{
	// ���_���Z�o����
	if (pLaunch->modelData.pos.x <= LAUNCH_PERFECT_RANGE)
	{ // �͈͂� PERFECT ������������ꍇ

		// �X�R�A��ݒ肷��
		pLaunch->nScore = 300;
	}
	else if (pLaunch->modelData.pos.x <= LAUNCH_GREAT_RANGE)
	{ // �͈͂� GREAT ������������ꍇ

		// �X�R�A��ݒ肷��
		pLaunch->nScore = 200;
	}
	else if (pLaunch->modelData.pos.x <= LAUNCH_GOOD_RANGE)
	{ // �͈͂� GOOD ������������ꍇ

		// �X�R�A��ݒ肷��
		pLaunch->nScore = LAUNCH_GOOD_SCORE;
	}
}