#ifndef _SHADER_H_
#define _SHADER_H_

#include "BasicIncludes.h" 
#include "ConstantBuffer.h"
#include "Lights.h"
#include <d3d11.h>
#include <vector>

namespace NTEngine
{

#pragma region BaseShader
	
	class Shader
	{
	public:
		Shader(ID3D11Device* device, const std::string& VertexShaderPath, const std::string& PixelShaderPath);
		virtual ~Shader();

		std::vector<char> compiledVertexShader;
		int compiledVertexShaderSize;

		ID3D11VertexShader* VertexShader;
		ID3D11PixelShader* PixelShader;

	};

#pragma endregion

	class BasicShader : public Shader
	{
	public:
		BasicShader(ID3D11Device* device, ID3D11DeviceContext* devcon, const std::string& VertexShaderPath, const std::string& PixelShaderPath);
		~BasicShader();
	

		DLLEXPORT VOID inline SetPerFrameData(FXMVECTOR CameraPos, const Lights::DirectionalLight& light);
		DLLEXPORT VOID inline SetPerObjectData(const Lights::Material& objMaterial);

		DLLEXPORT VOID inline SetTransform(CXMMATRIX World, CXMMATRIX View, CXMMATRIX Proj, CXMMATRIX TexTransform);
		DLLEXPORT VOID inline SetDiffuseMap(ID3D11ShaderResourceView* srv);
		DLLEXPORT VOID inline SetSampler(ID3D11SamplerState* SamplerState, UINT SamplerStateSlot);

		struct cbPerObject_VS
		{
			XMFLOAT4X4 World;
			XMFLOAT4X4 WorldInvTranspose;
			XMFLOAT4X4 WorldViewProj;
			XMFLOAT4X4 TexTransform;
		}CBPerObject_VS;

		struct cbPerObject_PS
		{
			Lights::Material ObjectMaterial;

		}CBPerObject_PS; 

		struct cbPerFrame_PS
		{
			Lights::DirectionalLight DirLight;
			
			XMFLOAT3 CameraPos;
			FLOAT Pad;

		}CBPerFrame_PS;

		ConstantBuffer* m_BufferPerObject_VS;
		ConstantBuffer* m_BufferPerObject_PS;
		ConstantBuffer* m_BufferPerFrame_PS;

	private:
		ID3D11DeviceContext* m_DevContext;
	};

	class Shaders
	{
	public:
		static void Initialize(ID3D11Device* device, ID3D11DeviceContext* devcon);
		static void Destroy();


		static BasicShader* Basic;
	};

}

#endif