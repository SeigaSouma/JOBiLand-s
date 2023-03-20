//=============================================================================
//
// �~�j�}�b�v�A�C�R�� [minimap_icon.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "player.h"
#include "minimap_icon.h"
#include "mapshape.h"

//�}�N����`
#define SIZE	(80.0f)	//�摜�T�C�Y
#define MAX_CHR	(1280)		//�L�����N�^�[��

//�O���[�o���ϐ��錾
const char *c_apFilenameMiniIcon[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\000.png",
	"data\\TEXTURE\\001.png",
	"data\\TEXTURE\\002.png",
	"data\\TEXTURE\\003.png",
	"data\\TEXTURE\\004.png",
};

LPDIRECT3DTEXTURE9 g_apTextureMiniIcon[(sizeof c_apFilenameMiniIcon) / sizeof(*c_apFilenameMiniIcon)] = {};
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiniIcon = NULL;
MINIICON g_aMiniIcon[MAX_CHR];		//�A�C�R���̏��

//==================================================================================
//�A�C�R���̏���������
//==================================================================================
void InitMiniIcon(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameMiniIcon) / sizeof(*c_apFilenameMiniIcon); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameMiniIcon[nCntTex],
			&g_apTextureMiniIcon[nCntTex]);
	}

	//�e�ϐ��̏�����
	for (int nCnt = 0; nCnt < MAX_CHR; nCnt++)
	{
		g_aMiniIcon[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aMiniIcon[nCnt].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aMiniIcon[nCnt].mtxWorld);
		g_aMiniIcon[nCnt].nType = 0;				//�A�C�R���̃^�C�v
		g_aMiniIcon[nCnt].bUse = false;				//�g�p����Ă��邩�ǂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_CHR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiniIcon,
		NULL);

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMiniIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_CHR; nCnt++)
	{
		//�ʒu���X�V
		pVtx[0].pos = D3DXVECTOR3(- SIZE, 0.0f, + SIZE);
		pVtx[1].pos = D3DXVECTOR3(+ SIZE, 0.0f, + SIZE);
		pVtx[2].pos = D3DXVECTOR3(- SIZE, 0.0f, - SIZE);
		pVtx[3].pos = D3DXVECTOR3(+ SIZE, 0.0f, - SIZE);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���i�߂�

	}
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMiniIcon->Unlock();
}

//==================================================================================
//�A�C�R���̏I������
//==================================================================================
void UninitMiniIcon(void)
{
	for (int nCnt = 0; nCnt < (sizeof c_apFilenameMiniIcon) / sizeof(*c_apFilenameMiniIcon); nCnt++)
	{
		//�e�N�X�`���̔j��
		if (g_apTextureMiniIcon[nCnt] != NULL)
		{
			g_apTextureMiniIcon[nCnt]->Release();
			g_apTextureMiniIcon[nCnt] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffMiniIcon != NULL)
	{
		g_pVtxBuffMiniIcon->Release();
		g_pVtxBuffMiniIcon = NULL;
	}
}

//==================================================================================
//�A�C�R���̍X�V����
//==================================================================================
void UpdateMiniIcon(void)
{

}

//==================================================================================
//�A�C�R���̈ʒu�X�V����
//==================================================================================
void UpdatePositionIcon(int nIdxIcon, D3DXVECTOR3 pos)
{
	if (g_aMiniIcon[nIdxIcon].bUse == true)
	{//�g�p����Ă�����

		//�A�C�R������������
		g_aMiniIcon[nIdxIcon].pos = pos;
	}
}

//==================================================================================
//�A�C�R���̈ʒu�X�V����
//==================================================================================
int SetIcon(int nType, float size)
{
	int nCntShadow = -1;

	VERTEX_3D *pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMiniIcon->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_CHR; nCount++)
	{
		if (g_aMiniIcon[nCount].bUse == false)
		{//�g�p���Ă��Ȃ�������

			//�ʒu���X�V
			pVtx[0].pos = D3DXVECTOR3(-size, 0.0f, +size);
			pVtx[1].pos = D3DXVECTOR3(+size, 0.0f, +size);
			pVtx[2].pos = D3DXVECTOR3(-size, 0.0f, -size);
			pVtx[3].pos = D3DXVECTOR3(+size, 0.0f, -size);

			g_aMiniIcon[nCount].nType = nType;
			nCntShadow = nCount;	//�C���f�b�N�X�ԍ��擾
			g_aMiniIcon[nCount].bUse = true;	//�g�p���Ă���󋵂ɂ���
			break;
		}

		pVtx += 4;		//���_�f�[�^�̃|�C���^��4���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffMiniIcon->Unlock();

	return nCntShadow;
}

//==================================================================================
//���u�̕`�揈��
//==================================================================================
void DrawMiniIcon(void)
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

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCnt = 0; nCnt < MAX_CHR; nCnt++)
	{
		if (g_aMiniIcon[nCnt].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aMiniIcon[nCnt].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aMiniIcon[nCnt].rot.y, g_aMiniIcon[nCnt].rot.x, g_aMiniIcon[nCnt].rot.z);
			D3DXMatrixMultiply(&g_aMiniIcon[nCnt].mtxWorld, &g_aMiniIcon[nCnt].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aMiniIcon[nCnt].pos.x, g_aMiniIcon[nCnt].pos.y, g_aMiniIcon[nCnt].pos.z);
			D3DXMatrixMultiply(&g_aMiniIcon[nCnt].mtxWorld, &g_aMiniIcon[nCnt].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aMiniIcon[nCnt].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffMiniIcon, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureMiniIcon[g_aMiniIcon[nCnt].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt * 4, 2);
		}
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

//==================================================================================
//�A�C�R���̏��擾
//==================================================================================
MINIICON *GetMiniIcon(void)
{
	return &g_aMiniIcon[0];
}