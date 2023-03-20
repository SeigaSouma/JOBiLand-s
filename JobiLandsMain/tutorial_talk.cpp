//=============================================================================
//
// �`���[�g���A���̉�b���� [tutorial_talk.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "tutorial_talk.h"
#include "tutorial.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
#include "player.h"
#include "buddy.h"
#include "dispatch.h"

//�}�N����`
#define MAX_TOP			(4)		//���_��
#define WIDTH			(640.0f)
#define HEIGHT			(360.0f)
#define OK_WIDTH		(250.0f)
#define OK_HEIGHT		(80.0f)
#define SET_TIME		(80)
#define ENTER_COOLTIME	(20)

//�v���g�^�C�v�錾
void CreatVtxTutorialTalk(void);
void CreatVtxTutorialOK(void);
void UpdateTutorialOK(void);

//�O���[�o���ϐ��錾
const char *c_apFilenameTutorialTalk[] =		//�t�@�C���ǂݍ���
{
	NULL,
	"data\\TEXTURE\\tutorial_talk01.png",
	"data\\TEXTURE\\tutorial_talk02.png",
	"data\\TEXTURE\\tutorial_talk03.png",
	"data\\TEXTURE\\tutorial_talk04.png",
	"data\\TEXTURE\\tutorial_talk05.png",
	"data\\TEXTURE\\tutorial_talk06.png",
	"data\\TEXTURE\\tutorial_talk07.png",
	"data\\TEXTURE\\tutorial_talk08.png",
	"data\\TEXTURE\\tutorial_talk09.png",
};

const char *c_apFilenameTutorialOK[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\tutorial_OK.png",
};
LPDIRECT3DTEXTURE9 g_pTextureTutorialTalk[(sizeof c_apFilenameTutorialTalk) / sizeof(*c_apFilenameTutorialTalk)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialTalk = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTutorialOK[(sizeof c_apFilenameTutorialOK) / sizeof(*c_apFilenameTutorialOK)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialOK = NULL;	//���_�o�b�t�@�ւ̃|�C���^
TutorialTalk g_aTutorialTalk;						//�`���[�g���A���̉�b�̏��
TutorialOK g_aTutorialOK;						//�`���[�g���A���̊����̏��

//==================================================================================
//�`���[�g���A���̉�b�̏���������
//==================================================================================
void InitTutorialTalk(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialTalk) / sizeof(*c_apFilenameTutorialTalk); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTutorialTalk[nCntTex],
			&g_pTextureTutorialTalk[nCntTex]);
	}

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialOK) / sizeof(*c_apFilenameTutorialOK); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTutorialOK[nCntTex],
			&g_pTextureTutorialOK[nCntTex]);
	}

	CreatVtxTutorialTalk();
	CreatVtxTutorialOK();
}

//==================================================================================
//�`���[�g���A���̉�b�̏I������
//==================================================================================
void UninitTutorialTalk(void)
{

	//�T�E���h�̒�~
	//StopSound();

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialTalk) / sizeof(*c_apFilenameTutorialTalk); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTutorialTalk[nCntTex] != NULL)
		{
			g_pTextureTutorialTalk[nCntTex]->Release();
			g_pTextureTutorialTalk[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorialTalk != NULL)
	{
		g_pVtxBuffTutorialTalk->Release();
		g_pVtxBuffTutorialTalk = NULL;
	}

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialOK) / sizeof(*c_apFilenameTutorialOK); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTutorialOK[nCntTex] != NULL)
		{
			g_pTextureTutorialOK[nCntTex]->Release();
			g_pTextureTutorialOK[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorialOK != NULL)
	{
		g_pVtxBuffTutorialOK->Release();
		g_pVtxBuffTutorialOK = NULL;
	}
}

//==================================================================================
//�`���[�g���A���̉�b�̍X�V����
//==================================================================================
void UpdateTutorialTalk(void)
{
	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();

	//�h���J�[�\���̏��擾
	DISPATCH *pDispatch = GetDispatch();

	//�X�e�b�v����
	g_aTutorialTalk.nStep = pTutorial->nStep;

	if (g_aTutorialTalk.bSet == true && g_aTutorialTalk.bUse == false)
	{
		//�ݒ莞�Ԍ��Z
		g_aTutorialTalk.nCntSet--;

		if (g_aTutorialTalk.nCntSet <= 0)
		{//���Ԍo��

			g_aTutorialTalk.bSet = false;
			g_aTutorialTalk.bUse = true;
			g_aTutorialOK.bUse = false;

			PlaySound(SOUND_LABEL_SE_TUTORIALWINDOW);
		}
	}

	if (g_aTutorialTalk.bUse == true)
	{//�g�p���Ă�����

		if (GetKeyboardRelease(DIK_L) == true || GetGamepadRelease(BUTTON_A, 0))
		{//L�������ꂽ && A�{�^���������ꂽ

			//�U����ԉ���
			pBuddy[BUDDYTYPE_HYOROGARI].bATK = false;
			pBuddy[BUDDYTYPE_DEPPA].bATK = false;
			pPlayer->bATK = false;

			//���[�V�����̐ݒ�
			SetMotisonBuddy(BUDDYTYPE_HYOROGARI, BUDDYMOTION_DEF);
			SetMotisonBuddy(BUDDYTYPE_DEPPA, BUDDYMOTION_DEF);
			SetMotisonPlayer(PLAYERMOTION_DEF);
		}

		if (pDispatch->bUse == true)
		{
			if (GetGamepadRelease(BUTTON_LB + pDispatch->nType, 0) || GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
			{//�h���̎g�p�󋵓���ւ�

				if (pDispatch->nType == 0)
				{
					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_DISPATCHL);
				}
				else if (pDispatch->nType == 1)
				{
					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_DISPATCHR);
				}

				//�h������
				SetBuddyDispatch(pDispatch->pos, pDispatch->nType);
				pDispatch->bUse = false;
				pDispatch->nType = -1;
			}
		}

		//�N�[���^�C�����Z
		g_aTutorialTalk.nCntEnter++;

		if ((GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0)) &&
			g_aTutorialTalk.nCntEnter >= ENTER_COOLTIME)
		{//�G���^�[�L�[�������ꂽ��

			pTutorial->nStep += 1;
			g_aTutorialTalk.bUse = false;
			g_aTutorialTalk.nCntEnter = 0;
		}
	}

	//OK�̍X�V����
	UpdateTutorialOK();

}

//==================================================================================
//�`���[�g���A���̊����̍X�V����
//==================================================================================
void UpdateTutorialOK(void)
{

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialOK->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aTutorialOK.bUse == true)
	{//�g�p����Ă�����

		//�`���[�g���A���̉�b�̏��̏�����
		g_aTutorialOK.pos.y += (360.0f - g_aTutorialOK.pos.y) * 0.15f;
		g_aTutorialOK.col.a += (1.0f - g_aTutorialOK.col.a) * 0.15f;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aTutorialOK.col;
		pVtx[1].col = g_aTutorialOK.col;
		pVtx[2].col = g_aTutorialOK.col;
		pVtx[3].col = g_aTutorialOK.col;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialOK->Unlock();
}

//==================================================================================
//�`���[�g���A���̉�b�̕`�揈��
//==================================================================================
void DrawTutorialTalk(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();


	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialOK, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aTutorialOK.bUse == true)
	{//�g�p���Ă�����

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorialOK[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}



	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialTalk, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	if (g_aTutorialTalk.bUse == true)
	{//�g�p���Ă�����

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorialTalk[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

		//�e�L�X�g�̕`��
		for (int nCntTutorialTalk = TUTORIAL_TALK_VTX_TEXT; nCntTutorialTalk < TUTORIAL_TALK_VTX_MAX; nCntTutorialTalk++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureTutorialTalk[g_aTutorialTalk.nStep + TUTORIAL_TALK_VTX_TEXT]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorialTalk * 4, 2);
		}
	}
}

//==================================================================================
//�`���[�g���A���̉�b�̐ݒ菈��
//==================================================================================
void SetTutorialTalk(void)
{
	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	if (g_aTutorialTalk.bUse == false && g_aTutorialTalk.bSet == false)
	{//�ݒ肵�Ă��Ȃ�������

		g_aTutorialTalk.bSet = true;
		g_aTutorialTalk.nCntSet = SET_TIME;
		g_aTutorialTalk.nCntEnter = 0;

		if (pTutorial->nStep >= TUTORIAL_STEP_MOVE)
		{
			SetTutorialOK();
			PlaySound(SOUND_LABEL_SE_TUTORIALCLEAR);
		}
	}
}

//==================================================================================
//�`���[�g���A���̊����̐ݒ菈��
//==================================================================================
void SetTutorialOK(void)
{

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialOK->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aTutorialOK.bUse == false)
	{
		//�`���[�g���A���̉�b�̏��̏�����
		g_aTutorialOK.pos = D3DXVECTOR3(640.0f, 650.0f, 0.0f);
		g_aTutorialOK.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_aTutorialOK.bUse = true;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aTutorialOK.col;
		pVtx[1].col = g_aTutorialOK.col;
		pVtx[2].col = g_aTutorialOK.col;
		pVtx[3].col = g_aTutorialOK.col;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialOK->Unlock();
}

//==================================================================================
//�`���[�g���A���̉�b���擾
//==================================================================================
void CreatVtxTutorialTalk(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	//�`���[�g���A���̉�b�̏��̏�����
	g_aTutorialTalk.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	g_aTutorialTalk.nStep = pTutorial->nStep;
	g_aTutorialTalk.nCntSet = 0;	//�ݒ�̃J�E���g
	g_aTutorialTalk.nCntEnter = 0;	//Enter�����܂ł̃N�[���^�C��
	g_aTutorialTalk.bSet = false;
	g_aTutorialTalk.bUse = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * TUTORIAL_TALK_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialTalk,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialTalk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorialTalk = 0; nCntTutorialTalk < TUTORIAL_TALK_VTX_MAX; nCntTutorialTalk++)
	{
		if (nCntTutorialTalk == TUTORIAL_TALK_VTX_FADE)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x - 640.0f, g_aTutorialTalk.pos.y - 360.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x + 640.0f, g_aTutorialTalk.pos.y - 360.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x - 640.0f, g_aTutorialTalk.pos.y + 360.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x + 640.0f, g_aTutorialTalk.pos.y + 360.0f, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
			pVtx[1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
			pVtx[2].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
			pVtx[3].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f);
		}
		else
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x - WIDTH, g_aTutorialTalk.pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x + WIDTH, g_aTutorialTalk.pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x - WIDTH, g_aTutorialTalk.pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTutorialTalk.pos.x + WIDTH, g_aTutorialTalk.pos.y + HEIGHT, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialTalk->Unlock();
}

//==================================================================================
//�`���[�g���A���̉�b���擾
//==================================================================================
void CreatVtxTutorialOK(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�`���[�g���A���̉�b�̏��̏�����
	g_aTutorialOK.pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	g_aTutorialOK.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_aTutorialOK.bUse = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTutorialOK,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialOK->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y - OK_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aTutorialOK.pos.x - OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aTutorialOK.pos.x + OK_WIDTH, g_aTutorialOK.pos.y + OK_HEIGHT, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[0].col = g_aTutorialOK.col;
	pVtx[1].col = g_aTutorialOK.col;
	pVtx[2].col = g_aTutorialOK.col;
	pVtx[3].col = g_aTutorialOK.col;

	//rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialOK->Unlock();
}

//==================================================================================
//�`���[�g���A���̉�b���擾
//==================================================================================
TutorialTalk *GetTutorialTalk(void)
{
	return &g_aTutorialTalk;
}