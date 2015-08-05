#include "Vertex.h"
#include "Shaders.h"


namespace NTEngine
{

	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] = { 0 };
	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::NormalMap[4] = { 0 };

	void InputLayoutDesc::Initialize()
	{


		//================= Basic32 Input Layout =======================//

		Basic32[0].SemanticName = "POSITION";
		Basic32[0].SemanticIndex = 0;
		Basic32[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		Basic32[0].InputSlot = 0;
		Basic32[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		Basic32[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		Basic32[0].InstanceDataStepRate = 0;

		Basic32[1].SemanticName = "NORMAL";
		Basic32[1].SemanticIndex = 0;
		Basic32[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		Basic32[1].InputSlot = 0;
		Basic32[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		Basic32[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		Basic32[1].InstanceDataStepRate = 0;

		Basic32[2].SemanticName = "TEXCOORD";
		Basic32[2].SemanticIndex = 0;
		Basic32[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		Basic32[2].InputSlot = 0;
		Basic32[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		Basic32[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		Basic32[2].InstanceDataStepRate = 0;

		//================== NormalMap Input Layout ====================//

		NormalMap[0].SemanticName = "POSITION";
		NormalMap[0].SemanticIndex = 0;
		NormalMap[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		NormalMap[0].InputSlot = 0;
		NormalMap[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		NormalMap[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		NormalMap[0].InstanceDataStepRate = 0;

		NormalMap[1].SemanticName = "NORMAL";
		NormalMap[1].SemanticIndex = 0;
		NormalMap[1].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		NormalMap[1].InputSlot = 0;
		NormalMap[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		NormalMap[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		NormalMap[1].InstanceDataStepRate = 0;

		NormalMap[2].SemanticName = "TEXCOORD";
		NormalMap[2].SemanticIndex = 0;
		NormalMap[2].Format = DXGI_FORMAT_R32G32_FLOAT;
		NormalMap[2].InputSlot = 0;
		NormalMap[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		NormalMap[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		NormalMap[2].InstanceDataStepRate = 0;

		NormalMap[3].SemanticName = "TANGENT";
		NormalMap[3].SemanticIndex = 0;
		NormalMap[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		NormalMap[3].InputSlot = 0;
		NormalMap[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		NormalMap[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		NormalMap[3].InstanceDataStepRate = 0;
		
		//==============================================================//
	}




	ID3D11InputLayout* InputLayouts::Basic32 = nullptr;
	ID3D11InputLayout* InputLayouts::NormalMap = nullptr;

	void InputLayouts::Initialize(ID3D11Device* device)
	{
		device->CreateInputLayout(InputLayoutDesc::Basic32, ARRAYSIZE(InputLayoutDesc::Basic32), &Shaders::Basic->compiledVertexShader[0], Shaders::Basic->compiledVertexShader.size(), &Basic32);
		device->CreateInputLayout(InputLayoutDesc::NormalMap, ARRAYSIZE(InputLayoutDesc::NormalMap), &Shaders::NormalMap->compiledVertexShader[0], Shaders::NormalMap->compiledVertexShader.size(), &NormalMap);
	}


	void InputLayouts::Destroy()
	{
		ReleaseCOM(Basic32);
		ReleaseCOM(NormalMap);
	}
}