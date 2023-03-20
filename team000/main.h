//==============================================================
//
//DirectX[main.h]
//Author:����������
//
//==============================================================
#ifndef _MAIN_H_		//���̃}�N����`������Ă��Ȃ�������
#define _MAIN_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include<windows.h>
#include<time.h>
#include"d3dx9.h"							//�`�揈���ɕK�v
#define DIRECTINPUT_VERSION		(0x0800)	//�r���h���̌x���Ώ��p�}�N��
#include"dinput.h"							//���͏����ɕK�v
#include"xaudio2.h"							//�T�E���h�����ɕK�v
#include<stdio.h>

//���C�u�����̃����N
#pragma comment(lib,"d3d9.lib")		//�`�揈���ɕK�v
#pragma comment(lib,"d3dx9.lib")	//[d3d9.lib]�̊g�����C�u����
#pragma comment(lib,"dxguid.lib")	//DirectX�R���|�[�l���g(���i)�g�p�ɕK�v
#pragma comment(lib,"winmm.lib")	//�V�X�e�������擾�ɕK�v
#pragma comment(lib,"dinput8.lib")	//���͏����ɕK�v

//�}�N����`
#define SCREEN_WIDTH		(1280)															//�E�C���h�E�̕�
#define SCREEN_HEIGHT		(720)															//�E�C���h�E�̍���
#define FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)					//���_�t�H�[�}�b�g2D
#define FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)		//���_�t�H�[�}�b�g3D
#define FVF_VERTEX_3D_MULTI	(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2)		//���_�t�H�[�}�b�g3D_MULTI
#define MOVE_LRUP			(0.75f)															//�΂ߏ�ړ�
#define MOVE_LR				(0.5f)															//�����ړ�
#define MOVE_LRDW			(0.25f)															//�΂ߏ�ړ�

#define MAX_MAT				(512)		//�}�e���A���T�C�Y
#define MAX_TX				(96)		//�e�N�X�`���T�C�Y
#define LIMIT_POS			(5000.0f)	//��l�̔���
#define CHARA_MOVE			(1.7f)		//�L�����N�^�[�̈ړ����x
#define MOVE_FACTOR			(0.25f)		//�L�����N�^�[�̈ړ��␳�W��
#define MOVE_FACTOR_HYPNOSIS	(0.5f)		//�L�����N�^�[�̈ړ��␳�W��

//���(���[�h)�̎��
typedef enum
{
	MODE_TITLE = 0,	//�^�C�g�����
	MODE_TUTORIAL,	//�`���[�g���A�����
	MODE_GAME,		//�Q�[�����
	MODE_RESULT,	//���U���g���
	MODE_RANKING,	//�����L���O���
	MODE_LOGO,		//�`�[�����S���
	MODE_MAX
} MODE;

typedef enum
{
	DRAWTYPE_MAIN = 0,
	DRAWTYPE_MAP,
	DRAWTYPE_UI,
	DRAWTYPE_MAX
}DRAWTYPE;

//���_���[2D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	float rhw;				//���W�ϊ��p�W��(1.0f�ŌŒ�)
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
} VERTEX_2D;

//���_���[3D]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
} VERTEX_3D;

//���_���[3D_MULTI]�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;		//���_���W
	D3DXVECTOR3 nor;		//�@���x�N�g��
	D3DCOLOR col;			//���_�J���[
	D3DXVECTOR2 tex;		//�e�N�X�`�����W
	D3DXVECTOR2 texM;		//�}���`�e�N�X�`�����W
} VERTEX_3D_MULTI;

//���f���̍\���̂��`
typedef struct
{

	D3DXVECTOR3 pos;		//���݂̈ʒu
	D3DXVECTOR3 posOld;		//�O��̈ʒu
	D3DXVECTOR3 posOrigin;	//���̈ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 rotOrigin;	//���̌���
	D3DXVECTOR3 move;		//�ړ���
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	D3DXMATRIX OldmtxWorld;	//���[���h�}�g���b�N�X

	int nIdxShadow;			//�e�̃C���f�b�N�X�ԍ�
	int nIdxRotate;			//���邮��̃C���f�b�N�X�ԍ�
	int nIdxHypGauge;		//�̗̓Q�[�W�̃C���f�b�N�X�ԍ�
	int nIdxSerif;			//�Z���t�̃C���f�b�N�X�ԍ�
	int nShadow;			//�e���g�����ǂ���
	int nFever;				//�e���g�����ǂ���

	int nNumVtx;			//���_��
	DWORD dwSizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y
	BYTE *pVtxBuff;			//���_�o�b�t�@�̃|�C���^
	D3DXVECTOR3 vtxMin;		//���f���̍ŏ��l
	D3DXVECTOR3	vtxMax;		//���f���̍ő�l

	int nType;			//�ǂݍ��ރ��f���̎��
	int nParent;		//�e�̔ԍ�
	bool bUse;			//�g�p���Ă��邩
	int nState;			//���
	

	LPD3DXMESH pMesh;			//���b�V��(���_���)�ւ̃|�C���^
	LPD3DXBUFFER pBuffMat;		//�}�e���A���ւ̃|�C���^
	D3DXMATERIAL pMatData[MAX_MAT];	//�}�e���A���̃f�[�^
	DWORD dwNumMat;			//�}�e���A���̐�
	LPDIRECT3DTEXTURE9 pTexture[MAX_TX];	//�e�N�X�`���ւ̃|�C���^
	char acFilename[512];	//�t�@�C����
}Model;

//2DUI�p�̍\���̂��`
typedef struct
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXCOLOR col;		//���_�J���[
	float fWidth;		//��
	float fHeight;		//����
	int nPatternAnim;	//�A�j���[�V�����p�^�[��No
	int nCntAnim;		//�A�j���[�V�����J�E���^�[
	D3DXVECTOR3 rot;	//����
	float fAngle;		//�p�x
	float fLength;		//�Ίp���̒���
	int nCount;			//�G�J�E���g
	bool bUse;			//�g�p���Ă��邩
	float aTexU;		//�e�N�X�`�����WU[�X�N���[���p]
	float aTexV;		//�e�N�X�`�����WV[�X�N���[���p]

} UI_2D;

//�v���g�^�C�v�錾
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);
LPD3DXFONT GetFont(void);
int GetFPS(void);

#endif