#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "ManagerModel.h"
#include "../LowLevel/CDxRenderer.h"
#include "../Resources/CBuffer.h"
#include <assert.h>
#include <sstream>
#include <istream>

std::map <std::string , DX11_MODEL* > ManagerModel::m_ModelList;
CBuffer* ManagerModel::m_ModelCBuffer;

void ManagerModel::LoadObj(const char* FileName, MODEL * Model)
{

	D3DXVECTOR3	*positionArray;
	D3DXVECTOR3	*normalArray;
	D3DXVECTOR2	*texcoordArray;

	unsigned int	positionNum = 0;
	unsigned int	normalNum = 0;
	unsigned int	texcoordNum = 0;
	unsigned int	vertexNum = 0;
	unsigned int	indexNum = 0;
	unsigned int	in = 0;
	unsigned int	subsetNum = 0;

	MODEL_MATERIAL	*materialArray = NULL;
	unsigned int	materialNum = 0;

	char str[256];
	char *s;
	char c;


	FILE *file;
	file = fopen(FileName, "rt");
	if (file == NULL)
	{
		printf("エラー:LoadModel %s \n", FileName);
		return;
	}



	//要素数カウント
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "v") == 0)
		{
			positionNum++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			normalNum++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			texcoordNum++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			subsetNum++;
		}
		else if (strcmp(str, "f") == 0)
		{
			in = 0;

			do
			{
				fscanf(file, "%s", str);
				vertexNum++;
				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//四角は三角に分割
			if (in == 4)
				in = 6;

			indexNum += in;
		}
	}


	//メモリ確保
	positionArray = new D3DXVECTOR3[positionNum];
	normalArray = new D3DXVECTOR3[normalNum];
	texcoordArray = new D3DXVECTOR2[texcoordNum];


	Model->VertexArray = new VERTEX_3D[vertexNum];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned int[indexNum];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[subsetNum];
	Model->SubsetNum = subsetNum;




	//要素読込
	D3DXVECTOR3 *position = positionArray;
	D3DXVECTOR3 *normal = normalArray;
	D3DXVECTOR2 *texcoord = texcoordArray;

	unsigned int   vc = 0;
	unsigned int   ic = 0;
	unsigned int   sc = 0;


	fseek(file, 0, SEEK_SET);

	//OBJ形式ファイルのパーサー
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "mtllib") == 0)
		{
			//マテリアルファイル
			fscanf(file, "%s", str);

			char path[256];
			strcpy(path, "asset/Material/");
			strcat(path, str);

			LoadMaterial(path, &materialArray, &materialNum);
		}
		else if (strcmp(str, "o") == 0)
		{
			//オブジェクト名
			fscanf(file, "%s", str);
		}
		else if (strcmp(str, "v") == 0)
		{
			//頂点座標
			fscanf(file, "%f", &position->x);
			fscanf(file, "%f", &position->y);
			fscanf(file, "%f", &position->z);
			position++;
		}
		else if (strcmp(str, "vn") == 0)
		{
			//法線
			fscanf(file, "%f", &normal->x);
			fscanf(file, "%f", &normal->y);
			fscanf(file, "%f", &normal->z);
			normal++;
		}
		else if (strcmp(str, "vt") == 0)
		{
			//テクスチャ座標
			fscanf(file, "%f", &texcoord->x);
			fscanf(file, "%f", &texcoord->y);
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if (strcmp(str, "usemtl") == 0)
		{
			//マテリアル
			fscanf(file, "%s", str);

			if (sc != 0)
				Model->SubsetArray[sc - 1].IndexNum = ic - Model->SubsetArray[sc - 1].StartIndex;

			Model->SubsetArray[sc].StartIndex = ic;


			for (unsigned int i = 0; i < materialNum; i++)
			{
				if (strcmp(str, materialArray[i].Name) == 0)
				{
					Model->SubsetArray[sc].Material.Material_CBuffer = materialArray[i].Material_CBuffer;
					strcpy(Model->SubsetArray[sc].Material.TextureName, materialArray[i].TextureName);
					strcpy(Model->SubsetArray[sc].Material.Name, materialArray[i].Name);

					break;
				}
			}

			sc++;

		}
		else if (strcmp(str, "f") == 0)
		{
			//f 11/22/11 9/23/9 12/24/12

			//面
			in = 0;

			do
			{
				fscanf(file, "%s", str);

				s = strtok(str, "/");
				Model->VertexArray[vc].Position = positionArray[atoi(s) - 1];
				if (s[strlen(s) + 1] != '/')
				{
					//テクスチャ座標が存在しない場合もある
					s = strtok(NULL, "/");
					Model->VertexArray[vc].TexCoord = texcoordArray[atoi(s) - 1];
				}
				s = strtok(NULL, "/");
				Model->VertexArray[vc].Normal = normalArray[atoi(s) - 1];

				Model->VertexArray[vc].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				Model->VertexArray[vc].Tangent = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				Model->VertexArray[vc].Binormal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');

			//四角は三角に分割
			if (in == 4)
			{
				Model->IndexArray[ic] = vc - 4;
				ic++;
				Model->IndexArray[ic] = vc - 2;
				ic++;
			}
		}
	}


	if (sc != 0)
		Model->SubsetArray[sc - 1].IndexNum = ic - Model->SubsetArray[sc - 1].StartIndex;

	for (unsigned int i = 0; i < indexNum; i+=3) {
		unsigned int index1 = Model->IndexArray[i];
		unsigned int index2 = Model->IndexArray[i + 1];
		unsigned int index3 = Model->IndexArray[i + 2];

		VERTEX_3D &vertex1 = Model->VertexArray[index1];
		VERTEX_3D &vertex2 = Model->VertexArray[index2];
		VERTEX_3D &vertex3 = Model->VertexArray[index3];

		
		D3DXVECTOR3 edge1 = vertex2.Position - vertex1.Position;
		D3DXVECTOR3 edge2 = vertex3.Position - vertex1.Position;
		D3DXVECTOR2 duv1 = vertex2.TexCoord - vertex1.TexCoord;
		D3DXVECTOR2 duv2 = vertex3.TexCoord - vertex1.TexCoord;

		//逆行列の前に出す定数
		//逆行列よりも転置行列使った方が計算が早いらしい...要調査
		float f = 1.0f / (duv1.x * duv2.y - duv1.y * duv2.x);
		D3DXVECTOR3 tangent = 
		{
			f * (duv2.y * edge1.x - duv1.y * edge2.x),
			f * (duv2.y * edge1.y - duv1.y * edge2.y),
			f * (duv2.y * edge1.z - duv1.y * edge2.z),
		};

		D3DXVECTOR3 binormal =
		{
			f * (duv2.x * edge1.x - duv1.x * edge2.x),
			f * (duv2.x * edge1.y - duv1.x * edge2.y),
			f * (duv2.x * edge1.z - duv1.x * edge2.z),
		};
		
		D3DXVec3Normalize(&tangent, &tangent);
		D3DXVec3Normalize(&binormal, &binormal);

		vertex1.Tangent = (tangent + vertex1.Tangent) / 2.0f;
		vertex2.Tangent = (tangent + vertex2.Tangent) / 2.0f;
		vertex3.Tangent = (tangent + vertex3.Tangent) / 2.0f;

		vertex1.Binormal = (binormal + vertex1.Binormal) / 2.0f;
		vertex2.Binormal = (binormal + vertex2.Binormal) / 2.0f;
		vertex3.Binormal = (binormal + vertex3.Binormal) / 2.0f;
	}



	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;

	fclose(file);
}

void ManagerModel::LoadMaterial(char* FileName, MODEL_MATERIAL ** MaterialArray, unsigned int * MaterialNum)
{
	char str[256];

	FILE *file;
	file = fopen(FileName, "rt");
	if (file == NULL)
	{
		printf("エラー:LoadMaterial %s \n", FileName);
		return;
	}

	MODEL_MATERIAL *materialArray;
	unsigned int materialNum = 0;

	//要素数カウント
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0)
		{
			materialNum++;
		}
	}


	//メモリ確保
	materialArray = new MODEL_MATERIAL[materialNum];
	ZeroMemory(materialArray, sizeof(MODEL_MATERIAL)*materialNum);


	//要素読込
	int mc = -1;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;


		if (strcmp(str, "newmtl") == 0)
		{
			//マテリアル名
			mc++;
			fscanf(file, "%s", materialArray[mc].Name);
			strcpy(materialArray[mc].TextureName, "");
			materialArray[mc].Material_CBuffer.noTexSampling = 1;
		}
		else if (strcmp(str, "Ka") == 0)
		{
			////アンビエント（環境光）
			//fscanf(file, "%f", &materialArray[mc].Material_CBuffer.Ambient.r);
			//fscanf(file, "%f", &materialArray[mc].Material_CBuffer.Ambient.g);
			//fscanf(file, "%f", &materialArray[mc].Material_CBuffer.Ambient.b);
			//materialArray[mc].Material_CBuffer.Ambient.a = 1.0f;
		}
		else if (strcmp(str, "Kd") == 0)
		{
			//ディフューズ（拡散光）
			fscanf(file, "%f", &materialArray[mc].Material_CBuffer.BaseColor.r);
			fscanf(file, "%f", &materialArray[mc].Material_CBuffer.BaseColor.g);
			fscanf(file, "%f", &materialArray[mc].Material_CBuffer.BaseColor.b);

			//MAYAのディフューズがゼロになる対策
			float color = 0.0f;
			color += materialArray[mc].Material_CBuffer.BaseColor.r;
			color += materialArray[mc].Material_CBuffer.BaseColor.g;
			color += materialArray[mc].Material_CBuffer.BaseColor.b;
			if (color <= 0.0f)
			{
				materialArray[mc].Material_CBuffer.BaseColor.r = 1.0f;
				materialArray[mc].Material_CBuffer.BaseColor.g = 1.0f;
				materialArray[mc].Material_CBuffer.BaseColor.b = 1.0f;
			}

			materialArray[mc].Material_CBuffer.BaseColor.a = 1.0f;
		}
		else if (strcmp(str, "Ks") == 0)
		{
			////スペキュラ（反射光）
			//fscanf(file, "%f", &materialArray[mc].Material_CBuffer.Specular.r);
			//fscanf(file, "%f", &materialArray[mc].Material_CBuffer.Specular.g);
			//fscanf(file, "%f", &materialArray[mc].Material_CBuffer.Specular.b);
			//materialArray[mc].Material_CBuffer.Specular.a = 1.0f;
		}
		else if (strcmp(str, "Ns") == 0)
		{
			//スペキュラ強度
			fscanf(file, "%f", &materialArray[mc].Material_CBuffer.Specular);
		}
		else if (strcmp(str, "d") == 0)
		{
			//アルファ
			fscanf(file, "%f", &materialArray[mc].Material_CBuffer.BaseColor.a);
		}
		else if (strcmp(str, "map_Kd") == 0)
		{
			//テクスチャ
			fscanf(file, "%s", str);

			char path[256];
			strcpy(path, "asset/texture/");
			strcat(path, str);

			strcat(materialArray[mc].TextureName, path);

			materialArray[mc].Material_CBuffer.noTexSampling = 0;
		}
		//エミッシブ（自己発光）
		materialArray[mc].Material_CBuffer.UseAlbedoMap = false;
		materialArray[mc].Material_CBuffer.UseAoMap = false;
		materialArray[mc].Material_CBuffer.UseEmmisive = false;
		materialArray[mc].Material_CBuffer.UseOccMetalRough = false;
		materialArray[mc].Material_CBuffer.NormalState = NORMAL_UNUSED;

	}


	*MaterialArray = materialArray;
	*MaterialNum = materialNum;

	fclose(file);
}

void ManagerModel::Init()
{
	if (!m_ModelCBuffer) {
		m_ModelCBuffer = new CBuffer(CBuffer::CreateBuffer(sizeof(MATERIAL_CBUFFER), D3D11_BIND_CONSTANT_BUFFER, nullptr));
	}
}

DX11_MODEL* ManagerModel::Load(std::string  file)
{
	DX11_MODEL* ret;

	if (m_ModelList.count(file) != 0) {
		
		/*memcpy(ret, m_ModelList[file], sizeof(DX11_MODEL));
		ret->SubsetArray = nullptr;
		ret->SubsetArray = new DX11_SUBSET;
		memcpy(ret->SubsetArray, m_ModelList[file]->SubsetArray, sizeof(DX11_SUBSET));*/
		ret = m_ModelList[file];
		return ret;
	}
	/*ret = ;*/
	ret = new DX11_MODEL();
	m_ModelList[file] = ret;
	MODEL model;

	LoadObj(file.c_str(), &model);

	// 頂点バッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;
		CDxRenderer::GetRenderer()->GetDevice()->CreateBuffer(&bd, &sd, &ret->VertexBuffer);
	}


	// インデックスバッファ生成
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		CDxRenderer::GetRenderer()->GetDevice()->CreateBuffer(&bd, &sd, &ret->IndexBuffer);
	}

	// サブセット設定
	{
		ret->SubsetArray = new DX11_SUBSET[model.SubsetNum];
		ret->SubsetNum = model.SubsetNum;

		for (unsigned int i = 0; i < model.SubsetNum; i++)
		{
			ret->SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			ret->SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			ret->SubsetArray[i].Material.Material_CBuffer = model.SubsetArray[i].Material.Material_CBuffer;

			std::string filename = model.SubsetArray[i].Material.TextureName;
			std::stringstream ss{ filename };
			std::string buf;

			std::vector<std::string> v;

			while (std::getline(ss, buf, '.')) {
				v.push_back(buf);
			}

			std::wstring result = std::wstring(filename.begin(), filename.end());
			LPCWSTR str = result.c_str();

			DirectX::ScratchImage image;
			DirectX::TexMetadata metadata;
			HRESULT hr;
			if (!v.empty()) {
				if (v.back() == "hdr") {
					hr = DirectX::LoadFromHDRFile(str, &metadata, image);
				}
				else {
					hr = DirectX::LoadFromWICFile(str, DirectX::WIC_FLAGS_NONE, &metadata, image);
				}
			}
			DirectX::CreateShaderResourceView(CDxRenderer::GetRenderer()->GetDevice(),
				image.GetImages(),
				image.GetImageCount(),
				image.GetMetadata(),
				&ret->SubsetArray[i].Material.Texture);


			ret->SubsetArray[i].Material.AlbedoTexture = nullptr;
			ret->SubsetArray[i].Material.AoTexture = nullptr;
			ret->SubsetArray[i].Material.EmissiveTexture = nullptr;
			ret->SubsetArray[i].Material.NormalTexture = nullptr;
			ret->SubsetArray[i].Material.OccMetalRoughTexture = nullptr;
			/*D3DX11CreateShaderResourceViewFromFile(CDxRenderer::GetRenderer()->GetDevice(),
				model.SubsetArray[i].MATERIAL_CBUFFER.TextureName,
				NULL,
				NULL,
				&ret->SubsetArray[i].MATERIAL_CBUFFER.Texture,
				NULL);*/
		}
	}
	
	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;
	

	return ret;
}

void ManagerModel::Draw(DX11_MODEL *Model)
{
	// 頂点バッファ設定
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetVertexBuffers(0, 1, &Model->VertexBuffer, &stride, &offset);

	// インデックスバッファ設定
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetIndexBuffer(Model->IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// プリミティブトポロジ設定
	CDxRenderer::GetRenderer()->GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	for (unsigned int i = 0; i < Model->SubsetNum; i++)
	{
		// マテリアル設定
		//CDxRenderer::GetRenderer()->SetMaterial(Model->SubsetArray[i].MATERIAL_CBUFFER.MATERIAL_CBUFFER);
		m_ModelCBuffer->UpdateBuffer(&(Model->SubsetArray[i].Material.Material_CBuffer));
		m_ModelCBuffer->VSSetCBuffer(3);
		m_ModelCBuffer->PSSetCBuffer(3);

		// テクスチャ設定
		if (Model->SubsetArray[i].Material.Material_CBuffer.noTexSampling == 0)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(0, 1, &Model->SubsetArray[i].Material.Texture);
		if(Model->SubsetArray[i].Material.Material_CBuffer.UseAlbedoMap == 1)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(1, 1, &Model->SubsetArray[i].Material.AlbedoTexture);
		if (Model->SubsetArray[i].Material.Material_CBuffer.UseOccMetalRough == 1)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(2, 1, &Model->SubsetArray[i].Material.OccMetalRoughTexture);
		if (Model->SubsetArray[i].Material.Material_CBuffer.UseAoMap == 1)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(3, 1, &Model->SubsetArray[i].Material.AoTexture);
		if (Model->SubsetArray[i].Material.Material_CBuffer.UseEmmisive == 1)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(4, 1, &Model->SubsetArray[i].Material.EmissiveTexture);
		if (Model->SubsetArray[i].Material.Material_CBuffer.NormalState != NORMAL_UNUSED)
			CDxRenderer::GetRenderer()->GetDeviceContext()->PSSetShaderResources(5, 1, &Model->SubsetArray[i].Material.NormalTexture);

		// ポリゴン描画
		CDxRenderer::GetRenderer()->GetDeviceContext()->DrawIndexed(Model->SubsetArray[i].IndexNum, Model->SubsetArray[i].StartIndex, 0);
	}

	
	
}

void ManagerModel::Unload()
{
	for (auto obj : m_ModelList) {
		obj.second->VertexBuffer->Release();
		obj.second->IndexBuffer->Release();


		for (unsigned int i = 0; i < obj.second->SubsetNum; i++) {
			if(obj.second->SubsetArray[i].Material.Texture)
				obj.second->SubsetArray[i].Material.Texture->Release();
			if(obj.second->SubsetArray[i].Material.AlbedoTexture)
				obj.second->SubsetArray[i].Material.AlbedoTexture->Release();
			if(obj.second->SubsetArray[i].Material.OccMetalRoughTexture)
				obj.second->SubsetArray[i].Material.OccMetalRoughTexture->Release();
			if(obj.second->SubsetArray[i].Material.AoTexture)
				obj.second->SubsetArray[i].Material.AoTexture->Release();
			if(obj.second->SubsetArray[i].Material.EmissiveTexture)
				obj.second->SubsetArray[i].Material.EmissiveTexture->Release();
			if(obj.second->SubsetArray[i].Material.NormalTexture)
				obj.second->SubsetArray[i].Material.NormalTexture->Release();


		}
		delete[] obj.second->SubsetArray;

		delete obj.second;
	}

	m_ModelList.clear();

	delete m_ModelCBuffer;

}