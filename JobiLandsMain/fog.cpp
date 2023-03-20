//<====================================================
//
//フォグ[霧]処理(fog.cpp)
//Author kazuki watanabe
//
//<====================================================
#include "fog.h"
#include "Input.h"

//マクロ定義
#define MAX_START (550.0f)//線形霧の開始位置の最大値
#define MAX_END (2500.0f)//線形霧の終了位置の最大値

//プロトタイプ宣言
void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density);
void SetupPixelFog(DWORD Color, DWORD Mode, BOOL UseRange);
void ChangeFogCol(void);

Fog g_Fog;//霧情報

//<===========================
//霧の初期化処理
//<===========================
void InitFog(void)
{
	g_Fog.FogCol = D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f);//霧の色
	g_Fog.FogType = D3DFOG_LINEAR;//霧の種類
}

//<===========================
//霧の終了処理
//<===========================
void UninitFog(void)
{

}

//<===========================
//霧の更新処理
//<===========================
void UpdateFog(void)
{
	SetupPixelFog(g_Fog.FogCol, g_Fog.FogType, 1);//ピクセルフォグの設定
#ifdef _DEBUG
	ChangeFogCol();//霧の色変更
#endif
}

//<===========================================
//頂点フォグの設定
//<===========================================
void SetupVertexFog(DWORD Color, DWORD Mode, BOOL UseRange, FLOAT Density)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	float Start = MAX_START;//線形霧の開始位置
	float End = MAX_END;//線形霧の終了位置

	//霧のブレンドを有効にする
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//色を設定
	pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);

	//霧のパラメーターを設定
	//もし線形霧モードだったら
	if (Mode == D3DFOG_LINEAR)
	{
		pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
	}
	//そのほか
	else
	{
		pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, Mode);
		pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));

		//霧密度は0.0fから1.0fの範囲で設定する
	}
	//頂点フォグだけでサポートされている場合はその範囲のフォグを有効にする
	//このUseRangeが0以外の場合、
	if (UseRange != 0)
	{
		pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
	}

}

//<===========================================
//ピクセルフォグの設定
//<===========================================
void SetupPixelFog(DWORD Color, DWORD Mode, BOOL UseRange)
{
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	//<========================================
	//線形モード用の変数
	float Start = MAX_START;//線形霧の開始位置
	float End = MAX_END;//線形霧の終了位置
	//
	//<========================================

	float Density = 0.0005f;//霧密度(0.0fから1.0fの範囲)

#ifdef _DEBUG
	//デバッグ用の霧密度変更
	if (GetKeyboardPress(DIK_F4) == true)
	{
		Density = 0.0015f;
	}
#endif
	//霧のブレンドを有効にする
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);

	//色を設定
	pDevice->SetRenderState(D3DRS_FOGCOLOR, Color);

	//もし線形霧モードだったら
	if (Mode == D3DFOG_LINEAR)
	{//線形霧のパラメータを設定する

		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
		pDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
		pDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));
	}
	//そのほか
	else
	{
		pDevice->SetRenderState(D3DRS_FOGTABLEMODE, Mode);
		pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD *)(&Density));
	}

	//フォグだけでサポートされている場合はその範囲のフォグを有効にする
	//このUseRangeが0以外の場合、
	if (UseRange != 0)
	{
		pDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);
	}
}

//<===========================================
//フォグの色変更処理
//<===========================================
void ChangeFogCol(void)
{
	//赤色
	if (GetKeyboardPress(DIK_R) == true)
	{
		//赤色の値を加算していく
		g_Fog.FogCol.r += 0.01f;
	}
	//緑色
	if (GetKeyboardPress(DIK_G) == true)
	{
		//緑色の値を加算していく
		g_Fog.FogCol.g += 0.01f;
	}
	//青色
	if (GetKeyboardPress(DIK_B) == true)
	{
		//青色の値を加算していく
		g_Fog.FogCol.b += 0.01f;
	}

	//<=========================================
	//補正処理
	//<=========================================
	//赤色が1.01を超えそうになったら
	if (g_Fog.FogCol.r > 1.01f)
	{
		//0にする
		g_Fog.FogCol.r = 0.0f;
	}
	//緑色が1.01を超えそうになったら
	if (g_Fog.FogCol.g > 1.01f)
	{
		//0にする
		g_Fog.FogCol.g = 0.0f;
	}
	//青色が1.01を超えそうになったら
	if (g_Fog.FogCol.b > 1.01f)
	{
		//0にする
		g_Fog.FogCol.b = 0.0f;
	}
	//
	//<=========================================
}