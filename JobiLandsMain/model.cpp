//=============================================================================
//
// ���f������ [model.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "model.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "minimap.h"
#include "calculation.h"
#include "debugproc.h"
#include "edit.h"

//�}�N����`
#define MOVE		(1.0f)
#define DMG_TIME	(20)		//�_���[�W��Ԃ̎���
#define MODEL_LIFE	(2)

//�v���g�^�C�v�錾
void ReadXFileModel(void);
void SaveSetModel(void);
void XLoadMeshModel(void);
void UpdateStateModel(int nCntModel);
void UpdateAlphaModel(void);
void DrawSkeltonModel(void);
void DrawUnSkeltonModel(void);
void ReadSetModel(void);

//�O���[�o���ϐ��錾
ModelModel g_aModel[MAX_MODEL];		//���f���̏��
int g_nModelFileNum;				//���f���̐�
Model g_aXLoadMeshModel[MAX_MODEL];	//X�t�@�C���̓ǂݍ���
int g_nModelNum;					//���f���̐�
D3DXVECTOR3 g_RandAngleModel;

//==================================================================================
//���f���̏���������
//==================================================================================
void InitModel(void)
{
	//�e�v�f������
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		g_aModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aModel[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
		g_aModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aModel[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
		g_aModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);
		g_aModel[nCntModel].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aModel[nCntModel].nShadow = 1;	//�e���g�����ǂ���
		g_aModel[nCntModel].nNumVtx = 0;			//���_��
		g_aModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
		g_aModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l
		g_aModel[nCntModel].nState = 0;			//���
		g_aModel[nCntModel].nCntState = 0;		//��ԃJ�E���^�[
		g_aModel[nCntModel].nType = 0;			//�ǂݍ��ރ��f���̎��
		g_aModel[nCntModel].nParent = -1;		//�e�̔ԍ�
		g_aModel[nCntModel].nWeapon = 0;		//���킩�ǂ���
		g_aModel[nCntModel].bUse = false;			//�g�p���Ă��邩
		g_aModel[nCntModel].bDisp = false;
		g_aModel[nCntModel].bSkelton = false;	//�����ɂȂ邩�ǂ���

		g_aModel[nCntModel].aXLoadMeshData.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aModel[nCntModel].aXLoadMeshData.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aModel[nCntModel].aXLoadMeshData.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
		g_aModel[nCntModel].aXLoadMeshData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aModel[nCntModel].aXLoadMeshData.rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
		g_aModel[nCntModel].aXLoadMeshData.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aModel[nCntModel].aXLoadMeshData.mtxWorld);
		D3DXMatrixIdentity(&g_aModel[nCntModel].aXLoadMeshData.OldmtxWorld);
		g_aModel[nCntModel].aXLoadMeshData.nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aModel[nCntModel].aXLoadMeshData.nIdxRotate = -1;	//���邮��̃C���f�b�N�X�ԍ�
		g_aModel[nCntModel].aXLoadMeshData.nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
		g_aModel[nCntModel].aXLoadMeshData.nShadow = 1;			//�e���g�����ǂ���

		g_aModel[nCntModel].aXLoadMeshData.nNumVtx = 0;			//���_��
		g_aModel[nCntModel].aXLoadMeshData.dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aModel[nCntModel].aXLoadMeshData.pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aModel[nCntModel].aXLoadMeshData.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
		g_aModel[nCntModel].aXLoadMeshData.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

		g_aModel[nCntModel].aXLoadMeshData.nType = 0;			//�ǂݍ��ރ��f���̎��
		g_aModel[nCntModel].aXLoadMeshData.nParent = -1;		//�e�̔ԍ�
		g_aModel[nCntModel].aXLoadMeshData.bUse = false;			//�g�p���Ă��邩
		g_aModel[nCntModel].aXLoadMeshData.nState = 0;			//���

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aModel[nCntModel].aXLoadMeshData.pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
			g_aModel[nCntModel].aXLoadMeshData.pMatData[nCntMat].pTextureFilename = {};	//�}�e���A���̃f�[�^
			g_aModel[nCntModel].aXLoadMeshData.acFilename[nCntMat] = {};	//�t�@�C����
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aModel[nCntModel].aXLoadMeshData.pTexture[nCntTex] = {};	//�e�N�X�`���ւ̃|�C���^
		}
		g_aModel[nCntModel].aXLoadMeshData.dwNumMat = NULL;			//�}�e���A���̐�
		g_aModel[nCntModel].aXLoadMeshData.pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^
		g_aModel[nCntModel].aXLoadMeshData.pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^


		//X�t�@�C���ǂݍ��݃f�[�^�̏�����
		g_aXLoadMeshModel[nCntModel].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aXLoadMeshModel[nCntModel].posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aXLoadMeshModel[nCntModel].posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
		g_aXLoadMeshModel[nCntModel].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aXLoadMeshModel[nCntModel].rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
		g_aXLoadMeshModel[nCntModel].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aXLoadMeshModel[nCntModel].mtxWorld);
		D3DXMatrixIdentity(&g_aXLoadMeshModel[nCntModel].OldmtxWorld);

		g_aXLoadMeshModel[nCntModel].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aXLoadMeshModel[nCntModel].nIdxRotate = -1;	//���邮��̃C���f�b�N�X�ԍ�
		g_aXLoadMeshModel[nCntModel].nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
		g_aXLoadMeshModel[nCntModel].nIdxSerif = -1;		//�Z���t�̃C���f�b�N�X�ԍ�
		g_aXLoadMeshModel[nCntModel].nShadow = 1;			//�e���g�����ǂ���

		g_aXLoadMeshModel[nCntModel].nNumVtx = 0;			//���_��
		g_aXLoadMeshModel[nCntModel].dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aXLoadMeshModel[nCntModel].pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aXLoadMeshModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
		g_aXLoadMeshModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

		g_aXLoadMeshModel[nCntModel].nType = 0;			//�ǂݍ��ރ��f���̎��
		g_aXLoadMeshModel[nCntModel].nParent = -1;		//�e�̔ԍ�
		g_aXLoadMeshModel[nCntModel].bUse = false;			//�g�p���Ă��邩
		g_aXLoadMeshModel[nCntModel].nState = 0;			//���

		g_aXLoadMeshModel[nCntModel].pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
		g_aXLoadMeshModel[nCntModel].pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^
		g_aXLoadMeshModel[nCntModel].dwNumMat = NULL;			//�}�e���A���̐�

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aXLoadMeshModel[nCntModel].pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
			g_aXLoadMeshModel[nCntModel].pMatData[nCntMat].pTextureFilename = NULL;	//�}�e���A���̃f�[�^
			g_aXLoadMeshModel[nCntModel].acFilename[nCntMat] = NULL;	//�t�@�C����
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aXLoadMeshModel[nCntModel].pTexture[nCntTex] = NULL;	//�e�N�X�`���ւ̃|�C���^
		}

	}
	g_nModelFileNum = 0;
	g_nModelNum = 0;	//���f���̐�
	g_RandAngleModel = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���f���ǂݍ���
	ReadXFileModel();
	ReadSetModel();
}

//==================================================================================
//���f���̏I������
//==================================================================================
void UninitModel(void)
{
	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aXLoadMeshModel[nCntModel].pTexture[nCntTex] != NULL)
			{
				g_aXLoadMeshModel[nCntModel].pTexture[nCntTex]->Release();
				g_aXLoadMeshModel[nCntModel].pTexture[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_aXLoadMeshModel[nCntModel].pMesh != NULL)
		{
			g_aXLoadMeshModel[nCntModel].pMesh->Release();
			g_aXLoadMeshModel[nCntModel].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aXLoadMeshModel[nCntModel].pBuffMat != NULL)
		{
			g_aXLoadMeshModel[nCntModel].pBuffMat->Release();
			g_aXLoadMeshModel[nCntModel].pBuffMat = NULL;
		}
	}
}

//==================================================================================
//���f���̍X�V����
//==================================================================================
void UpdateModel(void)
{
	//�G�f�B�b�g���擾
	Edit *pEdit = GetEdit();

	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		UpdateAlphaModel();
	}

	if (GetKeyboardTrigger(DIK_F9) == true)
	{//F9����������,�Z�[�u

		SaveSetModel();
	}

	PrintDebugProc("���f���̐��F%d\n", g_nModelNum);
}

//==================================================================================
//���f���̏�ԍX�V����
//==================================================================================
void UpdateStateModel(int nCntModel)
{
	//switch (g_aModel[nCntModel].nState)
	//{
	//case MODELSTATE_NONE:
	//	g_aModel[nCntModel].pos = g_aModel[nCntModel].posOrigin;
	//	break;

	//case MODELSTATE_DMG:

	//	//�h�炷
	//	if (g_aModel[nCntModel].nCntState % 10 == 0)
	//	{
	//		g_aModel[nCntModel].move.x *= -1;
	//		g_aModel[nCntModel].move.z *= -1;
	//	}

	//	//�_���[�W�J�E���^�[���Z
	//	g_aModel[nCntModel].nCntState--;

	//	g_aModel[nCntModel].move.x += sinf(g_RandAngleModel.x) * 0.4f;
	//	g_aModel[nCntModel].move.z += cosf(g_RandAngleModel.z) * 0.4f;

	//	if (g_aModel[nCntModel].nCntState <= 0)
	//	{//�_���[�W�J�E���^�[��0�ȉ�

	//		g_aModel[nCntModel].nState = MODELSTATE_NONE;
	//		g_aModel[nCntModel].move.x = 0.0f;
	//		g_aModel[nCntModel].move.z = 0.0f;
	//	}
	//	break;

	//case MODELSTATE_INVISIBLE:
	//	break;

	//case MODELSTATE_FIRE:

	//	
	//	break;
	//}
}

//==================================================================================
//���f���̏�ԍX�V����
//==================================================================================
void UpdateAlphaModel(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true)
		{//�g�p����Ă�����

			//���f���Ƃ̋���
			float fLenModel =
				sqrtf((g_aModel[nCntModel].pos.x - pCamera->posV.x) * (g_aModel[nCntModel].pos.x - pCamera->posV.x)
					+ (g_aModel[nCntModel].pos.z - pCamera->posV.z) * (g_aModel[nCntModel].pos.z - pCamera->posV.z));

			if (fLenModel <= 0)
			{
				int n = 0;
			}

			//�����_�Ƃ̋���
			float fLenPosV =
				sqrtf((pCamera->posR.x - pCamera->posV.x) * (pCamera->posR.x - pCamera->posV.x)
					+ (pCamera->posR.z - pCamera->posV.z) * (pCamera->posR.z - pCamera->posV.z));

			if (fLenPosV <= 0)
			{
				int n = 0;
			}

			//���_���猩�ă��f���̌���
			float fRot = atan2f((g_aModel[nCntModel].pos.x - pCamera->posV.x), (g_aModel[nCntModel].pos.z - pCamera->posV.z));

			fRot = RotNormalize(fRot);

			//�ڕW�Ƃ̍���
			float fRotDiff = pCamera->rot.y - fRot;

			fRotDiff = RotNormalize(fRotDiff);

			//�����̔�r
			if (fLenModel < fLenPosV &&
				fRotDiff >= -0.5f && fRotDiff <= 0.5f &&
				g_aModel[nCntModel].nType < 40)
			{//���f���ƒ����_�̋����̕����Z��

				//�����ɂ���
				g_aModel[nCntModel].bSkelton = true;
			}
			else
			{//�����_�ƃJ�����̋����̕����Z��

				//�����ɂ͂��Ȃ�
				g_aModel[nCntModel].bSkelton = false;
			}


			//�������Ă�����
			if (fLenModel < fLenPosV && bHitCharacter(
				&g_aModel[nCntModel].pos, &g_aModel[nCntModel].posOld, g_aModel[nCntModel].vtxMax, g_aModel[nCntModel].vtxMin,
				&pCamera->posV, D3DXVECTOR3(500.0f, 0.0f, 500.0f), D3DXVECTOR3(-500.0f, 0.0f, -500.0f)) == true &&
				g_aModel[nCntModel].nType < 40)
			{
				//�����ɂ���
				g_aModel[nCntModel].bSkelton = true;

			}

		}
	}
}

//==================================================================================
//���f���̕`�揈��
//==================================================================================
void DrawModel(int nType)
{
	switch (nType)
	{
	case 0:
		//�s�������f���̕`��
		DrawUnSkeltonModel();
		break;

	case 1:
		//�������f���̕`��
		DrawSkeltonModel();
		break;
	}
	
}

//==================================================================================
//�s�����̃��f���`�揈��
//==================================================================================
void DrawUnSkeltonModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true && g_aModel[nCntModel].bSkelton == false)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			if (g_aModel[nCntModel].aXLoadMeshData.pBuffMat != NULL)
			{
				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aModel[nCntModel].aXLoadMeshData.pBuffMat->GetBufferPointer();

				if (pMat != NULL)
				{
					//���_�����J��Ԃ�
					for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].aXLoadMeshData.dwNumMat; nCntMat++)
					{

						if (nCntMat > MAX_TX)
						{
							int n = 9;
						}

						//�}�e���A���̐ݒ�
						pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

						//�e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_aModel[nCntModel].aXLoadMeshData.pTexture[nCntMat]);

						//���f��(�p�[�c)�̕`��
						g_aModel[nCntModel].aXLoadMeshData.pMesh->DrawSubset(nCntMat);
					}
				}
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//�����̃��f���`�揈��
//==================================================================================
void DrawSkeltonModel(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATERIAL mInvisible;			//�����}�e���A��
	ZeroMemory(&mInvisible, sizeof(D3DXMATERIAL));

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == true && g_aModel[nCntModel].bSkelton == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aModel[nCntModel].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z);
			D3DXMatrixMultiply(&g_aModel[nCntModel].mtxWorld, &g_aModel[nCntModel].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aModel[nCntModel].mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			if (g_aModel[nCntModel].aXLoadMeshData.pBuffMat != NULL)
			{
				//�}�e���A���f�[�^�ւ̃|�C���^���擾
				pMat = (D3DXMATERIAL*)g_aModel[nCntModel].aXLoadMeshData.pBuffMat->GetBufferPointer();

				if (pMat != NULL)
				{
					//���_�����J��Ԃ�
					for (int nCntMat = 0; nCntMat < (int)g_aModel[nCntModel].aXLoadMeshData.dwNumMat; nCntMat++)
					{
						//�}�e���A���̐ݒ�
						mInvisible.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, 0.3f);
						mInvisible.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, 0.3f);
						pDevice->SetMaterial(&mInvisible.MatD3D);

						//�e�N�X�`���̐ݒ�
						pDevice->SetTexture(0, g_aModel[nCntModel].aXLoadMeshData.pTexture[nCntMat]);

						//���f��(�p�[�c)�̕`��
						g_aModel[nCntModel].aXLoadMeshData.pMesh->DrawSubset(nCntMat);
					}
				}
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//���f���̃t�@�C�����擾
//==================================================================================
int GetModelFileNum(void)
{
	return g_nModelFileNum;
}

//==================================================================================
//���f���̓ǂݍ��ݏ���
//==================================================================================
void ReadXFileModel(void)
{

	char aComment[MAX_COMMENT];	//�R�����g�p
	int nCntModel = 0;
	int nCntFileName = 0;

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
	}
	else if (GetMode() == MODE_TITLE)
	{
		pFile = fopen("data\\TEXT\\titleedit_info.txt", "r");
	}
	else if (GetMode() == MODE_RESULT)
	{
		pFile = fopen("data\\TEXT\\resultedit_info.txt", "r");
	}
	else if (GetMode() == MODE_RANKING)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
	}

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

			//������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//���f�����̐ݒ�
			if (strcmp(aComment, "NUM_MODEL") == 0)
			{//NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	//=�̕�
				fscanf(pFile, "%d", &g_nModelFileNum);	//���f����
			}

			while (nCntFileName != g_nModelFileNum)
			{//���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

				//������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);

				//���f�����̐ݒ�
				if (strcmp(aComment, "MODEL_FILENAME") == 0)
				{//NUM_MODEL��������

					fscanf(pFile, "%s", &aComment[0]);	//=�̕�
					fscanf(pFile, "%s", &g_aXLoadMeshModel[nCntFileName].acFilename[0]);	//�t�@�C����
					nCntFileName++;	//�t�@�C�������Z
				}
			}
			

			//���f���̐ݒ�
			if (strcmp(aComment, "MODELSET") == 0)
			{//���f���̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_MODELSET"))
				{//END_MODELSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "TYPE") == 0)
					{//TYPE���������ޓǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aModel[nCntModel].nType);	//���f����ނ̗�
					}

					if (strcmp(aComment, "POS") == 0)
					{//POS��������ʒu�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.x);	//X���W
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.y);	//Y���W
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.z);	//Z���W
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROT������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.x);	//X�̌���
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.y);	//Y�̌���
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.z);	//Z�̌���
					}

					if (strcmp(aComment, "SHADOW") == 0)
					{//SHADOW������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);	//=�̕�
						fscanf(pFile, "%d", &g_aModel[nCntModel].nShadow);		//�e���g�����ǂ���
					}

				}//END_MODELSET�̂�����

				//���f���̔ԍ����Z
				nCntModel++;
			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//�I�������Ń��[�v�𔲂���

				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	//X�t�@�C���ǂݍ���
	XLoadMeshModel();
}

//==================================================================================
//���f���̓ǂݍ��ݏ���
//==================================================================================
void ReadSetModel(void)
{

	char aComment[MAX_COMMENT];	//�R�����g�p
	int nCntModel = 0;
	int nCntFileName = 0;

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
	}
	else if (GetMode() == MODE_TITLE)
	{
		pFile = fopen("data\\TEXT\\titleedit_info.txt", "r");
	}
	else if (GetMode() == MODE_RESULT)
	{
		pFile = fopen("data\\TEXT\\resultedit_info.txt", "r");
	}
	else if (GetMode() == MODE_RANKING)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "r");
	}

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

			//������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//���f���̐ݒ�
			if (strcmp(aComment, "MODELSET") == 0)
			{//���f���̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_MODELSET"))
				{//END_MODELSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "TYPE") == 0)
					{//TYPE���������ޓǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aModel[nCntModel].nType);	//���f����ނ̗�
					}

					if (strcmp(aComment, "POS") == 0)
					{//POS��������ʒu�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.x);	//X���W
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.y);	//Y���W
						fscanf(pFile, "%f", &g_aModel[nCntModel].pos.z);	//Z���W
					}

					if (strcmp(aComment, "ROT") == 0)
					{//ROT������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.x);	//X�̌���
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.y);	//Y�̌���
						fscanf(pFile, "%f", &g_aModel[nCntModel].rot.z);	//Z�̌���
					}

					if (strcmp(aComment, "SHADOW") == 0)
					{//SHADOW������������ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);	//=�̕�
						fscanf(pFile, "%d", &g_aModel[nCntModel].nShadow);		//�e���g�����ǂ���
					}

				}//END_MODELSET�̂�����

				 //���f���̔ԍ����Z
				nCntModel++;
			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//�I�������Ń��[�v�𔲂���

				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	for (int nCount = 0; nCount < nCntModel; nCount++)
	{
		//���f���̔z�u
		SetModel(g_aModel[nCount].pos, g_aModel[nCount].rot, g_aModel[nCount].nType, g_aModel[nCount].nShadow);
	}

}

//==================================================================================
//���f�����̕ۑ�����
//==================================================================================
void SaveSetModel(void)
{

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	//�t�@�C�����J��
	if (GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "w");
	}
	if (GetMode() == MODE_TITLE)
	{
		pFile = fopen("data\\TEXT\\titleedit_info.txt", "w");
	}
	else if (GetMode() == MODE_RESULT)
	{
		pFile = fopen("data\\TEXT\\resultedit_info.txt", "w");
	}
	else if (GetMode() == MODE_RANKING)
	{
		pFile = fopen("data\\TEXT\\edit_info.txt", "w");
	}

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		//�t�@�C���ɏ����o��
		fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# ���f����\n"
		"#------------------------------------------------------------------------------\n"
		"NUM_MODEL = %d\n\n", g_nModelFileNum);

		fprintf(pFile,
		"#------------------------------------------------------------------------------\n"
		"# ���f���t�@�C����\n"
		"#------------------------------------------------------------------------------\n");

		//���f���t�@�C������
		for (int nCntFileNum = 0; nCntFileNum < g_nModelFileNum; nCntFileNum++)
		{
			fprintf(pFile, "MODEL_FILENAME = %s\t\t# [%d]\n", &g_aXLoadMeshModel[nCntFileNum].acFilename[0], nCntFileNum);
		}

		fprintf(pFile,
		"\n"
		"#==============================================================================\n"
		"# ���f���̔z�u\n"
		"#==============================================================================\n");

		for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
		{
			if (g_aModel[nCntModel].bUse == true)
			{//�g�p����Ă�����

				fprintf(pFile,
					"MODELSET\n"
					"\tTYPE = %d\n"
					"\tPOS = %.2f %.2f %.2f\n"
					"\tROT = %.2f %.2f %.2f\n"
					"\tSHADOW = %d\n"
					"END_MODELSET\n\n", 
					g_aModel[nCntModel].nType,
					g_aModel[nCntModel].pos.x, g_aModel[nCntModel].pos.y, g_aModel[nCntModel].pos.z,
					g_aModel[nCntModel].rot.x, g_aModel[nCntModel].rot.y, g_aModel[nCntModel].rot.z,
					g_aModel[nCntModel].nShadow);
			}
		}

		fprintf(pFile, "END_SCRIPT		# ���̍s�͐�Ώ����Ȃ����ƁI");

		//�t�@�C�������
		fclose(pFile);
	}
}

//==================================================================================
//X�t�@�C���̓ǂݍ��ݏ���
//==================================================================================
void XLoadMeshModel(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntFileNum = 0; nCntFileNum < g_nModelFileNum; nCntFileNum++)
	{//���f�������ǂݍ���

		//X�t�@�C���̓ǂݍ���
		HRESULT hr = D3DXLoadMeshFromX(&g_aXLoadMeshModel[nCntFileNum].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aXLoadMeshModel[nCntFileNum].pBuffMat,
			NULL,
			&g_aXLoadMeshModel[nCntFileNum].dwNumMat,
			&g_aXLoadMeshModel[nCntFileNum].pMesh);

		if (hr == E_FAIL)
		{
			int nnnnn = 0;
		}

		D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aXLoadMeshModel[nCntFileNum].pBuffMat->GetBufferPointer();

		//���_�����J��Ԃ�
		for (int nCntMat = 0; nCntMat < (int)g_aXLoadMeshModel[nCntFileNum].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�t�@�C�������݂���

				//�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aXLoadMeshModel[nCntFileNum].pTexture[nCntMat]);
			}
		}
	}
}

//==================================================================================
//���f���̏��擾
//==================================================================================
ModelModel *GetModel(void)
{
	return &g_aModel[0];
}

//==================================================================================
//X�t�@�C���̓ǂݍ��ݏ��擾
//==================================================================================
Model *GetXLoadData(void)
{
	return &g_aXLoadMeshModel[0];
}

//==================================================================================
//���f�����擾
//==================================================================================
int GetModelNum(void)
{
	return g_nModelNum;
}

//==================================================================================
//���f���̔z�u����
//==================================================================================
void SetModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nShadow)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++)
	{
		if (g_aModel[nCntModel].bUse == false)
		{//�g�p����Ă��Ȃ�������

			//���f�����擾
			g_aModel[nCntModel].aXLoadMeshData = g_aXLoadMeshModel[nType];

			//���f���̑S���_�`�F�b�N
			CheckVtx(g_aModel[nCntModel].rot.y, &g_aModel[nCntModel].vtxMax, &g_aModel[nCntModel].vtxMin, g_aXLoadMeshModel[nType].pMesh, g_aXLoadMeshModel[nType].pVtxBuff);

			//if (nType >= SMALL_NUM)
			//{//�����͔���폜

			//	g_aModel[nCntModel].vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);
			//	g_aModel[nCntModel].vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
			//}

			g_aModel[nCntModel].pos = pos;
			g_aModel[nCntModel].posOrigin = pos;
			g_aModel[nCntModel].rot = rot;
			g_aModel[nCntModel].nType = nType;
			g_aModel[nCntModel].bUse = true;	//�g�p��Ԃɂ���
			g_aModel[nCntModel].bDisp = true;
			//g_aModel[nCntModel].nState = MODELSTATE_NONE;
			g_nModelNum++;

			////�e�̈ʒu�X�V
			//SetPositionShadow(g_aModel[nCntModel].nIdxShadow, g_aModel[nCntModel].pos);

			if ((GetMode() == MODE_GAME || GetMode() == MODE_TUTORIAL) && (g_aModel[nCntModel].nType < SMALL_NUM || g_aModel[nCntModel].nType >= CONE_NUM))
			{
				//�~�j�}�b�v��̃r���ݒu
				SetMiniMap(g_aModel[nCntModel].pos, g_aModel[nCntModel].vtxMax, g_aModel[nCntModel].vtxMin);
			}
			break;
		}
	}
}