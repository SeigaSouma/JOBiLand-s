//=============================================================================
//
// エディット処理 [edit.h]
// Author : 相馬靜雅
//
//=============================================================================
#ifndef _EDIT_H_		//このマクロ定義がされていなかったら
#define _EDIT_H_		//二重インクルード防止のマクロを定義する

#include "main.h"
#include "model.h"

//エディットの構造体
typedef struct
{
	D3DXVECTOR3 pos;	//現在の位置
	D3DXVECTOR3 rot;	//向き
	int nNowModelType;	//今のモデル種類
	D3DXMATRIX mtxWorld;	//ワールドマトリックス
	int nShadow;		//影使うかどうか[0か1]
	bool bUse;			//使用しているか

	Model aXLoadMeshData[MAX_MODEL];	//Xファイルで読み込んだデータ
}Edit;

//プロトタイプ宣言
void InitEdit(void);
void UninitEdit(void);
void UpdateEdit(void);
void DrawEdit(void);
Edit *GetEdit(void);

#endif