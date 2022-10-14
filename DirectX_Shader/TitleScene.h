#pragma once
#include "FrameWork/Resources/CScene.h"

class TitleScene : public CScene
{
public:
	TitleScene() { SetObject(); }
	void SetObject()override;
};