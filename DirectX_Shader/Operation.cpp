#include "Operation.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "OperationPic.h"
#include "OperationWeapon.h"
#include "OpetationExplain.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "TitlePlayer.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"
#include "FrameWork/Resources/CShader.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/Sprite.h"
#include "PressEnterKey.h"
#include "TitleLogo.h"
#include "Sousasetumei.h"
#include "FrameWork/Manager/audio.h"

void Operation::Config()
{
	m_TextureName = "asset/texture/black.png";
	m_ArrowTextureName = "asset/texture/arrow.png";
	ManagerTexture::LoadTexture(m_TextureName);
	ManagerTexture::LoadTexture(m_ArrowTextureName);
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;

	m_Audio = new Audio();

}

void Operation::Init()
{
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;

	m_OperationPic = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<OperationPic>(e2DObject);
	m_OperationExplain = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<OperationExplain>(e2DObject);
	m_OperationWeapon = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<OperationWeapon>(e2DObject);

	m_IsOpen = false;
	m_ArrowPoint = 0;

	m_Audio->Load("asset/music/return.wav");
}

void Operation::Uninit()
{
	m_Audio->StopAll();
	delete m_Audio;
}

void Operation::Update()
{
	if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_Q)) {
		if (m_IsOpen) {
			m_OperationExplain->SetActive(false);
			m_OperationPic->SetActive(false);
			m_OperationWeapon->SetActive(false);
		}
		else {
			m_OperationExplain->SetActive(true);
			m_OperationPic->SetActive(true);
		}

		m_IsOpen = !m_IsOpen;
	}

	if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_D)) {
		if (m_IsOpen) {
			m_OperationExplain->SetActive(false);
			m_OperationPic->SetActive(false);
			m_OperationWeapon->SetActive(true);

			m_ArrowPoint = 1;
		}
	}

	if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_A)) {
		if (m_IsOpen) {
			m_OperationExplain->SetActive(true);
			m_OperationPic->SetActive(true);
			m_OperationWeapon->SetActive(false);

			m_ArrowPoint = 0;
		}
	}

	if (m_IsActive) {
		if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_L)) {
			if (!m_IsOpen) {
				ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<TitlePlayer>(e3DObject)->SetActive(true);
				ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<PressLKey>(e2DObject)->SetActive(false);
				ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Sousasetumei>(e2DObject)->SetActive(false);
				ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<TitleLogo>(e2DObject)->SetActive(false);
				m_Audio->Play();
				m_IsActive = false;
			}
		}
	}
}

void Operation::Draw()
{
	if (m_IsOpen) {
		CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureName), m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));

		if (m_ArrowPoint == 0) {
			Sprite2D::Draw2D(ManagerTexture::GetTexture(m_ArrowTextureName), D3DXVECTOR3(SCREEN_WIDTH - 50.0f, SCREEN_HEIGHT/2.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f,1.0f,1.0f,1.0f), D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
		}
		else {
			Sprite2D::Draw2D(ManagerTexture::GetTexture(m_ArrowTextureName), D3DXVECTOR3(50.0f, SCREEN_HEIGHT / 2.0f, 0.0f), D3DXVECTOR3(50.0f, 50.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f), D3DXVECTOR2(-1.0f, 1.0f));
		}
	}
}
