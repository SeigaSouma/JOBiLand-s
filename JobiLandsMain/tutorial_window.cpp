//=============================================================================
//
// �`���[�g���A�������o������ [tutorial_window.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "tutorial_window.h"
#include "tutorial.h"
#include "player.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"

//�}�N����`
#define MAX_TOP			(4)		//���_��
#define WIDTH		(640.0f)
#define HEIGHT		(110.0f)
#define SKIP_WIDTH	(125.0f)
#define SKIP_HEIGHT	(25.0f)
#define ALPHA_MOVE	(0.02f)

//�O���[�o���ϐ��錾
const char *c_apFilenameTutorialWindow[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\tutorial_window.png",
	"data\\TEXTURE\\tutorial_step00.png",
	"data\\TEXTURE\\tutorial_step01.png",
	"data\\TEXTURE\\tutorial_step02.png",
	"data\\TEXTURE\\tutorial_step03.png",
	"data\\TEXTURE\\tutorial_step04.png",
	"data\\TEXTURE\\tutorial_step05.png",
	"data\\TEXTURE\\tutorial_step06.png",
	"data\\TEXTURE\\tutorial_step07.png",
	"data\\TEXTURE\\tutorial_step08.png",
	"data\\TEXTURE\\tutorial_step09.png",
	"data\\TEXTURE\\tutorial_skip.png",
};
LPDIRECT3DTEXTURE9 g_pTextureTutorialWindow[(sizeof c_apFilenameTutorialWindow) / sizeof(*c_apFilenameTutorialWindow)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTutorialWindow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
TutorialWindow g_aTutorialWindow;						//�`���[�g���A�������o���̏��

//==================================================================================
//�`���[�g���A�������o���̏���������
//==================================================================================
void InitTutorialWindow(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialWindow) / sizeof(*c_apFilenameTutorialWindow); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTutorialWindow[nCntTex],
			&g_pTextureTutorialWindow[nCntTex]);
	}

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	//�`���[�g���A�������o���̏��̏�����
	g_aTutorialWindow.pos = D3DXVECTOR3(640.0f, 500.0f, 0.0f);
	g_aTutorialWindow.nStep = pTutorial->nStep;
	g_aTutorialWindow.nOldStep = -1;
	g_aTutorialWindow.fAlpha = 1.0f;
	g_aTutorialWindow.nWaitTime = 0;
	g_aTutorialWindow.nIdxMoveLog = SetMoveLog();	//�����g�̃C���f�b�N�X�擾
	g_aTutorialWindow.bMove = false;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * TUTORIAL_WINDOW_VTX_MAX,
								D3DUSAGE_WRITEONLY,
								FVF_VERTEX_2D,
								D3DPOOL_MANAGED,
								&g_pVtxBuffTutorialWindow,
								NULL);

	VERTEX_2D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialWindow->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTutorialWindow = 0; nCntTutorialWindow < TUTORIAL_WINDOW_VTX_MAX; nCntTutorialWindow++)
	{
		if (nCntTutorialWindow == TUTORIAL_WINDOW_VTX_SKIP)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(450.0f + g_aTutorialWindow.pos.x - SKIP_WIDTH, 110.0f + g_aTutorialWindow.pos.y - SKIP_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(450.0f + g_aTutorialWindow.pos.x + SKIP_WIDTH, 110.0f + g_aTutorialWindow.pos.y - SKIP_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(450.0f + g_aTutorialWindow.pos.x - SKIP_WIDTH, 110.0f + g_aTutorialWindow.pos.y + SKIP_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(450.0f + g_aTutorialWindow.pos.x + SKIP_WIDTH, 110.0f + g_aTutorialWindow.pos.y + SKIP_HEIGHT, 0.0f);
		}
		else
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - WIDTH, g_aTutorialWindow.pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + WIDTH, g_aTutorialWindow.pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - WIDTH, g_aTutorialWindow.pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + WIDTH, g_aTutorialWindow.pos.y + HEIGHT, 0.0f);
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialWindow->Unlock();
}

//==================================================================================
//�`���[�g���A�������o���̏I������
//==================================================================================
void UninitTutorialWindow(void)
{

	//�T�E���h�̒�~
	//StopSound();

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTutorialWindow) / sizeof(*c_apFilenameTutorialWindow); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTutorialWindow[nCntTex] != NULL)
		{
			g_pTextureTutorialWindow[nCntTex]->Release();
			g_pTextureTutorialWindow[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTutorialWindow != NULL)
	{
		g_pVtxBuffTutorialWindow->Release();
		g_pVtxBuffTutorialWindow = NULL;
	}
}

//==================================================================================
//�`���[�g���A�������o���̍X�V����
//==================================================================================
void UpdateTutorialWindow(void)
{

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	//�v���C���[���擾
	Player *pPlayer = GetPlayer();

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTutorialWindow->Lock(0, 0, (void**)&pVtx, 0);

	//�`���[�g���A���̐i�s�x�擾
	g_aTutorialWindow.nStep = pTutorial->nStep;

	if (g_aTutorialWindow.nOldStep != g_aTutorialWindow.nStep)
	{//�O��̐i�s�x�ƈ���Ă�����

		g_aTutorialWindow.nWaitTime = 20;	//�҂����ԌŒ�
		g_aTutorialWindow.fAlpha = 1.0f;

		//���S�̒������Z�b�g
		ResetMoveLog(g_aTutorialWindow.nIdxMoveLog);
		g_aTutorialWindow.nOldStep = g_aTutorialWindow.nStep;

		//�T�E���h�Đ�
		//PlaySound(SOUND_LABEL_SE_MSGWINDOW);
	}

	if (pPlayer->nNowMotionNum != PLAYERMOTION_DEF)
	{//�j���[�g�������[�V�����ȊO�ŏ����Ă���

		g_aTutorialWindow.nWaitTime--;	//�҂����ԉ��Z

		if (g_aTutorialWindow.nWaitTime <= 10)
		{
			g_aTutorialWindow.nWaitTime = 10;	//�҂����ԌŒ�
			g_aTutorialWindow.fAlpha -= ALPHA_MOVE;	//�s�����x����

			if (g_aTutorialWindow.fAlpha <= 0.4f)
			{//�K��l�ŌŒ�
				g_aTutorialWindow.fAlpha = 0.4f;
			}
		}
	}
	else
	{//���͏o��

		g_aTutorialWindow.nWaitTime++;	//�҂����ԉ��Z

		if (g_aTutorialWindow.nWaitTime >= 20)
		{
			g_aTutorialWindow.nWaitTime = 20;	//�҂����ԌŒ�
			g_aTutorialWindow.fAlpha += ALPHA_MOVE;	//�s�����x����

			if (g_aTutorialWindow.fAlpha >= 1.0f)
			{//�K��l�ŌŒ�
				g_aTutorialWindow.fAlpha = 1.0f;
			}
		}
	}
	
	//�傫���Ȃ遨�k�ށ��傫���Ȃ��ċK��l
	float fWidth = MoveLog(g_aTutorialWindow.nIdxMoveLog, WIDTH, 15);
	float fHeight = MoveLog(g_aTutorialWindow.nIdxMoveLog, HEIGHT, 15);

	//���_���W�̐ݒ�
	pVtx[TUTORIAL_WINDOW_VTX_WINDOW * 4 + 0].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - fWidth, g_aTutorialWindow.pos.y - fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_WINDOW * 4 + 1].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + fWidth, g_aTutorialWindow.pos.y - fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_WINDOW * 4 + 2].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - fWidth, g_aTutorialWindow.pos.y + fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_WINDOW * 4 + 3].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + fWidth, g_aTutorialWindow.pos.y + fHeight, 0.0f);

	pVtx[TUTORIAL_WINDOW_VTX_TEXT * 4 + 0].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - fWidth, g_aTutorialWindow.pos.y - fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_TEXT * 4 + 1].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + fWidth, g_aTutorialWindow.pos.y - fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_TEXT * 4 + 2].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x - fWidth, g_aTutorialWindow.pos.y + fHeight, 0.0f);
	pVtx[TUTORIAL_WINDOW_VTX_TEXT * 4 + 3].pos = D3DXVECTOR3(g_aTutorialWindow.pos.x + fWidth, g_aTutorialWindow.pos.y + fHeight, 0.0f);


	for (int nCntTutorialWindow = 0; nCntTutorialWindow < TUTORIAL_WINDOW_VTX_MAX; nCntTutorialWindow++)
	{
		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aTutorialWindow.fAlpha);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aTutorialWindow.fAlpha);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aTutorialWindow.fAlpha);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aTutorialWindow.fAlpha);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTutorialWindow->Unlock();

}

//==================================================================================
//�`���[�g���A�������o���̕`�揈��
//==================================================================================
void DrawTutorialWindow(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTutorialWindow, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorialWindow[0]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//�e�L�X�g�̕`��
	for (int nCntTutorialWindow = TUTORIAL_WINDOW_VTX_TEXT; nCntTutorialWindow < TUTORIAL_WINDOW_VTX_SKIP; nCntTutorialWindow++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTutorialWindow[g_aTutorialWindow.nStep + TUTORIAL_WINDOW_VTX_TEXT]);
	
		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTutorialWindow * 4, 2);
	}

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTutorialWindow[((sizeof c_apFilenameTutorialWindow) / sizeof(*c_apFilenameTutorialWindow)) - 1]);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, TUTORIAL_WINDOW_VTX_SKIP * 4, 2);
}

//==================================================================================
//�`���[�g���A�������o�����擾
//==================================================================================
TutorialWindow *GetTutorialWindow(void)
{
	return &g_aTutorialWindow;
}