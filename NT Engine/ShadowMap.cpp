#include "ShadowMap.h"


namespace NTEngine
{
	ShadowMap::ShadowMap(ID3D11Device* device, UINT Width, UINT Height)
		: m_Width(Width), 
		m_Height(Height),
		m_DepthMapDSV(nullptr),
		m_DepthMapSRV(nullptr)
	{
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;
		m_Viewport.Width = static_cast<FLOAT>(m_Width);
		m_Viewport.Height = static_cast<FLOAT>(m_Height);
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.MaxDepth = 1.0f;
		
		D3D11_TEXTURE2D_DESC td;
		td.Width = m_Width;
		td.Height = m_Height;
		td.MipLevels = 1;
		td.ArraySize = 1;
		td.Format = DXGI_FORMAT_R24G8_TYPELESS;
		td.SampleDesc.Count = 1;
		td.SampleDesc.Quality = 0;
		td.Usage = D3D11_USAGE_DEFAULT;
		td.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
		td.CPUAccessFlags = 0;
		td.MiscFlags = 0;

		ID3D11Texture2D* DepthMap = nullptr;
		HR(device->CreateTexture2D(&td, 0, &DepthMap));

		D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
		dsvDesc.Flags = 0;
		dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		dsvDesc.Texture2D.MipSlice = 0;
		HR(device->CreateDepthStencilView(DepthMap, &dsvDesc, &m_DepthMapDSV));

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = td.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;
		HR(device->CreateShaderResourceView(DepthMap, &srvDesc, &m_DepthMapSRV));

		ReleaseCOM(DepthMap);
	}

	ShadowMap::~ShadowMap()
	{
		ReleaseCOM(m_DepthMapDSV);
		ReleaseCOM(m_DepthMapSRV);
	}

	ID3D11ShaderResourceView* ShadowMap::GetDepthMap()
	{
		return m_DepthMapSRV;
	}

	void ShadowMap::PrepareForRendering(ID3D11DeviceContext* dc)
	{
		dc->RSSetViewports(1, &m_Viewport);

		//set null RenderTargetView to disable color writing
		ID3D11RenderTargetView* RTV[1] = { nullptr };

		dc->OMSetRenderTargets(1, RTV, m_DepthMapDSV);

		//Clear Depth/Stencil view to default value
		dc->ClearDepthStencilView(m_DepthMapDSV, D3D11_CLEAR_DEPTH, 1.0f, 0);
	}
}