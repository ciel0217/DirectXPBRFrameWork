#include "TitleLogo.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"
#include "FrameWork/Manager/audio.h"

void TitleLogo::Config()
{
	m_TextureName = "asset/texture/titlelogo.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;
}

void TitleLogo::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_Audio = new Audio;

	m_Audio->Load("asset/music/title.wav");
	m_Audio->Play(true);
}

void TitleLogo::Uninit()
{
	m_Audio->StopAll();
	delete m_Audio;
}

void TitleLogo::Update()
{
}
