#pragma once
#include "FrameWork/Resources/CGameObject.h"

class OperationPic;
class OperationExplain;
class OperationWeapon;
class Audio;


class Operation : public CGameObject2D
{
private:
	std::string m_ArrowTextureName;
	OperationPic* m_OperationPic;
	OperationExplain* m_OperationExplain;
	OperationWeapon* m_OperationWeapon;
	bool m_IsOpen;
	int m_ArrowPoint;
	Audio* m_Audio;


public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
};