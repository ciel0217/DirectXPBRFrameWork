#include "EnemyManager.h"
#include "Enemy.h"
#include "FrameWork/Manager/ManagerScene.h"
#include <random>
#include "EnemyNewCarrier.h"
#include "ResultScene.h"
#include "CloseBlackBG.h"

void EnemyManager::Config()
{
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_real_distribution<> rand30000(0.0f, 30000.0f);
	std::uniform_real_distribution<> rand2000(0.0f, 2000.0f);

	for (int i = 0; i < ENEMY_MAX; i++) {

		if (i % 3 != 0) {
			Enemy* enemy = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<Enemy>(e3DObject);
			enemy->SetInitPosition(D3DXVECTOR3(rand30000(mt) - 15000.0f, rand2000(mt), rand30000(mt) - 15000.0f));
			//enemy->SetInitPosition(D3DXVECTOR3(500.0f, 10.0f, 1000.0f));
			enemy->SetInitScale(D3DXVECTOR3(6.0f, 6.0f, 6.0f));
			enemy->Config();
			m_EnemyList.push_back(enemy);
		}
		else {
			EnemyNewCarrier* enemy = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<EnemyNewCarrier>(e3DObject);
			enemy->SetInitPosition(D3DXVECTOR3(rand30000(mt) - 15000.0f, -10.0f, rand30000(mt) - 15000.0f));
			//enemy->SetInitPosition(D3DXVECTOR3(0.0f, 10.0f, 100.0f));
			enemy->SetInitScale(D3DXVECTOR3(1.0f, 1.0f, 1.0f));
			enemy->Config();
			m_EnemyList.push_back(enemy);
		}
		
	
	}
}

void EnemyManager::Init()
{
	/*for (auto enemy : m_EnemyList) {
		if (!enemy)enemy->Init();
	}*/
}

void EnemyManager::Uninit()
{
	/*for (auto enemy : m_EnemyList) {
		if (!enemy)enemy->Uninit();
	}

	m_EnemyList.clear();*/
}

void EnemyManager::Update()
{
	if (m_EnemyList.size() <= 0) {
		ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->SetActive(true);
	}

	if (ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->GetCanNext()) {
		ManagerScene::GetInstance()->ChangeScene<ResultScene>();
	}


}

void EnemyManager::Draw()
{
	/*for (auto enemy : m_EnemyList) {
		if (!enemy)enemy->Draw();
	}*/
}

void EnemyManager::DeleteEnemy(CGameObject3D * obj)
{
	m_EnemyList.remove_if([obj](CGameObject3D* enemy) {return obj == enemy; });
}
