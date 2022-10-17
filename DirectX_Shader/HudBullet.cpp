#include "HudBullet.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Resources/Sprite.h"
#include "Player.h"
#include "FrameWork/Manager/ManagerScene.h"

void HudBullet::Config()
{
	m_TextureName = "asset/texture/hud_bullet.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_NumberTextureName = "asset/texture/number.png";
	ManagerTexture::LoadTexture(m_NumberTextureName);

	m_ArrowTextureName = "asset/texture/arrow.png";
	ManagerTexture::LoadTexture(m_ArrowTextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;
}

void HudBullet::Init()
{
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;
	m_Rotation = m_InitRotation;
}

void HudBullet::Uninit()
{
}

void HudBullet::Update()
{
}

void HudBullet::Draw()
{
	if (m_IsActive) {
		CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureName), m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
		
		Player* player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Player>(e3DObject);
		int missile_num = player->GetMissileNum();

		for (int i = 0; i < 3; i++) {
			float x = (float)(missile_num % 10);

			float px = (m_Position.x + 100.0f) - 30.0f * i;
			float py = m_Position.y - 95.0f;

			float tw = 1.0f / 10;//横10
			float th = 1.0f / 1;//縦１のスコアテクスチャ
			float tx = x * tw;
			float ty = 0.0f;


			Sprite2D::Draw2D(ManagerTexture::GetTexture(m_NumberTextureName), D3DXVECTOR3(px, py, 0.0f), D3DXVECTOR3(35.0f, 60.0f, 0.0f), m_Color, D3DXVECTOR2(tx, ty), D3DXVECTOR2(tw, th));
			missile_num /= 10;
		}

		int hp = player->GetHp();
		
		int wariai = (10 - hp) * 10;
		for (int i = 0; i < 2; i++) {
			float x = (float)(wariai % 10);

			float px = (m_Position.x + 100.0f) - 30.0f * i;
			float py = m_Position.y + 90.0f;

			float tw = 1.0f / 10;//横10
			float th = 1.0f / 1;//縦１のスコアテクスチャ
			float tx = x * tw;
			float ty = 0.0f;


			Sprite2D::Draw2D(ManagerTexture::GetTexture(m_NumberTextureName), D3DXVECTOR3(px, py, 0.0f), D3DXVECTOR3(35.0f, 60.0f, 0.0f), m_Color, D3DXVECTOR2(tx, ty), D3DXVECTOR2(tw, th));
			wariai /= 10;
		}

		int type = player->GetBulletType();

		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_ArrowTextureName), D3DXVECTOR3(m_Position.x - 180.0f, m_Position.y + (float)type * -95.0f, 0.0f), D3DXVECTOR3(35.0f, 35.0f, 0.0f), m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
		
	}
}
