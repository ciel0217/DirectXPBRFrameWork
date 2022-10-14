#pragma once
#include "FrameWork/Resources/CGameObject.h"

class Audio;

class ResultScore : public CGameObject2D
{
private:
	int m_Score;
	float m_FrameCount = 0.0f;

	bool m_IsDon;
	Audio* m_DramRoll;
	Audio* m_Don;
	const float PRINT_SCORE_MAX_FRAME = 300.0f;
	const int SCORE_DIGIT = 3;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};