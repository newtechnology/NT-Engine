#include "Model.h"
#include "TextureManager.h"
#include "Shaders.h"

namespace NTEngine
{

#pragma region BaseModel
	Model::Model()
	{
		m_VertexBuffer = new VertexBuffer();
		m_IndexBuffer = new IndexBuffer();

		m_Flags = aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_OptimizeMeshes |
			aiProcess_OptimizeGraph |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_ConvertToLeftHanded |
			aiProcess_SortByPType;
	}


	Model::~Model()
	{
		SafeDelete(m_VertexBuffer);
		SafeDelete(m_IndexBuffer);

#if defined(DEBUG) | defined(_DEBUG)
		OutputDebugString("Model released.\n");
#endif
	}

	void Model::LoadIndices(ID3D11Device* dev, std::vector<UINT>& indices)
	{
		std::vector<USHORT> Indices16(indices.size());
	
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


		if (UseIndices16)
			m_IndexBuffer->SetIndices(dev, &Indices16[0], Indices16.size(), DXGI_FORMAT_R16_UINT);
		else
			m_IndexBuffer->SetIndices(dev, &indices[0], indices.size(), DXGI_FORMAT_R32_UINT);
	}

	void Model::ExtractIndices(ID3D11Device* dev, const aiScene* pScene)
	{

#if defined(DEBUG) | defined(_DEBUG)
		assert(!m_Subsets.empty());

#endif

		std::vector<UINT> indices;
		
		for (UINT i = 0; i < pScene->mNumMeshes; ++i)
		{
			m_Subsets[i].FaceStart = indices.size() / 3;

			aiMesh* CurrentMesh = pScene->mMeshes[i];

			//for each triangle in this subset
			for (UINT j = 0; j < CurrentMesh->mNumFaces; ++j)
			{
				//for each index in this triangle
				for (UINT index = 0; index < CurrentMesh->mFaces[j].mNumIndices; ++index)
				{
					indices.push_back(m_Subsets[i].VertexStart + CurrentMesh->mFaces[j].mIndices[index]);
				}
			}

		}

		LoadIndices(dev, indices);

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

#pragma endregion


#pragma region BasicModel

	void BasicModel::LoadFromFile(ID3D11Device* dev, const std::string& file)
	{

		Assimp::Importer imp;

		const aiScene* pScene = imp.ReadFile(file, m_Flags);

		if (pScene == NULL)
			ShowError(imp.GetErrorString());

		std::vector<Vertex::Basic32> vertices;


		for (UINT i = 0; i < pScene->mNumMeshes; ++i)
		{
			aiMesh* CurrentMesh = pScene->mMeshes[i];

			Subset subset;

			subset.VertexCount = CurrentMesh->mNumVertices;
			subset.VertexStart = vertices.size();
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

			aiMaterial* CurrentMeshMaterial = pScene->mMaterials[subset.ID];

			//Extract diffuse maps
			LoadDiffuseMap(CurrentMeshMaterial);

			m_Subsets.push_back(subset);
		}

		ExtractIndices(dev, pScene);

		m_VertexBuffer->SetVertices(dev, &vertices[0], vertices.size());
	}


	void BasicModel::Render(ID3D11DeviceContext* devcon)
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

			Shaders::Basic->SetDiffuseMap(m_DiffuseMaps[i]);

			devcon->DrawIndexed(m_Subsets[i].FaceCount * 3, m_Subsets[i].FaceStart * 3, 0);
		}
	}

#pragma endregion


#pragma region NormalMappedModel

	void NormalMappedModel::LoadFromFile(ID3D11Device* dev, const std::string& file)
	{
		Assimp::Importer imp;

		const aiScene* pScene = imp.ReadFile(file, m_Flags);

		if (pScene == NULL)
			ShowError(imp.GetErrorString());

		std::vector<Vertex::NormalMap> vertices;

		for (UINT i = 0; i < pScene->mNumMeshes; ++i)
		{
			aiMesh* CurrentMesh = pScene->mMeshes[i];

			Subset subset;

			subset.VertexCount = CurrentMesh->mNumVertices;
			subset.VertexStart = vertices.size();
			subset.FaceCount = CurrentMesh->mNumFaces;
			subset.ID = CurrentMesh->mMaterialIndex;

			m_NumVertices += subset.VertexCount;
			m_NumFaces += subset.FaceCount;

			for (UINT j = 0; j < subset.VertexCount; ++j)
			{
				Vertex::NormalMap vertex;

				vertex.Pos.x = CurrentMesh->mVertices[j].x;
				vertex.Pos.y = CurrentMesh->mVertices[j].y;
				vertex.Pos.z = CurrentMesh->mVertices[j].z;

				vertex.Normal.x = CurrentMesh->mNormals[j].x;
				vertex.Normal.y = CurrentMesh->mNormals[j].y;
				vertex.Normal.z = CurrentMesh->mNormals[j].z;

				if (CurrentMesh->HasTangentsAndBitangents())
				{
					vertex.Tangent.x = CurrentMesh->mTangents[j].x;
					vertex.Tangent.y = CurrentMesh->mTangents[j].y;
					vertex.Tangent.z = CurrentMesh->mTangents[j].z;
				}

				if (CurrentMesh->HasTextureCoords(0))
				{
					vertex.Tex.x = CurrentMesh->mTextureCoords[0][j].x;
					vertex.Tex.y = CurrentMesh->mTextureCoords[0][j].y;
				}
				
				vertices.push_back(vertex);
			}

			aiMaterial* CurrentMeshMaterial = pScene->mMaterials[subset.ID];

			//Extract diffuse maps
			LoadDiffuseMap(CurrentMeshMaterial);

			//Extract normal maps
			LoadNormalMap(CurrentMeshMaterial);
		
			m_Subsets.push_back(subset);
		}

		ExtractIndices(dev, pScene);

		m_VertexBuffer->SetVertices(dev, &vertices[0], vertices.size());
	}


	void NormalMappedModel::LoadNormalMap(aiMaterial* mat)
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


	void NormalMappedModel::Render(ID3D11DeviceContext* devcon)
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
			//skip this mesh subset if diffuse map or normal map is missing
			//this is just a temporary hack and it's going to be removed soon 

			if (m_DiffuseMaps[i] == nullptr)
				continue;

			if (m_NormalMaps[i] == nullptr)
				continue;

			Shaders::NormalMap->SetDiffuseAndNormalMap(m_DiffuseMaps[i], m_NormalMaps[i]);

			devcon->DrawIndexed(m_Subsets[i].FaceCount * 3, m_Subsets[i].FaceStart * 3, 0);
		}
	}

#pragma endregion

}




