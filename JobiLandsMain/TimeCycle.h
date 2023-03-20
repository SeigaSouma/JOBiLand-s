//<====================================================
//
//�^�C���T�C�N������(TimeCycle.h)
//Author kazuki watanabe
//
//<====================================================
#ifndef  _TIMECYCLE_H_
#define _TIMECYCLE_H_

#include "main.h"

//�^�C���T�C�N���̍\����
typedef struct
{
	D3DXVECTOR3 pos;	//�ʒu
	D3DXVECTOR3 rot;	//����
	float fAngle;		//�p�x
	float fLength;		//�Ίp���̒���

}TIMECYCLE;

//�v���g�^�C�v�錾
void InitTimeCycle(void);
void UninitTimeCycle(void);
void UpdateTimeCycle(void);
void DrawTimeCycle(void);
#endif

