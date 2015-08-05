#include "Lights.hlsli"


cbuffer cbPerFrame : register(b0)
{
	DirectionalLight DirLights[MAX_DIR_LIGHTS];

	float3 CameraPosW;
	int NumDirLights;
};


cbuffer cbPerObject : register(b1)
{
	Material ObjectMaterial;
};


struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD;
	float3 TangentW : TANGENT;
};


Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);

sampler Sampler : register(s0);
SamplerComparisonState SamplerShadow : register(s1);


float4 main(VS_OUT pin) : SV_TARGET
{
	pin.NormalW = normalize(pin.NormalW);

	float4 TexColor = DiffuseMap.Sample(Sampler, pin.Tex);

	float4 FinalColor = TexColor;

	float3 toEye = CameraPosW - pin.PosW;

	float DistanceToEye = length(toEye);

	//normalize
	toEye /= DistanceToEye;


	float3 normalMapSample = NormalMap.Sample(Sampler, pin.Tex).rgb;

	float3 bumpedNormalW = NormalSampleToWorldSpace(normalMapSample, pin.NormalW, pin.TangentW);

	if (NumDirLights > 0)
	{
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		    [unroll]
			for (int i = 0; i < NumDirLights; ++i)
			{
				float4 A, D, S;
				ComputeDirectionalLight(ObjectMaterial, DirLights[i], bumpedNormalW, toEye, A, D, S);

				ambient += A;
				diffuse += D;
				spec += S;
			}

		FinalColor = TexColor * (ambient + diffuse) + spec;

		FinalColor.a = ObjectMaterial.Diffuse.a * TexColor.a;
	}

	
	

	return FinalColor;
}