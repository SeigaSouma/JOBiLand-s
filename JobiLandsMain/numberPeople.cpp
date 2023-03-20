//==============================================================
//
//�\������l��[numberPeople.cpp]
//Author:����������
//
//==============================================================
#include "numberPeople.h"
#include "powergage.h"

//�}�N����`
#define NUM_PEOPLE	(2)			//�l���̎��
#define MAX_NUMBER	(4)			//����
#define DIS_X		(45.0f)		//�l���̊Ԋu
#define WIDTH		(35.0f)		//�����̕�
#define HEIGHT		(30.0f)		//�����̍���
#define MOVE_V		(0.1f)		//V�̕�����

//�v���g�^�C�v�錾
void ScrollNumberPeople(int nCntPeople);

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTexturePeople[NUM_PEOPLE] = {};	//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPeople = NULL;		//���_�o�b�t�@�ւ̃|�C���^
People g_aPeople[NUM_PEOPLE];							//�l���̏��

//==============================================================
//�l���̏���������
//==============================================================
void InitPeople(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_045.png",
		&g_pTexturePeople[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\number_055.png",
		&g_pTexturePeople[1]);

	//������
	for (int nCntPeople = 0; nCntPeople < PEOPLESTATE_MAX; nCntPeople++)
	{
		g_aPeople[nCntPeople].nPeople = 0;							//�l��
		g_aPeople[nCntPeople].bUse = true;							//�g�p���Ă��Ȃ���Ԃɂ���

		for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			g_aPeople[PEOPLESTATE_NEET].pos[nCntNumber] = D3DXVECTOR3(365.0f + (DIS_X * nCntNumber), 27.0f, 0.0f);	//�ʒu������������
			g_aPeople[PEOPLESTATE_ENEMY].pos[nCntNumber] = D3DXVECTOR3(780.0f + (DIS_X * nCntNumber), 27.0f, 0.0f);	//�ʒu������������
			g_aPeople[nCntPeople].aDestTexV[nCntNumber] = 0.0f;		//�ڕW��V���W
			g_aPeople[nCntPeople].aTexV[nCntNumber] = 0.0f;			//���݂�V���W
		}
	}


	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * PEOPLESTATE_MAX * MAX_NUMBER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPeople,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPeople->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntPeople = 0; nCntPeople < PEOPLESTATE_MAX; nCntPeople++)
	{
		//�����̃e�N�X�`���ݒ�
		g_aPeople[nCntPeople].aTexV[0] = (g_aPeople[nCntPeople].nPeople % 10000 / 1000) * 0.1f;
		g_aPeople[nCntPeople].aTexV[1] = (g_aPeople[nCntPeople].nPeople % 1000 / 100) * 0.1f;
		g_aPeople[nCntPeople].aTexV[2] = (g_aPeople[nCntPeople].nPeople % 100 / 10) * 0.1f;
		g_aPeople[nCntPeople].aTexV[3] = (g_aPeople[nCntPeople].nPeople % 10 / 1) * 0.1f;

		for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aPeople[nCntPeople].pos[nCntNumber].x - WIDTH, g_aPeople[nCntPeople].pos[nCntNumber].y - HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aPeople[nCntPeople].pos[nCntNumber].x + WIDTH, g_aPeople[nCntPeople].pos[nCntNumber].y - HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aPeople[nCntPeople].pos[nCntNumber].x - WIDTH, g_aPeople[nCntPeople].pos[nCntNumber].y + HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aPeople[nCntPeople].pos[nCntNumber].x + WIDTH, g_aPeople[nCntPeople].pos[nCntNumber].y + HEIGHT, 0.0f);

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
			pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPeople[nCntPeople].aTexV[nCntNumber]);
			pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPeople[nCntPeople].aTexV[nCntNumber]);
			pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPeople[nCntPeople].aTexV[nCntNumber] + 0.1f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPeople[nCntPeople].aTexV[nCntNumber] + 0.1f);

			pVtx += 4;		//���_�f�[�^�̃|�C���^���S���W�߂�
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPeople->Unlock();
}

//==============================================================
//�l���̏I������
//==============================================================
void UninitPeople(void)
{
	int nCntPeople;

	//�e�N�X�`���̔j��
	for (nCntPeople = 0; nCntPeople < NUM_PEOPLE; nCntPeople++)
	{
		if (g_pTexturePeople[nCntPeople] != NULL)
		{
			g_pTexturePeople[nCntPeople]->Release();
			g_pTexturePeople[nCntPeople] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffPeople != NULL)
	{
		g_pVtxBuffPeople->Release();
		g_pVtxBuffPeople = NULL;
	}
}

//==============================================================
//�l���̍X�V����
//==============================================================
void UpdatePeople(void)
{
	PowerGage *pPowerGage = GetPowerGage();
	pPowerGage++;	//�P����΂�

	for (int nCntPeople = 0; nCntPeople < PEOPLESTATE_MAX; nCntPeople++, pPowerGage++)
	{
		//�l�����
		g_aPeople[nCntPeople].nPeople = pPowerGage->population;

		//��]����
		ScrollNumberPeople(nCntPeople);
	}
}

//==============================================================
//�l���̉�]����
//==============================================================
void ScrollNumberPeople(int nCntPeople)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffPeople->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += 4 * nCntPeople * MAX_NUMBER;

	//�ڕW�̃e�N�X�`�����W�ݒ�
	g_aPeople[nCntPeople].aDestTexV[0] = (g_aPeople[nCntPeople].nPeople % 10000 / 1000) * 0.1f;
	g_aPeople[nCntPeople].aDestTexV[1] = (g_aPeople[nCntPeople].nPeople % 1000 / 100) * 0.1f;
	g_aPeople[nCntPeople].aDestTexV[2] = (g_aPeople[nCntPeople].nPeople % 100 / 10) * 0.1f;
	g_aPeople[nCntPeople].aDestTexV[3] = (g_aPeople[nCntPeople].nPeople % 10 / 1) * 0.1f;

	//�����̕␳
	//for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	//{
	//	if (g_aPeople[nCntPeople].aTexV[nCntNumber] <= g_aPeople[nCntPeople].aDestTexV[nCntNumber])
	//	{//�ڕW�̒l�̕����傫����

	//		//�������␳
	//		g_aPeople[nCntPeople].aTexV[nCntNumber] += 9.0f;
	//	}
	//}

	//�␳���Ă���
	g_aPeople[nCntPeople].aTexV[0] += (g_aPeople[nCntPeople].aDestTexV[0] - g_aPeople[nCntPeople].aTexV[0]) * 0.15f;
	g_aPeople[nCntPeople].aTexV[1] += (g_aPeople[nCntPeople].aDestTexV[1] - g_aPeople[nCntPeople].aTexV[1]) * 0.15f;
	g_aPeople[nCntPeople].aTexV[2] += (g_aPeople[nCntPeople].aDestTexV[2] - g_aPeople[nCntPeople].aTexV[2]) * 0.15f;
	g_aPeople[nCntPeople].aTexV[3] += (g_aPeople[nCntPeople].aDestTexV[3] - g_aPeople[nCntPeople].aTexV[3]) * 0.15f;

	for (int nCntNumber = 0; nCntNumber < MAX_NUMBER; nCntNumber++)
	{
		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, g_aPeople[nCntPeople].aTexV[nCntNumber]);
		pVtx[1].tex = D3DXVECTOR2(1.0f, g_aPeople[nCntPeople].aTexV[nCntNumber]);
		pVtx[2].tex = D3DXVECTOR2(0.0f, g_aPeople[nCntPeople].aTexV[nCntNumber] + 0.1f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, g_aPeople[nCntPeople].aTexV[nCntNumber] + 0.1f);

		pVtx += 4;				//���_�f�[�^�̃|�C���^���S���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffPeople->Unlock();
}


//==============================================================
//�l���̕`�揈��
//==============================================================
void DrawPeople(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	int nCntPeople;

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffPeople, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntNumber = 0; nCntNumber < PEOPLESTATE_MAX; nCntNumber++)
	{
		if (g_aPeople[nCntNumber].bUse == true)
		{
			for (nCntPeople = 0; nCntPeople < MAX_NUMBER; nCntPeople++)
			{
				if (nCntNumber == PEOPLESTATE_ENEMY)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pTexturePeople[PEOPLESTATE_ENEMY]);
				}
				else if (nCntNumber == PEOPLESTATE_NEET)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_pTexturePeople[PEOPLESTATE_NEET]);
				}
				

				//�|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
					(4 * nCntPeople) + (4 * nCntNumber * MAX_NUMBER),			//�v���~�e�B�u�i�|���S���̐��j
					2);												//�`�悷��v���~�e�B�u��
			}
		}
	}
}