#include "TitlePlayer.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "Math/Quaternion.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "CloseBlackBG.h"
#include "TestScene.h"
#include "FrameWork/Manager/audio.h"


void TitlePlayer::Config()
{
	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");
	m_ModelName = "asset/model/jet.obj";
	m_Model = ManagerModel::Load(m_ModelName);
	m_Model->SetMetaric(0.5f);
	m_Model->SetRoughness(1.0f);

	m_Audio = new Audio();
	m_Audio->Load("asset/music/plane.wav");
}

void TitlePlayer::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Speed = 5.0f;
	m_IsActive = false;

	m_ParticleEmitter = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<ParticleEmitter>(e3DObject);

	m_ParticleEmitter->SetParent(this);
	m_ParticleEmitter->SetInitPosition(m_Position);
	m_ParticleEmitter->SetLimitLifeTime(200);
	m_ParticleEmitter->SetGenerateTime(6);
	m_ParticleEmitter->SetParticleScale(D3DXVECTOR3(15.0f, 15.0f, 15.0f));
	m_ParticleEmitter->SetSpeed(0.3f);
	m_ParticleEmitter->SetColor(D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f));
	m_ParticleEmitter->SetTextureName("asset/texture/index.png");
	m_ParticleEmitter->Config();
	m_ParticleEmitter->Init();
}

void TitlePlayer::Uninit()
{
	m_Audio->StopAll();
	delete m_Audio;
}

void TitlePlayer::Update()
{
	if (m_IsActive) {
		m_FrameCount++;
		if (m_FrameCount > 40 && m_FrameCount < 50)
		{

			D3DXVECTOR3 axis = Transform(UnitX, m_Rotation);
			m_Rotation = RotateAxis(axis, -1.0f *  ROTATION_VALUE, m_Rotation);
			
		}
		else if (m_FrameCount > 120) {
			ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->SetActive(true);
		}

		if (ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<CloseBlackBG>(e2DObject)->GetCanNext()) {
			ManagerScene::GetInstance()->ChangeScene<TestScene>();
		}


		D3DXVECTOR3 front = Transform(UnitZ, m_Rotation);
		m_Position += front * m_Speed;
	}
}

void TitlePlayer::SetActive(bool active)
{
	m_Audio->Play();
	m_IsActive = active;

	
}
