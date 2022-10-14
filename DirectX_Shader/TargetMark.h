#pragma once
#include "FrameWork/Resources/CGameObject.h"


class Audio;

class TargetMark : public CGameObject3D
{
private:
	std::string m_TextureName;
	MATERIAL_CBUFFER m_Material;
	CGameObject3D* m_Target;
	CGameObject3D* m_PreTarget;
	Audio* m_Audio;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetTarget(CGameObject3D* target) { m_Target = target; }
	
};