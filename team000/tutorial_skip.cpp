//=============================================================================
//
// �X�L�b�v�m�F���� [pause.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "tutorial.h"
#include "tutorial_skip.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "dispatch.h"
#include "buddy.h"
#include "player.h"

//�}�N����`
#define MAX_TOP			(4)				//���_��
#define POS_XSELECT		(270.0f)		//�I������X
#define POS_YSELECT		(100.0f)		//�I������Y
#define DIS_POSY		(430.0f)		//�I�����̊Ԋu

//�O���[�o���ϐ��錾
const char *c_apFilenameSkip[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\black_fade.png",
	"data\\TEXTURE\\tutorial_skip00.png",		//�w�i
	"data\\TEXTURE\\tutorial_skip01.png",		//�͂�
	"data\\TEXTURE\\tutorial_skip02.png",		//������

};

LPDIRECT3DTEXTURE9 g_pTextureSkip[(sizeof c_apFilenameSkip) / sizeof(*c_apFilenameSkip)] = {};//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSkip = NULL;	//���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3	g_posSkipWindow;					//�X�L�b�v�m�F�̈ʒu
D3DXVECTOR3	g_posSkipSelect;					//�X�L�b�v�m�F�̈ʒu
D3DXCOLOR g_colSkip;							//�X�L�b�v�m�F�̃J���[
SKIPMENU g_SkipMenu;							//�X�L�b�v�m�F���j���[�̑I����
int g_nSkipSelect;								//�X�L�b�v�m�F�̌��݂̑I����
int g_FadeSkipCount;                            //�t�F�[�h����J�E���g

//==================================================================================
//�X�L�b�v�m�F�̏���������
//==================================================================================
void InitTutorialSkip(void)
{
	g_posSkipWindow = D3DXVECTOR3(640.0f, 360.0f, 0.0f);	//�E�B���h�E�ʒu
	g_posSkipSelect = D3DXVECTOR3(420.0f, 440.0f, 0.0f);	//�I�����ʒu
	g_SkipMenu = SKIPMENU_SKIP;					//���j���[�̏�����
	g_nSkipSelect = SKIPMENU_SKIP;				//���j���[�̏�����
	g_colSkip = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//�X�L�b�v�m�F�̃J���[
	g_FadeSkipCount = 0;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSkip) / sizeof(*c_apFilenameSkip); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameSkip[nCntTex],
			&g_pTextureSkip[nCntTex]);

	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * MAX_TOP * SKIPVTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSkip,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSkip->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[SKIPVTX_FADE * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[SKIPVTX_FADE * 4 + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[SKIPVTX_FADE * 4 + 2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[SKIPVTX_FADE * 4 + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[SKIPVTX_FADE * 4 + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[SKIPVTX_FADE * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[SKIPVTX_FADE * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[SKIPVTX_FADE * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	//���j���[�E�B���h�E
	pVtx[SKIPVTX_WINDOW * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//���_�J���[�̐ݒ�
	pVtx[SKIPVTX_WINDOW * 4 + 0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[SKIPVTX_WINDOW * 4 + 3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//�X�L�b�v����
	pVtx[SKIPVTX_SKIP * 4 + 0].pos = D3DXVECTOR3(g_posSkipSelect.x - POS_XSELECT, g_posSkipSelect.y - POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_SKIP * 4 + 1].pos = D3DXVECTOR3(g_posSkipSelect.x + POS_XSELECT, g_posSkipSelect.y - POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_SKIP * 4 + 2].pos = D3DXVECTOR3(g_posSkipSelect.x - POS_XSELECT, g_posSkipSelect.y + POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_SKIP * 4 + 3].pos = D3DXVECTOR3(g_posSkipSelect.x + POS_XSELECT, g_posSkipSelect.y + POS_YSELECT, 0.0f);
	g_posSkipSelect.x += DIS_POSY;

	//�`���[�g���A��������
	pVtx[SKIPVTX_TUTORIAL * 4 + 0].pos = D3DXVECTOR3(g_posSkipSelect.x - POS_XSELECT, g_posSkipSelect.y - POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_TUTORIAL * 4 + 1].pos = D3DXVECTOR3(g_posSkipSelect.x + POS_XSELECT, g_posSkipSelect.y - POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_TUTORIAL * 4 + 2].pos = D3DXVECTOR3(g_posSkipSelect.x - POS_XSELECT, g_posSkipSelect.y + POS_YSELECT, 0.0f);
	pVtx[SKIPVTX_TUTORIAL * 4 + 3].pos = D3DXVECTOR3(g_posSkipSelect.x + POS_XSELECT, g_posSkipSelect.y + POS_YSELECT, 0.0f);

	//pVtx += 4 * SKIPVTX_CONTINUE;

	for (int nCount = 0; nCount < SKIPVTX_MAX; nCount++)
	{
		if (nCount >= SKIPVTX_SKIP)
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
	g_pVtxBuffSkip->Unlock();
}

//==================================================================================
//�X�L�b�v�m�F�̏I������
//==================================================================================
void UninitTutorialSkip(void)
{
	for (int nCount = 0; nCount < (sizeof c_apFilenameSkip) / sizeof(*c_apFilenameSkip); nCount++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureSkip[nCount] != NULL)
		{
			g_pTextureSkip[nCount]->Release();
			g_pTextureSkip[nCount] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSkip != NULL)
	{
		g_pVtxBuffSkip->Release();
		g_pVtxBuffSkip = NULL;
	}
}

//==================================================================================
//�X�L�b�v�m�F�̍X�V����
//==================================================================================
void UpdateTutorialSkip(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSkip->Lock(0, 0, (void**)&pVtx, 0);

	int nFade = GetFade();

	g_FadeSkipCount++;

	if (nFade == FADE_NONE)
	{//�t�F�[�h���Ă��Ȃ����

		//�I�����̍X�V����
		UpdateTutorialSelect(g_nSkipSelect);

		if (GetKeyboardTrigger(DIK_A) == true || GetGamepadTrigger(BUTTON_LEFT, 0))
		{//���n�������ꂽ

			//�p�^�[��No.���X�V
			g_nSkipSelect = (g_nSkipSelect + (SKIPMENU_MAX - 1)) % SKIPMENU_MAX;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
		}
		else if (GetKeyboardTrigger(DIK_D) == true || GetGamepadTrigger(BUTTON_RIGHT, 0))
		{//�E�n�������ꂽ

			//�p�^�[��No.���X�V
			g_nSkipSelect = (g_nSkipSelect + 1) % SKIPMENU_MAX;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
		}


		if (GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0)
		{//���ɓ|���ꂽ, �X�e�B�b�N�̔��肪OFF�̏ꍇ

			//���X�e�B�b�N�̔����n��
			SetEnableStickSelect(true);

			//�p�^�[��No.���X�V
			g_nSkipSelect = (g_nSkipSelect + (SKIPMENU_MAX - 1)) % SKIPMENU_MAX;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
		}
		else if (GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0)
		{//�E�ɓ|���ꂽ, �X�e�B�b�N�̔��肪OFF�̏ꍇ

			//���X�e�B�b�N�̔����n��
			SetEnableStickSelect(true);

			//�p�^�[��No.���X�V
			g_nSkipSelect = (g_nSkipSelect + 1) % SKIPMENU_MAX;

			//�T�E���h�Đ�
			PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
		}


		if (GetKeyboardTrigger(DIK_RETURN) == true || GetGamepadTrigger(BUTTON_A, 0))
		{//���肪�����ꂽ

			switch (g_nSkipSelect)
			{
			case SKIPMENU_SKIP:		//�X�L�b�v����

				//�T�E���h�Đ�
				PlaySound(SOUND_LABEL_SE_AMURO);

				//�Q�[��
				SetVoiceFade(MODE_GAME);

				break;

			case SKIPMENU_TUTORIAL:

				SetTutorialSkip();
				SetEnableSkip(false);		//�|�[�Y����������

				break;
			}
		}

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSkip->Unlock();



	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();

	//�h���J�[�\���̏��擾
	DISPATCH *pDispatch = GetDispatch();

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

				//�Z���t���Z�b�g
				//pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });
			}
			else if (pDispatch->nType == 1)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DISPATCHR);

				//�Z���t���Z�b�g
				//pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });
			}

			//�h������
			SetBuddyDispatch(pDispatch->pos, pDispatch->nType);
			pDispatch->bUse = false;
			pDispatch->nType = -1;
		}
	}

}

//==================================================================================
//�X�L�b�v�m�F���̑I�����X�V
//==================================================================================
void UpdateTutorialSelect(int SkipSelect)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSkip->Lock(0, 0, (void**)&pVtx, 0);

	//�I�����܂Ń|�C���^�ړ�
	pVtx += 4 * SKIPVTX_SKIP;

	static float fMoveAlpha = 0.008f;

	if (g_colSkip.a > 1.0f)
	{//�s�����x100%
		g_colSkip.a = 1.0f;
		fMoveAlpha *= -1;
	}
	else if (g_colSkip.a < 0.4f)
	{//�s�����x20%
		g_colSkip.a = 0.4f;
		fMoveAlpha *= -1;
	}

	//�s�����x�̍X�V
	g_colSkip.a += fMoveAlpha;

	//�I��������n�߂�
	for (int nCount = SKIPVTX_SKIP; nCount < SKIPVTX_MAX; nCount++)
	{
		if (nCount != SkipSelect + SKIPVTX_SKIP)
		{//���̑I��������Ȃ��Ƃ�
		 //���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
			pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}
		else
		{
			pVtx[0].col = g_colSkip;
			pVtx[1].col = g_colSkip;
			pVtx[2].col = g_colSkip;
			pVtx[3].col = g_colSkip;
		}

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSkip->Unlock();
}

//==================================================================================
//�X�L�b�v�m�F�̕`�揈��
//==================================================================================
void DrawTutorialSkip(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�ւ̃|�C���^

										//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffSkip, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCount = 0; nCount < SKIPVTX_MAX; nCount++)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureSkip[nCount]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * MAX_TOP, 2);
	}

}

//==================================================================================
//�X�L�b�v�m�F�̐ݒ菈��
//==================================================================================
void SetTutorialSkip(void)
{
	g_SkipMenu = SKIPMENU_SKIP;		//���j���[�̏�����
	g_nSkipSelect = SKIPMENU_SKIP;	//�I�����̏�����
}
