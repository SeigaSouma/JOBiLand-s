//=============================================================================
//
// �h������ [dispatch.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "dispatch.h"
#include "camera.h"
#include "player.h"
#include "input.h"
#include "buddy.h"
#include "crowd.h"
#include "tutorial.h"
#include "serif.h"
#include "sound.h"

//�}�N����`
#define POS_DISPATCH	(1000.0f)
#define POS_DISPATCH_X	(100.0f)
#define POS_DISPATCH_Z	(100.0f)
#define MOVE			(2.8f)
#define WIDTH			(1)
#define HEIGHT			(1)

//�O���[�o���ϐ��錾
const char *c_apFilenameDisPatch[] =	//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\target.png",
};
LPDIRECT3DTEXTURE9 g_apTextureDisPatch[(sizeof c_apFilenameDisPatch) / sizeof(*c_apFilenameDisPatch)] = {};	//�e�N�X�`���̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffDisPatch = NULL;	//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DINDEXBUFFER9 g_pIdxBuffDisPatch = NULL;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
DISPATCH g_aDisPatch[MAX_DISPACTH];		//�h���̏��
int g_nNumIndexaDisPatch;			//�C���f�b�N�X��
int g_nNumVertexaDisPatch;			//���_��

//==================================================================================
//�h���̏���������
//==================================================================================
void InitDisPatch(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�N�X�`���f�[�^�̔z�񕪌J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameDisPatch) / sizeof(*c_apFilenameDisPatch); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameDisPatch[nCntTex],
			&g_apTextureDisPatch[nCntTex]);
	}

	//�e�v�f������
	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		g_aDisPatch[nCntDis].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
		g_aDisPatch[nCntDis].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���
		g_aDisPatch[nCntDis].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

		//���[���h�}�g���b�N�X
		D3DXMatrixIdentity(&g_aDisPatch[nCntDis].mtxWorld);
		g_aDisPatch[nCntDis].bUse = false;				//�g�p���Ă��邩
		g_aDisPatch[nCntDis].nType = -1;				//���
	}

	g_nNumIndexaDisPatch = ((HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1))) * MAX_DISPACTH;	//�C���f�b�N�X��
	g_nNumVertexaDisPatch = ((WIDTH + 1) * (HEIGHT + 1)) * MAX_DISPACTH;	//���_��

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * g_nNumVertexaDisPatch,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffDisPatch,
		NULL);

	VERTEX_3D *pVtx;			//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	g_pVtxBuffDisPatch->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		//���_���̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT + 1; nCntHeight++)
		{//�c�̒��_�����J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				//���_���W�̐ݒ�
				pVtx[0].pos = D3DXVECTOR3(
					(POS_DISPATCH_X * nCntWidth) - ((POS_DISPATCH_X * WIDTH) * 0.5f),
					0.0f,
					-((POS_DISPATCH_Z * nCntHeight) - ((POS_DISPATCH_Z * HEIGHT) * 0.5f)));

				//�@���x�N�g���̐ݒ�
				pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

				//���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				//�e�N�X�`�����W�̐ݒ�
				pVtx[0].tex = D3DXVECTOR2
				(
					((float)nCntWidth / (1.0f / (float)(WIDTH + 1))) * (1.0f / (float)(WIDTH + 1)),
					((float)nCntHeight / (1.0f / (float)(HEIGHT + 1))) * (1.0f / (float)(HEIGHT + 1))
				);

				pVtx += 1;
			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	g_pVtxBuffDisPatch->Unlock();

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * g_nNumIndexaDisPatch,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&g_pIdxBuffDisPatch,
		NULL);

	WORD *pIdx;	//�C���f�b�N�X���ւ̃|�C���^

	//�C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��f�[�^�ւ̃|�C���^���擾
	g_pIdxBuffDisPatch->Lock(0, 0, (void**)&pIdx, 0);

	//�C���f�b�N�X�o�b�t�@�̗v�f�ԍ�
	int nCntIdx = 0;
	int nVtxPoint = 0;

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		//���_�ԍ��f�[�^�̐ݒ�
		for (int nCntHeight = 0; nCntHeight < HEIGHT; nCntHeight++)
		{//�����̕��������J��Ԃ�

			for (int nCntWidth = 0; nCntWidth < WIDTH + 1; nCntWidth++)
			{//���̒��_�����J��Ԃ�

				pIdx[nCntIdx + 0] = (nCntWidth + (WIDTH + 1) * (nCntHeight + 1)) + nVtxPoint;
				pIdx[nCntIdx + 1] = (nCntWidth + ((WIDTH + 1) * nCntHeight)) + nVtxPoint;
				nCntIdx += 2;	//2�����Ă邩��
			}

			if (nCntHeight + 1 < HEIGHT)
			{//�Ō�̂����͑ł��Ȃ�

				//��ł�2��
				pIdx[nCntIdx + 0] = (((WIDTH + 1) * (nCntHeight + 1)) - 1) + nVtxPoint;
				pIdx[nCntIdx + 1] = ((WIDTH + 1) * (nCntHeight + 2)) + nVtxPoint;
				nCntIdx += 2;	//2�����Ă邩��
			}
		}

		nVtxPoint += ((WIDTH + 1) * (HEIGHT + 1));	//����̃C���f�b�N�X�������Z
	}

	//�C���f�b�N�X�o�b�t�@���A�����b�N����
	g_pIdxBuffDisPatch->Unlock();

}

//==================================================================================
//�h���̏I������
//==================================================================================
void UninitDisPatch(void)
{
	//�{�C�X�I��
	StopSound();

	//�e�N�X�`���̔j��
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameDisPatch) / sizeof(*c_apFilenameDisPatch); nCntTex++)
	{
		if (g_apTextureDisPatch[nCntTex] != NULL)
		{
			g_apTextureDisPatch[nCntTex]->Release();
			g_apTextureDisPatch[nCntTex] = NULL;
		}
	}

	//���_�o�b�t�@�̔j��
	if (g_pVtxBuffDisPatch != NULL)
	{
		g_pVtxBuffDisPatch->Release();
		g_pVtxBuffDisPatch = NULL;
	}

	//���_�C���f�b�N�X�̔j��
	if (g_pIdxBuffDisPatch != NULL)
	{
		g_pIdxBuffDisPatch->Release();
		g_pIdxBuffDisPatch = NULL;
	}

}

//==================================================================================
//�h���̍X�V����
//==================================================================================
void UpdateDisPatch(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		if (g_aDisPatch[nCntDis].bUse == true)
		{//�g�p���Ă�����

			if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
			{//���L�[�������ꂽ,���ړ�

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//A+W,����ړ�

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//A+S,�����ړ�

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
				}
				else
				{//A,���ړ�

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
			{//D�L�[�������ꂽ,�E�ړ�

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//D+W,�E��ړ�

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//D+S,�E���ړ�

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
				}
				else
				{//D,�E�ړ�

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//W�������ꂽ�A���ړ�

				g_aDisPatch[nCntDis].move.x += sinf(pCamera->rot.y) * MOVE;
				g_aDisPatch[nCntDis].move.z += cosf(pCamera->rot.y) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//S�������ꂽ�A��O�ړ�

				g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI + pCamera->rot.y) * MOVE;
				g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI + pCamera->rot.y) * MOVE;
			}

			//�ʒu�X�V
			g_aDisPatch[nCntDis].pos += g_aDisPatch[nCntDis].move;

			//��������
			g_aDisPatch[nCntDis].move.x += (0.0f - g_aDisPatch[nCntDis].move.x) * 0.3f;
			g_aDisPatch[nCntDis].move.z += (0.0f - g_aDisPatch[nCntDis].move.z) * 0.3f;

			if (GetGamepadRelease(BUTTON_LB + g_aDisPatch[nCntDis].nType, 0) || GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
			{//�h���̎g�p�󋵓���ւ�

				Player *pPlayer = GetPlayer();

				if (g_aDisPatch[nCntDis].nType == 0)
				{
					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_DISPATCHL);

					//�Z���t���Z�b�g
					pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

					//�{�C�X�Z�b�g
					PlaySound(SOUND_LABEL_SE_YATTYAE);
				}
				else if (g_aDisPatch[nCntDis].nType == 1)
				{
					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_DISPATCHR);

					//�Z���t���Z�b�g
					pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

					//�{�C�X�Z�b�g
					PlaySound(SOUND_LABEL_SE_YATTYAE);
				}

				

				//�h������
				SetBuddyDispatch(g_aDisPatch[nCntDis].pos, g_aDisPatch[nCntDis].nType);
				g_aDisPatch[nCntDis].bUse = false;
				g_aDisPatch[nCntDis].nType = -1;
			}

		}
	}
}

//==================================================================================
//�h���̕`�揈��
//==================================================================================
void DrawDisPatch(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	int nIdxPoint = 0;

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		if (g_aDisPatch[nCntDis].bUse == true)
		{//�g�p���Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aDisPatch[nCntDis].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aDisPatch[nCntDis].rot.y, g_aDisPatch[nCntDis].rot.x, g_aDisPatch[nCntDis].rot.z);
			D3DXMatrixMultiply(&g_aDisPatch[nCntDis].mtxWorld, &g_aDisPatch[nCntDis].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aDisPatch[nCntDis].pos.x, g_aDisPatch[nCntDis].pos.y, g_aDisPatch[nCntDis].pos.z);
			D3DXMatrixMultiply(&g_aDisPatch[nCntDis].mtxWorld, &g_aDisPatch[nCntDis].mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aDisPatch[nCntDis].mtxWorld);

			//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(0, g_pVtxBuffDisPatch, 0, sizeof(VERTEX_3D));

			//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
			pDevice->SetIndices(g_pIdxBuffDisPatch);

			//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetFVF(FVF_VERTEX_3D);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureDisPatch[0]);

			//�|���S���̕`��
			pDevice->DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				((WIDTH + 1) * (HEIGHT + 1)),
				nIdxPoint,
				((HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1))) - 2);
		}

		nIdxPoint += ((HEIGHT * ((WIDTH + 1) * 2)) + (2 * (HEIGHT - 1)));	//����̃C���f�b�N�X�������Z
	}
}

//==================================================================================
//�h���̐ݒ菈��(BUDDYTYPE_DEPPA
//==================================================================================
void SetRightDisPatch(void)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();
	pBuddy += DISPATCH_TYPE_RIGHT;
	//BUDDYTYPE_DEPPA

	//�Q�O�̏����擾
	Crowd *pCrowd = GetCrowd();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	//�h���^�[�Q�b�g�̏o��
	if (g_aDisPatch[0].nType != DISPATCH_TYPE_LEFT && pBuddy->nState != BUDDYSTATE_DISPATCH)
	{//����������g���ĂȂ��Ƃ�

	    //���[�V�������Z�b�g
		if (pPlayer->nNowMotionNum != PLAYERMOTION_MOVECURSOR)
		{
			SetMotisonPlayer(PLAYERMOTION_MOVECURSOR);
		}

		pPlayer->bMove = false;

		//�g�p�󋵓���ւ�
		g_aDisPatch[0].bUse = true;
		g_aDisPatch[0].nType = DISPATCH_TYPE_RIGHT;
		//g_aDisPatch[nType].bUse = g_aDisPatch[nType].bUse ? false : true;
		g_aDisPatch[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDisPatch[0].pos = D3DXVECTOR3(pPlayer->pos.x - sinf(pPlayer->rot.y + D3DX_PI * 0.2f) * 200.0f, 0.5f, pPlayer->pos.z - cosf(pPlayer->rot.y + D3DX_PI * 0.2f) * 200.0f);
	}

	//========================================================================
	//�A�҂̃X�e�b�v�ȏ�ő���\
	//========================================================================
	if (GetMode() == MODE_GAME || (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_RETURN))
	{
		//�Ăі߂�
		if (pBuddy->nState == BUDDYSTATE_DISPATCH)
		{//�h����Ԃ̎�

			Player *pPlayer = GetPlayer();

			//�E�A�Ҋ���
			pTutorial->aControl.bReturnR = true;

			//���[�V�������Z�b�g
			SetMotisonPlayer(PLAYERMOTION_RETURNR);
			pPlayer->bMove = false;

			//�Z���t�̐ݒ�
			pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_RETURN, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

			//�ʏ��Ԃɖ߂�
			pBuddy->nState = BUDDYSTATE_RETURN;

			//PlaySound(SOUND_LABEL_SE_ORANGEKUN);

			for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++, pCrowd++)
			{
				if (pCrowd->aModel.bUse == true && pCrowd->nParent == PARENT_CHIBI)
				{//�K�����e�̎�

					//�����ύX����
					pBuddy->aFormation[pCrowd->aFormation.nIndex].bUse = false;

					//����̈ʒu�ݒ�
					pCrowd->aFormation.nIndex = SetFormationPos(PARENT_PLAYER);

					//�e���v���C���[�ɂ���
					pCrowd->nParent = PARENT_PLAYER;

					//�A�ҏ�Ԃɂ���
					pCrowd->nState = CROWDSTATE_RETURN;
				}
			}
		}
	}
}

//==================================================================================
//�h���̐ݒ菈��(BUDDYTYPE_HYOROGARI
//==================================================================================
void SetLeftDisPatch(void)
{
	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//���_�̏��擾
	Buddy *pBuddy = GetBuddy();
	pBuddy += DISPATCH_TYPE_LEFT;

	//�Q�O�̏����擾
	Crowd *pCrowd = GetCrowd();
	CrowdInfo *pCrowdInfo = GetCrowdInfo();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	if (g_aDisPatch[0].nType != DISPATCH_TYPE_RIGHT && pBuddy->nState != BUDDYSTATE_DISPATCH)
	{//����������g���ĂȂ��Ƃ�
		
	    //���[�V�������Z�b�g
		if (pPlayer->nNowMotionNum != PLAYERMOTION_MOVECURSOR)
		{
			SetMotisonPlayer(PLAYERMOTION_MOVECURSOR);
		}
		

		pPlayer->bMove = false;

		//�g�p�󋵓���ւ�
		g_aDisPatch[0].bUse = true;
		g_aDisPatch[0].nType = DISPATCH_TYPE_LEFT;
		g_aDisPatch[0].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aDisPatch[0].pos = D3DXVECTOR3(pPlayer->pos.x - sinf(pPlayer->rot.y + D3DX_PI * -0.2f) * 200.0f, 0.5f, pPlayer->pos.z - cosf(pPlayer->rot.y + D3DX_PI * -0.2f) * 200.0f);
	}

	//========================================================================
	//�A�҂̃X�e�b�v�ȏ�ő���\
	//========================================================================
	if (GetMode() == MODE_GAME || (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_RETURN))
	{
		//�Ăі߂�
		if (pBuddy->nState == BUDDYSTATE_DISPATCH)
		{//�h����Ԃ̎�

			Player *pPlayer = GetPlayer();

			//���A�Ҋ���
			pTutorial->aControl.bReturnL = true;

			//�ʏ��Ԃɂ���
			SetMotisonPlayer(PLAYERMOTION_RETURNL);
			pPlayer->bMove = false;

			//�Z���t�̐ݒ�
			pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_RETURN, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

			//�ʏ��Ԃɖ߂�
			pBuddy->nState = BUDDYSTATE_RETURN;

			//PlaySound(SOUND_LABEL_SE_ONMINO);

			for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++, pCrowd++)
			{
				if (pCrowd->aModel.bUse == true && pCrowd->nParent == PARENT_GARI)
				{//�K�����e�̎�

					//�����ύX����
					pBuddy->aFormation[pCrowd->aFormation.nIndex].bUse = false;

					//����̈ʒu�ݒ�
					pCrowd->aFormation.nIndex = SetFormationPos(PARENT_PLAYER);

					//�e���v���C���[�ɂ���
					pCrowd->nParent = PARENT_PLAYER;

					//�A�ҏ�Ԃɂ���
					pCrowd->nState = CROWDSTATE_RETURN;
				}
			}
		}
	}
}

//==================================================================================
//�h���̃`���[�g���A���X�V����
//==================================================================================
void UpdateTutorialDisPatch(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	for (int nCntDis = 0; nCntDis < MAX_DISPACTH; nCntDis++)
	{
		if (g_aDisPatch[nCntDis].bUse == true)
		{//�g�p���Ă�����

			if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
			{//���L�[�������ꂽ,���ړ�

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//A+W,����ړ�

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//A+S,�����ړ�

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
				}
				else
				{//A,���ړ�

					g_aDisPatch[nCntDis].move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
			{//D�L�[�������ꂽ,�E�ړ�

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//D+W,�E��ړ�

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * MOVE;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//D+S,�E���ړ�

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * MOVE;
				}
				else
				{//D,�E�ړ�

					g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
					g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * MOVE;
				}
			}
			else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//W�������ꂽ�A���ړ�

				g_aDisPatch[nCntDis].move.x += sinf(pCamera->rot.y) * MOVE;
				g_aDisPatch[nCntDis].move.z += cosf(pCamera->rot.y) * MOVE;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//S�������ꂽ�A��O�ړ�

				g_aDisPatch[nCntDis].move.x += sinf(D3DX_PI + pCamera->rot.y) * MOVE;
				g_aDisPatch[nCntDis].move.z += cosf(D3DX_PI + pCamera->rot.y) * MOVE;
			}

			//�ʒu�X�V
			g_aDisPatch[nCntDis].pos += g_aDisPatch[nCntDis].move;

			//��������
			g_aDisPatch[nCntDis].move.x += (0.0f - g_aDisPatch[nCntDis].move.x) * 0.3f;
			g_aDisPatch[nCntDis].move.z += (0.0f - g_aDisPatch[nCntDis].move.z) * 0.3f;

			if (GetGamepadRelease(BUTTON_LB + g_aDisPatch[nCntDis].nType, 0) || GetKeyboardRelease(DIK_LSHIFT) == true || GetKeyboardRelease(DIK_RSHIFT) == true)
			{//�h���̎g�p�󋵓���ւ�

				if (g_aDisPatch[nCntDis].nType == 0)
				{
					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_DISPATCHL);
					
					//�Z���t���Z�b�g
					pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

					//SE���Z�b�g
					PlaySound(SOUND_LABEL_SE_YATTYAE);

					//���h������
					pTutorial->aControl.bDispatchL = true;
				}
				else if (g_aDisPatch[nCntDis].nType == 1)
				{
					//���[�V�����̐ݒ�
					SetMotisonPlayer(PLAYERMOTION_DISPATCHR);

					//�Z���t���Z�b�g
					pPlayer->nIdxSerif = SetSerif(0.0f, 0.0f, 150, SERIFSTATE_DISPATCH, { pPlayer->pos.x, pPlayer->pos.y + 150.0f, pPlayer->pos.z });

					//SE���Z�b�g
					PlaySound(SOUND_LABEL_SE_YATTYAE);

					//�E�h������
					pTutorial->aControl.bDispatchR = true;
				}

				//�h������
				SetBuddyDispatch(g_aDisPatch[nCntDis].pos, g_aDisPatch[nCntDis].nType);
				g_aDisPatch[nCntDis].bUse = false;
				g_aDisPatch[nCntDis].nType = -1;
			}
		}
	}
}

//==================================================================================
//�h���̏��擾
//==================================================================================
DISPATCH *GetDispatch(void)
{
	return &g_aDisPatch[0];
}

