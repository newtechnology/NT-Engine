#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "BasicIncludes.h"
#include "Timer.h"
#include "Model.h"
#include "Shaders.h"
#include <d3d11.h>
#include <D3DX11.h>
#include <assert.h>

namespace NTEngine
{


	class Renderer
	{
	public:
		DLLEXPORT Renderer();
		DLLEXPORT ~Renderer();

		DLLEXPORT VOID Initialize(HWND HWnd, UINT Width, UINT Height);
		
		DLLEXPORT VOID AddModel(Model* model, CXMMATRIX World);
		DLLEXPORT VOID Destroy();

		DLLEXPORT VOID OnResize();
		DLLEXPORT VOID Draw();

#if defined(DEBUG) | defined(_DEBUG)
		DLLEXPORT VOID CalculateFrameStats(const std::string& WindowName, const GameTimer& timer, const HWND& Hwnd);
#endif

		DLLEXPORT inline VOID GetResolution(UINT& Width, UINT& Height);

		//Exported version: For calling outside DLL
		DLLEXPORT inline ID3D11Device* _GetDevice(); 
		//Inside version: For calling inside DLL
		inline ID3D11Device* GetDevice(); 

		//Exported version
		DLLEXPORT inline ID3D11DeviceContext* _GetDeviceContext();
		//Inside version
		inline ID3D11DeviceContext* GetDeviceContext();

	private:
		ID3D11Device* m_Device;
		ID3D11DeviceContext* m_DeviceContext;
		IDXGISwapChain* m_SwapChain;
		ID3D11RenderTargetView* m_RenderTargetView;
		ID3D11DepthStencilView* m_DepthStencilView;
		ID3D11Texture2D* m_DepthStencilBuffer;

		D3D11_VIEWPORT m_Viewport;

		bool m_EnableMSAA;

		UINT m_MultiSampleCount;
		UINT m_MultiSampleQuality;

		UINT m_ScreenWidth;
		UINT m_ScreenHeight;

		std::vector<Model*> m_Models;
		std::vector<XMFLOAT4X4> m_WorldMatrix;

	};
}

#endif