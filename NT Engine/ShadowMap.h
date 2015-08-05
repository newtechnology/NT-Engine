#ifndef _SHADOW_MAP_H_
#define _SHADOW_MAP_H_

#include "BasicIncludes.h"
#include <d3d11.h>
namespace NTEngine
{
	class ShadowMap
	{
	public:
		ShadowMap(ID3D11Device* dev, UINT Width, UINT Height);
		~ShadowMap();

		ID3D11ShaderResourceView* GetDepthMap();

		VOID PrepareForRendering(ID3D11DeviceContext* devcon);

	private:
		ShadowMap(const ShadowMap& rhs);
		ShadowMap& operator=(const ShadowMap& rhs);


	private:
		UINT m_Width;
		UINT m_Height;

		ID3D11ShaderResourceView* m_DepthMapSRV;
		ID3D11DepthStencilView* m_DepthMapDSV;

		D3D11_VIEWPORT m_Viewport;
	};
}


#endif