#pragma once
#include "FrameWork/Resources/CGameObject.h"

class UIDestroy : public CGameObject2D
{
private:
	int m_FrameCount;

	const int DESTROY_MAX_COUNT = 120;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};