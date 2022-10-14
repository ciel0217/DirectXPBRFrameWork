#pragma once
#include "FrameWork/Resources/CGameObject.h"

class Test3D : public CGameObject3D
{
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;

	void Load();
	void Settt(float, float);
};

