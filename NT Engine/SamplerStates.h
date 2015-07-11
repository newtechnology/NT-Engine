#ifndef _SAMPLER_STATES_H_
#define _SAMPLER_STATES_H_

#include "BasicIncludes.h"
#include <d3d11.h>

namespace NTEngine
{
	class SamplerStates
	{
	public:
		static void Initialize(ID3D11Device* dev);
		static void Destroy();


		static ID3D11SamplerState* SamLinear;
	};
}

#endif