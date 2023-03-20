//=============================================================================
//
// ��ʔ�����UI���� [manyMORS.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input.h"
#include "light.h"
#include "manyMORS.h"
#include "2D_effect.h"
#include "particle.h"
#include "camera.h"
#include "eventstamp.h"
#include "sound.h"

#define NUM_PLACE	(4)
#define MOVE_TIME	(20)
#define TEXT_X	(100.0f)
#define TEXT_Y	(100.0f)

//�O���[�o���ϐ��錾
const char *c_apFilenameManyMORS[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\security_01.png",	//�x
	"data\\TEXTURE\\security_02.png",	//��
	"data\\TEXTURE\\security_03.png",	//��
	"data\\TEXTURE\\security_04.png",	//��
};

LPDIRECT3DTEXTURE9 g_pTextureManyMORS[(sizeof c_apFilenameManyMORS) / sizeof(*c_apFilenameManyMORS)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManyMORS = NULL;		//���_�o�b�t�@�ւ̃|�C���^
MANY_MORS g_aManyMORS[NUM_PLACE];					//����
int g_nCntAfterManyMORS;

//=============================================================================
//��ʔ�����UI����������
//=============================================================================					   
void InitManyMORS(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameManyMORS) / sizeof(*c_apFilenameManyMORS); CntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameManyMORS[CntTex],
			&g_pTextureManyMORS[CntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffManyMORS,
		NULL);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		g_aManyMORS[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);	//�ʒu
		g_aManyMORS[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F
		g_aManyMORS[nCntTxt].nType = 0;			//���
		g_aManyMORS[nCntTxt].fWidth = 0.0f;		//��
		g_aManyMORS[nCntTxt].fHeight = 0.0f;		//����
		g_aManyMORS[nCntTxt].nCntZoom = 0;		//�傫���Ȃ鎞��
		g_aManyMORS[nCntTxt].bUse = false;			//�g�p���Ă��邩
	}
	g_nCntAfterManyMORS = 0;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffManyMORS->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - TEXT_X, g_aManyMORS[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + TEXT_X, g_aManyMORS[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - TEXT_X, g_aManyMORS[nCntTxt].pos.y + TEXT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + TEXT_X, g_aManyMORS[nCntTxt].pos.y + TEXT_Y, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffManyMORS->Unlock();
}

//=============================================================================
//��ʔ�����UI�I������
//=============================================================================
void UninitManyMORS(void)
{

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameManyMORS) / sizeof(*c_apFilenameManyMORS); CntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureManyMORS[CntTex] != NULL)
		{
			g_pTextureManyMORS[CntTex]->Release();
			g_pTextureManyMORS[CntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffManyMORS != NULL)
	{
		g_pVtxBuffManyMORS->Release();
		g_pVtxBuffManyMORS = NULL;
	}
}

//=============================================================================
//��ʔ�����UI�X�V����
//=============================================================================
void UpdateManyMORS(void)
{

	//�C�x���g�X�^���v�̏��擾
	EVENTSTAMP *pEventStamp = GetEventStamp();

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffManyMORS->Lock(0, 0, (void**)&pVtx, 0);

	int nCntUse = 0;
	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyMORS[nCntTxt].bUse == true)
		{//�g�p����Ă�����

			//�g�p�J�E���g�����Z
			nCntUse++;

			//�g��̎��ԉ��Z
			g_aManyMORS[nCntTxt].nCntZoom++;

			if (g_aManyMORS[nCntTxt].nCntZoom <= MOVE_TIME)
			{
				g_aManyMORS[nCntTxt].fWidth += (TEXT_X - g_aManyMORS[nCntTxt].fWidth) * 0.25f;
				g_aManyMORS[nCntTxt].fHeight += (TEXT_Y - g_aManyMORS[nCntTxt].fHeight) * 0.25f;
				g_aManyMORS[nCntTxt].pos.y += (200.0f - g_aManyMORS[nCntTxt].pos.y) * 0.25f;
			}

			if (g_aManyMORS[nCntTxt].nCntZoom == 10)
			{//���������ȂƂ��Ƀp�[�e�B�N��

				SetParticle(g_aManyMORS[nCntTxt].pos, PARTICLE_TYPE_MANYTXT_MORS);

				if (nCntTxt == 0)
				{
					PlaySound(SOUND_LABEL_SE_KYUWAWA);
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y - g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y - g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y + g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y + g_aManyMORS[nCntTxt].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	if (nCntUse >= NUM_PLACE - 1)
	{//�S���g�p����Ă�����
		
		//�]�C�̎��ԉ��Z
		g_nCntAfterManyMORS++;

		for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
		{
			if (g_nCntAfterManyMORS >= 150)
			{//�]�C�ŏ���

				g_aManyMORS[nCntTxt].fWidth += (10.0f - g_aManyMORS[nCntTxt].fWidth) * 0.25f;
				g_aManyMORS[nCntTxt].fHeight += (10.0f - g_aManyMORS[nCntTxt].fHeight) * 0.25f;
				//g_aManyMORS[nCntTxt].pos.y += (100.0f - g_aManyMORS[nCntTxt].pos.y) * 0.25f;
				g_aManyMORS[nCntTxt].pos.x += (1100.0f - g_aManyMORS[nCntTxt].pos.x) * 0.25f;

				if (g_aManyMORS[nCntTxt].fWidth < 12.0f)
				{
					g_aManyMORS[nCntTxt].bUse = false;

					if (pEventStamp[EVENTTYPE_MORS].bUse == false)
					{
						SetEventStamp(EVENTTYPE_MORS);
					}
				}
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffManyMORS->Unlock();

}
//=============================================================================
//��ʔ�����UI�`�揈��
//=============================================================================
void DrawManyMORS(void)
{

	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffManyMORS, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyMORS[nCntTxt].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureManyMORS[nCntTxt]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTxt * 4, 2);
		}
	}
}

//=============================================================================
//��ʔ�����UI�ݒ菈��
//=============================================================================
void SetManyMORS(int nType)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffManyMORS->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyMORS[nCntTxt].bUse == false)
		{//�g�p����Ă��Ȃ�������

			g_aManyMORS[nCntTxt].bUse = true;
			g_aManyMORS[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);
			g_aManyMORS[nCntTxt].fWidth = 1500.0f;
			g_aManyMORS[nCntTxt].fHeight = 1500.0f;
			g_aManyMORS[nCntTxt].nCntZoom = 0;
			g_nCntAfterManyMORS = 0;
			g_aManyMORS[nCntTxt].nType = nType;	//�C�x���g�̎��

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y - g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y - g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x - g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y + g_aManyMORS[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aManyMORS[nCntTxt].pos.x + g_aManyMORS[nCntTxt].fWidth, g_aManyMORS[nCntTxt].pos.y + g_aManyMORS[nCntTxt].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffManyMORS->Unlock();
}

//=============================================================================
//��ʔ�����UI���擾
//=============================================================================
MANY_MORS *GetManyMORS(void)
{
	return &g_aManyMORS[0];
}