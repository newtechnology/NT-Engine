

#define MAX_DIR_LIGHTS 8


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


float3 NormalSampleToWorldSpace(float3 NormalSampled, float3 UnitNormalW, float3 TangentW)
{
	//Uncompress from [0, 1] to [-1, 1]
	float3 normalT = 2.0f * NormalSampled - 1.0f;

	//Build orthonormal basis vectors
	float3 N = UnitNormalW;
	float3 T = normalize(TangentW - dot(TangentW, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 bumpedNormalW = mul(normalT, TBN);

	return bumpedNormalW;
}

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
		float3 v = reflect(-lightVector, Normal);
		float specFactor = pow(saturate(dot(v, toEye)), mat.Specular.w);

		diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
		spec = specFactor * mat.Specular * L.Specular;
	}


}

static const float SMAP_SIZE = 2048.0f;
static const float SMAP_DX = 1.0f / SMAP_SIZE;

#define NUM_SAMPLES 9 //3*3

float CalculateShadowFactor(SamplerComparisonState SamShadow, Texture2D ShadowMap, float4 ShadowPosH)
{
	//Depth in NDC Space
	float Depth = ShadowPosH.z;

	const float dx = SMAP_DX;
	float PercentLit = 0.0f;


	const float2 offsets[NUM_SAMPLES] =
	{
		float2(-dx, -dx), float2(0.0f, -dx), float2(dx, -dx),
		float2(-dx, 0.0f), float2(0.0f, 0.0f), float2(dx, 0.0f),
		float2(-dx, +dx), float2(0.0f, +dx), float2(dx, +dx)
	};

	[unroll]
	for (int i = 0; i < NUM_SAMPLES; ++i)
	{
		PercentLit += ShadowMap.SampleCmpLevelZero(SamShadow, ShadowPosH.xy + offsets[i], Depth).r;
	}

	return PercentLit /= NUM_SAMPLES;
}