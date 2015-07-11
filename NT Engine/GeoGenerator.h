#ifndef _GEOGENERATOR_H_
#define _GEOGENERATOR_H_

#include "BasicIncludes.h"

#include <vector>

namespace NTEngine
{
	class GeometryGenerator
	{
	public:
		struct Vertex
		{
			Vertex(){}
			Vertex(const XMFLOAT3& p, const XMFLOAT3& n, const XMFLOAT3& t, const XMFLOAT2& uv)
				: Position(p), Normal(n), TangentU(t), TexC(uv){}
			Vertex(
				float px, float py, float pz,
				float nx, float ny, float nz,
				float tx, float ty, float tz,
				float u, float v)
				: Position(px, py, pz), Normal(nx, ny, nz),
				TangentU(tx, ty, tz), TexC(u, v){}

			XMFLOAT3 Position;
			XMFLOAT3 Normal;
			XMFLOAT3 TangentU;
			XMFLOAT2 TexC;
		};

		struct MeshData
		{
			std::vector<Vertex> Vertices;
			std::vector<UINT> Indices;
		};

		///<summary>
		/// Creates a box centered at the origin with the given dimensions.
		///</summary>
		void CreateBox(float width, float height, float depth, MeshData& meshData);

	};
}

#endif //_GEOGENERATORH_H_
