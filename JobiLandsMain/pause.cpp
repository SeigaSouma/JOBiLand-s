//=============================================================================
//
// �|�[�Y���� [pause.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "pause.h"
#include "game.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "player.h"

//�}�N����`
#define MAX_TOP			(4)				//���_��
#define POS_XSELECT		(270.0f)		//�I������X
#define POS_YSELECT		(100.0f)		//�I������Y
#define DIS_POSY		(180.0f)		//�I�����̊Ԋu

//�O���[�o���ϐ��錾
const char *c_apFilenamePause[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\black_fade.png",
	"data\\TEXTURE\\pause_menu.png",
	"data\\TEXTURE\\pause_game.png",
	"data\\TEXTURE\\pause_retry.png",
	"data\\TEXTURE\\pause_title.png",
};

LPDIRECT3DTEXTURE9 g_pTexturePause[(sizeof c_apFilenamePause) / sizeof(*c_apFilenamePause)] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPause = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3	g_posWindow;						//�|�[�Y�̈ʒu
D3DXVECTOR3	g_posSelect;						//�|�[�Y�̈ʒu
D3DXCOLOR g_colPause;							//�|�[�Y�̃J���[
PAUSEMENU g_PauseMenu;							//�|�[�Y���j���[�̑I����
int g_nPauseSelect;								//�|�[�Y�̌��݂̑I����
int g_FadeCount;                                //�t�F�[�h����J�E���g

//==================================================================================
//�|�[�Y�̏���������
//==================================================================================
void InitPause(void)
{
	g_posWindow = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//�E�B���h�E�ʒu
	g_posSelect = D3DXVECTOR3(640.0f, 240.0f, 0.0f);	//�I�����ʒu
	g_PauseMenu = PAUSEMENU_CONTINUE;					//���j���[�̏�����
	g_nPauseSelect = PAUSEMENU_CONTINUE;				//���j���[�̏�����
	g_colPause = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�|�[�Y�̃J���[
	g_FadeCount = 0;

														//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenamePause) / sizeof(*c_apFilenamePause); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenamePause[nCntTex],
			&g_pTexturePause[nCntTex]);

	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * PAUSEVTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPause,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[PAUSEVTX_FADE * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[PAUSEVTX_FADE * 4 + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[PAUSEVTX_FADE * 4 + 2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[PAUSEVTX_FADE * 4 + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[PAUSEVTX_FADE * 4 + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[PAUSEVTX_FADE * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[PAUSEVTX_FADE * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[PAUSEVTX_FADE * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	//���j���[�E�B���h�E
	pVtx[PAUSEVTX_WINDOW * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[PAUSEVTX_WINDOW * 4 + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[PAUSEVTX_WINDOW * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�Q�[���ɖ߂�I����
	pVtx[PAUSEVTX_CONTINUE * 4 + 0].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_CONTINUE * 4 + 1].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_CONTINUE * 4 + 2].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_CONTINUE * 4 + 3].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	g_posSelect.y += DIS_POSY;

	//���g���C�̑I����
	pVtx[PAUSEVTX_RETRY * 4 + 0].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_RETRY * 4 + 1].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_RETRY * 4 + 2].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_RETRY * 4 + 3].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	g_posSelect.y += DIS_POSY;

	//�^�C�g���ɖ߂�̑I����
	pVtx[PAUSEVTX_QUIT * 4 + 0].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_QUIT * 4 + 1].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y - POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_QUIT * 4 + 2].pos = D3DXVECTOR3(g_posSelect.x - POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);
	pVtx[PAUSEVTX_QUIT * 4 + 3].pos = D3DXVECTOR3(g_posSelect.x + POS_XSELECT, g_posSelect.y + POS_YSELECT, 0.0f);

	//pVtx += 4 * PAUSEVTX_CONTINUE;

	for (int nCount = 0; nCount < PAUSEVTX_MAX; nCount++)
	{
		if (nCount >= PAUSEVTX_CONTINUE)
		{//�I�����܂ŗ�����

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
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


		pVtx += 4;

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//==================================================================================
//�|�[�Y�̏I������
//==================================================================================
void UninitPause(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenamePause) / sizeof(*c_apFilenamePause); nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTexturePause[nCount] != NULL)
		{
			g_pTexturePause[nCount]->Release();
			g_pTexturePause[nCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPause != NULL)
	{
		g_pVtxBuffPause->Release();
		g_pVtxBuffPause = NULL;
	}
}

//==================================================================================
//�|�[�Y�̍X�V����
//==================================================================================
void UpdatePause(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	int nFade = GetFade();

	g_FadeCount++;

	if (nFade == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ����

	 //�I�����̍X�V����
		UpdateSelect(g_nPauseSelect);

		if (GetKeyboardTrigger(DIK_W) == true || GetGamepadTrigger(BUTTON_UP, 0))
		{//��n�������ꂽ

		 //�p�^�[��No.���X�V
			g_nPauseSelect = (g_nPauseSelect + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}
		else if (GetKeyboardTrigger(DIK_S) == true || GetGamepadTrigger(BUTTON_DOWN, 0))
		{//���n�������ꂽ

		 //�p�^�[��No.���X�V
			g_nPauseSelect = (g_nPauseSelect + 1) % PAUSEMENU_MAX;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}


		if (GetStickSelect() == false && YGetStickPressL(BUTTON_LY, 0) > 0)
		{//��ɓ|���ꂽ, �X�e�B�b�N�̔��肪OFF�̏ꍇ

		 //���X�e�B�b�N�̔����n��
			SetEnableStickSelect(true);

			//�p�^�[��No.���X�V
			g_nPauseSelect = (g_nPauseSelect + (PAUSEMENU_MAX - 1)) % PAUSEMENU_MAX;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}
		else if (GetStickSelect() == false && YGetStickPressL(BUTTON_LY, 0) < 0)
		{//���ɓ|���ꂽ, �X�e�B�b�N�̔��肪OFF�̏ꍇ

		 //���X�e�B�b�N�̔����n��
			SetEnableStickSelect(true);

			//�p�^�[��No.���X�V
			g_nPauseSelect = (g_nPauseSelect + 1) % PAUSEMENU_MAX;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_CURSOR);
		}


		if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0))
		{//���肪�����ꂽ

			switch (g_nPauseSelect)
			{
			case PAUSEMENU_CONTINUE:
				SetPause();
				SetEnablePause(false);		//�|�[�Y����������
				break;

			case PAUSEMENU_RETRY:

				//�T�E���h�Đ�
				//PlaySound(SOUND_LABEL_SE_AMURO);

				//�Q�[�������g���C����
				SetVoiceFade(MODE_GAME);

				
				break;

			case PAUSEMENU_LEVELSELECT:

				//�T�E���h�Đ�
				//PlaySound(SOUND_LABEL_SE_MODORU);

				//�Q�[�������g���C����
				SetVoiceFade(MODE_TITLE);
					
				break;

			}
		}

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();

}

//==================================================================================
//�|�[�Y���̑I�����X�V
//==================================================================================
void UpdateSelect(int PauseSelect)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPause->Lock(0, 0, (void**)&pVtx, 0);

	//�I�����܂Ń|�C���^�ړ�
	pVtx += 4 * PAUSEVTX_CONTINUE;

	static float fMoveAlpha = 0.008f;

	if (g_colPause.a > 1.0f)
	{//�s�����x100%
		g_colPause.a = 1.0f;
		fMoveAlpha *= -1;
	}
	else if (g_colPause.a < 0.4f)
	{//�s�����x20%
		g_colPause.a = 0.4f;
		fMoveAlpha *= -1;
	}

	//�s�����x�̍X�V
	g_colPause.a += fMoveAlpha;

	//�I��������n�߂�
	for (int nCount = PAUSEVTX_CONTINUE; nCount < PAUSEVTX_MAX; nCount++)
	{
		if (nCount != PauseSelect + PAUSEVTX_CONTINUE)
		{//���̑I��������Ȃ��Ƃ�
		 //���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			pVtx[0].col = g_colPause;
			pVtx[1].col = g_colPause;
			pVtx[2].col = g_colPause;
			pVtx[3].col = g_colPause;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPause->Unlock();
}

//==================================================================================
//�|�[�Y�̕`�揈��
//==================================================================================
void DrawPause(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^

										//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPause, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < PAUSEVTX_MAX; nCount++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTexturePause[nCount]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);
	}

}

//==================================================================================
//�|�[�Y�̐ݒ菈��
//==================================================================================
void SetPause(void)
{
	g_PauseMenu = PAUSEMENU_CONTINUE;		//���j���[�̏�����
	g_nPauseSelect = PAUSEMENU_CONTINUE;	//�I�����̏�����
}
