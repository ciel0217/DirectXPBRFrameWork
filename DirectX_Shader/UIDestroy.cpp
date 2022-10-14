#include "UIDestroy.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"

void UIDestroy::Config()
{
	m_TextureName = "asset/texture/destroy.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");

	m_RenderQueue = e2D;

}

void UIDestroy::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_FrameCount = 0;
	m_IsActive = false;
}

void UIDestroy::Uninit()
{
}

void UIDestroy::Update()
{
	if (m_IsActive) {
		m_FrameCount++;
		if (m_FrameCount > DESTROY_MAX_COUNT) {
			m_IsActive = false;
			m_FrameCount = 0;
		}
	}
}
