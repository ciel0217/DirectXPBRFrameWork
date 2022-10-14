#include "CameraRenderer.h"
#include "../Manager/ManagerLight.h"
#include "PostProcessToneMap.h"
#include "../Resources/CPostProcess.h"
#include "../Resources/CBuffer.h"
#include "GBufferPass.h"
#include "../LowLevel/CDxRenderer.h"

#define VIEW_NEAR_Z (0.3f)
#define VIEW_FAR_Z  (10000000.0f)


DeferredRenderer*         CameraRenderer::m_DeferredRenderPass;
CPostProcess*             CameraRenderer::m_ToneMapPass;
ManagerLight*		      CameraRenderer::m_LightPass;
GBufferPass*			  CameraRenderer::m_GBufferPass;

CBuffer*				  CameraRenderer::m_ViewCBuffer;
CBuffer*				  CameraRenderer::m_ProjectionCBuffer;
CBuffer*				  CameraRenderer::m_CameraPosCBuffer;

CBuffer*				  CameraRenderer::m_ViewInverseCBuffer;
CBuffer*				  CameraRenderer::m_ProjectionInverseCBuffer;

SkyBox*				      CameraRenderer::m_SkyBox;



CameraRenderer::CameraRenderer()
{
	if (!m_DeferredRenderPass)
		m_DeferredRenderPass = new DeferredRenderer();
	if (!m_LightPass)
		m_LightPass = new ManagerLight;
	if (!m_ToneMapPass)
		m_ToneMapPass = new PostProcessToneMap();
	if (!m_GBufferPass)
		m_GBufferPass = new GBufferPass();

	if(!m_ViewCBuffer)
		m_ViewCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	if(!m_ProjectionCBuffer)
		m_ProjectionCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr));

	if (!m_ViewInverseCBuffer)
		m_ViewInverseCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	if (!m_ProjectionInverseCBuffer)
		m_ProjectionInverseCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DMATRIX), D3D11_BIND_CONSTANT_BUFFER, nullptr));

	if(!m_CameraPosCBuffer)
		m_CameraPosCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(D3DXCOLOR), D3D11_BIND_CONSTANT_BUFFER, nullptr));

	m_DeferredRenderPass->Config();
	m_LightPass->Config();
	m_ToneMapPass->Config();
}

void CameraRenderer::SetUpRenderer()
{
	
	for (auto effect : m_PostProcessToOpacity) {
		effect->Config();
	}

	for (auto effect : m_PostProcessToAll) {
		effect->Config();
	}
}

void CameraRenderer::Uninit()
{
	//static‚¶‚á‚È‚¢‚â‚Â‚ð‰ð•ú
	for (auto obj : m_PostProcessToOpacity) {
		obj->Uninit();
	}

	for (auto obj : m_PostProcessToAll) {
		obj->Uninit();
	}
	m_DeferredRenderPass->Uninit();

	m_PostProcessToOpacity.remove_if([](CPostProcess* obj) {
		return obj->Release();
		});
	m_PostProcessToOpacity.clear();

	m_PostProcessToAll.remove_if([](CPostProcess* obj) {
		return obj->Release();
		});
	m_PostProcessToAll.clear();
}

void CameraRenderer::DrawRenderer(std::list<CommonProcess *> gameobject[])
{

	CalcRenderingOrder(gameobject);
	CDxRenderer::GetRenderer()->SetDepthEnable(true);

	CDxRenderer::GetRenderer()->SetViewPortDefault();
	CDxRenderer::GetRenderer()->ClearRenderTextureSceneByDeferred(true);
	CDxRenderer::GetRenderer()->SetRenderTargetByDeffard();

	m_GBufferPass->Draw(m_GameObjectsOpacity);
	
	CDxRenderer::GetRenderer()->ClearIntermediateBuffer(false);
	CDxRenderer::GetRenderer()->SetRenderTargetIntermediateBuffer(false);

	m_LightPass->SetLightCBuffer();
	m_DeferredRenderPass->Draw();
	
	DrawPostProcessToOpacity();

	CDxRenderer::GetRenderer()->SetRenderTargetIntermediateBuffer(true);
	DrawTransparent();

	CDxRenderer::GetRenderer()->ClearBackBuffor(false);
	CDxRenderer::GetRenderer()->SetRenderTargetBackBuffor(true);

	m_ToneMapPass->Draw();
	
	Draw2D();
	DrawPostProcessToAll();

}

void CameraRenderer::CalcRenderingOrder(std::list<CommonProcess *> gameobject[])
{
	ClearGameObjectList();

	for (int i = 0; i < eMaxLayer; i++) {
		for (auto obj : gameobject[i]) {
		
			if (dynamic_cast<CLight*>(obj)) {
				m_LightPass->SetLight(dynamic_cast<CLight*>(obj));
				continue;
			}

			if (obj->GetRenderQueue() <= DrawObjectRenderQueue::eOpacity) {
				m_GameObjectsOpacity.push_back(obj);
			}
			else if (obj->GetRenderQueue() == DrawObjectRenderQueue::e2D) {
				m_GameObjects2D.push_back(obj);
			}
			else {
				m_GameObjectsTransparent.push_back(obj);
			}
		}
	}


	D3DXVECTOR3 pos = m_CameraPos;
	//‹ß‚¢‚Ù‚¤‚©‚ç•`‰æ(less)
	m_GameObjectsOpacity.sort([pos](CommonProcess* a, CommonProcess* b)
		{
			D3DXVECTOR3 a_dis = pos - a->GetPosition();
			D3DXVECTOR3 b_dis = pos - b->GetPosition();

			return D3DXVec3LengthSq(&a_dis) < D3DXVec3LengthSq(&b_dis);
		});

	//‰“‚¢‚Ù‚¤‚©‚ç•`‰æ(greater)
	m_GameObjectsTransparent.sort([pos](CommonProcess* a, CommonProcess* b)
		{
			D3DXVECTOR3 a_dis = pos - a->GetPosition();
			D3DXVECTOR3 b_dis = pos - b->GetPosition();

			return D3DXVec3LengthSq(&a_dis) > D3DXVec3LengthSq(&b_dis);
		});
}

void CameraRenderer::ClearGameObjectList()
{
	m_GameObjects2D.clear();
	m_GameObjectsOpacity.clear();
	m_GameObjectsTransparent.clear();
}

void CameraRenderer::DrawTransparent()
{
	for (auto obj : m_GameObjectsTransparent) {
		obj->Draw();
	}
}

void CameraRenderer::Draw2D()
{
	for (auto obj : m_GameObjects2D) {
		obj->Draw();
	}
}

void CameraRenderer::DrawPostProcessToOpacity()
{
	for (auto effect : m_PostProcessToOpacity) {
		effect->Draw();
	}
}

void CameraRenderer::DrawPostProcessToAll()
{
	for (auto effect : m_PostProcessToAll) {
		effect->Draw();
	}
}


void CameraRenderer::SetSkyBox(SkyBox* obj)
{
	m_SkyBox = obj;

	m_SkyBox->DrawByCubeMap();

	ID3D11ShaderResourceView* irradiance = m_SkyBox->GetIrradianceMap();
	ID3D11ShaderResourceView* prefilter = m_SkyBox->GetSpecularMap();
	ID3D11ShaderResourceView* brdflut = m_SkyBox->GetBrdfLUTMap();

	m_DeferredRenderPass->UseEnvMap(irradiance, prefilter, brdflut);

}

void CameraRenderer::Release()
{	
	delete m_DeferredRenderPass;
	delete m_LightPass;
	delete m_ToneMapPass;
	delete m_GBufferPass;
	delete m_ViewCBuffer;
	delete m_ProjectionCBuffer;
	delete m_CameraPosCBuffer;
}

void CameraRenderer::SetVPCBuffer(D3DXVECTOR3 pos, D3DXVECTOR3 lookat, D3DXVECTOR3 up)
{
	m_CameraPos = pos;

	D3DXMATRIX mtxView, mtxProjection, mtxTransView, mtxTransProj, mtxInverseView, mtxInverseProj;
	D3DXMatrixLookAtLH(&mtxView, &pos, &lookat, &up);
	D3DXMatrixTranspose(&mtxTransView, &mtxView);
	m_ViewCBuffer->UpdateBuffer(&mtxTransView);
	m_ViewCBuffer->VSSetCBuffer(1);
	m_ViewCBuffer->PSSetCBuffer(1);

	D3DXMatrixPerspectiveFovLH(&mtxProjection, 1.0f, (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, VIEW_NEAR_Z, VIEW_FAR_Z);
	D3DXMatrixTranspose(&mtxTransProj, &mtxProjection);
	m_ProjectionCBuffer->UpdateBuffer(&mtxTransProj);
	m_ProjectionCBuffer->VSSetCBuffer(2);
	m_ProjectionCBuffer->PSSetCBuffer(2);

	D3DXMatrixInverse(&mtxInverseView, NULL, &mtxView);
	m_ViewInverseCBuffer->UpdateBuffer(&mtxInverseView);
	m_ViewInverseCBuffer->VSSetCBuffer(7);
	m_ViewInverseCBuffer->PSSetCBuffer(7);


	D3DXMatrixInverse(&mtxInverseProj, NULL, &mtxProjection);
	m_ProjectionInverseCBuffer->UpdateBuffer(&mtxInverseProj);
	m_ProjectionInverseCBuffer->VSSetCBuffer(8);
	m_ProjectionInverseCBuffer->PSSetCBuffer(8);

	m_CameraPosCBuffer->UpdateBuffer(pos);
	m_CameraPosCBuffer->PSSetCBuffer(5);
}

