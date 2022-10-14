#pragma once
#include "CGameObject.h"

class Particle : public CGameObject3D
{
private:
	std::string m_TextureName;
	MATERIAL_CBUFFER m_Material;
	int m_FrameCount;
	int m_LimitLifeTime = 10;
	float m_Speed = 0;
	D3DXVECTOR3 m_AddScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_AddAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR   m_AddColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	const D3DXVECTOR3  UnitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3  UnitY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3  UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);


public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetSpeed(float speed) { m_Speed = speed; }
	void SetAddScale(D3DXVECTOR3 scale) { m_AddScale = scale; }
	void SetAddAngle(D3DXVECTOR3 angle) { m_AddAngle = angle; }
	void SetAddColor(D3DXCOLOR color) { m_AddColor = color; }

	void SetLimitLifeTime(int time) { m_LimitLifeTime = time; }
	void SetTextureName(std::string name) { m_TextureName = name; }

};