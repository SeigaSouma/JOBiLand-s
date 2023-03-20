//=============================================================================
//
// �^�C�g���I�������� [titleselect.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "titleselect.h"
#include "title.h"
#include "input.h"
#include "shadow.h"
#include "calculation.h"
#include "camera.h"
#include "buddy.h"

//�}�N����`
#define MOVE		(4.0f)
#define ROT_MOVE	(0.01f)

//�v���g�^�C�v�錾
void UpdateTitleSelectTKTK(int nSelect);

//�O���[�o���ϐ��錾
TitleSelect g_aTitleSelect[MAX_TITLESELECT];		//�^�C�g���I�����̏��

//==================================================================================
//�^�C�g���I�����̏���������
//==================================================================================
void InitTitleSelect(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�v�f������
	for (int nCntTitleSelect = 0; nCntTitleSelect < MAX_TITLESELECT; nCntTitleSelect++)
	{
		g_aTitleSelect[nCntTitleSelect].nFloating = 1;	//���V�̌���
		g_aTitleSelect[nCntTitleSelect].nTurnCounter = 0;	//��]�����񐔃J�E���g
		g_aTitleSelect[nCntTitleSelect].fPosDiffX = 0.0f;	//�ړI�̈ʒu�Ƃ̍���
		g_aTitleSelect[nCntTitleSelect].fPosDiffY = 0.0f;	//�ړI�̈ʒu�Ƃ̍���

		g_aTitleSelect[nCntTitleSelect].aModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aTitleSelect[nCntTitleSelect].aModel.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aTitleSelect[nCntTitleSelect].aModel.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
		g_aTitleSelect[nCntTitleSelect].aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aTitleSelect[nCntTitleSelect].aModel.rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
		g_aTitleSelect[nCntTitleSelect].aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld);
		D3DXMatrixIdentity(&g_aTitleSelect[nCntTitleSelect].aModel.OldmtxWorld);

		g_aTitleSelect[nCntTitleSelect].aModel.nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aTitleSelect[nCntTitleSelect].aModel.nIdxRotate = -1;	//���邮��̃C���f�b�N�X�ԍ�
		g_aTitleSelect[nCntTitleSelect].aModel.nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
		g_aTitleSelect[nCntTitleSelect].aModel.nShadow = 1;			//�e���g�����ǂ���

		g_aTitleSelect[nCntTitleSelect].aModel.nNumVtx = 0;			//���_��
		g_aTitleSelect[nCntTitleSelect].aModel.dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aTitleSelect[nCntTitleSelect].aModel.pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aTitleSelect[nCntTitleSelect].aModel.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
		g_aTitleSelect[nCntTitleSelect].aModel.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

		g_aTitleSelect[nCntTitleSelect].aModel.nType = 0;			//�ǂݍ��ރ��f���̎��
		g_aTitleSelect[nCntTitleSelect].aModel.nParent = -1;		//�e�̔ԍ�
		g_aTitleSelect[nCntTitleSelect].aModel.bUse = false;			//�g�p���Ă��邩
		g_aTitleSelect[nCntTitleSelect].aModel.nState = 0;			//���

		g_aTitleSelect[nCntTitleSelect].aModel.pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
		g_aTitleSelect[nCntTitleSelect].aModel.pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aTitleSelect[nCntTitleSelect].aModel.pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
			g_aTitleSelect[nCntTitleSelect].aModel.pMatData[nCntMat].pTextureFilename = NULL;	//�}�e���A���̃f�[�^
			g_aTitleSelect[nCntTitleSelect].aModel.acFilename[nCntMat] = NULL;	//�t�@�C����
		}

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aTitleSelect[nCntTitleSelect].aModel.pTexture[nCntTex] = NULL;	//�e�N�X�`���ւ̃|�C���^
		}
		g_aTitleSelect[nCntTitleSelect].aModel.dwNumMat = NULL;			//�}�e���A���̐�
	}

}

//==================================================================================
//�^�C�g���I�����̏I������
//==================================================================================
void UninitTitleSelect(void)
{

	

}

//==================================================================================
//�^�C�g���I�����̍X�V����
//==================================================================================
void UpdateTitleSelect(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�^�C�g���̏��擾
	Title *pTitle = GetTitle();

	for (int nCntTitleSelect = 0; nCntTitleSelect < MAX_TITLESELECT; nCntTitleSelect++)
	{
		if (g_aTitleSelect[nCntTitleSelect].aModel.bUse == true)
		{//�g�p���Ă�����

			//��]�̕���
			int nAngle = 1;

			if (pTitle->nSelect == TITLESELECT_END)
			{//�I���̏ꍇ

				//�t��]
				nAngle *= -1;
			}

			//�^�C�g���̑I�����Ɠ������̂���
			g_aTitleSelect[pTitle->nSelect].aModel.rot.y += ROT_MOVE * nAngle;

			//�p�x�̐��K��
			RotNormalize(&g_aTitleSelect[pTitle->nSelect].aModel.rot.y);

			//�I�����͕�������
			g_aTitleSelect[pTitle->nSelect].aModel.pos.y += 0.08f * g_aTitleSelect[nCntTitleSelect].nFloating;

			if (g_aTitleSelect[pTitle->nSelect].aModel.pos.y >= 20.0f)
			{
				g_aTitleSelect[nCntTitleSelect].nFloating *= -1;
			}
			else if (g_aTitleSelect[pTitle->nSelect].aModel.pos.y < 0.0f && g_aTitleSelect[nCntTitleSelect].nFloating == -1)
			{
				g_aTitleSelect[nCntTitleSelect].nFloating *= -1;
			}


			float fRotDest = 0.0f;	//�ڕW�̊p�x
			float fRotDiff = 0.0f;	//���݂ƖڕW�̍���
			float fPosDest = 0.0f;	//�ڕW�̊p�x
			float fPosDiff = 0.0f;	//���݂ƖڕW�̍���

			//�ڕW�Ƃ̍���
			fPosDiff = fPosDest - g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.pos.y;

			//�^�C�g���̑I�����ƈႤ���̂�0.0f�Ɍ�������
			g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.pos.y += fPosDiff * 0.25f;

			//�ڕW�Ƃ̍���
			fRotDiff = fRotDest - g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.rot.y;

			//�p�x�̐��K��
			RotNormalize(&fRotDiff);

			//�^�C�g���̑I�����ƈႤ���̂�0.0f�Ɍ�������
			g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.rot.y += fRotDiff * 0.25f;

			//�p�x�̐��K��
			RotNormalize(&g_aTitleSelect[(pTitle->nSelect + 1) % TITLESELECT_MAX].aModel.rot.y);
		}
	}

	if (pTitle->nState == TITLESTATE_TKTK)
	{//�^�C�g�����`�J�`�J���

		//�^�C�g���I�����̃`�J�`�J����
		UpdateTitleSelectTKTK(pTitle->nSelect);
	}

}

//==================================================================================
//�^�C�g���I�����̃`�J�`�J����
//==================================================================================
void UpdateTitleSelectTKTK(int nSelect)
{
	//�^�C�g���̏��擾
	Title *pTitle = GetTitle();

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();

	if (g_aTitleSelect[nSelect].nTurnCounter == 0)
	{
		//�ڕW�Ƃ̍���
		g_aTitleSelect[nSelect].fPosDiffX = 0.0f - g_aTitleSelect[nSelect].aModel.pos.x;
		g_aTitleSelect[nSelect].fPosDiffY = 60.0f - g_aTitleSelect[nSelect].aModel.pos.y;
	}

	if (g_aTitleSelect[nSelect].nTurnCounter <= 10)
	{//��]����30�ȉ��̎�

		pBuddy[BUDDYTYPE_HYOROGARI].formationPos.z = -350.0f;
		pBuddy[BUDDYTYPE_HYOROGARI].formationPos.x = -100.0f;
		pBuddy[BUDDYTYPE_DEPPA].formationPos.z = -350.0f;
		pBuddy[BUDDYTYPE_DEPPA].formationPos.x = 100.0f;

		//�^�C�g���̑I�����ƈႤ���̂�0.0f�Ɍ�������
		g_aTitleSelect[nSelect].aModel.pos.x += g_aTitleSelect[nSelect].fPosDiffX / (float)50;
		g_aTitleSelect[nSelect].aModel.pos.y += g_aTitleSelect[nSelect].fPosDiffY / (float)50;

		if (g_aTitleSelect[nSelect].aModel.pos.x >= 0.0f)
		{
			g_aTitleSelect[nSelect].aModel.pos.x = 0.0f;
			g_aTitleSelect[nSelect].aModel.pos.y = 60.0f;
		}

		//�^�C�g���̑I�����Ɠ������̂���
		g_aTitleSelect[nSelect].aModel.rot.y += 0.8f;

		if (g_aTitleSelect[nSelect].aModel.rot.y >= D3DX_PI)
		{
			//��]�����Z
			g_aTitleSelect[nSelect].nTurnCounter++;
		}

		//�p�x�̐��K��
		RotNormalize(&g_aTitleSelect[nSelect].aModel.rot.y);
	}
	else if (g_aTitleSelect[nSelect].nTurnCounter > 10)
	{//30��]���A�c��

		pBuddy[BUDDYTYPE_HYOROGARI].formationPos.z = -700.0f;
		pBuddy[BUDDYTYPE_HYOROGARI].formationPos.x = -100.0f;
		pBuddy[BUDDYTYPE_DEPPA].formationPos.z = -700.0f;
		pBuddy[BUDDYTYPE_DEPPA].formationPos.x = 100.0f;

		float fRotDest = 0.0f;	//�ڕW�̊p�x
		float fRotDiff = 0.0f;	//���݂ƖڕW�̍���

		//�ڕW�Ƃ̍���
		fRotDiff = fRotDest - g_aTitleSelect[nSelect].aModel.rot.y;

		//�p�x�̐��K��
		RotNormalize(&fRotDiff);

		//�^�C�g���̑I�����ƈႤ���̂�0.0f�Ɍ�������
		g_aTitleSelect[nSelect].aModel.rot.y = 0.0f;

		//�p�x�̐��K��
		RotNormalize(&g_aTitleSelect[nSelect].aModel.rot.y);

		g_aTitleSelect[nSelect].aModel.pos.z -= 5.0f;

		if (g_aTitleSelect[nSelect].aModel.pos.z <= -700.0f)
		{
			pTitle->nState = TITLESTATE_FADE;
			g_aTitleSelect[nSelect].aModel.pos.z = -700.0f;
		}
	}


}

//==================================================================================
//�^�C�g���I�����̕`�揈��
//==================================================================================
void DrawTitleSelect(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�^�C�g���̏��擾
	Title *pTitle = GetTitle();

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

	for (int nCntTitleSelect = 0; nCntTitleSelect < MAX_TITLESELECT; nCntTitleSelect++)
	{
		if (g_aTitleSelect[nCntTitleSelect].aModel.bUse == true)
		{//�g�p���Ă�����

			if (pTitle->nSelect == nCntTitleSelect)
			{//�I�����Ɠ������̂̓��C�e�B���OOFF

				// ���C�e�B���O�𖳌��ɂ���
				pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
			}

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aTitleSelect[nCntTitleSelect].aModel.rot.y, g_aTitleSelect[nCntTitleSelect].aModel.rot.x, g_aTitleSelect[nCntTitleSelect].aModel.rot.z);
			D3DXMatrixMultiply(&g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld, &g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aTitleSelect[nCntTitleSelect].aModel.pos.x, g_aTitleSelect[nCntTitleSelect].aModel.pos.y, g_aTitleSelect[nCntTitleSelect].aModel.pos.z);
			D3DXMatrixMultiply(&g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld, &g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aTitleSelect[nCntTitleSelect].aModel.mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aTitleSelect[nCntTitleSelect].aModel.pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aTitleSelect[nCntTitleSelect].aModel.dwNumMat; nCntMat++)
			{
				//�}�e���A���̐ݒ�
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

				//�e�N�X�`���̐ݒ�
				pDevice->SetTexture(0, g_aTitleSelect[nCntTitleSelect].aModel.pTexture[nCntMat]);

				//���f��(�p�[�c)�̕`��
				g_aTitleSelect[nCntTitleSelect].aModel.pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);

			// ���C�e�B���O��L���ɂ���
			pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
		}
	}

	//�f�t�H���g
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//==================================================================================
//�^�C�g���I�����̐ݒ菈��
//==================================================================================
void SetTitleSelect(D3DXVECTOR3 pos, int nModelType)
{
	//���f���̏��擾
	Model *pModel = GetXLoadData();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntTitleSelect = 0; nCntTitleSelect < MAX_TITLESELECT; nCntTitleSelect++)
	{
		if (g_aTitleSelect[nCntTitleSelect].aModel.bUse == false)
		{//�g�p���Ă��Ȃ�������

			//���f�����擾
			g_aTitleSelect[nCntTitleSelect].aModel = pModel[nModelType];

			g_aTitleSelect[nCntTitleSelect].aModel.pos = pos;	//�ʒu
			g_aTitleSelect[nCntTitleSelect].aModel.bUse = true;					//�g�p���Ă���󋵂ɂ���

			//���f���̑S���_�`�F�b�N
			CheckVtx(g_aTitleSelect[nCntTitleSelect].aModel.rot.y, &g_aTitleSelect[nCntTitleSelect].aModel.vtxMax, &g_aTitleSelect[nCntTitleSelect].aModel.vtxMin, g_aTitleSelect[nCntTitleSelect].aModel.pMesh, g_aTitleSelect[nCntTitleSelect].aModel.pVtxBuff);
			break;
		}
	}
}

//==================================================================================
//�^�C�g���I�����̏��擾
//==================================================================================
TitleSelect *GetTitleSelect(void)
{
	return &g_aTitleSelect[0];
}
