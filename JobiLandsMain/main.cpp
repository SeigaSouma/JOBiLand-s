//==============================================================
//
//HGS2022_Springs[main.cpp]
//Author:���n�Ή�
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

//���������[�N�o�͗p
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//�}�N����`
#define CLASS_NAME			"Window Class"			//�E�C���h�E�N���X�̖��O
#define WINDOW_NAME			"����"			//�E�C���h�E�̖��O

//�v���g�^�C�v�錾
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawObject(int nType);

//�O���[�o���ϐ�
LPDIRECT3D9 g_pD3D = NULL;						//Direct3D�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;			//Direct3D�f�o�C�X�ւ̃|�C���^
int g_nCountFPS = 0;							//FPS�J�E���^
MODE g_mode = MODE_GAME;						//���݂̃��[�h

//==============================================================
//���C���֐�
//==============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrey, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;				//�E�C���h�E�n���h��(���ʎq)
	MSG msg;				//���b�Z�[�W���i�[����ϐ�
	DWORD dwCurrentTime;	//���ݎ���
	DWORD dwExecLastTime;	//�Ō�ɏ�����������
	DWORD dwFrameCount;		//�t���[���J�E���g
	DWORD dwFPSLastTime;	//�Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							//�E�C���h�E�̃X�^�C��
		WindowProc,							//�E�C���h�E�̃X�^�C��
		0,									//0�ɂ���i�ʏ�͎g�p���Ȃ��j
		0,									//0�ɂ���i�ʏ�͎g�p���Ȃ��j
		hInstance,							//�C���X�^���X�n���h��
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),		//�^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			//�}�E�X�J�[�\��
		HBRUSH(COLOR_WINDOW + 1),			//�N���C�A���g�̈�̔w�i�F
		NULL,								//���j���[�o�[
		CLASS_NAME,							//�E�C���h�E�N���X�̖��O
		LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),		//�t�@�C���̃A�C�R��
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//�E�C���h�E�𐶐�
	hWnd = CreateWindowEx(0,		//�g���E�C���h�E�X�^�C��
		CLASS_NAME,					//�E�C���h�E�N���X�̖��O
		WINDOW_NAME,				//�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		//�E�C���h�E�X�^�C��
		CW_USEDEFAULT,				//�E�C���h�E�̍���X���W
		CW_USEDEFAULT,				//�E�C���h�E�̍���Y���W
		(rect.right - rect.left),	//�E�C���h�E�̕�
		(rect.bottom - rect.top),	//�E�C���h�E�̕�
		NULL,						//�e�E�C���h�E�̃n���h��
		NULL,						//���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,					//�C���X�^���X�n���h��
		NULL);

	//����������
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{//���������������s�����ꍇ
		return -1;
	}

	//����\��ݒ�
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//����������
	dwExecLastTime = timeGetTime();		//���ݎ������擾�i�ۑ��j
	dwFrameCount = 0;					//����������
	dwFPSLastTime = timeGetTime();		//���ݎ������擾�i�ۑ��j

	//�E�C���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);			//�E�C���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);					//�N���C�A���g�̈���X�V

	//���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windows�̏���
			if (msg.message == WM_QUIT)
			{//WM_QUIT���b�Z�[�W���󂯎�����烁�b�Z�[�W���[�v�𔲂���
				break;
			}
			else
			{
				//���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);		//���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕Ԋ�
				DispatchMessage(&msg);		//�E�C���h�E�v�����[����փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			dwCurrentTime = timeGetTime();	//���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5�b�o��
				//FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;					//�t���[���J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60����1�b�o��
				dwExecLastTime = dwCurrentTime;		//�����J�n�̎���[���ݎ���]��ۑ�
				dwFrameCount++;						//�t���[���J�E���g�����Z

				//�X�V����
				Update();

				//�`�揈��
				Draw();
			}
		}
	}

	//�I������
	Uninit();

	//����\��߂�
	timeEndPeriod(1);

	//�E�C���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	//
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);

	//���������[�N���o��
	_CrtDumpMemoryLeaks();

	return(int)msg.wParam;
}

//==============================================================
//�E�C���h�E�v���V�[�W��
//==============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;												//�Ԃ�l���i�[
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//�E�C���h�E�̗̈�i�l�p�j

	switch (uMsg)
	{
	case WM_DESTROY:			//�E�C���h�E�j���̃��b�Z�[�W
								//WM_QUIT���b�Z�[�W��Ԃ�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:			//�L�[�����̃��b�Z�[�W

		switch (wParam)
		{
		case VK_ESCAPE:			//[ESC]�L�[�������ꂽ
			nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
			if (nID == IDYES)
			{
				//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:				//����{�^�������̃��b�Z�[�W
		nID = MessageBox(hWnd, "�I�����܂����H", "�I�����b�Z�[�W", MB_YESNO);
		if (nID == IDYES)
		{
			//�E�C���h�E��j������(WM_DESTROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0��Ԃ��Ȃ��ƏI�����Ă��܂�
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//����̏�����Ԃ�
}

//==============================================================
//����������
//==============================================================
HRESULT Init(HINSTANCE hinstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//�f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	//�v���[���e�[�V�����p�����[�^

	//Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							//�p�����[�^�[�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//�Q�[����ʃT�C�Y�i���j
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//�Q�[����ʃT�C�Y�i�����j
	d3dpp.BackBufferFormat = d3ddm.Format;						//�o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									//�o�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//�_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;						//�f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//�f�v�X�o�b�t�@�Ƃ���bit16���g��
	d3dpp.Windowed = bWindow;									//�E�C���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//�C���^�[�o��

	//Direct3D�f�o�C�X�̐���
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

	//�����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//�T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		//�e�N�X�`���J��Ԃ��̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//�e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//�}���`�e�N�X�`��
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	g_pD3DDevice->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);

	////���̃u�����h��L���ɂ���
	//g_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	////�F��ݒ�
	//g_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(0.9f, 0.9f, 1.0f, 1.0f));

	//float Start = 7000.0f;//���`���̊J�n�ʒu
	//float End = 8000.0f;//���`���̏I���ʒu
	//					
	//g_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
	//g_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&Start));
	//g_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD *)(&End));

	//g_pD3DDevice->SetRenderState(D3DRS_RANGEFOGENABLE, TRUE);

	//�����̎��ݒ�
	srand((unsigned int)time(0));

	//���͋@��̏���������
	if (FAILED(InitInput(hinstance, hWnd)))
	{
		return E_FAIL;
	}

	//�����L���O�̃��Z�b�g
	//ResetRanking();

	//�T�E���h�̏�����
	InitSound(hWnd);

	//�f�o�b�O�v���b�N�̏���������
	InitDebugProc();

	//�J�����̏���������
	InitCamera();

	//���C�g�̏���������
	InitLight();

	//�v�Z�̏���������
	InitCalculastion();

	//�t�F�[�h�̐ݒ�
	InitFade(g_mode);

	//�t�F�[�h���Ȃ��t�F�[�h�̐ݒ�
	InitNoFade();

	return S_OK;
}

//==============================================================
//�I������
//==============================================================
void Uninit(void)
{
	//�^�C�g����ʂ̏I������
	UninitTitle();

	//�`���[�g���A����ʂ̏I������
	UninitTutorial();

	//�Q�[����ʂ̏I������
	UninitGame();

	//���U���g��ʂ̏I������
	UninitResult();

	//�����L���O��ʂ̏I������
	UninitRanking();

	//�T�E���h�̏I������
	UninitSound();

	//�t�F�[�h�̏I������
	UninitFade();

	//���͋@��̏I������
	UninitInput();

	//�f�o�b�O�v���b�N�̏I������
	UninitDebugProc();

	//���C�g�̏I������
	UnInitLight();

	//�J�����̏I������
	UninitCamera();

	//�t�F�[�h���Ȃ��t�F�[�h�̏I������
	UninitNoFade();

	//Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	//Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}

//==============================================================
//�X�V����
//==============================================================
void Update(void)
{
	//�J�����̏��擾
	Camera *pCamera = GetCamera();

	//���͋@��̍X�V����
	UpdateInput();

	//�f�o�b�O�v���b�N�̍X�V����
	UpdateDebugProc();
	
	if (GetPause() == false)
	{//�|�[�Y������Ȃ����

		//�J�����̍X�V����
		UpdateCamera();

		if (pCamera->nState != CAMERASTATE_MANYSPAWN)
		{//��ʔ����̃J��������Ȃ�������

			//���C�g�̍X�V����
			UpdateLight();
		}
	}

	switch (g_mode)
	{
	case MODE_LOGO:		//���S���
		UpdateLogo();
		break;

	case MODE_TITLE:		//�^�C�g�����
		UpdateTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		UpdateTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		UpdateGame();
		break;

	case MODE_RESULT:		//���U���g���
		UpdateResult();
		break;

	case MODE_RANKING:		//�����L���O���
		UpdateRanking();
		break;
	}

	//�t�F�[�h�̍X�V����
	UpdateFade();

	//�t�F�[�h���Ȃ��t�F�[�h�̍X�V����
	UpdateNoFade();

	//�I���̃t���O���^�Ȃ�
	if (GetEndFrag() == true)
	{
		//�E�C���h�E��j������(VK_DESTROY���b�Z�[�W�𑗂�)
		DestroyWindow(NULL);

		//WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
	}

}

//==============================================================
//�`�揈��
//==============================================================
void Draw(void)
{
	//��ʃN���A�i�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A�j
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//�ʏ�
	g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//�`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{//�`��J�n�����������ꍇ

		D3DVIEWPORT9 viewportDef;	//�r���[�|�[�g�ۑ�

		//���݂̃r���[�|�[�g���擾
		g_pD3DDevice->GetViewport(&viewportDef);

		//�J�����̐ݒ菈��
		SetCamera(CAMERATYPE_MAIN);

		//�e��I�u�W�F�N�g�̕`��
		DrawObject(DRAWTYPE_MAIN);

		//�J�����̐ݒ菈��
		SetCamera(CAMERATYPE_UI);

		//�e��I�u�W�F�N�g�̕`��
		DrawObject(DRAWTYPE_UI);

		//�r���[�|�[�g�����ɖ߂�
		g_pD3DDevice->SetViewport(&viewportDef);

		//�`��I��
		g_pD3DDevice->EndScene();
	}

	//�o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//==============================================================
//�e��I�u�W�F�N�g�̕`��
//==============================================================
void DrawObject(int nType)
{

	switch (g_mode)
	{
	case MODE_LOGO:		//���S���
		DrawLogo(nType);
		break;

	case MODE_TITLE:		//�^�C�g�����
		DrawTitle(nType);
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		DrawTutorial(nType);
		break;

	case MODE_GAME:			//�Q�[�����
		DrawGame(nType);
		break;

	case MODE_RESULT:		//���U���g���
		DrawResult(nType);
		break;

	case MODE_RANKING:		//�����L���O���
		DrawRanking(nType);
		break;
	}

	if (nType == DRAWTYPE_UI)
	{
		//�t�F�[�h�̕`�揈��
		DrawFade();

		//�t�F�[�h���Ȃ��t�F�[�h�̕`�揈��
		DrawNoFade();

		//�f�o�b�O�v���b�N�̕`�揈��
		DrawDebugProc();
	}
}

//==============================================================
//���[�h�ݒ�
//==============================================================
void SetMode(MODE mode)
{
	//���݂̉��(���[�h)�̏I������
	switch (g_mode)
	{
	case MODE_LOGO:		//���S���
		UninitLogo();
		break;

	case MODE_TITLE:		//�^�C�g�����
		UninitTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		UninitTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		UninitGame();
		break;

	case MODE_RESULT:		//���U���g���
		UninitResult();
		break;

	case MODE_RANKING:		//�����L���O���
		UninitRanking();
		break;
	}

	int nOldMode = g_mode;
	g_mode = mode;			//���[�h��ݒ肷��

	//�V�������(���[�h)�̏���������
	switch (mode)
	{
	case MODE_LOGO:		//���S���
		InitLogo();
		break;

	case MODE_TITLE:		//�^�C�g�����
		InitTitle();
		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����
		InitTutorial();
		break;

	case MODE_GAME:			//�Q�[�����
		InitGame();
		break;

	case MODE_RESULT:		//���U���g���
		InitResult();
		break;

	case MODE_RANKING:		//�����L���O���
		InitRanking(nOldMode);
		break;
	}

	g_mode = mode;			//���[�h��ݒ肷��
}

//==============================================================
//���[�h�̎擾
//==============================================================
MODE GetMode(void)
{
	return g_mode;
}

//==============================================================
//�f�o�C�X�̎擾
//==============================================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//==============================================================
//FPS�̎擾
//==============================================================
int GetFPS(void)
{
	return g_nCountFPS;
}
