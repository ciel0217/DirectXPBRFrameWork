#pragma once
#include "FrameWork/Resources/CGameObject.h"

class Audio;

class AnimeExplosion : public CGameObject3D
{
private:
	std::string m_TextureName;
	MATERIAL_CBUFFER m_Material;

	D3DXVECTOR2 m_Uv;
	int m_FrameCount;
	D3DXVECTOR2 m_Wh;
	D3DXVECTOR2 m_CurrentUv;
	Audio* m_Audio;

	const int NEXT_ANIME_COUNT = 10;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};