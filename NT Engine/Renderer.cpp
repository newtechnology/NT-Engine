#include "Renderer.h"
#include "Shaders.h"
#include "SamplerStates.h"
#include "TextureManager.h"
#include "TriangleDemo.h"


#if defined(DEBUG) | defined(_DEBUG) 
#include <sstream>
#endif


namespace NTEngine
{
	Renderer::Renderer()
	{
		m_Device = nullptr;
		m_DeviceContext = nullptr;
		m_SwapChain = nullptr;

		m_RenderTargetView = nullptr;
		m_DepthStencilView = nullptr;

		m_DepthStencilBuffer = nullptr;

		//default resolution
		m_ScreenWidth = 800;
		m_ScreenHeight = 600;

		//default count
		m_MultiSampleCount = 4;
		m_MultiSampleQuality = 0;
	}


	Renderer::~Renderer()
	{
		for (UINT i = 0; i < m_Models.size(); ++i)
		{
			SafeDelete(m_Models[i]);
		}
	}


	void Renderer::Initialize(HWND HWnd, UINT Width, UINT Height)
	{
		m_ScreenWidth = Width;
		m_ScreenHeight = Height;


		UINT CreateDeviceFlags = 0;


#if defined(DEBUG) | defined(_DEBUG)
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

#ifdef SINGLE_THREADED
		CreateDeviceFlags |= D3D11_CREATE_DEVICE_SINGLETHREADED;
#endif //SINGLE_THREADED

		D3D_FEATURE_LEVEL featureLevel;

		HRESULT hr = D3D11CreateDevice(NULL /*Primary Adapter*/,
			D3D_DRIVER_TYPE_HARDWARE,
			0,
			CreateDeviceFlags,
			NULL, NULL,
			D3D11_SDK_VERSION,
			&m_Device,
			&featureLevel,
			&m_DeviceContext);

		if (FAILED(hr))
			ShowError("D3D11CreateDevice() Failed.");

		if (featureLevel != D3D_FEATURE_LEVEL_11_0)
			ShowError("D3D_FEATURE_LEVEL_11_0 (DirectX 11) is not supported. This application won't run on your system.");


		HR(m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, m_MultiSampleCount, &m_MultiSampleQuality));

		assert(m_MultiSampleQuality > 0);


		//describe our swap chain
		DXGI_SWAP_CHAIN_DESC sd;


		sd.BufferDesc.Width = m_ScreenWidth;
		sd.BufferDesc.Height = m_ScreenHeight;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

		if (m_EnableMSAA)
		{
			sd.SampleDesc.Count = m_MultiSampleCount;
			sd.SampleDesc.Quality = m_MultiSampleQuality - 1;
		}
		else
		{
			sd.SampleDesc.Count = 1;
			sd.SampleDesc.Quality = 0;
		}

		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1; //Total 2 buffers: buffer 0 and buffer 1
		sd.OutputWindow = HWnd;
		sd.Windowed = TRUE;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
		sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


		IDXGIDevice* dxgiDevice = nullptr;
		HR(m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

		IDXGIAdapter* dxgiAdapter = nullptr;
		HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter));

		IDXGIFactory* dxgiFactory = nullptr;
		HR(dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

		HR(dxgiFactory->CreateSwapChain(m_Device, &sd, &m_SwapChain));

		ReleaseCOM(dxgiDevice);
		ReleaseCOM(dxgiAdapter);
		ReleaseCOM(dxgiFactory);

		//The remaining steps that need to be carried out for DirectX 11 Initialization 
		//also need to be carried out when our screen is resized
		//So, instead of duplication the code, we just call OnResize() function
		OnResize();

		Shaders::Initialize(m_Device, m_DeviceContext);
		InputLayoutDesc::Initialize();
		InputLayouts::Initialize(m_Device);
		SamplerStates::Initialize(m_Device);
		TextureManager::Instance()->Initialize(m_Device);

		Lights::Material mat;

		mat.Ambient = XMFLOAT4(0.4f, 0.4f, 0.4f, 1.0f);
		mat.Diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		mat.Specular = XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f);
		mat.Reflect = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

		Shaders::Basic->SetPerObjectData(mat);
	}

	void Renderer::OnResize()
	{
		//make sure these COMs are valid!
		assert(m_Device);
		assert(m_DeviceContext);
		assert(m_SwapChain);

		//Release the old view
		ReleaseCOM(m_RenderTargetView);
		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);

		//Re-size swapchain and re-create the RenderTargetView

		HR(m_SwapChain->ResizeBuffers(1, m_ScreenWidth, m_ScreenHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
		ID3D11Texture2D* backbuffer = nullptr;
		HR(m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backbuffer)));
		HR(m_Device->CreateRenderTargetView(backbuffer, 0, &m_RenderTargetView));

		//don't forget to release the backbuffer 2D Texture
		ReleaseCOM(backbuffer);

		//Create Depth/Stencil view

		D3D11_TEXTURE2D_DESC DepthStencilDesc;

		DepthStencilDesc.Width = m_ScreenWidth;
		DepthStencilDesc.Height = m_ScreenHeight;
		DepthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DepthStencilDesc.MipLevels = 1;
		DepthStencilDesc.ArraySize = 1;

		if (m_EnableMSAA)
		{
			DepthStencilDesc.SampleDesc.Count = m_MultiSampleCount;
			DepthStencilDesc.SampleDesc.Quality = m_MultiSampleQuality - 1;
		}
		else
		{
			DepthStencilDesc.SampleDesc.Count = 1;
			DepthStencilDesc.SampleDesc.Quality = 0;
		}

		DepthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
		DepthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		DepthStencilDesc.CPUAccessFlags = 0;
		DepthStencilDesc.MiscFlags = 0;

		HR(m_Device->CreateTexture2D(&DepthStencilDesc, 0, &m_DepthStencilBuffer));
		HR(m_Device->CreateDepthStencilView(m_DepthStencilBuffer, 0, &m_DepthStencilView));

		//Set RenderTargetView
		m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, m_DepthStencilView);

		//Set ViewPort
		m_Viewport.Width = static_cast<float>(m_ScreenWidth);
		m_Viewport.Height = static_cast<float>(m_ScreenHeight);
		m_Viewport.MaxDepth = 1.0f;
		m_Viewport.MinDepth = 0.0f;
		m_Viewport.TopLeftX = 0.0f;
		m_Viewport.TopLeftY = 0.0f;

		m_DeviceContext->RSSetViewports(1, &m_Viewport);
	}

	void Renderer::AddModel(Model* model, CXMMATRIX World)
	{
		m_Models.push_back(model);

		XMFLOAT4X4 Worldf;
		XMStoreFloat4x4(&Worldf, World);

		m_WorldMatrix.push_back(Worldf);
	}

	void Renderer::Draw()
	{
		m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, reinterpret_cast<const float*>(&Colors::Black));
		m_DeviceContext->ClearDepthStencilView(m_DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

		m_DeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		
		m_DeviceContext->IASetInputLayout(InputLayouts::Basic32);

		m_DeviceContext->VSSetShader(Shaders::Basic->VertexShader, nullptr, 0);
		m_DeviceContext->PSSetShader(Shaders::Basic->PixelShader, nullptr, 0);

		static Lights::DirectionalLight light;

		light.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
		light.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
		light.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
		light.Direction = XMFLOAT3(0.0f, 0.0f, 1.0f);


		for (UINT i = 0; i < m_Models.size(); ++i)
		{
			Shaders::Basic->SetTransform(XMLoadFloat4x4(&m_WorldMatrix[i]), TriangleDemo::Cam->GetViewXM(), TriangleDemo::Cam->GetProjXM(), XMMatrixIdentity());
			Shaders::Basic->SetSampler(SamplerStates::SamLinear, 0);
			Shaders::Basic->SetPerFrameData(TriangleDemo::Cam->GetPositionXM(), light);

			m_Models[i]->Render(m_DeviceContext, Shaders::Basic);
		}

		HR(m_SwapChain->Present(0, 0));
	}

#if defined(DEBUG) | defined(_DEBUG) 
	VOID Renderer::CalculateFrameStats(const std::string& WindowName, const GameTimer& timer, const HWND& Hwnd)
	{
		static int FrameCnt = 0;
		static float TimeElapsed = 0.0f;

		FrameCnt++;

		if ((timer.TotalTime() - TimeElapsed) >= 1.0f)
		{
			float FPS = (float)FrameCnt; //Frames per second = FrameCount / 1
			float spf = (1.0f / FPS); //Seconds per frame = 1 / FrameCount (Reciprocal)
			float mspf = spf * 1000.0f; //convert to milliseconds per frame

			std::wostringstream outs;
			outs.precision(8); //float precision

			outs << WindowName.c_str() << "    "
				<< "FPS: " << FPS << "    "
				<< "Frame Time: " << mspf << "  (ms)";

			SetWindowTextW(Hwnd, outs.str().c_str());

			FrameCnt = 0;
			TimeElapsed += 1.0f;
		}
	}
#endif

	VOID Renderer::GetResolution(UINT& Width, UINT& Height)
	{
		Width = m_ScreenWidth; 
		Height = m_ScreenHeight;
	}

	ID3D11Device* Renderer::GetDevice()
	{
#if defined(DEBUG) | defined(_DEBUG)
		assert(m_Device);
#endif

		return m_Device;
	}

	ID3D11Device* Renderer::_GetDevice()
	{
#if defined(DEBUG) | defined(_DEBUG)
		assert(m_Device);
#endif

		return m_Device;
	}

	ID3D11DeviceContext* Renderer::_GetDeviceContext()
	{
#if defined(DEBUG) | defined(_DEBUG)
		assert(m_DeviceContext);
#endif

		return m_DeviceContext;
	}

	ID3D11DeviceContext* Renderer::GetDeviceContext()
	{
#if defined(DEBUG) | defined(_DEBUG)
		assert(m_DeviceContext);
#endif

		return m_DeviceContext;
	}

	void Renderer::Destroy()
	{
		m_SwapChain->SetFullscreenState(FALSE, NULL);

		if (m_DeviceContext)
			m_DeviceContext->ClearState();

		ReleaseCOM(m_Device);
		ReleaseCOM(m_DeviceContext);
		ReleaseCOM(m_SwapChain);

		ReleaseCOM(m_DepthStencilView);
		ReleaseCOM(m_DepthStencilBuffer);

		ReleaseCOM(m_RenderTargetView);

		InputLayouts::Destroy();
		Shaders::Destroy();
		TriangleDemo::Destroy();
		SamplerStates::Destroy();
		TextureManager::Instance()->Destroy();
	}
}