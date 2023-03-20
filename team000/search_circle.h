//=============================================================================
//
// �T���͈͏��� [search_circle.h]
// Author : ���n�Ή�
//
//=============================================================================
#ifndef _SEARCH_CIRCLE_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SEARCH_CIRCLE_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�}�N����`
#define MAX_SEARCH_CIRCLE	(256)

//�񋓌^��`
typedef enum
{
	SEARCH_CIRCLE_TYPE_BLACK = 0,
	SEARCH_CIRCLE_TYPE_BLACK2,
	SEARCH_CIRCLE_TYPE_ORANGE,
	SEARCH_CIRCLE_TYPE_ORANGE2,
	SEARCH_CIRCLE_TYPE_BLUE,
	SEARCH_CIRCLE_TYPE_MAX
}SEARCH_CIRCLE_TYPE;

//���b�V���t�B�[���h�\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 nor[MAX_SEARCH_CIRCLE];	//�@���̌���
	D3DXCOLOR col;			//�F
	D3DXCOLOR colOrigin;	//���̐F
	D3DXMATRIX mtxWorld;	//���[���h�}�g���b�N�X
	float fOutWidth;		//����(�O)
	float fInWidth;			//��(��)
	float fRotWidth;		//1������������̊p�x����
	float fHeight;
	float fMove;			//�L���鑬�x
	int nLife;				//����
	int nMaxLife;			//�ő����
	int nTexType;			//�e�N�X�`�����
	bool bUse;				//�g�p���Ă��邩
}SEARCH_CIRCLE;

//�v���g�^�C�v�錾
void InitSearchCircle(void);
void UninitSearchCircle(void);
void UpdateSearchCircle(void);
void DrawSearchCircle(void);
int SetSearchCircle(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXCOLOR col, float fWidth, float fHeight, int nLife, float fMove, int nTexType);
void SetPositionSearchCircle(int nIdxSearchRadius, D3DXVECTOR3 pos);

#endif