#include "GameSceneCamera.h"
#include "FrameWork/Resources/CBuffer.h"
#include "FrameWork/Manager/ManagerInput.h"
#include <cmath>
#include "FrameWork/Resources/CLight.h"
#include "Player.h"
#include "FrameWork/Manager/ManagerScene.h"

#include "Math/Quaternion.h"

void GameSceneCamera::Init()
{
	m_Position = D3DXVECTOR3(.0f, 10.0f, -10.0f);
	m_Rotation = D3DXQUATERNION(.0f, .0f, .0f, 1.0f);
	m_CameraUp = D3DXVECTOR3(0.0f, 1.0f, .0f);
	m_LookAtPoint = D3DXVECTOR3(0.0f, 0.0f, 70.0f);
}

void GameSceneCamera::Uninit()
{
}

void GameSceneCamera::Update()
{
	Player* player = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Player>(e3DObject);
	D3DXQUATERNION player_qua = player->GetRotation();

	D3DXVECTOR3 player_z = Transform(UnitZ, player_qua);
	D3DXVECTOR3 player_y = Transform(UnitY, player_qua);
	D3DXVec3Normalize(&player_z, &player_z);
	D3DXVec3Normalize(&player_y, &player_y);

	//m_Position = player->GetPosition() + player_z * 60.0f;
	m_Position = player->GetPosition() + player_z * -60.0f + player_y * 20.0f;
	m_CameraUp = player_y;
	m_LookAtPoint = player->GetPosition() + player_z * 200.0f;

	SetVPCBuffer(m_Position, m_LookAtPoint, m_CameraUp);
}
