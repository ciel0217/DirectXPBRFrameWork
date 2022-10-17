#include "MissileAlert.h"
#include "FrameWork/Manager/ManagerTexture.h"

void MissileAlert::Config()
{
	m_TextureName = "asset/texture/missile.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;
}

void MissileAlert::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_IsActive = false;
}

void MissileAlert::Uninit()
{
}

void MissileAlert::Update()
{
}
