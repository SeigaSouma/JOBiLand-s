//=============================================================================
//
// ���A���^�C�������L���O���� [realtime_ranking.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _REALTIME_RANKING_H_				//���̃}�N����`������Ă��Ȃ�������
#define _REALTIME_RANKING_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�����L���O�\����

typedef enum
{
	REALRANKING_TYPE_RANKNUM = 0,	//���݂̏���
	REALRANKING_TYPE_RANKING,	//���̃����L���O�X�R�A
	REALRANKING_TYPE_MAX
}REALRANKING_TYPE;

typedef enum
{
	REALRANKINGTEXT_VTX_FADE = 0,	//���n
	REALRANKINGTEXT_VTX_NEXTNUM,	//���̏��ʂ̐l��
	REALRANKINGTEXT_VTX_NOW,		//���݂̏���&���݂̐l��
	REALRANKINGTEXT_VTX_TOP,		//�L�^�X�V��
	REALRANKINGTEXT_VTX_GIZA,		//�M�U�M�U
	REALRANKINGTEXT_VTX_MAX
}REALRANKINGTEXT_VTX;

//����1��1��
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
}EachScoreNumber;

//�����L���O1��1��
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	int nScore;		//���݂̃X�R�A
	int nDigit;		//����
	float fWidth;
	float fHeight;

	EachScoreNumber aScoreNum[4];
}EachRealRanking;

typedef struct
{
	int nNumRanking;	//�����L���O�̐�
	int nScore[512];	//�S�Ẵ����L���O�f�[�^
	int nNowRank;		//���݂̏���
	int nNowScore;		//���݂̃X�R�A
	bool bTop;			//1�ʂ��ǂ���

	EachRealRanking aEachRealRanking[REALRANKING_TYPE_MAX];	//���ꂼ��̃X�R�A
} RealTimeRanking;


typedef struct
{
	UI_2D aUI;	//UI���
} RealTimeText;

//�v���g�^�C�v�錾
void InitRealTimeRanking(void);
void UninitRealTimeRanking(void);
void UpdateRealTimeRanking(void);
void DrawRealTimeRanking(void);

#endif