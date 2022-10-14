#pragma once
#include "FrameWork/Resources/Camera.h"


class GameSceneCamera : public Camera
{
public:
	void Init()override;
	void Uninit()override;
	void Update()override;

};