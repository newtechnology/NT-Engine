#include "Model.h"
#include "Vertex.h"
#include "TextureManager.h"
#include "Shaders.h"

namespace NTEngine
{
	Model::Model()
	{
		m_VertexBuffer = nullptr;
		m_IndexBuffer = nullptr;
	}


	Model::~Model()
	{
		SafeDelete(m_VertexBuffer);
		SafeDelete(m_IndexBuffer);
	}


	void Model::LoadFromFile(ID3D11Device* dev, const std::string& file, bool HasNormalMaps)
	{
		m_VertexBuffer = new VertexBuffer();
		m_IndexBuffer = new IndexBuffer();

		m_HasNormalMaps = HasNormalMaps;

		Assimp::Importer imp;

		const aiScene* pScene = imp.ReadFile(file,
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_OptimizeMeshes |
			aiProcess_OptimizeGraph |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_ConvertToLeftHanded |
			aiProcess_SortByPType);

		if (pScene == NULL)
			ShowError(imp.GetErrorString());

		std::vector<Vertex::Basic32> vertices;
		std::vector<UINT> indices;


		for (UINT i = 0; i < pScene->mNumMeshes; ++i)
		{
			aiMesh* CurrentMesh = pScene->mMeshes[i];

			Subset subset;

			subset.VertexCount = CurrentMesh->mNumVertices;
			subset.VertexStart = vertices.size();
			subset.FaceStart = indices.size() / 3;
			subset.FaceCount = CurrentMesh->mNumFaces;
			subset.ID = CurrentMesh->mMaterialIndex;

			m_NumVertices += subset.VertexCount;
			m_NumFaces += subset.FaceCount; 

			for (UINT j = 0; j < subset.VertexCount; ++j)
			{
				Vertex::Basic32 vertex;

				vertex.Pos.x = CurrentMesh->mVertices[j].x;
				vertex.Pos.y = CurrentMesh->mVertices[j].y;
				vertex.Pos.z = CurrentMesh->mVertices[j].z;

				vertex.Normal.x = CurrentMesh->mNormals[j].x;
				vertex.Normal.y = CurrentMesh->mNormals[j].y;
				vertex.Normal.z = CurrentMesh->mNormals[j].z;


				if (CurrentMesh->HasTextureCoords(0))
				{
					vertex.Tex.x = CurrentMesh->mTextureCoords[0][j].x;
					vertex.Tex.y = CurrentMesh->mTextureCoords[0][j].y;
				}


				vertices.push_back(vertex);
			}

			
			//for each triangle in this subset
			for (UINT j = 0; j < subset.FaceCount; ++j)
			{
				//for each index in this triangle
				for (UINT index = 0; index < CurrentMesh->mFaces[j].mNumIndices; ++index)
				{
					indices.push_back(subset.VertexStart + CurrentMesh->mFaces[j].mIndices[index]);
				}
			}

			
			aiMaterial* CurrentMeshMaterial = pScene->mMaterials[subset.ID];

			//Extract diffuse maps
			LoadDiffuseMap(CurrentMeshMaterial);

			//Extract normal maps
			if (HasNormalMaps)
				LoadNormalMap(CurrentMeshMaterial);


			m_Subsets.push_back(subset);
		}

		std::vector<USHORT> Indices16;

		bool UseIndices16 = false;

		//use USHORT for indices if we can
		if (indices.size() < USHRT_MAX)
		{
			//convert to USHORT indices
			for (UINT k = 0; k < indices.size(); ++k)
			{
				Indices16[k] = static_cast<USHORT>(indices[k]);
			}

			UseIndices16 = true;
		}

#if defined(_DEBUG) | defined(DEBUG)
		assert(m_DiffuseMaps.size() == m_Subsets.size());
#endif

		if (UseIndices16)
		{
			m_IndexBuffer->SetIndices(dev, &Indices16[0], Indices16.size(), DXGI_FORMAT_R16_UINT);

#if defined(_DEBUG) | defined(DEBUG)
			char s[200];
			sprintf_s(s, "Using 16-bit Indices for %s\n", file.c_str());

			OutputDebugString(s);
#endif
		}
		else
		{
			m_IndexBuffer->SetIndices(dev, &indices[0], indices.size(), DXGI_FORMAT_R32_UINT);
			
#if defined(_DEBUG) | defined(DEBUG)
			char s[200];
			sprintf_s(s, "Using 32-bit Indices for %s\n", file.c_str());

			OutputDebugString(s);
#endif
		}

		m_VertexBuffer->SetVertices(dev, &vertices[0], vertices.size());
	}


	void Model::LoadDiffuseMap(aiMaterial* mat)
	{
		aiString path;

		ID3D11ShaderResourceView* SRV = nullptr;

		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 && mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			std::string FileName = path.C_Str();

			FileName = removeExtension(FileName);
			FileName = "Textures\\" + FileName + "_nrm" + ".dds";


			if (!FileExists(FileName))
			{
#if defined(DEBUG) | defined(_DEBUG)
				char s[200];
				sprintf_s(s, "File does not exists: %s\n", FileName.c_str());

				OutputDebugString(s);
#endif

				m_NormalMaps.push_back(SRV);

				return;
			}

			SRV = TextureManager::Instance()->Create(FileName);

			m_NormalMaps.push_back(SRV);
		}
		else
		{
			m_NormalMaps.push_back(SRV);
		}
	}
	
	void Model::LoadDiffuseMap(aiMaterial* mat)
	{
		aiString path;

		ID3D11ShaderResourceView* SRV = nullptr;

		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 && mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
		{
			std::string FileName = path.C_Str();

			FileName = removeExtension(FileName);
			FileName = "Textures\\" + FileName + ".dds";

		
			if (!FileExists(FileName))
			{
#if defined(DEBUG) | defined(_DEBUG)
				char s[200];
				sprintf_s(s, "File does not exists: %s\n", FileName.c_str());

				OutputDebugString(s);
#endif
				
				m_DiffuseMaps.push_back(SRV);

				return;
			}

			SRV = TextureManager::Instance()->Create(FileName);

			m_DiffuseMaps.push_back(SRV);
		}
		else
		{
			m_DiffuseMaps.push_back(SRV);
		}
	}

}




