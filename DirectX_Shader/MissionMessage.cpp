#include "MissionMessage.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"

void MissionMessage::Config()
{
	m_TextureName = "asset/texture/mission.png";
	ManagerTexture::LoadTexture(m_TextureName);
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");

	m_RenderQueue = e2D;
}

void MissionMessage::Init()
{
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;

	m_IsStop = false;
	m_FrameCount = 0;
}

void MissionMessage::Uninit()
{
}

void MissionMessage::Update()
{
	if (m_Position.x < SCREEN_WIDTH / 2.0f + 10.0f && m_Position.x > SCREEN_WIDTH / 2.0f - 10.0f) {
		m_FrameCount++;
		m_IsStop = true;
	}

	if ((m_FrameCount > STOP_MESSAGE_FINISH_COUNT) && m_IsStop) {
		m_IsStop = false;
	}

	if (!m_IsStop) {
		m_Position.x -= 10.0f;
	}

	if (m_Position.x < -200.0f)m_IsDestroy = true;
}
