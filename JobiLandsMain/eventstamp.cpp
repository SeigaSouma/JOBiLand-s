//=============================================================================
//
// �C�x���g�X�^���v���� [EventStamp.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "eventstamp.h"
#include "input.h"
#include "particle.h"
#include "sound.h"

#define WINDOW_X	(100.0f)
#define WINDOW_Y	(60.0f)
#define POS_Y		(100.0f)
#define DIS_Y		(140.0f)

//�O���[�o���ϐ��錾
const char *c_apFilenameEventStamp[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\tairyo_05.png",		//��ʔ���
	"data\\TEXTURE\\security_05.png",	//�x������
};

LPDIRECT3DTEXTURE9 g_pTextureEventStamp[(sizeof c_apFilenameEventStamp) / sizeof(*c_apFilenameEventStamp)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEventStamp = NULL;		//���_�o�b�t�@�ւ̃|�C���^
EVENTSTAMP g_EventStamp[MAX_EVENTSTAMP];			//����

//=============================================================================
//�C�x���g�X�^���v�̏���������
//=============================================================================					   
void InitEventStamp(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameEventStamp) / sizeof(*c_apFilenameEventStamp); CntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEventStamp[CntTex],
			&g_pTextureEventStamp[CntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EVENTSTAMP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEventStamp,
		NULL);

	//�e�ϐ�������
	for (int nCntStamp = 0; nCntStamp < MAX_EVENTSTAMP; nCntStamp++)
	{
		g_EventStamp[nCntStamp].bUse = false;		//�g�p���Ă��邩
		g_EventStamp[nCntStamp].nType = 0;		//���

		//UI���
		g_EventStamp[nCntStamp].aUI.pos = D3DXVECTOR3(1100.0f, POS_Y, 0.0f);	//���_���W
		g_EventStamp[nCntStamp].aUI.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���_�J���[
		g_EventStamp[nCntStamp].aUI.fWidth = WINDOW_X;		//��
		g_EventStamp[nCntStamp].aUI.fHeight = WINDOW_Y;		//����
		g_EventStamp[nCntStamp].aUI.nPatternAnim = 0;	//�A�j���[�V�����p�^�[��No
		g_EventStamp[nCntStamp].aUI.nCntAnim = 0;		//�A�j���[�V�����J�E���^�[
		g_EventStamp[nCntStamp].aUI.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		g_EventStamp[nCntStamp].aUI.fAngle = 0.0f;		//�p�x
		g_EventStamp[nCntStamp].aUI.fLength = 0.0f;		//�Ίp���̒���
		g_EventStamp[nCntStamp].aUI.nCount = 0;			//�G�J�E���g
		g_EventStamp[nCntStamp].aUI.bUse = false;			//�g�p���Ă��邩
		g_EventStamp[nCntStamp].aUI.aTexU = 0.0f;		//�e�N�X�`�����WU[�X�N���[���p]
		g_EventStamp[nCntStamp].aUI.aTexV = 0.0f;		//�e�N�X�`�����WV[�X�N���[���p]
	}

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEventStamp->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntStamp = 0; nCntStamp < MAX_EVENTSTAMP; nCntStamp++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x - g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y - g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x + g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y - g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x - g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y + g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x + g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y + g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = g_EventStamp[nCntStamp].aUI.col;
		pVtx[1].col = g_EventStamp[nCntStamp].aUI.col;
		pVtx[2].col = g_EventStamp[nCntStamp].aUI.col;
		pVtx[3].col = g_EventStamp[nCntStamp].aUI.col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEventStamp->Unlock();
}

//=============================================================================
//�C�x���g�X�^���v�̏I������
//=============================================================================
void UninitEventStamp(void)
{
	StopSound();

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameEventStamp) / sizeof(*c_apFilenameEventStamp); CntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureEventStamp[CntTex] != NULL)
		{
			g_pTextureEventStamp[CntTex]->Release();
			g_pTextureEventStamp[CntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEventStamp != NULL)
	{
		g_pVtxBuffEventStamp->Release();
		g_pVtxBuffEventStamp = NULL;
	}
}

//=============================================================================
//�C�x���g�X�^���v�̍X�V����
//=============================================================================
void UpdateEventStamp(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEventStamp->Lock(0, 0, (void**)&pVtx, 0);
	
	for (int nCntStamp = 0, nCntUse = 0; nCntStamp < MAX_EVENTSTAMP; nCntStamp++)
	{
		if (g_EventStamp[nCntStamp].bUse == true)
		{//�g�p����Ă�����

			//�ʒu�X�V
			g_EventStamp[nCntStamp].aUI.pos.y += (POS_Y + (nCntUse * DIS_Y) - g_EventStamp[nCntStamp].aUI.pos.y) * 0.25f;
			g_EventStamp[nCntStamp].aUI.fWidth += (WINDOW_X - g_EventStamp[nCntStamp].aUI.fWidth) * 0.25f;
			g_EventStamp[nCntStamp].aUI.fHeight += (WINDOW_Y - g_EventStamp[nCntStamp].aUI.fHeight) * 0.25f;

			//�J�E���g���Z
			g_EventStamp[nCntStamp].aUI.nCount++;

			if (g_EventStamp[nCntStamp].aUI.nCount == 10)
			{
				SetParticle(g_EventStamp[nCntStamp].aUI.pos, PARTICLE_TYPE_MANYTXT_MINI);

				PlaySound(SOUND_LABEL_SE_EVENTTEXT);
			}
			else if (g_EventStamp[nCntStamp].aUI.nCount > 10)
			{
				g_EventStamp[nCntStamp].aUI.nCount = 11;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x - g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y - g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x + g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y - g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x - g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y + g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_EventStamp[nCntStamp].aUI.pos.x + g_EventStamp[nCntStamp].aUI.fWidth, g_EventStamp[nCntStamp].aUI.pos.y + g_EventStamp[nCntStamp].aUI.fHeight, 0.0f);
		
			nCntUse++;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEventStamp->Unlock();

}

//=============================================================================
//�C�x���g�X�^���v�̕`�揈��
//=============================================================================
void DrawEventStamp(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEventStamp, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntStamp = 0; nCntStamp < MAX_EVENTSTAMP; nCntStamp++)
	{
		if (g_EventStamp[nCntStamp].bUse == true)
		{//�g�p����Ă�����

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEventStamp[g_EventStamp[nCntStamp].nType]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStamp * 4, 2);
		}
	}
}

//=============================================================================
//�C�x���g�X�^���v�̐ݒ菈��
//=============================================================================
void SetEventStamp(int nType)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEventStamp->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_EventStamp[nType].bUse == false)
	{//�g�p����Ă��Ȃ�������

		pVtx += 4 * nType;

		g_EventStamp[nType].bUse = true;
		g_EventStamp[nType].nType = nType;
		g_EventStamp[nType].aUI.pos = D3DXVECTOR3(1100.0f, 300.0f, 0.0f);
		g_EventStamp[nType].aUI.fWidth = WINDOW_X * 10.0f;
		g_EventStamp[nType].aUI.fHeight = WINDOW_Y * 10.0f;
		g_EventStamp[nType].aUI.nCount = 0;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_EventStamp[nType].aUI.pos.x - g_EventStamp[nType].aUI.fWidth, g_EventStamp[nType].aUI.pos.y - g_EventStamp[nType].aUI.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_EventStamp[nType].aUI.pos.x + g_EventStamp[nType].aUI.fWidth, g_EventStamp[nType].aUI.pos.y - g_EventStamp[nType].aUI.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_EventStamp[nType].aUI.pos.x - g_EventStamp[nType].aUI.fWidth, g_EventStamp[nType].aUI.pos.y + g_EventStamp[nType].aUI.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_EventStamp[nType].aUI.pos.x + g_EventStamp[nType].aUI.fWidth, g_EventStamp[nType].aUI.pos.y + g_EventStamp[nType].aUI.fHeight, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEventStamp->Unlock();
}

//=============================================================================
//�C�x���g�X�^���v�̏��擾
//=============================================================================
EVENTSTAMP *GetEventStamp(void)
{
	return &g_EventStamp[0];
}