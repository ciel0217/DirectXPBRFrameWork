#include "DamageEffect.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"

void DamageEffect::Config()
{
	m_TextureName = "asset/texture/damage.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;

	m_IsActive = false;
}

void DamageEffect::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_FrameCount = 0.0f;
}

void DamageEffect::Uninit()
{
}

void DamageEffect::Update()
{
	if (m_IsActive) {
		m_FrameCount += 1.0f;
		if (m_FrameCount > DESTROY_FRAME_COUNT) {
			m_FrameCount = 0.0f;
			m_IsActive = false;
		}
	}
}
