//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 相馬靜雅
//
//=============================================================================

#ifndef _CAMERA_H_		//このマクロ定義がされていなかったら
#define _CAMERA_H_		//二重インクルード防止のマクロを定義する

//マクロ定義
#define START_CAMERALEN	(590.0f)	//元の距離

//列挙型
typedef enum
{
	CAMERASTATE_NONE = 0,
	CAMERASTATE_SHAKE,	//振動
	CAMERASTATE_MANYSPAWN,	//大量発生
}CAMERASTATE;

typedef enum
{
	CAMERATYPE_MAIN = 0,	//メインカメラ
	CAMERATYPE_SIGN,		//矢印カメラ
	CAMERATYPE_SELECT,		//選択肢カメラ
	CAMERATYPE_MAP,         //マップカメラ
	CAMERATYPE_UI,         //UI用カメラ
	CAMERATYPE_MAX
}CAMERATYPE;

typedef enum
{
	CAMERAMOVE_SET = 0,
	CAMERAMOVE_UP,	//上昇
	CAMERAMOVE_MOVE,	//移動
	CAMERAMOVE_DOWN,	//下降
	CAMERAMOVE_RETURN,	//戻る
}CAMERAMOVE;

typedef enum
{
	CAMERAMOVE_TITLE_UP = 0,
	CAMERAMOVE_TITLE_DOWN,
	CAMERAMOVE_TITLE_MAX,
}CAMERAMOVE_TITLE;

//カメラの構造体
typedef struct
{
	D3DXVECTOR3 posRDest;	//目標の注視点
	D3DXVECTOR3 rotVDest;	//目標の視点の向き
	float rotVDiff;			//目標の視点の差分
	float fBeforDistance;	//元の距離
	D3DXVECTOR3 BeforRotV;	//元の向き
	D3DXVECTOR3 BeforPosR;	//元の注視点
	D3DXVECTOR3 BeforPosV;	//元の視点

}ManySpawnCamera;

typedef struct
{
	D3DXVECTOR3 posV;		//視点(カメラの位置)
	D3DXVECTOR3 posR;		//注視点(見たい場所)
	D3DXVECTOR3 posVDest;	//目標の視点
	D3DXVECTOR3 posRDest;	//目標の注視点
	D3DXVECTOR3 vecU;		//上方向ベクトル
	D3DXMATRIX	mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX	mtxView;	//ビューマトリックス
	D3DVIEWPORT9 viewport;		//ビューポート
	D3DXVECTOR3 move;		//移動量
	D3DXVECTOR3 rot;		//向き
	D3DXVECTOR3 rotVDest;	//目標の視点の向き
	int nState;				//状態
	int nMoveStep;			//段階
	int nCntStep;			//段階のカウント
	float rotVDiff;			//目標の視点の差分
	float fDistance;		//距離
	int nCntBackFollow;		//背面補正するまでの時間
	bool bBackFollow;		//背面追従するかどうか
	bool bFollow;			//追従するかどうか
	bool bManySE;			//SE鳴らしたか
	bool bSPush;            //S系が押されているかどうか

	ManySpawnCamera aMany;	//大量発生
}Camera;

//プロトタイプ宣言
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