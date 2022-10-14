#include "Score.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Resources/Sprite.h"
#include "FrameWork/Manager/audio.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "Player.h"

int Score::m_Score;

void Score::Config()
{

	m_TextureName = "asset/texture/number.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");

	m_Audio = new Audio();
	m_Audio->Load("asset/music/MusMus-BGM-067.wav");

	m_RenderQueue = e2D;
}

void Score::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Velocity = m_InitVelocity;

	m_Audio->Play(true, 0.5f);
	m_Score = 0;

	m_Player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Player>(e3DObject);
}

void Score::Uninit()
{
	m_Audio->StopAll();
	delete m_Audio;
}

void Score::Update()
{
	if (!m_Player->GetActive())m_Audio->StopAll();
}

void Score::Draw()
{
	if (m_IsActive) {
		CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());

		int number = m_Score;
		for (int i = 0; i < SCORE_DIGIT; i++) {

			float x = (float)(number % 10);

			float px = m_Position.x - m_Scale.x * i;
			float py = m_Position.y;
			
			float tw = 1.0f / 10;//横10
			float th = 1.0f / 1;//縦１のスコアテクスチャ
			float tx = x * tw;
			float ty = 0.0f;


			Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureName), D3DXVECTOR3(px,py,0.0f), m_Scale, m_Color, D3DXVECTOR2(tx, ty), D3DXVECTOR2(tw, th));
			number /= 10;
		}
	}
}
