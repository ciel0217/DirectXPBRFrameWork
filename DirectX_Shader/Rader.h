#pragma once
#include "FrameWork/Resources/CGameObject.h"
#include <list>

class EnemyManager;
class Player;

class Rader : public CGameObject2D
{
private:
	std::list<D3DXVECTOR3> m_EnemyPoint;
	CGameObject2D* m_RaderPoint;
	EnemyManager* m_EnemyManager;
	Player* m_Player;
	D3DXVECTOR2 m_Wh;
	
	const float SEARCH_RADIUS = 25000.0f;
	const D3DXVECTOR3 UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	const D3DXVECTOR3 UnitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};