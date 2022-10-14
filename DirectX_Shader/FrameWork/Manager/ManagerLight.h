#pragma once
#include "../Resources/CLight.h"
#include <list>
#define LIGHT_MAX 10

class CBuffer;
class ManagerLight {
private:
	std::list<CLight*> m_Lights;
	D3DXCOLOR m_GlobalAmbient;
	CBuffer* m_LightCBuffer;

public:
	ManagerLight();
	~ManagerLight();
	
	void Config();
	void Init();
	void Uninit();
	void Update();
	void SetLightCBuffer();

	void SetLight(CLight* light) {
		m_Lights.push_back(light);
	}

};