#include "OperationPic.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Resources/Sprite.h"
#include "FrameWork/Manager/ManagerInput.h"


void OperationPic::Config()
{
	m_TextureNames[0] = "asset/texture/default.png";
	m_TextureNames[1] = "asset/texture/right.png";
	m_TextureNames[2] = "asset/texture/default.png";
	m_TextureNames[3] = "asset/texture/left.png";
	m_TextureNames[4] = "asset/texture/default.png";
	m_TextureNames[5] = "asset/texture/up.png";
	m_TextureNames[6] = "asset/texture/default.png";
	m_TextureNames[7] = "asset/texture/down.png";

	for (int i = 0; i < 8; i++) {
		ManagerTexture::LoadTexture(m_TextureNames[i]);
	}

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");

	m_RenderQueue = e2D;
}

void OperationPic::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_TextureName = m_TextureNames[0];
	m_TextureNameNext = m_TextureNames[1];
	m_Index = 1;
	m_FrameCount = 0.0f;
	m_StopCount = 0.0f;

	m_PageNumber = 0;
	m_IsActive = false;
}

void OperationPic::Uninit()
{
}

void OperationPic::Update()
{
	if (m_IsActive) {

		
		m_StopCount += 1.0f;

		if (m_StopCount > STOP_COUNT) {
			m_FrameCount += 1.0f;
		}

		if (m_FrameCount > NEXT_FRAME_COUNT) {
			m_Index++;
			m_Index %= 8;

			m_TextureName = m_TextureNameNext;
			m_TextureNameNext = m_TextureNames[m_Index];
			m_FrameCount = 0.0f;
			m_StopCount = 0.0f;
		}
		
	}
	
}

void OperationPic::Draw()
{
	if (m_IsActive) {
		CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());

		
		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureNameNext), m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureName), m_Position, m_Scale, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - m_FrameCount / NEXT_FRAME_COUNT), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
		
		
	}
}
