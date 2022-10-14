#pragma once
#include "FrameWork/Resources/CGameObject.h"

class Player;
class Enemy : public CGameObject3D
{
private:
	Player* m_Player;
	int m_Rpm;

	const int MAX_RPM = 30;
	const int SCORE = 10;
	const D3DXVECTOR3 UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
public:
	Enemy() {}
	~Enemy();
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;

	void OnCollisionEnter(CGameObject3D* collider)override;
};