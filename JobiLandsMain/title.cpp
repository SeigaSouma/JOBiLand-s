//=============================================================================
//
// �^�C�g������ [title.cpp]
// Author : ���n�Ή�
//
//=============================================================================
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "title.h"
#include "edit.h"
#include "fade.h"
#include "game.h"
#include "light.h"
#include "camera.h"
#include "model.h"
#include "shadow.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "particle.h"
#include "effect.h"
#include "2D_effect.h"
#include "player.h"
#include "sound.h"
#include "titleselect.h"
#include "titlelogo.h"

//�}�N����`
#define CHANGE_TIME	(60 * 25)	//�؂�ւ��^�C�}�[
#define TITLETEX	"data/TEXTURE/title.png"
#define PRESSENTERTEX	"data/TEXTURE/pressenter.png"
#define FLASHING_TIME   (40)  //�_�Ŏ���
#define MAX_TITLE		(2)		//�e�N�X�`���̍ő吔

//=======================================
//�񋓌^
//=======================================
typedef enum
{
	TITLE_LOGO,  //�^�C�g�����S
	TITLE_ENTER, //PressEnter
	TITLE_MAX,
}TITLE;

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9 g_aTextureTitle[MAX_TITLE] = {};      //�e�N�X�`��(3����)�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;       //���_�o�b�t�@�ւ̃|�C���^
bool g_bEndFrag;									//�I���̃t���O
Title g_Title;
int g_EnterCounter;   //PressEnter�̓_�ŃJ�E���^�[
int g_EnterState;     //���݂̐F
bool g_TitleFade;     //false�Ɏg�p����Ƃ��ɕK�v

//==================================================================================
//�^�C�g���̏���������
//==================================================================================
void InitTitle(void)
{
	LPDIRECT3DDEVICE9  pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	g_Title.OldSelect = TITLESELECT_START;	//�O��̑I����
	g_Title.nSelect = TITLESELECT_START;	//����̑I����
	g_Title.nCntChange = 0;
	g_Title.nState = TITLESTATE_NONE;	//���
	g_bEndFrag = false;

	//�e�̏���������
	InitShadow();

	//���f���̏���������
	InitModel();

	//�G�f�B�b�g�̏�����
	InitEdit();

	//�^�C�g���̑I��������������
	InitTitleSelect();

	// �^�C�g���̃��S�̏���������
	InitTitleLogo();

	//�v���C���[�̏���������
	InitPlayer();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//�G�t�F�N�g�̏���������
	InitEffect();

	//2D�G�t�F�N�g�̏���������
	InitEffect_2D();

	//�p�[�e�B�N���̏���������
	InitParticle();

	//�^�C�g���̃J��������������
	InitCamera();

	//BGM���Z�b�g
	PlaySound(SOUND_LABEL_BGM_TITLE);

	//0�Ԗڂ̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TITLETEX, &g_aTextureTitle[0]);

	//1�Ԗڂ̃e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, PRESSENTERTEX, &g_aTextureTitle[1]);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TITLE,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//PressEnter�̓_�ŃJ�E���^�[
	g_EnterCounter = 0;

	//���݂̐F
	g_EnterState = 0;


	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MAX_TITLE; nCnt++)
	{

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();

}

//==================================================================================
//�^�C�g���̏I������
//==================================================================================
void UninitTitle(void)
{
	StopSound();

	//���f���̏I������
	UninitModel();

	//�G�f�B�b�g�̏I������
	UninitEdit();

	//�^�C�g���̑I�����I������
	UninitTitleSelect();

	// �^�C�g���̃��S�̏I������
	UninitTitleLogo();

	//�v���C���[�̏I������
	UninitPlayer();

	//�e�̏I������
	UninitShadow();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���E�H�[���̏I������
	UninitMeshDome();

	//�G�t�F�N�g�̏I������
	UninitEffect();

	//2D�G�t�F�N�g�̏I������
	UninitEffect_2D();

	//�p�[�e�B�N���̏I������
	UninitParticle();

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		//�e�N�X�`���̔j��
		if (g_aTextureTitle[nCntTitle] != NULL)
		{
			g_aTextureTitle[nCntTitle]->Release();

			g_aTextureTitle[nCntTitle] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffTitle != NULL)
	{
		g_pVtxBuffTitle->Release();
		g_pVtxBuffTitle = NULL;
	}

}

//==================================================================================
//�^�C�g���̍X�V����
//==================================================================================
void UpdateTitle(void)
{
	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntTitle = 0; nCntTitle < MAX_TITLE; nCntTitle++)
	{
		switch (nCntTitle)
		{
		case TITLE_LOGO:  //�^�C�g�����S
			break;
		case TITLE_ENTER: //PressEnter
			g_EnterCounter++;

			if (g_EnterCounter >= FLASHING_TIME)
			{
				g_EnterCounter = 0;

				g_EnterState ^= 1;

				if (g_EnterState == 1)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
				}
				if (g_EnterState == 0)
				{
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}
			}
			break;
		}

		//�e�N�X�`�����W�̍X�V
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTitle->Unlock();

	//����L�[(ENTER�L�[)�������ꂽ
	if (GetKeyboardTrigger(DIK_RETURN) == true/* || GetPadTrigger(BUTTON_A, 0) == true*/ && g_TitleFade == false)
	{
		//���[�h�ݒ�(�`���[�g���A���Ɉڍs)
		SetFade(MODE_TUTORIAL);

		g_TitleFade = true;
	}

	//�J�����̍X�V����
	UpdateCamera();

	//���f���̍X�V����
	UpdateModel();

	//�G�f�B�b�g�̍X�V����
	UpdateEdit();

	//�^�C�g���̑I�����X�V����
	UpdateTitleSelect();

	// �^�C�g���̃��S�̍X�V����
	UpdateTitleLogo();

	//�e�̍X�V����
	UpdateShadow();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//2D�G�t�F�N�g�̍X�V����
	UpdateEffect_2D();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	if (g_Title.nCntChange >= CHANGE_TIME)
	{//�����J�ڂ̃J�E���^�[���K��l�ɒB������

		//�����L���O�ɑJ��
		SetFade(MODE_RANKING);
	}
}

//==================================================================================
//�^�C�g���̕`�揈��
//==================================================================================
void DrawTitle(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{
		//���b�V���t�B�[���h�̕`�揈��
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���E�H�[���̕`�揈��
		DrawMeshDome();

		//�e�̕`�揈��
		DrawShadow();

		//���f���̕`�揈��
		DrawModel(0);

		//�G�f�B�b�g�̕`�揈��
		DrawEdit();

		// �^�C�g���̃��S�̕`�揈��
		DrawTitleLogo();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//�I�����̕`��
		DrawTitleSelect();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();

		LPDIRECT3DDEVICE9 pDevice = GetDevice();	//�f�o�C�X�̎擾

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCntTitle = 0; nCntTitle < 2; nCntTitle++)
		{
			pDevice->SetTexture(0, g_aTextureTitle[nCntTitle]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntTitle * 4, 2);
			
		}
	}

	if (nType == DRAWTYPE_UI)
	{
		//2D�G�t�F�N�g�̕`�揈��
		DrawEffect_2D();
	}
}

//==================================================================================
//�I�����ʂ̍X�V����
//==================================================================================
void UpdateSelectTitle(void)
{

	if (GetKeyboardTrigger(DIK_D) == true || GetGamepadTrigger(BUTTON_RIGHT, 0) ||
		(GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0))
	{//�E�n�������ꂽ

	 //���X�e�B�b�N�̔����n��
		SetEnableStickSelect(true);

		//�p�^�[��No.���X�V
		g_Title.nSelect = (g_Title.nSelect + 1) % TITLESELECT_MAX;

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange = 0;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_CURSOR);
	}
	else if (GetKeyboardTrigger(DIK_A) == true || GetGamepadTrigger(BUTTON_LEFT, 0) ||
		(GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0))
	{//���n�������ꂽ

	 //���X�e�B�b�N�̔����n��
		SetEnableStickSelect(true);

		//�p�^�[��No.���X�V
		g_Title.nSelect = (g_Title.nSelect + (TITLESELECT_MAX - 1)) % TITLESELECT_MAX;

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange = 0;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_CURSOR);
	}


	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0))
	{//Enter�������ꂽ

		if (g_Title.nSelect == TITLESELECT_START)
		{//�X�^�[�g�̎������`�J�`�J

			//�`�J�`�J��ԂɈڍs
			g_Title.nState = TITLESTATE_FADE;
		}
		else
		{//���͂����J��

			//�t�F�[�h��ԂɈڍs
			g_Title.nState = TITLESTATE_FADE;
		}

		//�T�E���h�Đ�
		//PlaySound(SOUND_LABEL_SE_SYUTUGEKI);
	}

	//�I�����̐F�X�V
	//UpdateSelectTitleCol(g_Title.nSelect);

}

//==================================================================================
//�I���̃t���O���擾
//==================================================================================
bool GetEndFrag(void)
{
	return g_bEndFrag;
}

//==================================================================================
//�^�C�g���̏��擾
//==================================================================================
Title *GetTitle(void)
{
	return &g_Title;
}