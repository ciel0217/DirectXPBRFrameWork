#pragma once
#include "../Resources/CommonProcess.h"
#include "../Resources/ObjectTag.h"
#include <list>
#include "../Resources/DevelopEnum.h"
#include "DeferredRenderer.h"
#include "../Resources/SkyBox.h"
#include <string>
#include "../Manager/ManagerLight.h"
#include <type_traits>


enum RenderPattern {
	eForward = 0,
	eDeffard,
};

class CBuffer;
class CLight;
class DeferredRenderer;
class CPostProcess;
class SkyBox;
class GBufferPass;

class CameraRenderer {

private:
	static DeferredRenderer* m_DeferredRenderPass;
	static CPostProcess*  m_ToneMapPass;
	static ManagerLight* m_LightPass;
	static GBufferPass*  m_GBufferPass;


	static CBuffer*				  m_ViewCBuffer;
	static CBuffer*				  m_ProjectionCBuffer;
	static CBuffer*				  m_CameraPosCBuffer;

	static CBuffer*				  m_ViewInverseCBuffer;
	static CBuffer*				  m_ProjectionInverseCBuffer;

	static SkyBox*				  m_SkyBox;
	
	RenderPattern			   m_RenderPattern;

	std::list<CommonProcess *> m_GameObjects2D;			//2dのオブジェクト
	std::list<CommonProcess *> m_GameObjectsOpacity;		//不透明なオブジェクト
	std::list<CommonProcess *> m_GameObjectsTransparent;	//透明 or 半透明なオブジェクト

	std::list<CPostProcess *> m_PostProcessToOpacity;
	std::list<CPostProcess *> m_PostProcessToAll;

	int m_TagMask = TAG_EVERYTHING;
	D3DXVECTOR3 m_CameraPos;

	void CalcRenderingOrder(std::list<CommonProcess *> gameobject[]);
	void ClearGameObjectList();

	void DrawTransparent();
	void Draw2D();
	void DrawPostProcessToOpacity();
	void DrawPostProcessToAll();


public:
	CameraRenderer();
	virtual ~CameraRenderer() {};

	static void Release();

	void SetUpRenderer();
	void Uninit();
	void DrawRenderer(std::list<CommonProcess *> gameobject[]);

	void SetSkyBox(SkyBox* obj);

	void SetPostProcess(CPostProcess* obj, PostProcessType type) {
		switch (type)
		{
		case eOnlyOpacity:
			m_PostProcessToOpacity.push_back(obj);
			break;
		case eAll:
			m_PostProcessToAll.push_back(obj);
			break;
		default:
			break;
		}
	}

	void SetLightObject(CLight* obj) {
		m_LightPass->SetLight(obj);
	}

	void SetVPCBuffer(D3DXVECTOR3 pos, D3DXVECTOR3 lookat, D3DXVECTOR3 up);

};