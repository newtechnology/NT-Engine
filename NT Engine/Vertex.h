#ifndef _VERTEX_H_
#define _VERTEX_H_


#include "BasicIncludes.h"
#include <d3d11.h>

namespace NTEngine
{
	namespace Vertex
	{

		struct Basic32
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT3 Normal;
			DirectX::XMFLOAT2 Tex;
		};

		struct NormalMap
		{
			DirectX::XMFLOAT3 Pos;
			DirectX::XMFLOAT3 Normal;
			DirectX::XMFLOAT2 Tex;
			DirectX::XMFLOAT3 Tangent;
		};
	}


	class InputLayoutDesc
	{
	public:
		static void Initialize();

		static D3D11_INPUT_ELEMENT_DESC Basic32[3];
		static D3D11_INPUT_ELEMENT_DESC NormalMap[4];
	};


	class InputLayouts
	{
	public:
		static void Initialize(ID3D11Device* device);
		static void Destroy();

		static ID3D11InputLayout* Basic32;
		static ID3D11InputLayout* NormalMap;
	};
}

#endif