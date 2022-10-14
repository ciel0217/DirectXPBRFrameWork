#pragma once
#include "FrameWork/Resources/CGameObject.h"

class BulletEnemy : public CGameObject3D
{
private:
	std::string m_TextureName;
	MATERIAL_CBUFFER m_Material;
	float m_Speed;
	int m_LifeFrame;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void OnCollisionEnter(CGameObject3D* collider)override;
};