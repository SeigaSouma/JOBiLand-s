//==========================================================================
//
//�A�j���[�V����[animation.cpp]
//Author:�����V����
//
//==========================================================================
#include"main.h"
#include"animation.h"
#include"input.h"
#include"model.h"
#include"player.h"

//�}�N����`
#define SIZE	(300.0f)		//�ǂ̃T�C�Y
#define MOVE_U	(1.0f / 6.0f)
#define ANIM_TIME	(100)

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9	g_pTextureAnimation[MAX_TEX] = {};		//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffAnimation = NULL;			//���_�̃o�b�t�@
Animation g_aAnimation[NUM_TEX];

//�e�N�X�`���t�@�C����
const char *c_apFilenameAnimation[] =
{
	"data\\TEXTURE\\manga000.png",
	"data\\TEXTURE\\mang001.png"
};

//==========================================================================
//�A�j���[�V�����̏���������
//==========================================================================
void InitAnimation(void)
{

	//�f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	

	//�e�N�X�`���̓ǂݍ���
	for (int nCntAnim = 0; nCntAnim < MAX_TEX; nCntAnim++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameAnimation[nCntAnim],
			&g_pTextureAnimation[nCntAnim]);
	}

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		g_aAnimation[nCntAnim].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̏�����
		g_aAnimation[nCntAnim].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����������������(Z�l���g�p����)
		g_aAnimation[nCntAnim].bUse = false;
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * NUM_TEX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffAnimation,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos.x = (-SIZE);
		pVtx[1].pos.x = (+SIZE * 2);
		pVtx[2].pos.x = (-SIZE);
		pVtx[3].pos.x = (+SIZE * 2);

		pVtx[0].pos.y = (+SIZE * 2);
		pVtx[1].pos.y = (+SIZE * 2);
		pVtx[2].pos.y = (+0.0f);
		pVtx[3].pos.y = (+0.0f);

		pVtx[0].pos.z = (0.0f);
		pVtx[1].pos.z = (0.0f);
		pVtx[2].pos.z = (0.0f);
		pVtx[3].pos.z = (0.0f);

		////�����̐ݒ�(y��)
		//pVtx[0].rot.y = D3DX_PI * 0.0f; //����
		//pVtx[1].rot.y = D3DX_PI * 0.5f; //�E����
		//pVtx[2].rot.y = D3DX_PI * 1.0f; //������
		//pVtx[3].rot.y = D3DX_PI * -0.5f; //������

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
		pVtx[1].tex = D3DXVECTOR2(1.0f / 6.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f / 6.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAnimation->Unlock();

	//--------------------------------------
	//�ǂ̐ݒu
	//--------------------------------------
	SetAnimation(D3DXVECTOR3(-845.00f, 525.0f, 758.00f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 0, 85.0f, 100.0f);  //��0
	SetAnimation(D3DXVECTOR3(-830.00f, 283.0f, 50.00f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 1, 68.0f, 65.0f); //��1
	SetAnimation(D3DXVECTOR3(-845.10f, 718.0f, 715.00f), D3DXVECTOR3(0.0f, D3DX_PI * -0.5f, 0.0f), 1, 142.0f, 135.0f);  //��2
	SetAnimation(D3DXVECTOR3(2780.0f, 235.0f, 78.00f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 0, 70.0f, 64.0f); //���0
	SetAnimation(D3DXVECTOR3(2950.00f, 410.0f, 93.00f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 1, 160.0f, 155.0f); //���1
	SetAnimation(D3DXVECTOR3(1890.00f, 535.0f, -3270.00f), D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, 0.0f), 1, 112.0f, 162.0f); //���2
}

//==========================================================================
//�A�j���[�V�����̏I������
//==========================================================================
void UninitAnimation(void)
{
	for (int nCntAnim = 0; nCntAnim < MAX_TEX; nCntAnim++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureAnimation[nCntAnim] != NULL)
		{
			g_pTextureAnimation[nCntAnim]->Release();
			g_pTextureAnimation[nCntAnim] = NULL;
		}
	}

	//���_�o�b�t�@�̏���
	if (g_pVtxBuffAnimation != NULL)
	{
		g_pVtxBuffAnimation->Release();
		g_pVtxBuffAnimation = NULL;
	}
}

//==========================================================================
//�A�j���[�V�����̍X�V����
//==========================================================================
void UpdateAnimation(void)
{
	VERTEX_3D *pVtx;
	Player *pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		if (g_aAnimation[nCntAnim].bUse == true)
		{
			g_aAnimation[nCntAnim].nCountAnim++;	//�J�E���^�[�����Z

			if ((g_aAnimation[nCntAnim].nCountAnim % ANIM_TIME) == 0)
			{//�K��J�E���g�i�񂾂�

				//�p�^�[��No.���X�V
				g_aAnimation[nCntAnim].nPatternAnim = (g_aAnimation[nCntAnim].nPatternAnim + 1) % 6;
				g_aAnimation[nCntAnim].nCountAnim = 0;

			}

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2((g_aAnimation[nCntAnim].nPatternAnim % 6) * MOVE_U, 0.0f);
			pVtx[1].tex = D3DXVECTOR2((g_aAnimation[nCntAnim].nPatternAnim % 6 + 1) * MOVE_U, 0.0f);
			pVtx[2].tex = D3DXVECTOR2((g_aAnimation[nCntAnim].nPatternAnim % 6) * MOVE_U, 1.0f);
			pVtx[3].tex = D3DXVECTOR2((g_aAnimation[nCntAnim].nPatternAnim % 6 + 1) * MOVE_U, 1.0f);
		}

		pVtx += 4;

	}


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAnimation->Unlock();

}

//==========================================================================
//�A�j���[�V�����̕`�揈��
//==========================================================================
void DrawAnimation(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxRot, mtxTrans;					//�v�Z�p�}�g���b�N�X

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		if (g_aAnimation[nCntAnim].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aAnimation[nCntAnim].g_mtxWorld);

			//�����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aAnimation[nCntAnim].rot.y, g_aAnimation[nCntAnim].rot.x, g_aAnimation[nCntAnim].rot.z);
			D3DXMatrixMultiply(&g_aAnimation[nCntAnim].g_mtxWorld, &g_aAnimation[nCntAnim].g_mtxWorld, &mtxRot);

			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_aAnimation[nCntAnim].pos.x, g_aAnimation[nCntAnim].pos.y, g_aAnimation[nCntAnim].pos.z);
			D3DXMatrixMultiply(&g_aAnimation[nCntAnim].g_mtxWorld, &g_aAnimation[nCntAnim].g_mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aAnimation[nCntAnim].g_mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffAnimation, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			switch (g_aAnimation[nCntAnim].nType)
			{
			case 0:
				//�`�揈��
				pDevice->SetTexture(0, g_pTextureAnimation[0]);
				break;

			case 1:
				//�`�揈��
				pDevice->SetTexture(0, g_pTextureAnimation[1]);
				break;
			}

			//�ǂ̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntAnim * 4, 2);		//�v���~�e�B�u�̎��
		}
	}


	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==========================================================================
//�A�j���[�V�����̐ݒ菈��
//==========================================================================
void SetAnimation(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, float fWidth, float fHeight)
{
	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffAnimation->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntAnim = 0; nCntAnim < NUM_TEX; nCntAnim++)
	{
		if (g_aAnimation[nCntAnim].bUse == false)
		{//�G���g�p����Ȃ�
			g_aAnimation[nCntAnim].pos = pos;
			g_aAnimation[nCntAnim].rot = rot;
			g_aAnimation[nCntAnim].nType = nType;
			g_aAnimation[nCntAnim].bUse = true;
			g_aAnimation[nCntAnim].fWidth = fWidth;
			g_aAnimation[nCntAnim].fHeight = fHeight;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = (-fWidth);
			pVtx[1].pos.x = (+fWidth * 2);
			pVtx[2].pos.x = (-fWidth);
			pVtx[3].pos.x = (+fWidth * 2);

			pVtx[0].pos.y = (+fHeight * 2);
			pVtx[1].pos.y = (+fHeight * 2);
			pVtx[2].pos.y = (+0.0f);
			pVtx[3].pos.y = (+0.0f);

			pVtx[0].pos.z = (0.0f);
			pVtx[1].pos.z = (0.0f);
			pVtx[2].pos.z = (0.0f);
			pVtx[3].pos.z = (0.0f);

			break;
		}
		pVtx += 4;
	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffAnimation->Unlock();
}

//==========================================================================
//�A�j���[�V�����̎擾����
//==========================================================================
Animation *GetAnimation(void)
{
	return &g_aAnimation[0];
}
