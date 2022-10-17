

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register(b0)
{
	matrix World;
}

cbuffer ViewBuffer : register(b1)
{
	matrix View;
}

cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

// �}�e���A���o�b�t�@
struct MATERIAL
{
	float4  	BaseColor;
	float4  	Normal;
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

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL	Material;
}

cbuffer InverseWorldBuffer : register(b6)
{
	matrix InverseWorld;
}


struct Output_VS 
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 texcoord : TEXCOORD0;
};

struct Output_PS {
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 rough_meta_spe : SV_Target2;
	float4 emmision : SV_Target3;
};

//=============================================================================
// ���_�V�F�[�_
//=============================================================================
Output_VS VS_main(in  float3 inPosition		: POSITION0,
	in  float3 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0,
	in	float3 inTangent : TANGENT,
	in  float3 inBinormal : BINORMAL
)
{
	Output_VS output;
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	float4 pos = float4(inPosition, 1.0f);
	
	output.pos = mul(pos, wvp);

	float4 worldNormal, normal;
	normal = float4(inNormal, 0.0f);
	worldNormal = mul(normal, InverseWorld);
	worldNormal = normalize(worldNormal);
	output.normal = worldNormal.xyz;
	
	
	output.texcoord = inTexCoord;

	output.color = inDiffuse;
	return output;
}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture : register(t0);
Texture2D		g_AlbedoTexture : register(t1);
Texture2D		g_OccMetalRoughTexture : register(t2);
Texture2D		g_AoTexture : register(t3);
Texture2D		g_EmissiveTexture : register(t4);
Texture2D		g_NormalTexture : register(t5);

SamplerState	g_SamplerState : register(s0);


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
Output_PS PS_main(Output_VS a)
{
	Output_PS output;
	float4 color = 0.0;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, a.texcoord);
		//color = Material.Roughness;

		//color = float4(1.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		//color = a.color;
		color = float4(1.0f,1.0f, 1.0f, 1.0f);
		
	}

	
	
	output.color = color;
	output.normal = float4(a.normal* 0.5f + 0.5f, 1);
	//output.normal = float4(a.normal, 1);
	output.rough_meta_spe = float4(Material.Roughness, Material.Metaric, Material.Specular, 1.0f);

	//output.color = float4(Material.Metaric, Material.Metaric, Material.Metaric, 1.0f);
	//output.color = float4(.0f, 0.0f, 1.0f, 1.0f);
	return output;
}
