//==============================================================
//
//��[flag.cpp]
//Author:����������
//
//==============================================================
#include "flag.h"
#include "crowd.h"
#include "calculation.h"
#include "sound.h"
#include "game.h"
#include "camera.h"

//�}�N����`
#define MAX_FLAG			(3)			//���̍ő�l
#define POS_X				(25.0f)		//���̑傫��X
#define POS_Y				(40.0f)		//���̑傫��Y
#define ANIM_CHANGECNT		(7)		//�A�j���[�V�����ύX����
#define TEX_POS				(0.125f)	//�e�N�X�`���̊Ԋu
#define TEX_DIV				(8)			//�e�N�X�`���̕�����

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_pTextureFlag = NULL;				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFlag = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Flag g_aFlag[MAX_FLAG];		//���̏��

//==============================================================
//���̏���������
//==============================================================
void InitFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\flag.png",
		&g_pTextureFlag);

	//�ϐ�������
	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		g_aFlag[nCntFlag].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		D3DXMatrixIdentity(&g_aFlag[nCntFlag].mtxWorld);				//���[���h�}�g���b�N�X
		g_aFlag[nCntFlag].nCntAnim = 0;		//�A�j���[�V�����ύX�J�E���^�[
		g_aFlag[nCntFlag].nPatternAnim = 0;	//�A�j���[�V����No.
		g_aFlag[nCntFlag].bUse = false;		//�g�p���Ă��邩
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_FLAG,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFlag,
		NULL);

	VERTEX_3D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
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
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.9f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(TEX_POS, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(TEX_POS, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFlag->Unlock();
}

//==============================================================
//���̏I������
//==============================================================
void UnInitFlag(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureFlag != NULL)
	{
		g_pTextureFlag->Release();
		g_pTextureFlag = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFlag != NULL)
	{
		g_pVtxBuffFlag->Release();
		g_pVtxBuffFlag = NULL;
	}
}

//==============================================================
//���̍X�V����
//==============================================================
void UpdateFlag(void)
{
	VERTEX_3D *pVtx;				//���_���ւ̃|�C���^

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	float fCamera;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFlag->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == true)
		{//�g�p���Ă���

			g_aFlag[nCntFlag].nCntAnim++;		//�A�j���[�V�����J�E���^�[���Z

			fCamera = (pCamera->fDistance - 800.0f) * 0.00077f;		//���̑傫���v�Z

			if (GetGameState() == GAMESTATE_BEFOREEND)
			{//�I��������

				g_aFlag[nCntFlag].bUse = false;		//�g�p���ĂȂ���Ԃɂ���
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3((-POS_X * fCamera) - POS_X, (POS_Y * fCamera) + POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3((POS_X * fCamera) + POS_X, (POS_Y * fCamera) + POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3((-POS_X * fCamera) - POS_X, (-POS_Y * fCamera) - POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3((POS_X * fCamera) + POS_X, (-POS_Y * fCamera) - POS_Y, 0.0f);

			if ((g_aFlag[nCntFlag].nCntAnim % ANIM_CHANGECNT) == 0)
			{//��莞�Ԍo��

				//�A�j���[�V����No.�ύX
				g_aFlag[nCntFlag].nPatternAnim = (g_aFlag[nCntFlag].nPatternAnim + 1) % TEX_DIV;

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2((g_aFlag[nCntFlag].nPatternAnim % TEX_DIV) * TEX_POS, 0.0f);
				pVtx[1].tex = D3DXVECTOR2((g_aFlag[nCntFlag].nPatternAnim % TEX_DIV) * TEX_POS + TEX_POS, 0.0f);
				pVtx[2].tex = D3DXVECTOR2((g_aFlag[nCntFlag].nPatternAnim % TEX_DIV) * TEX_POS, 1.0f);
				pVtx[3].tex = D3DXVECTOR2((g_aFlag[nCntFlag].nPatternAnim % TEX_DIV) * TEX_POS + TEX_POS, 1.0f);

			}
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFlag->Unlock();
}

//==============================================================
//���̕`�揈��
//==============================================================
void DrawFlag(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	D3DXMATRIX mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DXMATRIX mtxView;				//�r���[�}�g���b�N�X�擾�p

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == true)
		{//�g�p���Ă���

			//���[���h�}�g���b�N�X��������
			D3DXMatrixIdentity(&g_aFlag[nCntFlag].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aFlag[nCntFlag].mtxWorld, NULL, &mtxView);		//�t�s������߂�
			g_aFlag[nCntFlag].mtxWorld._41 = 0.0f;
			g_aFlag[nCntFlag].mtxWorld._42 = 0.0f;
			g_aFlag[nCntFlag].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aFlag[nCntFlag].pos.x,
				g_aFlag[nCntFlag].pos.y,
				g_aFlag[nCntFlag].pos.z);

			D3DXMatrixMultiply(&g_aFlag[nCntFlag].mtxWorld,
				&g_aFlag[nCntFlag].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aFlag[nCntFlag].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffFlag, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureFlag);

			//���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				4 * nCntFlag,									//�v���~�e�B�u�i���̐��j
				2);												//�`�悷��v���~�e�B�u��
		}
	}

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==============================================================
//���̈ʒu�X�V����
//==============================================================
void SetPositionFlag(int nIdxFlag, D3DXVECTOR3 pos)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	float fCamera;

	fCamera = (pCamera->fDistance - 800.0f) * 0.0001f;		//���̑傫���v�Z

	if (g_aFlag[nIdxFlag].bUse == true)
	{
		//�w�肵�����̈ʒu��ݒ�
		g_aFlag[nIdxFlag].pos.x = pos.x;
		g_aFlag[nIdxFlag].pos.y = pos.y * fCamera + pos.y;
		g_aFlag[nIdxFlag].pos.z = pos.z;

	}
}

//==============================================================
//���̐ݒ菈��
//==============================================================
int SetFlag(void)
{
	int nCounter = -1;

	for (int nCntFlag = 0; nCntFlag < MAX_FLAG; nCntFlag++)
	{
		if (g_aFlag[nCntFlag].bUse == false)
		{
			nCounter = nCntFlag;

			g_aFlag[nCntFlag].bUse = true;

			break;
		}
	}

	return nCounter;
}

//==============================================================
//���̏��擾
//==============================================================
Flag *GetFlag(void)
{
	return &g_aFlag[0];
}