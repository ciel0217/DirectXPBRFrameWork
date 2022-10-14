#include "Lose.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "enum.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "TitleScene.h"
#include "CloseBlackBG.h"
#include "FrameWork/Manager/audio.h"

void Lose::Config()
{
	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");

	m_TextureName = "asset/texture/black_lose.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_RenderQueue = e2D;

	m_Audio = new Audio();
}

void Lose::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_IsActive = false;
	m_Audio->Load("asset/music/zannense.wav");
}

void Lose::Uninit()
{
	m_Audio->StopAll();
	delete m_Audio;
}

void Lose::Update()
{
	if (m_IsActive) {
		m_Color.a += 0.05f;
		if (m_Color.a > 0.7f)m_Color.a = 0.7f;

		if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_L)) {
			ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->SetActive(true);
		}

		if (ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->GetCanNext()) {
			ManagerScene::GetInstance()->ChangeScene<TitleScene>();
		}
	}

}

void Lose::SetActive(bool active)
{
	if(!m_IsActive)m_Audio->Play();
	m_IsActive = active;
}
