#include "OpetationExplain.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "OperationPic.h"
#include "OperationWeapon.h"
#include "TitlePlayer.h"

void OperationExplain::Config()
{
	m_TextureName = "asset/texture/esc_1.png";
	ManagerTexture::LoadTexture(m_TextureName);
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");

	m_RenderQueue = e2D;
}

void OperationExplain::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_IsActive = false;
	m_SceneChange = false;
}

void OperationExplain::Uninit()
{
}

void OperationExplain::Update()
{
	
}
