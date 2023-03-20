//=============================================================================
//
// �Ö��Q�[�W���� [hypnosis_gauge.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _HYPNISISGAUGE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _HYPNISISGAUGE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_HYPNOSISGAUGE	(256)	//�Ö��Q�[�W�̍ő吔

//�񋓌^��`
typedef enum
{
	HYPNISISGAUGE_VTX_BLACK = 0,	//���Q�[�W
	HYPNISISGAUGE_VTX_PINK,		//�s���N�Q�[�W
	HYPNISISGAUGE_VTX_FRAM,		//�g
	HYPNISISGAUGE_VTX_MAX
}HYPNISISGAUGE_VTX;


//�Ö��x�Q�[�W�\����
typedef struct
{
	D3DXCOLOR col;		//�F
	float fWidth;		//����
	float fMaxWidth;	//���ő啝
	float fHeight;		//�c��
	float fWidthDest;	//�ڕW�̕�
	float fWidthDiff;	//���̍���

}HypnosisGauge_Frame;

typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	bool bUse;				//�g�p���Ă��邩
	bool bDisp;				//�`�悷�邩
	int nMaxHyp;			//�ő�l
	int nHypnosis;			//���ݒn

	HypnosisGauge_Frame aFrame[HYPNISISGAUGE_VTX_MAX];	//�g
}HypnosisGauge;

//�v���g�^�C�v�錾
void InitHypnosisGauge(void);
void UninitHypnosisGauge(void);
void UpdateHypnosisGauge(void);
void DrawHypnosisGauge(void);
int SetHypnosisGauge(float fWidth, float fHeight, int nLife);
void SetPositionHypnosisGauge(int nIdxGauge, D3DXVECTOR3 pos, int nMaxLife);
HypnosisGauge *GetHypnosisGauge(void);
void GetHypnosisUse(int nIdxGauge, bool bUse);

#endif