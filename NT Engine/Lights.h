#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include "BasicIncludes.h"

namespace NTEngine
{
	namespace Lights	
	{
		struct Material
		{
			Material()
			{
				ZeroMemory(this, sizeof(Material));
			}

			DirectX::XMFLOAT4 Ambient;
			DirectX::XMFLOAT4 Diffuse;
			DirectX::XMFLOAT4 Specular; //w = SpecPower
			DirectX::XMFLOAT4 Reflect;

		};

		struct DirectionalLight
		{
			DirectionalLight()
			{
				ZeroMemory(this, sizeof(DirectionalLight));
			}

			DirectX::XMFLOAT4 Ambient;
			DirectX::XMFLOAT4 Diffuse;
			DirectX::XMFLOAT4 Specular;
			DirectX::XMFLOAT3 Direction;
			float pad;
		};
	}
}



#endif