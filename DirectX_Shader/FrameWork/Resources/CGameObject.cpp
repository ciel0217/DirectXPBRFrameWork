#include "CGameObject.h"
#include "../LowLevel/CDxRenderer.h"
#include "Sprite.h"
#include "../Manager/ManagerTexture.h"
#include "../Manager/ManagerModel.h"
#include "CShader.h"
#include "CBuffer.h"


//2DのDraw
void CGameObject2D::Draw()
{
	if (m_IsActive) {
		CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
		CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
		CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
		Sprite2D::Draw2D(ManagerTexture::GetTexture(m_TextureName), m_Position, m_Scale, m_Color, D3DXVECTOR2(0.0f, 0.0f), D3DXVECTOR2(1.0f, 1.0f));
	}
}


CBuffer* CGameObject3D::m_WorldCBuffer = nullptr;
CBuffer* CGameObject3D::m_InverseWorldCBuffer = nullptr;

void CGameObject3D::SetUpGameObject()
{
	if (!m_WorldCBuffer) {
		m_WorldCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	}

	if (!m_InverseWorldCBuffer) {
		m_InverseWorldCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	}
}

void CGameObject3D::ReleaseGameObject()
{
	delete m_WorldCBuffer;
	delete m_InverseWorldCBuffer;
}

void CGameObject3D::Draw()
{
	if (m_IsActive) {
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
}
