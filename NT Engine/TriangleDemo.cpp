#include "TriangleDemo.h"



NTEngine::VertexBuffer* TriangleDemo::VB = nullptr;
NTEngine::IndexBuffer* TriangleDemo::IB = nullptr;



void TriangleDemo::Initialize(ID3D11Device* dev)
{
	VB = new NTEngine::VertexBuffer();
	IB = new NTEngine::IndexBuffer();

	// create a triangle using the VERTEX struct
	NTEngine::Vertex::Color OurVertices[] =
	{
		{ XMFLOAT3(0.0f, 0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(0.45f, -0.5, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-0.45f, -0.5f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }
	};

	UINT Indices[] =
	{
		0, 1, 2
	};

	VB->SetVertices(dev, OurVertices, 3);
	IB->SetIndices(dev, Indices, 3);


}

void TriangleDemo::Destroy()
{

	SafeDelete(VB);
	SafeDelete(IB);

}