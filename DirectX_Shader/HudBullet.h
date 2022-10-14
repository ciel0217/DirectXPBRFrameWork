#pragma once
#include "FrameWork/Resources/CGameObject.h"

class HudBullet : public CGameObject2D
{
private:
	std::string m_NumberTextureName;
	std::string m_ArrowTextureName;

public:
	//設定の初期化
	void Config()override;
	//パラメーターの初期化
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};