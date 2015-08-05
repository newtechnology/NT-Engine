

struct VS_IN
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float2 Tex      : TEXCOORD;
};

struct VS_OUT
{
	float4 PosH : SV_POSITION;
	float2 Tex  : TEXCOORD;
};


VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

	vout.PosH = float4(vin.PosL, 1.0f);

	return vout;
}