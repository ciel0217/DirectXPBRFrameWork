#include "ResultSceneCamera.h"



void ResultSceneCamera::Config()
{
	SetUpRenderer();
}

void ResultSceneCamera::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;

	m_CameraUp = D3DXVECTOR3(0.0f, 1.0f, .0f);
	m_LookAtPoint = D3DXVECTOR3(-10000.0f, 0.0f, 0.0f);
}

void ResultSceneCamera::Uninit()
{
}
