#include "Enemy.h"
#include "FrameWork/Resources/BoxCollision.h"
#include "Player.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "Math/Quaternion.h"
#include "BulletEnemy.h"
#include "UIDestroy.h"
#include "Score.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "AnimeExplosion.h"
#include "EnemyManager.h"

Enemy::~Enemy()
{
	
}

void Enemy::Config()
{
	m_ModelName = "asset/model/enemy.obj";
	m_Model = ManagerModel::Load(m_ModelName);
	m_Model->SetMetaric(0.5f);
	m_Model->SetRoughness(1.0f);

	m_Tag = TAG_ENEMY;

	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");
}

void Enemy::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_Rpm = 0;

	m_Collision = new BoxCollision(this, D3DXVECTOR3(250.0f, 250.0f, 250.0f));
	m_Collision->SetTagMask(TAG_PLAYER);

	m_Player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Player>(e3DObject);
}

void Enemy::Uninit()
{
	delete m_Collision;
}

void Enemy::Update()
{
	if (!m_IsPause) 
	{
		D3DXVECTOR3 dir = m_Player->GetPosition() - m_Position;
		if (m_Player->GetActive()) 
		{
			if (D3DXVec3Length(&dir) < 10000.0f)
			{

				m_Rotation = FromToRotation(-UnitZ, dir);

				m_Rpm++;
				if (m_Rpm > MAX_RPM)
				{
					BulletEnemy* bullet = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<BulletEnemy>(e3DObject);
					bullet->SetInitPosition(m_Position);
					bullet->SetInitScale(D3DXVECTOR3(25.0f, 25.0f, 1.0f));

					D3DXVECTOR3 player_front = Transform(UnitZ, m_Player->GetRotation());
					D3DXVec3Normalize(&player_front, &player_front);
					D3DXVECTOR3 target_pos = m_Player->GetPosition() + player_front * 600.0f;
					D3DXVECTOR3 axisz = target_pos - m_Position;

					D3DXVec3Normalize(&axisz, &axisz);
					bullet->SetInitVelocity(axisz);
					bullet->Config();
					bullet->Init();
					m_Rpm = 0;
				}

			}
		}

	}
	m_Collision->UpdateCollision();
}

void Enemy::OnCollisionEnter(CGameObject3D * collider)
{
	CGameObject2D* ui = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<UIDestroy>(e2DObject);
	ui->SetActive(true);

	Score* score = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Score>(e2DObject);
	score->SetScore(SCORE);


	AnimeExplosion* expl = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<AnimeExplosion>(e3DObject);
	expl->SetInitPosition(m_Position);
	expl->SetInitScale(D3DXVECTOR3(200.0f, 200.0f, 0.0f));
	expl->Config();
	expl->Init();

	m_IsDestroy = true;
	ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<EnemyManager>(e3DObject)->DeleteEnemy(this);
}
