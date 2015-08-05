#ifndef _MODEL_H_
#define _MODEL_H_

#include "BasicIncludes.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Vertex.h"

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
		virtual DLLEXPORT ~Model();

		DLLEXPORT virtual VOID LoadFromFile(ID3D11Device* dev, const std::string& file) = 0;
	
	protected:
		DLLEXPORT VOID ExtractIndices(ID3D11Device* dev, const aiScene* scene);
		DLLEXPORT VOID LoadIndices(ID3D11Device* dev, std::vector<UINT>& indices);
		VOID inline LoadDiffuseMap(aiMaterial* mat);

	protected:
		VertexBuffer* m_VertexBuffer;
		IndexBuffer* m_IndexBuffer;

		UINT m_NumVertices;
		UINT m_NumFaces;
		UINT m_Flags;

		bool m_HasNormalMaps;

		std::vector<Subset> m_Subsets;

		std::vector<ID3D11ShaderResourceView*> m_DiffuseMaps;

	};

	class BasicModel : public Model
	{
	public:
		BasicModel() {};
		~BasicModel() {};

		DLLEXPORT VOID LoadFromFile(ID3D11Device* dev, const std::string& file);

		DLLEXPORT VOID inline Render(ID3D11DeviceContext* devcon);

	};


	class NormalMappedModel : public Model
	{
	public:
		NormalMappedModel() {}
		~NormalMappedModel() {}

		DLLEXPORT VOID LoadFromFile(ID3D11Device* dev, const std::string& file);

		DLLEXPORT VOID inline Render(ID3D11DeviceContext* devcon);

	private:
		VOID inline LoadNormalMap(aiMaterial* mat);

	private:
		std::vector<ID3D11ShaderResourceView*> m_NormalMaps;
	};

	

}

#endif