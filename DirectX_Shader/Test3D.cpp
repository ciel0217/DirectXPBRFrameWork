#include "Test3D.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Manager/ManagerInput.h"
#include "FrameWork/Resources/CShader.h"
#include <cmath>


void Test3D::Config()
{

	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");
	
	
	
	m_RenderQueue = eOpacity;

}

void Test3D::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;


	//m_Model->SetMetaric(.1f);
	//m_Model->SetSpecular(1.0f);
	//m_Model->SetRoughness(1.0f);

}

void Test3D::Uninit()
{
}

void Test3D::Update()
{

	
}

void Test3D::Load()
{
	m_ModelName = "asset/model/Sphere.obj";
	m_Model = ManagerModel::Load(m_ModelName);

}

void Test3D::Settt(float a, float b)
{
	m_Model->SetMetaric(a);
	m_Model->SetRoughness(b);
}


