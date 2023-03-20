//=============================================================================
//
// �������J�o�[���� [meshcylinder.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "cutepillow.h"
#include "input.h"
#include "debugproc.h"
#include "calculation.h"
#include "mob.h"
#include "rotate.h"
#include "hypnosis_gauge.h"
#include "crowd.h"
#include "shadow.h"
#include "minimap_icon.h"
#include "particle.h"
#include "sound.h"

//�}�N����`
#define POS_Cutepillow	(50.0f * 0.7f)
#define POS_Cutepillow_Y	(30.0f * 0.7f)
#define MOVE		(8.0f)
#define WIDTH		(8)
#define HEIGHT		(8)

//�v���g�^�C�v�錾
void UpdatePillowThrow(int nCntPillow, D3DXVECTOR3 *pos, float fRot);
void CollisionMobPillow(int nCntPillow);
void HomingLenPillow(int nCntPillow);

//�O���[�o���ϐ��錾
const char *c_apFilenameCutePillow[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\p_kurokami.png",
	"data\\TEXTURE\\p_bobchan.png",
	"data\\TEXTURE\\p_tuin.png",
	"data\\TEXTURE\\p_seifuku.png",
	"data\\TEXTURE\\p_one.png",
	"data\\TEXTURE\\p_kode.png",
	"data\\TEXTURE\\p_goldtuin.png",
};
LPDIRECT3DTEXTURE9 g_apTextureCutePillow[(sizeof c_apFilenameCutePillow) / sizeof(*c_apFilenameCutePillow)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCutePillow = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffCutePillow = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
Cutepillow g_aCutePillow[MAX_CUTEPILLOW];			//�������J�o�[�̏��
int g_nNumIndexCutePillow;			//�C���f�b�N�X��
int g_nNumVertexCutePillow;			//���_��
float g_fHigh = 9.5f;
float g_fMove = 4.4f;
float g_fHeight = 0.07f;
float g_fRot = 0.3f;

//==================================================================================
//�������J�o�[�̏���������
//==================================================================================
void InitCutepillow(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCutePillow) / sizeof(*c_apFilenameCutePillow); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCutePillow[nCntTex],
			&g_apTextureCutePillow[nCntTex]);
	}

	//�e�v�f������
	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		g_aCutePillow[nCntPillow].pos = D3DXVECTOR3(0.0f, -(POS_Cutepillow_Y * HEIGHT), 0.0f);		//�ʒu
		g_aCutePillow[nCntPillow].posOld = D3DXVECTOR3(0.0f, -(POS_Cutepillow_Y * HEIGHT), 0.0f);		//�ʒu
		g_aCutePillow[nCntPillow].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		g_aCutePillow[nCntPillow].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

		//���[���h�}�g���b�N�X
		D3DXMatrixIdentity(&g_aCutePillow[nCntPillow].mtxWorld);
		g_aCutePillow[nCntPillow].nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aCutePillow[nCntPillow].nNumIndex = (HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1));			//�C���f�b�N�X��
		g_aCutePillow[nCntPillow].nNumVertex = (HEIGHT + 1) * (WIDTH + 1);			//���_��
		g_aCutePillow[nCntPillow].nState = 0;				//���
		g_aCutePillow[nCntPillow].nType = 0;				//���
		g_aCutePillow[nCntPillow].nParent = 0;			//�e
		g_aCutePillow[nCntPillow].bUse = false;				//�g�p���Ă��邩
		g_aCutePillow[nCntPillow].fWidth = POS_Cutepillow;			//��
		g_aCutePillow[nCntPillow].fHeight = POS_Cutepillow_Y;			//�c
	}
	g_fHigh = 9.5f;
	g_fMove = 4.4f;
	g_fHeight = 0.07f;
	g_fRot = 0.3f;

	g_nNumIndexCutePillow = ((HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1))) * MAX_CUTEPILLOW;		//�C���f�b�N�X��
	g_nNumVertexCutePillow = ((HEIGHT + 1) * (WIDTH + 1)) * MAX_CUTEPILLOW;		//���_��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexCutePillow,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffCutePillow,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCutePillow->Lock(0, 0, (void**)&pVtx, 0);

	D3DXVECTOR3 pos[MAX_CUTEPILLOW];		//�v�Z�p�̍��W
	float fRot = (D3DX_PI * 2) / (float)(WIDTH);	//1���Ƃ̊p�x�����߂�
	D3DXVECTOR3 NormalizeNor[MAX_CUTEPILLOW];

	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			float fWidth = POS_Cutepillow;
			if (nCntHeight == 0)
			{
				fWidth *= 0.5f;
			}

			pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * fWidth,
				(POS_Cutepillow_Y * HEIGHT) - (POS_Cutepillow_Y * HEIGHT) - ((POS_Cutepillow_Y * nCntHeight)),
				cosf(nCntWidth % WIDTH * fRot) * fWidth
			);

			pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * fWidth,
				(POS_Cutepillow_Y * HEIGHT) - (POS_Cutepillow_Y * HEIGHT) - ((POS_Cutepillow_Y * (nCntHeight + 1))),
				cosf(nCntWidth % WIDTH * fRot) * fWidth
			);

			//�e���_���猴�_������
			NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (nCntHeight * (WIDTH + 1))] - g_aCutePillow[0].pos;
			NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] - g_aCutePillow[0].pos;

			//�o�������̒l�𐳋K������
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))], &NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))]);
			D3DXVec3Normalize(&NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))], &NormalizeNor[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))]);
		}
	}

	//���_���̐ݒ�
	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//���_���W�̐ݒ�
				pVtx[0].pos = pos[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = NormalizeNor[nCntWidth + (nCntHeight * (WIDTH + 1))];

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				(
					nCntWidth * (1.0f / (float)(WIDTH)),
					nCntHeight * (1.0f / (float)(HEIGHT))
				);

				pVtx += 1;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCutePillow->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexCutePillow,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffCutePillow,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffCutePillow->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;
	int nVtxPoint = 0;

	//���_�ԍ��f�[�^�̐ݒ�
	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//�����̕��������J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				pIdx[nCntIdx + 1] = (nCntWidth + (WIDTH + 1) * (nCntHeight + 1)) + nVtxPoint;
				pIdx[nCntIdx + 0] = (nCntWidth + ((WIDTH + 1) * nCntHeight)) + nVtxPoint;

				nCntIdx += 2;	//2�����Ă邩��
			}

			if (nCntHeight + 1 < HEIGHT)
			{//�Ō�̂����͑ł��Ȃ�

				pIdx[nCntIdx + 1] = (((WIDTH + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
				pIdx[nCntIdx + 0] = ((WIDTH + 1) * (nCntHeight + 2)) + nVtxPoint;

				nCntIdx += 2;	//2�����Ă邩��
			}
		}

		nVtxPoint += g_aCutePillow[nCntPillow].nNumVertex;	//����̃C���f�b�N�X�������Z
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffCutePillow->Unlock();

}

//==================================================================================
//�������J�o�[�̏I������
//==================================================================================
void UninitCutepillow(void)
{
	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCutePillow) / sizeof(*c_apFilenameCutePillow); nCntTex++)
	{
		if (g_apTextureCutePillow[nCntTex] != NULL)
		{
			g_apTextureCutePillow[nCntTex]->Release();
			g_apTextureCutePillow[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffCutePillow != NULL)
	{
		g_pVtxBuffCutePillow->Release();
		g_pVtxBuffCutePillow = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffCutePillow != NULL)
	{
		g_pIdxBuffCutePillow->Release();
		g_pIdxBuffCutePillow = NULL;
	}

}

//==================================================================================
//�������J�o�[�̍X�V����
//==================================================================================
void UpdateCutepillow(void)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffCutePillow->Lock(0, 0, (void**)&pVtx, 0);

	//�e�̏��擾
	SHADOW *pShadow = GetShadow();

	D3DXVECTOR3 pos[MAX_CUTEPILLOW];		//�v�Z�p�̍��W
	float fRot = (D3DX_PI * 2) / (float)(WIDTH);	//1���Ƃ̊p�x�����߂�
	D3DXVECTOR3 NormalizeNor[MAX_CUTEPILLOW];

	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		if (g_aCutePillow[nCntPillow].bUse == true)
		{
			//�O��̈ʒu�X�V
			g_aCutePillow[nCntPillow].posOld = g_aCutePillow[nCntPillow].pos;

			//�����̍X�V����
			UpdatePillowThrow(nCntPillow, &pos[0], fRot);

			//�e�̈ʒu�̍X�V
			g_aCutePillow[nCntPillow].pos += g_aCutePillow[nCntPillow].move;

			//�Q�O�Ƃ̓����蔻��
			CollisionMobPillow(nCntPillow);

			//�n�ʂɕt���������
			if (g_aCutePillow[nCntPillow].pos.y <= 0.0f && g_aCutePillow[nCntPillow].nState == CUTEPILLOW_STATE_FALL)
			{
				g_aCutePillow[nCntPillow].bUse = false;

				//�e������
				pShadow[g_aCutePillow[nCntPillow].nIdxShadow].bUse = false;
			}

			for (int nCntHeight = 0, nCntVtx = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
			{//�c�̒��_�����J��Ԃ�

				for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
				{//���̒��_�����J��Ԃ�

					//���_���W�̐ݒ�
					pVtx[nCntVtx].pos = pos[nCntVtx];
					nCntVtx++;
				}
			}

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aCutePillow[nCntPillow].nIdxShadow, g_aCutePillow[nCntPillow].pos);
		}

		//���_�������Z
		pVtx += g_aCutePillow[nCntPillow].nNumVertex;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffCutePillow->Unlock();
}

//==================================================================================
//�������J�o�[�̓���������
//==================================================================================
void UpdatePillowThrow(int nCntPillow, D3DXVECTOR3 *pos, float fRot)
{

	//�L�΂�����
	if (g_aCutePillow[nCntPillow].move.y >= 0.0f)
	{//�㏸��

		g_aCutePillow[nCntPillow].move.y -= 0.5f;
		g_aCutePillow[nCntPillow].fHeight = POS_Cutepillow_Y * g_fHeight;
		g_aCutePillow[nCntPillow].rot.y += g_fRot;
		HomingLenPillow(nCntPillow);
	}
	else
	{//���~��

		g_aCutePillow[nCntPillow].nState = CUTEPILLOW_STATE_FALL;
		g_aCutePillow[nCntPillow].move.y -= 0.005f;
		g_aCutePillow[nCntPillow].move.x *= 0.96f;
		g_aCutePillow[nCntPillow].move.z *= 0.96f;
		g_aCutePillow[nCntPillow].fHeight += (POS_Cutepillow_Y - g_aCutePillow[nCntPillow].fHeight) * 0.025f;
		g_aCutePillow[nCntPillow].rot.y += g_fRot * 0.1f;
	}

	//�p�x�̐��K��
	RotNormalize(&g_aCutePillow[nCntPillow].rot.y);

	for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
	{//�c�̒��_�����J��Ԃ�

		for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
		{//���̒��_�����J��Ԃ�

			float fWidth = g_aCutePillow[nCntPillow].fWidth;
			float fHeight = g_aCutePillow[nCntPillow].fHeight;

			if (nCntHeight == 0)
			{
				fWidth *= 0.5f;
			}

			pos[nCntWidth + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * fWidth,
				(POS_Cutepillow_Y * HEIGHT) - (fHeight * HEIGHT) - ((fHeight * nCntHeight)),
				cosf(nCntWidth % WIDTH * fRot) * fWidth
			);

			pos[nCntWidth + (WIDTH + 1) + (nCntHeight * (WIDTH + 1))] = D3DXVECTOR3
			(
				sinf(nCntWidth % WIDTH * fRot) * fWidth,
				(POS_Cutepillow_Y * HEIGHT) - (fHeight * HEIGHT) - ((fHeight * (nCntHeight + 1))),
				cosf(nCntWidth % WIDTH * fRot) * fWidth
			);
		}
	}

}

//==================================================================================
//�������J�o�[�̓���������
//==================================================================================
void HomingLenPillow(int nCntPillow)
{

	//���u�̏��擾
	Mob *pMob = GetMob();

	float MaxLength = 9999999.9f;	//�����̍ő�l
	int nIdxCrawl = -1;	//���݂̏���ʒu�ԍ�

	int nn = 0;
	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++, nn++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS)
		{//���u���g�p����Ă�����

			//�������J�o�[�ƃ��u�̋���
			float fNowLength =
				(pMob->aModel.pos.x - g_aCutePillow[nCntPillow].pos.x) * (pMob->aModel.pos.x - g_aCutePillow[nCntPillow].pos.x)
				+ (pMob->aModel.pos.z - g_aCutePillow[nCntPillow].pos.z) * (pMob->aModel.pos.z - g_aCutePillow[nCntPillow].pos.z);

			fNowLength *= 0.5f;

			//�ő�l������̒������傫��������
			if (MaxLength >= fNowLength)
			{
				MaxLength = fNowLength;	//�ő�l����ւ�
				nIdxCrawl = nCntMob;	//�ԍ�����ւ�
			}
		}
	}

	pMob -= MAX_MOB;

	int nAA = pMob[nIdxCrawl].aModel.nIdxRotate;

	if (CircleRange(pMob[nIdxCrawl].aModel.pos, g_aCutePillow[nCntPillow].pos) <= (60.0f * 60.0f))
	{//�K��l�ɒ����Ă�����

		g_aCutePillow[nCntPillow].move.x *= 0.9f;
		g_aCutePillow[nCntPillow].move.z *= 0.9f;
	}
}

//==================================================================================
//�������J�o�[�ƃ��u�̓����蔻��
//==================================================================================
void CollisionMobPillow(int nCntPillow)
{
	//���u�̏��擾
	Mob *pMob = GetMob();
	Rotate *pRotate = GetRotate();
	HypnosisGauge *pHypnosis = GetHypnosisGauge();
	SHADOW *pShadow = GetShadow();
	SwitchSE *pSwitchSE = GetSwitchSE();

	//�A�C�R���̏��擾
	MINIICON *pMiniIcon = GetMiniIcon();

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS)
		{//�Q�O���g�p����Ă����� && �Ö����

			//�������J�o�[�ƃ��u�̋���
			float fLength = pMob->aModel.vtxMax.x + g_aCutePillow[nCntPillow].fWidth;

			if (SphereRange(g_aCutePillow[nCntPillow].pos, pMob->aModel.pos) <= (fLength * fLength) &&
				pMob->aModel.pos.y + pMob->aModel.vtxMax.y >= g_aCutePillow[nCntPillow].pos.y)
			{//�~�̒��ɓ������� && �������

				//�Q�O�̐ݒ�
				SetCrowd(pMob->aModel.pos, pMob->aModel.nType, pMob->nTexType, g_aCutePillow[nCntPillow].nParent);
				pMob->aModel.bUse = false;
				StopSound(SOUND_LABEL_SE_PIYOPIYO);
				pSwitchSE->bUseRotate = false;


				//���邮��폜
				pRotate[pMob->aModel.nIdxRotate].bUse = false;
				pRotate[pMob->aModel.nIdxRotate].bDisp = false;
				pMob->aModel.nIdxRotate = -1;

				//�Ö��x�Q�[�W�폜
				pHypnosis[pMob->aModel.nIdxHypGauge].bUse = false;
				pMob->aModel.nIdxHypGauge = -1;

				//�e�폜
				pShadow[pMob->aModel.nIdxShadow].bUse = false;

				//�~�j�}�b�v�A�C�R���̍폜
				pMiniIcon[pMob->nIdxIcon].bUse = false;
			}
		}
	}
}

//==================================================================================
//�������J�o�[�̕`�揈��
//==================================================================================
void DrawCutepillow(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾

	// ���C�e�B���O�𖳌��ɂ���
	//pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntPillow = 0, nIdxPoint = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		if (g_aCutePillow[nCntPillow].bUse == true)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aCutePillow[nCntPillow].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCutePillow[nCntPillow].rot.y, g_aCutePillow[nCntPillow].rot.x, g_aCutePillow[nCntPillow].rot.z);
			D3DXMatrixMultiply(&g_aCutePillow[nCntPillow].mtxWorld, &g_aCutePillow[nCntPillow].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aCutePillow[nCntPillow].pos.x, g_aCutePillow[nCntPillow].pos.y, g_aCutePillow[nCntPillow].pos.z);
			D3DXMatrixMultiply(&g_aCutePillow[nCntPillow].mtxWorld, &g_aCutePillow[nCntPillow].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aCutePillow[nCntPillow].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffCutePillow, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffCutePillow);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureCutePillow[g_aCutePillow[nCntPillow].nType]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				g_aCutePillow[nCntPillow].nNumVertex,
				nIdxPoint,
				g_aCutePillow[nCntPillow].nNumIndex - 2);
		}

		//�C���f�b�N�X�����Z
		nIdxPoint += g_aCutePillow[nCntPillow].nNumIndex;
	}
	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

}

//==================================================================================
//�������J�o�[�̐ݒ菈��
//==================================================================================
void SetCutepillow(D3DXVECTOR3 pos, D3DXVECTOR3 rot ,int nParent)
{
	
	for (int nCntPillow = 0; nCntPillow < MAX_CUTEPILLOW; nCntPillow++)
	{
		if (g_aCutePillow[nCntPillow].bUse == false)
		{//�g�p����Ă��Ȃ�������


			g_aCutePillow[nCntPillow].bUse = true;
			g_aCutePillow[nCntPillow].pos = pos;
			g_aCutePillow[nCntPillow].pos.y = pos.y - POS_Cutepillow_Y;
			g_aCutePillow[nCntPillow].posOld = pos;
			g_aCutePillow[nCntPillow].move = D3DXVECTOR3(sinf(D3DX_PI + rot.y) * g_fMove, g_fHigh, cosf(D3DX_PI + rot.y) * g_fMove);
			g_aCutePillow[nCntPillow].nState = CUTEPILLOW_STATE_TROW;
			g_aCutePillow[nCntPillow].nType = rand() % ((sizeof c_apFilenameCutePillow) / sizeof(*c_apFilenameCutePillow));
			g_aCutePillow[nCntPillow].nParent = nParent;

			//�e��ݒ�
			g_aCutePillow[nCntPillow].nIdxShadow = SetShadow(POS_Cutepillow, POS_Cutepillow);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aCutePillow[nCntPillow].nIdxShadow, g_aCutePillow[nCntPillow].pos);

			//���˃p�[�e�B�N���̐ݒ�
			SetParticle(D3DXVECTOR3(g_aCutePillow[nCntPillow].pos.x, g_aCutePillow[nCntPillow].pos.y + POS_Cutepillow * (HEIGHT - 5), g_aCutePillow[nCntPillow].pos.z), PARTICLE_TYPE_PILLOWTHROW);
			break;
		}
	}
}

//==================================================================================
//�������J�o�[�̏��擾
//==================================================================================
Cutepillow *GetCutepillow(void)
{
	return &g_aCutePillow[0];
}

