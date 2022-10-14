#pragma once
#include "DirectX_Shader/FrameWork/LowLevel/DirectX.h"
#include <string>
#include "../../struct.h"
#include "DirectX_Shader/FrameWork/Manager/ManagerShader.h"

class CShader;
class CRenderInfo
{
protected:
	
	CShader* m_Shader;
	int m_RenderQueue;

	D3DXCOLOR m_Color = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

public:
	int GetRenderQueue() { return m_RenderQueue; }
	void SetColor(D3DXCOLOR color) { m_Color = color; }
};

