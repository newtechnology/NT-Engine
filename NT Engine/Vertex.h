#ifndef _VERTEX_H_
#define _VERTEX_H_


#include "BasicIncludes.h"
#include <d3d11.h>

namespace NTEngine
{
	namespace Vertex
	{
		struct Color
		{
			XMFLOAT3 Pos;
			XMFLOAT4 RGBAColor;
		};
	}


	class InputLayoutDesc
	{
	public:
		static void Initialize();

		static D3D11_INPUT_ELEMENT_DESC PosColor[2];
	};


	class InputLayouts
	{
	public:
		static void Initialize(ID3D11Device* device);
		static void Destroy();

		static ID3D11InputLayout* PosColor;

	};
}

#endif