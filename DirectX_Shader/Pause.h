#pragma once
#include "FrameWork/Resources/CGameObject.h"


class Lose;

class Pause : public CGameObject2D
{
private:
	std::string m_GameTextureName;
	std::string m_TitleTextureName;
	std::string m_ArrowTextureName;

	int m_ArrowPoint;
	D3DXVECTOR3 m_GamePosition;
	D3DXVECTOR3 m_TitlePosition;
	D3DXVECTOR3 m_ArrowPosition[2];

	Lose* m_Lose;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};