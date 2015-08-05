#include "SamplerStates.h"


namespace NTEngine
{
	ID3D11SamplerState* SamplerStates::SamLinear = nullptr;
	ID3D11SamplerState* SamplerStates::SamShadow = nullptr;
	
	void SamplerStates::Initialize(ID3D11Device* dev)
	{
		D3D11_SAMPLER_DESC sd;

		ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
		sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		sd.MipLODBias = 0.0f;
		sd.MaxAnisotropy = 4;
		sd.ComparisonFunc = D3D11_COMPARISON_NEVER;
		sd.BorderColor[0] = 0;
		sd.BorderColor[1] = 0;
		sd.BorderColor[2] = 0;
		sd.BorderColor[3] = 0;
		sd.MinLOD = 0;
		sd.MaxLOD = D3D11_FLOAT32_MAX;

		HR(dev->CreateSamplerState(&sd, &SamLinear));

		ZeroMemory(&sd, sizeof(D3D11_SAMPLER_DESC));
		sd.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		sd.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
		sd.ComparisonFunc = D3D11_COMPARISON_LESS;
		sd.BorderColor[0] = 0;
		sd.BorderColor[1] = 0;
		sd.BorderColor[2] = 0;
		sd.BorderColor[3] = 0;
		sd.MinLOD = 0;
		sd.MaxLOD = D3D11_FLOAT32_MAX;

		HR(dev->CreateSamplerState(&sd, &SamShadow));
	}

	void SamplerStates::Destroy()
	{
		ReleaseCOM(SamLinear);
		ReleaseCOM(SamShadow);
	}
}