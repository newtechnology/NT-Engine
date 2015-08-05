#include "RasterizerStates.h"



namespace NTEngine
{
	ID3D11RasterizerState* RasterizerStates::ShadowMapDepth = nullptr;

	void RasterizerStates::Initialize(ID3D11Device* dev)
	{
		D3D11_RASTERIZER_DESC rd;
		ZeroMemory(&rd, sizeof(D3D11_RASTERIZER_DESC));

		rd.DepthBias = 100000;
		rd.DepthBiasClamp = 0.0f;
		rd.SlopeScaledDepthBias = 1.0f;

		HR(dev->CreateRasterizerState(&rd, &ShadowMapDepth));
	}

	void RasterizerStates::Destroy()
	{
		ReleaseCOM(ShadowMapDepth);
	}
}