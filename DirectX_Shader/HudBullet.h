#pragma once
#include "FrameWork/Resources/CGameObject.h"

class HudBullet : public CGameObject2D
{
private:
	std::string m_NumberTextureName;
	std::string m_ArrowTextureName;

public:
	//�ݒ�̏�����
	void Config()override;
	//�p�����[�^�[�̏�����
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};