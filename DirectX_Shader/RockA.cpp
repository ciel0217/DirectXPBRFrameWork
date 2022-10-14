#include "RockA.h"

void RockA::Config()
{
	m_ModelName = "asset/model/RockSet05A.obj";
	m_Model = ManagerModel::Load(m_ModelName);

	m_Shader = ManagerShader::GetShader("Shader/shader.hlsl");
}

void RockA::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
}

void RockA::Uninit()
{
}

void RockA::Update()
{
}
