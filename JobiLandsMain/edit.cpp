//=============================================================================
//
// ���f������ [edit.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "edit.h"
#include "input.h"
#include "camera.h"
#include "game.h"

#include "calculation.h"

//�}�N����`
#define MOVE		(5.0f)
#define MOVE_SLOW	(1.0f)

//�v���g�^�C�v�錾
void ControllEdit(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot);
void ChangeEditType(void);
void GrabEditModel(void);
void DeleteEditModel(int nIdxDelete);

//�O���[�o���ϐ��錾
Edit g_aEdit;	//���f���̏��

//==================================================================================
//�G�f�B�b�g�̏���������
//==================================================================================
void InitEdit(void)
{
	//�e�v�f������
	g_aEdit.pos = D3DXVECTOR3(100.0f, 0.0f, 0.0f);	//���݂̈ʒu
	g_aEdit.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	g_aEdit.nNowModelType = 0;	//���̃��f�����

	//���[���h�}�g���b�N�X
	D3DXMatrixIdentity(&g_aEdit.mtxWorld);
	g_aEdit.nShadow = 1;		//�e�g�����ǂ���[0��1]
	g_aEdit.bUse = false;			//�g�p���Ă��邩

	for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	{
		g_aEdit.aXLoadMeshData[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aEdit.aXLoadMeshData[nCntEdit].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aEdit.aXLoadMeshData[nCntEdit].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
		g_aEdit.aXLoadMeshData[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aEdit.aXLoadMeshData[nCntEdit].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
		g_aEdit.aXLoadMeshData[nCntEdit].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

		//���[���h�}�g���b�N�X
		D3DXMatrixIdentity(&g_aEdit.aXLoadMeshData[nCntEdit].mtxWorld);
		D3DXMatrixIdentity(&g_aEdit.aXLoadMeshData[nCntEdit].OldmtxWorld);

		g_aEdit.aXLoadMeshData[nCntEdit].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aEdit.aXLoadMeshData[nCntEdit].nShadow = 1;			//�e���g�����ǂ���

		g_aEdit.aXLoadMeshData[nCntEdit].nNumVtx = 0;			//���_��
		g_aEdit.aXLoadMeshData[nCntEdit].dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aEdit.aXLoadMeshData[nCntEdit].pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aEdit.aXLoadMeshData[nCntEdit].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
		g_aEdit.aXLoadMeshData[nCntEdit].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

		g_aEdit.aXLoadMeshData[nCntEdit].nType = 0;			//�ǂݍ��ރ��f���̎��
		g_aEdit.aXLoadMeshData[nCntEdit].nParent = -1;		//�e�̔ԍ�
		g_aEdit.aXLoadMeshData[nCntEdit].bUse = false;			//�g�p���Ă��邩
		g_aEdit.aXLoadMeshData[nCntEdit].nState = 0;			//���

		g_aEdit.aXLoadMeshData[nCntEdit].pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
		g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aEdit.aXLoadMeshData[nCntEdit].pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
			g_aEdit.aXLoadMeshData[nCntEdit].pMatData[nCntMat].pTextureFilename = NULL;	//�}�e���A���̃f�[�^
			g_aEdit.aXLoadMeshData[nCntEdit].acFilename[nCntMat] = NULL;	//�t�@�C����
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntTex] = NULL;	//�e�N�X�`���ւ̃|�C���^
		}
		g_aEdit.aXLoadMeshData[nCntEdit].dwNumMat = NULL;			//�}�e���A���̐�
	}


	//���f���̏��擾
	Model *pModelData = GetXLoadData();

	for (int nCntEdit = 0; nCntEdit < GetModelFileNum(); nCntEdit++)
	{//���f���̃t�@�C�������ǂݍ���

		//X�t�@�C���̃f�[�^�擾
		g_aEdit.aXLoadMeshData[nCntEdit] = pModelData[nCntEdit];

		//���f���̑S���_�`�F�b�N
		CheckVtx(g_aEdit.aXLoadMeshData[nCntEdit].rot.y, &g_aEdit.aXLoadMeshData[nCntEdit].vtxMax, &g_aEdit.aXLoadMeshData[nCntEdit].vtxMin, g_aEdit.aXLoadMeshData[nCntEdit].pMesh, g_aEdit.aXLoadMeshData[nCntEdit].pVtxBuff);
	}
}

//==================================================================================
//�G�f�B�b�g�̏I������
//==================================================================================
void UninitEdit(void)
{

	//for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	//{
	//	for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
	//	{
	//		//�e�N�X�`���̔j��
	//		if (g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntTex] != NULL)
	//		{
	//			g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntTex]->Release();
	//			g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntTex] = NULL;
	//		}
	//	}

	//	//���b�V���̔j��
	//	if (g_aEdit.aXLoadMeshData[nCntEdit].pMesh != NULL)
	//	{
	//		g_aEdit.aXLoadMeshData[nCntEdit].pMesh->Release();
	//		g_aEdit.aXLoadMeshData[nCntEdit].pMesh = NULL;
	//	}

	//	//�}�e���A���̔j��
	//	if (g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat != NULL)
	//	{
	//		g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat->Release();
	//		g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat = NULL;
	//	}
	//}
}

//==================================================================================
//�G�f�B�b�g�̍X�V����
//==================================================================================
void UpdateEdit(void)
{

	//���f���̏��擾
	ModelModel *pModel = GetModel();

#ifdef _DEBUG
	if (GetKeyboardTrigger(DIK_F8) == true)
	{//F8�������ꂽ,�g�p�󋵐؂�ւ�

		g_aEdit.bUse = g_aEdit.bUse ? false : true;

		if (g_aEdit.bUse == false)
		{//�G�f�B�b�g���
			SetGameState(GAMESTATE_NONE, 0);
		}
	}
#endif

	if (g_aEdit.bUse == true)
	{//�g�p���Ă�����

		//�G�f�B�b�g���
		//SetGameState(GAMESTATE_EDIT, 0);

		//���쏈��
		ControllEdit(&g_aEdit.pos, &g_aEdit.rot);

		//�t�@�C����ޕύX
		ChangeEditType();

		//���ޏ���
		GrabEditModel();

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{//ENTER�������ꂽ,,���f���ݒu

			if (g_aEdit.nNowModelType != GetModelFileNum() - 1)
			{//�ڈ�͒u���Ȃ�

				SetModel(g_aEdit.pos, g_aEdit.rot, g_aEdit.nNowModelType, g_aEdit.nShadow);
			}
		}
	}
}

//==================================================================================
//�G�f�B�b�g�̑��쏈��
//==================================================================================
void ControllEdit(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pRot)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//���f���̏��擾
	ModelModel *pModel = GetModel();

	if (GetKeyboardPress(DIK_LEFT) == true)
	{//���L�[�������ꂽ,���ړ�

		if (GetKeyboardPress(DIK_UP) == true)
		{//A+W,����ړ�

			pPos->x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
			pPos->z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//A+S,�����ړ�

			pPos->x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
			pPos->z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
		}
		else
		{//A,���ړ�

			pPos->x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
			pPos->z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_RIGHT) == true)
	{//D�L�[�������ꂽ,�E�ړ�

		if (GetKeyboardPress(DIK_UP) == true)
		{//D+W,�E��ړ�

			pPos->x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
			pPos->z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
		}
		else if (GetKeyboardPress(DIK_DOWN) == true)
		{//D+S,�E���ړ�

			pPos->x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
			pPos->z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
		}
		else
		{//D,�E�ړ�

			pPos->x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
			pPos->z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
		}
	}
	else if (GetKeyboardPress(DIK_UP) == true)
	{//W�������ꂽ�A���ړ�

		pPos->x += sinf(pCamera->rot.y) * MOVE;
		pPos->z += cosf(pCamera->rot.y) * MOVE;
	}
	else if (GetKeyboardPress(DIK_DOWN) == true)
	{//S�������ꂽ�A��O�ړ�

		pPos->x += sinf(D3DX_PI + pCamera->rot.y) * MOVE;
		pPos->z += cosf(D3DX_PI + pCamera->rot.y) * MOVE;
	}

	if (GetKeyboardPress(DIK_A) == true)
	{//���L�[�������ꂽ,���ړ�

		if (GetKeyboardPress(DIK_W) == true)
		{//A+W,����ړ�

			pPos->x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE_SLOW;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//A+S,�����ړ�

			pPos->x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE_SLOW;
		}
		else
		{//A,���ړ�

			pPos->x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE_SLOW;
		}
	}
	else if (GetKeyboardPress(DIK_D) == true)
	{//D�L�[�������ꂽ,�E�ړ�

		if (GetKeyboardPress(DIK_W) == true)
		{//D+W,�E��ړ�

			pPos->x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE_SLOW;
		}
		else if (GetKeyboardPress(DIK_S) == true)
		{//D+S,�E���ړ�

			pPos->x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE_SLOW;
		}
		else
		{//D,�E�ړ�

			pPos->x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE_SLOW;
			pPos->z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE_SLOW;
		}
	}
	else if (GetKeyboardPress(DIK_W) == true)
	{//W�������ꂽ�A���ړ�

		pPos->x += sinf(pCamera->rot.y) * MOVE_SLOW;
		pPos->z += cosf(pCamera->rot.y) * MOVE_SLOW;
	}
	else if (GetKeyboardPress(DIK_S) == true)
	{//S�������ꂽ�A��O�ړ�

		pPos->x += sinf(D3DX_PI + pCamera->rot.y) * MOVE_SLOW;
		pPos->z += cosf(D3DX_PI + pCamera->rot.y) * MOVE_SLOW;
	}

	if (GetKeyboardTrigger(DIK_LSHIFT) == true)
	{//��]

		pRot->y += D3DX_PI * 0.1f;
	}
	if (GetKeyboardTrigger(DIK_RSHIFT) == true)
	{//��]

		pRot->y -= D3DX_PI * 0.1f;
	}

	if (GetKeyboardPress(DIK_I) == true)
	{//I�L�[�������ꂽ,�㏸

		pPos->y += MOVE;
	}
	else if (GetKeyboardPress(DIK_K) == true)
	{//K�L�[�������ꂽ,���~

		pPos->y -= MOVE;
	}

	//�p�x���K��
	RotNormalize(&pRot->y);

}

//==================================================================================
//�G�f�B�b�g�̃t�@�C����ޕύX
//==================================================================================
void ChangeEditType(void)
{
	//�t�@�C���̎��
	if (GetKeyboardTrigger(DIK_1) == true)
	{//1�������ꂽ

		//�F�̎�ލX�V
		g_aEdit.nNowModelType = (g_aEdit.nNowModelType + (GetModelFileNum() - 1)) % GetModelFileNum();

	}
	else if (GetKeyboardTrigger(DIK_2) == true)
	{//2�������ꂽ

		//�F�̎�ލX�V
		g_aEdit.nNowModelType = (g_aEdit.nNowModelType + 1) % GetModelFileNum();
	}
}

//==================================================================================
//�G�f�B�b�g�̒͂ݏ���
//==================================================================================
void GrabEditModel(void)
{
	//���f���̏��擾
	ModelModel *pModel = GetModel();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true)
		{//�g�p����Ă�����

			if (pModel->pos.x + pModel->vtxMax.x >= g_aEdit.pos.x + g_aEdit.aXLoadMeshData[g_aEdit.nNowModelType].vtxMin.x &&	//�E����߂荞��ł�
				pModel->pos.x + pModel->vtxMin.x <= g_aEdit.pos.x + g_aEdit.aXLoadMeshData[g_aEdit.nNowModelType].vtxMax.x &&	//������߂荞��ł�
				pModel->pos.z + pModel->vtxMax.z >= g_aEdit.pos.z + g_aEdit.aXLoadMeshData[g_aEdit.nNowModelType].vtxMin.z &&	//����߂荞��ł�
				pModel->pos.z + pModel->vtxMin.z <= g_aEdit.pos.z + g_aEdit.aXLoadMeshData[g_aEdit.nNowModelType].vtxMax.z)		//�O����߂荞��ł�
			{//�����蔻����ɓ����Ă�����͂߂�

				if (GetKeyboardPress(DIK_SPACE) == true)
				{//SPACE�������ꂽ,,����ňړ�

					//���쏈��
					ControllEdit(&pModel->pos, &pModel->rot);
					pModel->posOrigin = pModel->pos;

					if (GetKeyboardTrigger(DIK_LSHIFT) == true)
					{
						pModel->rot.y += D3DX_PI * 0.1f;
					}
					if (GetKeyboardTrigger(DIK_RSHIFT) == true)
					{//��]

						pModel->rot.y -= D3DX_PI * 0.1f;
					}
					//break;
				}

				if (GetKeyboardTrigger(DIK_DELETE) == true)
				{//Delete�������ꂽ�A�폜

					//�폜����
					DeleteEditModel(nCntModel);
				}
			}
		}
	}
}

//==================================================================================
//�G�f�B�b�g�̍폜����
//==================================================================================
void DeleteEditModel(int nIdxDelete)
{
	//���f���̏��擾
	ModelModel *pModel = GetModel();

	//�|�C���^���C���f�b�N�X���i�߂�
	pModel += nIdxDelete;

	//�g�p���ĂȂ���Ԃɂ���
	pModel->bUse = false;

	if (pModel->nShadow != 0)
	{//�e���g�p���Ă���ꍇ

		//SetEnableShadow(pModel->nIdxShadow);
	}

	for (int nCntModel = nIdxDelete; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{//�������ԍ��̃��f���������ւ�

		if (nCntModel + 1 < MAX_MODEL)
		{//�Ō��1�O�܂�

			//����ւ�
			pModel = &pModel[nCntModel + 1];
		}
		else
		{//�Ōゾ��

			pModel = NULL;
		}
	}
}

//==================================================================================
//�G�f�B�b�g�̕`�揈��
//==================================================================================
void DrawEdit(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL mInvisible;			//�_���[�W�}�e���A��
	ZeroMemory(&mInvisible, sizeof(D3DXMATERIAL));

	for (int nCntEdit = 0; nCntEdit < GetModelFileNum(); nCntEdit++)
	{
		if (g_aEdit.nNowModelType == nCntEdit && g_aEdit.bUse == true)
		{//���̃��f����ނƓ�����������

			if (g_aEdit.nNowModelType == 33)
			{
				int nnn = 0;
			}

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aEdit.mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aEdit.rot.y, g_aEdit.rot.x, g_aEdit.rot.z);
			D3DXMatrixMultiply(&g_aEdit.mtxWorld, &g_aEdit.mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aEdit.pos.x, g_aEdit.pos.y, g_aEdit.pos.z);
			D3DXMatrixMultiply(&g_aEdit.mtxWorld, &g_aEdit.mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aEdit.mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aEdit.aXLoadMeshData[nCntEdit].pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aEdit.aXLoadMeshData[nCntEdit].dwNumMat; nCntMat++)
			{

				mInvisible.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, 0.5f);
				mInvisible.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, 0.5f);

				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&mInvisible.MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aEdit.aXLoadMeshData[nCntEdit].pTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aEdit.aXLoadMeshData[nCntEdit].pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//�G�f�B�b�g�̏��擾
//==================================================================================
Edit *GetEdit(void)
{
	return &g_aEdit;
}