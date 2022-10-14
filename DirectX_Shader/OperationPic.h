#pragma once
#include "FrameWork/Resources/CGameObject.h"

class OperationPic : public CGameObject2D
{
private:
	std::string m_TextureNames[8];
	std::string m_TextureNameNext;
	std::string m_TextureWeapon;
	int m_Index;
	float m_FrameCount;
	float m_StopCount;
	int m_PageNumber;

	const float NEXT_FRAME_COUNT = 200.0f;
	const float STOP_COUNT = 180.0f;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};