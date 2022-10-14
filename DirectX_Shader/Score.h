#pragma once
#include "FrameWork/Resources/CGameObject.h"
 
class Audio;
class Player;

class Score : public CGameObject2D
{
private:
	static int m_Score;

	Audio* m_Audio;
	Player* m_Player;
	const int SCORE_DIGIT = 3;
public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;

	void SetScore(int score) { m_Score += score; }
	static int GetScore(){ return m_Score; }
};