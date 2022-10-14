#include "Sousasetumei.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"

void Sousasetumei::Config()
{
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_TextureName = "asset/texture/Operation.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_RenderQueue = e2D;
}

void Sousasetumei::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

}

void Sousasetumei::Uninit()
{
}
