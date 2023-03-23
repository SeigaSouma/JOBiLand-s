//==============================================================
//
//���U���g��ʏ���[result.cpp]
//Author:hinosawa
//
//==============================================================
#include"result.h"
#include"sound.h"
#include"input.h"
#include"resultscore.h"
#include"score.h"
#include"light.h"
#include"camera.h"
#include"shadow.h"
#include"model.h"
#include"edit.h"
#include"player.h"
#include"meshfield.h"
#include"meshcylinder.h"
#include"meshdome.h"
#include"fade.h"

//�}�N����`
#define NUM_RESULT		(3)		//�w�i�̐�
#define CHANGE_TIME		(60 * 30)
#define COL_R			(255)	//�ԐF�̐��l
#define COL_G			(255)	//�ΐF�̐��l
#define COL_B			(255)	//�F�̐��l
#define COL_A			(255)	//�s�����x�̐��l
#define RESULT_TIME		(800)	//���U���g��ʕ\������

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureResult[NUM_RESULT] = {};	//�e�N�X�`��(3����)�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;		//���_�o�b�t�@�ւ̃|�C���^
int g_nCounterResult;
int g_nCounterRS;

//==============================================================
//���U���g��ʂ̏���������
//==============================================================
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	int nCntResult;

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\p_one.png",
		&g_pTextureResult[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\score.jpg",
		&g_pTextureResult[1]);

	g_nCounterResult = 0;		//�J�E���^�[�̏�����
	g_nCounterRS = 0;

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RESULT,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D * pVtx;		//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntResult = 0; nCntResult < NUM_RESULT; nCntResult++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(1.0f, 1.0f, 0.0f);

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[0].col = D3DCOLOR_RGBA(COL_R, COL_G, COL_B, COL_A);
		pVtx[1].col = D3DCOLOR_RGBA(COL_R, COL_G, COL_B, COL_A);
		pVtx[2].col = D3DCOLOR_RGBA(COL_R, COL_G, COL_B, COL_A);
		pVtx[3].col = D3DCOLOR_RGBA(COL_R, COL_G, COL_B, COL_A);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//���_�f�[�^�̃|�C���^���S���i�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffResult->Unlock();

	//�^�C�g���̃J��������������
	InitResultCamera();

	//�e�̏���������
	InitShadow();

	//���f���̏���������
	InitModel();

	//�G�f�B�b�g�̏�����
	InitEdit();

	//�v���C���[�̏���������
	InitPlayer();

	//���b�V���t�B�[���h�̏���������
	InitMeshField();

	//���b�V���V�����_�[�̏���������
	InitMeshCylinder();

	//���b�V���h�[���̏���������
	InitMeshDome();

	//�X�R�A�̏���������
	InitResultScore();

	Score *pSc = GetScore();

	SetResultScore(pSc->nScore);

	//�T�E���h�̍Đ�
	PlaySound(SOUND_LABEL_BGM_RESULT);

}

//==============================================================
//���U���g��ʂ̏I������
//==============================================================
void UninitResult(void)
{
	//�T�E���h�̒�~
	StopSound();

	//�e�̏I������
	UninitShadow();

	//���f���̏I������
	UninitModel();

	//�G�f�B�b�g�̏I������
	UninitEdit();

	//�v���C���[�̏I������
	UninitPlayer();

	//���b�V���t�B�[���h�̏I������
	UninitMeshField();

	//���b�V���V�����_�[�̏I������
	UninitMeshCylinder();

	//���b�V���h�[���̏I������
	UninitMeshDome();

	//�X�R�A�̏I������
	UninitResultScore();

	for (int nCntGame = 0; nCntGame < NUM_RESULT; nCntGame++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureResult[nCntGame] != NULL)
		{
			g_pTextureResult[nCntGame]->Release();
			g_pTextureResult[nCntGame] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
}

//==============================================================
//���U���g��ʂ̍X�V����
//==============================================================
void UpdateResult(void)
{
	//�����J�ڂ̃J�E���g���Z
	g_nCounterResult++;
	g_nCounterRS++;

	if (GetKeyboardTrigger(DIK_RETURN) == true ||
		GetGamepadTrigger(BUTTON_A, 0) ||
		GetGamepadTrigger(BUTTON_START, 0) || CHANGE_TIME <= g_nCounterResult)
	{//Enter�������ꂽ

		//���[�h�ݒ�(�Q�[����ʂɈڍs)
		SetFade(MODE_TITLE);				//�t�F�[�h�A�E�g
	}

	if (g_nCounterRS >= 60)
	{

		//�X�R�A���U���g�̍X�V����
		UpdateResultScore();
	}
}

//==============================================================
//���U���g��ʂ̕`�揈��
//==============================================================
void DrawResult(int nType)
{
	if (nType == DRAWTYPE_MAIN)
	{
		//�e�̕`�揈��
		DrawShadow();

		//���f���̕`�揈��
		DrawModel(0);

		//�G�f�B�b�g�̕`�揈��
		DrawEdit();

		//�v���C���[�̕`�揈��
		DrawPlayer();

		//���b�V���t�B�[���h�̕`�揈��
		DrawMeshField(DRAWFIELD_TYPE_MAIN);

		//���b�V���V�����_�[�̕`�揈��
		DrawMeshCylinder();

		//���b�V���h�[���̕`�揈��
		DrawMeshDome();
	}

	if (nType == DRAWTYPE_UI)
	{
		//�X�R�A�̕`�揈��
		DrawResultScore();

		LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffResult, 0, sizeof(VERTEX_2D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureResult[0]);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			0,												//�v���~�e�B�u�i�|���S���̐��j
			2);												//�`�悷��v���~�e�B�u��
	}
}