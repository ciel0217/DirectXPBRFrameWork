#pragma once
#include "FrameWork/Resources/CGameObject.h"

class OpenBlackBG : public CGameObject2D
{
private:
	float m_OpenSpeed;
	bool m_FinishOpen;

public:
	//設定の初期化
	void Config()override;
	//パラメーターの初期化
	void Init()override;
	void Uninit()override;
	void Update()override;

	bool GetFinishOpen() { return m_FinishOpen; }
};