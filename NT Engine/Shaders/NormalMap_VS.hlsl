struct VS_IN
{
	float3 PosL : POSITION;
	float3 NormalL : NORMAL;
	float2 Tex : TEXCOORD;
	float3 TangentL : TANGENT;
};


struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float3 PosW : POSITION;
	float3 NormalW : NORMAL;
	float2 Tex : TEXCOORD;
	float3 TangentW : TANGENT;
};


cbuffer cbPerObject : register(b0)
{
	float4x4 World;
	float4x4 WorldViewProj;
	float4x4 WorldInvTranspose;
	float4x4 TexTransform;
};



VS_OUT main(VS_IN vin)
{
	VS_OUT vout; 

	vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj);
	vout.PosW = mul(float4(vin.PosL, 1.0f), World).xyz;
	vout.NormalW = mul(vin.NormalL, (float3x3)WorldInvTranspose);
	vout.TangentW = mul(vin.TangentL, (float3x3)World);
	vout.Tex = mul(float4(vin.Tex, 0.0f, 1.0f), TexTransform).xy;

	return vout;
}