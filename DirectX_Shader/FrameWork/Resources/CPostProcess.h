#pragma once
#include "CTransform.h"
#include "CommonProcess.h"

class CShader;
class CPostProcess : public CommonProcess
{
protected:
	bool m_IsActive;
	CShader* m_Shader;

public:
	virtual void Config() = 0;
	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	bool Release();

};