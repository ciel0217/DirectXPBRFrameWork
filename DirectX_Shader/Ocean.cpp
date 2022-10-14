#include "Ocean.h"
#include "FrameWork/Manager/ManagerTexture.h"
#include "FrameWork/Resources/CBuffer.h"
#include "FrameWork/LowLevel/CDxRenderer.h"
#include "FrameWork/Resources/CShader.h"

void Ocean::Config()
{
	m_NormalMapTextureName = "asset/texture/NormalMap.png";
	m_NormalTexture = ManagerTexture::LoadTexture(m_NormalMapTextureName);

	m_ModelName = "asset/model/plane.obj";
	m_Model = ManagerModel::Load(m_ModelName);
	D3DXCOLOR basecolor = D3DXCOLOR(0.1373f, 0.6627f, 0.8784f, 1.0f);
	//D3DXCOLOR basecolor = D3DXCOLOR(0.0196f, 0.1349f, 0.4745f, 1.0f);
	m_Model->SetBaseColor(basecolor);
	m_Model->SetNormalTexture(m_NormalTexture);
	m_Model->SetMetaric(1.0f);
	m_Model->SetRoughness(0.2f);

	m_Shader = ManagerShader::GetShader("Shader/Ocean.hlsl");

	m_CBufferValue.Time = 0.0f;
	m_CBufferValue.WaveSpeed = 0.1f;
	m_CBufferValue.MixColor = D3DXCOLOR(0.0353f, 0.2392f, 0.3216f, 1.0f);
	//m_CBufferValue.MixColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_CBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(OCEAN_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr));
}

void Ocean::Init()
{
	m_Position = m_InitPosition;
	m_Rotation = m_InitRotation;
	m_Scale = m_InitScale;
}

void Ocean::Uninit()
{
}

void Ocean::Update()
{
	m_CBufferValue.Time += 0.1f;
}

void Ocean::Draw()
{
	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX mtxWorld, mtxInverseWorld;

	D3DXMatrixIdentity(&mtxWorld);
	D3DXMatrixIdentity(&mtxInverseWorld);

	//サイズ
	D3DXMatrixScaling(&mtxScl, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	//回転
	D3DXMatrixRotationQuaternion(&mtxRot, &m_Rotation);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//移動
	D3DXMatrixTranslation(&mtxTranslate, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);

	D3DXMatrixInverse(&mtxInverseWorld, nullptr, &mtxWorld);

	D3DXMatrixTranspose(&mtxWorld, &mtxWorld);
	D3DXMatrixTranspose(&mtxInverseWorld, &mtxInverseWorld);

	m_WorldCBuffer->UpdateBuffer(&mtxWorld);
	m_WorldCBuffer->PSSetCBuffer(0);
	m_WorldCBuffer->VSSetCBuffer(0);

	m_InverseWorldCBuffer->UpdateBuffer(&mtxInverseWorld);
	m_InverseWorldCBuffer->PSSetCBuffer(6);
	m_InverseWorldCBuffer->VSSetCBuffer(6);

	m_CBuffer->UpdateBuffer(&m_CBufferValue);
	m_CBuffer->PSSetCBuffer(4);

	//カリングなし
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);

	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	//objモデルの描画
	ManagerModel::Draw(m_Model);

	//カリングを元に戻す
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);

	CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);
}
