#include "ManagerShader.h"
#include "../Resources/CShader.h"
#include "../LowLevel/CDxRenderer.h"
#include <vector>

std::map<std::string, CShader*> ManagerShader:: m_ShaderList;
const std::string ManagerShader::m_ShaderName[SHADER_MAX] =
{"Shader/shader.hlsl","Shader/Deferred.hlsl","Shader/2DTexture.hlsl", "Shader/DefaultModel.hlsl", "Shader/CubeMap.hlsl", "Shader/ToneMap.hlsl"
, "Shader/IrradianceMap.hlsl", "Shader/SpecularMap.hlsl", "Shader/BrdfLUT.hlsl", "Shader/Ocean.hlsl"};

void ManagerShader::LoadVertexShader(std::string name)
{
	HRESULT hr;
	// ���_�V�F�[�_�R���p�C���E����
	ID3DBlob* pErrorBlob;
	ID3DBlob* pVSBlob = NULL;

	hr = D3DX11CompileFromFile(name.c_str(), NULL, NULL, "VS_main", "vs_4_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	}

	ID3D11VertexShader* shader;
	CDxRenderer::GetRenderer()->GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), NULL, &shader);
	m_ShaderList[name]->GetShaderVS()->VertexShader.Attach(shader);
	
	//�V�F�[�_�[���t���N�V�����̐���
	ID3D11ShaderReflection* rf;
	hr = D3DReflect(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&rf);
	if (FAILED(hr)) 
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "VS", MB_OK | MB_ICONERROR);
	}
	m_ShaderList[name]->GetShaderVS()->ShaderRF.Attach(rf);
	//�C���v�b�g���C�A�E�g���I�ǂݍ���
	LoadInputLayout(name,  pVSBlob);
}


void ManagerShader::LoadPixelShader(std::string name)
{
	HRESULT hr;
	// ���_�V�F�[�_�R���p�C���E����
	ID3DBlob* pErrorBlob;
	ID3DBlob* pPSBlob = NULL;

	//	hr = D3DX11CompileFromFile( "Shader/shader.hlsl", NULL, NULL, "VertexShaderPolygon", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pVSBlob, &pErrorBlob, NULL );
	hr = D3DX11CompileFromFile(name.c_str(), NULL, NULL, "PS_main", "ps_4_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pPSBlob, &pErrorBlob, NULL);
	//hr = D3DX11CompileFromFile("Shader/shader.hlsl", NULL, NULL, "VS_main", "vs_4_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, (char*)pErrorBlob->GetBufferPointer(), "PS", MB_OK | MB_ICONERROR);
	}

	ID3D11PixelShader* shader;
	CDxRenderer::GetRenderer()->GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), NULL, &(shader));
	//a.Attach(shader);
	//auto a = m_ShaderList[name]->GetShaderPS();
	m_ShaderList[name]->SetShaderPS(shader);
}

void ManagerShader::LoadGeometryShader(std::string name)
{
	HRESULT hr;
	// ���_�V�F�[�_�R���p�C���E����
	ID3DBlob* pErrorBlob;
	ID3DBlob* pGSBlob = NULL;

	//	hr = D3DX11CompileFromFile( "Shader/shader.hlsl", NULL, NULL, "VertexShaderPolygon", "vs_4_0", D3DCOMPILE_ENABLE_STRICTNESS, 0, NULL, &pVSBlob, &pErrorBlob, NULL );
	hr = D3DX11CompileFromFile(name.c_str(), NULL, NULL, "GS_main", "gs_4_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pGSBlob, &pErrorBlob, NULL);
	//hr = D3DX11CompileFromFile("Shader/shader.hlsl", NULL, NULL, "VS_main", "vs_4_0", D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, &pVSBlob, &pErrorBlob, NULL);
	if (FAILED(hr))
	{
		return;
	}

	ID3D11GeometryShader* shader;
	CDxRenderer::GetRenderer()->GetDevice()->CreateGeometryShader(pGSBlob->GetBufferPointer(), pGSBlob->GetBufferSize(), NULL, &(shader));
	
	m_ShaderList[name]->SetShaderGS(shader);
}

void ManagerShader::LoadInputLayout(std::string name, ID3D10Blob* blob)
{
	D3D11_SHADER_DESC shaderdesc;
	ID3D11ShaderReflection* reflection = m_ShaderList[name]->GetShaderVS()->ShaderRF.Get();

	reflection->GetDesc(&shaderdesc);

	std::vector<D3D11_INPUT_ELEMENT_DESC> vbElement;

	for (unsigned int i = 0; i < shaderdesc.InputParameters; i++) 
	{
		D3D11_SIGNATURE_PARAMETER_DESC sigdesc;
		reflection->GetInputParameterDesc(i, &sigdesc);

		auto format = GetDxgiFormat(sigdesc.ComponentType, sigdesc.Mask);

		D3D11_INPUT_ELEMENT_DESC eledesc = {
				sigdesc.SemanticName // Semantic��
				, sigdesc.SemanticIndex // POSITION0�Ƃ��̐����B�������0
				, format // DXGI_FORMAT
				, 0 // ���ߑł�
				, D3D11_APPEND_ALIGNED_ELEMENT // ���ߑł�
				, D3D11_INPUT_PER_VERTEX_DATA // ���ߑł�
				, 0 // ���ߑł�
		};
		vbElement.push_back(eledesc);
	}
	
	if (!vbElement.empty() && lstrcmp(vbElement[0].SemanticName, "SV_VertexID") != 0) 
	{
		HRESULT hr;
		ID3D11InputLayout* layout;
		hr = CDxRenderer::GetRenderer()->GetDevice()->CreateInputLayout(&vbElement[0], vbElement.size(),
			blob->GetBufferPointer(), blob->GetBufferSize(), &layout);

		m_ShaderList[name]->GetShaderVS()->Layout.Attach(layout);
		if (FAILED(hr))
			return ;
	}
}

void ManagerShader::Load(std::string name)
{
	LoadVertexShader(name);
	LoadPixelShader(name);
	LoadGeometryShader(name);
	
}

DXGI_FORMAT ManagerShader::GetDxgiFormat(D3D10_REGISTER_COMPONENT_TYPE type, BYTE mask)
{

	if ((mask & 0x0F) == 0x0F)
	{
		// xyzw
		switch (type)
		{
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
	}

	if ((mask & 0x07) == 0x07)
	{
		// xyz
		switch (type)
		{
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		}
	}

	if ((mask & 0x3) == 0x3)
	{
		// xy
		switch (type)
		{
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32_FLOAT;
		}
	}

	if ((mask & 0x1) == 0x1)
	{
		// x
		switch (type)
		{
		case D3D10_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32_FLOAT;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}




bool ManagerShader::CompileShader()
{
	for (int i = 0; i < SHADER_MAX; i++) 
	{
		m_ShaderList.insert({ m_ShaderName[i], new CShader()});
		Load(m_ShaderName[i]);
		
	}

	return false;
}

CShader* ManagerShader::GetShader(std::string name)
{
	if (m_ShaderList.count(name)) 
	{
		return m_ShaderList[name];
	}
	return nullptr;
}

void ManagerShader::Release()
{
	for (auto shader : m_ShaderList)
	{
		delete shader.second;
	}

	m_ShaderList.clear();
}
