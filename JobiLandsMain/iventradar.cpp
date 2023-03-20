//==============================================================
//
//�C�x���g�T�C���̉��n[IventRadar.cpp]
//Author:�匴�叫
//
//==============================================================
#include "iventradar.h"
#include "mob.h"
#include "comment.h"

//�}�N����`
#define NUM_RADAR			(2)			//�l���̎��
#define NUMBER_WIDTH		(250.0f)	//��
#define NUMBER_HEIGHT		(250.0f)	//����

////�O���[�o���ϐ��錾
//const char *c_apFilenameIventRadar[] =	//�t�@�C���ǂݍ���
//{
//	"data\\TEXTURE\\radar1.png",	//��ʔ����Ȃ�
//	//"data\\TEXTURE\\radar2.png",	//��ʔ�����
//};

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureIventRadar[NUM_RADAR] = {};	//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffIventRadar = NULL;		//���_�o�b�t�@�ւ̃|�C���^
IventRadar g_aIventRadar[NUM_RADAR];						//�l���̏��

//==============================================================
//�l���̏���������
//==============================================================
void InitIventRadar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;								//���_���ւ̃|�C���^
	
	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\radar2.png",
		&g_pTextureIventRadar[0]);

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\radar1.png",
		&g_pTextureIventRadar[1]);

	//������
	for (int nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		g_aIventRadar[nCntIventRadar].pos = D3DXVECTOR3(1380.0f, 590.0f, 0.0f);	//�ʒu������������
		g_aIventRadar[nCntIventRadar].col = { 1.0f, 1.0f, 1.0f, 1.0f };     //�F�̏�����
		g_aIventRadar[nCntIventRadar].fWidth = NUMBER_WIDTH;				//��������������
		g_aIventRadar[nCntIventRadar].fHeight = NUMBER_HEIGHT;				//����������������
		g_aIventRadar[nCntIventRadar].bUse = true;							//�g�p���Ă��Ȃ���Ԃɂ���

		if (nCntIventRadar == 0)
		{//��ʔ�����

			g_aIventRadar[nCntIventRadar].type = RADARTYPE_MANYPEOPLE;
		}
		
		if (nCntIventRadar == 1)
		{//��ʔ����Ȃ�

			g_aIventRadar[nCntIventRadar].type = RADARTYPE_FEWPEOPLE;
		}
	}

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_RADAR,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffIventRadar,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffIventRadar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		if (nCntIventRadar == 0)
		{//��ʔ�����

		    //���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
		}
		
		if (nCntIventRadar == 1)
		{//��ʔ����Ȃ�

		    //���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
		}

		//rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			//���_�f�[�^�̃|�C���^���S���W�߂�
		
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIventRadar->Unlock();
}

//==============================================================
//�l���̏I������
//==============================================================
void UninitIventRadar(void)
{
	int nCntIventRadar;

	//�e�N�X�`���̔j��
	for (nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		if (g_pTextureIventRadar[nCntIventRadar] != NULL)
		{
			g_pTextureIventRadar[nCntIventRadar]->Release();
			g_pTextureIventRadar[nCntIventRadar] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffIventRadar != NULL)
	{
		g_pVtxBuffIventRadar->Release();
		g_pVtxBuffIventRadar = NULL;
	}
}

//==============================================================
//�l���̍X�V����
//==============================================================
void UpdateIventRadar(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	SwitchSpawn *pSwitchSpawn =  GetSwitchSpawn();    //��ʔ��������ǂ����̏����擾

	Commnet *pCommnet = GetCommnet();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffIventRadar->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		//g_aIventRadar[nCntIventRadar].pos = D3DXVECTOR3(1160.0f, 590.0f, 0.0f);	//�ʒu

		if (g_aIventRadar[nCntIventRadar].type == RADARTYPE_MANYPEOPLE)
		{//��ʔ����Ȃ�

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
		}

		if (g_aIventRadar[nCntIventRadar].type == RADARTYPE_FEWPEOPLE)
		{//��ʔ�����
			
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y - NUMBER_HEIGHT, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x - NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aIventRadar[nCntIventRadar].pos.x + NUMBER_WIDTH, g_aIventRadar[nCntIventRadar].pos.y + NUMBER_HEIGHT, 0.0f);
		}

		if (pSwitchSpawn->bUseSpawn == false && pCommnet->bUse == false)
		{//��ʔ������Ă��Ȃ��Ƃ�

			float fDest = 1380.0f;	//�ڕW�̊p�x
			float fDiff = fDest - g_aIventRadar[nCntIventRadar].pos.x;	//���݂ƖڕW�̍���

			D3DXVECTOR3 move;

			

			if (g_aIventRadar[RADARTYPE_FEWPEOPLE].col.a >= 1.0f)
			{//�����x��1�ȏゾ������

				g_aIventRadar[nCntIventRadar].pos.x += fDiff * 0.15f;
			}

			if (g_aIventRadar[nCntIventRadar].type == RADARTYPE_FEWPEOPLE)
			{//��ʔ������Ă��Ȃ��Ƃ��̃��[�_�[��������

				g_aIventRadar[nCntIventRadar].col.a += 0.015f;      //�����x��������

				if (g_aIventRadar[nCntIventRadar].col.a >= 1.0f)
				{//�����x��1�ȏゾ������

					g_aIventRadar[nCntIventRadar].col.a = 1.0f;    //1�ɖ߂�
				}

				//���_�J���[
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
			}
		}

		if (pCommnet->bUse == true)
		{//��ʔ������Ă���Ƃ�

			float fDest = 1160.0f;	//�ڕW�̊p�x
			float fDiff = fDest - g_aIventRadar[nCntIventRadar].pos.x;	//���݂ƖڕW�̍���
			g_aIventRadar[nCntIventRadar].pos.x += fDiff * 0.15f;

			D3DXVECTOR3 move;

			//if (fDiff >= 0)
			//{//������
			//	move.x = 5.0f;

			//	g_aIventRadar[nCntIventRadar].pos.x -= move.x;
			//}
			
			if (g_aIventRadar[nCntIventRadar].type == RADARTYPE_FEWPEOPLE)
			{//��ʔ������Ă��Ȃ��Ƃ��̃��[�_�[��������

				if (fDiff <= 0)
				{
					g_aIventRadar[nCntIventRadar].col.a -= 0.015f;     //�����x���グ��

					if (g_aIventRadar[nCntIventRadar].col.a <= 0.0f)
					{//�����x��0�ȉ���������

						g_aIventRadar[nCntIventRadar].col.a = 0.0f;    //0�ɖ߂�
					}

					//���_�J���[
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aIventRadar[nCntIventRadar].col.a);
				}
			}
			
		}

		pVtx += 4;     //���_�f�[�^�̃|�C���^���S���W�߂�
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffIventRadar->Unlock();
}

//==============================================================
//�l���̕`�揈��
//==============================================================
void DrawIventRadar(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	////Z�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffIventRadar, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntIventRadar = 0; nCntIventRadar < NUM_RADAR; nCntIventRadar++)
	{
		if (g_aIventRadar[nCntIventRadar].bUse == true)
		{//�g�p����Ă�����

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_pTextureIventRadar[nCntIventRadar]);

			//�|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
				4 * nCntIventRadar,			                    //�v���~�e�B�u�i�|���S���̐��j
				2);												//�`�悷��v���~�e�B�u��
		}
	}

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==============================================================
//���[�_�[�̏��
//==============================================================
IventRadar *GetIventRadar(void)
{
	return &g_aIventRadar[0];
}