#pragma once
#include "FrameWork/Resources/CGameObject.h"
#include "FrameWork/Resources/ParticleEmitter.h"


class Audio;
class TitlePlayer : public CGameObject3D
{
private:
	float m_Speed;
	int m_FrameCount;

	Audio* m_Audio;
	ParticleEmitter* m_ParticleEmitter;
	const float ROTATION_VALUE = 0.04f;
	const D3DXVECTOR3  UnitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3  UnitY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3  UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);


public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;

	void SetActive(bool active)override;
};