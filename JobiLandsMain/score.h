//=============================================================================
//
// ���A���^�C�������L���O���� [realtime_ranking.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _SCORE_H_				//���̃}�N����`������Ă��Ȃ�������
#define _SCORE_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�����L���O�\����
//����1��1��
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
}EachScore;

typedef struct
{
	int nScore;		//���݂̃X�R�A
	int nDigit;		//����
	D3DXVECTOR3 pos;	//�ʒu
	float fWidth;
	float fHeight;

	EachScore aScoreNum[4];
} Score;

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);

#endif