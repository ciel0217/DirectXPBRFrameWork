#include "ParticleEmitter.h"
#include "DevelopEnum.h"
#include "CGameObject.h"
#include "Particle.h"
#include <algorithm>
#include "../../Math/Quaternion.h"

void ParticleEmitter::Config()
{
	m_ParticleList.reserve(m_ParticleNum);
	m_RenderQueue = eTransparent;
}

void ParticleEmitter::Init()
{
	m_FrameCount = 0;
	m_Position = m_InitPosition;
}

void ParticleEmitter::Uninit()
{
	m_ParticleList.clear();
	m_ParticleList.shrink_to_fit();
}

void ParticleEmitter::Update()
{
	if (m_Parent) {
		m_Position = m_Parent->GetPosition();
		m_Rotation = m_Parent->GetRotation();
	}

	m_FrameCount++;
	if (m_FrameCount > m_GenerateTime) {
		m_FrameCount = 0;
		Particle* particle = new Particle();
		particle->SetAddAngle(m_AddAngle);
		particle->SetAddColor(m_AddColor);
		particle->SetAddScale(m_AddScale);
		particle->SetLimitLifeTime(m_LimitLifeTime);
		particle->SetSpeed(m_Speed);
		particle->SetTextureName(m_TextureName);
		particle->SetInitPosition(m_Position);
		particle->SetInitScale(m_ParticleScale);
		particle->SetInitRotation(m_ParticleRotation);

		D3DXVECTOR3 axis = Transform(UnitX, m_Rotation);
		particle->SetInitVelocity(axis);
		particle->SetColor(m_ParticleColor);

		particle->Config();
		particle->Init();

		m_ParticleList.push_back(particle);
		
	}

	m_ParticleList.erase(std::remove_if(m_ParticleList.begin(), m_ParticleList.end(), [](CommonProcess* obj) {return obj->Destroy(); }), m_ParticleList.end());

	for (auto particle : m_ParticleList) {
		particle->Update();
	}
}

void ParticleEmitter::Draw()
{
	for (auto particle : m_ParticleList) {
		particle->Draw();
	}
}
