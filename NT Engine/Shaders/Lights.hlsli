
struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float pad;
};


struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular; // w = SpecPower
	float4 Reflect;
};


void ComputeDirectionalLight(Material mat, DirectionalLight L,
	float3 Normal, float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	float3 lightVector = -L.Direction;

	ambient = mat.Ambient * L.Ambient;

	float diffuseFactor = dot(lightVector, Normal); //cosine of angle between light vector and normal

	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float v = reflect(-lightVector, Normal);
		float specFactor = pow(saturate(dot(v, toEye)), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}


}