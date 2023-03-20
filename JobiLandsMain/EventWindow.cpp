//=============================================================================
//
// �C�x���g�E�B���h�E���� [EventWindow.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "EventWindow.h"
#include "manyMORS.h"
#include "input.h"
#include "light.h"
#include "sound.h"

#define MOVE_WINDOW (1.5f)
#define MAX_TIME	(1350)
#define TEXT_X		(500.0f)
#define WINDOW_X	(700.0f)
#define WINDOW_Y	(100.0f)
#define LINE_Y		(25.0f)

//�v���g�^�C�v�錾
void EWindowStepRed(void);
void EWindowStepFramMove(void);
void EWindowStepTextLine(void);
void EWindowStepOut(void);

//�O���[�o���ϐ��錾
const char *c_apFilenameE_Window[] =	//�t�@�C���ǂݍ���
{
	NULL,	//��ʔ������̕���
	"data\\TEXTURE\\warning_fram.png",	//�x�������̘g
	"data\\TEXTURE\\warning_text.png",	//�x�������̃e�L�X�g
	"data\\TEXTURE\\warning_01.png",	//�x�������̃e�L�X�g
	"data\\TEXTURE\\warning_01.png",	//�x�������̃e�L�X�g
};

LPDIRECT3DTEXTURE9 g_pTextureE_Window[(sizeof c_apFilenameE_Window) / sizeof(*c_apFilenameE_Window)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffE_Window = NULL;		//���_�o�b�t�@�ւ̃|�C���^
EVENTWINDOW g_E_Window;			//����

//=============================================================================
//�C�x���g�E�B���h�E�̏���������
//=============================================================================					   
void InitEventWindow(void)
{

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//�e�N�X�`���̐����ǂݍ���
	for (int CntTex = 0; CntTex < (sizeof c_apFilenameE_Window) / sizeof(*c_apFilenameE_Window); CntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameE_Window[CntTex],
			&g_pTextureE_Window[CntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * EVENTWINDOW_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffE_Window,
		NULL);

	//�e�v�f������
	g_E_Window.bUse = false;		//�g�p���Ă��邩
	g_E_Window.nStep = EVENTWINDOW_STEP_RED;		//�X�e�b�v

	//UI��񏉊���
	for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
	{
		g_E_Window.aUI[nCntWindow].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 500.0f, 0.0f);	//���_���W
		g_E_Window.aUI[nCntWindow].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//���_�J���[
		g_E_Window.aUI[nCntWindow].fWidth = WINDOW_X;		//��
		g_E_Window.aUI[nCntWindow].fHeight = WINDOW_Y;		//����
		g_E_Window.aUI[nCntWindow].nPatternAnim = 0;	//�A�j���[�V�����p�^�[��No
		g_E_Window.aUI[nCntWindow].nCntAnim = 0;		//�A�j���[�V�����J�E���^�[
		g_E_Window.aUI[nCntWindow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
		g_E_Window.aUI[nCntWindow].fAngle = 0.0f;		//�p�x
		g_E_Window.aUI[nCntWindow].fLength = 0.0f;		//�Ίp���̒���
		g_E_Window.aUI[nCntWindow].nCount = 0;			//�G�J�E���g
		g_E_Window.aUI[nCntWindow].bUse = false;			//�g�p���Ă��邩
		g_E_Window.aUI[nCntWindow].aTexU = 0.0f;		//�e�N�X�`�����WU[�X�N���[���p]
		g_E_Window.aUI[nCntWindow].aTexV = 0.0f;		//�e�N�X�`�����WV[�X�N���[���p]
	}

	//�F�̐ݒ�
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU = 0.0f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU = 1.0f;

	//�ʒu�̐ݒ�
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f);
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f);
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f);

	//�������̐ݒ�
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].fWidth = 640.0f;
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].fHeight = 360.0f;
	g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].fWidth = WINDOW_X;
	g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].fHeight = WINDOW_Y;
	g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].fWidth = TEXT_X;
	g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].fHeight = WINDOW_Y;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].fWidth = WINDOW_X;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].fHeight = LINE_Y;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].fWidth = WINDOW_X;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].fHeight = LINE_Y;

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffE_Window->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x - g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y - g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x + g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y - g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x - g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y + g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x + g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y + g_E_Window.aUI[nCntWindow].fHeight, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = g_E_Window.aUI[nCntWindow].col;
		pVtx[1].col = g_E_Window.aUI[nCntWindow].col;
		pVtx[2].col = g_E_Window.aUI[nCntWindow].col;
		pVtx[3].col = g_E_Window.aUI[nCntWindow].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffE_Window->Unlock();
}

//=============================================================================
//�C�x���g�E�B���h�E�̏I������
//=============================================================================
void UninitEventWindow(void)
{

	for (int CntTex = 0; CntTex < (sizeof c_apFilenameE_Window) / sizeof(*c_apFilenameE_Window); CntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureE_Window[CntTex] != NULL)
		{
			g_pTextureE_Window[CntTex]->Release();
			g_pTextureE_Window[CntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffE_Window != NULL)
	{
		g_pVtxBuffE_Window->Release();
		g_pVtxBuffE_Window = NULL;
	}
}

//=============================================================================
//�C�x���g�E�B���h�E�̍X�V����
//=============================================================================
void UpdateEventWindow(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffE_Window->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_E_Window.bUse == true)
	{//�g�p����Ă�����

		switch (g_E_Window.nStep)
		{
		case EVENTWINDOW_STEP_RED:
			EWindowStepRed();
			break;

		case EVENTWINDOW_STEP_FRAMMOVE:
			EWindowStepFramMove();
			break;

		case EVENTWINDOW_STEP_TEXTLINE:
			EWindowStepTextLine();
			break;

		case EVENTWINDOW_STEP_OUT:
			EWindowStepOut();
			break;
		}

		for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x - g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y - g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x + g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y - g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x - g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y + g_E_Window.aUI[nCntWindow].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_E_Window.aUI[nCntWindow].pos.x + g_E_Window.aUI[nCntWindow].fWidth, g_E_Window.aUI[nCntWindow].pos.y + g_E_Window.aUI[nCntWindow].fHeight, 0.0f);

			//���_�J���[
			pVtx[0].col = g_E_Window.aUI[nCntWindow].col;
			pVtx[1].col = g_E_Window.aUI[nCntWindow].col;
			pVtx[2].col = g_E_Window.aUI[nCntWindow].col;
			pVtx[3].col = g_E_Window.aUI[nCntWindow].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(g_E_Window.aUI[nCntWindow].aTexU, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_E_Window.aUI[nCntWindow].aTexU + 1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_E_Window.aUI[nCntWindow].aTexU, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_E_Window.aUI[nCntWindow].aTexU + 1.0f, 1.0f);

			pVtx += 4;
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffE_Window->Unlock();

}

//=============================================================================
//�C�x���g�E�B���h�E�̃X�e�b�v(�Ԗ��E�B���E�B��)
//=============================================================================
void EWindowStepRed(void)
{
	static int nMove = 1;

	//�s�����x�X�V
	g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a += (0.4f / (float)25) * nMove;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a > 0.4f)
	{//0.4f�ȏ�𒴂�����

		g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a = 0.4f;
		nMove *= -1;
	}
	else if (g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a < 0.0f)
	{//0.0f�����ɂȂ�����

		g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a = 0.0f;
		nMove *= -1;

		//�񐔉��Z
		g_E_Window.aUI[EVENTWINDOW_VTX_RED].nCount++;
	}

	
	if(g_E_Window.aUI[EVENTWINDOW_VTX_RED].nCount == 2)
	{//�K��񐔐ԓ_�ł�����

		//1.0f�Œ�
		g_E_Window.aUI[EVENTWINDOW_VTX_RED].col.a = 0.0f;

		//�X�e�b�v������
		g_E_Window.nStep = EVENTWINDOW_STEP_FRAMMOVE;
		PlaySound(SOUND_LABEL_SE_BACHIBACHI);
	}
}

//=============================================================================
//�C�x���g�E�B���h�E�̃X�e�b�v(�g�ړ�)
//=============================================================================
void EWindowStepFramMove(void)
{
	//�����Ɉړ�������
	g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x += (630.0f - g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x) * 0.20f;

	//�s�����x�X�V
	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a < 0.4f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a += (0.4f / (float)15);

		//��̃��C���ƐF����
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].col = g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col;
	}

	if (g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x <= 640.0f)
	{//�^�񒆂ɒ�������

		//�ʒu�Œ�
		g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x = 640.0f;

		//�X�e�b�v������
		g_E_Window.nStep = EVENTWINDOW_STEP_TEXTLINE;
	}
}

//=============================================================================
//�C�x���g�E�B���h�E�̃X�e�b�v(�e�L�X�g&���C���ړ�)
//=============================================================================
void EWindowStepTextLine(void)
{
	static int nMove = 1;

	//�����Ɉړ�������(�Ō�͂������)
	g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x += (660.0f - g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x) * 0.1f;

	//�s�����x�X�V
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a += (0.2f / (float)15) * nMove;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a > 1.0f)
	{//1.0f�ȏ�𒴂�����

		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a = 1.0f;
		nMove *= -1;

		//�񐔉��Z
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].nCount++;
	}
	else if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a < 0.4f)
	{//0.4f�����ɂȂ�����

		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col.a = 0.4f;
		nMove *= -1;
	}

	//��̃��C���ƐF����
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].col = g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].nCount == 0)
	{
		//�x������������
		PlaySound(SOUND_LABEL_SE_SECURITY);
	}

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].nCount == 3)
	{
		//�X�e�b�v������
		g_E_Window.nStep = EVENTWINDOW_STEP_OUT;

		//�C�x���g���͂̐ݒ�
		SetManyMORS(0);
	}

	//���C���̓X�N���[��������
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU += 0.001f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU -= 0.001f;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU >= 1.0f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU = 0.0f;
	}
	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU <= 0.0f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU = 1.0f;
	}

}

//=============================================================================
//�C�x���g�E�B���h�E�̃X�e�b�v(�ޏ�)
//=============================================================================
void EWindowStepOut(void)
{
	//�����Ɉړ�������(�Ō�͂������)
	g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x += (-700.0f - g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x) * 0.1f;
	g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x += (-700.0f - g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x) * 0.1f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].pos.x += (-700.0f - g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].pos.x) * 0.1f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].pos.x += (-700.0f - g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].pos.x) * 0.1f;

	//���C���̓X�N���[��������
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU += 0.001f;
	g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU -= 0.001f;

	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU >= 1.0f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU = 0.0f;
	}
	if (g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU <= 0.0f)
	{
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU = 1.0f;
	}

	if (g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x < -640.0f)
	{//�݂�ȑޏꂵ����

		g_E_Window.bUse = false;
	}
}

//=============================================================================
//�C�x���g�E�B���h�E�̕`�揈��
//=============================================================================
void DrawEventWindow(void)
{
	//�f�o�C�X�̃|�C���^�擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffE_Window, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_E_Window.bUse == true)
	{//�g�p����Ă�����

		for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureE_Window[nCntWindow]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntWindow * 4, 2);
		}
	}
}

//=============================================================================
//�C�x���g�E�B���h�E�̐ݒ菈��
//=============================================================================
void SetEventWindow(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffE_Window->Lock(0, 0, (void**)&pVtx, 0);
	
	if (g_E_Window.bUse == false)
	{//�g�p����Ă��Ȃ�������

		g_E_Window.bUse = true;

		for (int nCntWindow = 0; nCntWindow < EVENTWINDOW_VTX_MAX; nCntWindow++)
		{
			g_E_Window.aUI[nCntWindow].nCount = 0;
		}

		g_E_Window.aUI[EVENTWINDOW_VTX_FRAM].pos.x = SCREEN_WIDTH + SCREEN_WIDTH;
		g_E_Window.aUI[EVENTWINDOW_VTX_TEXT].pos.x = SCREEN_WIDTH + SCREEN_WIDTH;
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 400.0f, 0.0f);
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f);

		//�F�̐ݒ�
		g_E_Window.aUI[EVENTWINDOW_VTX_RED].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f);
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

		//�e�N�X�`�����W�̐ݒ�
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEUP].aTexU = 0.0f;
		g_E_Window.aUI[EVENTWINDOW_VTX_LINEDW].aTexU = 1.0f;

		g_E_Window.nStep = EVENTWINDOW_STEP_RED;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffE_Window->Unlock();
}

//=============================================================================
//�C�x���g�E�B���h�E�̏��擾
//=============================================================================
EVENTWINDOW *GetEventWindow(void)
{
	return &g_E_Window;
}