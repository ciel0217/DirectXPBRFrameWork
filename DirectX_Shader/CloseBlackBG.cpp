#include "CloseBlackBG.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"

void CloseBlackBG::Config()
{
	m_TextureName = "asset/texture/black.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;
}

void CloseBlackBG::Init()
{
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;
	m_Rotation = m_InitRotation;

	m_CloseSpeed = 30.0f;
	m_IsActive = false;
	m_CanNext = false;
}

void CloseBlackBG::Uninit()
{
}

void CloseBlackBG::Update()
{
	if (m_IsActive) {
		m_Position.x -= m_CloseSpeed;

		if (m_Position.x <= SCREEN_WIDTH / 2.0f) {
			m_CanNext = true;
			m_Position.x = SCREEN_WIDTH / 2.0f;
		}
	}
}
