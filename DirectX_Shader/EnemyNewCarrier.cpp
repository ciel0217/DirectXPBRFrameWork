#include "EnemyNewCarrier.h"
#include "FrameWork/Resources/BoxCollision.h"
#include "Player.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "Math/Quaternion.h"
#include "BulletEnemy.h"
#include "UIDestroy.h"
#include "Score.h"
#include "AnimeExplosion.h"
#include "EnemyManager.h"
#include "EnemyRocket.h"

EnemyNewCarrier::~EnemyNewCarrier()
{
}

void EnemyNewCarrier::Config()
{
	m_ModelName = "asset/model/NewCarrier.obj";
	m_Model = ManagerModel::Load(m_ModelName);
	m_Model->SetMetaric(0.5f);
	m_Model->SetRoughness(1.0f);

	m_Tag = TAG_ENEMY;

	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");
}

void EnemyNewCarrier::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_Rpm = 0;

	m_Collision = new BoxCollision(this, D3DXVECTOR3(1000.0f, 1000.0f, 2000.0f), D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	m_Collision->SetTagMask(TAG_PLAYER || TAG_ROCKET);

	m_Player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Player>(e3DObject);
}

void EnemyNewCarrier::Uninit()
{
	delete m_Collision;
}

void EnemyNewCarrier::Update()
{
	if (!m_IsPause) {
		D3DXVECTOR3 dir = m_Player->GetPosition() - m_Position;
		if (m_Player->GetActive()) {
			if (D3DXVec3Length(&dir) < 15000.0f && D3DXVec3Length(&dir) > 5000.0f)
			{

				m_Rpm++;
				if (m_Rpm > MAX_RPM)
				{
					EnemyRocket* temp = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<EnemyRocket>(e3DObject);
					if (!temp) {
						EnemyRocket* bullet = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<EnemyRocket>(e3DObject);
						bullet->SetInitPosition(m_Position + D3DXVECTOR3(0.0f, 300.0f, 0.0f));
						bullet->SetInitScale(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
						bullet->SetTargetObject(m_Player);
						D3DXVec3Normalize(&dir, &dir);
						bullet->SetInitVelocity(dir);
						bullet->Config();
						bullet->Init();
					}
					m_Rpm = 0;
				}

			}
		}
		m_Collision->UpdateCollision();
	}
}

void EnemyNewCarrier::OnCollisionEnter(CGameObject3D * collider)
{
	UIDestroy* ui = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<UIDestroy>(e2DObject);
	ui->SetActive(true);

	Score* score = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Score>(e2DObject);
	score->SetScore(SCORE);

	AnimeExplosion* expl = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<AnimeExplosion>(e3DObject);
	expl->SetInitPosition(m_Position);
	expl->SetInitScale(D3DXVECTOR3(200.0f, 200.0f, 0.0f));
	expl->Config();
	expl->Init();

	m_IsDestroy = true;
	if (m_IsDestroy)ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<EnemyManager>(e3DObject)->DeleteEnemy(this);
}
