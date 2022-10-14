#include "Player.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "quaternion.h"
#include "FrameWork/Resources/BoxCollision.h"
#include "Bullet.h"
#include "FrameWork/Manager/ManagerScene.h"
#include "DamageEffect.h"
#include "Rocket.h"
#include "EnemyManager.h"
#include "Enemy.h"
#include "GameSceneCamera.h"
#include "TargetMark.h"
#include "AnimeExplosion.h"
#include "ResultScene.h"
#include "FrameWork/Manager/audio.h"
#include "Lose.h"
#include "PressEnterKey.h"

CGameObject3D * Player::CalcTargetObject()
{
	float min_length = 100000.0f;
	CGameObject3D* target = nullptr;
	std::list<CGameObject3D*> enemy_list = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<EnemyManager>(e3DObject)->GetEnemyList();
	for (auto enemy : enemy_list) {
		if (!enemy)continue;
		
		if (CanTarget(enemy)) {

			D3DXVECTOR3 dir = enemy->GetPosition() - m_Position;
			float length = D3DXVec3Length(&dir);

			if (length < min_length) {
				min_length = length;
				target = enemy;
			}
		}

	}
	return target;
}

bool Player::CanTarget(CGameObject3D* object)
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX mtxWorld;
	D3DXVECTOR3 target_pos = object->GetPosition();
	D3DXMatrixIdentity(&mtxWorld);


	Camera * camera = ManagerScene::GetInstance()->GetCurrentScene()->GetCurrentMainCamera();
	D3DXVECTOR3 up = camera->GetCameraUp();
	D3DXVECTOR3 lookat = camera->GetCameraLookAtPoint();


	D3DXMATRIX mtxProjection, mtxView;
	D3DXMatrixLookAtLH(&mtxView, &m_Position, &lookat, &up);

	D3DXMatrixPerspectiveFovLH(&mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.5f, 10000.0f);


	D3DXVECTOR4 result;

	//world viewïœä∑
	result = D3DXVECTOR4(
		mtxView._11 * target_pos.x + mtxView._21 * target_pos.y + mtxView._31 * target_pos.z + mtxView._41 * 1.0f,
		mtxView._12 * target_pos.x + mtxView._22 * target_pos.y + mtxView._32 * target_pos.z + mtxView._42 * 1.0f,
		mtxView._13 * target_pos.x + mtxView._23 * target_pos.y + mtxView._33 * target_pos.z + mtxView._43 * 1.0f,
		mtxView._14 * target_pos.x + mtxView._24 * target_pos.y + mtxView._34 * target_pos.z + mtxView._44 * 1.0f
	);
	//view projectionïœä∑
	result = D3DXVECTOR4(
		mtxProjection._11 * result.x + mtxProjection._21 * result.y + mtxProjection._31 * result.z + mtxProjection._41 * result.w,
		mtxProjection._12 * result.x + mtxProjection._22 * result.y + mtxProjection._32 * result.z + mtxProjection._42 * result.w,
		mtxProjection._13 * result.x + mtxProjection._23 * result.y + mtxProjection._33 * result.z + mtxProjection._43 * result.w,
		mtxProjection._14 * result.x + mtxProjection._24 * result.y + mtxProjection._34 * result.z + mtxProjection._44 * result.w
	);

	//(xÇÃèåè && yÇÃèåè && zÇÃèåè) = éãêçë‰ÇÃÇ»Ç©Ç…Ç†ÇÈ
	if ((-result.w < result.x && result.x < result.w) && (-result.w < result.y && result.y < result.w) && (-result.w < result.z && result.z < result.w))
		return true;


	return false;
}

void Player::Config()
{
	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");
	m_ModelName = "asset/model/jet.obj";
	m_Model = ManagerModel::Load(m_ModelName);
	m_Model->SetMetaric(.5f);
	m_Model->SetRoughness(1.0f);

	m_Tag = TAG_PLAYER;

	m_Collision = new BoxCollision(this, D3DXVECTOR3(50.0f,50.0f,50.0f));
	m_Collision->SetTagMask(TAG_ENEMY);
	
	m_TargetMark = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<TargetMark>(e3DObject);

	m_Audio = new Audio();
	m_Audio->Load("asset/music/plane_game.wav");
	m_BulletSE = new Audio();
	m_BulletSE->Load("asset/music/bullet1.wav");
	m_RocketSE = new Audio();
	m_RocketSE->Load("asset/music/launcher1.wav");
}

void Player::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
	m_Rpm = 0;
	m_Speed = 10.0f;
	m_HP = 10;
	m_BulletType = eBullet;
	m_MissileNum = 100;
	m_DeathCount = 0;

	m_Audio->Play(true, 0.2f);
}

void Player::Uninit()
{
	m_Audio->StopAll();
	m_BulletSE->StopAll();
	m_RocketSE->StopAll();

	delete m_Audio;
	delete m_BulletSE;
	delete m_RocketSE;

}

void Player::Update()
{
	if (m_IsActive) {
		if (!m_IsPause) {
			m_Rpm++;
			m_TargetMark->SetTarget(nullptr);

			if (m_Position.y <= 5.0f && m_IsActive) {
				m_IsActive = false;

				m_Explo = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<AnimeExplosion>(e3DObject);
				m_Explo->SetInitPosition(m_Position);
				m_Explo->SetInitScale(D3DXVECTOR3(200.0f, 200.0f, 0.0f));
				m_Explo->Config();
				m_Explo->Init();

				m_Audio->StopAll();
				m_BulletSE->StopAll();
				m_RocketSE->StopAll();
				
			}

			if (m_BulletType == eBullet) {
				if (ManagerInput::GetInstance()->GetKeyboardPress(DIK_L)) {
					if (m_Rpm > 5) {
						m_BulletSE->Play();

						Bullet* bullet = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<Bullet>(e3DObject);

						D3DXVECTOR3 axisy = Transform(UnitY, m_Rotation);
						D3DXVECTOR3 axisz = Transform(UnitZ, m_Rotation);
						bullet->SetInitPosition(m_Position - axisy * -10.0f);
						bullet->SetInitScale(D3DXVECTOR3(5.0f, 5.0f, 1.0f));
						bullet->SetInitVelocity(axisz);
						bullet->Config();
						bullet->Init();
						m_Rpm = 0;
					}
				}
			}
			else {
				CGameObject3D* target = CalcTargetObject();
				m_TargetMark->SetTarget(target);
				if (m_MissileNum > 0) {
					if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_L)) {
						m_RocketSE->Play();
						Rocket* bullet = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<Rocket>(e3DObject);
						D3DXVECTOR3 axisy = Transform(UnitY, m_Rotation);
						D3DXVECTOR3 axisz = Transform(UnitZ, m_Rotation);
						bullet->SetInitPosition(m_Position + axisy * -50.0f);
						bullet->SetInitScale(D3DXVECTOR3(100.0f, 100.0f, 100.0f));
						bullet->SetTargetObject(target);
						bullet->SetInitVelocity(axisz);
						bullet->Config();
						bullet->Init();
						m_Rpm = 0;
						m_MissileNum--;
					}
				}

			}


			if (ManagerInput::GetInstance()->GetKeyboardPress(DIK_S))
			{
				D3DXVECTOR3 axis = Transform(UnitX, m_Rotation);
				//D3DXQuaternionRotationAxis(&g_Player.rot, &axis, VALUE_ROTATE);
				m_Rotation = RotateAxis(axis, ROTATION_VALUE, m_Rotation);

			}
			if (ManagerInput::GetInstance()->GetKeyboardPress(DIK_W))
			{
				D3DXVECTOR3 axis = Transform(UnitX, m_Rotation);
				//D3DXQuaternionRotationAxis(&g_Player.rot, &axis, -1.0f * VALUE_ROTATE);
				m_Rotation = RotateAxis(axis, -1.0f *  ROTATION_VALUE, m_Rotation);
				/*g_Player.spd = VALUE_MOVE;
				g_Rot = D3DX_PI / 2;*/
			}
			if (ManagerInput::GetInstance()->GetKeyboardPress(DIK_A))
			{
				/*D3DXVECTOR3 axisy = Transform(UnitY, g_Player.rot);
				g_Player.rot = RotateAxis(axisy, -1.0f * VALUE_ROTATE, g_Player.rot);*/
				D3DXVECTOR3 axisz = Transform(UnitZ, m_Rotation);
				m_Rotation = RotateAxis(axisz, ROTATION_VALUE, m_Rotation);

				/*g_Player.spd = VALUE_MOVE;
				g_Rot = 0.0f;*/
			}
			if (ManagerInput::GetInstance()->GetKeyboardPress(DIK_D))
			{
				///*D3DXVECTOR3 axisy = Transform(UnitY, g_Player.rot);
				//g_Player.rot = RotateAxis(axisy,  VALUE_ROTATE, g_Player.rot);*/
				D3DXVECTOR3 axisz = Transform(UnitZ, m_Rotation);
				m_Rotation = RotateAxis(axisz, -1.0f * ROTATION_VALUE, m_Rotation);

			}

			if (ManagerInput::GetInstance()->GetKeyboardTrigger(DIK_I))
			{
				m_BulletType = (PlayerBulletType)(1 - m_BulletType);

			}

			D3DXVECTOR3 front = Transform(UnitZ, m_Rotation);
			m_Position += front * m_Speed;

			m_Collision->UpdateCollision();
		}
	}
	else {
		m_DeathCount++;
		if (m_DeathCount > 120) {
			ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Lose>(e2DObject)->SetActive(true);
			ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<PressLKey>(e2DObject)->SetActive(true);
		}
	}
}

void Player::OnCollisionEnter(CGameObject3D * collider)
{
	
	if (m_IsActive) {
		DamageEffect* effect = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<DamageEffect>(e2DObject);
		effect->SetActive(true);
		effect->Init();
		
		m_Explo = ManagerScene::GetInstance()->GetCurrentScene()->AddGameObject<AnimeExplosion>(e3DObject);
		m_Explo->SetInitPosition(m_Position);
		m_Explo->SetInitScale(D3DXVECTOR3(200.0f, 200.0f, 0.0f));
		m_Explo->Config();
		m_Explo->Init();
		m_HP--;
		if (m_HP <= 0)m_HP = 0;

		if (m_HP <= 0) {
			m_HP = 0;
			m_IsActive = false;
			//m_IsDestroy = true;

			m_Audio->StopAll();
			m_BulletSE->StopAll();
			m_RocketSE->StopAll();


		}
	}

	
}
