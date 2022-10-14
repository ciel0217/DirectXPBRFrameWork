#pragma once
#include "FrameWork/Resources/CGameObject.h"

class RockA : public CGameObject3D
{
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};