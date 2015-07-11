#ifndef _MODEL_H_
#define _MODEL_H_

#include "BasicIncludes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include <vector>
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

namespace NTEngine
{
	struct Subset
	{
		Subset()
		{
			ZeroMemory(this, sizeof(Subset));
		}

		UINT ID;
		UINT VertexStart;
		UINT VertexCount;
		UINT FaceStart;
		UINT FaceCount;
	};


	class Model
	{
	public:
		DLLEXPORT Model();
		DLLEXPORT ~Model();

		DLLEXPORT VOID LoadFromFile(ID3D11Device* dev, const std::string& file, bool HasNormalMaps);

		template <typename ShaderType>
		DLLEXPORT VOID inline Render(ID3D11DeviceContext* devcon, ShaderType* Shader);

	private:
		VOID inline LoadDiffuseMap(aiMaterial* mat);
		VOID inline LoadNormalMap(aiMaterial* mat);

	private:
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;

		UINT m_NumVertices;
		UINT m_NumFaces;

		bool m_HasNormalMaps;

		std::vector<Subset> m_Subsets;
		std::vector<ID3D11ShaderResourceView*> m_DiffuseMaps;
		std::vector<ID3D11ShaderResourceView*> m_NormalMaps;
	};


	template <typename ShaderType>
	void Model::Render(ID3D11DeviceContext* devcon, ShaderType* Shader)
	{
		UINT offset = 0;

		const UINT& VertexStride = m_VertexBuffer->GetVertexStride();

		ID3D11Buffer* VB = m_VertexBuffer->GetVertexBuffer();
		ID3D11Buffer* IB = m_IndexBuffer->GetIndexBuffer();

		devcon->IASetVertexBuffers(0, 1, &VB, &VertexStride, &offset);
		devcon->IASetIndexBuffer(IB, m_IndexBuffer->GetIndexBufferFormat(), 0);

		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		for (UINT i = 0; i < m_Subsets.size(); ++i)
		{
			if (m_DiffuseMaps[i] == nullptr)
				continue;

	
			Shader->SetDiffuseMap(m_DiffuseMaps[i]);

			devcon->DrawIndexed(m_Subsets[i].FaceCount * 3, m_Subsets[i].FaceStart * 3, 0);
		}
	}

}

#endif