#include "Shaders.h"
#include "Math.h"
#include "SamplerStates.h"
#include <fstream>

namespace NTEngine
{

#pragma region BaseShader
	Shader::Shader(ID3D11Device* device, const std::string& VertexShaderPath, const std::string& PixelShaderPath) :
		VertexShader(nullptr),
		PixelShader(nullptr),
		compiledVertexShaderSize(0)
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
		: Shader(device, VertexShaderPath, PixelShaderPath)
	{
		m_DevContext = devcon;
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

	void BasicShader::SetPerObjectData(const Lights::Material& objMaterial)
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

	void BasicShader::SetPerFrameData(FXMVECTOR CameraPos,  const Lights::DirectionalLight& light)
	{

		XMFLOAT3 CameraPosf;
		XMStoreFloat3(&CameraPosf, CameraPos);

		//Update CBPerFrame_PS
		CBPerFrame_PS.DirLight = light;
		CBPerFrame_PS.CameraPos = CameraPosf;
		CBPerFrame_PS.Pad = 0.0f;
		
		m_BufferPerFrame_PS->UpdateBuffer(m_DevContext, CBPerFrame_PS);

		ID3D11Buffer* CB = m_BufferPerFrame_PS->GetConstantBuffer();

		m_DevContext->PSSetConstantBuffers(m_BufferPerFrame_PS->GetBufferSlot(), 1, &CB);


	}
#pragma endregion


#pragma region Shaders
	BasicShader* Shaders::Basic = nullptr;

	void Shaders::Initialize(ID3D11Device* device, ID3D11DeviceContext* devcon)
	{
		Basic = new BasicShader(device, devcon, "Shaders//basic_vs.cso", "Shaders//basic_ps.cso");
	}


	void Shaders::Destroy()
	{
		SafeDelete(Basic);
	}
#pragma endregion

}