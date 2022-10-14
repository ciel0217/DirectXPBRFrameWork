#pragma once
#include "FrameWork/Resources/CGameObject.h"

class CloseBlackBG : public CGameObject2D
{
private:
	float m_CloseSpeed;
	bool m_CanNext;

public:
	//�ݒ�̏�����
	void Config()override;
	//�p�����[�^�[�̏�����
	void Init()override;
	void Uninit()override;
	void Update()override;

	bool GetCanNext() { return m_CanNext; }
};