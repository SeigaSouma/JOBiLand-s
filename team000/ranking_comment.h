//=============================================================================
//
// �ŏ��̉�b���� [ranking_comment.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _RANKING_COMMENT_H_		//���̃}�N����`������Ă��Ȃ�������
#define _RANKING_COMMENT_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"
#include "comment.h"


//�\���̂̒�`
typedef struct
{
	int nNumSpawn;	//�o��������
	CommnetInfo aInfo[MAX_NICOCOME];
}RankingComment;

//�v���g�^�C�v�錾
void InitRankingComment(void);
void UninitRankingComment(void);
void UpdateRankingComment(void);
void DrawRankingComment(void);
void SetRankingComment(int nTexV, int nType);
void SetTartgetWindow(void);
RankingComment *GetRankingComment(void);

#endif