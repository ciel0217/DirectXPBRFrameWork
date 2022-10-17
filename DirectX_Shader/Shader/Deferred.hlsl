#include "NDCFullScreen.hlsli"
#include "BRDF.hlsli"
#include "CalcLight.hlsli"

cbuffer ViewBuffer : register(b7)
{
	matrix View;
}

cbuffer ProjectionBuffer : register(b8)
{
	matrix Projection;
}

cbuffer CameraPos : register(b5)
{
	float4 CameraPos;
}

struct DEFERRED_CBUFFER
{
	int UseAO;
	int UseEnvMap;
	int Dummy[2];
};

cbuffer DeferredBuffer : register(b9)
{
	DEFERRED_CBUFFER DeferredBuffer;
}

struct Output_VS
{
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD0;
	
};

struct Output_PS {
	float4 color : SV_Target0;
};



//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
Texture2D		g_ColorTex			: register(t0);
Texture2D		g_NormalTex			: register(t1);
Texture2D		g_RoughMetaSpeTex	: register(t2);
Texture2D		g_EmissionTex		: register(t3);
Texture2D		g_DepthStencilTex	: register(t4);


TextureCube		g_EnvMapTex			: register(t8);
TextureCube		g_IrradianceTex		: register(t9);
TextureCube		g_SpecularMap		: register(t10);
Texture2D		g_BrdfLUTex			: register(t11);

SamplerState	g_SamplerState	: register(s0);


float4 CalculateWorldFromDepth(float2 texcoord, float depth)
{
	// -1 ~ 1�̃N���b�v���
	// y�𔽓]�����āA������ɂ���
	float2 clipXY = texcoord * 2.0 - 1.0;
	clipXY.y = -clipXY.y;

	// NOTE : �[�x�l�����`����Ȃ�
	// �͈�[0 ~ 1]
	float4 clipSpace = float4(clipXY, depth, 1);
	
	float4 viewSpace = mul(Projection, clipSpace);
	

	// �N���b�v���W��w�Ŋ����NDC���W�ɂȂ�
	viewSpace /= viewSpace.w;

	float4 worldSpace = mul(View, viewSpace);

	return worldSpace;
}


Output_VS VS_main(
	uint VertexID : SV_VertexID
	) 
{
	Output_VS output;
	output.pos = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
	output.texcoord = arrUV[VertexID].xy;

	return output;
}


//=============================================================================
// �s�N�Z���V�F�[�_
//=============================================================================
Output_PS PS_main(Output_VS vs)
{
	Output_PS output;
	float3 albedo = g_ColorTex.Sample(g_SamplerState, vs.texcoord).rgb;
	float depth = g_DepthStencilTex.Sample(g_SamplerState, vs.texcoord).r;
	float3 RoughMetaSpe = g_RoughMetaSpeTex.Sample(g_SamplerState, vs.texcoord).rgb;
	float roughness = RoughMetaSpe.r;
	float metalic = RoughMetaSpe.g;
	float specular = RoughMetaSpe.b;
	float3 emmisive = g_EmissionTex.Sample(g_SamplerState, vs.texcoord).rgb;

	float3 normal =  g_NormalTex.Sample(g_SamplerState, vs.texcoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	
	SurfaceInfo surf;
	surf.WorldPosition = CalculateWorldFromDepth(vs.texcoord, depth);
	surf.Normal = normal;
	surf.EyeV = normalize(CameraPos.xyz - surf.WorldPosition.xyz);
	//surf.EyeV = normalize(surf.WorldPosition.xyz - CameraPos.xyz);
	surf.NdotV = dot(surf.Normal, surf.EyeV);

	float3 Lo = float3(.0f, .0f, .0f);
	float3 F0 = float3(.04f, .04f, .04f);

	F0 = lerp(F0, albedo, metalic);

	for (int i = 0; i < LIGHT_MAX; i++) {
		if (Lights[i].Status == LIGHT_DISABLE) 
			continue;
		LightingInfo light_info;
		CalcLight(surf, Lights[i], light_info);

		float d = NormalDistributionGGX(saturate(light_info.NdotH), roughness);
		float g = GeometrySmith(saturate(surf.NdotV), saturate(light_info.NdotL), roughness);
		float3 f = FresnelSchlick(F0, saturate(dot(light_info.HalfV, surf.EyeV)));

		float3 ks = f;
		float3 kd = (1.0f - ks) * (1.0f - metalic);

		float3 numerator = d * g * f;
		float denominator = 4.0f * saturate(surf.NdotV) * saturate(light_info.NdotL);
		float3 specularBRDF = numerator / max(denominator, 0.001f);
		Lo += (kd * albedo / PI + specularBRDF) * Lights[i].Diffuse.rgb * light_info.Attenuation * saturate(light_info.NdotL);

	}

	float3 ambient = GlobalAmbient.rgb * albedo;
	if (DeferredBuffer.UseEnvMap == 1) {
		float3 ks = FresnelSchlickRoughness(F0, saturate(surf.NdotV), roughness);
		float3 kd = (1.0f - ks);
		kd *= 1.0f - metalic;
		
		float3 irradiance = g_IrradianceTex.Sample(g_SamplerState, surf.Normal).rgb;
		float3 diffuse = irradiance * albedo;

		float3 R = normalize(reflect(-surf.EyeV, surf.Normal));
		float3 prefiltercolor = g_SpecularMap.SampleLevel(g_SamplerState, R, roughness * 5.0f).rgb;//5.0f�́A���O�v�Z�̎��� i / 5.0f�Ń��t�l�X���v�Z��������
		float2 brdflut = g_BrdfLUTex.Sample(g_SamplerState, float2(saturate(surf.NdotV), roughness)).rg;
		
		float3 specular = prefiltercolor * (ks * brdflut.x + brdflut.y);
		ambient = (kd * diffuse + specular);
	
		
	}
	

	//output.color = float4(F0 , 1.0f);
	
	output.color = float4(Lo + ambient + emmisive, 1.0f);
	

	//output.color = float4(depth, depth, depth, 1.0f);
//	output.color = float4(ambient, 1.0f);
	
	/*if (.Enable == 1) {
		for (int i = 0; i < 10; i++) {
			if (Light.Flags[i].y == 1) {
				
			}
		}
	}*/
	//output.color = g_ColorTex.Sample(g_SamplerState, vs.texcoord);
	
	/*if (output.color.a < 0.15f)discard;*/

	//float3 norm = normalize(vs.pos - float4(.0f, 0.0f, .0f, 1.0f));
	//float3 a = normalize(vs.normal);
	//float3 R = reflect(norm, a);
//	output.color = g_CubeMapTex.Sample(g_SamplerState, vs.reflect);
	
	//output.color = g_RoughMetaSpeTex.Sample(g_SamplerState, vs.texcoord);
	//output.color = g_NormalTex.Sample(g_SamplerState, vs.texcoord);
	//output.color = g_SpecularTex.Sample(g_SamplerState, vs.texcoord);

	
	return output;

}
