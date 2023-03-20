//==============================================================
//
//DirectX[ranking.h]
//Author:����������
//
//==============================================================
#ifndef _RANKING_H_				//���̃}�N����`������Ă��Ȃ�������
#define _RANKING_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//����1��1��
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
}EachRankingNumber;

//�����L���O1��1��
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 posDest;	//�ڕW�̈ʒu
	int nScore;		//���݂̃X�R�A
	int nDigit;		//����
	float fWidth;
	float fHeight;
	bool bUse;		//�g�p���Ă��邩

	EachRankingNumber aScoreNum[4];
}EachRanking;

typedef struct
{
	int nNumRanking;	//�����L���O�̐�
	int nScore[512];	//�S�Ẵ����L���O�f�[�^
	int nChangeTime;	//�����J�ڂ̃J�E���^�[
	int nCntSetCounter;	//�Z�b�g����܂ł̃J�E���^�[
	int nCntUse;		//�g�p�J�E���g
	int nThisScore;		//����̃X�R�A
	int nNewRecordNum;	//�j���[���R�[�h�̔ԍ�
	bool bNewRecord;	//�j���[���R�[�h�̔���
	int nOldMode;		//�J�ڑO�̃��[�h

	EachRanking aEachRealRanking[5];	//���ꂼ��̃X�R�A
} Ranking;


typedef struct
{
	UI_2D aUI;	//UI���
} RankingText;

//�v���g�^�C�v�錾
void InitRanking(int nOldMode);
void UninitRanking(void);
void UpdateRanking(void);
void DrawRanking(int nType);
void SetRanking(int nScore);
void SetRankingPos(void);
Ranking *GetRanking(void);

#endif