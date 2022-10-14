#pragma once
#include "FrameWork/Resources/CGameObject.h"

class Player;

class Reticle : public CGameObject3D
{
private:
	std::string m_TextureName;
	MATERIAL_CBUFFER m_Material;

	Player* m_Player;

	const D3DXVECTOR3 UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};