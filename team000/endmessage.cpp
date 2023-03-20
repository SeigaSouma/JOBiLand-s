//==============================================================
//
//�I���O�ɏo�����b�Z�[�W[endmessage.cpp]
//Author:�匴�叫
//
//==============================================================
#include "endmessage.h"
#include "input.h"
#include "sound.h"

//�}�N����`
#define NUMBER_WIDTH		(300.0f * 1.5f)	//��
#define NUMBER_HEIGHT		(185.0f * 1.5f)	//����
#define COUNTZOOM           (15)

////�O���[�o���ϐ��錾g_aEndmessage
//const char *c_apFilenameEndmessage[] =	//�t�@�C���ǂݍ���
//{
//	"data\\TEXTURE\\radar1.png",	//��ʔ����Ȃ�
//	//"data\\TEXTURE\\radar2.png",	//��ʔ�����
//};

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureEndmessage = NULL;	//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEndmessage = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Endmessage g_aEndmessage;

//==============================================================
//�I��鏭���O�ɏo�����b�Z�[�W�̏���������
//==============================================================
void InitEndmessage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\endmessage.png",
		&g_pTextureEndmessage);

	
	//������
	g_aEndmessage.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu������������
	g_aEndmessage.col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);   //�F������������
	g_aEndmessage.nCounter = 0;                          //�J�E���^�[��������
	g_aEndmessage.nCntZoom = 0;                          //�J�E���^�[��������
	g_aEndmessage.fWidth = 0.0f;				 //��������������
	g_aEndmessage.fHeight = 0.0f;			     //����������������
	g_aEndmessage.bUse = false;                          //�g�p���Ă��Ȃ���Ԃɂ���

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEndmessage,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEndmessage->Lock(0, 0, (void**)&pVtx, 0);


	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aEndmessage.pos.x - NUMBER_WIDTH, g_aEndmessage.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aEndmessage.pos.x + NUMBER_WIDTH, g_aEndmessage.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aEndmessage.pos.x - NUMBER_WIDTH, g_aEndmessage.pos.y + NUMBER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aEndmessage.pos.x + NUMBER_WIDTH, g_aEndmessage.pos.y + NUMBER_HEIGHT, 0.0f);

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEndmessage->Unlock();
}

//==============================================================
//�I��鏭���O�ɏo�����b�Z�[�W�̏I������
//==============================================================
void UninitEndmessage(void)
{
	if (g_pTextureEndmessage != NULL)
	{
		g_pTextureEndmessage->Release();
		g_pTextureEndmessage = NULL;
	}


	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffEndmessage != NULL)
	{
		g_pVtxBuffEndmessage->Release();
		g_pVtxBuffEndmessage = NULL;
	}
}

//==============================================================
//�I��鏭���O�ɏo�����b�Z�[�W�̍X�V����
//==============================================================
void UpdateEndmessage(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffEndmessage->Lock(0, 0, (void**)&pVtx, 0);

	if (g_aEndmessage.bUse == true)
	{//�g�p���Ă���Ƃ�

		g_aEndmessage.nCounter--;    //�J�E���^�[�����Z

		if (g_aEndmessage.nCounter <= 0)
		{//�J�E���^�[��0�ȉ��̂Ƃ�

			g_aEndmessage.col.a -= 0.08f;

			if (g_aEndmessage.col.a <= 0.0f)
			{
				g_aEndmessage.bUse = false;    //�g�p���Ă��Ȃ���Ԃɂ���

				g_aEndmessage.col.a = 0.0f;    //�����x��0�ɂ���

				g_aEndmessage.nCntZoom = 0;

				g_aEndmessage.fHeight = 0.0f;
				g_aEndmessage.fWidth = 0.0f;
			}
		}

		g_aEndmessage.col.a += 0.005f;
		g_aEndmessage.fHeight += NUMBER_HEIGHT * 0.005f;
		g_aEndmessage.fWidth += NUMBER_WIDTH * 0.005f;

		if (g_aEndmessage.col.a >= 0.6f)
		{//�����x��0.5�ȏ�̂Ƃ�

			g_aEndmessage.col.a = 0.6f;      //�����x��0.5�ɂ���

			
				g_aEndmessage.nCntZoom++;
			
			
			//if (g_aEndmessage.nCntZoom <= COUNTZOOM)
			{
				g_aEndmessage.fHeight += NUMBER_HEIGHT * 0.3f;
				g_aEndmessage.fWidth += NUMBER_WIDTH * 0.3f;
			}
		}

		/*if (g_aEndmessage.nCntZoom >= COUNTZOOM && g_aEndmessage.fWidth >= -20.0f && g_aEndmessage.fHeight >= -20.0f)
		{
				g_aEndmessage.fHeight -= NUMBER_HEIGHT * 0.01f;
				g_aEndmessage.fWidth -= NUMBER_WIDTH * 0.01f;
		}*/



		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEndmessage.pos.x - NUMBER_WIDTH - g_aEndmessage.fWidth, g_aEndmessage.pos.y - NUMBER_HEIGHT - g_aEndmessage.fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEndmessage.pos.x + NUMBER_WIDTH + g_aEndmessage.fWidth, g_aEndmessage.pos.y - NUMBER_HEIGHT - g_aEndmessage.fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEndmessage.pos.x - NUMBER_WIDTH - g_aEndmessage.fWidth, g_aEndmessage.pos.y + NUMBER_HEIGHT + g_aEndmessage.fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEndmessage.pos.x + NUMBER_WIDTH + g_aEndmessage.fWidth, g_aEndmessage.pos.y + NUMBER_HEIGHT + g_aEndmessage.fHeight, 0.0f);

		//���_�J���[
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndmessage.col.a);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndmessage.col.a);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndmessage.col.a);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, g_aEndmessage.col.a);

		pVtx += 4;     //���_�f�[�^�̃|�C���^���S���W�߂�
	}
	
	

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEndmessage->Unlock();
}

//==============================================================
//�I��鏭���O�ɏo�����b�Z�[�W�̕`�揈��
//==============================================================
void DrawEndmessage(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^

													//�A���t�@�e�X�g��L���ɂ���
	/*pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);*/

	////Z�e�X�g�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	//pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEndmessage, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_aEndmessage.bUse == true)
	{
		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureEndmessage);

		//�|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
			0,			                                    //�v���~�e�B�u�i�|���S���̐��j
			2);												//�`�悷��v���~�e�B�u��
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
//�I��鏭���O�ɏo�����b�Z�[�W�̏��
//==============================================================
void SetEndmessage(D3DXVECTOR3 pos, int nCounter)
{
	if (g_aEndmessage.bUse == false)
	{//�g�p���Ă��Ȃ�������

		//�g�p���Ă����Ԃɂ���
		g_aEndmessage.bUse = true;

		g_aEndmessage.pos = pos;
		g_aEndmessage.nCounter = nCounter;

		//������
		PlaySound(SOUND_LABEL_SE_KOKE);
	}
}

//==============================================================
//�I��鏭���O�ɏo�����b�Z�[�W�̏��
//==============================================================
Endmessage *GetEndmessage(void)
{
	return &g_aEndmessage;
}