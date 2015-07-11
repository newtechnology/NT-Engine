#include "TextureManager.h"
#include <D3DX11.h>
#include <assert.h>

namespace NTEngine
{
	TextureManager* TextureManager::_instance = nullptr;


	TextureManager* TextureManager::Instance()
	{
		if (_instance == nullptr)
			_instance = new TextureManager();
		

		return _instance;
	}

	void TextureManager::Initialize(ID3D11Device* dev)
	{
		m_Device = dev;
	}

	ID3D11ShaderResourceView* TextureManager::Create(std::string filePath)
	{
#if defined(DEBUG) | defined(_DEBUG)
		if (m_Device == nullptr)
			ShowError("TextureManager is not initialized or Device is null.");
#endif
		ID3D11ShaderResourceView* SRV = nullptr;

		if (m_Textures.find(filePath) != m_Textures.end())
		{
			SRV = m_Textures[filePath];
		}
		else
		{
			HR(D3DX11CreateShaderResourceViewFromFile(m_Device, filePath.c_str(), 0, 0, &SRV, 0));

			m_Textures[filePath] = SRV;
		}

		return SRV;
	}

	void TextureManager::Destroy()
	{
		for (auto it = m_Textures.begin(); it != m_Textures.end(); ++it)
		{
			ReleaseCOM(it->second);
		}

		m_Textures.clear();

		if (_instance != nullptr)
		{
			delete _instance;
			_instance = nullptr;
		}
	}
}