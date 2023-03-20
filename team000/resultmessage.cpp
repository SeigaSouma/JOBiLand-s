//=============================================================================
//
// ���ʂ̃��b�Z�[�W���� [resultmessage.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input.h"
#include "light.h"
#include "resultmessage.h"
#include "2D_effect.h"
#include "particle.h"
#include "sound.h"

#define NUM_PLACE	(2)
#define MOVE_TIME	(20)
#define TEXT_X	(360.0f * 1.2f)
#define TEXT_Y	(130.0f * 1.2f)

//�O���[�o���ϐ��錾
const char *c_apFilenameResultMessage[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\resultclear_01.png",	//���t
	"data\\TEXTURE\\resultclear_02.png",	//����
	"data\\TEXTURE\\resultfailed_01.png",	//���t
	"data\\TEXTURE\\resultfailed_02.png",	//���s
};

LPDIRECT3DTEXTURE9 g_pTextureResultMessage[(sizeof c_apFilenameResultMessage) / sizeof(*c_apFilenameResultMessage)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResultMessage = NULL;		//���_�o�b�t�@�ւ̃|�C���^
RESULT_MESSAGE g_aResultMessage[NUM_PLACE];					//����
int g_nCntAfterResultMessage;

//=============================================================================
//���ʂ̃��b�Z�[�W����������
//=============================================================================					   
void InitResultMessage(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameResultMessage) / sizeof(*c_apFilenameResultMessage); CntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameResultMessage[CntTex],
			&g_pTextureResultMessage[CntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResultMessage,
		NULL);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		g_aResultMessage[nCntTxt].pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//�ʒu
		g_aResultMessage[nCntTxt].posOrigin = g_aResultMessage[nCntTxt].pos;	//�ʒu
		g_aResultMessage[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F
		g_aResultMessage[nCntTxt].nType = 0;			//���
		g_aResultMessage[nCntTxt].fWidth = 0.0f;		//��
		g_aResultMessage[nCntTxt].fHeight = 0.0f;		//����
		g_aResultMessage[nCntTxt].nCntZoom = 0;		//�傫���Ȃ鎞��
		g_aResultMessage[nCntTxt].bUse = false;			//�g�p���Ă��邩
	}
	g_nCntAfterResultMessage = 0;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - TEXT_X, g_aResultMessage[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + TEXT_X, g_aResultMessage[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - TEXT_X, g_aResultMessage[nCntTxt].pos.y + TEXT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + TEXT_X, g_aResultMessage[nCntTxt].pos.y + TEXT_Y, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = g_aResultMessage[nCntTxt].col;
		pVtx[1].col = g_aResultMessage[nCntTxt].col;
		pVtx[2].col = g_aResultMessage[nCntTxt].col;
		pVtx[3].col = g_aResultMessage[nCntTxt].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultMessage->Unlock();
}

//=============================================================================
//���ʂ̃��b�Z�[�W�I������
//=============================================================================
void UninitResultMessage(void)
{

	//SE���I��
	StopSound();

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameResultMessage) / sizeof(*c_apFilenameResultMessage); CntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureResultMessage[CntTex] != NULL)
		{
			g_pTextureResultMessage[CntTex]->Release();
			g_pTextureResultMessage[CntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResultMessage != NULL)
	{
		g_pVtxBuffResultMessage->Release();
		g_pVtxBuffResultMessage = NULL;
	}
}

//=============================================================================
//���ʂ̃��b�Z�[�W�X�V����
//=============================================================================
void UpdateResultMessage(void)
{

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aResultMessage[nCntTxt].bUse == true)
		{//�g�p����Ă�����

			//�g��̎��ԉ��Z
			g_aResultMessage[nCntTxt].nCntZoom++;

			//�␳���Ĉړ�
			g_aResultMessage[nCntTxt].pos.x += (g_aResultMessage[nCntTxt].posOrigin.x - g_aResultMessage[nCntTxt].pos.x) * 0.25f;
			g_aResultMessage[nCntTxt].pos.y += (g_aResultMessage[nCntTxt].posOrigin.y - g_aResultMessage[nCntTxt].pos.y) * 0.05f;
			g_aResultMessage[nCntTxt].col.a += (1.0f - g_aResultMessage[nCntTxt].col.a) * 0.05f;

			if (g_aResultMessage[nCntTxt].nCntZoom == 10 && nCntTxt == 0 && g_aResultMessage[nCntTxt].nType == RESULTTYPE_CLEAR)
			{//���������ȂƂ��Ƀp�[�e�B�N��

				SetParticle(g_aResultMessage[nCntTxt].posOrigin, PARTICLE_TYPE_RESULTMESSAGE);

				//�N���A��
				PlaySound(SOUND_LABEL_SE_GAMECLEAR);
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y - g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y - g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y + g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y + g_aResultMessage[nCntTxt].fHeight, 0.0f);

			//���_�J���[
			pVtx[0].col = g_aResultMessage[nCntTxt].col;
			pVtx[1].col = g_aResultMessage[nCntTxt].col;
			pVtx[2].col = g_aResultMessage[nCntTxt].col;
			pVtx[3].col = g_aResultMessage[nCntTxt].col;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultMessage->Unlock();

}
//=============================================================================
//���ʂ̃��b�Z�[�W�`�揈��
//=============================================================================
void DrawResultMessage(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffResultMessage, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aResultMessage[nCntTxt].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureResultMessage[(g_aResultMessage[nCntTxt].nType * NUM_PLACE) + nCntTxt]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTxt * 4, 2);
		}
	}

	if (g_aResultMessage[0].bUse == true)
	{
		//2D�G�t�F�N�g�̕`�揈��
		DrawEffect_2D();
	}

}

//=============================================================================
//���ʂ̃��b�Z�[�W�ݒ菈��
//=============================================================================
void SetResultMessage(int nType)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResultMessage->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aResultMessage[nCntTxt].bUse == false)
		{//�g�p����Ă��Ȃ�������

			g_aResultMessage[nCntTxt].bUse = true;

			if (nType == RESULTTYPE_CLEAR)
			{
				g_aResultMessage[nCntTxt].pos = D3DXVECTOR3(-600.0f + nCntTxt * 1880.0f, 360.0f, 0.0f);	//�ʒu
				g_aResultMessage[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F

			}
			else
			{
				g_aResultMessage[nCntTxt].pos = D3DXVECTOR3(640.0f, 400.0f, 0.0f);	//�ʒu
				g_aResultMessage[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		//�F

				//���[�ނ��[�΁[��
				PlaySound(SOUND_LABEL_SE_GAMEFAILED);
			}
			g_aResultMessage[nCntTxt].fWidth = TEXT_X;
			g_aResultMessage[nCntTxt].fHeight = TEXT_Y;
			g_aResultMessage[nCntTxt].nCntZoom = 0;
			g_nCntAfterResultMessage = 0;
			g_aResultMessage[nCntTxt].nType = nType;	//�C�x���g�̎��

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y - g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y - g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x - g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y + g_aResultMessage[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aResultMessage[nCntTxt].pos.x + g_aResultMessage[nCntTxt].fWidth, g_aResultMessage[nCntTxt].pos.y + g_aResultMessage[nCntTxt].fHeight, 0.0f);

			//���_�J���[
			pVtx[0].col = g_aResultMessage[nCntTxt].col;
			pVtx[1].col = g_aResultMessage[nCntTxt].col;
			pVtx[2].col = g_aResultMessage[nCntTxt].col;
			pVtx[3].col = g_aResultMessage[nCntTxt].col;
			//break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResultMessage->Unlock();
}

//=============================================================================
//���ʂ̃��b�Z�[�W���擾
//=============================================================================
RESULT_MESSAGE *GetResultMessage(void)
{
	return &g_aResultMessage[0];
}