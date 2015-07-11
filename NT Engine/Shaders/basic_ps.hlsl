#include "Lights.hlsli"

cbuffer cbPerFrame : register(b0)
{
	DirectionalLight DirLight;

	float3 CameraPosW;
	float Pad;
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

	float4 FinalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 toEye = CameraPosW - pin.PosW;

	float DistanceToEye = length(toEye);

	//normalize
	toEye /= DistanceToEye;

	float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	ComputeDirectionalLight(ObjectMaterial, DirLight, pin.NormalW, toEye, ambient, diffuse, spec);

	FinalColor = TexColor * (ambient + diffuse) + spec;

	FinalColor.a = ObjectMaterial.Diffuse.a * TexColor.a;

	return FinalColor;
}