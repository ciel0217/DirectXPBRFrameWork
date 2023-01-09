#include "Particle.h"
#include "../Manager/ManagerTexture.h"
#include "Sprite.h"
#include "../LowLevel/CDxRenderer.h"
#include "CShader.h"
#include "DevelopEnum.h"
#include "../../Math/Quaternion.h"
void Particle::Config()
{
	ManagerTexture::LoadTexture(m_TextureName);
	m_Shader = ManagerShader::GetShader("Shader/DefaultModel.hlsl");

	ZeroMemory(&m_Material, sizeof(m_Material));
	m_RenderQueue = eTransparent;
}

void Particle::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_FrameCount = 0;
}

void Particle::Uninit()
{
}

void Particle::Update()
{
	m_FrameCount++;
	if (m_FrameCount > m_LimitLifeTime) 
	{
		m_IsDestroy = true;
		return;
	}

	m_Position += m_Velocity * m_Speed;
	D3DXVECTOR3 axisx = Transform(UnitX, m_Rotation);
	D3DXVECTOR3 axisy = Transform(UnitY, m_Rotation);
	D3DXVECTOR3 axisz = Transform(UnitZ, m_Rotation);

	m_Rotation = RotateAxis(axisx, m_AddAngle.x, m_Rotation);
	m_Rotation = RotateAxis(axisy, m_AddAngle.y, m_Rotation);
	m_Rotation = RotateAxis(axisz, m_AddAngle.z, m_Rotation);

	m_Scale += m_AddScale;
	m_Color += m_AddColor;

}

void Particle::Draw()
{
	ID3D11ShaderResourceView* texture = ManagerTexture::GetTexture(m_TextureName);
	//CDxRenderer::GetRenderer()->SetDepthEnable(true);
	CDxRenderer::GetRenderer()->SetBlendState(BLEND_MODE_ADD);
	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	Sprite2D::DrawBillboard(texture, m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), m_Material);
	CDxRenderer::GetRenderer()->SetBlendState(BLEND_MODE_ALPHABLEND);
}
