//==============================================================
//
//�C�x���g�T�C���̉��n[minimapframe.cpp]
//Author:�匴�叫
//
//==============================================================
#include "minimapframe.h"

//�}�N����`
#define NUM_RADAR			(2)			//�~�j�}�b�v�t���[���̎��
#define NUMBER_WIDTH		(183.0f)	//��
#define NUMBER_HEIGHT		(185.0f)	//����

////�O���[�o���ϐ��錾g_aMinimapframe
//const char *c_apFilenameminimapframe[] =	//�t�@�C���ǂݍ���
//{
//	"data\\TEXTURE\\radar1.png",	//��ʔ����Ȃ�
//	//"data\\TEXTURE\\radar2.png",	//��ʔ�����
//};

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureminimapframe = NULL;	//�e�N�X�`��1�ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffminimapframe = NULL;		//���_�o�b�t�@�ւ̃|�C���^
Minimapframe g_aMinimapframe;

//==============================================================
//�~�j�}�b�v�t���[���̏���������
//==============================================================
void InitMinimapframe(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�ւ̃|�C���^
	VERTEX_2D *pVtx;								//���_���ւ̃|�C���^

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\minimapframe1.png",
		&g_pTextureminimapframe);

	//������
	
	g_aMinimapframe.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);   //�ʒu������������
	g_aMinimapframe.fWidth = NUMBER_WIDTH;				   //��������������
	g_aMinimapframe.fHeight = NUMBER_HEIGHT;			   //����������������
	
	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffminimapframe,
		NULL);

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffminimapframe->Lock(0, 0, (void**)&pVtx, 0);

	
	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aMinimapframe.pos.x - NUMBER_WIDTH, g_aMinimapframe.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aMinimapframe.pos.x + NUMBER_WIDTH, g_aMinimapframe.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aMinimapframe.pos.x - NUMBER_WIDTH, g_aMinimapframe.pos.y + NUMBER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aMinimapframe.pos.x + NUMBER_WIDTH, g_aMinimapframe.pos.y + NUMBER_HEIGHT, 0.0f);

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
	g_pVtxBuffminimapframe->Unlock();
}

//==============================================================
//�~�j�}�b�v�t���[���̏I������
//==============================================================
void UninitMinimapframe(void)
{
	if (g_pTextureminimapframe != NULL)
	{
		g_pTextureminimapframe->Release();
		g_pTextureminimapframe = NULL;
	}
	

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffminimapframe != NULL)
	{
		g_pVtxBuffminimapframe->Release();
		g_pVtxBuffminimapframe = NULL;
	}
}

//==============================================================
//�~�j�}�b�v�t���[���̍X�V����
//==============================================================
void UpdateMinimapframe(void)
{
	VERTEX_2D *pVtx;				//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffminimapframe->Lock(0, 0, (void**)&pVtx, 0);

	g_aMinimapframe.pos = D3DXVECTOR3(119.0f, 720.0f - 145.0f, 0.0f);	//�ʒu

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(g_aMinimapframe.pos.x - NUMBER_WIDTH, g_aMinimapframe.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(g_aMinimapframe.pos.x + NUMBER_WIDTH, g_aMinimapframe.pos.y - NUMBER_HEIGHT, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(g_aMinimapframe.pos.x - NUMBER_WIDTH, g_aMinimapframe.pos.y + NUMBER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(g_aMinimapframe.pos.x + NUMBER_WIDTH, g_aMinimapframe.pos.y + NUMBER_HEIGHT, 0.0f);
	
	pVtx += 4;     //���_�f�[�^�̃|�C���^���S���W�߂�
	
	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffminimapframe->Unlock();
}

//==============================================================
//�~�j�}�b�v�t���[���̕`�揈��
//==============================================================
void DrawMinimapframe(void)
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
	pDevice->SetStreamSource(0, g_pVtxBuffminimapframe, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);
	
	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureminimapframe);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,			                                    //�v���~�e�B�u�i�|���S���̐��j
		2);												//�`�悷��v���~�e�B�u��
	
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
Minimapframe *GetMinimapframe(void)
{
	return &g_aMinimapframe;
}