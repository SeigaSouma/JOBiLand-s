//<====================================================
//
//�^�C���T�C�N������(TimeCycle.cpp)
//Author takumi hinosawa
//
//<====================================================
#include "TimeCycle.h"
#include "Input.h"
#include "game.h"
#include "nofade.h"

//�}�N����`
#define SIZE (175.0f)		//�摜�T�C�Y

//�O���[�o���ϐ��錾
LPDIRECT3DTEXTURE9	g_pTextureTimeCycle = NULL;			//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9	g_pVtxBuffTimeCycle = NULL;		//���_�̃o�b�t�@
TIMECYCLE g_TimeCycle;

//=====================================
//�^�C���T�C�N���̏���������
//=====================================					   
void InitTimeCycle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();;		//�f�o�C�X�̎擾

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\Time_C001.png",
		&g_pTextureTimeCycle);

	//�e��ϐ�������
	g_TimeCycle.pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SIZE * 0.5f, 0.0f);		//�ʒu�̏�����
	g_TimeCycle.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					//����������������(Z�l���g�p����)
													
	g_TimeCycle.fAngle = atan2f(SIZE, SIZE);						//�Ίp���̊p�x���Z�o����
	g_TimeCycle.fLength = sqrtf(SIZE * SIZE + SIZE * SIZE)* 0.5f;	//�Ίp���̒������Z�o����

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimeCycle,
		NULL);

	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTimeCycle->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + (-D3DX_PI + g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[0].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + (-D3DX_PI + g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + (D3DX_PI - g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[1].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + (D3DX_PI - g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z - g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[2].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z - g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[3].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[3].pos.z = 0.0f;

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

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimeCycle->Unlock();
}

//<=====================================
//�^�C���T�C�N���̏I������
//<=====================================
void UninitTimeCycle(void)
{
	//�e�N�X�`���̔j��
	if (g_pTextureTimeCycle != NULL)
	{
		g_pTextureTimeCycle->Release();
		g_pTextureTimeCycle = NULL;
	}
	//���_�o�b�t�@�̏���
	if (g_pVtxBuffTimeCycle != NULL)
	{
		g_pVtxBuffTimeCycle->Release();
		g_pVtxBuffTimeCycle = NULL;
	}
}

//<=====================================
//�^�C���T�C�N���̍X�V����
//<=====================================
void UpdateTimeCycle(void)
{
	VERTEX_2D *pVtx;

	//���_�o�b�t�@�����b�N�����_���ւ̃|�C���^���擾
	g_pVtxBuffTimeCycle->Lock(0, 0, (void**)&pVtx, 0);

	if (GetMode() == MODE_GAME && GetGameState() == GAMESTATE_NONE)
	{
		//�^�C���T�C�N������
		g_TimeCycle.rot.z -= D3DX_PI / (float)GAMETIME;

		if (g_TimeCycle.rot.z <= -D3DX_PI)
		{//���]�������]���I��鏈��

			g_TimeCycle.rot.z = -D3DX_PI;
			//SetGameState(GAMESTATE_END, 60);
			//SetGameState(GAMESTATE_GAMEOVER, 0);
			SetNoFade(80);
			SetGameState(GAMESTATE_AFTEREND, 0);

			
		}
	}

	//���_���W�̐ݒ�
	pVtx[0].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + (-D3DX_PI + g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[0].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + (-D3DX_PI + g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + (D3DX_PI - g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[1].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + (D3DX_PI - g_TimeCycle.fAngle)) * g_TimeCycle.fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z - g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[2].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z - g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = g_TimeCycle.pos.x + sinf(g_TimeCycle.rot.z + g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[3].pos.y = g_TimeCycle.pos.y + cosf(g_TimeCycle.rot.z + g_TimeCycle.fAngle) * g_TimeCycle.fLength;
	pVtx[3].pos.z = 0.0f;

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTimeCycle->Unlock();
}

//<=====================================
//�^�C���T�C�N���̕`�揈��
//<=====================================
void DrawTimeCycle(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//�f�o�C�X�̃|�C���^�擾

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Z�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);	//��ɕ`�悷��

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTimeCycle, 0, sizeof(VERTEX_2D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�v���C���[�̕`�揈��
	pDevice->SetTexture(0, g_pTextureTimeCycle);

	//�|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);	//�v���~�e�B�u�̎��

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}
