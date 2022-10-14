#include "TargetMark.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Resources/Sprite.h"
#include "enum.h"
#include "FrameWork/Manager/audio.h"

void TargetMark::Config()
{
	m_TextureName = "asset/texture/rockon.png";
	ManagerTexture::LoadTexture(m_TextureName);
	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");

	ZeroMemory(&m_Material, sizeof(m_Material));
	m_RenderQueue = eTransparent;

	m_Audio = new Audio();
}

void TargetMark::Init()
{
	m_Target = nullptr;
	m_Scale = m_InitScale;

	m_IsActive = false;

	m_PreTarget = nullptr;
	m_Audio->Load("asset/music/lockon000.wav");
}

void TargetMark::Uninit()
{
	m_Audio->StopAll();
	delete m_Audio;
}

void TargetMark::Update()
{
	m_IsActive = false;
	if (m_Target) {
		if (m_PreTarget != m_Target)m_Audio->Play();
		m_PreTarget = m_Target;

		m_Position = m_Target->GetPosition();
		m_Position.y += 10.0f;
		m_IsActive = true;
	}
}

void TargetMark::Draw()
{
	if (m_IsActive) {
		ID3D11ShaderResourceView* texture = ManagerTexture::GetTexture(m_TextureName);
		CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
		Sprite2D::DrawBillboard(texture, m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), m_Material);
	}
}
