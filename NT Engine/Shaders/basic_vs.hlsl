
cbuffer cbPerObject : register (b0)
{
	float4x4 World;
	float4x4 WorldInvTranspose;
	float4x4 WorldViewProj;
	float4x4 TexTransform;
};


struct VertexIn
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosW    : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex     : TEXCOORD;
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	//transform to world space
	vout.PosW = mul(float4(vin.PosL, 1.0f), World).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)WorldInvTranspose);

	//transform to homogeneous clip space
	vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj);

	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), TexTransform).xy;

	return vout;
}