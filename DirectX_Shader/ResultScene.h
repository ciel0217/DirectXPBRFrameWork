#pragma once
#include "FrameWork/Resources/CScene.h"

class ResultScene : public CScene
{
public:
	ResultScene() { SetObject(); }
	void SetObject()override;
};