#include "CDxMainApplication.h"
#include <time.h>
#include "CDxRenderer.h"
#include "../Manager/ManagerTexture.h"
#include "../Resources/Sprite.h"
#include "../Manager/ManagerScene.h"
#include "../Manager/ManagerInput.h"
#include "../Manager/ManagerShader.h"
#include "../Manager/ManagerCollsionDetection.h"
#include "../Resources/CGameObject.h"
#include "../Manager/audio.h"
#include "../RenderingPass/CameraRenderer.h"

CDxMainApplication *pApp = nullptr;

CDxMainApplication::CDxMainApplication()
{
	pApp = this;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	UNREFERENCED_PARAMETER(hPrevInstance);	// 無くても良いけど、警告が出る（未使用宣言）
	UNREFERENCED_PARAMETER(lpCmdLine);		// 無くても良いけど、警告が出る（未使用宣言）
	CDxMainApplication tmpApp;

	pApp->Run(hInstance);
}

int CDxMainApplication::Run(HINSTANCE hInstance)
{

	m_hInstance = hInstance;
	m_DxWindow = std::make_unique<CDxWindow>(hInstance);
	

	if (!RegistWndClass()) 
	{
		return -1;
	}

	if (!CreateMainWnd()) 
	{
		return -1;
	}

	if (!Initialize()) 
	{
		return -1;
	}

	Loop();

	ReleaseApp();


	return S_OK;
}


BOOL CDxMainApplication::RegistWndClass()
{
	//TODOちゃんとエラー処理
	HRESULT hr = m_DxWindow->RegisterWindowClass(&m_WndClassWx);
	if (hr == S_OK)return 1;
	return -1;
}

BOOL CDxMainApplication::CreateMainWnd()
{
	//TODOちゃんとエラー処理
	HRESULT hr = m_DxWindow->CreateMainWindow();
	if (hr == S_OK)return 1;
	return -1;
}

BOOL CDxMainApplication::Initialize()
{
	//初期化処理するやつ
	HRESULT hr;
	m_Renderer = new CDxRenderer(m_DxWindow->GetHWND(), m_hInstance);
	hr = m_Renderer->InitRenderer();
	if (hr != S_OK)return -1;

	Sprite2D::InitSprite();
	ManagerInput::Create();
	ManagerInput::GetInstance()->InitInput(m_hInstance, m_DxWindow->GetHWND());
	ManagerShader::CompileShader();
	ManagerModel::Init();
	ManagerCollisionDetection::Create();

	Audio::InitMaster();
	CGameObject3D::SetUpGameObject();
	ManagerScene::Create();
	

	return 1;
}

void CDxMainApplication::Loop()
{
	srand((unsigned int)time(NULL));

	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	
	MSG msg;
	
	// フレームカウント初期化
	timeBeginPeriod(1);	// 分解能を設定
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// システム時刻をミリ秒単位で取得
	dwCurrentTime = dwFrameCount = 0;
	

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳と送出
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1秒ごとに実行
			{
#ifdef _DEBUG
				m_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPSを測定した時刻を保存
				dwFrameCount = 0;							// カウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60秒ごとに実行
			{
				dwExecLastTime = dwCurrentTime;	// 処理した時刻を保存

#ifdef _DEBUG	// デバッグ版の時だけFPSを表示する
				wsprintf(m_DebugStr, WINDOW_NAME);
				wsprintf(&m_DebugStr[strlen(m_DebugStr)], " FPS:%d", m_CountFPS);
				SetWindowText(m_DxWindow->GetHWND(), m_DebugStr);
#endif

				Update();			// 更新処理
				Draw();				// 描画処理

				dwFrameCount++;
			}
		}
	}
}

void CDxMainApplication::ReleaseApp()
{
	timeEndPeriod(1);				// 分解能を戻す

	ManagerInput::GetInstance()->UninitInput();
	CGameObject3D::ReleaseGameObject();
	ManagerModel::Unload();
	ManagerCollisionDetection::Release();
	ManagerScene::Release();
	Sprite2D::Release();
	CameraRenderer::Release();
	ManagerShader::Release();
	
	Audio::UninitMaster();


	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, m_WndClassWx.hInstance);
}

void CDxMainApplication::Update()
{
	//InputとSceneのUpdate
	ManagerInput::GetInstance()->UpdateInput();
	ManagerScene::GetInstance()->Update();
}

void CDxMainApplication::Draw()
{
	ManagerScene::GetInstance()->Draw();

	m_Renderer->Present();
}
