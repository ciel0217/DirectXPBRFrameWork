#include "PostProcessToneMap.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Manager/ManagerShader.h"
#include "../Manager/ManagerScene.h"
#include "../Resources/CShader.h"
#include "../Resources/SkyBox.h"

void PostProcessToneMap::Config()
{
	m_Shader = ManagerShader::GetShader("Shader/ToneMap.hlsl");
}

void PostProcessToneMap::Init()
{

}

void PostProcessToneMap::Uninit()
{
	delete m_Shader;
	m_MeshData.clear();
	m_IndexData.clear();
}

void PostProcessToneMap::Draw()
{
	CDxRenderer::GetRenderer()->SetVertexShader(m_Shader->GetShaderVS()->VertexShader.Get());
	CDxRenderer::GetRenderer()->SetInputLayout(m_Shader->GetShaderVS()->Layout.Get());
	CDxRenderer::GetRenderer()->SetPixelShader(m_Shader->GetShaderPS().Get());
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	DirectXTexture2D* tonemap = CDxRenderer::GetRenderer()->GetSceneTexture(eIntermediate);
	ID3D11ShaderResourceView* t0 = tonemap->GetSRV();
	
	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &t0);

	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);
	CDxRenderer::GetRenderer()->GetDeviceContext()->Draw(4, 0);
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);
	CDxRenderer::GetRenderer()->UnbindShaderResourceView(0);


}
