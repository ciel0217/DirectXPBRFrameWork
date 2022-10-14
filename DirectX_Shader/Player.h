#pragma once
#include "FrameWork/Resources/CGameObject.h"
#include "enum.h"

class TargetMark;
class AnimeExplosion;
class Audio;

class Player : public CGameObject3D
{
private:
	float m_Speed;
	int m_HP;
	
	int m_Rpm;
	int m_MissileNum;
	int m_DeathCount;

	PlayerBulletType m_BulletType;
	TargetMark* m_TargetMark;
	AnimeExplosion* m_Explo;
	Audio* m_Audio;
	Audio* m_BulletSE;
	Audio* m_RocketSE;

	const float ROTATION_VALUE = 0.02f;
	const D3DXVECTOR3  UnitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3  UnitY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3  UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	const float MAX_RPM = 5;

	CGameObject3D* CalcTargetObject();
	bool CanTarget(CGameObject3D* object);
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;

	void OnCollisionEnter(CGameObject3D* collider)override;
	int GetMissileNum() { return m_MissileNum; }
	int GetHp() { return m_HP; }
	PlayerBulletType GetBulletType() { return m_BulletType; }
};