//=============================================================================
//
// �Q�O���� [crowd.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "crowd.h"
#include "input.h"
#include "camera.h"
#include "game.h"
#include "player.h"
#include "calculation.h"
#include "mob.h"
#include "hypnosis_gauge.h"
#include "rotate.h"
#include "shadow.h"
#include "cutepillow.h"
#include "buddy.h"
#include "particle.h"
#include "buddy.h"
#include "tutorial.h"
#include "sound.h"
#include "resultscore.h"

//�}�N����`
#define CHASE_CNT	(60)	//�Ǐ]�܂ł̃J�E���g
#define THROW_COOLTIME	(90)	//�������Ƃ��̃N�[���^�C��
#define RETURN_MOVE		(2.5f)	//�A�Ҏ��̈ړ���
#define HART_COOLTIME	(10)	//�n�[�g�̃N�[���^�C��

//�v���g�^�C�v�錾
void ControllCrowd(int nCntCrowd);
void UpdateStateCrowd(int nCntCrowd);
void LimitPosCrowd(int nCntCrowd);
void CollisionModelCrowd(int nCntCrowd);
void ThrowPillowCrowd(int nCntCrowd);

//�O���[�o���ϐ��錾
const char *c_apFilenameCrowd[] =					//�t�@�C���ǂݍ���
{
	"data\\TEXTURE\\crowd02.jpg",
	"data\\TEXTURE\\crowd05.jpg",
	"data\\TEXTURE\\crowd04.jpg",
	"data\\TEXTURE\\crowd03.jpg",
	"data\\TEXTURE\\crowd07.jpg",
	"data\\TEXTURE\\crowd08.jpg",
};
LPDIRECT3DTEXTURE9 g_apTextureCrowd[(sizeof c_apFilenameCrowd) / sizeof(*c_apFilenameCrowd)] = {};	//�e�N�X�`���̃|�C���^
Crowd g_aCrowd[MAX_CROWD];		//�Q�O�̏��
CrowdInfo g_aCrowdInfo;         //���
CrowdNum g_aCrowdNum;			//�Q�O�̐��̏��
D3DXVECTOR3 FormationPos[MAX_CROWD];
int g_nHartSoundCooltime;

//==================================================================================
//�Q�O�̏���������
//==================================================================================
void InitCrowd(void)
{

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCrowd) / sizeof(*c_apFilenameCrowd); nCntTex++)
	{
		//�e�N�X�`���̓ǂݍ���
		D3DXCreateTextureFromFile(pDevice,
			c_apFilenameCrowd[nCntTex],
			&g_apTextureCrowd[nCntTex]);
	}

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�e�v�f������
	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		//�Q�O�\���̂̏�����
		g_aCrowd[nCntCrowd].nState = CROWDSTATE_NONE;	//���
		g_aCrowd[nCntCrowd].nCntChase = 0;			//�ǂ��|����J�E���g
		g_aCrowd[nCntCrowd].fFadeAlpha = 1.0f;		//�t�F�[�h�̕s�����x
		g_aCrowd[nCntCrowd].formationPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����̈ʒu
		g_aCrowd[nCntCrowd].ParentPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�e�̈ʒu
		g_aCrowd[nCntCrowd].ParentRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�e�̌���
		g_aCrowd[nCntCrowd].fThrowRadius = 240.0f;		//������͈�
		g_aCrowd[nCntCrowd].nThrowCnt = 0;				//������J�E���g
		g_aCrowd[nCntCrowd].nType = 0;				//���
		g_aCrowd[nCntCrowd].nParent = 0;			//�e
		g_aCrowd[nCntCrowd].nTexType = 0;			//�e�N�X�`���^�C�v
		g_aCrowd[nCntCrowd].bFever = false;			//�t�B�[�o�[��Ԃ�

		//���f���\���̂̏�����
		g_aCrowd[nCntCrowd].aModel.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���݂̈ʒu
		g_aCrowd[nCntCrowd].aModel.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
		g_aCrowd[nCntCrowd].aModel.posOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̈ʒu
		g_aCrowd[nCntCrowd].aModel.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		g_aCrowd[nCntCrowd].aModel.rotOrigin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���̌���
		g_aCrowd[nCntCrowd].aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ���

		//���[���h�}�g���b�N�X
		D3DXMatrixIdentity(&g_aCrowd[nCntCrowd].aModel.mtxWorld);
		D3DXMatrixIdentity(&g_aCrowd[nCntCrowd].aModel.OldmtxWorld);

		g_aCrowd[nCntCrowd].aModel.nIdxShadow = -1;			//�e�̃C���f�b�N�X�ԍ�
		g_aCrowd[nCntCrowd].aModel.nIdxRotate = -1;			//���邮��̃C���f�b�N�X�ԍ�
		g_aCrowd[nCntCrowd].aModel.nIdxHypGauge = -1;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
		g_aCrowd[nCntCrowd].aModel.nShadow = 1;			//�e���g�����ǂ���

		g_aCrowd[nCntCrowd].aModel.nNumVtx = 0;			//���_��
		g_aCrowd[nCntCrowd].aModel.dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aCrowd[nCntCrowd].aModel.pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aCrowd[nCntCrowd].aModel.vtxMin = D3DXVECTOR3(10.0f, 10.0f, 10.0f);		//���f���̍ŏ��l
		g_aCrowd[nCntCrowd].aModel.vtxMax = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);		//���f���̍ő�l

		g_aCrowd[nCntCrowd].aModel.nType = 0;			//�ǂݍ��ރ��f���̎��
		g_aCrowd[nCntCrowd].aModel.nParent = -1;		//�e�̔ԍ�
		g_aCrowd[nCntCrowd].aModel.bUse = false;			//�g�p���Ă��邩
		g_aCrowd[nCntCrowd].aModel.nState = 0;			//���

		g_aCrowd[nCntCrowd].aModel.pMesh = NULL;			//���b�V��(���_���)�ւ̃|�C���^
		g_aCrowd[nCntCrowd].aModel.pBuffMat = NULL;		//�}�e���A���ւ̃|�C���^

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aCrowd[nCntCrowd].aModel.pMatData[nCntMat].MatD3D = {};	//�}�e���A���̃f�[�^
			g_aCrowd[nCntCrowd].aModel.pMatData[nCntMat].pTextureFilename = NULL;	//�}�e���A���̃f�[�^
			g_aCrowd[nCntCrowd].aModel.acFilename[nCntMat] = NULL;	//�t�@�C����
		}
		g_aCrowd[nCntCrowd].aModel.dwNumMat = NULL;			//�}�e���A���̐�

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aCrowd[nCntCrowd].aModel.pTexture[nCntTex] = NULL;	//�e�N�X�`���ւ̃|�C���^
		}
		
		//����̏�񏉊���
		g_aCrowd[nCntCrowd].aFormation.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����̈ʒu
		g_aCrowd[nCntCrowd].aFormation.nIndex = -1;			//����̃C���f�b�N�X�ԍ�
		g_aCrowd[nCntCrowd].aFormation.bUse = false;			//������g�p���Ă��邩
	}

	//�Q�O���̏�����
	//g_aCrowdInfo.nCrowdValue = 0;        //�G�̑���
	g_aCrowdInfo.nParentPlayerNum = 0;	//�e���v���C���[�̐�
	g_aCrowdInfo.nCrowdCnter = 0;		//�J�E���^�[������
	g_aCrowdInfo.nCrowdKeep = 0;
	g_aCrowdInfo.nCroedTutorial = 0;	//�`���[�g���A���ۑ��p

	//�Q�O�̐��̏��
	g_aCrowdNum.nNum = 0;			//�Q�O�̐�
	g_aCrowdNum.nNumOld = 0;		//�ߋ��̌Q�O�̐�

	//�n�[�g�p�[�e�B�N���̃N�[���_�E��
	g_nHartSoundCooltime = 0;

	//����̈ʒu������
	for (int nCntCrowd = 0; nCntCrowd < MAX_MOB; nCntCrowd++)
	{
		FormationPos[nCntCrowd] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

}

//==================================================================================
//�Q�O�̏I������
//==================================================================================
void UninitCrowd(void)
{
	StopSound();

	//�t�@�C�������J��Ԃ�
	for (int nCntTex = 0; nCntTex < (sizeof c_apFilenameCrowd) / sizeof(*c_apFilenameCrowd); nCntTex++)
	{
		if (g_apTextureCrowd[nCntTex] != NULL)
		{
			g_apTextureCrowd[nCntTex]->Release();
			g_apTextureCrowd[nCntTex] = NULL;
		}
	}
}

//==================================================================================
//�Q�O�̍X�V����
//==================================================================================
void UpdateCrowd(void)
{
	//���f���̏��擾
	ModelModel *pModel = GetModel();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�o�f�B�[�̏��擾
	Buddy *pBuddy = GetBuddy();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	//�ڕW�̐l���̏��擾
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

#if _DEBUG
	if (GetKeyboardPress(DIK_G) == true)
	{//Y�L�[�������ꂽ,���_��ړ�

		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33, 0, PARENT_PLAYER);
		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33, 0, PARENT_PLAYER);
		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33, 0, PARENT_PLAYER);
		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33, 0, PARENT_PLAYER);
		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33,0, PARENT_PLAYER);
	
	}

	if (GetKeyboardTrigger(DIK_F) == true)
	{//Y�L�[�������ꂽ,���_��ړ�

		SetCrowd(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 33,0, PARENT_PLAYER);
	}
#endif

	int nCntUse = 0, nCntPlayer = 0, nCntDebu = 0, nCntGari = 0;
	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aCrowd[nCntCrowd].aModel.bUse == true)
		{//�g�p���Ă�����

			//�ߋ��̈ʒu�ۑ�
			g_aCrowd[nCntCrowd].aModel.posOld = g_aCrowd[nCntCrowd].aModel.pos;

			//�ߋ��̌Q�O�̐��ۑ�
			g_aCrowdNum.nNumOld = g_aCrowdNum.nNum;

			if (g_aCrowd[nCntCrowd].nState != CROWDSTATE_FADE)
			{
				//�h����
				if (g_aCrowd[nCntCrowd].nParent == PARENT_PLAYER)
				{//�v���C���[

					//pPlayer->aFormation[g_aCrowd[nCntCrowd].aFormation.nIndex].bUse = false;

					//����̈ʒu
					g_aCrowd[nCntCrowd].formationPos = pPlayer->aFormation[nCntPlayer].pos;

					nCntPlayer++;
				}

				if (g_aCrowd[nCntCrowd].nParent == PARENT_GARI)
				{//�q�����K�����K�l�A�L��

					//����̈ʒu
					//g_aCrowd[nCntCrowd].formationPos = pBuddy[BUDDYTYPE_HYOROGARI].aFormation[g_aCrowd[nCntCrowd].aFormation.nIndex].pos;
					g_aCrowd[nCntCrowd].formationPos = pBuddy[BUDDYTYPE_HYOROGARI].aFormation[nCntGari].pos;

					nCntGari++;
				}

				if (g_aCrowd[nCntCrowd].nParent == PARENT_CHIBI)
				{//�N�\�`�r
					
					//����̈ʒu
					//g_aCrowd[nCntCrowd].formationPos = pBuddy[BUDDYTYPE_DEPPA].aFormation[g_aCrowd[nCntCrowd].aFormation.nIndex].pos;
					g_aCrowd[nCntCrowd].formationPos = pBuddy[BUDDYTYPE_DEPPA].aFormation[nCntDebu].pos;

					nCntDebu++;
				}

				//�g�p�̃J�E���g���Z
				nCntUse++;

				if (g_aCrowd[nCntCrowd].nState != CROWDSTATE_WAIT)
				{
					//�Q�O�̑���
					ControllCrowd(nCntCrowd);

					//�Q�O���m�̓����蔻��
					CollisionModelCrowd(nCntCrowd);

					//�Q�O�̃J�o�[��������
					ThrowPillowCrowd(nCntCrowd);

					//���W����
					LimitPosCrowd(nCntCrowd);

					//�Ǐ]�J�E���g�ɒB���Ă�����
					if (g_aCrowd[nCntCrowd].nCntChase >= CHASE_CNT)
					{
						g_aCrowd[nCntCrowd].nCntChase = CHASE_CNT;
						g_aCrowd[nCntCrowd].nState = CROWDSTATE_CHASE;
					}
				}
			}

			//��ԍX�V
			UpdateStateCrowd(nCntCrowd);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aCrowd[nCntCrowd].aModel.nIdxShadow, g_aCrowd[nCntCrowd].aModel.pos);
		}
	}

	//�������Z
	g_aCrowdInfo.nCrowdValue = nCntUse;

	if (GetMode() == MODE_TUTORIAL && pTutorial->nStep < TUTORIAL_STEP_HYPNOSISFEVER)
	{
		g_aCrowdInfo.nCroedTutorial = nCntUse;
	}

	//�ڕW�̐l��
	if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_HYPNOSISFEVER)
	{
		pGetnTargetNop->nCntPeople = 10 - g_aCrowdInfo.nCrowdValue + g_aCrowdInfo.nCroedTutorial;    //�ڕW�̐l�������炷
	}
	else
	{
		pGetnTargetNop->nCntPeople = TARGETHITO - g_aCrowdInfo.nCrowdValue - 3;    //�ڕW�̐l�������炷
	}

	//�ۑ�
	g_aCrowdInfo.nCrowdKeep = g_aCrowdInfo.nCrowdValue;

	//�e���v���C���[�̐����
	g_aCrowdInfo.nParentPlayerNum = nCntPlayer;

	//�N�[���^�C������
	g_nHartSoundCooltime--;

	if (g_nHartSoundCooltime <= 0)
	{//0�����������

		//�Œ�
		g_nHartSoundCooltime = 0;
	}

	if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_MORS && g_aCrowdInfo.nCrowdValue == 0)
	{//���[�h���`���[�g���A�� && MORS�̃X�e�b�v�ȏ� && ���u��0

		//�Ö��������
		pTutorial->aControl.bReleaseCrowd = true;
	}
}

//==================================================================================
//�Q�O�̑���
//==================================================================================
void ControllCrowd(int nCntCrowd)
{

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�o�f�B�[�̏��擾
	Buddy *pBuddy = GetBuddy();

	//�d�͏���
	g_aCrowd[nCntCrowd].aModel.move.y -= GRAVITY;

	float fRotDest = g_aCrowd[nCntCrowd].aModel.rot.y;	//�ڕW�̊p�x
	float fRotDiff = 0.0f;	//���݂ƖڕW�̍���

	//�ڕW�̊p�x�����߂�
	fRotDest = atan2f((g_aCrowd[nCntCrowd].aModel.pos.x - g_aCrowd[nCntCrowd].formationPos.x), (g_aCrowd[nCntCrowd].aModel.pos.z - g_aCrowd[nCntCrowd].formationPos.z));
	fRotDiff = fRotDest - g_aCrowd[nCntCrowd].aModel.rot.y;

	//�p�x�̐��K��
	RotNormalize(&fRotDiff);

	//�p�x�̕␳������
	g_aCrowd[nCntCrowd].aModel.rot.y += fRotDiff * 0.2f;

	//�p�x�̐��K��
	RotNormalize(&g_aCrowd[nCntCrowd].aModel.rot.y);

	if (CircleRange(g_aCrowd[nCntCrowd].formationPos, g_aCrowd[nCntCrowd].aModel.pos) >= (50.0f * 50.0f) && g_aCrowd[nCntCrowd].nState != CROWDSTATE_JOIN)
	{//�܂��K��l�ɒ����Ă��Ȃ������� && �����Ȃ����

		if (g_aCrowd[nCntCrowd].nState != CROWDSTATE_RETURN)
		{
			g_aCrowd[nCntCrowd].aModel.move.x += sinf(D3DX_PI + g_aCrowd[nCntCrowd].aModel.rot.y) * CHARA_MOVE;
			g_aCrowd[nCntCrowd].aModel.move.z += cosf(D3DX_PI + g_aCrowd[nCntCrowd].aModel.rot.y) * CHARA_MOVE;
		}
		else if (g_aCrowd[nCntCrowd].nState == CROWDSTATE_RETURN)
		{
			g_aCrowd[nCntCrowd].aModel.move.x += sinf(D3DX_PI + g_aCrowd[nCntCrowd].aModel.rot.y) * RETURN_MOVE;
			g_aCrowd[nCntCrowd].aModel.move.z += cosf(D3DX_PI + g_aCrowd[nCntCrowd].aModel.rot.y) * RETURN_MOVE;
		}
	}

	if (g_aCrowd[nCntCrowd].bFever == true)
	{//�t�B�[�o�[���Ȃ�

		//����
		g_aCrowd[nCntCrowd].aModel.move.x *= 1.1f;
		g_aCrowd[nCntCrowd].aModel.move.z *= 1.1f;
	}

	//�ʒu�X�V
	g_aCrowd[nCntCrowd].aModel.pos += g_aCrowd[nCntCrowd].aModel.move;
	//CollisionModelCrowd(nCntCrowd);

	//����
	g_aCrowd[nCntCrowd].aModel.move.x += (0.0f - g_aCrowd[nCntCrowd].aModel.move.x) * MOVE_FACTOR;
	g_aCrowd[nCntCrowd].aModel.move.z += (0.0f - g_aCrowd[nCntCrowd].aModel.move.z) * MOVE_FACTOR;

	if (CircleRange(g_aCrowd[nCntCrowd].formationPos, g_aCrowd[nCntCrowd].aModel.pos) <= (50.0f * 50.0f) && g_aCrowd[nCntCrowd].nState == CROWDSTATE_CHASE)
	{//�K��l�ɒ����Ă� && �ǂ��|�����

		//�Ȃɂ����ĂȂ���Ԃɂ���
		g_aCrowd[nCntCrowd].nState = CROWDSTATE_NONE;
		g_aCrowd[nCntCrowd].nCntChase = 0;
	}
}

//==================================================================================
//�Q�O�̍��W����
//==================================================================================
void LimitPosCrowd(int nCntCrowd)
{

	if (g_aCrowd[nCntCrowd].aModel.pos.y < 0.0f)
	{//�n�ʂ�艺�ɍs������

		g_aCrowd[nCntCrowd].aModel.pos.y = 0.0f;

		if (g_aCrowd[nCntCrowd].nState == CROWDSTATE_JOIN)
		{
			g_aCrowd[nCntCrowd].nState = CROWDSTATE_NONE;
		}

		//�d�͏���
		g_aCrowd[nCntCrowd].aModel.move.y = 0.0f;
	}

	//�ړ�����
	if (g_aCrowd[nCntCrowd].aModel.pos.x + g_aCrowd[nCntCrowd].aModel.vtxMin.x < -LIMIT_POS) { g_aCrowd[nCntCrowd].aModel.pos.x = -LIMIT_POS - g_aCrowd[nCntCrowd].aModel.vtxMin.x; }
	if (g_aCrowd[nCntCrowd].aModel.pos.x + g_aCrowd[nCntCrowd].aModel.vtxMax.x > LIMIT_POS) { g_aCrowd[nCntCrowd].aModel.pos.x = LIMIT_POS - g_aCrowd[nCntCrowd].aModel.vtxMax.x; }
	if (g_aCrowd[nCntCrowd].aModel.pos.z + g_aCrowd[nCntCrowd].aModel.vtxMax.z > LIMIT_POS) { g_aCrowd[nCntCrowd].aModel.pos.z = LIMIT_POS - g_aCrowd[nCntCrowd].aModel.vtxMax.z; }
	if (g_aCrowd[nCntCrowd].aModel.pos.z + g_aCrowd[nCntCrowd].aModel.vtxMin.z < -LIMIT_POS) { g_aCrowd[nCntCrowd].aModel.pos.z = -LIMIT_POS - g_aCrowd[nCntCrowd].aModel.vtxMin.z; }

}

//==================================================================================
//�Q�O�̏�ԍX�V����
//==================================================================================
void UpdateStateCrowd(int nCntCrowd)
{

	//�e�̏��擾
	SHADOW *pShadow = GetShadow();

	//�ڕW�̐l���̏��擾
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

	switch (g_aCrowd[nCntCrowd].nState)
	{
	case CROWDSTATE_NONE:
		break;

	case CROWDSTATE_JOIN:
		break;

	case CROWDSTATE_FADE:
		//CrowdInfo *pCrowdInfo = GetCrowdInfo();

		//�����Ă���
		g_aCrowd[nCntCrowd].fFadeAlpha -= 0.01f;
		g_aCrowd[nCntCrowd].nState = CROWDSTATE_FADE;

		if (g_aCrowd[nCntCrowd].fFadeAlpha <= 0.0f)
		{
			//�����Ă���
			g_aCrowd[nCntCrowd].aModel.bUse = false;

			//�e������
			pShadow[g_aCrowd[nCntCrowd].aModel.nIdxShadow].bUse = false;

			//�ڕW�̐l���𑝂₷
			g_aCrowdInfo.nCrowdValue++;
			
		}
		break;

	case CROWDSTATE_WAIT:

		//�ˌ��҂���Ԃɂ���
		g_aCrowd[nCntCrowd].nState = CROWDSTATE_WAIT;

		g_aCrowd[nCntCrowd].nCntChase = 0;

		//�ړ��ʂ�0�ɂ���
		g_aCrowd[nCntCrowd].aModel.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;

	case CROWDSTATE_RETURN:

		if (CircleRange(g_aCrowd[nCntCrowd].formationPos, g_aCrowd[nCntCrowd].aModel.pos) <= (50.0f * 50.0f))
		{//�K��l�ɒ����Ă�

			g_aCrowd[nCntCrowd].nState = CROWDSTATE_NONE;
		}
		break;
	}
	
}

//==================================================================================
//�Q�O�̕������J�o�[��������
//==================================================================================
void ThrowPillowCrowd(int nCntCrowd)
{
	//���u�̏��擾
	Mob *pMob = GetMob();

	for (int nCntMob = 0; nCntMob < MAX_MOB; nCntMob++, pMob++)
	{
		if (pMob->aModel.bUse == true && pMob->aModel.nState == MOBSTATE_HYPNOSIS)
		{//�g�p����Ă��� && �Ö����

			if (CircleRange(g_aCrowd[nCntCrowd].aModel.pos, pMob->aModel.pos) <= (g_aCrowd[nCntCrowd].fThrowRadius * g_aCrowd[nCntCrowd].fThrowRadius))
			{//�T���T�[�N���̒��ɓ�������

				if (g_aCrowd[nCntCrowd].nThrowCnt >= 1)
				{//�N�[���^�C������������

					g_aCrowd[nCntCrowd].nThrowCnt = (g_aCrowd[nCntCrowd].nThrowCnt + 1) % THROW_COOLTIME;
				}

				if (g_aCrowd[nCntCrowd].nThrowCnt <= 0)
				{
					//�������J�o�[�̌��������߂�
					float fRotDest = atan2f((g_aCrowd[nCntCrowd].aModel.pos.x - pMob->aModel.pos.x), (g_aCrowd[nCntCrowd].aModel.pos.z - pMob->aModel.pos.z));

					//�������̐ݒ菈��
					SetCutepillow(g_aCrowd[nCntCrowd].aModel.pos, D3DXVECTOR3(0.0f, fRotDest, 0.0f), g_aCrowd[nCntCrowd].nParent);

					//�N�[���^�C���ݒ�
					g_aCrowd[nCntCrowd].nThrowCnt = 1;
				}
				break;
			}
		}
	}
}

//==================================================================================
//�Q�O�̍s���X�V����
//==================================================================================
void UpdateActionCrowd(int nCntCrowd)
{
	
}

//==================================================================================
//�I�u�W�F�N�g�Ƃ̓����蔻��
//==================================================================================
void CollisionModelCrowd(int nCntCrowd)
{

	//���f���̏��擾
	ModelModel *pModel = GetModel();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	bool bHit = false;

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true && g_aCrowd[nCntCrowd].nState != CROWDSTATE_CHASE && pModel->nType < SMALL_NUM)
		{//���f�����g�p����Ă����� && �ǂ��|����Ԃ���Ȃ�������

			//�������Ă�����
			if (bHitCharacter(
				&g_aCrowd[nCntCrowd].aModel.pos, &g_aCrowd[nCntCrowd].aModel.posOld, g_aCrowd[nCntCrowd].aModel.vtxMax, g_aCrowd[nCntCrowd].aModel.vtxMin,
				&pModel->pos, pModel->vtxMax, pModel->vtxMin) == true)
			{
				bHit = true;
			}

			//�L�����N�^�[���m�̓����蔻��
			CollisionCharacter(
				&g_aCrowd[nCntCrowd].aModel.pos, &g_aCrowd[nCntCrowd].aModel.posOld, g_aCrowd[nCntCrowd].aModel.vtxMax, g_aCrowd[nCntCrowd].aModel.vtxMin,
				&pModel->pos, pModel->vtxMax, pModel->vtxMin);
		}
	}

	//�������Ă�����
	if (bHit == true)
	{
		//�Ǐ]�J�E���g�����Z
		g_aCrowd[nCntCrowd].nCntChase++;
	}
	else
	{
		//�Ǐ]�J�E���g�����Z
		g_aCrowd[nCntCrowd].nCntChase--;

		if (g_aCrowd[nCntCrowd].nCntChase <= 0)
		{
			g_aCrowd[nCntCrowd].nCntChase = 0;
		}
	}

}

//==================================================================================
//�Q�O�̕`�揈��
//==================================================================================
void DrawCrowd(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	//D3DXMATERIAL mMatRed;			//�_���[�W�}�e���A��
	D3DXMATERIAL mFade;				//�t�F�[�h�}�e���A��
	D3DXMATERIAL mDefalt;			//�f�t�H���g�}�e���A��

	//���̏��N���A
	ZeroMemory(&mDefalt, sizeof(D3DXMATERIAL));
	mDefalt.MatD3D.Diffuse = D3DXCOLOR(0.59f, 0.40f, 0.94f, 1.0f);
	mDefalt.MatD3D.Ambient = D3DXCOLOR(0.59f, 0.40f, 0.94f, 1.0f);


	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aCrowd[nCntCrowd].aModel.bUse == true)
		{//�g�p���Ă�����

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aCrowd[nCntCrowd].aModel.mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aCrowd[nCntCrowd].aModel.rot.y, g_aCrowd[nCntCrowd].aModel.rot.x, g_aCrowd[nCntCrowd].aModel.rot.z);
			D3DXMatrixMultiply(&g_aCrowd[nCntCrowd].aModel.mtxWorld, &g_aCrowd[nCntCrowd].aModel.mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aCrowd[nCntCrowd].aModel.pos.x, g_aCrowd[nCntCrowd].aModel.pos.y, g_aCrowd[nCntCrowd].aModel.pos.z);
			D3DXMatrixMultiply(&g_aCrowd[nCntCrowd].aModel.mtxWorld, &g_aCrowd[nCntCrowd].aModel.mtxWorld, &mtxTrans);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aCrowd[nCntCrowd].aModel.mtxWorld);

			//���݂̃}�e���A�����擾
			pDevice->GetMaterial(&matDef);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aCrowd[nCntCrowd].aModel.pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aCrowd[nCntCrowd].aModel.dwNumMat; nCntMat++)
			{
				if (GetMode() != MODE_RESULT)
				{

					//�}�e���A���̐ݒ�
					switch (g_aCrowd[nCntCrowd].nState)
					{
					case CROWDSTATE_FADE:
						mFade.MatD3D.Diffuse = D3DXCOLOR(pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, g_aCrowd[nCntCrowd].fFadeAlpha);
						mFade.MatD3D.Ambient = D3DXCOLOR(pMat[nCntMat].MatD3D.Ambient.r, pMat[nCntMat].MatD3D.Ambient.g, pMat[nCntMat].MatD3D.Ambient.b, g_aCrowd[nCntCrowd].fFadeAlpha);
						pDevice->SetMaterial(&mFade.MatD3D);
						break;

					default:
						pDevice->SetMaterial(&mDefalt.MatD3D);
						//pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
						break;
					}
				}
				else
				{
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
				}

				//�e�N�X�`���̐ݒ�
				if (pMat[nCntMat].pTextureFilename != NULL)
				{
					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_apTextureCrowd[g_aCrowd[nCntCrowd].nTexType]);
					//pDevice->SetTexture(0, NULL);
				}
				else
				{
					pDevice->SetTexture(0, NULL);
				}

				//�Q�O(�p�[�c)�̕`��
				g_aCrowd[nCntCrowd].aModel.pMesh->DrawSubset(nCntMat);
			}

			//�ۑ����Ă����}�e���A����߂�
			pDevice->SetMaterial(&matDef);
		}
	}
}

//==================================================================================
//�Q�O�̈ʒu�ݒ�
//==================================================================================
int SetFormationPos(int nParent)
{
	int nIndex = -1;

	//����̈ʒu�ݒ�
	switch (nParent)
	{
	case PARENT_PLAYER:
		nIndex = SetFormationPosPlayer();
		break;

	case PARENT_GARI:
		nIndex = SetFormationPosBuddy(BUDDYTYPE_HYOROGARI);
		break;

	case PARENT_CHIBI:
		nIndex = SetFormationPosBuddy(BUDDYTYPE_DEPPA);
		break;
	}

	return nIndex;
}

//==================================================================================
//�Q�O�̐ݒ菈��
//==================================================================================
void SetCrowd (D3DXVECTOR3 pos, int nType, int nTexType, int nParent)
{
	//���f���̏��擾
	Model *pModel = GetXLoadData();

	//�ڕW�̐l���̏��擾
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aCrowd[nCntCrowd].aModel.bUse == false)
		{//�g�p���Ă��Ȃ�������

			//���f�����擾
			g_aCrowd[nCntCrowd].aModel = pModel[nType];
			g_aCrowd[nCntCrowd].aModel.pos = pos;	//�ʒu
			g_aCrowd[nCntCrowd].nState = CROWDSTATE_JOIN;	//���
			g_aCrowd[nCntCrowd].aModel.move.y += 10.0f;
			g_aCrowd[nCntCrowd].aModel.bUse = true;	//�g�p���Ă���󋵂ɂ���
			g_aCrowd[nCntCrowd].fFadeAlpha = 1.0f;	//�t�F�[�h�̕s�����x
			g_aCrowd[nCntCrowd].nThrowCnt = 0;			//������J�E���g
			g_aCrowd[nCntCrowd].nParent = nParent;			//������J�E���g
			g_aCrowd[nCntCrowd].nTexType = nTexType;	//�e�N�X�`���^�C�v

			//pGetnTargetNop->nCntPeople--;    //�ڕW�̐l�������炷

			g_aCrowdNum.nNum++;		//�Q�O�̐��ǉ�

			if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_SENNOU)
			{//���[�h���`���[�g���A�� && ���]�̃X�e�b�v�ȏ�

				//���]�������
				pTutorial->aControl.bCutePillow = true;

				if (pTutorial->nStep >= TUTORIAL_STEP_HYPNOSISFEVER)
				{//�t�B�[�o�[���]�̃X�e�b�v�ȏ�

				 //�t�B�[�o�[���ɕ߂܂����l�����Z
					pTutorial->aControl.nCntHypnosisFever++;
				}
			}

			if (pPlayer->bFever == true)
			{//�t�B�[�o�[���̎�

				g_aCrowd[nCntCrowd].bFever = true;
			}

			//���f���̑S���_�`�F�b�N
			CheckVtx(g_aCrowd[nCntCrowd].aModel.rot.y, &g_aCrowd[nCntCrowd].aModel.vtxMax, &g_aCrowd[nCntCrowd].aModel.vtxMin, g_aCrowd[nCntCrowd].aModel.pMesh, g_aCrowd[nCntCrowd].aModel.pVtxBuff);

			//����̈ʒu�ݒ�
			g_aCrowd[nCntCrowd].aFormation.nIndex = SetFormationPos(nParent);

			//�e��ݒ�
			g_aCrowd[nCntCrowd].aModel.nIdxShadow = SetShadow(g_aCrowd[nCntCrowd].aModel.vtxMax.x * 1.5f, g_aCrowd[nCntCrowd].aModel.vtxMax.z * 1.5f);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aCrowd[nCntCrowd].aModel.nIdxShadow, g_aCrowd[nCntCrowd].aModel.pos);

			if (GetMode() != MODE_RESULT && g_nHartSoundCooltime <= 0)
			{//�T�E���h�̃N�[���^�C�����I����Ă�����

				//�n�[�g�̃p�[�e�B�N�����Z�b�g
				SetParticle(D3DXVECTOR3(g_aCrowd[nCntCrowd].aModel.pos.x, g_aCrowd[nCntCrowd].aModel.pos.y, g_aCrowd[nCntCrowd].aModel.pos.z), PARTICLE_TYPE_HART);

				//SE���Z�b�g
				PlaySound(SOUND_LABEL_SE_HYPNOSISEND);

				//�N�[���^�C���ݒ�
				g_nHartSoundCooltime = HART_COOLTIME;
			}
			break;
		}
	}
}

//==================================================================================
//�Q�O�̐ݒ菈��
//==================================================================================
void SetCrowdResult(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType, int nTexType, int nParent)
{
	//���f���̏��擾
	Model *pModel = GetXLoadData();

	//�ڕW�̐l���̏��擾
	TARGETNOP *pGetnTargetNop = GetnTargetNop();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�v���C���[�̏��擾
	Player *pPlayer = GetPlayer();

	//�`���[�g���A���̏��擾
	Tutorial *pTutorial = GetTutorial();

	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		if (g_aCrowd[nCntCrowd].aModel.bUse == false)
		{//�g�p���Ă��Ȃ�������

		 //���f�����擾
			g_aCrowd[nCntCrowd].aModel = pModel[nType];

			g_aCrowd[nCntCrowd].aModel.rot = rot;
			g_aCrowd[nCntCrowd].aModel.pos = pos;	//�ʒu
			g_aCrowd[nCntCrowd].nState = CROWDSTATE_JOIN;	//���

			g_aCrowd[nCntCrowd].aModel.move.y = (float)((rand() % 19 - 9)) * 0.01f;

			g_aCrowd[nCntCrowd].aModel.bUse = true;			//�g�p���Ă���󋵂ɂ���
			g_aCrowd[nCntCrowd].fFadeAlpha = 1.0f;			//�t�F�[�h�̕s�����x
			g_aCrowd[nCntCrowd].nThrowCnt = 0;				//������J�E���g
			g_aCrowd[nCntCrowd].nParent = nParent;			//������J�E���g
			g_aCrowd[nCntCrowd].nTexType = nTexType;		//�e�N�X�`���^�C�v

			//pGetnTargetNop->nCntPeople--;    //�ڕW�̐l�������炷

			g_aCrowdNum.nNum++;		//�Q�O�̐��ǉ�

			if (GetMode() == MODE_TUTORIAL && pTutorial->nStep >= TUTORIAL_STEP_SENNOU)
			{//���[�h���`���[�g���A�� && ���]�̃X�e�b�v�ȏ�

				//���]�������
				pTutorial->aControl.bCutePillow = true;
			}

			if (pPlayer->bFever == true)
			{//�t�B�[�o�[���̎�

				g_aCrowd[nCntCrowd].bFever = true;
			}

			//���f���̑S���_�`�F�b�N
			CheckVtx(g_aCrowd[nCntCrowd].aModel.rot.y, &g_aCrowd[nCntCrowd].aModel.vtxMax, &g_aCrowd[nCntCrowd].aModel.vtxMin, g_aCrowd[nCntCrowd].aModel.pMesh, g_aCrowd[nCntCrowd].aModel.pVtxBuff);

			//����̈ʒu�ݒ�
			g_aCrowd[nCntCrowd].aFormation.nIndex = SetFormationPos(nParent);

			//�e��ݒ�
			g_aCrowd[nCntCrowd].aModel.nIdxShadow = SetShadow(g_aCrowd[nCntCrowd].aModel.vtxMax.x * 1.5f, g_aCrowd[nCntCrowd].aModel.vtxMax.z * 1.5f);

			//�e�̈ʒu�X�V
			SetPositionShadow(g_aCrowd[nCntCrowd].aModel.nIdxShadow, g_aCrowd[nCntCrowd].aModel.pos);
			break;
		}
	}
}


//==================================================================================
//�Q�O�̏��擾
//==================================================================================
Crowd *GetCrowd(void)
{
	return &g_aCrowd[0];
}

//==================================================================================
//�Q�O�̏��擾
//==================================================================================
CrowdInfo *GetCrowdInfo(void)
{
	return &g_aCrowdInfo;
}

//==================================================================================
//�Q�O�̐��̏��擾
//==================================================================================
CrowdNum *GetCrowdNum(void)
{
	return &g_aCrowdNum;
}

//==================================================================================
//���U���g���̌Q�O�̍X�V����
//==================================================================================
void UpdateResultCrowd(void)
{
	for (int nCntCrowd = 0; nCntCrowd < MAX_CROWD; nCntCrowd++)
	{
		//�d�͉����x
		g_aCrowd[nCntCrowd].aModel.move.y -= 0.1f;

		//�d��
		g_aCrowd[nCntCrowd].aModel.pos.y += g_aCrowd[nCntCrowd].aModel.move.y;

		if (g_aCrowd[nCntCrowd].aModel.pos.y <= 0)
		{//0�ȉ��̎�

			g_aCrowd[nCntCrowd].aModel.move.y *= -1;

			g_aCrowd[nCntCrowd].aModel.move.x = ((float)(rand() % 629 - 314) / 100.0f) * 5.0f;
		}

		g_aCrowd[nCntCrowd].aModel.pos.x += g_aCrowd[nCntCrowd].aModel.move.x;

		if (g_aCrowd[nCntCrowd].aModel.pos.x <= -900.0f)
		{
			g_aCrowd[nCntCrowd].aModel.pos.x = -900.0f;
		}
		else if (g_aCrowd[nCntCrowd].aModel.pos.x >= 900.0f)
		{
			g_aCrowd[nCntCrowd].aModel.pos.x = 900.0f;
		}

	}
}
