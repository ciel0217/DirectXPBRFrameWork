#include "CDxRenderer.h"

CDxRenderer* CDxRenderer::m_Renderer;

CDxRenderer::CDxRenderer(HWND hWnd, HINSTANCE hInstance)
{
	m_hWnd = hWnd;
	m_hInstance = hInstance;
	m_Renderer = this;
}

CDxRenderer::~CDxRenderer()
{
}

HRESULT CDxRenderer::InitRenderer()
{

	HRESULT hr = S_OK;

	// �f�o�C�X�A�X���b�v�`�F�[���A�R���e�L�X�g����
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = SCREEN_WIDTH;
	sd.BufferDesc.Height = SCREEN_HEIGHT;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	sd.OutputWindow = m_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	hr = D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&m_SwapChain,
		&m_D3DDevice,
		&m_FeatureLevel,
		&m_ImmediateContext);
	if (FAILED(hr))
		return hr;


	// �����_�[�^�[�Q�b�g�r���[�����A�ݒ�
	ID3D11Texture2D* pBackBuffer = NULL;
	ID3D11RenderTargetView *temp;
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	m_D3DDevice->CreateRenderTargetView(pBackBuffer, NULL, &temp);
	m_BackBuffer.Attach(temp);

	pBackBuffer->Release();

	

	//�}���`�p�X�����_�[�^�[�Q�b�g
	for (int i = 0; i < RENDER_TARGET_NUM + 1; i++) 
	{
		m_RenderTarggers[i] = DirectXTexture2D::CreateTexture(sd.BufferDesc.Width, sd.BufferDesc.Height,
			DXGI_FORMAT_R16G16B16A16_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS);
		m_RenderTarggers[i]->CreateSRV(DXGI_FORMAT_R16G16B16A16_FLOAT);
		m_RenderTarggers[i]->CreateRTV(DXGI_FORMAT_R16G16B16A16_FLOAT);
		m_RenderTarggers[i]->CreateUAV(DXGI_FORMAT_R16G16B16A16_FLOAT);
	}

	//�}���`�p�X�����_�[�^�[�Q�b�g��ݒ肵�₷�����邽�߂�RTV�������o���Ă���
	for (int i = 0; i < RENDER_TARGET_NUM; i++) 
	{
		m_Rtv[i] = m_RenderTarggers[i]->GetRTV();
	}

	//���ԃo�b�t�@�[(PostEffect�p)
	m_IntermediateRtv = m_RenderTarggers[RENDER_TARGET_NUM]->GetRTV();
	
	//�X�e���V���p�e�N�X�`���[�쐬
	D3D11_TEXTURE2D_DESC td;
	ZeroMemory(&td, sizeof(td));
	td.Width = sd.BufferDesc.Width;
	td.Height = sd.BufferDesc.Height;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_R24G8_TYPELESS;
	//td.Format = DXGI_FORMAT_R32_FLOAT;
	td.SampleDesc = sd.SampleDesc;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	m_D3DDevice->CreateTexture2D(&td, NULL, &m_DepthTexture);


	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	//srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	ID3D11ShaderResourceView* temp1;
	m_D3DDevice->CreateShaderResourceView(m_DepthTexture, &srvDesc, &temp1);
	m_DepthStencilSRV.Attach(temp1);

	//�X�e���V���^�[�Q�b�g�쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	m_D3DDevice->CreateDepthStencilView(m_DepthTexture, &dsvd, &m_DepthStencilView);

	//m_ImmediateContext->OMSetRenderTargets(1, &m_BackBuffer, m_DepthStencilView.Get());


	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &vp);
	

	// ���X�^���C�U�X�e�[�g�ݒ�
	D3D11_RASTERIZER_DESC rd;
	ZeroMemory(&rd, sizeof(rd));
	rd.FillMode = D3D11_FILL_SOLID;
	rd.CullMode = D3D11_CULL_NONE;
	rd.DepthBias = 0;
	rd.DepthBiasClamp = 0.0f;
	rd.DepthClipEnable = TRUE;
	rd.MultisampleEnable = FALSE;
	m_D3DDevice->CreateRasterizerState(&rd, &m_RasterStateCullOff);

	rd.CullMode = D3D11_CULL_FRONT;
	m_D3DDevice->CreateRasterizerState(&rd, &m_RasterStateCullCW);

	rd.CullMode = D3D11_CULL_BACK;
	m_D3DDevice->CreateRasterizerState(&rd, &m_RasterStateCullCCW);

	// �J�����O���[�h�ݒ�iCCW�j
	SetCullingMode(CULL_MODE_BACK);


	// �u�����h�X�e�[�g�ݒ�
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_D3DDevice->CreateBlendState(&blendDesc, &m_BlendStateAlphaBlend);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &m_BlendStateNone);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &m_BlendStateAdd);

	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_D3DDevice->CreateBlendState(&blendDesc, &m_BlendStateSubtract);

	// �A���t�@�u�����h�ݒ�
	SetBlendState(BLEND_MODE_ALPHABLEND);


	// �[�x�X�e���V���X�e�[�g�ݒ�
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = FALSE;

	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateEnable);//�[�x�L���X�e�[�g

	//depthStencilDesc.DepthEnable = FALSE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	m_D3DDevice->CreateDepthStencilState(&depthStencilDesc, &m_DepthStateDisable);//�[�x�����X�e�[�g

	m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable.Get(), NULL);




	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));
	//samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	//samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	//samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	//samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	m_BorderColor = D3DXCOLOR(.0f,.0f, .0f, 1.0f);

	samplerDesc.BorderColor[0] = m_BorderColor.r;//BORDER�̐F���w��
	samplerDesc.BorderColor[1] = m_BorderColor.g;
	samplerDesc.BorderColor[2] = m_BorderColor.b;
	samplerDesc.BorderColor[3] = m_BorderColor.a;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	//samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	m_ImmediateContext->PSSetSamplers(0, 1, &samplerState);
	samplerState->Release();

	return S_OK;
}


ID3D11InputLayout* CDxRenderer::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC * data, UINT num, const std::string & a, const std::string & b)
{
	ID3D11InputLayout* layout;
	//ID3DBlob* pErrorBlob;
	ID3DBlob* pVSBlob = NULL;

	m_D3DDevice->CreateInputLayout(data,
		num,
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&layout);

	pVSBlob->Release();

	return layout;
}

ID3D11Buffer * CDxRenderer::CreateVertexBuffer(UINT VertexNum, VERTEX_3D * obj)
{
	// ���_�o�b�t�@����
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.ByteWidth = sizeof(VERTEX_3D) * VertexNum;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	static D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = obj;

	ID3D11Buffer* hpBuffer;

	if (!obj) {
		if (FAILED(m_D3DDevice->CreateBuffer(&bd, NULL, &hpBuffer))) return nullptr;
		
		return hpBuffer;
	}

	if (FAILED(m_D3DDevice->CreateBuffer(&bd, &vertexSubresourceData, &hpBuffer))) return nullptr;
	

	return hpBuffer;
}

ID3D11Buffer* CDxRenderer::CreateIndexBuffer(UINT * Index, UINT IndexNum)
{
	//�C���f�b�N�X�o�b�t�@�쐬
	D3D11_BUFFER_DESC hBufferDesc;
	ZeroMemory(&hBufferDesc, sizeof(hBufferDesc));
	hBufferDesc.ByteWidth = sizeof(UINT) * IndexNum;
	hBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	hBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	hBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	//�C���f�b�N�X�̔z���o�^
	D3D11_SUBRESOURCE_DATA hSubResourceData;
	ZeroMemory(&hSubResourceData, sizeof(hSubResourceData));
	hSubResourceData.pSysMem = Index;

	ID3D11Buffer* hpBuffer;
	if (FAILED(m_D3DDevice->CreateBuffer(&hBufferDesc, &hSubResourceData, &hpBuffer))) return nullptr;
	
	return hpBuffer;
}

void CDxRenderer::SetInputLayout(ID3D11InputLayout * VertexLayout)
{
	m_ImmediateContext->IASetInputLayout(VertexLayout);
}

void CDxRenderer::SetVertexShader(ID3D11VertexShader * vs)
{
	m_ImmediateContext->VSSetShader(vs, nullptr, 0);
}

void CDxRenderer::SetPixelShader(ID3D11PixelShader * ps)
{
	m_ImmediateContext->PSSetShader(ps, nullptr, 0);
}

void CDxRenderer::SetGeometryShader(ID3D11GeometryShader * gs)
{
	m_ImmediateContext->GSSetShader(gs, nullptr, 0);
}

void CDxRenderer::SetVertexBuffer(ID3D11Buffer * VertexBuffer, UINT VertexSize)
{
	// ���_�o�b�t�@�ݒ�
	UINT offset = 0;

	m_ImmediateContext->IASetVertexBuffers(0, 1, &VertexBuffer, &VertexSize, &offset);
}

void CDxRenderer::SetIndexBuffer(ID3D11Buffer * IndexBuffer)
{
	m_ImmediateContext->IASetIndexBuffer(IndexBuffer, DXGI_FORMAT_R32_UINT, 0);
}


void CDxRenderer::SetDepthEnable(bool Enable)
{
	if (Enable)
		m_ImmediateContext->OMSetDepthStencilState(m_DepthStateEnable.Get(), NULL);
	else
		m_ImmediateContext->OMSetDepthStencilState(m_DepthStateDisable.Get(), NULL);
}

void CDxRenderer::SetBlendState(BLEND_MODE bm)
{
	m_BlendStateParam = bm;

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	switch (m_BlendStateParam)
	{
	case BLEND_MODE_NONE:
		m_ImmediateContext->OMSetBlendState(m_BlendStateNone.Get(), blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ALPHABLEND:
		m_ImmediateContext->OMSetBlendState(m_BlendStateAlphaBlend.Get(), blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_ADD:
		m_ImmediateContext->OMSetBlendState(m_BlendStateAdd.Get(), blendFactor, 0xffffffff);
		break;
	case BLEND_MODE_SUBTRACT:
		m_ImmediateContext->OMSetBlendState(m_BlendStateSubtract.Get(), blendFactor, 0xffffffff);
		break;
	}
}

void CDxRenderer::SetCullingMode(CULL_MODE cm)
{
	switch (cm)
	{
	case CULL_MODE_NONE:
		m_ImmediateContext->RSSetState(m_RasterStateCullOff.Get());
		break;
	case CULL_MODE_FRONT:
		m_ImmediateContext->RSSetState(m_RasterStateCullCW.Get());
		break;
	case CULL_MODE_BACK:
		m_ImmediateContext->RSSetState(m_RasterStateCullCCW.Get());
		break;
	}
}

void CDxRenderer::SetAlphaTestEnable(BOOL flag)
{
	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	if (flag)
		blendDesc.AlphaToCoverageEnable = TRUE;
	else
		blendDesc.AlphaToCoverageEnable = FALSE;

	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;

	switch (m_BlendStateParam)
	{
	case BLEND_MODE_NONE:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ALPHABLEND:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_ADD:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	case BLEND_MODE_SUBTRACT:
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}

	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	ID3D11BlendState* blendState = NULL;
	m_D3DDevice->CreateBlendState(&blendDesc, &blendState);
	m_ImmediateContext->OMSetBlendState(blendState, blendFactor, 0xffffffff);

	if (blendState != NULL)
		blendState->Release();
}

void CDxRenderer::SetSamplerState(FILTER_MODE fm, ADDRESS_MODE am)
{
	// �T���v���[�X�e�[�g�ݒ�
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(samplerDesc));

	switch (fm)
	{
	case FILTER_MODE_POINT:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
		break;
	case FILTER_MODE_LINEAR:
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		break;
	case FILTER_MODE_ANISOTROPIC:
		samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
		break;
	}

	//�e�N�X�`���̃A�h���b�V���O���[�h�̐ݒ�		
	switch (am)
	{
	case ADDRESS_MODE_WRAP:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		break;
	case ADDRESS_MODE_MIRROR:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
		break;
	case ADDRESS_MODE_CLAMP:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		break;
	case ADDRESS_MODE_BORDER:
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		break;
	}

	samplerDesc.BorderColor[0] = m_BorderColor.r;
	samplerDesc.BorderColor[1] = m_BorderColor.g;
	samplerDesc.BorderColor[2] = m_BorderColor.b;
	samplerDesc.BorderColor[3] = m_BorderColor.a;

	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 16;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ID3D11SamplerState* samplerState = NULL;
	m_D3DDevice->CreateSamplerState(&samplerDesc, &samplerState);

	m_ImmediateContext->PSSetSamplers(0, 1, &samplerState);

	samplerState->Release();
	samplerState = NULL;
}

void CDxRenderer::SetSamplerBorderColor(D3DXCOLOR col)
{
	m_BorderColor = col;
}

void CDxRenderer::UnbindShaderResourceView(UINT num)
{
	ID3D11ShaderResourceView* nullSRV[] = { nullptr };
	m_ImmediateContext->PSSetShaderResources(num, 1, nullSRV);
}

void CDxRenderer::SetRenderTargetBackBuffor(bool enableDepthStancil)
{
	if (enableDepthStancil) 
	{
		m_ImmediateContext->OMSetRenderTargets(1, m_BackBuffer.GetAddressOf(), m_DepthStencilView.Get());
	}
	else
	{
		m_ImmediateContext->OMSetRenderTargets(1, m_BackBuffer.GetAddressOf(), NULL);
	}

	
}

void CDxRenderer::SetRenderTargetByDeffard()
{
	m_ImmediateContext->OMSetRenderTargets(RENDER_TARGET_NUM, m_Rtv, m_DepthStencilView.Get());
	//m_ImmediateContext->OMSetRenderTargets(RENDER_TARGET_NUM, m_Rtv, nullptr);
}

void CDxRenderer::SetRenderTargetIntermediateBuffer(bool enableDepthStencil)
{
	if (enableDepthStencil) 
	{
		m_ImmediateContext->OMSetRenderTargets(1, &m_IntermediateRtv, m_DepthStencilView.Get());
	}
	else 
	{
		m_ImmediateContext->OMSetRenderTargets(1, &m_IntermediateRtv, NULL);
	}

}



void CDxRenderer::ClearBackBuffor(bool enableDepthStencilClear)
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_ImmediateContext->ClearRenderTargetView(m_BackBuffer.Get(), ClearColor);

	if (enableDepthStencilClear) 
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
}

void CDxRenderer::ClearRenderTextureSceneByDeferred(bool enableDepthStencilClear)
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	for (int i = 0; i < RENDER_TARGET_NUM; i++) {
		m_ImmediateContext->ClearRenderTargetView(m_RenderTargets[i].RenderTargetView.Get(), ClearColor);
	}
	if(enableDepthStencilClear)
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void CDxRenderer::ClearIntermediateBuffer(bool enableDepthStencilClear)
{
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	m_ImmediateContext->ClearRenderTargetView(m_IntermediateRtv, ClearColor);

	if (enableDepthStencilClear) 
		m_ImmediateContext->ClearDepthStencilView(m_DepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	
}


void CDxRenderer::SetViewPortDefault()
{
	// �r���[�|�[�g�ݒ�
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)SCREEN_WIDTH;
	vp.Height = (FLOAT)SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_ImmediateContext->RSSetViewports(1, &vp);
}



void CDxRenderer::Present()
{
	m_SwapChain->Present(0, 0);
}
