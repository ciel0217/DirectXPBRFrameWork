#include "UIPitchUp.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "Player.h"
#include "FrameWork/Manager/ManagerScene.h"

void UIPitchUp::Config()
{
	m_TextureName = "asset/texture/pitchup.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;
}

void UIPitchUp::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_Player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Player>(e3DObject);
	m_IsActive = false;
}

void UIPitchUp::Uninit()
{
}

void UIPitchUp::Update()
{
	
	if (m_Player->GetPosition().y > UNDER_YAXIS) {
		m_IsActive = false;
	}
	else {
		m_IsActive = true;
	}

}
