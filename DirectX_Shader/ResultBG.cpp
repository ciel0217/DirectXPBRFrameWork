#include "ResultBG.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "CloseBlackBG.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "TitleScene.h"
#include "FrameWork/Manager/audio.h"

void ResultBG::Config()
{
	m_TextureName = "asset/texture/resultbg.png";
	ManagerTexture::LoadTexture(m_TextureName);

	m_Shader = ManagerShader::GetShader("Shader/2DTexture.hlsl");
	m_RenderQueue = e2D;

	m_Audio = new Audio();
}

void ResultBG::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;

	m_SceneChange = false;
	m_Audio->Load("asset/music/return.wav");
}

void ResultBG::Uninit()
{
	m_Audio->StopAll();
	delete m_Audio;
}

void ResultBG::Update()
{
	if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_L) && !m_SceneChange) {
		ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->SetActive(true);
		m_Audio->Play();
		m_SceneChange = true;
	}

	if (m_SceneChange && ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->GetCanNext()) {
		ManagerScene::GetInstance()->ChangeScene<TitleScene>();
	}
}
