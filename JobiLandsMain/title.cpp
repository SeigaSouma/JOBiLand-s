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
#include "buddy.h"
#include "sound.h"
#include "titleselect.h"

#define MIN_ALPHA	(0.4f)		//�s�����x�ŏ��l
#define MAX_ALPHA	(1.0f)		//�s�����x�ő�l
#define CHANGE_TIME	(60 * 25)	//�؂�ւ��^�C�}�[
#define MAX_TKTK	(25)		//�`�J�`�J�̍ő吔
#define DISTANCE_X	(500.0f)	//�Ԋu
#define POS_X		(180.0f)	//����X
#define POS_Y		(100.0f)	//����Y
#define LOG_WIDTH	(400.0f)
#define LOG_HEIGHT	(180.0f)
#define START_POSX	(390.0f)
#define MOVE_TIME	(20)

//�v���g�^�C�v�錾
void UpdateMoveLog(void);

//�O���[�o���ϐ��錾
const char *c_apFilenameTitle[] =					//�t�@�C���ǂݍ���
{
	NULL,
	"data\\TEXTURE\\titlelog_01.png",
	"data\\TEXTURE\\titlelog_02.png",
	"data\\TEXTURE\\titlelog_03.png",
	"data\\TEXTURE\\titlelog_04.png",
	"data\\TEXTURE\\titlelog_00.png",
};
LPDIRECT3DTEXTURE9 g_pTextureTitle[(sizeof c_apFilenameTitle) / sizeof(*c_apFilenameTitle)] = {};	//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTitle = NULL;		//���_�o�b�t�@�ւ̃|�C���^
bool g_bEndFrag;									//�I���̃t���O
Title g_Title;

//==================================================================================
//�^�C�g���̏���������
//==================================================================================
void InitTitle(void)
{
	g_Title.nTKTKFrame = 0;					//�`�J�`�J�̃t���[���J�E���g
	g_Title.nTKTKCounter = 0;				//�`�J�`�J�̉�
	g_Title.OldSelect = TITLESELECT_START;	//�O��̑I����
	g_Title.nSelect = TITLESELECT_START;	//����̑I����
	g_Title.nCntChange = 0;
	g_Title.nCntMoveLog = 0;
	g_Title.fVolume = 1.0f;
	g_Title.bMoveLog = false;
	g_Title.col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	for (int nCntLog = 0; nCntLog < TITLELOG_MAX; nCntLog++)
	{
		g_Title.Logcol[nCntLog] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);
	}

	g_Title.Logcol[TITLELOG_LINE] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Title.Logcol[TITLELOG_BLACK] = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	g_Title.pos = D3DXVECTOR3(START_POSX, 580.0f, 0.0f);
	g_Title.posLog[TITLELOG_BLACK] = D3DXVECTOR3(640.0f, 360.0f, 0.0f);
	g_Title.posLog[TITLELOG_LOG] = D3DXVECTOR3(640.0f, SCREEN_HEIGHT + 360.0f, 0.0f);
	g_Title.posLog[TITLELOG_LINE] = D3DXVECTOR3(-640.0f, 360.0f, 0.0f);
	g_Title.posLog[TITLELOG_LIBELLION] = D3DXVECTOR3(SCREEN_WIDTH + 640.0f, 360.0f, 0.0f);
	g_Title.posLog[TITLELOG_HONKI] = D3DXVECTOR3(640.0f, -360.0f, 0.0f);

	g_Title.posLog[TITLELOG_TITLELOGO] = D3DXVECTOR3(640.0f, -360.0f, 0.0f);
	g_Title.Logcol[TITLELOG_TITLELOGO] = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	g_Title.nType = 0;

	g_Title.nState = TITLESTATE_MOVELOG;	//���
	g_bEndFrag = false;

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameTitle) / sizeof(*c_apFilenameTitle); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameTitle[nCntTex],
			&g_pTextureTitle[nCntTex]);
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * TITLEVTX_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTitle,
		NULL);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);


	for (int nCount = 0; nCount < TITLEVTX_MAX; nCount++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_Title.posLog[nCount].x - LOG_WIDTH, g_Title.posLog[nCount].y - LOG_HEIGHT, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Title.posLog[nCount].x + LOG_WIDTH, g_Title.posLog[nCount].y - LOG_HEIGHT, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Title.posLog[nCount].x - LOG_WIDTH, g_Title.posLog[nCount].y + LOG_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Title.posLog[nCount].x + LOG_WIDTH, g_Title.posLog[nCount].y + LOG_HEIGHT, 0.0f);

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_Title.Logcol[nCount];
		pVtx[1].col = g_Title.Logcol[nCount];
		pVtx[2].col = g_Title.Logcol[nCount];
		pVtx[3].col = g_Title.Logcol[nCount];
	

		//rhw�̐ݒ�
		pVtx[0].rhw =
		pVtx[1].rhw =
		pVtx[2].rhw =
		pVtx[3].rhw = 1.0f;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//�e�̏���������
	InitShadow();

	//���f���̏���������
	InitModel();

	//�G�f�B�b�g�̏�����
	InitEdit();

	//�^�C�g���̑I��������������
	InitTitleSelect();

	//�v���C���[�̏���������
	InitPlayer();

	//���_�̏���������
	InitBuddy();

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
	InitTitleCamera();

	//BGM���Z�b�g
	PlaySound(SOUND_LABEL_BGM_TITLE);
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

	//�v���C���[�̏I������
	UninitPlayer();

	//���_�̏I������
	UninitBuddy();

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

	for (int nCount = 0; nCount < (sizeof c_apFilenameTitle) / sizeof(*c_apFilenameTitle); nCount++)
	{
		if (g_pTextureTitle[nCount] != NULL)
		{
			g_pTextureTitle[nCount]->Release();
			g_pTextureTitle[nCount] = NULL;
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

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (GetFade() == FADE_NONE && g_Title.nState == TITLESTATE_MOVELOG)
	{//�������Ă��Ȃ��Ƃ�

		//���S�̈ړ���������
		UpdateMoveLog();

	}
	else if (GetFade() == FADE_NONE && g_Title.nState == TITLESTATE_NONE)
	{//�������Ă��Ȃ��Ƃ�

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange++;

		//�I������
		UpdateSelectTitle();

	}
	else if (g_Title.nState == TITLESTATE_TKTK)
	{//�`�J�`�J��Ԃ̂Ƃ�

		//�I�����̃`�J�`�J����
		UpdateTitleTKTK();
	}
	else if (g_Title.nState == TITLESTATE_FADE)
	{//�J�ڏ�Ԃ̂Ƃ�

		switch (g_Title.nSelect)
		{

		case TITLESELECT_START:
			SetFade(MODE_TUTORIAL);
			break;

		case TITLESELECT_END:
			g_bEndFrag = true;	//�I���̃t���OON
			break;

		}

	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();

	//�J�����̍X�V����
	UpdateCamera();

	//���f���̍X�V����
	UpdateModel();

	//�G�f�B�b�g�̍X�V����
	UpdateEdit();

	//�^�C�g���̑I�����X�V����
	UpdateTitleSelect();

	//�v���C���[�̍X�V����
	UpdateTitlePlayer();

	//���_�̍X�V����
	UpdateTitleBuddy();

	//�e�̍X�V����
	UpdateShadow();

	//�G�t�F�N�g�̍X�V����
	UpdateEffect();

	//2D�G�t�F�N�g�̍X�V����
	UpdateEffect_2D();

	//�p�[�e�B�N���̍X�V����
	UpdateParticle();

	if (g_Title.nCntChange >= CHANGE_TIME)
	{

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

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//���_�̕`�揈��
		DrawBuddy();

		//�I�����̕`��
		DrawTitleSelect();

		//�G�t�F�N�g�̕`�揈��
		DrawEffect();

		//�p�[�e�B�N���̕`�揈��
		DrawParticle();
	}

	if (nType == DRAWTYPE_UI)
	{
		//�f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = GetDevice();

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureTitle[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);


		//2D�G�t�F�N�g�̕`�揈��
		DrawEffect_2D();


		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffTitle, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		for (int nCount = 1; nCount < TITLEVTX_MAX; nCount++)
		{
			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureTitle[nCount]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCount * 4, 2);
		}
	}
}

//==================================================================================
//���S�̈ړ�����
//==================================================================================
void UpdateMoveLog(void)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�J�����̍X�V����
	UpdateTitleCamera(CAMERATYPE_MAIN);

	//���_���ւ̃|�C���^
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	if (g_Title.nCntMoveLog <= 30 && g_Title.nType == 0)
	{//�܂��K��l�ɒB���Ă��Ȃ��Ƃ�

		//���S�̓������ԉ��Z
		g_Title.nCntMoveLog++;
	}
	else if (g_Title.nCntMoveLog > 20 && g_Title.nType == 0)
	{
		g_Title.nType = 1;
		g_Title.nCntMoveLog = 0;
	}

	if (g_Title.nCntMoveLog <= 60 && g_Title.nType == 1)
	{//�܂��K��l�ɒB���Ă��Ȃ��Ƃ�

		//���S�̓������ԉ��Z
		g_Title.nCntMoveLog++;

		g_Title.posLog[TITLELOG_LINE].x += SCREEN_WIDTH / 60;

		//�G�t�F�N�g�̐ݒ�
		SetEffect_2D(D3DXVECTOR3(g_Title.posLog[TITLELOG_LINE].x + 600.0f, g_Title.posLog[TITLELOG_LINE].y + 60.0f, g_Title.posLog[TITLELOG_LINE].z),
			D3DXCOLOR(0.1f, 0.5f, 0.9f, 1.0f),
			40.0f,
			10,
			D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
			MOVEEFFECT_NONE, EFFECTTYPE_NORMAL);

		SetEffect_2D(D3DXVECTOR3(g_Title.posLog[TITLELOG_LINE].x + 600.0f, g_Title.posLog[TITLELOG_LINE].y + 60.0f, g_Title.posLog[TITLELOG_LINE].z),
			D3DXCOLOR(0.1f, 0.5f, 0.9f, 1.0f),
			40.0f,
			10,
			D3DXVECTOR3(-10.0f, 0.0f, 0.0f),
			MOVEEFFECT_NONE, EFFECTTYPE_NORMAL);
	}
	else if (g_Title.nCntMoveLog > 60 && g_Title.nType == 1)
	{
		g_Title.nType = 2;
		g_Title.nCntMoveLog = 0;
	}

	if (g_Title.nCntMoveLog < MOVE_TIME && g_Title.nType == 2)
	{//�܂��K��l�ɒB���Ă��Ȃ��Ƃ�

		//���S�̓������ԉ��Z
		g_Title.nCntMoveLog++;

		//�s�����x���Z
		g_Title.Logcol[TITLELOG_BLACK].a += 0.05f;

		g_Title.posLog[TITLELOG_LOG].y -= SCREEN_HEIGHT / MOVE_TIME;
		g_Title.posLog[TITLELOG_HONKI].y += SCREEN_HEIGHT / MOVE_TIME;
		g_Title.posLog[TITLELOG_LIBELLION].x -= SCREEN_WIDTH / MOVE_TIME;

		g_Title.Logcol[TITLELOG_LOG].a += 1.0f / (float)MOVE_TIME;
		g_Title.Logcol[TITLELOG_HONKI].a += 1.0f / (float)MOVE_TIME;
		g_Title.Logcol[TITLELOG_LIBELLION].a += 1.0f / (float)MOVE_TIME;

		if (g_Title.Logcol[TITLELOG_LOG].a >= 1.0f)
		{
			g_Title.Logcol[TITLELOG_LOG].a = 1.0f;
			g_Title.Logcol[TITLELOG_HONKI].a = 1.0f;
			g_Title.Logcol[TITLELOG_LIBELLION].a = 1.0f;
		}

		if (g_Title.Logcol[TITLELOG_BLACK].a >= 1.0f)
		{
			g_Title.Logcol[TITLELOG_BLACK].a = 1.0f;
		}

		if (g_Title.nCntMoveLog == 12)
		{
			SetParticle(D3DXVECTOR3(640.0f, 360.0f, 0.0f), PARTICLE_TYPE_TITLELOG);
		}

	}
	else if (g_Title.nCntMoveLog >= MOVE_TIME && g_Title.nType == 2)
	{
		pCamera->nMoveStep = 1;
		g_Title.nCntMoveLog++;

		//�^�C�g���̑I�����Z�b�g
		SetTitleSelect(D3DXVECTOR3(-100.0f, 0.0f, -300.0f), 25);
		SetTitleSelect(D3DXVECTOR3(100.0f, 0.0f, -300.0f), 26);

		if (g_Title.nCntMoveLog >= 150)
		{
			g_Title.nType = 3;
			g_Title.nCntMoveLog = 0;

			pPlayer->bUse = true;
			pPlayer->nIdxShadow = SetShadow(pPlayer->fRadius * 0.5f, pPlayer->fRadius * 0.5f);
			SetPositionShadow(pPlayer->nIdxShadow, pPlayer->pos);
			pBuddy[BUDDYTYPE_HYOROGARI].bUse = true;
			pBuddy[BUDDYTYPE_DEPPA].bUse = true;

			pBuddy[BUDDYTYPE_HYOROGARI].nIdxShadow = SetShadow(pBuddy[BUDDYTYPE_HYOROGARI].fRadius * 0.5f, pBuddy[BUDDYTYPE_HYOROGARI].fRadius * 0.5f);
			SetPositionShadow(pBuddy[BUDDYTYPE_HYOROGARI].nIdxShadow, pBuddy[BUDDYTYPE_HYOROGARI].pos);

			pBuddy[BUDDYTYPE_DEPPA].nIdxShadow = SetShadow(pBuddy[BUDDYTYPE_DEPPA].fRadius * 0.5f, pBuddy[BUDDYTYPE_DEPPA].fRadius * 0.5f);
			SetPositionShadow(pBuddy[BUDDYTYPE_DEPPA].nIdxShadow, pBuddy[BUDDYTYPE_DEPPA].pos);
		}
	}

	int nMoveTime = 60;
	if (g_Title.nCntMoveLog <= nMoveTime && g_Title.nType == 3)
	{//�܂��K��l�ɒB���Ă��Ȃ��Ƃ�

		//�����Ă���
		g_Title.Logcol[TITLEVTX_BLACK].a -= 1.0f / (float)nMoveTime;
		g_Title.Logcol[TITLEVTX_LOG].a -= 1.0f / (float)nMoveTime;
		g_Title.Logcol[TITLELOG_LINE].a -= 1.0f / (float)nMoveTime;
		g_Title.Logcol[TITLEVTX_LIBELLION].a -= 1.0f / (float)nMoveTime;
		g_Title.Logcol[TITLEVTX_HONKI].a -= 1.0f / (float)nMoveTime;

		g_Title.posLog[TITLELOG_TITLELOGO].y += (360.0f + 130.0f) / (float)nMoveTime;

		if (g_Title.Logcol[TITLEVTX_BLACK].a <= 0.0f)
		{
			g_Title.Logcol[TITLEVTX_BLACK].a = 0.0f;
			g_Title.Logcol[TITLEVTX_LOG].a = 0.0f;
			g_Title.Logcol[TITLELOG_LINE].a = 0.0f;
			g_Title.Logcol[TITLEVTX_LIBELLION].a = 0.0f;
			g_Title.Logcol[TITLEVTX_HONKI].a = 0.0f;

			g_Title.nType = 4;
			g_Title.nCntMoveLog = 0;
			g_Title.nState = TITLESTATE_NONE;
		}
	}



	for (int nCntLog = 0; nCntLog < TITLEVTX_MAX; nCntLog++)
	{

		//�w�i
		if (nCntLog != TITLEVTX_TITLELOGO)
		{
			pVtx[0].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x - 640.0f, g_Title.posLog[nCntLog].y - 360.0f, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x + 640.0f, g_Title.posLog[nCntLog].y - 360.0f, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x - 640.0f, g_Title.posLog[nCntLog].y + 360.0f, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x + 640.0f, g_Title.posLog[nCntLog].y + 360.0f, 0.0f);
		}
		else
		{
			pVtx[0].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x - LOG_WIDTH, g_Title.posLog[nCntLog].y - LOG_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x + LOG_WIDTH, g_Title.posLog[nCntLog].y - LOG_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x - LOG_WIDTH, g_Title.posLog[nCntLog].y + LOG_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_Title.posLog[nCntLog].x + LOG_WIDTH, g_Title.posLog[nCntLog].y + LOG_HEIGHT, 0.0f);
		}

		//���_�J���[�̐ݒ�
		pVtx[0].col = g_Title.Logcol[nCntLog];
		pVtx[1].col = g_Title.Logcol[nCntLog];
		pVtx[2].col = g_Title.Logcol[nCntLog];
		pVtx[3].col = g_Title.Logcol[nCntLog];

		pVtx += 4;
	}


	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTitle->Unlock();
}

//==================================================================================
//�I�����ʂ̍X�V����
//==================================================================================
void UpdateSelectTitle(void)
{

	if (GetKeyboardTrigger(DIK_D) == true || GetGamepadTrigger(BUTTON_RIGHT, 0) ||
		GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0)
	{//�E�n�������ꂽ

	 //���X�e�B�b�N�̔����n��
		SetEnableStickSelect(true);

		//�p�^�[��No.���X�V
		g_Title.nSelect = (g_Title.nSelect + 1) % TITLESELECT_MAX;
		g_Title.col.a = MAX_ALPHA;

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange = 0;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
	}
	else if (GetKeyboardTrigger(DIK_A) == true || GetGamepadTrigger(BUTTON_LEFT, 0) ||
		GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0)
	{//���n�������ꂽ

	 //���X�e�B�b�N�̔����n��
		SetEnableStickSelect(true);

		//�p�^�[��No.���X�V
		g_Title.nSelect = (g_Title.nSelect + (TITLESELECT_MAX - 1)) % TITLESELECT_MAX;
		g_Title.col.a = MAX_ALPHA;

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange = 0;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
	}


	if (GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) < 0)
	{//���ɓ|���ꂽ, �X�e�B�b�N�̔��肪OFF�̏ꍇ

	 //���X�e�B�b�N�̔����n��
		SetEnableStickSelect(true);

		//�p�^�[��No.���X�V
		g_Title.nSelect = (g_Title.nSelect + (TITLESELECT_MAX - 1)) % TITLESELECT_MAX;
		g_Title.col.a = MAX_ALPHA;

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange = 0;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
	}
	else if (GetStickSelect() == false && XGetStickPressL(BUTTON_LX, 0) > 0)
	{//�E�ɓ|���ꂽ, �X�e�B�b�N�̔��肪OFF�̏ꍇ

	 //���X�e�B�b�N�̔����n��
		SetEnableStickSelect(true);

		//�p�^�[��No.���X�V
		g_Title.nSelect = (g_Title.nSelect + 1) % TITLESELECT_MAX;
		g_Title.col.a = MAX_ALPHA;

		//�����J�ڃJ�E���^�[���Z
		g_Title.nCntChange = 0;

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_DHUHUHUHU);
	}


	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0))
	{//Enter�������ꂽ

		if (g_Title.nSelect == TITLESELECT_START)
		{//�X�^�[�g�̎������`�J�`�J

			//�`�J�`�J��ԂɈڍs
			g_Title.nState = TITLESTATE_TKTK;
		}
		else
		{//���͂����J��

			//�t�F�[�h��ԂɈڍs
			g_Title.nState = TITLESTATE_FADE;
		}

		//�T�E���h�Đ�
		PlaySound(SOUND_LABEL_SE_SYUTUGEKI);
	}

	//�I�����̐F�X�V
	UpdateSelectTitleCol(g_Title.nSelect);

}

//==================================================================================
// �I�����̐F�X�V����
//==================================================================================
void UpdateSelectTitleCol(int Select)
{
	//VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//					//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	////�I�����܂Ń|�C���^�ړ�
	//pVtx += 4 * TITLEVTX_MAX;

	//static float fMoveAlpha = 0.008f;

	//if (g_Title.col.a > 1.0f)
	//{//�s�����x100%

	//	g_Title.col.a = 1.0f;
	//	fMoveAlpha *= -1;
	//}
	//else if (g_Title.col.a < 0.4f)
	//{//�s�����x20%

	//	g_Title.col.a = 0.4f;
	//	fMoveAlpha *= -1;
	//}

	////�s�����x�̍X�V
	//g_Title.col.a += fMoveAlpha;

	////�I��������n�߂�
	//for (int nCount = TITLEVTX_MAX; nCount < TITLEVTX_MAX; nCount++)
	//{
	//	if (nCount != Select + TITLEVTX_MAX)
	//	{//���̑I��������Ȃ��Ƃ�

	//	 //�I����
	//		pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X - POS_X, g_Title.pos.y - POS_Y, 0.0f);
	//		pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X + POS_X, g_Title.pos.y - POS_Y, 0.0f);
	//		pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X - POS_X, g_Title.pos.y + POS_Y, 0.0f);
	//		pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X + POS_X, g_Title.pos.y + POS_Y, 0.0f);

	//		//���_�J���[�̐ݒ�
	//		pVtx[0].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//		pVtx[1].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//		pVtx[2].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//		pVtx[3].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	//	}
	//	else
	//	{//���݂̑I����

	//		float LengthX = POS_X;
	//		float LengthY = POS_Y;

	//		//�I����
	//		pVtx[0].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X - LengthX, g_Title.pos.y - LengthY, 0.0f);
	//		pVtx[1].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X + LengthX, g_Title.pos.y - LengthY, 0.0f);
	//		pVtx[2].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X - LengthX, g_Title.pos.y + LengthY, 0.0f);
	//		pVtx[3].pos = D3DXVECTOR3(g_Title.pos.x + (nCount - TITLEVTX_MAX) * DISTANCE_X + LengthX, g_Title.pos.y + LengthY, 0.0f);

	//		//���_�J���[�̐ݒ�
	//		pVtx[0].col = g_Title.col;
	//		pVtx[1].col = g_Title.col;
	//		pVtx[2].col = g_Title.col;
	//		pVtx[3].col = g_Title.col;
	//	}

	//	pVtx += 4;
	//}

	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffTitle->Unlock();
}

//==================================================================================
// �I�����̃`�J�`�J����
//==================================================================================
void UpdateTitleTKTK(void)
{

	//VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//					//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	//g_pVtxBuffTitle->Lock(0, 0, (void**)&pVtx, 0);

	//�`�J�`�J�t���[�����Z
	/*g_Title.nTKTKFrame++;

	if (g_Title.nTKTKFrame >= 120)
	{
		g_Title.nState = TITLESTATE_FADE;
	}*/

	//if ((g_Title.nTKTKFrame % 2) == 0)
	//{//2�t���[����1��

	//	g_Title.nTKTKFrame = 0;
	//	g_Title.nTKTKCounter++;	//�`�J�`�J�񐔂����Z

	//	if (g_Title.col.a >= MAX_ALPHA)
	//	{//�s�����x100%

	//		g_Title.col.a = MIN_ALPHA;
	//	}
	//	else
	//	{//�s�����x�ŏ��l

	//		g_Title.col.a = MAX_ALPHA;
	//	}

	//}

	//if (g_Title.nTKTKCounter >= MAX_TKTK)
	//{//�`�J�`�J���I�������

	//	g_Title.nState = TITLESTATE_FADE;

	//	//�ő�l�ɐݒ�
	//	g_Title.col.a = MAX_ALPHA;
	//}

	////���[�h���`�J�`�J
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 0].col = g_Title.col;
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 1].col = g_Title.col;
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 2].col = g_Title.col;
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 3].col = g_Title.col;

	////�I����
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 0].pos = D3DXVECTOR3(g_Title.pos.x + g_Title.nSelect * DISTANCE_X - POS_X, g_Title.pos.y - POS_Y, 0.0f);
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 1].pos = D3DXVECTOR3(g_Title.pos.x + g_Title.nSelect * DISTANCE_X + POS_X, g_Title.pos.y - POS_Y, 0.0f);
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 2].pos = D3DXVECTOR3(g_Title.pos.x + g_Title.nSelect * DISTANCE_X - POS_X, g_Title.pos.y + POS_Y, 0.0f);
	//pVtx[(g_Title.nSelect + TITLEVTX_MAX) * 4 + 3].pos = D3DXVECTOR3(g_Title.pos.x + g_Title.nSelect * DISTANCE_X + POS_X, g_Title.pos.y + POS_Y, 0.0f);

	////���_�o�b�t�@���A�����b�N����
	//g_pVtxBuffTitle->Unlock();
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