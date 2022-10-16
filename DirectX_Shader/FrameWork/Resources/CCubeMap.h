#pragma once
#include <string>
#include "../LowLevel/DirectX.h"
#include "../Resources/CGameObject.h"
#include <vector>
#include "DirectXTexture2D.h"

#define CUBE_MAP_SIZE (512)
class CBuffer;
class CCubeMap : public CGameObject3D{

	const D3DXVECTOR3 forward[6] =
	{
		{ 1, 0, 0 },
		{ -1, 0, 0},
		{ 0, 1, 0 },
		{ 0, -1, 0},
		{ 0, 0, 1},
		{ 0, 0, -1},
	};

	const D3DXVECTOR3 up[6] =
	{
		{ 0, 1, 0},
		{ 0, 1, 0},
		{ 0, 0, -1},
		{ 0, 0, 1},
		{ 0, 1, 0},
		{ 0, 1, 0},
	};

private:
	D3D11_VIEWPORT m_ViewPort;

	D3DXVECTOR3 m_LookAtPoint[6];
	D3DXVECTOR3 m_CameraUp[6];
	D3DXVECTOR3 m_Position;

	UINT m_MipMapLevel;

	DirectXTexture2D* m_Texture;

	CBuffer* m_ViewCBuffer;
	CBuffer* m_ProjectionCBuffer;
	void CreateCubeMapResourveAndView();
	
public:
	CCubeMap(UINT mipmap) : m_MipMapLevel(mipmap) {};
	void Init();
	void Uninit();
	void Update();
	void Draw();

	ID3D11RenderTargetView** GetRenderTargetViewPointerPointer(UINT index) { return m_Texture->GetRTVPointerToPointer(index); }
	ID3D11RenderTargetView*  GetRenderTargetViewPointer(UINT index) { return m_Texture->GetRTV(index); }
	//ID3D11DepthStencilView* GetDepthStencilView() { return m_CubeMapDSV.Get(); }
	//D3D11_VIEWPORT GetViewPort() { return m_ViewPort; }
	ID3D11ShaderResourceView* GetShaderResourceView() { return m_Texture->GetSRV(); }

	void SetViewPort();
	void SetCBuffer();

	void SetEnvMapCamera(int index);
};