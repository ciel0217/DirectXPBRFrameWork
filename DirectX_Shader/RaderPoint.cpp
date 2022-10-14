#include "RaderPoint.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"

void RaderPoint::Config()
{
	m_TextureName = "asset/texture/bullet000.png";
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");


	ManagerTexture::LoadTexture(m_TextureName);

	m_RenderQueue = e2D;
}

void RaderPoint::Init()
{
}

void RaderPoint::Uninit()
{
}

void RaderPoint::Update()
{
}
