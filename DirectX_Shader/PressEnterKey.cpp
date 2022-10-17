#include "PressEnterKey.h"
#include "FrameWork/Manager/ManagerTexture.h"


void PressLKey::Config()
{
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_TextureName = "asset/texture/enter.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_RenderQueue = e2D;
}

void PressLKey::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_FrameCount = 0.0f;
}

void PressLKey::Uninit()
{
}

void PressLKey::Update()
{
	m_FrameCount += 0.05f;

	m_Color.a = sinf(m_FrameCount) * 0.5f + 0.5f;
}
