#pragma once
#include "FrameWork/Resources/CGameObject.h"

class OperationExplain : public CGameObject2D
{
private:

	bool m_SceneChange;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};