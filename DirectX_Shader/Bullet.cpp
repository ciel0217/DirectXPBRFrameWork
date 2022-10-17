#include "Bullet.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Resources/Sprite.h"
#include "FrameWork/Resources/BoxCollision.h"


void Bullet::Config()
{
	m_TextureName = "asset/texture/bullet.png";
	ManagerTexture::LoadTexture(m_TextureName);
	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");
	m_Tag = TAG_ROCKET;
	m_RenderQueue = eTransparent;

	ZeroMemory(&m_Material, sizeof(m_Material));
	m_Material.Metaric = 0.0f;
	m_Material.Roughness = 1.0f;
	m_Material.Specular = 0.0f;

	m_Collision = new BoxCollision(this, D3DXVECTOR3(40.0f, 40.0f, 40.0f));
	m_Collision->SetTagMask(TAG_ENEMY);
}

void Bullet::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_Speed = 60.0f;
	m_LifeFrame = 0;
}

void Bullet::Uninit()
{
	delete m_Collision;
}

void Bullet::Update()
{
	if (!m_IsPause) {
		m_Position += m_Velocity * m_Speed;

		m_LifeFrame++;
		if (m_LifeFrame > 300)m_IsDestroy = true;

		m_Collision->UpdateCollision();
	}
}

void Bullet::Draw()
{
	ID3D11ShaderResourceView* texture = ManagerTexture::GetTexture(m_TextureName);
	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	Sprite2D::DrawBillboard(texture, m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), m_Material);
}

void Bullet::OnCollisionEnter(CGameObject3D * collider)
{
	m_IsDestroy = true;
}
