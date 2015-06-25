#include "Vertex.h"
#include "Shaders.h"


namespace NTEngine
{

	D3D11_INPUT_ELEMENT_DESC InputLayoutDesc::PosColor[2] = { 0 };

	void InputLayoutDesc::Initialize()
	{
		//============== Color Input Layout =====================//

		PosColor[0].SemanticName = "POSITION";
		PosColor[0].SemanticIndex = 0;
		PosColor[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		PosColor[0].InputSlot = 0;
		PosColor[0].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		PosColor[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		PosColor[0].InstanceDataStepRate = 0;

		PosColor[1].SemanticName = "COLOR";
		PosColor[1].SemanticIndex = 0;
		PosColor[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		PosColor[1].InputSlot = 0;
		PosColor[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		PosColor[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		PosColor[1].InstanceDataStepRate = 0;

		//========================================================//

	}



	ID3D11InputLayout* InputLayouts::PosColor = nullptr;

	void InputLayouts::Initialize(ID3D11Device* device)
	{
		device->CreateInputLayout(InputLayoutDesc::PosColor, 2, &Shaders::Color->compiledVertexShader[0], Shaders::Color->compiledVertexShaderSize, &PosColor);

	}


	void InputLayouts::Destroy()
	{
		ReleaseCOM(PosColor);
	}
}