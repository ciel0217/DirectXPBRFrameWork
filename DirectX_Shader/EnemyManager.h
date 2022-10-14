#pragma once
#include "FrameWork/Resources/CGameObject.h"
#include <list>


class EnemyManager : public CGameObject3D
{
private:
	std::list<CGameObject3D*> m_EnemyList;

	const int ENEMY_MAX = 15;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void DeleteEnemy(CGameObject3D* obj);
	std::list<CGameObject3D*> GetEnemyList() { return m_EnemyList; }
};