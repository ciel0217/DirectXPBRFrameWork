#pragma once
#include "FrameWork/Resources/Camera.h"

class ResultSceneCamera : public Camera
{
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
};