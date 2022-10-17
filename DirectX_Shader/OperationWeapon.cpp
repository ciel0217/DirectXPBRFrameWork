#include "OperationWeapon.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "OpetationExplain.h"
#include "OperationPic.h"

void OperationWeapon::Config()
{
	m_TextureName = "asset/texture/esc_2.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");

	m_RenderQueue = e2D;
}

void OperationWeapon::Init()
{
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;

	m_IsActive = false;
}

void OperationWeapon::Uninit()
{
}

void OperationWeapon::Update()
{
	if (m_IsActive) {
		if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_A)) {
			m_IsActive = false;
			ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<OperationExplain>(e2DObject)->SetActive(true);
			ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<OperationPic>(e2DObject)->SetActive(true);

		}
	}
}
