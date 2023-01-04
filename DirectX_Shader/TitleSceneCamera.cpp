#include "TitleSceneCamera.h"
#include "TitlePlayer.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "FrameWork/Manager/ManagerInput.h"

void TitleSceneCamera::Config()
{
	SetUpRenderer();
}

void TitleSceneCamera::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	
	m_CameraUp = D3DXVECTOR3(0.0f, 1.0f, .0f);
	m_LookAtPoint = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
	//m_LookAtPoint = D3DXVECTOR3(0.0f, 100.0f, 100.0f);
	m_count = 0.0f;
}

void TitleSceneCamera::Uninit()
{
}

void TitleSceneCamera::Update()
{
	TitlePlayer* player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<TitlePlayer>(e3DObject);
	
	if(player->GetPosition().z > 0.0f){
		m_LookAtPoint = player->GetPosition();
	}

	/*if (ManagerInput::GetInstance()->GetKeyboardPress(DIK_A)) {
		m_count += 0.1f;
		m_Position.x = m_LookAtPoint.x + 120.0f * sinf(m_count);
		m_Position.z = m_LookAtPoint.z + -120.0f * cosf(m_count);
	}*/



	SetVPCBuffer(m_Position, m_LookAtPoint, m_CameraUp);
}
