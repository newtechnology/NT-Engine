
/*cbuffer cbPerObject
{
	float4x4 gWorldViewProj;
}; */

struct VertexIn
{
	float3 PosL : POSITION;
	float4 Color : COLOR;
};

struct VertexOut
{
	float4 PosH : SV_POSITION;  
	float4 Color : COLOR;
};


VertexOut VS(VertexIn vin) 
{
	VertexOut vout;


	vout.PosH = float4(vin.PosL, 1.0f);  //mul(float4(vin.PosL, 1.0f), gWorldViewProj);
	 
	//pass the color
	vout.Color = vin.Color; 

	return vout;
}


