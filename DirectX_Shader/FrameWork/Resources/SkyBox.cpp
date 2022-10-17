#include "SkyBox.h"
#include "../Manager/ManagerShader.h"
#include "DevelopEnum.h"
#include "../LowLevel/CDxRenderer.h"
#include "CShader.h"
#include "CCubeMap.h"
#include "CBuffer.h"
#include "../Resources/Camera.h"
#include "../Manager/ManagerScene.h"

void SkyBox::Config()
{

	

}

void SkyBox::Init()
{
	m_Color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Position = m_InitPosition;
	m_Scale = m_InitScale;
	m_Rotation = m_InitRotation;

	m_Camera = ManagerScene::GetInstance()->GetCurrentScene()->GetGameObject<Camera>(e3DObject);
}

void SkyBox::Uninit()
{

	delete m_BrdfLUTTexture;
	delete m_CubeMap;
	delete m_IrradianceMap;
	delete m_PreFilterMap;


	delete m_RoughnessCBuffer;
}

void SkyBox::Update()
{
	if (m_Camera) {
		m_Position.x = m_Camera->GetPosition().x;
		m_Position.z = m_Camera->GetPosition().y;
	}
}

void SkyBox::DrawByCubeMap()
{

	m_ModelName = "asset/model/SkyBox.obj";
	m_Model = ManagerModel::Load(m_ModelName);

	m_Shader = ManagerShader::GetShader("Shader/DefaultModel.hlsl");
	m_ShaderCubeMap = ManagerShader::GetShader("Shader/CubeMap.hlsl");
	m_ShaderIrradianceMap = ManagerShader::GetShader("Shader/IrradianceMap.hlsl");
	m_ShaderPreFilterMap = ManagerShader::GetShader("Shader/SpecularMap.hlsl");
	m_ShaderBrdfLUT = ManagerShader::GetShader("Shader/BrdfLUT.hlsl");

	m_RenderQueue = DrawObjectRenderQueue::eSkyBox;

	m_CubeMap = new CCubeMap(1);
	m_CubeMap->Init();

	m_IrradianceMap = new CCubeMap(1);
	m_IrradianceMap->Init();

	m_PreFilterMap = new CCubeMap(6);
	m_PreFilterMap->Init();

	m_BrdfLUTTexture = DirectXTexture2D::CreateTexture(
		512, 512, DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	m_BrdfLUTTexture->CreateSRV(DXGI_FORMAT_R16G16B16A16_FLOAT);
	m_BrdfLUTTexture->CreateRTV(DXGI_FORMAT_R16G16B16A16_FLOAT);


	m_RoughnessCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DXCOLOR), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	m_WorldCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DXMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr));

	D3DXMATRIX mtxScl, mtxRot, mtxTranslate;
	D3DXMATRIX mtxWorld;

	D3DXMatrixIdentity(&mtxWorld);

	m_Position = m_InitPosition;
	m_Scale = D3DXVECTOR3(200.0f,200.0f,200.0f);
	m_Rotation = m_InitRotation;

	//サイズ
	D3DXMatrixScaling(&mtxScl, m_Scale.x, m_Scale.y, m_Scale.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxScl);

	//回転
	D3DXMatrixRotationQuaternion(&mtxRot, &m_Rotation);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

	//移動
	D3DXMatrixTranslation(&mtxTranslate, m_Position.x, m_Position.y, m_Position.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTranslate);
	D3DXMatrixTranspose(&mtxWorld, &mtxWorld);

	m_WorldCBuffer->UpdateBuffer(mtxWorld);
	m_WorldCBuffer->VSSetCBuffer(0);
	//CDxRenderer::GetRenderer()->SetWorldMatrix(&mtxWorld);
	//カリングなし
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);

	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	

	//環境マップ作成
	for (int i = 0; i < 6; i++) {

		CDxRenderer::GetRenderer()->GetDeviceContext()->ClearRenderTargetView(m_CubeMap->GetRenderTargetViewPointer(i), ClearColor);
	}

	m_CubeMap->SetViewPort();
	for (int i = 0; i < 6; i++) {
		
		CDxRenderer::GetRenderer()->GetDeviceContext()->OMSetRenderTargets(1, m_CubeMap->GetRenderTargetViewPointerPointer(i), nullptr);

		m_CubeMap->SetEnvMapCamera(i);

		CDxRenderer::GetRenderer()->GetDeviceContext()->VSSetShader(m_ShaderCubeMap->GetShaderVS()->VertexShader.Get(), nullptr, 0);
		CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShader(m_ShaderCubeMap->GetShaderPS().Get(), nullptr, 0);
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetInputLayout(m_ShaderCubeMap->GetShaderVS()->Layout.Get());

		//objモデルの描画
		ManagerModel::Draw(m_Model);
	}

	//放射照度マップ作成
	for (int i = 0; i < 6; i++) {

		CDxRenderer::GetRenderer()->GetDeviceContext()->ClearRenderTargetView(m_IrradianceMap->GetRenderTargetViewPointer(i), ClearColor);
	}

	m_IrradianceMap->SetViewPort();
	for (int i = 0; i < 6; i++) {
		CDxRenderer::GetRenderer()->GetDeviceContext()->OMSetRenderTargets(1, m_IrradianceMap->GetRenderTargetViewPointerPointer(i), nullptr);


		m_IrradianceMap->SetEnvMapCamera(i);

		CDxRenderer::GetRenderer()->GetDeviceContext()->VSSetShader(m_ShaderIrradianceMap->GetShaderVS()->VertexShader.Get(), nullptr, 0);
		CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShader(m_ShaderIrradianceMap->GetShaderPS().Get(), nullptr, 0);
		CDxRenderer::GetRenderer()->GetDeviceContext()->IASetInputLayout(m_ShaderIrradianceMap->GetShaderVS()->Layout.Get());
		ID3D11ShaderResourceView* t0 = m_CubeMap->GetShaderResourceView();
		CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(8, 1, &t0);

		//objモデルの描画
		ManagerModel::Draw(m_Model);

		CDxRenderer::GetRenderer()->UnbindShaderResourceView(8);
	}


	////

	//環境マップのPreFilter
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			CDxRenderer::GetRenderer()->GetDeviceContext()->ClearRenderTargetView(m_PreFilterMap->GetRenderTargetViewPointer(i * 6 + j), ClearColor);
		}
	}

	float map_size = CUBE_MAP_SIZE;
	m_RoughnessCBuffer->PSSetCBuffer(0);
	for (int i = 0; i < 6; i++) {
		float roughness = (float)i / 5.0f;
		m_RoughnessCBuffer->UpdateBuffer(&roughness);
		m_RoughnessCBuffer->PSSetCBuffer(4);
		D3D11_VIEWPORT view_port;
		view_port.Height = (float)map_size;
		view_port.Width = (float)map_size;
		view_port.TopLeftX = 0;
		view_port.TopLeftY = 0;
		view_port.MinDepth = 0;
		view_port.MaxDepth = 1;

		CDxRenderer::GetRenderer()->GetDeviceContext()->RSSetViewports(1, &view_port);
		for (int j = 0; j < 6; j++) {
			CDxRenderer::GetRenderer()->GetDeviceContext()->OMSetRenderTargets(1, m_PreFilterMap->GetRenderTargetViewPointerPointer(i * 6 + j), nullptr);
			m_PreFilterMap->SetEnvMapCamera(j);

			CDxRenderer::GetRenderer()->GetDeviceContext()->VSSetShader(m_ShaderPreFilterMap->GetShaderVS()->VertexShader.Get(), nullptr, 0);
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShader(m_ShaderPreFilterMap->GetShaderPS().Get(), nullptr, 0);
			CDxRenderer::GetRenderer()->GetDeviceContext()->IASetInputLayout(m_ShaderPreFilterMap->GetShaderVS()->Layout.Get());
			ID3D11ShaderResourceView* t0 = m_CubeMap->GetShaderResourceView();
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(8, 1, &t0);

			//objモデルの描画
			ManagerModel::Draw(m_Model);
		}

		map_size /= 2.0f;
	}

	D3D11_VIEWPORT view_port;
	view_port.Height = 512;
	view_port.Width = 512;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	view_port.MinDepth = 0;
	view_port.MaxDepth = 1;
	CDxRenderer::GetRenderer()->GetDeviceContext()->RSSetViewports(1, &view_port);
	//環境マップ用のBRDF
	CDxRenderer::GetRenderer()->GetDeviceContext()->ClearRenderTargetView(m_BrdfLUTTexture->GetRTV(), ClearColor);
	CDxRenderer::GetRenderer()->GetDeviceContext()->OMSetRenderTargets(1, m_BrdfLUTTexture->GetRTVPointerToPointer(), nullptr);

	CDxRenderer::GetRenderer()->GetDeviceContext()->VSSetShader(m_ShaderBrdfLUT->GetShaderVS()->VertexShader.Get(), nullptr, 0);
	CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShader(m_ShaderBrdfLUT->GetShaderPS().Get(), nullptr, 0);
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetInputLayout(m_ShaderBrdfLUT->GetShaderVS()->Layout.Get());
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_NONE);
	CDxRenderer::GetRenderer()->GetDeviceContext()->Draw(4, 0);

	//カリングを元に戻す
	CDxRenderer::GetRenderer()->SetCullingMode(CULL_MODE_BACK);
	
}
