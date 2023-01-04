#pragma once
#include "CommonProcess.h"
#include <vector>


class Particle;
class CGameObject3D;
class ParticleEmitter : public CommonProcess
{
private:
	std::vector<Particle*> m_ParticleList;
	CGameObject3D* m_Parent;
	
	int m_FrameCount;
	int m_GenerateTime = 10;
	int m_CoolTime = 1;
	int m_ParticleNum = 1;

	std::string m_TextureName;
	int m_LimitLifeTime = 10;
	float m_Speed = 0;

	D3DXVECTOR3 m_ParticleScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_ParticleVelocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXQUATERNION m_ParticleRotation = D3DXQUATERNION(0.0f, 0.0f, 0.0f, 1.0f);
	D3DXCOLOR m_ParticleColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	D3DXVECTOR3 m_AddScale = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 m_AddAngle = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXCOLOR   m_AddColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	const D3DXVECTOR3  UnitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3  UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetSpeed(float speed) { m_Speed = speed; }
	void SetParticleScale(D3DXVECTOR3 scale) { m_ParticleScale = scale; }
	void SetParticleVelocity(D3DXVECTOR3 velo) { m_ParticleVelocity = velo; }
	void SetParticleRotation(D3DXQUATERNION qua) { m_ParticleRotation = qua; }
	void SetColor(D3DXCOLOR color) { m_ParticleColor = color; }
	void SetAddScale(D3DXVECTOR3 scale) { m_AddScale = scale; }
	void SetAddAngle(D3DXVECTOR3 angle) { m_AddAngle = angle; }
	void SetAddColor(D3DXCOLOR color) { m_AddColor = color; }

	void SetLimitLifeTime(int time) { m_LimitLifeTime = time; }
	void SetTextureName(std::string name) { m_TextureName = name; }

	void SetGenerateTime(int time) { m_GenerateTime = time; }
	void SetParent(CGameObject3D* parent) { m_Parent = parent; }
};