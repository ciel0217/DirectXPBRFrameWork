#pragma once
#include "../Resources/DevelopStruct.h"


class CBuffer;
class CShader;
class CommonProcess;

class DeferredRenderer {
private:
	CShader* m_Shader;
	CBuffer* m_DeferredCBuffer;

	DEFERRED_CBUFFER m_CBufferValue;
	ID3D11ShaderResourceView* m_IrradianceTexture;
	ID3D11ShaderResourceView* m_PrefilterTexture;
	ID3D11ShaderResourceView* m_BrdfLUTexture;

public:
	void Config();
	void Init();
	void Uninit();
	void Draw();

	void UseEnvMap(ID3D11ShaderResourceView* irradiance, ID3D11ShaderResourceView* prefileter, ID3D11ShaderResourceView* brdflut);
};