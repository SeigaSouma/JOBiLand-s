//=============================================================================
//
// �㉺�̘g���� [updown_fram.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input.h"
#include "updown_fram.h"
#include "sound.h"

#define MOVE_TIME	(20)
#define POS_X		(640.0f)
#define POS_Y		(50.0f)

//�v���g�^�C�v�錾
void UpdateInUpDownFram(void);
void UpdateOutUpDownFram(void);

//�O���[�o���ϐ��錾
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffUpDownFram = NULL;		//���_�o�b�t�@�ւ̃|�C���^
UPDOWN_FRAM g_aUpDownFram;		//�㉺�̘g

//=============================================================================
//�㉺�̘g����������
//=============================================================================					   
void InitUpDownFram(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * FRAMTYPE_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffUpDownFram,
		NULL);

	//�e�v�f������
	g_aUpDownFram.pos[FRAMTYPE_UP] = D3DXVECTOR3(POS_X, -POS_Y, 0.0f);	//�ʒu
	g_aUpDownFram.pos[FRAMTYPE_DOWN] = D3DXVECTOR3(POS_X, SCREEN_HEIGHT + POS_Y, 0.0f);	//�ʒu
	g_aUpDownFram.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//�F
	g_aUpDownFram.nState = UPDOWN_FRAM_STATE_IN;	//�����Ă����
	g_aUpDownFram.bUse = false;			//�g�p���Ă��邩
	g_aUpDownFram.bEnd = false;			//�������Ă��邩

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUpDownFram->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntFram = 0; nCntFram < FRAMTYPE_MAX; nCntFram++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = g_aUpDownFram.col;
		pVtx[1].col = g_aUpDownFram.col;
		pVtx[2].col = g_aUpDownFram.col;
		pVtx[3].col = g_aUpDownFram.col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUpDownFram->Unlock();
}

//=============================================================================
//�㉺�̘g�I������
//=============================================================================
void UninitUpDownFram(void)
{

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffUpDownFram != NULL)
	{
		g_pVtxBuffUpDownFram->Release();
		g_pVtxBuffUpDownFram = NULL;
	}
}

//=============================================================================
//�㉺�̘g�X�V����
//=============================================================================
void UpdateUpDownFram(void)
{

	switch (g_aUpDownFram.nState)
	{
	case UPDOWN_FRAM_STATE_IN:

		//�㉺�̘g�i��ł�����
		UpdateInUpDownFram();
		break;

	case UPDOWN_FRAM_STATE_OUT:

		//�㉺�̘g�͂��Ă�����
		UpdateOutUpDownFram();
		break;
	}

}

//=============================================================================
//�㉺�̘g�i��ł�����
//=============================================================================
void UpdateInUpDownFram(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUpDownFram->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aUpDownFram.bUse == true)
	{//�g�p����Ă�����

		//�ʒu�X�V
		if (g_aUpDownFram.pos[FRAMTYPE_UP].y >= POS_Y)
		{//�K��l�𒴂�����

			g_aUpDownFram.pos[FRAMTYPE_UP].y = POS_Y;
			g_aUpDownFram.pos[FRAMTYPE_DOWN].y = SCREEN_HEIGHT - POS_Y;
			g_aUpDownFram.bEnd = true;
		}
		else
		{
			//�㉺�ł͂���
			g_aUpDownFram.pos[FRAMTYPE_UP].y += 2.0f;
			g_aUpDownFram.pos[FRAMTYPE_DOWN].y -= 2.0f;
		}

		//�s�����x�X�V
		if (g_aUpDownFram.col.a >= 1.0f)
		{
			g_aUpDownFram.col.a = 1.0f;
		}
		else
		{
			g_aUpDownFram.col.a += 0.025f;
		}

		for (int nCntFram = 0; nCntFram < FRAMTYPE_MAX; nCntFram++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);

			//���_�J���[
			pVtx[0].col = g_aUpDownFram.col;
			pVtx[1].col = g_aUpDownFram.col;
			pVtx[2].col = g_aUpDownFram.col;
			pVtx[3].col = g_aUpDownFram.col;

			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUpDownFram->Unlock();
}

//=============================================================================
//�㉺�̘g�͂��Ă�����
//=============================================================================
void UpdateOutUpDownFram(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUpDownFram->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aUpDownFram.bUse == true)
	{//�g�p����Ă�����

	 //�ʒu�X�V
		if (g_aUpDownFram.pos[FRAMTYPE_UP].y <= -POS_Y)
		{//�K��l�𒴂�����

			g_aUpDownFram.pos[FRAMTYPE_UP].y = -POS_Y;
			g_aUpDownFram.pos[FRAMTYPE_DOWN].y = SCREEN_HEIGHT + POS_Y;
		}
		else
		{
			//�㉺�����Ă�
			g_aUpDownFram.pos[FRAMTYPE_UP].y -= 2.0f;
			g_aUpDownFram.pos[FRAMTYPE_DOWN].y += 2.0f;
		}

		//�s�����x�X�V
		if (g_aUpDownFram.col.a <= 0.0f)
		{
			g_aUpDownFram.col.a = 0.0f;
			g_aUpDownFram.bUse = false;
		}
		else
		{
			g_aUpDownFram.col.a -= 0.025f;
		}

		for (int nCntFram = 0; nCntFram < FRAMTYPE_MAX; nCntFram++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y - POS_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x - POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aUpDownFram.pos[nCntFram].x + POS_X, g_aUpDownFram.pos[nCntFram].y + POS_Y, 0.0f);

			//���_�J���[
			pVtx[0].col = g_aUpDownFram.col;
			pVtx[1].col = g_aUpDownFram.col;
			pVtx[2].col = g_aUpDownFram.col;
			pVtx[3].col = g_aUpDownFram.col;

			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUpDownFram->Unlock();
}

//=============================================================================
//�㉺�̘g�`�揈��
//=============================================================================
void DrawUpDownFram(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffUpDownFram, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aUpDownFram.bUse == true)
	{
		for (int nCntFram = 0; nCntFram < FRAMTYPE_MAX; nCntFram++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, NULL);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntFram * 4, 2);
		}
	}
}

//=============================================================================
//�㉺�̘g�ݒ菈��
//=============================================================================
void SetUpDownFram(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffUpDownFram->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aUpDownFram.bUse == false)
	{//�g�p����Ă��Ȃ�������

		g_aUpDownFram.pos[FRAMTYPE_UP] = D3DXVECTOR3(POS_X, -POS_Y, 0.0f);	//�ʒu
		g_aUpDownFram.pos[FRAMTYPE_DOWN] = D3DXVECTOR3(POS_X, SCREEN_HEIGHT + POS_Y, 0.0f);	//�ʒu
		g_aUpDownFram.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);		//�F
		g_aUpDownFram.nState = UPDOWN_FRAM_STATE_IN;	//�����Ă����
		g_aUpDownFram.bUse = true;	//�g�p���Ă����Ԃɂ���
		g_aUpDownFram.bEnd = false;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffUpDownFram->Unlock();
}

//=============================================================================
//�㉺�̘g���擾
//=============================================================================
UPDOWN_FRAM *GetUpDownFram(void)
{
	return &g_aUpDownFram;
}