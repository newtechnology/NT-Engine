#ifndef _TRIANGLE_DEMO_H_
#define _TRIANGLE_DEMO_H_


#include "BasicIncludes.h"
#include "Vertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"


class TriangleDemo
{
public:
	static void Initialize(ID3D11Device* dev);
	static void Destroy();


	static NTEngine::VertexBuffer* VB;
	static NTEngine::IndexBuffer* IB;
};





#endif