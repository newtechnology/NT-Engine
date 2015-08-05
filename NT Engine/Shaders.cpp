#include "Shaders.h"
#include "Math.h"
#include "SamplerStates.h"
#include <fstream>


using namespace DirectX;


namespace NTEngine
{

#pragma region BaseShader
	Shader::Shader(ID3D11Device* device, ID3D11DeviceContext* devcon, const std::string& VertexShaderPath, const std::string& PixelShaderPath) :
		VertexShader(nullptr),
		PixelShader(nullptr),
		m_DevContext(devcon)
	{
		/***** Load Compiled Vertex Shader ****/

		int compiledVertexShaderSize = 0;

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

		int compiledPixelShaderSize = 0;
		std::vector<char> compiledPixelShader;

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



#pragma region BasicShader
	BasicShader::BasicShader(ID3D11Device* device, ID3D11DeviceContext* devcon, const std::string& VertexShaderPath, const std::string& PixelShaderPath)
		: Shader(device, devcon, VertexShaderPath, PixelShaderPath)
	{

		m_BufferPerFrame_PS = new ConstantBuffer();
		m_BufferPerObject_PS = new ConstantBuffer();
		m_BufferPerObject_VS = new ConstantBuffer();

		//pixel shader constant buffers
		m_BufferPerFrame_PS->SetConstantBuffer(device, sizeof(cbPerFrame_PS), 0);
		m_BufferPerObject_PS->SetConstantBuffer(device, sizeof(cbPerObject_PS), 1);

		//vertex shader constant buffer
	    m_BufferPerObject_VS->SetConstantBuffer(device, sizeof(cbPerObject_VS), 0);

	}

	BasicShader::~BasicShader()
	{
		SafeDelete(m_BufferPerFrame_PS);
		SafeDelete(m_BufferPerObject_PS);
		SafeDelete(m_BufferPerObject_VS);
	}

	void BasicShader::SetMaterial(const Lights::Material& objMaterial)
	{
		//Update CBPerObject_PS
		CBPerObject_PS.ObjectMaterial = objMaterial;

		m_BufferPerObject_PS->UpdateBuffer(m_DevContext, CBPerObject_PS);

		ID3D11Buffer* CB = m_BufferPerObject_PS->GetConstantBuffer();

		m_DevContext->PSSetConstantBuffers(m_BufferPerObject_PS->GetBufferSlot(), 1, &CB);

	} 

	void BasicShader::SetDiffuseMap(ID3D11ShaderResourceView* SRV)
	{
		//Set texture
		m_DevContext->PSSetShaderResources(0, 1, &SRV);
	}

	void BasicShader::SetTransform(CXMMATRIX World, CXMMATRIX View, CXMMATRIX Proj, CXMMATRIX TexTransform)
	{
		//Update CBPerObject_VS
		XMFLOAT4X4 Worldf;
		XMStoreFloat4x4(&Worldf, XMMatrixTranspose(World));

		XMFLOAT4X4 WorldViewProjf;
		XMStoreFloat4x4(&WorldViewProjf, XMMatrixTranspose(World * View * Proj));

		XMFLOAT4X4 WorldInvTransposef;
		XMStoreFloat4x4(&WorldInvTransposef, XMMatrixTranspose(Math::InverseTranspose(World))); //we can avoid transpose here

		XMFLOAT4X4 TexTransformf;
		XMStoreFloat4x4(&TexTransformf, XMMatrixTranspose(TexTransform));

		CBPerObject_VS.World = Worldf;
		CBPerObject_VS.WorldViewProj = WorldViewProjf;
		CBPerObject_VS.WorldInvTranspose = WorldInvTransposef;
		CBPerObject_VS.TexTransform = TexTransformf;

		m_BufferPerObject_VS->UpdateBuffer(m_DevContext, CBPerObject_VS);

		ID3D11Buffer* CB = m_BufferPerObject_VS->GetConstantBuffer();

		m_DevContext->VSSetConstantBuffers(m_BufferPerObject_VS->GetBufferSlot(), 1, &CB);

	}

	void BasicShader::SetSampler(ID3D11SamplerState* SamplerState, UINT SamplerStateSlot)
	{
		//Set Sampler
		m_DevContext->PSSetSamplers(SamplerStateSlot, 1, &SamplerState);
	}

	void BasicShader::SetPerFrameData(const Lights::DirectionalLight lights[MAX_DIR_LIGHTS], int NumUsedLights, FXMVECTOR CameraPos)
	{

		XMFLOAT3 CameraPosf;
		XMStoreFloat3(&CameraPosf, CameraPos);

		//Update CBPerFrame_PS
		for (UINT i = 0; i < MAX_DIR_LIGHTS; ++i)
		{
			CBPerFrame_PS.DirLights[i] = lights[i];
		}

		CBPerFrame_PS.NumDirLights = NumUsedLights;
		CBPerFrame_PS.CameraPos = CameraPosf;
		CBPerFrame_PS.NumDirLights = NumUsedLights;
		
		m_BufferPerFrame_PS->UpdateBuffer(m_DevContext, CBPerFrame_PS);

		ID3D11Buffer* CB = m_BufferPerFrame_PS->GetConstantBuffer();

		m_DevContext->PSSetConstantBuffers(m_BufferPerFrame_PS->GetBufferSlot(), 1, &CB);


	}
#pragma endregion


#pragma region NormalMapShader

	NormalMapShader::NormalMapShader(ID3D11Device* device, ID3D11DeviceContext* devcon, const std::string& VertexShaderPath, const std::string& PixelShaderPath)
		: Shader(device, devcon, VertexShaderPath, PixelShaderPath)
	{
		//VS Buffers
		m_BufferPerObject_VS = new ConstantBuffer();


		m_BufferPerObject_VS->SetConstantBuffer(device, sizeof(cbPerObject_VS), 0);

		//PS Buffers
		m_BufferPerFrame_PS = new ConstantBuffer();
		m_BufferPerObject_PS = new ConstantBuffer();

		m_BufferPerFrame_PS->SetConstantBuffer(device, sizeof(cbPerFrame_PS), 0);
		m_BufferPerObject_PS->SetConstantBuffer(device, sizeof(cbPerObject_PS), 1);
	}

	void NormalMapShader::SetDiffuseAndNormalMap(ID3D11ShaderResourceView* diffusemap, ID3D11ShaderResourceView* normalmap)
	{
		ID3D11ShaderResourceView* views[] = { diffusemap, normalmap };
		m_DevContext->PSSetShaderResources(0, 2, views);

	}

	void NormalMapShader::SetSampler(ID3D11SamplerState* sampler, UINT SamplerStateSlot)
	{
		m_DevContext->PSSetSamplers(SamplerStateSlot, 1, &sampler);
	}

	void NormalMapShader::SetPerObjectData(CXMMATRIX World, CXMMATRIX View, CXMMATRIX Proj, CXMMATRIX TexTransform)
	{
		XMFLOAT4X4 Worldf;
		XMStoreFloat4x4(&Worldf, XMMatrixTranspose(World));

		XMFLOAT4X4 WorldViewProjf;
		XMStoreFloat4x4(&WorldViewProjf, XMMatrixTranspose(World * View * Proj));

		XMFLOAT4X4 WorldInvTransposef;
		XMStoreFloat4x4(&WorldInvTransposef, XMMatrixTranspose(Math::InverseTranspose(World)));

		XMFLOAT4X4 TexTransformf;
		XMStoreFloat4x4(&TexTransformf, XMMatrixTranspose(TexTransform));

		CBPerObject_VS.World = Worldf;
		CBPerObject_VS.WorldViewProj = WorldViewProjf;
		CBPerObject_VS.WorldInvTranspose = WorldInvTransposef;
		CBPerObject_VS.TexTransform = TexTransformf;

		m_BufferPerObject_VS->UpdateBuffer(m_DevContext, CBPerObject_VS);

		ID3D11Buffer* CB = m_BufferPerObject_VS->GetConstantBuffer();

		m_DevContext->VSSetConstantBuffers(m_BufferPerObject_VS->GetBufferSlot(), 1, &CB);
	}

	void NormalMapShader::SetMaterial(const Lights::Material& mat)
	{
		//Fill CBPerObject_PS
		CBPerObject_PS.ObjectMaterial = mat;

		m_BufferPerObject_PS->UpdateBuffer(m_DevContext, CBPerObject_PS);

		ID3D11Buffer* CB = m_BufferPerObject_PS->GetConstantBuffer();

		m_DevContext->PSSetConstantBuffers(m_BufferPerObject_PS->GetBufferSlot(), 1, &CB);
	}

	void NormalMapShader::SetPerFrameData(const Lights::DirectionalLight lights[MAX_DIR_LIGHTS], int NumberOfUsedLights, FXMVECTOR CameraPos)
	{
		//Fill CBPerFrame_PS buffer
		XMFLOAT3 CameraPosf;
		XMStoreFloat3(&CameraPosf, CameraPos);

		CBPerFrame_PS.CameraPos = CameraPosf;
		CBPerFrame_PS.NumDirLights = NumberOfUsedLights;
		
		for (UINT i = 0; i < MAX_DIR_LIGHTS; ++i)
		{
			CBPerFrame_PS.DirLights[i] = lights[i];
		}

		m_BufferPerFrame_PS->UpdateBuffer(m_DevContext, CBPerFrame_PS);

		ID3D11Buffer* CB = m_BufferPerFrame_PS->GetConstantBuffer();

		m_DevContext->PSSetConstantBuffers(m_BufferPerFrame_PS->GetBufferSlot(), 1, &CB);
	}

	NormalMapShader::~NormalMapShader()
	{
		SafeDelete(m_BufferPerObject_VS);
		SafeDelete(m_BufferPerFrame_PS);
		SafeDelete(m_BufferPerObject_PS);
	}
#pragma endregion

#pragma region Shaders
	BasicShader* Shaders::Basic = nullptr;
	NormalMapShader* Shaders::NormalMap = nullptr; 


	void Shaders::Initialize(ID3D11Device* device, ID3D11DeviceContext* devcon)
	{
		Basic = new BasicShader(device, devcon, "Shaders//basic_vs.cso", "Shaders//basic_ps.cso");
		NormalMap = new NormalMapShader(device, devcon, "Shaders//NormalMap_VS.cso", "Shaders//NormalMap_PS.cso");
	}


	void Shaders::Destroy()
	{
		SafeDelete(Basic);
		SafeDelete(NormalMap);
	}
#pragma endregion

}