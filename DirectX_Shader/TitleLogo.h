#pragma once
#include "FrameWork/Resources/CGameObject.h"

class Audio;
class TitleLogo : public CGameObject2D
{
private:
	Audio* m_Audio;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};