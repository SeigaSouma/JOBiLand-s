//=============================================================================
//
// �ŏ��̉�b���� [start_talk.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "start_talk.h"
#include "game.h"
#include "input.h"
#include "sound.h"
#include "calculation.h"
#include "updown_fram.h"
#include "enemy.h"

//�}�N����`
#define MAX_TOP			(4)		//���_��
#define WIDTH		(470.0f)
#define HEIGHT		(110.0f)
#define WINDOW_WIDTH	(350.0f)
#define WINDOW_HEIGHT	(150.0f)
#define SKIP_WIDTH	(160.0f * 1.2f)
#define SKIP_HEIGHT	(60.0f * 1.2f)
#define ALPHA_MOVE	(0.02f)

//�v���g�^�C�v�錾
void CreatVtxTextStartTalk(void);
void CreatVtxWindowStartTalk(void);
void UpdateTextStartTalk(void);
void UpdateFadeStartTalk(void);
void UpdateSkipStartTalk(void);
void UpdateEndStartTalk(void);
void UpdateTargetWindow(void);
void DrawStartTalkBuff(void);
void DrawTargetWindowBuff(void);

//�O���[�o���ϐ��錾
const char *c_apFilenameStartTalk[] =		//�t�@�C���ǂݍ���
{
	NULL,
	"data\\TEXTURE\\talk_011.png",
	"data\\TEXTURE\\talk_022.png",
	"data\\TEXTURE\\talk_033.png",
	"data\\TEXTURE\\talk_044.png",
	"data\\TEXTURE\\talk_055.png",
	"data\\TEXTURE\\talk_skip.png",
};

const char *c_apFilenameTargetWindow[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\targetwindow_01.png",
};
LPDIRECT3DTEXTURE9 g_pTextureStartTalk[(sizeof c_apFilenameStartTalk) / sizeof(*c_apFilenameStartTalk)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureTargetWindow[(sizeof c_apFilenameTargetWindow) / sizeof(*c_apFilenameTargetWindow)] = {};			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStartTalk = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTargetWindow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
StartTalk g_aStartTalk[START_TALK_VTX_MAX];						//�ŏ��̉�b�̏��
int g_nStartTalkStep;	//�X�e�b�v
bool g_bEndStartTalk;	//�I����Ԃ��ǂ���
int g_nIdxMoveLogTargetWindow;	//����UI�̃C���f�b�N�X�ԍ�
bool g_bUseTargetWindow;	//�g�p���Ă��邩

//==================================================================================
//�ŏ��̉�b�̏���������
//==================================================================================
void InitStartTalk(void)
{

	//�e�ϐ�������
	for (int nCntTalk = 0; nCntTalk < START_TALK_VTX_MAX; nCntTalk++)
	{
		g_aStartTalk[nCntTalk].pos = D3DXVECTOR3(640.0f, 500.0f, 0.0f);
		g_aStartTalk[nCntTalk].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		//�F
		g_aStartTalk[nCntTalk].bUse = false;	//�g�p���Ă��Ȃ���Ԃɂ���
	}
	g_aStartTalk[START_TALK_VTX_FADE].pos = D3DXVECTOR3(640.0f, 360.0f, 0.0f);		//���̈ʒu
	g_aStartTalk[START_TALK_VTX_FADE].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);		//�F
	g_aStartTalk[START_TALK_VTX_SKIP].pos = D3DXVECTOR3(1100.0f, 650.0f, 0.0f);
	g_aStartTalk[START_TALK_VTX_SKIP].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.4f);		//�F
	g_nStartTalkStep = -1;
	g_bEndStartTalk = false;	//�I����Ԃ��ǂ���

	//�������S�̃C���f�b�N�X�ԍ��擾
	g_nIdxMoveLogTargetWindow = SetMoveLog();

	//�E�B���h�E�̎g�p��
	g_bUseTargetWindow = false;

	//��b�̒��_�o�b�t�@����
	CreatVtxTextStartTalk();

	//�E�B���h�E�̒��_�o�b�t�@����
	CreatVtxWindowStartTalk();

}

//==================================================================================
//�ŏ��̉�b�̏I������
//==================================================================================
void UninitStartTalk(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameStartTalk) / sizeof(*c_apFilenameStartTalk); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureStartTalk[nCntTex] != NULL)
		{
			g_pTextureStartTalk[nCntTex]->Release();
			g_pTextureStartTalk[nCntTex] = NULL;
		}
	}

	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTargetWindow) / sizeof(*c_apFilenameTargetWindow); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureTargetWindow[nCntTex] != NULL)
		{
			g_pTextureTargetWindow[nCntTex]->Release();
			g_pTextureTargetWindow[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffStartTalk != NULL)
	{
		g_pVtxBuffStartTalk->Release();
		g_pVtxBuffStartTalk = NULL;
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTargetWindow != NULL)
	{
		g_pVtxBuffTargetWindow->Release();
		g_pVtxBuffTargetWindow = NULL;
	}
}

//==================================================================================
//�ŏ��̉�b�̍X�V����
//==================================================================================
void UpdateStartTalk(void)
{
	if (GetGameState() == GAMESTATE_START)
	{
		if (g_bEndStartTalk == false && g_aStartTalk[START_TALK_VTX_TEXT1].bUse == true && (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0)))
		{//�I�����Ă��Ȃ� && �g�p����Ă��� && Enter��A

			SetStartTalk();
		}

		if (g_bEndStartTalk == false && g_aStartTalk[START_TALK_VTX_TEXT1].bUse == true && (GetKeyboardTrigger(DIK_BACKSPACE) == true || GetGamepadTrigger(BUTTON_START, 0)))
		{
			g_bEndStartTalk = true;

			//�g�̏�ԕύX
			GetUpDownFram()->nState = UPDOWN_FRAM_STATE_OUT;
		}

		if (g_bEndStartTalk == true)
		{//�I�����Ă�����

			UpdateEndStartTalk();
		}
		else
		{
			//�e�v�f�̍X�V����
			UpdateTextStartTalk();
			UpdateFadeStartTalk();
			UpdateSkipStartTalk();
		}

		//�E�B���h�E�̍X�V����
		UpdateTargetWindow();
	}
}

//==================================================================================
//�ŏ��̉�b�̃e�L�X�g�X�V����
//==================================================================================
void UpdateTextStartTalk(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * START_TALK_VTX_TEXT1;

	for (int nCntTalk = START_TALK_VTX_TEXT1; nCntTalk < START_TALK_VTX_SKIP; nCntTalk++)
	{
		if (g_aStartTalk[nCntTalk].bUse == true)
		{//�g�p����Ă�����

			//�s�����x�̕␳
			if (g_aStartTalk[nCntTalk].col.a < 1.0f)
			{
				g_aStartTalk[nCntTalk].col.a += 0.05f;
			}
			else
			{//1.0f�ȏ�ŌŒ�

				g_aStartTalk[nCntTalk].col.a = 1.0f;
			}

			//��Ɉړ�������
			g_aStartTalk[nCntTalk].pos.y -= 15.0f;

			//����鏈��
			if ((400.0f + (nCntTalk - 1) * 240.0f) - 240.0f * g_nStartTalkStep > g_aStartTalk[nCntTalk].pos.y)
			{
				g_aStartTalk[nCntTalk].pos.y = (400.0f + (nCntTalk - 1) * 240.0f) - 240.0f * g_nStartTalkStep;
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aStartTalk[nCntTalk].col;
			pVtx[1].col = g_aStartTalk[nCntTalk].col;
			pVtx[2].col = g_aStartTalk[nCntTalk].col;
			pVtx[3].col = g_aStartTalk[nCntTalk].col;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//�ŏ��̉�b�̖��X�V����
//==================================================================================
void UpdateFadeStartTalk(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aStartTalk[START_TALK_VTX_FADE].bUse == true)
	{//�g�p����Ă�����

		//�s�����x�̕␳
		if (g_aStartTalk[START_TALK_VTX_FADE].col.a < 0.3f)
		{
			g_aStartTalk[START_TALK_VTX_FADE].col.a += 0.025f;
		}
		else
		{//0.3f�ȏ�ŌŒ�

			g_aStartTalk[START_TALK_VTX_FADE].col.a = 0.3f;

			if (g_aStartTalk[START_TALK_VTX_TEXT1].bUse == false)
			{
				//��b�̐ݒ�
				SetStartTalk();

				g_aStartTalk[START_TALK_VTX_SKIP].bUse = true;
			}
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aStartTalk[START_TALK_VTX_FADE].col;
		pVtx[1].col = g_aStartTalk[START_TALK_VTX_FADE].col;
		pVtx[2].col = g_aStartTalk[START_TALK_VTX_FADE].col;
		pVtx[3].col = g_aStartTalk[START_TALK_VTX_FADE].col;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//�ŏ��̉�b�̃X�L�b�v�X�V����
//==================================================================================
void UpdateSkipStartTalk(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aStartTalk[START_TALK_VTX_SKIP].bUse == true)
	{//�g�p����Ă�����

		static float fMoveAlpha = 0.01f;

		if (g_aStartTalk[START_TALK_VTX_SKIP].col.a > 1.0f)
		{//�s�����x100%

			g_aStartTalk[START_TALK_VTX_SKIP].col.a = 1.0f;
			fMoveAlpha *= -1;
		}
		else if (g_aStartTalk[START_TALK_VTX_SKIP].col.a < 0.4f)
		{//�s�����x20%

			g_aStartTalk[START_TALK_VTX_SKIP].col.a = 0.4f;
			fMoveAlpha *= -1;
		}

		//�s�����x�̍X�V
		g_aStartTalk[START_TALK_VTX_SKIP].col.a += fMoveAlpha;

		//���_�J���[�̐ݒ�
		pVtx[START_TALK_VTX_SKIP * 4 + 0].col = g_aStartTalk[START_TALK_VTX_SKIP].col;
		pVtx[START_TALK_VTX_SKIP * 4 + 1].col = g_aStartTalk[START_TALK_VTX_SKIP].col;
		pVtx[START_TALK_VTX_SKIP * 4 + 2].col = g_aStartTalk[START_TALK_VTX_SKIP].col;
		pVtx[START_TALK_VTX_SKIP * 4 + 3].col = g_aStartTalk[START_TALK_VTX_SKIP].col;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//�ŏ��̉�b�̏I��(�t�F�[�h)����
//==================================================================================
void UpdateEndStartTalk(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	int nCntUse = 0;
	for (int nCntTalk = 0; nCntTalk < START_TALK_VTX_MAX; nCntTalk++)
	{
		if (g_aStartTalk[nCntTalk].bUse == true)
		{//�g�p����Ă�����

			//�d�l�J�E���g���Z
			nCntUse++;

			//�s�����x�̍X�V
			g_aStartTalk[nCntTalk].col.a -= 0.025f;

			//�s�����x�̕␳
			if (g_aStartTalk[nCntTalk].col.a < 0.0f)
			{
				g_aStartTalk[nCntTalk].col.a = 0.0f;
				g_aStartTalk[nCntTalk].bUse = false;
			}

			//���_�J���[�̐ݒ�
			pVtx[0].col = g_aStartTalk[nCntTalk].col;
			pVtx[1].col = g_aStartTalk[nCntTalk].col;
			pVtx[2].col = g_aStartTalk[nCntTalk].col;
			pVtx[3].col = g_aStartTalk[nCntTalk].col;
		}

		pVtx += 4;
	}

	if (nCntUse == 0 && g_bUseTargetWindow == false)
	{//�S���g���Ă��Ȃ�������

		//�E�B���h�E�̐ݒ菈��
		SetTartgetWindow();
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//�ڕW�g�̍X�V����
//==================================================================================
void UpdateTargetWindow(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTargetWindow->Lock(0, 0, (void**)&pVtx, 0);

	if (g_bUseTargetWindow == true)
	{//�g�p����Ă�����

		//�傫���Ȃ遨�k�ށ��傫���Ȃ��ċK��l
		float fWidth = MoveLog(g_nIdxMoveLogTargetWindow, WINDOW_WIDTH, 15);
		float fHeight = MoveLog(g_nIdxMoveLogTargetWindow, WINDOW_HEIGHT, 15);

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(640.0f - fWidth, 360.0f - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(640.0f + fWidth, 360.0f - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(640.0f - fWidth, 360.0f + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(640.0f + fWidth, 360.0f + fHeight, 0.0f);

		if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0) ||
			GetKeyboardTrigger(DIK_BACKSPACE) == true || GetGamepadTrigger(BUTTON_START, 0))
		{//�I�����Ă��Ȃ� && �g�p����Ă��� && Enter��A

			g_bUseTargetWindow = false;
			SetGameState(GAMESTATE_NONE, 0);
			ReadSetEnemy();
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTargetWindow->Unlock();
}

//==================================================================================
//�ŏ��̉�b�̕`�揈��
//==================================================================================
void DrawStartTalk(void)
{
	//�e�L�X�g�`�揈��
	DrawStartTalkBuff();

	//�E�B���h�E�`�揈��
	DrawTargetWindowBuff();
}

//==================================================================================
//�ŏ��̉�b�̃e�L�X�g�`�揈��
//==================================================================================
void DrawStartTalkBuff(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffStartTalk, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�L�X�g�̕`��
	for (int nCntTalk = 0; nCntTalk < START_TALK_VTX_MAX; nCntTalk++)
	{
		if (g_aStartTalk[nCntTalk].bUse == true)
		{//�g�p����Ă�����

		 //�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureStartTalk[nCntTalk]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTalk * 4, 2);
		}
	}
}

//==================================================================================
//�ŏ��̉�b�̃E�B���h�E�`�揈��
//==================================================================================
void DrawTargetWindowBuff(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTargetWindow, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�E�B���h�E�̕`��
	if (g_bUseTargetWindow == true)
	{//�g�p����Ă�����

		 //�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTargetWindow[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

//==================================================================================
//�ŏ��̉�b�ݒ菈��
//==================================================================================
void SetStartTalk(void)
{

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);
	pVtx += 4;

	int nCntUse = 0;
	for (int nCntTalk = START_TALK_VTX_TEXT1; nCntTalk < START_TALK_VTX_SKIP; nCntTalk++)
	{
		if (g_aStartTalk[nCntTalk].bUse == false)
		{//�g�p���Ă��Ȃ�������

			//�g�p�J�E���g���Z
			nCntUse++;

			g_aStartTalk[nCntTalk].pos.y = 640.0f;
			g_aStartTalk[nCntTalk].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.3f);		//�F
			g_aStartTalk[nCntTalk].bUse = true;
			g_aStartTalk[START_TALK_VTX_FADE].bUse = true;

			if (nCntTalk == START_TALK_VTX_TEXT1 || nCntTalk == START_TALK_VTX_TEXT5)
			{
				PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
			}

			if (nCntTalk == START_TALK_VTX_TEXT3)
			{
				PlaySound(SOUND_LABEL_SE_DHUFU);
			}

			//�X�e�b�v���Z
			g_nStartTalkStep++;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
			break;
		}

		pVtx += 4;
	}

	if (nCntUse == 0)
	{//�S�Ďg�p���Ă�����

		//�I������
		g_bEndStartTalk = true;

		//�g�̏�ԕύX
		GetUpDownFram()->nState = UPDOWN_FRAM_STATE_OUT;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//�ŏ��̉�b�ڕW�E�B���h�E�\��
//==================================================================================
void SetTartgetWindow(void)
{
	if (g_bUseTargetWindow == false)
	{
		g_bUseTargetWindow = true;
	}
}

//==================================================================================
//�e�L�X�g�̒��_�o�b�t�@����
//==================================================================================
void CreatVtxTextStartTalk(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameStartTalk) / sizeof(*c_apFilenameStartTalk); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameStartTalk[nCntTex],
			&g_pTextureStartTalk[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * START_TALK_VTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffStartTalk,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffStartTalk->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTalk = 0; nCntTalk < START_TALK_VTX_MAX; nCntTalk++)
	{
		if (nCntTalk == START_TALK_VTX_FADE)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - 640.0f, g_aStartTalk[nCntTalk].pos.y - 360.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + 640.0f, g_aStartTalk[nCntTalk].pos.y - 360.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - 640.0f, g_aStartTalk[nCntTalk].pos.y + 360.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + 640.0f, g_aStartTalk[nCntTalk].pos.y + 360.0f, 0.0f);
		}
		else if (nCntTalk == START_TALK_VTX_SKIP)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - SKIP_WIDTH, g_aStartTalk[nCntTalk].pos.y - SKIP_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + SKIP_WIDTH, g_aStartTalk[nCntTalk].pos.y - SKIP_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - SKIP_WIDTH, g_aStartTalk[nCntTalk].pos.y + SKIP_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + SKIP_WIDTH, g_aStartTalk[nCntTalk].pos.y + SKIP_HEIGHT, 0.0f);
		}
		else
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x - WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aStartTalk[nCntTalk].pos.x + WIDTH, g_aStartTalk[nCntTalk].pos.y + HEIGHT, 0.0f);
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_aStartTalk[nCntTalk].col;
		pVtx[1].col = g_aStartTalk[nCntTalk].col;
		pVtx[2].col = g_aStartTalk[nCntTalk].col;
		pVtx[3].col = g_aStartTalk[nCntTalk].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;	//���_�f�[�^�̃|�C���^�𒸓_�����i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffStartTalk->Unlock();
}

//==================================================================================
//�E�B���h�E�̒��_�o�b�t�@����
//==================================================================================
void CreatVtxWindowStartTalk(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTargetWindow) / sizeof(*c_apFilenameTargetWindow); nCntTex++)
	{
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTargetWindow[nCntTex],
			&g_pTextureTargetWindow[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTargetWindow,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTargetWindow->Lock(0, 0, (void**)&pVtx, 0);
	
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(640.0f - WINDOW_WIDTH, 360.0f - WINDOW_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(640.0f + WINDOW_WIDTH, 360.0f - WINDOW_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(640.0f - WINDOW_WIDTH, 360.0f + WINDOW_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(640.0f + WINDOW_WIDTH, 360.0f + WINDOW_HEIGHT, 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTargetWindow->Unlock();
}

//==================================================================================
//�ŏ��̉�b���擾
//==================================================================================
StartTalk *GetStartTalk(void)
{
	return &g_aStartTalk[0];
}