#ifndef _SHADER_H_
#define _SHADER_H_

#include "BasicIncludes.h" 
#include <d3d11.h>
#include <vector>

namespace NTEngine
{

#pragma region Shader
	class Shader
	{
	public:
		Shader(ID3D11Device* device, const std::string& VertexShaderPath, const std::string& PixelShaderPath);
		~Shader();


		std::vector<char> compiledVertexShader;
		int compiledVertexShaderSize;


		std::vector<char> compiledPixelShader;
		int compiledPixelShaderSize;

		ID3D11VertexShader* VertexShader;
		ID3D11PixelShader* PixelShader;
	};

#pragma endregion


	class Shaders
	{
	public:
		static void Initialize(ID3D11Device* device);
		static void Destroy();


		static Shader* Color;
	};

}

#endif