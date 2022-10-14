#pragma once

#include "DirectX.h"
#include "../../struct.h"

//#include "DirectX_Shader/FrameWork/Resources/CCubeMap.h"

#include <string>

//1�p�X�ڂɎg�p����e�N�X�`���[�̐�
#define RENDER_TARGET_NUM 4

class CDxRenderer {
private:
	static CDxRenderer *m_Renderer;
	D3D_FEATURE_LEVEL       m_FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	Microsoft::WRL::ComPtr<ID3D11Device> m_D3DDevice = NULL;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_ImmediateContext = NULL;

	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain = NULL;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_BackBuffer = NULL;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView = NULL;

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader = NULL;

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader = NULL;

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_VertexLayout = NULL;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStateEnable = NULL;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStateDisable = NULL;

	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateNone = NULL;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateAlphaBlend = NULL;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateAdd = NULL;
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStateSubtract = NULL;

	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterStateCullOff = NULL;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterStateCullCW = NULL;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterStateCullCCW = NULL;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_DepthStencilSRV = NULL;

	
	ID3D11Texture2D* m_DepthTexture = NULL;

	//0 : color 1 : Normal, 2 : r:roughness g:metaric b:specular 3:Emission + (lightmaps) 4 ���ԃo�b�t�@�[
	RenderTarget m_RenderTargets[RENDER_TARGET_NUM + 1];

	ID3D11RenderTargetView* m_Rtv[RENDER_TARGET_NUM];
	ID3D11RenderTargetView* m_IntermediateRtv;





	HWND m_hWnd;
	HINSTANCE m_hInstance;

	D3DXCOLOR m_BorderColor;
	
	BLEND_MODE				m_BlendStateParam;
	MATERIAL_CBUFFER	m_Material;
	LIGHT_CBUFFER	m_Light;

public:
	CDxRenderer() = delete;
	CDxRenderer(HWND, HINSTANCE);
	~CDxRenderer();

	//Init���Ă����Ԃ���
	static CDxRenderer *GetRenderer() { return m_Renderer; }

	HRESULT InitRenderer();

	ID3D11InputLayout* CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * , UINT , const std::string &, const std::string &);

	ID3D11Buffer* CreateVertexBuffer(UINT VertexNum, VERTEX_3D* obj);

	ID3D11Buffer* CreateIndexBuffer(UINT* Index, UINT IndexNum);

	ID3D11Device* GetDevice() { return m_D3DDevice.Get(); }
	ID3D11DeviceContext *GetDeviceContext() { return m_ImmediateContext.Get(); }
	ID3D11ShaderResourceView* GetDepthStencilSRV() { return m_DepthStencilSRV.Get(); }

	void SetInputLayout(ID3D11InputLayout* VertexLayout);
	void SetVertexShader(ID3D11VertexShader* vs);
	void SetPixelShader(ID3D11PixelShader* ps);
	void SetGeometryShader(ID3D11GeometryShader* gs);

	void SetVertexBuffer(ID3D11Buffer* VertexBuffer, UINT VertexSize);
	void SetIndexBuffer(ID3D11Buffer* IndexBuffer);


	void SetDepthEnable(bool Enable);
	void SetBlendState(BLEND_MODE bm);
	void SetCullingMode(CULL_MODE cm);
	void SetAlphaTestEnable(BOOL flag);
	void SetSamplerState(FILTER_MODE fm, ADDRESS_MODE am);
	void SetSamplerBorderColor(D3DXCOLOR col);

	void UnbindShaderResourceView(UINT);
	void SetRenderTargetBackBuffor(bool );
	void SetRenderTargetByDeffard();
	//���ԃo�b�t�@�[
	void SetRenderTargetIntermediateBuffer(bool);

	//DepthStencil���N���A���邩
	void ClearBackBuffor(bool);
	//DepthStencil���N���A���邩
	void ClearRenderTextureSceneByDeferred(bool);
	//DepthStencil���N���A���邩
	void ClearIntermediateBuffer(bool);

	void SetViewPortDefault();

	void Present();

	RenderTarget GetSceneTexture(SCENE_TEXTURE tex) { return m_RenderTargets[tex]; }
};


