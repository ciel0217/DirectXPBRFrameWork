#include "Reticle.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Resources/Sprite.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "Player.h"
#include "quaternion.h"
#include "enum.h"

void Reticle::Config()
{
	m_TextureName = "asset/texture/Reticle.png";
	ManagerTexture::LoadTexture(m_TextureName);
	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");

	ZeroMemory(&m_Material, sizeof(m_Material));
	m_Material.Metaric = 0.0f;
	m_Material.Roughness = 1.0f;
	m_Material.Specular = 0.0f;

	m_RenderQueue = eTransparent;
}

void Reticle::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_Player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Player>(e3DObject);
}

void Reticle::Uninit()
{
}

void Reticle::Update()
{
	D3DXQUATERNION player_qua = m_Player->GetRotation();
	D3DXVECTOR3 player_pos = m_Player->GetPosition();

	D3DXVECTOR3 player_axisz = Transform(UnitZ, player_qua);

	m_Position = player_pos + player_axisz * 600.0f;
}

void Reticle::Draw()
{
	ID3D11ShaderResourceView* texture = ManagerTexture::GetTexture(m_TextureName);
	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	Sprite2D::DrawBillboard(texture, m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f), m_Material);
}
