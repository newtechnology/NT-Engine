#ifndef _SHADER_H_
#define _SHADER_H_

#include "BasicIncludes.h" 
#include "ConstantBuffer.h"
#include "Lights.h"
#include <d3d11.h>
#include <vector>

#define MAX_DIR_LIGHTS 8

namespace NTEngine
{

#pragma region BaseShader
	
	class Shader
	{
	public:
		Shader(ID3D11Device* device, ID3D11DeviceContext* devcon, const std::string& VertexShaderPath, const std::string& PixelShaderPath);
		virtual ~Shader();

		std::vector<char> compiledVertexShader;

		ID3D11VertexShader* VertexShader;
		ID3D11PixelShader* PixelShader;


	protected:
		ID3D11DeviceContext* m_DevContext;
	};

#pragma endregion


#pragma region BasicShader
	class BasicShader : public Shader
	{
	public:
		BasicShader(ID3D11Device* device, ID3D11DeviceContext* devcon, const std::string& VertexShaderPath, const std::string& PixelShaderPath);
		~BasicShader();
	

		DLLEXPORT VOID inline SetPerFrameData(const Lights::DirectionalLight light[MAX_DIR_LIGHTS], int NumUsedLights, DirectX::FXMVECTOR CameraPos);
		DLLEXPORT VOID inline SetMaterial(const Lights::Material& objMaterial);

		DLLEXPORT VOID inline SetTransform(DirectX::CXMMATRIX World, DirectX::CXMMATRIX View, DirectX::CXMMATRIX Proj, DirectX::CXMMATRIX TexTransform);
		DLLEXPORT VOID inline SetDiffuseMap(ID3D11ShaderResourceView* srv);
		DLLEXPORT VOID inline SetSampler(ID3D11SamplerState* SamplerState, UINT SamplerStateSlot);

		struct cbPerObject_VS
		{
			DirectX::XMFLOAT4X4 World;
			DirectX::XMFLOAT4X4 WorldInvTranspose;
			DirectX::XMFLOAT4X4 WorldViewProj;
			DirectX::XMFLOAT4X4 TexTransform;
		}CBPerObject_VS;

		struct cbPerObject_PS
		{
			Lights::Material ObjectMaterial;

		}CBPerObject_PS; 

		struct cbPerFrame_PS
		{
			Lights::DirectionalLight DirLights[MAX_DIR_LIGHTS];
			
			DirectX::XMFLOAT3 CameraPos;
			INT NumDirLights;

		}CBPerFrame_PS;

		ConstantBuffer* m_BufferPerObject_VS;
		ConstantBuffer* m_BufferPerObject_PS;
		ConstantBuffer* m_BufferPerFrame_PS;

	};

#pragma endregion



#pragma region NormalMap

	class NormalMapShader : public Shader
	{
	public:
		NormalMapShader(ID3D11Device* device, ID3D11DeviceContext* devcon, const std::string& VertexShaderPath, const std::string& PixelShaderPath);
		~NormalMapShader();


		//Vertex shader buffers
		struct cbPerObject_VS
		{
			DirectX::XMFLOAT4X4 World;
			DirectX::XMFLOAT4X4 WorldViewProj;
			DirectX::XMFLOAT4X4 WorldInvTranspose;
			DirectX::XMFLOAT4X4 TexTransform;
		}CBPerObject_VS;


		//Pixel shader buffers
		struct cbPerFrame_PS
		{
			Lights::DirectionalLight DirLights[MAX_DIR_LIGHTS];

			//pack into 4D vector
			DirectX::XMFLOAT3 CameraPos;
			int NumDirLights;

		}CBPerFrame_PS;

		struct cbPerObject_PS
		{
			Lights::Material ObjectMaterial;

		}CBPerObject_PS;

		DLLEXPORT VOID inline SetDiffuseAndNormalMap(ID3D11ShaderResourceView* DiffuseMap, ID3D11ShaderResourceView* NormalMap);
		DLLEXPORT VOID inline SetPerObjectData(DirectX::CXMMATRIX World, DirectX::CXMMATRIX View, DirectX::CXMMATRIX Proj, DirectX::CXMMATRIX TexTransform);
		DLLEXPORT VOID inline SetPerFrameData(const Lights::DirectionalLight lights[MAX_DIR_LIGHTS], int NumberOfUsedLights, DirectX::FXMVECTOR CameraPos);
		DLLEXPORT VOID inline SetMaterial(const Lights::Material& mat);

		DLLEXPORT VOID inline SetSampler(ID3D11SamplerState* SamplerState, UINT SamplerStateSlot);

		//VS Buffers
		ConstantBuffer* m_BufferPerObject_VS;

		//PS Buffers
		ConstantBuffer* m_BufferPerFrame_PS;
		ConstantBuffer* m_BufferPerObject_PS;

	};

#pragma endregion


	class Shaders
	{
	public:
		static void Initialize(ID3D11Device* device, ID3D11DeviceContext* devcon);
		static void Destroy();


		static BasicShader* Basic; 
		static NormalMapShader* NormalMap;

	};

}

#endif