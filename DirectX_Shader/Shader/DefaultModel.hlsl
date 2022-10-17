

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
	//float		Dummy[2];	//16byte���E�p
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL	Material;
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
	output.normal = normalize(mul(float4(inNormal, .0), World).xyz);
	output.texcoord = inTexCoord;

	output.color = inDiffuse;
	return output;
}



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
Output_PS PS_main(Output_VS a)
{
	Output_PS output;
	float4 color;

	if (Material.noTexSampling == 0)
	{
		color = g_Texture.Sample(g_SamplerState, a.texcoord);
		//color = Material.Roughness;

		//color = float4(1.0f, 0.0f, 1.0f, 1.0f);
	}
	else
	{
		color = a.color;
		color = float4(1.0f, 0.0f, 1.0f, 1.0f);

	}

	output.color = color;
	


	return output;
}
