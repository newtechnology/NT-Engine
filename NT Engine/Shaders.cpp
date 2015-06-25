#include "Shaders.h"
#include <fstream>

namespace NTEngine
{

#pragma region Shader
	Shader::Shader(ID3D11Device* device, const std::string& VertexShaderPath, const std::string& PixelShaderPath) :
		VertexShader(nullptr),
		PixelShader(nullptr),
		compiledVertexShaderSize(0),
		compiledPixelShaderSize(0)
	{
		/***** Load Compiled Vertex Shader ****/

		std::ifstream VSFile(VertexShaderPath.c_str(), std::ios::binary);

		VSFile.seekg(0, std::ios_base::end);

		compiledVertexShaderSize = (int)VSFile.tellg();

		VSFile.seekg(0, std::ios_base::beg);
		
		compiledVertexShader.resize(compiledVertexShaderSize);

		VSFile.read(&compiledVertexShader[0], compiledVertexShaderSize);
		VSFile.close();

		HRESULT Hr = device->CreateVertexShader(&compiledVertexShader[0], compiledVertexShaderSize, nullptr, &VertexShader);

		if (FAILED(Hr))
		{
			char v[256];
			sprintf_s(v, "Could not load %s\n", VertexShaderPath);

			ShowError(v);
		}

		/**** Load Compiled Pixel Shader ****/

		std::ifstream PSFile(PixelShaderPath.c_str(), std::ios::binary);
		PSFile.seekg(0, std::ios_base::end);

		compiledPixelShaderSize = (int)PSFile.tellg();

		PSFile.seekg(0, std::ios_base::beg);

		compiledPixelShader.resize(compiledPixelShaderSize);

		PSFile.read(&compiledPixelShader[0], compiledPixelShaderSize);
		PSFile.close();

		Hr = device->CreatePixelShader(&compiledPixelShader[0], compiledPixelShaderSize, nullptr, &PixelShader);

		if (FAILED(Hr))
		{
			char p[256];
			sprintf_s(p, "Could not load %s\n", PixelShaderPath);

			ShowError(p);
		}

	}


	Shader::~Shader()
	{
		ReleaseCOM(VertexShader);
		ReleaseCOM(PixelShader);
	}

#pragma endregion

	Shader* Shaders::Color = nullptr;

	void Shaders::Initialize(ID3D11Device* device)
	{
		Color = new Shader(device, "Shaders//color_vs.cso", "Shaders//color_ps.cso");
	}


	void Shaders::Destroy()
	{
		SafeDelete(Color);
	}

}