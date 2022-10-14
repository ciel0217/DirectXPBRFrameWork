#pragma once
#include "FrameWork/Resources/CGameObject.h"

class Player;
class UIPitchUp : public CGameObject2D
{
private:
	Player* m_Player;

	const float UNDER_YAXIS = 200.0f;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};