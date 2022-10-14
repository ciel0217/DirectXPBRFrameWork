#pragma once
#include "FrameWork/Resources/CGameObject.h"

class PressLKey : public CGameObject2D
{
private:
	float m_FrameCount;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};