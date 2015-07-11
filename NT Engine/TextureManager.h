#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "BasicIncludes.h"
#include <map>
#include <d3d11.h>

namespace NTEngine
{
	class TextureManager
	{
	public:
		static TextureManager* Instance();

		ID3D11ShaderResourceView* Create(std::string filePath);

		void Initialize(ID3D11Device* dev);
		void Destroy();

	private:
		TextureManager() {}
		TextureManager(TextureManager& const) {}
		~TextureManager() {}
		TextureManager operator=(TextureManager& const) {}

		ID3D11Device* m_Device;

		std::map<std::string, ID3D11ShaderResourceView*> m_Textures;

		static TextureManager* _instance;

	};
}







#endif