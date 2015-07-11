#include "Vertex.h"
#include "Shaders.h"


namespace NTEngine
{

	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::Basic32[3] = { 0 };

	void InputLayoutDesc::Initialize()
	{


		//================= Basic32 Input Layout ==================//

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

		//=======================================================//
	}




	ID3D11InputLayout* InputLayouts::Basic32 = nullptr;

	void InputLayouts::Initialize(ID3D11Device* device)
	{
		device->CreateInputLayout(InputLayoutDesc::Basic32, ARRAYSIZE(InputLayoutDesc::Basic32), &Shaders::Basic->compiledVertexShader[0], Shaders::Basic->compiledVertexShaderSize, &Basic32);
	}


	void InputLayouts::Destroy()
	{
		ReleaseCOM(Basic32);
	}
}