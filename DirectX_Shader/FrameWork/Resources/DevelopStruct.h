#pragma once
#include "DirectX_Shader/FrameWork/LowLevel/DirectX.h"
//*********************************************************
// �}�N����`
//*********************************************************
#define LIGHT_MAX		(10)


enum BLEND_MODE
{
	BLEND_MODE_NONE,		//�u�����h����
	BLEND_MODE_ALPHABLEND,	//���u�����h
	BLEND_MODE_ADD,			//���Z�u�����h
	BLEND_MODE_SUBTRACT,	//���Z�u�����h

	BLEDD_MODE_NUM
};

enum CULL_MODE
{
	CULL_MODE_NONE,			//�J�����O����
	CULL_MODE_FRONT,		//�\�̃|���S����`�悵�Ȃ�(CW)
	CULL_MODE_BACK,			//���̃|���S����`�悵�Ȃ�(CCW)

	CULL_MODE_NUM
};

enum ADDRESS_MODE
{
	ADDRESS_MODE_WRAP,
	ADDRESS_MODE_MIRROR,
	ADDRESS_MODE_CLAMP,
	ADDRESS_MODE_BORDER,

	ADDRESS_MODE_NUM
};

enum FILTER_MODE
{
	FILTER_MODE_POINT,
	FILTER_MODE_LINEAR,
	FILTER_MODE_ANISOTROPIC,

	FILTER_MODE_NUM
};

enum SCENE_TEXTURE 
{
	eAlbedo = 0,
	eNormal,
	eRoughnessAndMeralicAndSpecular,
	eEmmision,

	eIntermediate,
	eSceneMax,
};

//*********************************************************
// �\����
//*********************************************************

// ���_�\����
struct VERTEX_3D
{
	D3DXVECTOR3 Position;//���W
	D3DXVECTOR3 Normal;//�@��
	D3DXVECTOR2 TexCoord;//UV�l
	D3DXCOLOR   Diffuse;//���_�J���[
	D3DXVECTOR3 Tangent;//tangent
	D3DXVECTOR3 Binormal;//�]�@��
	
};

// �}�e���A���p�萔�o�b�t�@�\����
struct MATERIAL_CBUFFER
{
	D3DXCOLOR  	BaseColor;
	D3DXCOLOR  	Normal;
	float    	Roughness;
	float    	Metaric;
	float   	Specular;
	int			noTexSampling;
	int			UseAlbedoMap;
	int			UseOccMetalRough;
	int			UseAoMap;
	int			UseEmmisive;
	int			NormalState;
	int			Dummy[3];
};

// ���C�g�\����
struct LIGHT {
	D3DXVECTOR4 Direction;	// ���C�g�̕���
	D3DXVECTOR4 Position;	// ���C�g�̈ʒu
	D3DXCOLOR	Diffuse;	// �g�U���̐F
	D3DXCOLOR   Ambient;	// �����̐F
	float		Attenuation;// ������
	float		ConstantAtt;
	float		LinearAtt;
	float		QuadAtt;
	float		SpotAngle;  //�X�|�b�g���C�g�̎��̊p�x
	int			Status;		// ���C�g���
	int			LightType;		// �L���t���O
	float		Dummy;
};

// ���C�g�p�萔�o�b�t�@�\����
struct LIGHT_CBUFFER
{
	D3DXCOLOR GlobalAmbient;
	LIGHT Lights[LIGHT_MAX];
};

struct DEFERRED_CBUFFER
{
	int UseAO = 0;
	int UseEnvMap = 0;
	int Dummy[2];
};

struct RenderTarget
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> RenderTargetTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> RenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> UnorderedAccessView;
};

struct VertexShaderRelated {
	Microsoft::WRL::ComPtr<ID3D11VertexShader> VertexShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>  Layout;
	Microsoft::WRL::ComPtr<ID3D11ShaderReflection> ShaderRF;
};

