//==============================================================
//
//�J�ڂ��Ȃ��t�F�[�h[nofade.cpp]
//Author:�匴�叫
//
//==============================================================
#include "nofade.h"
#include "game.h"

//�}�N����`
#define DEF_POSX_UP			(0.0f)		//pos.x�̏�̏����l
#define DEF_POSX_DOWN		(1280.0f)	//pos.x�̉��̏����l
#define DEF_POSY_UP			(0.0f)		//pos.y�̏�̏����l
#define DEF_POSY_DOWN		(720.0f)	//pos.y�̉��̏����l
#define DEF_POS_Z			(0.0f)		//pos.z�̏����l
#define COL_R				(0.0f)		//�ԐF�̐��l
#define COL_G				(0.0f)		//�ΐF�̐��l
#define COL_B				(0.0f)		//�F�̐��l
#define COL_A				(1.0f)		//�s�����x�̐��l
#define TEX_LEFT_X			(0.0f)		//�e�N�X�`���̍�x�̏����l
#define TEX_RIGHT_X			(1.0f)		//�e�N�X�`���̉Ex�̏����l
#define TEX_UP_Y			(0.0f)		//�e�N�X�`���̏�y�̏����l
#define TEX_DOWN_Y			(1.0f)		//�e�N�X�`���̉�y�̏����l

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffNoFade = NULL;		//���_�o�b�t�@�ւ̃|�C���^
NOFADE g_Nofade;										//�t�F�[�h�̏��

int g_uuu;

//==============================================================
//�t�F�[�h�̏���������
//==============================================================
void InitNoFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Nofade.nState = NOFADE_OUT;	//�t�F�[�h�C����Ԃ�
	g_Nofade.bUse = false;          //�g�p���Ă��Ȃ���Ԃɂ���
	g_Nofade.colorNoFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);		//�����|���S���i�s�����j�ɂ��Ă���
	g_Nofade.nCounter = 0;
	g_uuu = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffNoFade,
		NULL);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNoFade->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(DEF_POSX_UP, DEF_POSY_UP, DEF_POS_Z);
	pVtx[1].pos = D3DXVECTOR3(DEF_POSX_DOWN, DEF_POSY_UP, DEF_POS_Z);
	pVtx[2].pos = D3DXVECTOR3(DEF_POSX_UP, DEF_POSY_DOWN, DEF_POS_Z);
	pVtx[3].pos = D3DXVECTOR3(DEF_POSX_DOWN, DEF_POSY_DOWN, DEF_POS_Z);

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(COL_R, COL_G, COL_B, COL_A);
	pVtx[1].col = D3DXCOLOR(COL_R, COL_G, COL_B, COL_A);
	pVtx[2].col = D3DXCOLOR(COL_R, COL_G, COL_B, COL_A);
	pVtx[3].col = D3DXCOLOR(COL_R, COL_G, COL_B, COL_A);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(TEX_LEFT_X, TEX_UP_Y);
	pVtx[1].tex = D3DXVECTOR2(TEX_RIGHT_X, TEX_UP_Y);
	pVtx[2].tex = D3DXVECTOR2(TEX_LEFT_X, TEX_DOWN_Y);
	pVtx[3].tex = D3DXVECTOR2(TEX_RIGHT_X, TEX_DOWN_Y);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNoFade->Unlock();
}

//==============================================================
//�t�F�[�h�̏I������
//==============================================================
void UninitNoFade(void)
{
	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffNoFade != NULL)
	{
		g_pVtxBuffNoFade->Release();
		g_pVtxBuffNoFade = NULL;
	}
}

//==============================================================
//�t�F�[�h�̍X�V����
//==============================================================
void UpdateNoFade(void)
{
	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

	

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffNoFade->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Nofade.bUse == true)
	{
		g_Nofade.nCounter--;

		if (g_Nofade.nCounter <= 0)
		{
			if (g_Nofade.nState == NOFADE_IN)
			{//�t�F�[�h�C�����
				g_Nofade.colorNoFade.a -= 0.025f;		//�|���S���𓧖��ɂ��Ă���

				if (g_Nofade.colorNoFade.a <= 0.0f)
				{
					g_Nofade.colorNoFade.a = 0.0f;
					g_Nofade.nCounter = 0;
					g_Nofade.bUse = false;              //�g�p���Ă��Ȃ���Ԃɂ���
				}
			}
			else if (g_Nofade.nState == NOFADE_OUT)
			{//�t�F�[�h�A�E�g���

				g_Nofade.colorNoFade.a += 0.025f;		//�|���S����s�����ɂ��Ă���

				if (g_Nofade.colorNoFade.a >= 1.0f)
				{
					g_Nofade.colorNoFade.a = 1.0f;
					g_Nofade.nState = NOFADE_IN;		//�t�F�[�h�C����Ԃ�
					//SetGameState(GAMESTATE_BEFOREEND, 0);
				}
			}
		}
		


		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_Nofade.colorNoFade.a);
		pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_Nofade.colorNoFade.a);
		pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_Nofade.colorNoFade.a);
		pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, g_Nofade.colorNoFade.a);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffNoFade->Unlock();
}

//==============================================================
//�t�F�[�h�̕`�揈��
//==============================================================
void DrawNoFade(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//�f�o�C�X�ւ̃|�C���^

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffNoFade, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_Nofade.bUse == true)
	{//�g�p����Ă�����

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			0,												//�v���~�e�B�u�i�|���S���̐��j
			2);												//�`�悷��v���~�e�B�u��
	}
}

//==============================================================
//�t�F�[�h�̐ݒ菈��
//==============================================================
void SetNoFade(int nCounter)
{
	if (g_Nofade.bUse == false)
	{//�g�p����Ă��Ȃ�������

		g_Nofade.nState = NOFADE_OUT;							//�t�F�[�h�A�E�g���
		g_Nofade.bUse = true;                                   //�g�p���Ă����Ԃɂ���
		g_Nofade.colorNoFade = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//�����|���S���i�����j�ɂ��Ă���
		g_Nofade.nCounter = nCounter;
	}
}

//==============================================================
//�t�F�[�h�̎擾����
//==============================================================
NOFADE GetNoFade(void)
{
	return g_Nofade;
}