//=============================================================================
//
// �I��UI [finish.cpp]
// Author : �匴�叫
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
#include "fenish.h"

#define NUM_PLACE	(4)
#define MOVE_TIME	(20)
#define TEXT_X	(250.0f)
#define TEXT_Y	(125.0f)

////�O���[�o���ϐ��錾
//const char *c_apFilenameFinish[] =	//�t�@�C���ǂݍ���
//{
//	"data\\TEXTURE\\tairyo_01.png",	//��
//};

LPDIRECT3DTEXTURE9 g_pTextureFinish = NULL;	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffFinish = NULL;		//���_�o�b�t�@�ւ̃|�C���^
FENISH g_aFinish;					//����
int g_nCntAfterFinish;

//=============================================================================
//�I��UI�̏���������
//=============================================================================					   
void InitFinish(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\endcount_finish.png",
		&g_pTextureFinish);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffFinish,
		NULL);

	
	g_aFinish.pos = D3DXVECTOR3(1280.0f, 340.0f, 0.0f);	//�ʒu
	g_aFinish.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�F
	g_aFinish.nType = 0;			//���
	g_aFinish.fWidth = 0.0f;		//��
	g_aFinish.fHeight = 0.0f;		//����
	g_aFinish.nCntZoom = 0;		//�傫���Ȃ鎞��
	g_aFinish.bUse = false;		//�g�p���Ă��邩
	
	g_nCntAfterFinish = 0;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFinish->Lock(0, 0, (void**)&pVtx, 0);

	
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aFinish.pos.x - TEXT_X, g_aFinish.pos.y - TEXT_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aFinish.pos.x + TEXT_X, g_aFinish.pos.y - TEXT_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aFinish.pos.x - TEXT_X, g_aFinish.pos.y + TEXT_Y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aFinish.pos.x + TEXT_X, g_aFinish.pos.y + TEXT_Y, 0.0f);

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
	

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFinish->Unlock();
}

//=============================================================================
//�I��UI�̏I������
//=============================================================================
void UninitFinish(void)
{

	//�T�E���h�I��
	StopSound();
	
	//�e�N�X�`���̔j��
	if (g_pTextureFinish != NULL)
	{
		g_pTextureFinish->Release();
		g_pTextureFinish = NULL;
	}
	

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffFinish != NULL)
	{
		g_pVtxBuffFinish->Release();
		g_pVtxBuffFinish = NULL;
	}
}

//=============================================================================
//�I��UI�̍X�V����
//=============================================================================
void UpdateFinish(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	VERTEX_2D *pVtx;

	//�C�x���g�X�^���v�̏��擾
	EVENTSTAMP *pEventStamp = GetEventStamp();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFinish->Lock(0, 0, (void**)&pVtx, 0);

	int nCntUse = 0;

	if (GetKeyboardTrigger(DIK_5) == true)
	{//���L�[�������ꂽ

		SetFinish(0);
	}

	if (g_aFinish.bUse == true)
	{//�g�p����Ă�����

		//�g�p�J�E���g�����Z
		nCntUse++;

		//�g��̎��ԉ��Z
		g_aFinish.nCntZoom++;

		if (g_aFinish.nCntZoom <= MOVE_TIME)
		{
			g_aFinish.fWidth += (TEXT_X - g_aFinish.fWidth) * 0.25f;
			g_aFinish.fHeight += (TEXT_Y - g_aFinish.fHeight) * 0.25f;
			g_aFinish.pos.y += (360.0f - g_aFinish.pos.y) * 0.25f;
		}
		
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aFinish.pos.x - g_aFinish.fWidth, g_aFinish.pos.y - g_aFinish.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFinish.pos.x + g_aFinish.fWidth, g_aFinish.pos.y - g_aFinish.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFinish.pos.x - g_aFinish.fWidth, g_aFinish.pos.y + g_aFinish.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFinish.pos.x + g_aFinish.fWidth, g_aFinish.pos.y + g_aFinish.fHeight, 0.0f);

		pVtx += 4;
	}

	
	

	//if (nCntUse >= NUM_PLACE - 1)
	//{//�S���g�p����Ă�����

	//	if (g_nCntAfterFinish == 60)
	//	{
	//		//���A�c�L�^�[�I
	//		PlaySound(SOUND_LABEL_SE_FINISH);
	//	}

	//	//�]�C�̎��ԉ��Z
	//	g_nCntAfterFinish++;

	//	
	//	if (g_nCntAfterFinish >= 150)
	//	{//�]�C�ŏ���

	//		g_aFinish.fWidth += (10.0f - g_aFinish.fWidth) * 0.25f;
	//		g_aFinish.fHeight += (10.0f - g_aFinish.fHeight) * 0.25f;
	//		//g_aFinish.pos.y += (100.0f - g_aFinish.pos.y) * 0.25f;
	//		g_aFinish.pos.x += (1100.0f - g_aFinish.pos.x) * 0.25f;

	//		if (g_aFinish.fWidth < 12.0f)
	//		{
	//			//�ڕW�̒n�_�ݒ�
	//			pCamera->nMoveStep = CAMERAMOVE_UP;
	//			g_aFinish.bUse = false;

	//			if (pEventStamp[EVENTTYPE_MANYMOB].bUse == false)
	//			{
	//				SetEventStamp(EVENTTYPE_MANYMOB);
	//			}
	//		}
	//	}
		
	//}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFinish->Unlock();

}
//=============================================================================
//�I��UI�̕`�揈��
//=============================================================================
void DrawFinish(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffFinish, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	
	if (g_aFinish.bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureFinish);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
	
}

//=============================================================================
//�I��UI�̐ݒ菈��
//=============================================================================
void SetFinish(int nType)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffFinish->Lock(0, 0, (void**)&pVtx, 0);

	
	if (g_aFinish.bUse == false)
	{//�g�p����Ă��Ȃ�������

		g_aFinish.bUse = true;
		g_aFinish.pos = D3DXVECTOR3(1280.0f * 0.5f, 340.0f, 0.0f);	//�ʒu
		g_aFinish.fWidth = 3000.0f;
		g_aFinish.fHeight = 1500.0f;
		g_aFinish.nCntZoom = 0;
		g_nCntAfterFinish = 0;
		g_aFinish.nType = nType;	//�C�x���g�̎��

		//���A�c�L�^�[�I
		PlaySound(SOUND_LABEL_SE_FINISH);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aFinish.pos.x - g_aFinish.fWidth, g_aFinish.pos.y - g_aFinish.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aFinish.pos.x + g_aFinish.fWidth, g_aFinish.pos.y - g_aFinish.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aFinish.pos.x - g_aFinish.fWidth, g_aFinish.pos.y + g_aFinish.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aFinish.pos.x + g_aFinish.fWidth, g_aFinish.pos.y + g_aFinish.fHeight, 0.0f);

		pVtx += 4;
	}

	
	

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffFinish->Unlock();
}

//=============================================================================
//�I��UI�̏��擾
//=============================================================================
FENISH *GetFinish(void)
{
	return &g_aFinish;
}