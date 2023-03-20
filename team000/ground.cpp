//==============================================================
//
//�\���̔w�i[ground.cpp]
//Author:����������
//
//==============================================================
#include "ground.h"
#include "numberPeople.h"

//�}�N����`
#define MAX_GROUND			(2)			//���
#define MAX_NUMBER			(4)			//����
#define NUMBER_WIDTH		(45.0f)		//�\���w�i�̕�
#define NUMBER_HEIGHTU		(68.0f)		//�l���̏㍂��
#define NUMBER_HEIGHTD		(63.0f)		//�l���̉�����
#define NEET_WIDTHL			(310.0f)	//�j�[�g�̐l���̍��̈ʒu
#define NEET_WIDTHR			(240.0f)	//�j�[�g�̐l���̉E�̈ʒu
#define ENEMY_WIDTHL		(105.0f)	//�G�̐l���̍��̈ʒu
#define ENEMY_WIDTHR		(175.0f)	//�G�̐l���̉E�̈ʒu

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureGround[MAX_GROUND] = {};				//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGround = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Ground g_aGround[MAX_GROUND];							//�\���w�i�̏��

//==============================================================
//�\���w�i�̏���������
//==============================================================
void InitGround(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ground002.png",
		&g_pTextureGround[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\ground001.png",
		&g_pTextureGround[1]);

	VERTEX_2D *pVtx;								//���_���ւ̃|�C���^

	for (int nCntGround = 0; nCntGround < MAX_GROUND; nCntGround++)
	{
		g_aGround[nCntGround].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu������������
		g_aGround[nCntGround].fWidth = NUMBER_WIDTH;				//��������������
		g_aGround[nCntGround].fHeight = NUMBER_HEIGHTD;				//����������������
		g_aGround[nCntGround].bUse = true;							//�g�p���Ă��Ȃ���Ԃɂ���
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_NUMBER * MAX_GROUND,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGround,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffGround->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPeople = 0; nCntPeople < MAX_GROUND; nCntPeople++)
	{
		for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			if (nCntPeople == 0)
			{//�j�[�g

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 - NEET_WIDTHL), g_aGround[nCntPeople].fHeight - NUMBER_HEIGHTU, 0.0f);
				pVtx[1].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 - NEET_WIDTHR), g_aGround[nCntPeople].fHeight - NUMBER_HEIGHTU, 0.0f);
				pVtx[2].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 - NEET_WIDTHL), g_aGround[nCntPeople].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 - NEET_WIDTHR), g_aGround[nCntPeople].fHeight, 0.0f);
			}
			else if (nCntPeople == 1)
			{//�G

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 + ENEMY_WIDTHL), g_aGround[nCntPeople].fHeight - NUMBER_HEIGHTU, 0.0f);
				pVtx[1].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 + ENEMY_WIDTHR), g_aGround[nCntPeople].fHeight - NUMBER_HEIGHTU, 0.0f);
				pVtx[2].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 + ENEMY_WIDTHL), g_aGround[nCntPeople].fHeight, 0.0f);
				pVtx[3].pos = D3DXVECTOR3((nCntNumber * g_aGround[nCntPeople].fWidth) + (SCREEN_WIDTH / 2 + ENEMY_WIDTHR), g_aGround[nCntPeople].fHeight, 0.0f);
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

			pVtx += 4;											//���_�f�[�^�̃|�C���^���S���W�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffGround->Unlock();
}

//==============================================================
//�\���w�i�̏I������
//==============================================================
void UninitGround(void)
{
	for (int nCntGround = 0; nCntGround < MAX_GROUND; nCntGround++)
	{
		if (g_pTextureGround[nCntGround] != NULL)
		{
			g_pTextureGround[nCntGround]->Release();
			g_pTextureGround[nCntGround] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffGround != NULL)
	{
		g_pVtxBuffGround->Release();
		g_pVtxBuffGround = NULL;
	}
}

//==============================================================
//�\���w�i�̍X�V����
//==============================================================
void UpdateGround(void)
{
	
}

//==============================================================
//�\���w�i�̕`�揈��
//==============================================================
void DrawGround(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	int nCntGround;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffGround, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumber = 0; nCntNumber < MAX_GROUND; nCntNumber++)
	{
		if (g_aGround[nCntNumber].bUse == true)
		{
			for (nCntGround = 0; nCntGround < MAX_NUMBER; nCntGround++)
			{
				if (nCntNumber == PEOPLESTATE_ENEMY)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pTextureGround[PEOPLESTATE_ENEMY]);
				}
				else if (nCntNumber == PEOPLESTATE_NEET)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pTextureGround[PEOPLESTATE_NEET]);
				}

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
					(4 * nCntGround) + (4 * nCntNumber * MAX_NUMBER),			//�v���~�e�B�u�i�|���S���̐��j
					2);												//�`�悷��v���~�e�B�u��
			}
		}
	}
}