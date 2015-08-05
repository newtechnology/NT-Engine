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

SamplerState samLinear : register(s0);

Texture2D DiffuseMap : register(t0);


struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
};


float4 PS(VertexOut pin) : SV_Target
{
	       
	pin.NormalW = normalize(pin.NormalW);

	float4 TexColor = DiffuseMap.Sample(samLinear, pin.Tex);

	float4 FinalColor = TexColor;

	float3 toEye = CameraPosW - pin.PosW;

	float DistanceToEye = length(toEye);

	//normalize
	toEye /= DistanceToEye;

	if (NumDirLights > 0)
	{
		float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
		float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

		[unroll]
		for (int i = 0; i < NumDirLights; ++i)
		{
			float4 A, D, S;
			ComputeDirectionalLight(ObjectMaterial, DirLights[i], pin.NormalW, toEye, A, D, S);

			ambient += A;
			diffuse += D;
			spec += S;
		}

		FinalColor = TexColor * (ambient + diffuse) + spec;

		FinalColor.a = ObjectMaterial.Diffuse.a * TexColor.a;
	}

	return FinalColor;
}