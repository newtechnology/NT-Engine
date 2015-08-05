#ifndef _RASTERIZER_STATES_H_
#define _RASTERIZER_STATES_H_

#include "BasicIncludes.h"
#include <d3d11.h>

namespace NTEngine
{
	class RasterizerStates
	{
	public:
		static void Initialize(ID3D11Device* dev);
		static void Destroy();

		static ID3D11RasterizerState* ShadowMapDepth;
	};
}


#endif