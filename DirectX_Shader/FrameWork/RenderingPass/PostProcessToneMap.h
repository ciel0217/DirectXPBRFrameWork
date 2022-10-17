#pragma once
#include "../Resources/CPostProcess.h"
#include <vector>
#include "../Resources/DevelopStruct.h"

class PostProcessToneMap : public CPostProcess {
private:
	std::vector<VERTEX_3D> m_MeshData;
	std::vector<UINT>	   m_IndexData;


	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer;

public:
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override {};
	void Draw()override;
};