//<====================================================
//
//フォグ[霧]処理(fog.h)
//Author kazuki watanabe
//
//<====================================================
#ifndef _FOG_H_		//このマクロ定義がされていなかったら
#define _FOG_H_		//二重インクルード防止のマクロを定義する

#include "main.h"

//霧情報の構造体
typedef struct
{
	D3DXCOLOR FogCol;//霧の色
	D3DFOGMODE FogType;//霧の種類

}Fog;

//プロトタイプ宣言
void InitFog(void);
void UninitFog(void);
void UpdateFog(void);//必ずこの順番で書く。(これは重要)

#endif
