#pragma once
#include "FrameWork/Resources/CGameObject.h"

class Audio;
class ResultBG : public CGameObject2D
{
private:
	Audio* m_Audio;
	bool m_SceneChange;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};