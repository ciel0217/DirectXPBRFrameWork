#pragma once
#include "CommonProcess.h"
#include "ObjectTag.h"
#include <list>
#include "../../enum.h"
#include "CPostProcess.h"
#include "../RenderingPass/DeferredRenderer.h"
#include "SkyBox.h"
#include "../RenderingPass/PostProcessToneMap.h"
#include "../Manager/ManagerLight.h"
#include "../RenderingPass/CameraRenderer.h"


class CBuffer;
class CLight;

class Camera : public CommonProcess, public CameraRenderer {
protected:
	D3DXVECTOR3 m_LookAtPoint;
	D3DXVECTOR3 m_CameraUp;

	const D3DXVECTOR3  UnitX = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	const D3DXVECTOR3  UnitY = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	const D3DXVECTOR3  UnitZ = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

public:

	Camera() { }
	virtual ~Camera() {};

	virtual void Config()override { SetUpRenderer(); }
	virtual void Init()override {};
	virtual void Uninit()override {};
	virtual void Update()override { SetVPCBuffer(m_Position, m_LookAtPoint, m_CameraUp); }
	virtual void Draw()override {};


	D3DXVECTOR3 GetCameraLookAtPoint() { return m_LookAtPoint; }
	D3DXVECTOR3 GetCameraUp() { return m_CameraUp; }
	

};