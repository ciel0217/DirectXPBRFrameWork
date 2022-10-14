#pragma once
#include "FrameWork/Resources/CGameObject.h"

class CloseBlackBG : public CGameObject2D
{
private:
	float m_CloseSpeed;
	bool m_CanNext;

public:
	//設定の初期化
	void Config()override;
	//パラメーターの初期化
	void Init()override;
	void Uninit()override;
	void Update()override;

	bool GetCanNext() { return m_CanNext; }
};