#pragma once
#include "FrameWork/Resources/CGameObject.h"

class RaderPoint : public CGameObject2D
{

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};