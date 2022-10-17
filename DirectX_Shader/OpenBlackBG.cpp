#include "OpenBlackBG.h"
#include "FrameWork/Manager/ManagerTexture.h"

void OpenBlackBG::Config()
{
	m_TextureName = "asset/texture/black.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;
}

void OpenBlackBG::Init()
{
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;
	m_Rotation = m_InitRotation;

	m_OpenSpeed = 30.0f;
	m_FinishOpen = false;
}

void OpenBlackBG::Uninit()
{
}

void OpenBlackBG::Update()
{
	if (m_IsActive) {
		m_Position.x += m_OpenSpeed;

		if (m_Position.x > SCREEN_WIDTH + SCREEN_WIDTH / 2.0f) {
			m_IsActive = false;
			m_FinishOpen = true;
			m_Position.x = SCREEN_WIDTH + SCREEN_WIDTH / 2.0f;
		}
	}
}
