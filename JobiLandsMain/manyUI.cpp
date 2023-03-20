//=============================================================================
//
// ��ʔ�����UI���� [manyUI.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input.h"
#include "light.h"
#include "manyUI.h"
#include "2D_effect.h"
#include "particle.h"
#include "camera.h"
#include "sound.h"
#include "eventstamp.h"

#define NUM_PLACE	(4)
#define MOVE_TIME	(20)
#define TEXT_X	(125.0f)
#define TEXT_Y	(125.0f)

//�O���[�o���ϐ��錾
const char *c_apFilenameManyUI[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\tairyo_01.png",	//��
	"data\\TEXTURE\\tairyo_02.png",	//��
	"data\\TEXTURE\\tairyo_03.png",	//��
	"data\\TEXTURE\\tairyo_04.png",	//��
};

LPDIRECT3DTEXTURE9 g_pTextureManyUI[(sizeof c_apFilenameManyUI) / sizeof(*c_apFilenameManyUI)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffManyUI = NULL;		//���_�o�b�t�@�ւ̃|�C���^
MANY_UI g_aManyUI[NUM_PLACE];					//����
int g_nCntAfterManyUI;

//=============================================================================
//��ʔ�����UI����������
//=============================================================================					   
void InitManyUI(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameManyUI) / sizeof(*c_apFilenameManyUI); CntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameManyUI[CntTex],
			&g_pTextureManyUI[CntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffManyUI,
		NULL);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		g_aManyUI[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);	//�ʒu
		g_aManyUI[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F
		g_aManyUI[nCntTxt].nType = 0;			//���
		g_aManyUI[nCntTxt].fWidth = 0.0f;		//��
		g_aManyUI[nCntTxt].fHeight = 0.0f;		//����
		g_aManyUI[nCntTxt].nCntZoom = 0;		//�傫���Ȃ鎞��
		g_aManyUI[nCntTxt].bUse = false;			//�g�p���Ă��邩
	}
	g_nCntAfterManyUI = 0;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffManyUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - TEXT_X, g_aManyUI[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + TEXT_X, g_aManyUI[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - TEXT_X, g_aManyUI[nCntTxt].pos.y + TEXT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + TEXT_X, g_aManyUI[nCntTxt].pos.y + TEXT_Y, 0.0f);

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
	g_pVtxBuffManyUI->Unlock();
}

//=============================================================================
//��ʔ�����UI�I������
//=============================================================================
void UninitManyUI(void)
{

	//SE���I��
	StopSound();

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameManyUI) / sizeof(*c_apFilenameManyUI); CntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureManyUI[CntTex] != NULL)
		{
			g_pTextureManyUI[CntTex]->Release();
			g_pTextureManyUI[CntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffManyUI != NULL)
	{
		g_pVtxBuffManyUI->Release();
		g_pVtxBuffManyUI = NULL;
	}
}

//=============================================================================
//��ʔ�����UI�X�V����
//=============================================================================
void UpdateManyUI(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	VERTEX_2D *pVtx;

	//�C�x���g�X�^���v�̏��擾
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffManyUI->Lock(0, 0, (void**)&pVtx, 0);

	int nCntUse = 0;
	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyUI[nCntTxt].bUse == true)
		{//�g�p����Ă�����

			//�g�p�J�E���g�����Z
			nCntUse++;

			//�g��̎��ԉ��Z
			g_aManyUI[nCntTxt].nCntZoom++;

			if (g_aManyUI[nCntTxt].nCntZoom <= MOVE_TIME)
			{
				g_aManyUI[nCntTxt].fWidth += (TEXT_X - g_aManyUI[nCntTxt].fWidth) * 0.25f;
				g_aManyUI[nCntTxt].fHeight += (TEXT_Y - g_aManyUI[nCntTxt].fHeight) * 0.25f;
				g_aManyUI[nCntTxt].pos.y += (360.0f - g_aManyUI[nCntTxt].pos.y) * 0.25f;
			}
			else if(g_aManyUI[nCntTxt].nCntZoom == MOVE_TIME + 1)
			{//�ړ����I�������

				//���̕����̐ݒ�
				SetManyUI(0);
			}

			if (g_aManyUI[nCntTxt].nCntZoom == 10)
			{//���������ȂƂ��Ƀp�[�e�B�N��

				SetParticle(g_aManyUI[nCntTxt].pos, PARTICLE_TYPE_MANYTXT);

				PlaySound(SOUND_LABEL_SE_EVENTTEXT);
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y - g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y - g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y + g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y + g_aManyUI[nCntTxt].fHeight, 0.0f);
		}

		pVtx += 4;
	}

	if (nCntUse >= NUM_PLACE - 1)
	{//�S���g�p����Ă�����
		
		if (g_nCntAfterManyUI == 60)
		{
			//���A�c�L�^�[�I
			PlaySound(SOUND_LABEL_SE_GEKIATSU);
		}

		//�]�C�̎��ԉ��Z
		g_nCntAfterManyUI++;

		for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
		{
			if (g_nCntAfterManyUI >= 150)
			{//�]�C�ŏ���

				g_aManyUI[nCntTxt].fWidth += (10.0f - g_aManyUI[nCntTxt].fWidth) * 0.25f;
				g_aManyUI[nCntTxt].fHeight += (10.0f - g_aManyUI[nCntTxt].fHeight) * 0.25f;
				//g_aManyUI[nCntTxt].pos.y += (100.0f - g_aManyUI[nCntTxt].pos.y) * 0.25f;
				g_aManyUI[nCntTxt].pos.x += (1100.0f - g_aManyUI[nCntTxt].pos.x) * 0.25f;

				if (g_aManyUI[nCntTxt].fWidth < 12.0f)
				{
					//�ڕW�̒n�_�ݒ�
					pCamera->nMoveStep = CAMERAMOVE_UP;
					g_aManyUI[nCntTxt].bUse = false;

					if (pEventStamp[EVENTTYPE_MANYMOB].bUse == false)
					{
						SetEventStamp(EVENTTYPE_MANYMOB);
					}
				}
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffManyUI->Unlock();

}
//=============================================================================
//��ʔ�����UI�`�揈��
//=============================================================================
void DrawManyUI(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffManyUI, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyUI[nCntTxt].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureManyUI[nCntTxt]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTxt * 4, 2);
		}
	}
}

//=============================================================================
//��ʔ�����UI�ݒ菈��
//=============================================================================
void SetManyUI(int nType)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffManyUI->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aManyUI[nCntTxt].bUse == false)
		{//�g�p����Ă��Ȃ�������

			g_aManyUI[nCntTxt].bUse = true;
			g_aManyUI[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);
			g_aManyUI[nCntTxt].fWidth = 1500.0f;
			g_aManyUI[nCntTxt].fHeight = 1500.0f;
			g_aManyUI[nCntTxt].nCntZoom = 0;
			g_nCntAfterManyUI = 0;
			g_aManyUI[nCntTxt].nType = nType;	//�C�x���g�̎��

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y - g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y - g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x - g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y + g_aManyUI[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aManyUI[nCntTxt].pos.x + g_aManyUI[nCntTxt].fWidth, g_aManyUI[nCntTxt].pos.y + g_aManyUI[nCntTxt].fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffManyUI->Unlock();
}

//=============================================================================
//��ʔ�����UI���擾
//=============================================================================
MANY_UI *GetManyUI(void)
{
	return &g_aManyUI[0];
}