#pragma once
#include "FrameWork/Resources/CGameObject.h"

class DamageEffect : public CGameObject2D
{
private:
	float m_FrameCount;

	const float DESTROY_FRAME_COUNT = 300.0f;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};