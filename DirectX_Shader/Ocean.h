#pragma once
#include "FrameWork/Resources/CGameObject.h"
#include "UserStruct.h"

class CBuffer;

class Ocean : public CGameObject3D 
{
private:
	OCEAN_CBUFFER m_CBufferValue;
	CBuffer* m_CBuffer;
	std::string m_NormalMapTextureName;
	ID3D11ShaderResourceView* m_NormalTexture;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};