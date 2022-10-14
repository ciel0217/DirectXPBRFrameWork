#include "ResultScoreFrame.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"

void ResultScoreFrame::Config()
{
	m_TextureName = "asset/texture/ScoreFrame.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;
}

void ResultScoreFrame::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
}

void ResultScoreFrame::Uninit()
{
}

void ResultScoreFrame::Update()
{
}
