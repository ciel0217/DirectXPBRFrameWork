#pragma once
#include "FrameWork/Resources/CGameObject.h"
#include "FrameWork/Resources/BoxCollision.h"

class Player;
class EnemyNewCarrier : public CGameObject3D
{
private:
	Player* m_Player;
	int m_Rpm;


	const int MAX_RPM = 450;
	const int SCORE = 15;

public:
	EnemyNewCarrier() {}
	~EnemyNewCarrier();
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;

	void OnCollisionEnter(CGameObject3D* collider)override;
};