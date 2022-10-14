#pragma once
#include "CommonProcess.h"
#include "../Manager/ManagerModel.h"

class CBuffer;

class CGameObject2D : public CommonProcess 
{
protected:
	std::string m_TextureName;
public:
	virtual void Config()override {};
	virtual void Init()override {};
	virtual void Uninit()override {};
	virtual void Update()override {};
	virtual void Draw()override;
};

class CGameObject3D : public CommonProcess 
{
protected:
	DX11_MODEL* m_Model;
	std::string m_ModelName;
	static CBuffer* m_WorldCBuffer;
	static CBuffer* m_InverseWorldCBuffer;

public:
	CGameObject3D() {};
	virtual ~CGameObject3D()override { };

	static void SetUpGameObject();
	static void ReleaseGameObject();

	virtual void Config()override {};
	virtual void Init()override {};
	virtual void Uninit()override {};
	virtual void Update()override {};
	virtual void Draw()override;

	virtual void OnCollisionEnter(CGameObject3D* collider) {};
};