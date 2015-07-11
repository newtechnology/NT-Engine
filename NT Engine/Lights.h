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

			XMFLOAT4 Ambient;
			XMFLOAT4 Diffuse;
			XMFLOAT4 Specular; //w = SpecPower
			XMFLOAT4 Reflect;

		};

		struct DirectionalLight
		{
			DirectionalLight()
			{
				ZeroMemory(this, sizeof(DirectionalLight));
			}

			XMFLOAT4 Ambient;
			XMFLOAT4 Diffuse;
			XMFLOAT4 Specular;
			XMFLOAT3 Direction;
			float pad;
		};
	}
}



#endif