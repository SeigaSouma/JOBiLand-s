//==============================================================
//
//HGS2022_Springs[main.cpp]
//Author:相馬靜雅
//
//==============================================================
#include "main.h"
#include "input.h"
#include "debugproc.h"
#include "camera.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "logo.h"
#include "fade.h"
#include "ranking.h"
#include "titleselect.h"
#include "light.h"
#include "model.h"
#include "calculation.h"
#include "sound.h"
#include "nofade.h"
#include "resource.h"

//メモリリーク出力用
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//マクロ定義
#define CLASS_NAME			"Window Class"			//ウインドウクラスの名前
#define WINDOW_NAME			"無題"			//ウインドウの名前

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawObject(int nType);

//グローバル変数
LPDIRECT3D9 g_pD3D = NULL;						//Direct3Dオブジェクトへのポインタ
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			//Direct3Dデバイスへのポインタ
int g_nCountFPS = 0;							//FPSカウンタ
MODE g_mode = MODE_GAME;						//現在のモード

//==============================================================
//メイン関数
//==============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrey, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;				//ウインドウハンドル(識別子)
	MSG msg;				//メッセージを格納する変数
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウのスタイル
		0,									//0にする（通常は使用しない）
		0,									//0にする（通常は使用しない）
		hInstance,							//インスタンスハンドル
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			//マウスカーソル
		HBRUSH(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),		//ファイルのアイコン
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,		//拡張ウインドウスタイル
		CLASS_NAME,					//ウインドウクラスの名前
		WINDOW_NAME,				//ウインドウの名前
		WS_OVERLAPPEDWINDOW,		//ウインドウスタイル
		CW_USEDEFAULT,				//ウインドウの左上X座標
		CW_USEDEFAULT,				//ウインドウの左上Y座標
		(rect.right - rect.left),	//ウインドウの幅
		(rect.bottom - rect.top),	//ウインドウの幅
		NULL,						//親ウインドウのハンドル
		NULL,						//メニューハンドルまたは子ウインドウID
		hInstance,					//インスタンスハンドル
		NULL);

	//初期化処理
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//初期化処理が失敗した場合
		return -1;
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得（保存）
	dwFrameCount = 0;					//初期化する
	dwFPSLastTime = timeGetTime();		//現在時刻を取得（保存）

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);			//ウインドウの表示状態を設定
	UpdateWindow(hWnd);					//クライアント領域を更新

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理
			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);		//仮想キーメッセージを文字メッセージへ返還
				DispatchMessage(&msg);		//ウインドウプロしーじゃへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			dwCurrentTime = timeGetTime();	//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過
				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPSを測定した時刻を保存
				dwFrameCount = 0;					//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過
				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存
				dwFrameCount++;						//フレームカウントを加算

				//更新処理
				Update();

				//描画処理
				Draw();
			}
		}
	}

	//終了処理
	Uninit();

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	//メモリリークを出力
	_CrtDumpMemoryLeaks();

	return(int)msg.wParam;
}

//==============================================================
//ウインドウプロシージャ
//==============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;												//返り値を格納
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//ウインドウの領域（四角）

	switch (uMsg)
	{
	case WM_DESTROY:			//ウインドウ破棄のメッセージ
								//WM_QUITメッセージを返す
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:			//キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE:			//[ESC]キーが押された
			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
			if (nID == IDYES)
			{
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:				//閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);
		if (nID == IDYES)
		{
			//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0を返さないと終了してしまう
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//既定の処理を返す
}

//==============================================================
//初期化処理
//==============================================================
HRESULT Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							//パラメーターのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;						//バックバッファの形式
	d3dpp.BackBufferCount = 1;									//バクバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;						//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//デプスバッファとしてbit16を使う
	d3dpp.Windowed = bWindow;									//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの生成
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		//テクスチャ繰り返しの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//マルチテクスチャ
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	////霧のブレンドを有効にする
	//g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	////色を設定
	//g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(0.9f, 0.9f, 1.0f, 1.0f));

	//float Start = 7000.0f;//線形霧の開始位置
	//float End = 8000.0f;//線形霧の終了位置
	//					
	//g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	//g_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	//g_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));

	//g_pD3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

	//乱数の種を設定
	srand((unsigned int)time(0));

	//入力機器の初期化処理
	if (FAILED(InitInput(hinstance, hWnd)))
	{
		return E_FAIL;
	}

	//ランキングのリセット
	//ResetRanking();

	//サウンドの初期化
	InitSound(hWnd);

	//デバッグプロックの初期化処理
	InitDebugProc();

	//カメラの初期化処理
	InitCamera();

	//ライトの初期化処理
	InitLight();

	//計算の初期化処理
	InitCalculastion();

	//フェードの設定
	InitFade(g_mode);

	//フェードしないフェードの設定
	InitNoFade();

	return S_OK;
}

//==============================================================
//終了処理
//==============================================================
void Uninit(void)
{
	//タイトル画面の終了処理
	UninitTitle();

	//チュートリアル画面の終了処理
	UninitTutorial();

	//ゲーム画面の終了処理
	UninitGame();

	//リザルト画面の終了処理
	UninitResult();

	//ランキング画面の終了処理
	UninitRanking();

	//サウンドの終了処理
	UninitSound();

	//フェードの終了処理
	UninitFade();

	//入力機器の終了処理
	UninitInput();

	//デバッグプロックの終了処理
	UninitDebugProc();

	//ライトの終了処理
	UnInitLight();

	//カメラの終了処理
	UninitCamera();

	//フェードしないフェードの終了処理
	UninitNoFade();

	//Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//==============================================================
//更新処理
//==============================================================
void Update(void)
{
	//カメラの情報取得
	Camera *pCamera = GetCamera();

	//入力機器の更新処理
	UpdateInput();

	//デバッグプロックの更新処理
	UpdateDebugProc();
	
	if (GetPause() == false)
	{//ポーズ中じゃなければ

		//カメラの更新処理
		UpdateCamera();

		if (pCamera->nState != CAMERASTATE_MANYSPAWN)
		{//大量発生のカメラじゃなかったら

			//ライトの更新処理
			UpdateLight();
		}
	}

	switch (g_mode)
	{
	case MODE_LOGO:		//ロゴ画面
		UpdateLogo();
		break;

	case MODE_TITLE:		//タイトル画面
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UpdateTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		UpdateGame();
		break;

	case MODE_RESULT:		//リザルト画面
		UpdateResult();
		break;

	case MODE_RANKING:		//ランキング画面
		UpdateRanking();
		break;
	}

	//フェードの更新処理
	UpdateFade();

	//フェードしないフェードの更新処理
	UpdateNoFade();

	//終了のフラグが真なら
	if (GetEndFrag() == true)
	{
		//ウインドウを破棄する(VK_DESTROYメッセージを送る)
		DestroyWindow(NULL);

		//WM_QUITメッセージを送る
		PostQuitMessage(0);
	}

}

//==============================================================
//描画処理
//==============================================================
void Draw(void)
{
	//画面クリア（バックバッファとZバッファのクリア）
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//通常
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		D3DVIEWPORT9 viewportDef;	//ビューポート保存

		//現在のビューポートを取得
		g_pD3DDevice->GetViewport(&viewportDef);

		//カメラの設定処理
		SetCamera(CAMERATYPE_MAIN);

		//各種オブジェクトの描画
		DrawObject(DRAWTYPE_MAIN);

		//カメラの設定処理
		SetCamera(CAMERATYPE_UI);

		//各種オブジェクトの描画
		DrawObject(DRAWTYPE_UI);

		//ビューポートを元に戻す
		g_pD3DDevice->SetViewport(&viewportDef);

		//描画終了
		g_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================================
//各種オブジェクトの描画
//==============================================================
void DrawObject(int nType)
{

	switch (g_mode)
	{
	case MODE_LOGO:		//ロゴ画面
		DrawLogo(nType);
		break;

	case MODE_TITLE:		//タイトル画面
		DrawTitle(nType);
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		DrawTutorial(nType);
		break;

	case MODE_GAME:			//ゲーム画面
		DrawGame(nType);
		break;

	case MODE_RESULT:		//リザルト画面
		DrawResult(nType);
		break;

	case MODE_RANKING:		//ランキング画面
		DrawRanking(nType);
		break;
	}

	if (nType == DRAWTYPE_UI)
	{
		//フェードの描画処理
		DrawFade();

		//フェードしないフェードの描画処理
		DrawNoFade();

		//デバッグプロックの描画処理
		DrawDebugProc();
	}
}

//==============================================================
//モード設定
//==============================================================
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	switch (g_mode)
	{
	case MODE_LOGO:		//ロゴ画面
		UninitLogo();
		break;

	case MODE_TITLE:		//タイトル画面
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		UninitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		UninitGame();
		break;

	case MODE_RESULT:		//リザルト画面
		UninitResult();
		break;

	case MODE_RANKING:		//ランキング画面
		UninitRanking();
		break;
	}

	int nOldMode = g_mode;
	g_mode = mode;			//モードを設定する

	//新しい画面(モード)の初期化処理
	switch (mode)
	{
	case MODE_LOGO:		//ロゴ画面
		InitLogo();
		break;

	case MODE_TITLE:		//タイトル画面
		InitTitle();
		break;

	case MODE_TUTORIAL:		//チュートリアル画面
		InitTutorial();
		break;

	case MODE_GAME:			//ゲーム画面
		InitGame();
		break;

	case MODE_RESULT:		//リザルト画面
		InitResult();
		break;

	case MODE_RANKING:		//ランキング画面
		InitRanking(nOldMode);
		break;
	}

	g_mode = mode;			//モードを設定する
}

//==============================================================
//モードの取得
//==============================================================
MODE GetMode(void)
{
	return g_mode;
}

//==============================================================
//デバイスの取得
//==============================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==============================================================
//FPSの取得
//==============================================================
int GetFPS(void)
{
	return g_nCountFPS;
}
