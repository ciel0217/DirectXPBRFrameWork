#pragma once
#include "../LowLevel/DirectX.h"
#include "../../struct.h"
#include <string>
#include <map>



#define NORMAL_MAP 1
#define BUMP_MAP 2

#define NORMAL_UNUSED 0
// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL_CBUFFER			Material_CBuffer;
	char						TextureName[256];
};

// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};

// モデル構造体
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;
	unsigned int	*IndexArray;
	unsigned int	IndexNum;
	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};

struct ModelObject {
	MODEL* model;
	SUBSET* subset;
	unsigned int subset_num;
	ID3D11Buffer* vertex_buffer;
	ID3D11Buffer* index_buffer;
};

// マテリアル構造体
struct DX11_MODEL_MATERIAL
{
	MATERIAL_CBUFFER			Material_CBuffer;
	ID3D11ShaderResourceView	*Texture;
	ID3D11ShaderResourceView	*AlbedoTexture;
	ID3D11ShaderResourceView	*OccMetalRoughTexture;
	ID3D11ShaderResourceView	*AoTexture;
	ID3D11ShaderResourceView	*EmissiveTexture;
	ID3D11ShaderResourceView	*NormalTexture;

};

// 描画サブセット構造体
struct DX11_SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	DX11_MODEL_MATERIAL	Material;
};

struct DX11_MODEL
{
	ID3D11Buffer*	VertexBuffer;
	ID3D11Buffer*	IndexBuffer;

	DX11_SUBSET		*SubsetArray;
	unsigned int	SubsetNum;

	void SetBaseColor(D3DXCOLOR color) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.Material_CBuffer.BaseColor = color;
		}
	}

	void SetSpecular(float specular) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.Material_CBuffer.Specular = specular;
		}
	};
	void SetRoughness(float roughness) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.Material_CBuffer.Roughness = roughness;
		}
	}
	;
	void SetMetaric(float metaric) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.Material_CBuffer.Metaric = metaric;
		}
	};

	void SetAlbedoTexture(ID3D11ShaderResourceView* texture) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.AlbedoTexture = texture;
			this->SubsetArray[i].Material.Material_CBuffer.UseAlbedoMap = 1;
		}
	}

	void SetOccMetalRoughTexture(ID3D11ShaderResourceView* texture) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.OccMetalRoughTexture = texture;
			this->SubsetArray[i].Material.Material_CBuffer.UseOccMetalRough = 1;
		}
	
	}

	void SetAoTexture(ID3D11ShaderResourceView* texture) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.AoTexture = texture;
			this->SubsetArray[i].Material.Material_CBuffer.UseAoMap = 1;
		}
	}

	void SetEmissiveTexture(ID3D11ShaderResourceView* texture) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.EmissiveTexture = texture;
			this->SubsetArray[i].Material.Material_CBuffer.UseEmmisive = 1;
		}
	}

	void SetNormalTexture(ID3D11ShaderResourceView* texture) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.NormalTexture = texture;
			this->SubsetArray[i].Material.Material_CBuffer.NormalState = NORMAL_MAP;
		}
	}

	void SetBumpTexture(ID3D11ShaderResourceView* texture) {
		for (unsigned int i = 0; i < this->SubsetNum; i++)
		{
			this->SubsetArray[i].Material.NormalTexture = texture;
			this->SubsetArray[i].Material.Material_CBuffer.NormalState = BUMP_MAP;
		}
	}
};

class CBuffer;

class ManagerModel {
private:
	static std::map <std::string, DX11_MODEL*> m_ModelList;
	static CBuffer* m_ModelCBuffer;

	static void LoadObj(const char* FileName, MODEL *Model);
	static void LoadMaterial(char* File, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum);
public:
	static void Init();
	static DX11_MODEL* Load(std::string file);
	static void Draw(DX11_MODEL *Model);
	static void Unload();
};

