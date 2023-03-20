//=============================================================================
//
// �v���C���[���� [player.h]
// Author : ���n�Ή�
//
//=============================================================================
#include "main.h"
#include "player.h"
#include "input.h"
#include "camera.h"
#include "model.h"
#include "meshwall.h"
#include "game.h"
#include "edit.h"
#include "tutorial.h"
#include "impactwave.h"
#include "calculation.h"
#include "shadow.h"
#include "particle.h"
#include "debugproc.h"
#include "sound.h"


//�}�N����`
#define DMG_TIME	(10)	//�_���[�W��Ԃ̎���
#define ULT_WAIT	(100)	//�҂����
#define FLAG_POS	(150.0f)	//���̈ʒu
#define FLEN		(800.0f)
#define PLAYER_TXT	"data\\TEXT\\motion_set_player.txt"

//�v���g�^�C�v�錾
void UpdateGamePlayer(void);
void ControllPlayer(void);
void ReadSetPlayer(void);
void ReadSetMotionPlayer(void);
void UpdateMotionPlayer(void);
void LimitPosPlayer(void);
void CollisionCharPlayer(void);
void CollisionWallPlayer(void);
void UpdateStatePlayer(void);
void UpdateATKPlayer(void);
void CollisionATKPlayer(float fDistance, float fAngle, int nValue);

//�O���[�o���ϐ��錾
Player g_aPlayer;		//�v���C���[�̏��
int g_nGameTimePlayer;

//==================================================================================
//�v���C���[�̏���������
//==================================================================================
void InitPlayer(void)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//�e�v�f������
	g_aPlayer.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.posOld = g_aPlayer.pos;
	g_aPlayer.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_aPlayer.bUse = true;										//�g�p���Ă��邩

	g_aPlayer.vecMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ��x�N�g��
	g_aPlayer.vecToPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�x�N�g���̒���
	g_aPlayer.fRotDiff = 0.0f;									//�����̍���
	g_aPlayer.fRotDest = g_aPlayer.rot.y;						//�����̍���

																//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);
	g_aPlayer.nIdxShadow = -1;									//�e�̃C���f�b�N�X�ԍ�
	g_aPlayer.nIdxHPGauge = -1;									//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
	g_aPlayer.nState = PLAYERSTATE_NONE;						//���
	g_aPlayer.nCntState = 0;									//��ԃJ�E���g
	g_aPlayer.bDisp = true;										//�`�悵�Ă��邩


																//���[�V�����n������
	g_aPlayer.nNowMotionNum = -1;		//���݂̃��[�V�����ԍ�
	g_aPlayer.nCntAllFrame = 0;			//���t���[���J�E���g
	g_aPlayer.nCntFrame = 0;			//�t���[���̃J�E���g
	g_aPlayer.nPatternKey = 0;			//���ڂ̃L�[��
	g_aPlayer.nPartsNum = 0;			//�p�[�c��
	g_aPlayer.bMove = false;			//�ړ����Ă��邩
	g_aPlayer.bJump = false;			//�W�����v���Ă��邩
	g_aPlayer.bATK = false;				//�U�����Ă邩

	g_aPlayer.nCntWalk = 0;				//�ړ��̃J�E���g
	g_aPlayer.nCntPenlight = 0;			//�Ö��̃J�E���g
	g_aPlayer.nCntDis = 0;				//�h���̃J�E���g
	g_aPlayer.nCntReturn = 0;			//�A�҂̃J�E���g
	g_aPlayer.fRadius = 50.0f;			//���a

	for (int nCount = 0; nCount < MAX_MODEL; nCount++)
	{
		g_aPlayer.aModel[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���݂̈ʒu
		g_aPlayer.aModel[nCount].posOld = g_aPlayer.aModel[nCount].pos;			//�O��̈ʒu
		g_aPlayer.aModel[nCount].posOrigin = g_aPlayer.aModel[nCount].pos;		//���̈ʒu
		g_aPlayer.aModel[nCount].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
		g_aPlayer.aModel[nCount].rotOrigin = g_aPlayer.aModel[nCount].rot;		//���̌���
		g_aPlayer.aModel[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ���

																				//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].OldmtxWorld);

		g_aPlayer.aModel[nCount].nIdxShadow = -1;		//�e�̃C���f�b�N�X�ԍ�
		g_aPlayer.aModel[nCount].nShadow = -1;			//�e���g�����ǂ���

		g_aPlayer.aModel[nCount].nNumVtx = 0;			//���_��
		g_aPlayer.aModel[nCount].dwSizeFVF = NULL;		//���_�t�H�[�}�b�g�̃T�C�Y
		g_aPlayer.aModel[nCount].pVtxBuff = NULL;			//���_�o�b�t�@�̃|�C���^
		g_aPlayer.aModel[nCount].vtxMin = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);		//���f���̍ŏ��l
		g_aPlayer.aModel[nCount].vtxMax = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);			//���f���̍ő�l

		g_aPlayer.aModel[nCount].nType = -1;		//�ǂݍ��ރ��f���̎��
		g_aPlayer.aModel[nCount].nParent = -1;		//�e�̔ԍ�
		g_aPlayer.aModel[nCount].bUse = false;		//�g�p���Ă��邩
		g_aPlayer.aModel[nCount].nState = 0;		//���

		g_aPlayer.aModel[nCount].pMesh = NULL;				//���b�V��(���_���)�ւ̃|�C���^
		g_aPlayer.aModel[nCount].pBuffMat = NULL;			//�}�e���A���ւ̃|�C���^

		for (int nCntMat = 0; nCntMat < MAX_MAT; nCntMat++)
		{
			g_aPlayer.aModel[nCount].pMatData[nCntMat].MatD3D = {};
			g_aPlayer.aModel[nCount].pMatData[nCntMat].pTextureFilename = {};	//�}�e���A���̃f�[�^
			g_aPlayer.aModel[nCount].acFilename[nCntMat] = {};		//�t�@�C����
		}

		g_aPlayer.aModel[nCount].dwNumMat = NULL;			//�}�e���A���̐�

		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			g_aPlayer.aModel[nCount].pTexture[nCntTex] = {};		//�e�N�X�`���ւ̃|�C���^
		}

	}

	for (int nCntMotion = 0; nCntMotion < MAX_MOTION; nCntMotion++)
	{//���[�V�����̐��J��Ԃ�

		for (int nCntKey = 0; nCntKey < MAX_KEY; nCntKey++)
		{//�L�[�̐��J��Ԃ�

			for (int nCntParts = 0; nCntParts < MAX_PARTS; nCntParts++)
			{//�p�[�c�̐��J��Ԃ�

				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̌���
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rotDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�����̍���
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ڕW�̈ʒu
				g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].posDiff = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu�̍���

			}

			g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].nFrame = 0;	//�Đ��t���[��
		}

		g_aPlayer.aMotionData[nCntMotion].nNumKey = 0;				//�L�[�̐�
		g_aPlayer.aMotionData[nCntMotion].nLoop = MOTION_LOOP_ON;	//���[�v����
		g_aPlayer.aMotionData[nCntMotion].bUse = false;				//�g�p���Ă��邩
	}

	g_nGameTimePlayer = 0;

	//�O���t�@�C���ǂݍ���
	ReadSetPlayer();
	ReadSetMotionPlayer();

	//���[�V�����̐ݒ�
	SetMotisonPlayer(PLAYERMOTION_DEF);
}

//==================================================================================
//�v���C���[�̏I������
//==================================================================================
void UninitPlayer(void)
{

	for (int nCount = 0; nCount < MAX_MODEL; nCount++)
	{
		for (int nCntTex = 0; nCntTex < MAX_TX; nCntTex++)
		{
			//�e�N�X�`���̔j��
			if (g_aPlayer.aModel[nCount].pTexture[nCntTex] != NULL)
			{
				g_aPlayer.aModel[nCount].pTexture[nCntTex]->Release();
				g_aPlayer.aModel[nCount].pTexture[nCntTex] = NULL;
			}
		}

		//���b�V���̔j��
		if (g_aPlayer.aModel[nCount].pMesh != NULL)
		{
			g_aPlayer.aModel[nCount].pMesh->Release();
			g_aPlayer.aModel[nCount].pMesh = NULL;
		}

		//�}�e���A���̔j��
		if (g_aPlayer.aModel[nCount].pBuffMat != NULL)
		{
			g_aPlayer.aModel[nCount].pBuffMat->Release();
			g_aPlayer.aModel[nCount].pBuffMat = NULL;
		}
	}

}

//==================================================================================
//�v���C���[�̍X�V����
//==================================================================================
void UpdatePlayer(void)
{
	switch (GetMode())
	{
	case MODE_GAME:

		//�Q�[�����̍X�V����
		UpdateGamePlayer();
		break;
	}
}

//==================================================================================
//�Q�[�����̍X�V����
//==================================================================================
void UpdateGamePlayer(void)
{
	if (g_aPlayer.bUse == true)
	{//�g�p���Ă�����

	 //�ߋ��̈ʒu�ۑ�
		g_aPlayer.posOld = g_aPlayer.pos;

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//�p�[�c���J��Ԃ�

		 //�p�[�c���Ƃ̃}�g���b�N�X�擾
			D3DXMATRIX mtxParts = GetParentMatrix(nCntParts);

			g_aPlayer.aModel[nCntParts].OldmtxWorld = mtxParts;
		}

		//�v���C���[�̑���
		ControllPlayer();

		//�U������
		UpdateATKPlayer();

		if (g_aPlayer.bMove == true && g_aPlayer.bJump == false)
		{//�ړ����Ă�Ƃ�

			g_aPlayer.nCntWalk++;	//�J�E���g���Z

			if (g_aPlayer.nCntWalk > 24)
			{
				g_aPlayer.nCntWalk = 0;

				//���s�p�[�e�B�N���̐ݒ�
				SetParticle(g_aPlayer.pos, PARTICLE_TYPE_WALK);

				//PlaySound(SOUND_LABEL_SE_ASIOTO);
			}

			//���[�V�����̐ݒ�
			SetMotisonPlayer(PLAYERMOTION_WALK);
		}
		else
		{//�j���[�g�������[�V����

			if (g_aPlayer.nNowMotionNum != PLAYERMOTION_ACTION)
			{
				//�U����ԉ���
				g_aPlayer.bATK = false;

				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
		}

		//���[�V�����̍X�V
		UpdateMotionPlayer();

		//�L�����N�^�[�Ƃ̓����蔻��
		CollisionCharPlayer();

		//�ǂƂ̓����蔻��
		//CollisionWall(&g_aPlayer.pos, g_aPlayer.posOld);

		//���W����
		LimitPosPlayer();

		//�e�̈ʒu�X�V
		SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
	}

	//��ԍX�V
	UpdateStatePlayer();
}

//==================================================================================
//�Q�[���I�����̍X�V����
//==================================================================================
void ControllEndPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	g_aPlayer.bMove = true;

	g_aPlayer.rot.y = 0.0f;

	g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
	g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
	g_aPlayer.fRotDest = pCamera->rot.y;

	//�d�͏���
	//g_aPlayer.move.y -= 1.0f;

	//�ʒu�X�V
	g_aPlayer.pos += g_aPlayer.move;

	//����
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;
}

//==================================================================================
//���f���̑���
//==================================================================================
void ControllPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//�G�f�B�b�g�̏��擾
	Edit  *pEdit = GetEdit();

	if (GetGameState() == GAMESTATE_NONE && pEdit->bUse == false)
	{//�^�[�Q�b�g���ȊO

		if (g_aPlayer.nNowMotionNum != PLAYERMOTION_DISPATCHL)
		{//�ړ��s���[�V�������͋����L�����Z��

			if (GetKeyboardPress(DIK_A) == true || XGetStickPressL(BUTTON_LX, 0) < 0)
			{//���L�[�������ꂽ,���ړ�

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//A+W,����ړ�

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LRUP + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//A+S,�����ړ�

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LRDW + pCamera->rot.y;
				}
				else
				{//A,���ړ�

					g_aPlayer.move.x += sinf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(-D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = D3DX_PI * MOVE_LR + pCamera->rot.y;
				}

				g_aPlayer.bMove = true;
			}
			else if (GetKeyboardPress(DIK_D) == true || XGetStickPressL(BUTTON_LX, 0) > 0)
			{//D�L�[�������ꂽ,�E�ړ�

				if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
				{//D+W,�E��ړ�

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRDW + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRUP + pCamera->rot.y;
				}
				else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
				{//D+S,�E���ړ�

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LRUP + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LRDW + pCamera->rot.y;
				}
				else
				{//D,�E�ړ�

					g_aPlayer.move.x += sinf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.move.z += cosf(D3DX_PI * MOVE_LR + pCamera->rot.y) * CHARA_MOVE;
					g_aPlayer.fRotDest = -D3DX_PI * MOVE_LR + pCamera->rot.y;
				}

				g_aPlayer.bMove = true;
			}
			else if (GetKeyboardPress(DIK_W) == true || YGetStickPressL(BUTTON_LY, 0) > 0)
			{//W�������ꂽ�A���ړ�

				g_aPlayer.move.x += sinf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.move.z += cosf(pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.fRotDest = D3DX_PI + pCamera->rot.y;

				g_aPlayer.bMove = true;
			}
			else if (GetKeyboardPress(DIK_S) == true || YGetStickPressL(BUTTON_LY, 0) < 0)
			{//S�������ꂽ�A��O�ړ�

				g_aPlayer.move.x += sinf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.move.z += cosf(D3DX_PI + pCamera->rot.y) * CHARA_MOVE;
				g_aPlayer.fRotDest = pCamera->rot.y;

				g_aPlayer.bMove = true;
			}
			else
			{//�Ȃ�̃L�[��������Ă��Ȃ�

				if (g_aPlayer.move.x <= 1.0f && g_aPlayer.move.x >= -1.0f ||
					g_aPlayer.move.z <= 1.0f && g_aPlayer.move.z >= -1.0f)
				{
					g_aPlayer.bMove = false;

				}
			}
		}
	}

	//���݂ƖڕW�̍��������߂�
	g_aPlayer.fRotDiff = g_aPlayer.fRotDest - g_aPlayer.rot.y;

	//�p�x�̐��K��
	RotNormalize(&g_aPlayer.fRotDiff);

	//�p�x�̕␳������
	g_aPlayer.rot.y += g_aPlayer.fRotDiff * 0.1f;

	//�p�x�̐��K��
	RotNormalize(&g_aPlayer.rot.y);

	//�d�͏���
	//g_aPlayer.move.y -= 1.0f;

	//�ʒu�X�V
	g_aPlayer.pos += g_aPlayer.move;

	//��������
	g_aPlayer.move.x += (0.0f - g_aPlayer.move.x) * MOVE_FACTOR;
	g_aPlayer.move.z += (0.0f - g_aPlayer.move.z) * MOVE_FACTOR;

	PrintDebugProc(
		"\n------�v���C���[�̑���------\n"
		"<�ړ�> W/A/S/D\n"
		"<�ʒu> [%f, %f, %f]\n"
		"<�ړ���> [%f, %f, %f]\n",
		g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z,
		g_aPlayer.move.x, g_aPlayer.move.y, g_aPlayer.move.z);

}

//==================================================================================
//�v���C���[�̍��W����
//==================================================================================
void LimitPosPlayer(void)
{
	if (g_aPlayer.pos.y < 0.0f)
	{//�n�ʂ�艺�ɍs������

	 //g_aPlayer.pos.y = 0.0f;

	 //�d�͏���
		g_aPlayer.move.y = 0.0f;

		g_aPlayer.bJump = false;
	}

	//�ړ�����
	if (g_aPlayer.pos.x - g_aPlayer.fRadius <= -LIMIT_POS && g_aPlayer.posOld.x - g_aPlayer.fRadius >= -LIMIT_POS)
	{//����-1000�ȉ�, �O��-1000�ȏ�

		g_aPlayer.pos.x = -LIMIT_POS + g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.x + g_aPlayer.fRadius >= LIMIT_POS && g_aPlayer.posOld.x + g_aPlayer.fRadius <= LIMIT_POS)
	{//����1000�ȏ�, �O��1000�ȉ�

		g_aPlayer.pos.x = LIMIT_POS - g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.z + g_aPlayer.fRadius >= LIMIT_POS && g_aPlayer.posOld.z + g_aPlayer.fRadius <= LIMIT_POS)
	{//����1000�ȏ�, �O��1000�ȉ�

		g_aPlayer.pos.z = LIMIT_POS - g_aPlayer.fRadius;
	}
	if (g_aPlayer.pos.z - g_aPlayer.fRadius <= -LIMIT_POS && g_aPlayer.posOld.z - g_aPlayer.fRadius >= -LIMIT_POS)
	{//����-1000�ȉ�, �O��-1000�ȏ�

		g_aPlayer.pos.z = -LIMIT_POS + g_aPlayer.fRadius;
	}
}

//==================================================================================
//�U������
//==================================================================================
void UpdateATKPlayer(void)
{
	switch (g_aPlayer.nNowMotionNum)
	{
	case PLAYERMOTION_ACTION:

		//�U���̓����蔻��
		CollisionATKPlayer(PENLIGHT_RADIUS, PENLIGHT_ANGLE, 1);
		break;
	}
}

//==================================================================================
//�v���C���[�̍U��
//==================================================================================
void CollisionATKPlayer(float fDistance, float fAngle, int nValue)
{
	//�y�����C�g�̍U��
	bool bAtk = false;

	//�y�����C�g�̓����蔻��
	/*bAtk = CollisionFan(g_aPlayer.pos,
	D3DXVECTOR3(g_aPlayer.pos.x + sinf(fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(fAngle + g_aPlayer.rot.y) * -fDistance),
	D3DXVECTOR3(g_aPlayer.pos.x + sinf(-fAngle + g_aPlayer.rot.y) * -fDistance, g_aPlayer.pos.y, g_aPlayer.pos.z + cosf(-fAngle + g_aPlayer.rot.y) * -fDistance),
	pMob->aModel.pos, pMob->aModel.posOld);*/

	if (bAtk == true)
	{//��͈̔͂ɓ�������

	 //���u�̃q�b�g����
	 //HitMob(nCntMob, nValue);
	}
}

//==================================================================================
//�ǂƂ̓����蔻��
//==================================================================================
void CollisionWallPlayer(void)
{
	//�ǂ̏��擾
	MESHWALL *pMeshWall = GetMeshWall();

	for (int nCntWall = 0; nCntWall < MAX_WALL; nCntWall++, pMeshWall++)
	{//�ǂ̐����J��Ԃ�

		if (pMeshWall->bUse == true)
		{//�ǂ��g�p����Ă���Ƃ�

		 //�ǂ̕�
			float fLen = (pMeshWall->nWidth * POS_MESHWALL) * 0.5f;

			//���̒��_���W
			D3DXVECTOR3 pos0 = D3DXVECTOR3(
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * -fLen,
				g_aPlayer.pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * -fLen);

			//�E�̒��_���W
			D3DXVECTOR3 pos1 = D3DXVECTOR3(
				pMeshWall->pos.x + sinf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen,
				g_aPlayer.pos.y,
				pMeshWall->pos.z + cosf(pMeshWall->rot.y + D3DX_PI * MOVE_LR) * fLen);

			//���E���̃x�N�g��
			D3DXVECTOR3 vecLine;
			vecLine.x = pos1.x - pos0.x;
			vecLine.z = pos1.z - pos0.z;

			//�v���C���[�̋��E���̃x�N�g��
			D3DXVECTOR3 vecLinePlayer;
			vecLinePlayer.x = g_aPlayer.pos.x - g_aPlayer.posOld.x;
			vecLinePlayer.z = g_aPlayer.pos.z - g_aPlayer.posOld.z;

			//�v���C���[�ƕǂ̃x�N�g��
			D3DXVECTOR3 vecToPosPlayer;
			vecToPosPlayer.x = pos1.x - g_aPlayer.posOld.x;
			vecToPosPlayer.z = pos1.z - g_aPlayer.posOld.z;

			//�ʐς̍ő�l
			float fMaxAreaPlayer = (vecLinePlayer.z * vecLine.x) - (vecLinePlayer.x * vecLine.z);

			//����̖ʐ�
			float fNowAreaPlayer = (vecToPosPlayer.z * vecLine.x) - (vecToPosPlayer.x * vecLine.z);

			//����
			float fRatePlayer = fNowAreaPlayer / fMaxAreaPlayer;

			if (fRatePlayer >= 0.0f && fRatePlayer <= 1.0f)
			{//�ʐς͈͓̔��ɂ����画��

				if ((vecLinePlayer.z * vecToPosPlayer.x) - (vecLinePlayer.x * vecToPosPlayer.z) > 0)
				{//�ǂɓ���������

				 //��_����͂ݏo����
					D3DXVECTOR3 CollisionPointPlayer = D3DXVECTOR3(0.0f, g_aPlayer.pos.y, 0.0f);
					CollisionPointPlayer.x = (g_aPlayer.pos.x + (vecLinePlayer.x * (fRatePlayer - 1.0f)));
					CollisionPointPlayer.z = (g_aPlayer.pos.z + (vecLinePlayer.z * (fRatePlayer - 1.0f)));

					//�@���x�N�g��(���E���x�N�g����X��Z���])
					D3DXVECTOR3 vecNor = D3DXVECTOR3(vecLine.z, 0.0f, -vecLine.x);

					//�x�N�g���̐��K��
					D3DXVec3Normalize(&vecNor, &vecNor);

					//�v���C���[�̋t�ړ���
					D3DXVECTOR3 PlayerInverceMove;
					PlayerInverceMove.z = vecLinePlayer.z * (fRatePlayer - 1.0f);
					PlayerInverceMove.x = vecLinePlayer.x * (fRatePlayer - 1.0f);

					//����(�ǂ̖@���ƃv���C���[�̋t�ړ���)
					float fDot = (PlayerInverceMove.x * vecNor.x) + (PlayerInverceMove.z * vecNor.z);

					//�ǂ���ړ���
					D3DXVECTOR3 MoveWall = vecNor * fDot;

					D3DXVECTOR3 ppp = (CollisionPointPlayer - g_aPlayer.pos);
					D3DXVECTOR3 pppp = ppp + MoveWall;

					//�Ԃ������_�ɕ␳
					g_aPlayer.pos += MoveWall + (vecNor * 0.1f);

				}

			}

		}

	}
}

//==================================================================================
//�v���C���[�̏�ԍX�V
//==================================================================================
void UpdateStatePlayer(void)
{

	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	D3DXMATERIAL *pMat;	//�}�e���A���f�[�^�ւ̃|�C���^

	switch (g_aPlayer.nState)
	{
	case PLAYERSTATE_NONE:

		for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
		{//�p�[�c���J��Ԃ�

		 //�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

			//���_�����J��Ԃ�
			for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
			{
				//���̐F�ɖ߂�
				pMat[nCntMat].MatD3D.Diffuse = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Diffuse;
				pMat[nCntMat].MatD3D.Ambient = g_aPlayer.aModel[nCntParts].pMatData[nCntMat].MatD3D.Ambient;
			}
		}
		break;
	}
}

//==================================================================================
//�L�����N�^�[�Ƃ̓����蔻��
//==================================================================================
void CollisionCharPlayer(void)
{

	//���f���̏��擾
	ModelModel *pModel = GetModel();

	for (int nCntModel = 0; nCntModel < MAX_MODEL; nCntModel++, pModel++)
	{
		if (pModel->bUse == true)
		{//���f�����g�p����Ă�����

		 //�L�����N�^�[���m�̓����蔻��
			CollisionCharacter(
				&g_aPlayer.pos, &g_aPlayer.posOld, D3DXVECTOR3(g_aPlayer.fRadius, 0.0f, g_aPlayer.fRadius), D3DXVECTOR3(-g_aPlayer.fRadius, 0.0f, -g_aPlayer.fRadius),
				&pModel->posOrigin, pModel->vtxMax, pModel->vtxMin);
		}
	}
}

//==================================================================================
//�v���C���[�̃��[�V��������
//==================================================================================
void SetMotisonPlayer(int nMotionType)
{

	//���̃��[�V�����̍X�V
	if (g_aPlayer.nNowMotionNum != nMotionType)
	{//�Z�b�g���郂�[�V�����ƌ��݂̃��[�V��������������Ȃ�������

		g_aPlayer.nNowMotionNum = nMotionType;
		g_aPlayer.nPatternKey = 0;	//���݂̃L�[�ԍ����Z�b�g
		g_aPlayer.nCntFrame = 0;	//�t���[���J�E���g���Z�b�g
		g_aPlayer.nCntAllFrame = 0;	//���t���[�������Z�b�g
	}
}

//==================================================================================
//�v���C���[�̃��[�V�����X�V����
//==================================================================================
void UpdateMotionPlayer(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

	 //���̃L�[
		int nNextKey = (g_aPlayer.nPatternKey + 1) % g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey;
		int nnn;

		if (g_aPlayer.nPatternKey == g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
		{//���[�v���Ȃ��Ƃ�

		 //nNextKey = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		if (g_aPlayer.nPatternKey == g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1 && g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_ON)
		{//���[�v�̎�

			nnn = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey - 1;
		}

		//�O��Ǝ��̃L�[�̍����擾
		float rotDiffX = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.x -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x;

		float rotDiffY = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.y -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y;

		float rotDiffZ = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].rot.z -
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z;

		//�p�[�c�̌�����ݒ�
		g_aPlayer.aModel[nCntParts].rot.x =
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.x +
			rotDiffX *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		g_aPlayer.aModel[nCntParts].rot.y =
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.y +
			rotDiffY *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		g_aPlayer.aModel[nCntParts].rot.z =
			g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].rot.z +
			rotDiffZ *
			(
			(float)g_aPlayer.nCntFrame /
				(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame
				);

		//�p�x�̐��K��
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.x);
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.y);
		RotNormalize(&g_aPlayer.aModel[g_aPlayer.nNowMotionNum].rot.z);


		//�p�[�c�̈ʒu��ݒ�
		if (nCntParts == 0)
		{
			//�ڕW�̈ʒu�Ƃ̍��������߂�
			float posDiffX = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.x -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x;

			float posDiffY = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.y -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y;

			float posDiffZ = g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[nNextKey].aParts[nCntParts].pos.z -
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z;

			//�ʒu�␳
			g_aPlayer.pos.x +=
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.x +
				posDiffX *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);

			g_aPlayer.pos.y =
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.y +
				posDiffY *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);

			g_aPlayer.pos.z +=
				g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].aParts[nCntParts].pos.z +
				posDiffZ *
				(
				(float)g_aPlayer.nCntFrame /
					(float)g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame);
		}
	}

	//�t���[���̃J�E���g�����Z
	g_aPlayer.nCntFrame++;
	g_aPlayer.nCntAllFrame++;

	if (g_aPlayer.nCntFrame >= g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].aKey[g_aPlayer.nPatternKey].nFrame)
	{//�t���[���̃J�E���g���t���[�����ɒB������

	 //�t���[���̃J�E���g���[���ɖ߂�
		g_aPlayer.nCntFrame = 0;

		//�p�^�[��NO.�X�V
		g_aPlayer.nPatternKey = (g_aPlayer.nPatternKey + 1) % g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nNumKey;

		if (g_aPlayer.nPatternKey == 0)
		{//�L�[���[���̎�

		 //�U����ԉ���
			g_aPlayer.nCntAllFrame = 0;	//���t���[�������Z�b�g

			if (g_aPlayer.aMotionData[g_aPlayer.nNowMotionNum].nLoop == MOTION_LOOP_OFF)
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(PLAYERMOTION_DEF);
			}
			else
			{
				//���[�V�����̐ݒ�
				SetMotisonPlayer(g_aPlayer.nNowMotionNum);
			}
		}
	}
}

//==================================================================================
//�v���C���[�̕`�揈��
//==================================================================================
void DrawPlayer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;				//�}�e���A���f�[�^�ւ̃|�C���^
	D3DXMATRIX mtxnParent;			//�e�̌v�Z�p�}�g���b�N�X

	if (g_aPlayer.bDisp == true && g_aPlayer.bUse == true)
	{//�g�p���Ă�����

	 //���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

		//�����𔽉f����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

		//�ʒu�𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
		D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		for (int nCount = 0; nCount < g_aPlayer.nPartsNum; nCount++)
		{

			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);

			//�����𔽉f����
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCount].rot.y, g_aPlayer.aModel[nCount].rot.x, g_aPlayer.aModel[nCount].rot.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxRot);

			//�ʒu�𔽉f����
			D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCount].pos.x, g_aPlayer.aModel[nCount].pos.y, g_aPlayer.aModel[nCount].pos.z);
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxTrans);


			if (g_aPlayer.aModel[nCount].nParent == -1)
			{//NULL�̏ꍇ,�e

			 //�v���C���[�̌����𔽉f����
				mtxnParent = g_aPlayer.mtxWorld;
			}
			else
			{//�p�[�c

			 //�e�̌����𔽉f����
				mtxnParent = g_aPlayer.aModel[g_aPlayer.aModel[nCount].nParent].mtxWorld;
			}

			//�����̂�ɐe�̃��[���h�}�g���b�N�X���|���Z
			D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxnParent);

			//���[���h�}�g���b�N�X�̐ݒ�
			pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCount].mtxWorld);

			//�}�e���A���f�[�^�ւ̃|�C���^���擾
			pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCount].pBuffMat->GetBufferPointer();

			if (g_aPlayer.aModel[nCount].bUse == true)
			{//�p�[�c���g�p����Ă�����

			 //���_�����J��Ԃ�
				for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCount].dwNumMat; nCntMat++)
				{
					//�}�e���A���̐ݒ�
					pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

					//�e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, g_aPlayer.aModel[nCount].pTexture[nCntMat]);

					//�v���C���[(�p�[�c)�̕`��
					g_aPlayer.aModel[nCount].pMesh->DrawSubset(nCntMat);
				}
			}
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}

}

//==================================================================================
//�v���C���[�̐ݒ菈��
//==================================================================================
void SetPlayer(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	for (int nCntParts = 0; nCntParts < g_aPlayer.nPartsNum; nCntParts++)
	{//�p�[�c���J��Ԃ�

	 //X�t�@�C���̓ǂݍ���
		D3DXLoadMeshFromX(&g_aPlayer.aModel[nCntParts].acFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&g_aPlayer.aModel[nCntParts].pBuffMat,
			NULL,
			&g_aPlayer.aModel[nCntParts].dwNumMat,
			&g_aPlayer.aModel[nCntParts].pMesh);

		D3DXMATERIAL *pMat;		//�}�e���A���f�[�^�ւ̃|�C���^

								//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)g_aPlayer.aModel[nCntParts].pBuffMat->GetBufferPointer();

		//���_�����J��Ԃ�
		for (int nCntMat = 0; nCntMat < (int)g_aPlayer.aModel[nCntParts].dwNumMat; nCntMat++)
		{
			//�}�e���A���̃f�[�^�擾
			g_aPlayer.aModel[nCntParts].pMatData[nCntMat] = pMat[nCntMat];

			if (pMat[nCntMat].pTextureFilename != NULL)
			{//�t�@�C�������݂���

			 //�e�N�X�`���̓ǂݍ���
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&g_aPlayer.aModel[nCntParts].pTexture[nCntMat]);
			}
		}

		//�p�[�c���g�p���
		g_aPlayer.aModel[nCntParts].bUse = true;

		//���f���̑S���_�`�F�b�N
		CheckVtx(g_aPlayer.aModel[nCntParts].rot.y, &g_aPlayer.aModel[nCntParts].vtxMax, &g_aPlayer.aModel[nCntParts].vtxMin, g_aPlayer.aModel[nCntParts].pMesh, g_aPlayer.aModel[nCntParts].pVtxBuff);
	}

	//�e�̐ݒ�
	g_aPlayer.nIdxShadow = SetShadow(g_aPlayer.fRadius * 0.5f, g_aPlayer.fRadius * 0.5f);
	SetPositionShadow(g_aPlayer.nIdxShadow, g_aPlayer.pos);
}

//==================================================================================
//��΃}�g���b�N�X�̎擾
//==================================================================================
D3DXMATRIX GetParentMatrix(int nIdxParts)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X�錾
	D3DXMATRIX mtxnParent;			//�e�̌v�Z�p�}�g���b�N�X

									//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&g_aPlayer.mtxWorld);

	//�����𔽉f����
	D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.rot.y, g_aPlayer.rot.x, g_aPlayer.rot.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxRot);

	//�ʒu�𔽉f����
	D3DXMatrixTranslation(&mtxTrans, g_aPlayer.pos.x, g_aPlayer.pos.y, g_aPlayer.pos.z);
	D3DXMatrixMultiply(&g_aPlayer.mtxWorld, &g_aPlayer.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.mtxWorld);

	for (int nCount = 0; nCount < g_aPlayer.nPartsNum; nCount++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&g_aPlayer.aModel[nCount].mtxWorld);

		//�����𔽉f����
		D3DXMatrixRotationYawPitchRoll(&mtxRot, g_aPlayer.aModel[nCount].rot.y, g_aPlayer.aModel[nCount].rot.x, g_aPlayer.aModel[nCount].rot.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxRot);

		//�ʒu�𔽉f����
		D3DXMatrixTranslation(&mtxTrans, g_aPlayer.aModel[nCount].pos.x, g_aPlayer.aModel[nCount].pos.y, g_aPlayer.aModel[nCount].pos.z);
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxTrans);


		if (g_aPlayer.aModel[nCount].nParent == -1)
		{//NULL�̏ꍇ,�e

		 //�v���C���[�̌����𔽉f����
			mtxnParent = g_aPlayer.mtxWorld;
		}
		else
		{//�p�[�c

		 //�e�̌����𔽉f����
			mtxnParent = g_aPlayer.aModel[g_aPlayer.aModel[nCount].nParent].mtxWorld;
		}

		//�����̂�ɐe�̃��[���h�}�g���b�N�X���|���Z
		D3DXMatrixMultiply(&g_aPlayer.aModel[nCount].mtxWorld, &g_aPlayer.aModel[nCount].mtxWorld, &mtxnParent);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &g_aPlayer.aModel[nCount].mtxWorld);
	}

	return g_aPlayer.aModel[nIdxParts].mtxWorld;
}

//==================================================================================
//�v���C���[�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetPlayer(void)
{
	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

						//�t�@�C�����J��
	pFile = fopen(PLAYER_TXT, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		char aComment[MAX_COMMENT];	//�R�����g�A�m�F��

		int nCntSetParts = 0;
		int nCntFileName = 0;

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

		 //������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//���f�����̐ݒ�
			if (strcmp(aComment, "NUM_MODEL") == 0)
			{//NUM_MODEL��������

				fscanf(pFile, "%s", &aComment[0]);	//=�̕�
				fscanf(pFile, "%d", &g_aPlayer.nPartsNum);	//���f����
			}

			while (nCntFileName != g_aPlayer.nPartsNum)
			{//���f���̐����ǂݍ��ނ܂ŌJ��Ԃ�

			 //������̓ǂݍ���
				fscanf(pFile, "%s", &aComment[0]);

				//���f�����̐ݒ�
				if (strcmp(aComment, "MODEL_FILENAME") == 0)
				{//NUM_MODEL��������

					fscanf(pFile, "%s", &aComment[0]);	//=�̕�
					fscanf(pFile, "%s", &g_aPlayer.aModel[nCntFileName].acFilename[0]);	//�t�@�C����
					nCntFileName++;	//�t�@�C�������Z
				}
			}

			//�e�p�[�c�̐ݒ�
			if (strcmp(aComment, "CHARACTERSET") == 0)
			{//�L�����N�^�[���̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_CHARACTERSET") != 0)
				{//END_CHARACTERSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "PARTSSET") == 0)
					{//PARTSSET�Ńp�[�c���̓ǂݍ��݊J�n

						while (strcmp(aComment, "END_PARTSSET") != 0)
						{//END_PARTSSET������܂ŌJ��Ԃ�

							fscanf(pFile, "%s", &aComment[0]);	//�m�F����

							if (strcmp(aComment, "INDEX") == 0)
							{//INDEX�������烂�f���ԍ��ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nType);	//���f����ނ̗�

							}

							if (strcmp(aComment, "PARENT") == 0)
							{//PARENT��������e�̔ԍ��ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aPlayer.aModel[nCntSetParts].nParent);	//�e�̔ԍ�

							}

							if (strcmp(aComment, "POS") == 0)
							{//POS��������ʒu�ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.x);	//X���W
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.y);	//Y���W
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].pos.z);	//Z���W

																							//���̈ʒu���蓖��
								g_aPlayer.aModel[nCntSetParts].posOrigin = g_aPlayer.aModel[nCntSetParts].pos;

							}

							if (strcmp(aComment, "ROT") == 0)
							{//ROT������������ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.x);	//X�̊p�x
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.y);	//Y�̊p�x
								fscanf(pFile, "%f", &g_aPlayer.aModel[nCntSetParts].rot.z);	//Z�̊p�x

							}

							if (strcmp(aComment, "WEAPON") == 0)
							{//WEAPON�������畐�킩�ǂ����ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�

							}

						}//END_PARTSSET�̂�����

						nCntSetParts++;	//�p�[�c�̃J�E���g�����Z
					}

				}//END_CHARACTERSET�̂�����

			}

			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//�I�������Ń��[�v�𔲂���

				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}

	//�v���C���[�̔z�u
	SetPlayer();

}

//==================================================================================
//�v���C���[�̓ǂݍ��ݏ���
//==================================================================================
void ReadSetMotionPlayer(void)
{

	FILE *pFile = NULL;	//�t�@�C���|�C���^��錾

	char aComment[MAX_COMMENT];	//�R�����g�p
	int nCntMotion = 0, nCntKey = 0, nCntParts = 0;

	//�t�@�C�����J��
	pFile = fopen(PLAYER_TXT, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (1)
		{//END_SCRIPT������܂ŌJ��Ԃ�

		 //������̓ǂݍ���
			fscanf(pFile, "%s", &aComment[0]);

			//==================================================
			//�e���[�V�����̐ݒ�
			//==================================================
			if (strcmp(aComment, "MOTIONSET") == 0)
			{//���[�V�������̓ǂݍ��݂��J�n

				while (strcmp(aComment, "END_MOTIONSET") != 0)
				{//END_MOTIONSET������܂ŌJ��Ԃ�

					fscanf(pFile, "%s", &aComment[0]);	//�m�F����

					if (strcmp(aComment, "LOOP") == 0)
					{//LOOP�������烋�[�vON/OFF�ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aPlayer.aMotionData[nCntMotion].nLoop);	//���[�v0��1

					}

					if (strcmp(aComment, "NUM_KEY") == 0)
					{//NUM_KEY��������L�[���ǂݍ���

						fscanf(pFile, "%s", &aComment[0]);		//=�̕�
						fscanf(pFile, "%d", &g_aPlayer.aMotionData[nCntMotion].nNumKey);	//�L�[��

					}

					//==================================================
					//�e�L�[�̐ݒ�
					//==================================================
					if (strcmp(aComment, "KEYSET") == 0)
					{//KEYSET�ŃL�[���̓ǂݍ��݊J�n

						while (strcmp(aComment, "END_KEYSET") != 0)
						{//END_KEYSET������܂ŌJ��Ԃ�

							fscanf(pFile, "%s", &aComment[0]);	//�m�F����

							if (strcmp(aComment, "FRAME") == 0)
							{//FRAME��������Đ��t���[�����ǂݍ���

								fscanf(pFile, "%s", &aComment[0]);		//=�̕�
								fscanf(pFile, "%d", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].nFrame);	//�Đ��t���[��

							}

							//==================================================
							//�p�[�c�̐ݒ�
							//==================================================
							if (strcmp(aComment, "PARTS") == 0)
							{//PARTS�Ńp�[�c���̓ǂݍ��݊J�n

								while (strcmp(aComment, "END_PARTS") != 0)
								{//END_PARTS������܂ŌJ��Ԃ�

									fscanf(pFile, "%s", &aComment[0]);	//�m�F����

									if (strcmp(aComment, "POS") == 0)
									{//POS��������ʒu�ǂݍ���

										fscanf(pFile, "%s", &aComment[0]);		//=�̕�
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.x);	//X���W
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.y);	//Y���W
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].pos.z);	//Z���W
									}

									if (strcmp(aComment, "ROT") == 0)
									{//ROT��������p�x�ǂݍ���

										fscanf(pFile, "%s", &aComment[0]);		//=�̕�
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.x);	//X�p�x
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.y);	//Y�p�x
										fscanf(pFile, "%f", &g_aPlayer.aMotionData[nCntMotion].aKey[nCntKey].aParts[nCntParts].rot.z);	//Z�p�x
									}

								}//END_PARTS�̂�����

								nCntParts++;	//�p�[�c�̃J�E���g���Z
							}


						}//END_KEYSET�̂�����

						nCntParts = 0;	//�p�[�c�̃J�E���g���Z�b�g
						nCntKey++;	//�L�[�̃J�E���g���Z
					}

				}//END_MOTIONSET�̂�����

				nCntKey = 0;	//�L�[�̃J�E���g���Z�b�g
				nCntMotion++;	//���[�V�����̃J�E���g���Z

			}


			if (strcmp(aComment, "END_SCRIPT") == 0)
			{//�I�������Ń��[�v�𔲂���

				break;
			}
		}

		//�t�@�C�������
		fclose(pFile);
	}
}

//==================================================================================
//�v���C���[�̏��擾
//==================================================================================
Player *GetPlayer(void)
{
	return &g_aPlayer;
}