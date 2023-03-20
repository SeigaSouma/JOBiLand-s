//=============================================================================
//
// ��ʔ�����UI���� [manyUI.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "input.h"
#include "light.h"
#include "endcountdown.h"
#include "2D_effect.h"
#include "particle.h"
#include "camera.h"
#include "sound.h"
#include "eventstamp.h"

#define NUM_PLACE	(5)
#define MOVE_TIME	(60)
#define TEXT_X	(125.0f)
#define TEXT_Y	(125.0f)

//�O���[�o���ϐ��錾
const char *c_apFilenameEndCountDown[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\endcount_5.png",	//5
	"data\\TEXTURE\\endcount_4.png",	//4
	"data\\TEXTURE\\endcount_3.png",	//3
	"data\\TEXTURE\\endcount_2.png",	//2
	"data\\TEXTURE\\endcount_1.png",	//1
};

LPDIRECT3DTEXTURE9 g_pTextureEndCountDown[(sizeof c_apFilenameEndCountDown) / sizeof(*c_apFilenameEndCountDown)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEndCountDown = NULL;		//���_�o�b�t�@�ւ̃|�C���^
ENDCOUNTDOWN g_aEndCountDown[NUM_PLACE];					//����
int g_nCntAfterEndCountDown;

//=============================================================================
//��ʔ�����UI����������
//=============================================================================					   
void InitEndCountDown(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameEndCountDown) / sizeof(*c_apFilenameEndCountDown); CntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameEndCountDown[CntTex],
			&g_pTextureEndCountDown[CntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEndCountDown,
		NULL);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//g_aEndCountDown[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);	//�ʒu
		g_aEndCountDown[nCntTxt].pos = D3DXVECTOR3(1280.0f * 0.5f, 500.0f, 0.0f);	//�ʒu
		g_aEndCountDown[nCntTxt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F
		g_aEndCountDown[nCntTxt].nType = 0;			//���
		g_aEndCountDown[nCntTxt].fWidth = 0.0f;		//��
		g_aEndCountDown[nCntTxt].fHeight = 0.0f;		//����
		g_aEndCountDown[nCntTxt].nCntZoom = 0;		//�傫���Ȃ鎞��
		g_aEndCountDown[nCntTxt].bUse = false;			//�g�p���Ă��邩
	}
	g_nCntAfterEndCountDown = 0;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEndCountDown->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - TEXT_X, g_aEndCountDown[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + TEXT_X, g_aEndCountDown[nCntTxt].pos.y - TEXT_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - TEXT_X, g_aEndCountDown[nCntTxt].pos.y + TEXT_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + TEXT_X, g_aEndCountDown[nCntTxt].pos.y + TEXT_Y, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.6f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEndCountDown->Unlock();
}

//=============================================================================
//��ʔ�����UI�I������
//=============================================================================
void UninitEndCountDown(void)
{

	//SE���I��
	StopSound();

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameEndCountDown) / sizeof(*c_apFilenameEndCountDown); CntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureEndCountDown[CntTex] != NULL)
		{
			g_pTextureEndCountDown[CntTex]->Release();
			g_pTextureEndCountDown[CntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEndCountDown != NULL)
	{
		g_pVtxBuffEndCountDown->Release();
		g_pVtxBuffEndCountDown = NULL;
	}
}

//=============================================================================
//��ʔ�����UI�X�V����
//=============================================================================
void UpdateEndCountDown(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	VERTEX_2D *pVtx;

	//�C�x���g�X�^���v�̏��擾
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEndCountDown->Lock(0, 0, (void**)&pVtx, 0);

	if (GetKeyboardTrigger(DIK_4) == true)
	{//���L�[�������ꂽ

		SetEndCountDown(0);
	}

	int nCntUse = 0;
	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aEndCountDown[nCntTxt].bUse == true)
		{//�g�p����Ă�����

			//�g�p�J�E���g�����Z
			nCntUse++;

			//�g��̎��ԉ��Z
			g_aEndCountDown[nCntTxt].nCntZoom++;

			g_aEndCountDown[nCntTxt].col.a -= 0.02f;

			if (g_aEndCountDown[nCntTxt].nCntZoom <= MOVE_TIME)
			{
				g_aEndCountDown[nCntTxt].fHeight += TEXT_Y * 0.2f;
				g_aEndCountDown[nCntTxt].fWidth += TEXT_X * 0.2f;

				//g_aEndCountDown[nCntTxt].fWidth += (TEXT_X - g_aEndCountDown[nCntTxt].fWidth) * 0.25f;
				//g_aEndCountDown[nCntTxt].fHeight += (TEXT_Y - g_aEndCountDown[nCntTxt].fHeight) * 0.25f;
				//[nCntTxt].pos.y += (360.0f - g_aEndCountDown[nCntTxt].pos.y) * 0.25f;
			}
			else if (g_aEndCountDown[nCntTxt].nCntZoom == MOVE_TIME + 1)
			{//�ړ����I�������

			    //���̕����̐ݒ�
				SetEndCountDown(0);
			}

			if (g_aEndCountDown[nCntTxt].nCntZoom == 3)
			{
				PlaySound(SOUND_LABEL_SE_ENDCOUNTDOWN);
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y - g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y - g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y + g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y + g_aEndCountDown[nCntTxt].fHeight, 0.0f);

			//���_�J���[
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndCountDown[nCntTxt].col.a);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndCountDown[nCntTxt].col.a);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndCountDown[nCntTxt].col.a);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndCountDown[nCntTxt].col.a);
		}

		pVtx += 4;
	}

	if (g_aEndCountDown[4].col.a <= 0.0f)
	{
		for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
		{
			g_aEndCountDown[nCntTxt].bUse = false;

			g_aEndCountDown[nCntTxt].col.a = 0.6f;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEndCountDown->Unlock();

}
//=============================================================================
//��ʔ�����UI�`�揈��
//=============================================================================
void DrawEndCountDown(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEndCountDown, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aEndCountDown[nCntTxt].bUse == true)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureEndCountDown[nCntTxt]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTxt * 4, 2);
		}
	}
}

//=============================================================================
//��ʔ�����UI�ݒ菈��
//=============================================================================
void SetEndCountDown(int nType)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEndCountDown->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTxt = 0; nCntTxt < NUM_PLACE; nCntTxt++)
	{
		if (g_aEndCountDown[nCntTxt].bUse == false)
		{//�g�p����Ă��Ȃ�������

			g_aEndCountDown[nCntTxt].bUse = true;
			//g_aEndCountDown[nCntTxt].pos = D3DXVECTOR3(250.0f + nCntTxt * 250.0f, 500.0f, 0.0f);	//�ʒu
			g_aEndCountDown[nCntTxt].pos = D3DXVECTOR3(1280.0f * 0.5f, 350.0f, 0.0f);
			g_aEndCountDown[nCntTxt].fWidth = 10.0f;
			g_aEndCountDown[nCntTxt].fHeight = 10.0f;
			g_aEndCountDown[nCntTxt].nCntZoom = 0;
			g_nCntAfterEndCountDown = 0;
			g_aEndCountDown[nCntTxt].nType = nType;	//�C�x���g�̎��

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y - g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y - g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x - g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y + g_aEndCountDown[nCntTxt].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEndCountDown[nCntTxt].pos.x + g_aEndCountDown[nCntTxt].fWidth, g_aEndCountDown[nCntTxt].pos.y + g_aEndCountDown[nCntTxt].fHeight, 0.0f);

			break;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEndCountDown->Unlock();
}

//=============================================================================
//��ʔ�����UI���擾
//=============================================================================
ENDCOUNTDOWN *GetEndCountDown(void)
{
	return &g_aEndCountDown[0];
}