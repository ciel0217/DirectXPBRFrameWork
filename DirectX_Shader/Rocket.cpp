#include "Rocket.h"
#include "quaternion.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "FrameWork/Resources/BoxCollision.h"
#include "FrameWork/Resources/ParticleEmitter.h"
#include "FrameWork/Manager/ManagerScene.h"

void Rocket::Config()
{
	m_ModelName = "asset/model/p.obj";
	m_Model = ManagerModel::Load(m_ModelName);

	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");

	
	m_Collision = new BoxCollision(this, D3DXVECTOR3(10.0f, 10.0f, 50.0f));
	m_Collision->SetTagMask(TAG_ENEMY);
}

void Rocket::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	if (m_TargetObject) {
		D3DXVECTOR3 dir = m_TargetObject->GetPosition() - m_Position;
		D3DXVec3Normalize(&dir, &dir);
		m_Rotation = FromToRotation(UnitX, dir);
	}
	else {
		m_Rotation = FromToRotation(UnitX, m_Velocity);
	}
	m_ParticleEmitter = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<ParticleEmitter>(e3DObject);
	
	m_ParticleEmitter->SetParent(this);
	m_ParticleEmitter->SetInitPosition(m_Position);
	m_ParticleEmitter->SetLimitLifeTime(100);
	m_ParticleEmitter->SetGenerateTime(1);
	m_ParticleEmitter->SetParticleScale(D3DXVECTOR3(10.0f,10.0f,10.0f));
	m_ParticleEmitter->SetSpeed(5.0f);
	m_ParticleEmitter->SetTextureName("asset/texture/particle-smoke.png");
	m_ParticleEmitter->Config();
	m_ParticleEmitter->Init();

}

void Rocket::Uninit()
{
	
}

void Rocket::Update()
{
	if (!m_IsPause) {
		m_FrameCount++;
		if (m_FrameCount > LIFE_TIME) {
			m_IsDestroy = true;
			m_ParticleEmitter->SetDestroy(m_IsDestroy);
			return;
		}
		//今現在のベクトル
		D3DXVECTOR3 axisx = Transform(UnitX, m_Rotation);


		m_RocketUp = Transform(UnitY, m_Rotation);
		m_LookAtPoint = m_Position + axisx * 60.0f;

		if (m_TargetObject && CanTracking()) {
			//回転後のベクトル
			D3DXVECTOR3 dir = m_TargetObject->GetPosition() - m_Position;
			D3DXVec3Normalize(&dir, &dir);

			float dot = D3DXVec3Dot(&axisx, &dir);
			float arccos = dot / (D3DXVec3Length(&axisx) * D3DXVec3Length(&dir));
			float degree = acosf(arccos);

			if (degree > 45.0f) {
				m_Rotation = RotateAxis(axisx, 45.0f, m_Rotation);
			}
			else {
				m_Rotation = FromToRotation(UnitX, dir);
			}
			
		}
		axisx = Transform(UnitX, m_Rotation);
		m_Position += axisx * 60.0f;

		m_Collision->UpdateCollision();
	}
	
}

void Rocket::OnCollisionEnter(CGameObject3D * collider)
{
	m_IsDestroy = true;
	m_ParticleEmitter->SetDestroy(m_IsDestroy);
}

bool Rocket::CanTracking()
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 target_pos = m_TargetObject->GetPosition();
	D3DXMatrixIdentity(&mtxWorld);


	D3DXMATRIX mtxProjection, mtxView;
	D3DXMatrixLookAtLH(&mtxView, &m_Position, &m_LookAtPoint, &m_RocketUp);

	D3DXMatrixPerspectiveFovLH(&mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, m_Near, m_Far);
	

	D3DXVECTOR4 result;

	//world view変換
	result = D3DXVECTOR4(
		mtxView._11 * target_pos.x + mtxView._21 * target_pos.y + mtxView._31 * target_pos.z + mtxView._41 * 1.0f,
		mtxView._12 * target_pos.x + mtxView._22 * target_pos.y + mtxView._32 * target_pos.z + mtxView._42 * 1.0f,
		mtxView._13 * target_pos.x + mtxView._23 * target_pos.y + mtxView._33 * target_pos.z + mtxView._43 * 1.0f,
		mtxView._14 * target_pos.x + mtxView._24 * target_pos.y + mtxView._34 * target_pos.z + mtxView._44 * 1.0f
	);
	//view projection変換
	result = D3DXVECTOR4(
		mtxProjection._11 * result.x + mtxProjection._21 * result.y + mtxProjection._31 * result.z + mtxProjection._41 * result.w,
		mtxProjection._12 * result.x + mtxProjection._22 * result.y + mtxProjection._32 * result.z + mtxProjection._42 * result.w,
		mtxProjection._13 * result.x + mtxProjection._23 * result.y + mtxProjection._33 * result.z + mtxProjection._43 * result.w,
		mtxProjection._14 * result.x + mtxProjection._24 * result.y + mtxProjection._34 * result.z + mtxProjection._44 * result.w
	);

	//(xの条件 && yの条件 && zの条件) = 視錐台のなかにある
	if ((-result.w < result.x && result.x < result.w) && (-result.w < result.y && result.y < result.w) && (-result.w < result.z && result.z < result.w))
		return true;
	

	return false;
}