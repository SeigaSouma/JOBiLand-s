//=============================================================================
//
// �Ö��Q�[�W���� [hypnosis_gauge.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "hypnosis_gauge.h"
#include "player.h"
#include "camera.h"
#include "mob.h"

//�}�N����`
#define MAX_WIDTH	(25.0f)	//����
#define MAX_HEIGHT	(5.0f)		//�c��
#define MAX_TOP		(4)
#define MAX_HYP		(10)		//�Ö��x�̍ő�l

//�v���g�^�C�v�錾
void GaugeDecrement(int nCntGauge);
void ChangeColorHypnosisGauge(int nCntGauge);

//�O���[�o���ϐ��錾
const char *c_apFilenameHypnosisGauge[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\hypnosis_fram.png",
};
LPDIRECT3DTEXTURE9 g_pTextureHypnosisGauge[(sizeof c_apFilenameHypnosisGauge) / sizeof(*c_apFilenameHypnosisGauge)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffHypnosisGauge = NULL;	//���_�o�b�t�@�ւ̃|�C���^
HypnosisGauge g_aHypnosisGauge[MAX_MOB];				//HP�Q�[�W�̏��

//==================================================================================
//�Ö��x�Q�[�W�̏���������
//==================================================================================
void InitHypnosisGauge(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameHypnosisGauge) / sizeof(*c_apFilenameHypnosisGauge); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameHypnosisGauge[nCntTex],
			&g_pTextureHypnosisGauge[nCntTex]);
	}

	//�e�v�f������
	for (int nCntGauge = 0; nCntGauge < MAX_MOB; nCntGauge++)
	{//�Q�[�W��������

		g_aHypnosisGauge[nCntGauge].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aHypnosisGauge[nCntGauge].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aHypnosisGauge[nCntGauge].mtxWorld);
		g_aHypnosisGauge[nCntGauge].bUse = false;				//�g�p���Ă��邩
		g_aHypnosisGauge[nCntGauge].bDisp = false;				//�`�悷�邩
		g_aHypnosisGauge[nCntGauge].nMaxHyp = MAX_HYP;			//�ő�l
		g_aHypnosisGauge[nCntGauge].nHypnosis = 0;			//���ݒn

		for (int nCntFrame = 0; nCntFrame < HYPNISISGAUGE_VTX_MAX; nCntFrame++)
		{//�g��������

			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);		//�F
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth = MAX_WIDTH;		//����
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fMaxWidth = MAX_WIDTH;	//���ő啝
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight = MAX_HEIGHT;		//�c��
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidthDest = MAX_WIDTH;	//�ڕW�̕�
			g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidthDiff = 0.0f;	//���̍���
		}
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * (MAX_MOB * HYPNISISGAUGE_VTX_MAX),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffHypnosisGauge,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHypnosisGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_MOB; nCntGauge++)
	{
		for (int nCntFrame = 0; nCntFrame < HYPNISISGAUGE_VTX_MAX; nCntFrame++)
		{//�g��������

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth, g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth, g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth, -g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth, -g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);

			//�@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
			pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			//���_�J���[�̐ݒ�
			switch (nCntFrame)
			{
			case HYPNISISGAUGE_VTX_BLACK:		//���n

				pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
				break;

			case HYPNISISGAUGE_VTX_PINK:		//�ϓ�����Q�[�W

				pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);
				break;

			case HYPNISISGAUGE_VTX_FRAM:		//�t���[��

				pVtx[0].col =
				pVtx[1].col =
				pVtx[2].col =
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				break;
			}

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

			pVtx += MAX_TOP;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHypnosisGauge->Unlock();

	
}

//==================================================================================
//�Ö��x�Q�[�W�̏I������
//==================================================================================
void UninitHypnosisGauge(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameHypnosisGauge) / sizeof(*c_apFilenameHypnosisGauge); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureHypnosisGauge[nCntTex] != NULL)
		{
			g_pTextureHypnosisGauge[nCntTex]->Release();
			g_pTextureHypnosisGauge[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffHypnosisGauge != NULL)
	{
		g_pVtxBuffHypnosisGauge->Release();
		g_pVtxBuffHypnosisGauge = NULL;
	}
}

//==================================================================================
//�Ö��x�Q�[�W�̍X�V����
//==================================================================================
void UpdateHypnosisGauge(void)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHypnosisGauge->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_MOB; nCntGauge++)
	{
		if (g_aHypnosisGauge[nCntGauge].bUse == true && g_aHypnosisGauge[nCntGauge].bDisp == true)
		{//�g�p����Ă�����

			//�Q�[�W��������
			GaugeDecrement(nCntGauge);

			//�F�X�V
			//ChangeColorHypnosisGauge(nCntGauge);

			//���_���W�̐ݒ�
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 0].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth), g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fHeight, 0.0f);
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 1].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth), g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fHeight, 0.0f);
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 2].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth), -g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fHeight, 0.0f);
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 3].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth), -g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fHeight, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 0].col =
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 1].col =
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 2].col =
			pVtx[HYPNISISGAUGE_VTX_PINK * 4 + 3].col = g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col;
		}

		pVtx += HYPNISISGAUGE_VTX_MAX * 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHypnosisGauge->Unlock();
}

//==================================================================================
//�Ö��x�Q�[�W�̌�������
//==================================================================================
void GaugeDecrement(int nCntGauge)
{
	//�����ŏ��X�Ɍ��炵�Ă���
	g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth +=
		(g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidthDest - g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth) * 0.025f;
}

//==================================================================================
//�Ö��x�Q�[�W�̐F�X�V
//==================================================================================
void ChangeColorHypnosisGauge(int nCntGauge)
{

	if (
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth <= 0.95f &&
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth > 0.6f)
	{//HP�Q�[�W8��
		
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.2f, 0.6f, 0.2f, 1.0f);
	}
	else if (
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth <= 0.6f &&
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth > 0.4f)
	{//HP�Q�[�W6��

		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.8f, 0.7f, 0.2f, 1.0f);
	}
	else if (
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth <= 0.4f &&
		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth > 0.2f)
	{//HP�Q�[�W4��

		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.8f, 0.5f, 0.2f, 1.0f);
	}
	else if (g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidth / g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth <= 0.2f)
	{//�댯�G���A

		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.8f, 0.2f, 0.2f, 1.0f);
	}
	else
	{//���^��

		g_aHypnosisGauge[nCntGauge].aFrame[HYPNISISGAUGE_VTX_PINK].col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}

}

//==================================================================================
//�Ö��x�Q�[�W�̈ʒu�X�V
//==================================================================================
void SetPositionHypnosisGauge(int nIdxGauge, D3DXVECTOR3 pos, int nHypnosis)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	if (g_aHypnosisGauge[nIdxGauge].bUse == true && g_aHypnosisGauge[nIdxGauge].bDisp == true)
	{//�g�p����Ă�����

			g_aHypnosisGauge[nIdxGauge].pos.x = pos.x;
			g_aHypnosisGauge[nIdxGauge].pos.y = pos.y + 70.0f;
			g_aHypnosisGauge[nIdxGauge].pos.z = pos.z;
			g_aHypnosisGauge[nIdxGauge].nHypnosis = nHypnosis;

			//�ڕW�̕��ݒ�
			g_aHypnosisGauge[nIdxGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fWidthDest =
				g_aHypnosisGauge[nIdxGauge].aFrame[HYPNISISGAUGE_VTX_PINK].fMaxWidth * ((float)g_aHypnosisGauge[nIdxGauge].nHypnosis / (float)g_aHypnosisGauge[nIdxGauge].nMaxHyp);
	}
}

//==================================================================================
//�Ö��x�Q�[�W�̕`�揈��
//==================================================================================
void DrawHypnosisGauge(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_MOB; nCntGauge++)
	{
		if (g_aHypnosisGauge[nCntGauge].bUse == true && g_aHypnosisGauge[nCntGauge].bDisp == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aHypnosisGauge[nCntGauge].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aHypnosisGauge[nCntGauge].mtxWorld, NULL, &mtxView);	//�t�z������߂�

			//��ŏ�����
			g_aHypnosisGauge[nCntGauge].mtxWorld._21 = 0.0f;
			g_aHypnosisGauge[nCntGauge].mtxWorld._23 = 0.0f;
			g_aHypnosisGauge[nCntGauge].mtxWorld._24 = 0.0f;

			g_aHypnosisGauge[nCntGauge].mtxWorld._41 = 0.0f;
			g_aHypnosisGauge[nCntGauge].mtxWorld._42 = 0.0f;
			g_aHypnosisGauge[nCntGauge].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aHypnosisGauge[nCntGauge].pos.x, g_aHypnosisGauge[nCntGauge].pos.y, g_aHypnosisGauge[nCntGauge].pos.z);
			D3DXMatrixMultiply(&g_aHypnosisGauge[nCntGauge].mtxWorld, &g_aHypnosisGauge[nCntGauge].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aHypnosisGauge[nCntGauge].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffHypnosisGauge, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			for (int nCntFrame = 0; nCntFrame < HYPNISISGAUGE_VTX_MAX; nCntFrame++)
			{//�g��

				//�e�N�X�`���̐ݒ�
				switch (nCntFrame)
				{
				case HYPNISISGAUGE_VTX_FRAM:
					pDevice->SetTexture(0, g_pTextureHypnosisGauge[0]);
					break;

				default:
					pDevice->SetTexture(0, NULL);
					break;
				}

				//HP�Q�[�W�̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntGauge * 4 * HYPNISISGAUGE_VTX_MAX) + (nCntFrame * 4), 2);

			}
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==================================================================================
//�Ö��x�Q�[�W�̐ݒ菈��
//==================================================================================
int SetHypnosisGauge(float fWidth, float fHeight, int nMaxHyp)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffHypnosisGauge->Lock(0, 0, (void**)&pVtx, 0);

	int nCntGauge = -1;
	int nn = 0;

	for (int nCount = 0; nCount < MAX_MOB; nCount++)
	{
		if (g_aHypnosisGauge[nCount].bUse == false)
		{//�g�p���Ă��Ȃ�������

			for (int nCntFrame = 0; nCntFrame < HYPNISISGAUGE_VTX_MAX; nCntFrame++)
			{//�g��

				nCntGauge = nCount;		//�C���f�b�N�X�ԍ��擾
				g_aHypnosisGauge[nCntGauge].bUse = true;		//�g�p���Ă����Ԃɂ���
				g_aHypnosisGauge[nCntGauge].bDisp = true;		//�`�悵�Ă����Ԃɂ���
				g_aHypnosisGauge[nCntGauge].nMaxHyp = MAX_HYP;	//�ő�l�ݒ�

				g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight = fHeight;

				if (nCntFrame == HYPNISISGAUGE_VTX_PINK)
				{//�s���N�̎�
					g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth = 0;
					g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fMaxWidth = fWidth;

				}
				else
				{
					g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth = fWidth;
					g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fMaxWidth = g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth;
				}



				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth), g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
				pVtx[1].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth), g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
				pVtx[2].pos = D3DXVECTOR3(-g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth), -g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3(g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth - (MAX_WIDTH - g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fWidth), -g_aHypnosisGauge[nCntGauge].aFrame[nCntFrame].fHeight, 0.0f);

				pVtx += MAX_TOP;
			}
			break;
		}

		pVtx += HYPNISISGAUGE_VTX_MAX * 4;
	
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffHypnosisGauge->Unlock();

	return nCntGauge;
}

//==================================================================================
//�Ö��x�Q�[�W�̏��擾
//==================================================================================
HypnosisGauge *GetHypnosisGauge(void)
{
	return &g_aHypnosisGauge[0];
}

//==================================================================================
//�Ö��x�Q�[�W���g�p����ĂȂ���Ԃɂ��鏈��
//==================================================================================
void GetHypnosisUse(int nIdxGauge, bool bUse)
{
	g_aHypnosisGauge[nIdxGauge].bUse = bUse;
}