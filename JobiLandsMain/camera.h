//=============================================================================
//
// �J�������� [camera.h]
// Author : ���n�Ή�
//
//=============================================================================

#ifndef _CAMERA_H_		//���̃}�N����`������Ă��Ȃ�������
#define _CAMERA_H_		//��d�C���N���[�h�h�~�̃}�N�����`����

//�}�N����`
#define START_CAMERALEN	(590.0f)	//���̋���

//�񋓌^
typedef enum
{
	CAMERASTATE_NONE = 0,
	CAMERASTATE_SHAKE,	//�U��
	CAMERASTATE_MANYSPAWN,	//��ʔ���
}CAMERASTATE;

typedef enum
{
	CAMERATYPE_MAIN = 0,	//���C���J����
	CAMERATYPE_SIGN,		//���J����
	CAMERATYPE_SELECT,		//�I�����J����
	CAMERATYPE_MAP,         //�}�b�v�J����
	CAMERATYPE_UI,         //UI�p�J����
	CAMERATYPE_MAX
}CAMERATYPE;

typedef enum
{
	CAMERAMOVE_SET = 0,
	CAMERAMOVE_UP,	//�㏸
	CAMERAMOVE_MOVE,	//�ړ�
	CAMERAMOVE_DOWN,	//���~
	CAMERAMOVE_RETURN,	//�߂�
}CAMERAMOVE;

typedef enum
{
	CAMERAMOVE_TITLE_UP = 0,
	CAMERAMOVE_TITLE_DOWN,
	CAMERAMOVE_TITLE_MAX,
}CAMERAMOVE_TITLE;

//�J�����̍\����
typedef struct
{
	D3DXVECTOR3 posRDest;	//�ڕW�̒����_
	D3DXVECTOR3 rotVDest;	//�ڕW�̎��_�̌���
	float rotVDiff;			//�ڕW�̎��_�̍���
	float fBeforDistance;	//���̋���
	D3DXVECTOR3 BeforRotV;	//���̌���
	D3DXVECTOR3 BeforPosR;	//���̒����_
	D3DXVECTOR3 BeforPosV;	//���̎��_

}ManySpawnCamera;

typedef struct
{
	D3DXVECTOR3 posV;		//���_(�J�����̈ʒu)
	D3DXVECTOR3 posR;		//�����_(�������ꏊ)
	D3DXVECTOR3 posVDest;	//�ڕW�̎��_
	D3DXVECTOR3 posRDest;	//�ڕW�̒����_
	D3DXVECTOR3 vecU;		//������x�N�g��
	D3DXMATRIX	mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX	mtxView;	//�r���[�}�g���b�N�X
	D3DVIEWPORT9 viewport;		//�r���[�|�[�g
	D3DXVECTOR3 move;		//�ړ���
	D3DXVECTOR3 rot;		//����
	D3DXVECTOR3 rotVDest;	//�ڕW�̎��_�̌���
	int nState;				//���
	int nMoveStep;			//�i�K
	int nCntStep;			//�i�K�̃J�E���g
	float rotVDiff;			//�ڕW�̎��_�̍���
	float fDistance;		//����
	int nCntBackFollow;		//�w�ʕ␳����܂ł̎���
	bool bBackFollow;		//�w�ʒǏ]���邩�ǂ���
	bool bFollow;			//�Ǐ]���邩�ǂ���
	bool bManySE;			//SE�炵����
	bool bSPush;            //S�n��������Ă��邩�ǂ���

	ManySpawnCamera aMany;	//��ʔ���
}Camera;

//�v���g�^�C�v�錾
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void SetCamera(int nIdx);
Camera *GetCamera(void);

void InitGameCamera(void);
void InitTitleCamera(void);
void InitRankingCamera(void);
void InitResultCamera(void);
void UpdateGameCamera(int nCntCamera);
void UpdateTutorialCamera(int nCntCamera);
void UpdateTitleCamera(int nCntCamera);
//void ShakeCamera(void);

#endif