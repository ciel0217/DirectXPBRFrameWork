#include "Pause.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "enum.h"
#include "FrameWork/Resources/Sprite.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "TitleScene.h"
#include "CloseBlackBG.h"
#include "Lose.h"

void Pause::Config()
{
	m_TextureName = "asset/texture/black.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_GameTextureName = "asset/texture/gameback.png";
	ManagerTexture::LoadTexture(m_GameTextureName);

	m_TitleTextureName = "asset/texture/titleback.png";
	ManagerTexture::LoadTexture(m_TitleTextureName);

	m_ArrowTextureName = "asset/texture/arrow.png";
	ManagerTexture::LoadTexture(m_ArrowTextureName);

	m_RenderQueue = e2D;
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
}

void Pause::Init()
{
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;

	m_GamePosition = D3DXVECTOR3(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f + 100.0f, 0.0f);
	m_TitlePosition = D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f + 200.0f, 0.0f);

	m_ArrowPosition[0] = m_GamePosition;
	m_ArrowPosition[0].x -= 120.0f;
	m_ArrowPosition[1] = m_TitlePosition;
	m_ArrowPosition[1].x -= 120.0f;
	m_ArrowPoint = 0;
	m_IsActive = false;

	m_Lose = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Lose>(e2DObject);
}

void Pause::Uninit()
{
}

void Pause::Update()
{
	if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_SPACE) && !m_Lose->GetActive()) {
		m_IsActive = !m_IsActive;
		std::list<CommonProcess*>* list = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObjectAll();

		for (int i = 0; i < eMaxLayer; i++) {
			for (auto game : list[i]) {
				game->SetPause(m_IsActive);
			}
		}
	}

	if (m_IsActive) {
		if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_W)) {
			m_ArrowPoint = 0;
		}
		if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_S)) {
			m_ArrowPoint = 1;
		}

		if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_L)) {
			if (m_ArrowPoint == 0) {
				m_IsActive = false;
				std::list<CommonProcess*>* list = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObjectAll();
				for (int i = 0; i < eMaxLayer; i++) {
					for (auto game : list[i]) {
						game->SetPause(m_IsActive);
					}
				}
			}
			else {
				ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->SetActive(true);
			}
		}

		if (ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->GetCanNext()) {
			ManagerScene::GetInstance()->ChangeScene<TitleScene>();
		}

		
	}
}

void Pause::Draw()
{
	if (m_IsActive) {
		CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());


		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureName), m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_GameTextureName), m_GamePosition, D3DXVECTOR3(200.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TitleTextureName), m_TitlePosition, D3DXVECTOR3(200.0f, 75.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_ArrowTextureName), m_ArrowPosition[m_ArrowPoint], D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

	}
}
