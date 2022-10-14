#pragma once
#include "FrameWork/Resources/CGameObject.h"

class MissionMessage : public CGameObject2D
{
private:
	int m_FrameCount;
	bool m_IsStop;

	const int STOP_MESSAGE_FINISH_COUNT = 120;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};