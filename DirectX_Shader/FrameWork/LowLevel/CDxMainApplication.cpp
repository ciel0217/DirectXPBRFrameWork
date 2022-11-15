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
	UNREFERENCED_PARAMETER(hPrevInstance);	// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
	UNREFERENCED_PARAMETER(lpCmdLine);		// �����Ă��ǂ����ǁA�x�����o��i���g�p�錾�j
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
	//TODO�����ƃG���[����
	HRESULT hr = m_DxWindow->RegisterWindowClass(&m_WndClassWx);
	if (hr == S_OK)return 1;
	return -1;
}

BOOL CDxMainApplication::CreateMainWnd()
{
	//TODO�����ƃG���[����
	HRESULT hr = m_DxWindow->CreateMainWindow();
	if (hr == S_OK)return 1;
	return -1;
}

BOOL CDxMainApplication::Initialize()
{
	//����������������
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
	
	// �t���[���J�E���g������
	timeBeginPeriod(1);	// ����\��ݒ�
	dwExecLastTime = dwFPSLastTime = timeGetTime();	// �V�X�e���������~���b�P�ʂŎ擾
	dwCurrentTime = dwFrameCount = 0;
	

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂ꂽ�烋�[�v�I��
				break;
			}
			else
			{
				// ���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();

			if ((dwCurrentTime - dwFPSLastTime) >= 1000)	// 1�b���ƂɎ��s
			{
#ifdef _DEBUG
				m_CountFPS = dwFrameCount;
#endif
				dwFPSLastTime = dwCurrentTime;				// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;							// �J�E���g���N���A
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))	// 1/60�b���ƂɎ��s
			{
				dwExecLastTime = dwCurrentTime;	// ��������������ۑ�

#ifdef _DEBUG	// �f�o�b�O�ł̎�����FPS��\������
				wsprintf(m_DebugStr, WINDOW_NAME);
				wsprintf(&m_DebugStr[strlen(m_DebugStr)], " FPS:%d", m_CountFPS);
				SetWindowText(m_DxWindow->GetHWND(), m_DebugStr);
#endif

				Update();			// �X�V����
				Draw();				// �`�揈��

				dwFrameCount++;
			}
		}
	}
}

void CDxMainApplication::ReleaseApp()
{
	timeEndPeriod(1);				// ����\��߂�

	ManagerInput::GetInstance()->UninitInput();
	CGameObject3D::ReleaseGameObject();
	ManagerModel::Unload();
	ManagerCollisionDetection::Release();
	ManagerScene::Release();
	Sprite2D::Release();
	CameraRenderer::Release();
	ManagerShader::Release();
	
	Audio::UninitMaster();


	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, m_WndClassWx.hInstance);
}

void CDxMainApplication::Update()
{
	//Input��Scene��Update
	ManagerInput::GetInstance()->UpdateInput();
	ManagerScene::GetInstance()->Update();
}

void CDxMainApplication::Draw()
{
	ManagerScene::GetInstance()->Draw();

	m_Renderer->Present();
}
