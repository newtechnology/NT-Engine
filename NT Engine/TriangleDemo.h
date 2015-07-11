#ifndef _TRIANGLE_DEMO_H_
#define _TRIANGLE_DEMO_H_

#include "BasicIncludes.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "Camera.h"


class TriangleDemo
{
public:
	DLLEXPORT static void Initialize(ID3D11Device* dev, NTEngine::Renderer& renderer);
	static void Destroy();

	DLLEXPORT static void Update(float dt, NTEngine::Renderer& renderer);

	static NTEngine::VertexBuffer* VB;
	static NTEngine::IndexBuffer* IB;
	static NTEngine::Camera* Cam;
};





#endif