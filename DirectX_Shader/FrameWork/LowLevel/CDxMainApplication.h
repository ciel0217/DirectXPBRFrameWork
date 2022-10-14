#pragma once
#include "CDxWindow.h"
#include <memory>


class CDxRenderer;
class ManagerTexture;

class CDxMainApplication {
private:
	HINSTANCE m_hInstance;
	WNDCLASSEX m_WndClassWx;
	std::unique_ptr<CDxWindow> m_DxWindow;
	CDxRenderer *m_Renderer;
	ManagerTexture *m_ManagerTexture;


	
	//�f�o�b�O�p
	int m_CountFPS;
	char m_DebugStr[2048] = "Debug";

public:
	CDxMainApplication();
	~CDxMainApplication() {};
	int Run(HINSTANCE hInstance);
	
	HINSTANCE GetHInstance() { return m_hInstance; }
	void GetWndClass(WNDCLASSEX *wc) {
		*wc = m_WndClassWx;
	}

	CDxWindow* GetDxWindow() { return m_DxWindow.get(); }

private:
	BOOL RegistWndClass();
	BOOL CreateMainWnd();
	BOOL Initialize();
	void Loop();
	void ReleaseApp();
	void Update();
	void Draw();

	//�f�o�b�O�p
	char* GetDebugStr(void) {
		return m_DebugStr;
	}
};