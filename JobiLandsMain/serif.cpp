//=============================================================================
//
// �Z���t���� [serif.h]
// Author : �匴�叫
//
//=============================================================================
#include "main.h"
#include "serif.h"
#include "player.h"
#include "camera.h"
#include "mob.h"
#include "enemy.h"

//�}�N����`
#define MAX_WIDTH		(70.0f)		//����
#define MAX_HEIGHT		(60.0f)		//�c��
#define MAX_TOP			(4)
#define MAX_HYP			(10)		//�Ö��x�̍ő�l
#define MOVE_TIME		(20)
#define NUM_ENEMYSERIF	(6)			//�G�̕��ʂ̃Z���t�̐�
#define NUM_DISCSERIF	(5)			//�G�̔������̃Z���t�̐�
#define NUM_BOCCHISERIF	(5)			//�ڂ����̃Z���t�̐�
#define NUM_YAKUZASERIF	(5)			//�₭���̃Z���t�̐�
#define NUM_WORKSERIF	(4)			//��Ј��̃Z���t�̐�

//�v���g�^�C�v�錾
//void GaugeDecrement(int nCntGauge);
//void ChangeColorSerif(int nCntGauge);
void SelectBocchiSerif(int nCntSerif);
void SelectYakuzaSerif(int nCntSerif);
void SelectWorkSerif(int nCntSerif);
void SelectEnemyDiscSerif(int nCntSerif);
void SelectEnemyNoneSerif(int nCntSerif);

//�O���[�o���ϐ��錾
const char *c_apFilenameSerif[] =		//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\serif000.png",		//�h��
	"data\\TEXTURE\\serif001.png",		//�A��
	"data\\TEXTURE\\serif002.png",		//�G�i�I�j
	"data\\TEXTURE\\serif019.png",		//�G�i�j�[�g�����I�j
	"data\\TEXTURE\\serif020.png",		//�G�i�܂��܂����ˁj
	"data\\TEXTURE\\serif021.png",		//�G�i�ӑāj
	"data\\TEXTURE\\serif022.png",		//�G�i�o���Ȃ���ƍ߂���Ȃ��j
	"data\\TEXTURE\\serif003.png",		//���C�g
	"data\\TEXTURE\\serif004.png",		//�ڂ����i�����j
	"data\\TEXTURE\\serif005.png",		//�ڂ����i�����j
	"data\\TEXTURE\\serif010.png",		//�ڂ����i���A�j
	"data\\TEXTURE\\serif011.png",		//�ڂ����i�C�L���āj
	"data\\TEXTURE\\serif012.png",		//�ڂ����i�������畉���j
	"data\\TEXTURE\\serif006.png",		//���N�U�i�{�X�j
	"data\\TEXTURE\\serif013.png",		//���N�U�i���N�U�����b�p�[�j
	"data\\TEXTURE\\serif014.png",		//���N�U�i���w�Ȃ��j
	"data\\TEXTURE\\serif015.png",		//���N�U�i�����p�j
	"data\\TEXTURE\\serif016.png",		//���N�U�iheke�j
	"data\\TEXTURE\\serif008.png",		//��Ј��i��Ёj
	"data\\TEXTURE\\serif009.png",		//��Ј��i�d�Ԓx���j
	"data\\TEXTURE\\serif017.png",		//��Ј��i�A�j�������j
	"data\\TEXTURE\\serif018.png",		//��Ј��i�Q�O�A�΁c�j
	"data\\TEXTURE\\serif023.png",		//�G�i�j�[�g�͂ǂ����j
	"data\\TEXTURE\\serif024.png",		//�G�i�j�[�g�o�Łj
	"data\\TEXTURE\\serif025.png",		//�G�i�H�t�����Ȃ��Ɓj
	"data\\TEXTURE\\serif026.png",		//�G�i�j�[�g�ɐl���Ȃ��j
	"data\\TEXTURE\\serif027.png",		//�G�i�������Ǝv��Ȃ��́j
	"data\\TEXTURE\\serif028.png",		//�G�i�����͏��ł��[�I�j
};
LPDIRECT3DTEXTURE9 g_pTextureSerif[(sizeof c_apFilenameSerif) / sizeof(*c_apFilenameSerif)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffSerif = NULL;	//���_�o�b�t�@�ւ̃|�C���^
Serif g_aSerif[MAX_SERIF];				//HP�Q�[�W�̏��
bool g_bUseSerif;		//�Z���t��������������

//==================================================================================
//�Z���t�̏���������
//==================================================================================
void InitSerif(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSerif) / sizeof(*c_apFilenameSerif); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameSerif[nCntTex],
			&g_pTextureSerif[nCntTex]);
	}


	//�e�v�f������
	for (int nCntGauge = 0; nCntGauge < MAX_SERIF; nCntGauge++)
	{
		g_aSerif[nCntGauge].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aSerif[nCntGauge].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aSerif[nCntGauge].mtxWorld);
		g_aSerif[nCntGauge].fWidth = 0.0f;
		g_aSerif[nCntGauge].fHeight = 0.0f;
		g_aSerif[nCntGauge].nType = SERIFSTATE_DISPATCH;
		g_aSerif[nCntGauge].nLife = 0;
		g_aSerif[nCntGauge].nBocchiSelect = 0;			//�ڂ��������̃Z���t�̑I��
		g_aSerif[nCntGauge].nYakuzaSelect = 0;			//�₭���̃Z���t�̑I��
		g_aSerif[nCntGauge].nWorkSelect = 0;			//��Ј��̃Z���t�̑I��
		g_aSerif[nCntGauge].nEnemyDiscSelect = 0;		//�G�̔������̃Z���t�̑I��
		g_aSerif[nCntGauge].nEnemyNoneSelect = 0;		//�G�̕��ʂ̃Z���t�̑I��
		g_aSerif[nCntGauge].bUse = false;
		g_aSerif[nCntGauge].bDisp = false;

	}

	g_bUseSerif = false;		//�Z���t��������������

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * MAX_TOP * MAX_SERIF,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffSerif,
		NULL);

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSerif->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_SERIF; nCntGauge++)
	{
		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);

		//�@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);


		pVtx[0].col =
		pVtx[1].col =
		pVtx[2].col =
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.8f);


		//�e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += MAX_TOP;
		
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSerif->Unlock();
}

//==================================================================================
//�Z���t�̏I������
//==================================================================================
void UninitSerif(void)
{
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameSerif) / sizeof(*c_apFilenameSerif); nCntTex++)
	{
		//�e�N�X�`���̔j��
		if (g_pTextureSerif[nCntTex] != NULL)
		{
			g_pTextureSerif[nCntTex]->Release();
			g_pTextureSerif[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffSerif != NULL)
	{
		g_pVtxBuffSerif->Release();
		g_pVtxBuffSerif = NULL;
	}
}

//==================================================================================
//�Z���t�̍X�V����
//==================================================================================
void UpdateSerif(void)
{

	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSerif->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_SERIF; nCntGauge++)
	{
		if (g_aSerif[nCntGauge].bUse == true && g_aSerif[nCntGauge].bDisp == true)
		{//�g�p����Ă�����

			if (g_aSerif[nCntGauge].fWidth <= MAX_WIDTH && g_aSerif[nCntGauge].fHeight <= MAX_HEIGHT && g_aSerif[nCntGauge].nLife > 0)
			{//������50�ȉ����c����25�ȉ���������0���傫����

				g_aSerif[nCntGauge].fWidth += MAX_WIDTH / (float)MOVE_TIME;     //���������Z
				g_aSerif[nCntGauge].fHeight += MAX_HEIGHT / (float)MOVE_TIME;   //�c�������Z
			}
			else
			{
				int nn = 0;
			}
			
			if (g_aSerif[nCntGauge].nLife > 0)
			{//������0��葽���Ƃ�

				g_aSerif[nCntGauge].nLife--;    //���������炷
			}
			

			if (g_aSerif[nCntGauge].nLife <= 0)
			{//������0�̂Ƃ�

				g_aSerif[nCntGauge].fWidth -= (MAX_WIDTH / (float)MOVE_TIME) * 0.5f;    //���������炷
				g_aSerif[nCntGauge].fHeight -= (MAX_HEIGHT / (float)MOVE_TIME) * 0.5f;  //�c�������炷

				if (g_aSerif[nCntGauge].fWidth <= 0.0f && g_aSerif[nCntGauge].fHeight <= 0.0f)
				{//�������c����0�ȉ��ɂȂ�����

					//�G�̏��擾
					Enemy *pEnemy = GetEnemy();

					//���u�̏��擾
					Mob *pMob = GetMob();

					//�v���C���[�̏��擾
					Player *pPlayer = GetPlayer();

					g_aSerif[nCntGauge].bUse = false;     //�g�p���Ă��Ȃ���Ԃɂ���
					g_aSerif[nCntGauge].bDisp = false;    //�`�悵�Ȃ�
					g_bUseSerif = false;

					//�G�̃C���f�b�N�X������
					if (g_bUseSerif == false)
					{
						for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
						{
							if (nCntGauge == pEnemy->nIdxSerif && pEnemy->bUse == true && pEnemy->bMoreSpawn == true)
							{
								pEnemy->nIdxSerif = -1;
								g_bUseSerif = true;

								break;
							}
						}
					}
					
					//���u�̃C���f�b�N�X������
					if (g_bUseSerif == false)
					{
						for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
						{
							if (nCntGauge == pMob->aModel.nIdxSerif && pMob->aModel.bUse == true)
							{
								pMob->aModel.nIdxSerif = -1;
								g_bUseSerif = true;

								break;

							}
						}
					}

					//�v���C���[�̃C���f�b�N�X������
					if (g_bUseSerif == false)
					{
						if (nCntGauge == pPlayer->nIdxSerif && pPlayer->bUse == true)
						{
							pPlayer->nIdxSerif = -1;
							g_bUseSerif = true;
						}
					}

					g_bUseSerif = false;
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);
		}

		if ((g_aSerif[nCntGauge].bDisp == false && g_aSerif[nCntGauge].bUse == true && g_aSerif[nCntGauge].nType == SERIFSTATE_DISPATCH) ||
			(g_aSerif[nCntGauge].bDisp == false && g_aSerif[nCntGauge].bUse == true && g_aSerif[nCntGauge].nType == SERIFSTATE_RETURN))
		{//�g�p���Ă��� && �h�� ||
		 //�g�p���Ă��� && �A��

			g_aSerif[nCntGauge].bUse = false;			//�g�p���ĂȂ���Ԃɂ���

		}

		pVtx += MAX_TOP;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSerif->Unlock();
}

//==================================================================================
//�Z���t�̈ʒu�X�V
//==================================================================================
void SetPositionSerif(int nIdxGauge, D3DXVECTOR3 pos)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	if (g_aSerif[nIdxGauge].bUse == true && g_aSerif[nIdxGauge].bDisp == true)
	{//�g�p����Ă�����

		g_aSerif[nIdxGauge].pos = pos;
		g_aSerif[nIdxGauge].pos.y = pos.y + 15.0f;
	}
}

//==================================================================================
//�Z���t�̕`�揈��
//==================================================================================
void DrawSerif(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxView;		//�r���[�}�g���b�N�X�擾�p

	// ���C�e�B���O�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	for (int nCntGauge = 0; nCntGauge < MAX_SERIF; nCntGauge++)
	{
		if (g_aSerif[nCntGauge].bUse == true && g_aSerif[nCntGauge].bDisp == true)
		{//�g�p����Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aSerif[nCntGauge].mtxWorld);

			//�r���[�}�g���b�N�X���擾
			pDevice->GetTransform(D3DTS_VIEW, &mtxView);

			//�|���S�����J�����ɑ΂��Đ��ʂɌ�����
			D3DXMatrixInverse(&g_aSerif[nCntGauge].mtxWorld, NULL, &mtxView);	//�t�z������߂�

			//��ŏ�����
			g_aSerif[nCntGauge].mtxWorld._21 = 0.0f;
			g_aSerif[nCntGauge].mtxWorld._23 = 0.0f;
			g_aSerif[nCntGauge].mtxWorld._24 = 0.0f;

			g_aSerif[nCntGauge].mtxWorld._41 = 0.0f;
			g_aSerif[nCntGauge].mtxWorld._42 = 0.0f;
			g_aSerif[nCntGauge].mtxWorld._43 = 0.0f;

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aSerif[nCntGauge].pos.x, g_aSerif[nCntGauge].pos.y, g_aSerif[nCntGauge].pos.z);
			D3DXMatrixMultiply(&g_aSerif[nCntGauge].mtxWorld, &g_aSerif[nCntGauge].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aSerif[nCntGauge].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffSerif, 0, sizeof(VERTEX_3D));

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			switch (g_aSerif[nCntGauge].nType)
			{
			case SERIFSTATE_DISPATCH:		//�h��

				pDevice->SetTexture(0, g_pTextureSerif[0]);
				break;

			case SERIFSTATE_RETURN:			//�A��

				pDevice->SetTexture(0, g_pTextureSerif[1]);
				break;

			case SERIFSTATE_ENEMYNONE:		//�G�i���낿���j

				//�G�̃Z���t�̑I������
				SelectEnemyNoneSerif(nCntGauge);

				break;

			case SERIFSTATE_ENEMYDISC:		//�G�i�������j

				//�G�̃Z���t�̑I������
				SelectEnemyDiscSerif(nCntGauge);
				break;

			case SERIFSTATE_MOB_BOCCHI:			//�ڂ���

				//���u�̃Z���t�̑I������
				SelectBocchiSerif(nCntGauge);

				break;

			case SERIFSTATE_MOB_YAKUZA:			//�₭��

				//���u�̃Z���t�̑I������
				SelectYakuzaSerif(nCntGauge);

				break;

			case SERIFSTATE_MOB_WORK:			//��Ј�

				//���u�̃Z���t�̑I������
				SelectWorkSerif(nCntGauge);

				break;
			}
			
			//HP�Q�[�W�̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, (nCntGauge * 4), 2);
		}
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O��L���ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//==================================================================================
//�Z���t�̐ݒ菈��
//==================================================================================
int SetSerif(float fWidth, float fHeight, int nLife, int nType, D3DXVECTOR3 pos)
{
	VERTEX_3D *pVtx;	//���_���ւ̃|�C���^

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffSerif->Lock(0, 0, (void**)&pVtx, 0);

	int nCntGauge = -1;
	int nn = 0;

	//�P�g�p���Ă������
	if (nType == SERIFSTATE_DISPATCH || nType == SERIFSTATE_RETURN)
	{
		for (int nCount = 0; nCount < MAX_SERIF; nCount++)
		{
			if ((g_aSerif[nCount].bUse == true && g_aSerif[nCount].nType == SERIFSTATE_DISPATCH) ||
				(g_aSerif[nCount].bUse == true && g_aSerif[nCount].nType == SERIFSTATE_RETURN))
			{//�g�p���Ă��� && �h�� ||
			 //�g�p���Ă��� && �A��

				g_aSerif[nCount].bDisp = false;			//�`�悵�ĂȂ���Ԃɂ���

			}

			if (g_aSerif[nCount].bUse == false)
			{
				pPlayer->nIdxSerif = -1;		//�v���C���[�̃C���f�b�N�X������
			}
		}
	}

	for (int nCount = 0; nCount < MAX_SERIF; nCount++)
	{
		if (g_aSerif[nCount].bUse == false)
		{//�g�p���Ă��Ȃ�������

			nCntGauge = nCount;						//�C���f�b�N�X�ԍ��擾
			g_aSerif[nCntGauge].bUse = true;		//�g�p���Ă����Ԃɂ���
			g_aSerif[nCntGauge].bDisp = true;		//�`�悵�Ă����Ԃɂ���

			g_aSerif[nCntGauge].fHeight = fHeight;
			g_aSerif[nCntGauge].fWidth = fWidth;
			g_aSerif[nCntGauge].nLife = nLife;

			if (g_aSerif[nCntGauge].nLife != 150)
			{

				g_aSerif[nCntGauge].bUse = true;		//�g�p���Ă����Ԃɂ���
			}
			g_aSerif[nCntGauge].nType = nType;
			g_aSerif[nCntGauge].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(-g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aSerif[nCntGauge].fWidth, -g_aSerif[nCntGauge].fHeight, 0.0f);

			//�Z���t�̎�ތ��߂�
			if (g_aSerif[nCount].nType == SERIFSTATE_ENEMYNONE)
			{//�G�̕��ʂ̃Z���t��������

				g_aSerif[nCntGauge].nEnemyNoneSelect = rand() % NUM_ENEMYSERIF;
			}
			if (g_aSerif[nCount].nType == SERIFSTATE_ENEMYDISC)
			{//�G�̔������̃Z���t��������

				g_aSerif[nCntGauge].nEnemyDiscSelect = rand() % NUM_DISCSERIF;
			}
			else if (g_aSerif[nCount].nType == SERIFSTATE_MOB_BOCCHI)
			{//�ڂ����̃Z���t��������

				g_aSerif[nCntGauge].nBocchiSelect = rand() % NUM_BOCCHISERIF;
			}
			else if (g_aSerif[nCount].nType == SERIFSTATE_MOB_YAKUZA)
			{//�₭���̃Z���t��������

				g_aSerif[nCntGauge].nYakuzaSelect = rand() % NUM_YAKUZASERIF;
			}
			else if (g_aSerif[nCount].nType == SERIFSTATE_MOB_WORK)
			{//��Ј��̃Z���t��������

				g_aSerif[nCntGauge].nWorkSelect = rand() % NUM_WORKSERIF;
			}
			
			break;
		}

		pVtx += MAX_TOP;
	}

	if (nCntGauge == -1)
	{
		int nn = 0;
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffSerif->Unlock();

	return nCntGauge;
}

//==================================================================================
//�G�̕��ʂ̃Z���t�̑I������
//==================================================================================
void SelectEnemyNoneSerif(int nCntSerif)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���ݒ�
	switch (g_aSerif[nCntSerif].nEnemyNoneSelect)
	{
	case ENEMYSERIFSTATE_SEEK:		//�j�[�g�͂ǂ���

		pDevice->SetTexture(0, g_pTextureSerif[22]);

		break;

	case ENEMYSERIFSTATE_ERADIC:	//�j�[�g�o��

		pDevice->SetTexture(0, g_pTextureSerif[23]);

		break;

	case ENEMYSERIFSTATE_AKIBA:		//�H�t���܂���Ȃ���

		pDevice->SetTexture(0, g_pTextureSerif[24]);

		break;

	case ENEMYSERIFSTATE_ZINKEN:	//�j�[�g�ɐl���Ȃ�

		pDevice->SetTexture(0, g_pTextureSerif[25]);

		break;

	case ENEMYSERIFSTATE_WORK:		//�������Ǝv��Ȃ���

		pDevice->SetTexture(0, g_pTextureSerif[26]);

		break;

	case ENEMYSERIFSTATE_OBUTSU:	//�����͏��ł��[�I

		pDevice->SetTexture(0, g_pTextureSerif[27]);

		break;
	}
}

//==================================================================================
//�G�̔������̃Z���t�̑I������
//==================================================================================
void SelectEnemyDiscSerif(int nCntSerif)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���ݒ�
	switch (g_aSerif[nCntSerif].nEnemyDiscSelect)
	{
	case DISCSERIFSTATE_BIKKURI:		//�I

		pDevice->SetTexture(0, g_pTextureSerif[2]);

		break;

	case DISCSERIFSTATE_NEET:			//�j�[�g����

		pDevice->SetTexture(0, g_pTextureSerif[3]);

		break;

	case DISCSERIFSTATE_MADAMADA:		//�܂��܂�����

		pDevice->SetTexture(0, g_pTextureSerif[4]);

		break;

	case DISCSERIFSTATE_TAIDA:			//�ӑăf�X�˂�

		pDevice->SetTexture(0, g_pTextureSerif[5]);

		break;

	case DISCSERIFSTATE_HANZAI:		//�o���Ȃ���ƍ߂���Ȃ�

		pDevice->SetTexture(0, g_pTextureSerif[6]);

		break;
	}
}

//==================================================================================
//�ڂ����̃Z���t�̑I������
//==================================================================================
void SelectBocchiSerif(int nCntSerif)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�ڂ��������
	switch (g_aSerif[nCntSerif].nBocchiSelect)
	{

	case BOCCHISERIFSTATE_CLUB:			//�ڂ����i�����j

		pDevice->SetTexture(0, g_pTextureSerif[8]);

		break;

	case BOCCHISERIFSTATE_REAL:			//�ڂ����i�����j

		pDevice->SetTexture(0, g_pTextureSerif[9]);

		break;

	case BOCCHISERIFSTATE_EEE:			//�ڂ����i���A�j

		pDevice->SetTexture(0, g_pTextureSerif[10]);

		break;

	case BOCCHISERIFSTATE_SORRY:		//�ڂ����i�C�L���Ă��݂܂���j

		pDevice->SetTexture(0, g_pTextureSerif[11]);

		break;

	case BOCCHISERIFSTATE_WORK:			//�ڂ����i�������畉���j

		pDevice->SetTexture(0, g_pTextureSerif[12]);

		break;
	}
}

//==================================================================================
//�₭���̃Z���t�̑I������
//==================================================================================
void SelectYakuzaSerif(int nCntSerif)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�₭��
	switch (g_aSerif[nCntSerif].nYakuzaSelect)
	{
	case YAKUZASERIFSTATE_BOSS:			//�₭���i�{�X�j

		pDevice->SetTexture(0, g_pTextureSerif[13]);

		break;

	case YAKUZASERIFSTATE_RAPPER:		//�₭���i���N�U�����b�p�[�j

		pDevice->SetTexture(0, g_pTextureSerif[14]);

		break;

	case YAKUZASERIFSTATE_KOYUBI:		//�₭���i���w�Ȃ��j

		pDevice->SetTexture(0, g_pTextureSerif[15]);

		break;

	case YAKUZASERIFSTATE_TOKYO:		//�₭���i�����p�j

		pDevice->SetTexture(0, g_pTextureSerif[16]);

		break;

	case YAKUZASERIFSTATE_HEKE:			//�₭���i�w�P�j

		pDevice->SetTexture(0, g_pTextureSerif[17]);

		break;
	}
}

//==================================================================================
//��Ј��̃Z���t�̑I������
//==================================================================================
void SelectWorkSerif(int nCntSerif)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//��Ј�
	switch (g_aSerif[nCntSerif].nWorkSelect)
	{

	case WORKSERIFSTATE_BLACK:			//��Ј��i��Ђ�߂����j

		pDevice->SetTexture(0, g_pTextureSerif[18]);

		break;

	case WORKSERIFSTATE_STATION:		//��Ј��i�d�Ԓx���j

		pDevice->SetTexture(0, g_pTextureSerif[19]);

		break;

	case WORKSERIFSTATE_ANIME:			//��Ј��i�A�j�������j

		pDevice->SetTexture(0, g_pTextureSerif[20]);

		break;

	case WORKSERIFSTATE_KEEP:			//��Ј��i�Q�O�A�΁j

		pDevice->SetTexture(0, g_pTextureSerif[21]);

		break;
	}
}

//==================================================================================
//�Z���t�̏��擾
//==================================================================================
Serif *GetSerif(void)
{
	return &g_aSerif[0];

	//�匴�͐_
	//�������
	//���́H
	//���������Ă�
	///�匴�S�p�g���ȎE����by okusada
	//��
	//�o�O���Ă�
	//���܂�
}
