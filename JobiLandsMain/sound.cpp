//=============================================================================
//
// サウンド処理 [sound.cpp]
// Author : 相馬靜雅
//
//=============================================================================
#include "sound.h"

//マクロ定義
#define START_VOLUME	(1.0f)		//音量補正の初期値
#define MAX_VOLUME		(1.0f)		//音量補正の最大値
#define MIN_VOLUME		(0.0f)		//音量補正の最小値

//*****************************************************************************
// サウンド情報の構造体定義
//*****************************************************************************
typedef struct
{
	char *pFilename;	// ファイル名
	int nCntLoop;		// ループカウント
} SOUNDINFO;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
IXAudio2 *g_pXAudio2 = NULL;								// XAudio2オブジェクトへのインターフェイス
IXAudio2MasteringVoice *g_pMasteringVoice = NULL;			// マスターボイス
IXAudio2SourceVoice *g_apSourceVoice[SOUND_LABEL_MAX] = {};	// ソースボイス
BYTE *g_apDataAudio[SOUND_LABEL_MAX] = {};					// オーディオデータ
DWORD g_aSizeAudio[SOUND_LABEL_MAX] = {};					// オーディオデータサイズ
//DSBUFFERDESC SetVolume;
//float g_fVolume;		//音量

// サウンドの情報
SOUNDINFO g_aSoundInfo[SOUND_LABEL_MAX] =
{
	{ "data/BGM/bgm004.wav", -1 },	// タイトル
	{ "data/BGM/clear.wav", -1 },	// ゲーム
	{ "data/BGM/bgm006.wav", -1 },	// リザルト
	{ "data/BGM/takumi.wav", -1 },	// ランキング
	{ "data/BGM/tutorial.wav", -1 },	// チュートリアル
	{ "data/SE/dhuhuhu.wav", 0 },	// カーソル移動
	{ "data/SE/arm_move.wav",0},	//腕の動く音
	{ "data/SE/arm_push.wav",0 },	//腕がものに当たる音
	{ "data/SE/enter.wav",0},		//エンター決定音
	{ "data/SE/wind.wav",0 },	//腕がものに当たる音音
};

//=============================================================================
// 初期化処理
//=============================================================================
HRESULT InitSound(HWND hWnd)
{
	HRESULT hr;

	// COMライブラリの初期化
	CoInitializeEx(NULL, COINIT_MULTITHREADED);

	// XAudio2オブジェクトの作成
	hr = XAudio2Create(&g_pXAudio2, 0);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "XAudio2オブジェクトの作成に失敗！", "警告！", MB_ICONWARNING);

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}
	
	// マスターボイスの生成
	hr = g_pXAudio2->CreateMasteringVoice(&g_pMasteringVoice);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マスターボイスの生成に失敗！", "警告！", MB_ICONWARNING);

		if(g_pXAudio2 != NULL)
		{
			// XAudio2オブジェクトの開放
			g_pXAudio2->Release();
			g_pXAudio2 = NULL;
		}

		// COMライブラリの終了処理
		CoUninitialize();

		return E_FAIL;
	}

	// サウンドデータの初期化
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		HANDLE hFile;
		DWORD dwChunkSize = 0;
		DWORD dwChunkPosition = 0;
		DWORD dwFiletype;
		WAVEFORMATEXTENSIBLE wfx;
		XAUDIO2_BUFFER buffer;

		// バッファのクリア
		memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

		// サウンドデータファイルの生成
		hFile = CreateFile(g_aSoundInfo[nCntSound].pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
		if(hFile == INVALID_HANDLE_VALUE)
		{
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(1)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
		if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
		{// ファイルポインタを先頭に移動
			MessageBox(hWnd, "サウンドデータファイルの生成に失敗！(2)", "警告！", MB_ICONWARNING);
			return HRESULT_FROM_WIN32(GetLastError());
		}
	
		// WAVEファイルのチェック
		hr = CheckChunk(hFile, 'FFIR', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &dwFiletype, sizeof(DWORD), dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		if(dwFiletype != 'EVAW')
		{
			MessageBox(hWnd, "WAVEファイルのチェックに失敗！(3)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// フォーマットチェック
		hr = CheckChunk(hFile, ' tmf', &dwChunkSize, &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		hr = ReadChunkData(hFile, &wfx, dwChunkSize, dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "フォーマットチェックに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// オーディオデータ読み込み
		hr = CheckChunk(hFile, 'atad', &g_aSizeAudio[nCntSound], &dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(1)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
		g_apDataAudio[nCntSound] = (BYTE*)malloc(g_aSizeAudio[nCntSound]);
		hr = ReadChunkData(hFile, g_apDataAudio[nCntSound], g_aSizeAudio[nCntSound], dwChunkPosition);
		if(FAILED(hr))
		{
			MessageBox(hWnd, "オーディオデータ読み込みに失敗！(2)", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}
	
		// ソースボイスの生成
		hr = g_pXAudio2->CreateSourceVoice(&g_apSourceVoice[nCntSound], &(wfx.Format));
		if(FAILED(hr))
		{
			MessageBox(hWnd, "ソースボイスの生成に失敗！", "警告！", MB_ICONWARNING);
			return S_FALSE;
		}

		// バッファの値設定
		memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
		buffer.AudioBytes = g_aSizeAudio[nCntSound];
		buffer.pAudioData = g_apDataAudio[nCntSound];
		buffer.Flags      = XAUDIO2_END_OF_STREAM;
		buffer.LoopCount  = g_aSoundInfo[nCntSound].nCntLoop;
		//SetVolume.dwFlags = DSBCAPS_CTRLVOLUME;		//音量調整のフラグ

		//g_fVolume = START_VOLUME;

		//音量をセットする
		//g_pMasteringVoice->SetVolume(START_VOLUME);

		// オーディオバッファの登録
		g_apSourceVoice[nCntSound]->SubmitSourceBuffer(&buffer);

		// ファイルをクローズ
		CloseHandle(hFile);

	}

	return S_OK;
}

//=============================================================================
// 終了処理
//=============================================================================
void UninitSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
	
			// ソースボイスの破棄
			g_apSourceVoice[nCntSound]->DestroyVoice();
			g_apSourceVoice[nCntSound] = NULL;
	
			// オーディオデータの開放
			free(g_apDataAudio[nCntSound]);
			g_apDataAudio[nCntSound] = NULL;
		}
	}
	
	// マスターボイスの破棄
	g_pMasteringVoice->DestroyVoice();
	g_pMasteringVoice = NULL;
	
	if(g_pXAudio2 != NULL)
	{
		// XAudio2オブジェクトの開放
		g_pXAudio2->Release();
		g_pXAudio2 = NULL;
	}
	
	// COMライブラリの終了処理
	CoUninitialize();
}

//=============================================================================
// セグメント再生(再生中なら停止)
//=============================================================================
HRESULT PlaySound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	// バッファの値設定
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = g_aSizeAudio[label];
	buffer.pAudioData = g_apDataAudio[label];
	buffer.Flags      = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount  = g_aSoundInfo[label].nCntLoop;
	//SetVolume.dwFlags = DSBCAPS_CTRLVOLUME;		//音量調整のフラグ

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);
	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}

	// オーディオバッファの登録
	g_apSourceVoice[label]->SubmitSourceBuffer(&buffer);

	// 再生
	g_apSourceVoice[label]->Start(0);

	return S_OK;
}

//=============================================================================
// セグメント停止(ラベル指定)
//=============================================================================
void StopSound(SOUND_LABEL label)
{
	XAUDIO2_VOICE_STATE xa2state;

	// 状態取得
	g_apSourceVoice[label]->GetState(&xa2state);

	if(xa2state.BuffersQueued != 0)
	{// 再生中
		// 一時停止
		g_apSourceVoice[label]->Stop(0);

		// オーディオバッファの削除
		g_apSourceVoice[label]->FlushSourceBuffers();
	}
}

//=============================================================================
// セグメント停止(全て)
//=============================================================================
void StopSound(void)
{
	// 一時停止
	for(int nCntSound = 0; nCntSound < SOUND_LABEL_MAX; nCntSound++)
	{
		if(g_apSourceVoice[nCntSound] != NULL)
		{
			// 一時停止
			g_apSourceVoice[nCntSound]->Stop(0);
		}
	}
}

//=============================================================================
// チャンクのチェック
//=============================================================================
HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD dwRead;
	DWORD dwChunkType;
	DWORD dwChunkDataSize;
	DWORD dwRIFFDataSize = 0;
	DWORD dwFileType;
	DWORD dwBytesRead = 0;
	DWORD dwOffset = 0;
	
	if(SetFilePointer(hFile, 0, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを先頭に移動
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	while(hr == S_OK)
	{
		if(ReadFile(hFile, &dwChunkType, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if(ReadFile(hFile, &dwChunkDataSize, sizeof(DWORD), &dwRead, NULL) == 0)
		{// チャンクデータの読み込み
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch(dwChunkType)
		{
		case 'FFIR':
			dwRIFFDataSize  = dwChunkDataSize;
			dwChunkDataSize = 4;
			if(ReadFile(hFile, &dwFileType, sizeof(DWORD), &dwRead, NULL) == 0)
			{// ファイルタイプの読み込み
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if(SetFilePointer(hFile, dwChunkDataSize, NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{// ファイルポインタをチャンクデータ分移動
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		dwOffset += sizeof(DWORD) * 2;
		if(dwChunkType == format)
		{
			*pChunkSize         = dwChunkDataSize;
			*pChunkDataPosition = dwOffset;

			return S_OK;
		}

		dwOffset += dwChunkDataSize;
		if(dwBytesRead >= dwRIFFDataSize)
		{
			return S_FALSE;
		}
	}
	
	return S_OK;
}

//=============================================================================
// チャンクデータの読み込み
//=============================================================================
HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset)
{
	DWORD dwRead;
	
	if(SetFilePointer(hFile, dwBufferoffset, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{// ファイルポインタを指定位置まで移動
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if(ReadFile(hFile, pBuffer, dwBuffersize, &dwRead, NULL) == 0)
	{// データの読み込み
		return HRESULT_FROM_WIN32(GetLastError());
	}
	
	return S_OK;
}

//=============================================================================
// 音量調整(0.0〜1.0まで)
//=============================================================================
void VolumeChange(float fVolume)
{

	//g_fVolume += fVolume;

	//if (g_fVolume >= MAX_VOLUME)
	//{
	//	g_fVolume = MAX_VOLUME;
	//}

	//if (g_fVolume <= MIN_VOLUME)
	//{
	//	g_fVolume = MIN_VOLUME;
	//}

	////音量をセットする
	//g_pMasteringVoice->SetVolume(g_fVolume);
}

////=============================================================================
//// 音量取得(3桁表示)
////=============================================================================
//int GetVolume(void)
//{
//	return (int)((g_fVolume + 0.009) * 100);
//}
