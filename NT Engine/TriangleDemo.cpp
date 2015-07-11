#include "TriangleDemo.h"
#include "GeoGenerator.h"
#include <vector>
#include "Math.h"
#include "MouseInput.h"


NTEngine::VertexBuffer* TriangleDemo::VB = nullptr;
NTEngine::IndexBuffer* TriangleDemo::IB = nullptr;
NTEngine::Camera* TriangleDemo::Cam = nullptr;


void TriangleDemo::Update(float dt, NTEngine::Renderer& renderer)
{
	bool CameraUpdated = false;

	if (GetAsyncKeyState('W') & 0x8000)
	{
		Cam->Walk(50.0f * dt);
		CameraUpdated = true;
	}

	if (GetAsyncKeyState('S') & 0x8000)
	{
		Cam->Walk(-50.0f * dt);
		CameraUpdated = true;
	}

	if (GetAsyncKeyState('A') & 0x8000)
	{
		Cam->Strafe(-50.0f * dt);
		CameraUpdated = true;
	}

	if (GetAsyncKeyState('D') & 0x8000)
	{
		Cam->Strafe(50.0f * dt);
		CameraUpdated = true;
	}

	if (NTEngine::MouseInput::Instance()->MouseMoved())
	{
		POINT CurrentMousePos = NTEngine::MouseInput::Instance()->GetCurrentMousePos();
		POINT LastMousePos = NTEngine::MouseInput::Instance()->GetLastMousePos();


		float ScaleFactor = 0.35f;

		float dx = ScaleFactor * static_cast<float>(CurrentMousePos.x - LastMousePos.x);
		dx = XMConvertToRadians(dx); //convert to radians

		float dy = ScaleFactor * static_cast<float>(CurrentMousePos.y - LastMousePos.y);
		dy = XMConvertToRadians(dy); //convert to radians

		Cam->RotateY(dx);
		Cam->Pitch(dy);

		CameraUpdated = true;
	}

	//only update viewmatrix if required
	//save cpu cycles
	if (CameraUpdated)
		Cam->UpdateViewMatrix();

}

void TriangleDemo::Initialize(ID3D11Device* dev, NTEngine::Renderer& renderer)
{
	VB = new NTEngine::VertexBuffer();
	IB = new NTEngine::IndexBuffer();
	Cam = new NTEngine::Camera();

	Cam->SetPosition(0.0f, 0.0f, -5.0f);
	
	Cam->SetLens(0.25f * NTEngine::Math::PI, static_cast<float>(1024) / static_cast<float>(768), 1.0f, 1000.0f);

	std::vector<USHORT> Indices;
	std::vector<NTEngine::Vertex::Basic32> Vertices;

	NTEngine::GeometryGenerator geoGen;
	NTEngine::GeometryGenerator::MeshData data;

	geoGen.CreateBox(1.0f, 1.0f, 1.0f, data);

	Vertices.resize(data.Vertices.size());
	Indices.resize(data.Indices.size());

	for (UINT i = 0; i < data.Vertices.size(); ++i)
	{
		Vertices[i].Pos = data.Vertices[i].Position;
		Vertices[i].Normal = data.Vertices[i].Normal;
		Vertices[i].Tex = data.Vertices[i].TexC;
	}


	for (UINT i = 0; i < data.Indices.size(); ++i)
	{
		Indices[i] = static_cast<USHORT>(data.Indices[i]);
	}

	VB->SetVertices(dev, &Vertices[0], Vertices.size());
	IB->SetIndices(dev, &Indices[0], Indices.size(), DXGI_FORMAT_R16_UINT);

}

void TriangleDemo::Destroy()
{

	SafeDelete(VB);
	SafeDelete(IB);
	SafeDelete(Cam);
}