#pragma once
#include "FrameWork/Resources/Camera.h"

class TitleSceneCamera : public Camera
{
private:
	float m_count;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};