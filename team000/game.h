//==============================================================
//
//�Q�[������[game.h]
//Author:����������
//
//==============================================================
#ifndef _GAME_H_				//���̃}�N����`������Ă��Ȃ�������
#define _GAME_H_				//2�d�C���N���[�h�h�~�̃}�N�����`����

#include"main.h"

//�}�N����`
#define GAMETIME	(60 * 240)	//�Q�[������
#define TARGETHITO  (500)       //�ڕW�̐l��

//�Q�[���̏��
typedef enum
{
	GAMESTATE_NONE = 0,		//�������Ă��Ȃ����
	GAMESTATE_BEFOREEND,	//�I���O���
	GAMESTATE_AFTEREND,		//
	GAMESTATE_END,			//�I�����
	GAMESTATE_EDIT,			//�G�f�B�b�g���[�h
	GAMESTATE_START,		//�o�����
	GAMESTATE_MAX
} GAMESTATE;

//�Q�[���̍\����
typedef struct
{
	int nCntPeople;    //�ڕW�̐l��
} TARGETNOP;

//�v���g�^�C�v�錾
void InitGame(void);
void UninitGame(void);
void UpdateGame(void);
void DrawGame(int nType);
void SetGameState(GAMESTATE state, int nCounter);
GAMESTATE GetGameState(void);
void SetEnablePause(bool bPouse);
TARGETNOP *GetnTargetNop(void);
bool GetPause(void);
void SetGameEnd(void);

#endif
