//==============================================================
//
//DirectX[resultscore.h]
//Author:�����V����
//
//==============================================================
#ifndef _RESULTSCORE_H_
#define _RESULTSCORE_H_

#include"main.h"

//���U���g�X�R�A�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ�
	D3DXCOLOR col;			//�J���[

	float fHeight;		//����
	float fWidth;		//��

	bool bUse;			//�g�p����Ă��邩�ǂ���
}ResultScore;

//�v���g�^�C�v�錾
void InitResultScore(void);
void UninitResultScore(void);
void UpdateResultScore(void);
void DrawResultScore(void);
void SetResultScore(int nScore);
void AddResultScore(int nValue);
int GetResultScore(void);

#endif
