#pragma once
#include "FrameWork/Resources/CScene.h"
class TestScene : public CScene
{
public:
	TestScene() { SetObject(); }
	void SetObject()override;
};

