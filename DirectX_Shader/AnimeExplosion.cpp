#include "AnimeExplosion.h"
#include "FrameWork/Resources/Sprite.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "UserEnum.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Manager/audio.h"

void AnimeExplosion::Config()
{
	m_TextureName = "asset/texture/explosion.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/DefaultModel.hlsl");
	ZeroMemory(&m_Material, sizeof(m_Material));
	m_RenderQueue = eTransparent;

	m_Audio = new Audio();
	m_Audio->Load("asset/music/bomb000.wav");
}

void AnimeExplosion::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_FrameCount = 0;
	m_Wh = D3DXVECTOR2(1.0f / 7.0f, 1.0f);
	m_CurrentUv = D3DXVECTOR2(0.0f, 0.0f);
	m_Audio->Play();
}

void AnimeExplosion::Uninit()
{
	m_Audio->StopAll();
	delete m_Audio;
}

void AnimeExplosion::Update()
{
	m_FrameCount++;
	if (m_FrameCount > NEXT_ANIME_COUNT)
	{
		m_CurrentUv.x += m_Wh.x;
		m_FrameCount = 0;
		if (m_CurrentUv.x > 1.0f)m_IsDestroy = true;
	}
}

void AnimeExplosion::Draw()
{
	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	Sprite2D::DrawBillboard(ManagerTexture::GetTexture(m_TextureName), m_Position, m_Scale, m_Color, m_CurrentUv, m_Wh, m_Material);
}
