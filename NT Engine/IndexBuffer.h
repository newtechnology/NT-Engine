#ifndef _INDEX_BUFFER_H_
#define _INDEX_BUFFER_H_

#include "BasicIncludes.h"
#include <assert.h>
#include <d3d11.h>

namespace NTEngine
{
	class IndexBuffer
	{
	public:
		DLLEXPORT IndexBuffer();
		DLLEXPORT ~IndexBuffer();

		DLLEXPORT inline ID3D11Buffer* GetIndexBuffer();
		DLLEXPORT inline UINT GetIndexCount()const;


		template <typename T>
		VOID SetIndices(ID3D11Device* device, const T* Indices, UINT NumberOfIndices,
			D3D11_USAGE Usage = D3D11_USAGE_IMMUTABLE,
			UINT CPUAccessFlags = 0, UINT MiscFlags = 0, UINT StructureByteStride = 0);

	private:
		ID3D11Buffer* m_IndexBuffer;
		UINT m_IndexCount;
	};


	template <typename IndexType>
	void IndexBuffer::SetIndices(ID3D11Device* device, const IndexType* indices, UINT NumberOfIndices,
		D3D11_USAGE Usage, UINT CPUAccessFlags, UINT MiscFlags, UINT StructureByteStride)
	{
		//Release the old Index Buffer in case we're re-using it
		ReleaseCOM(m_IndexBuffer);

		assert(device);

#if defined(DEBUG) | defined(_DEBUG)
		if (NumberOfIndices == 0)
			ShowError("The NumberOfIndices parameter in IndexBuffer::SetIndices() cannot be zero.");
#endif


		m_IndexCount = NumberOfIndices;

		D3D11_BUFFER_DESC ibd;

		ibd.Usage = Usage;
		ibd.ByteWidth = sizeof(IndexType) * NumberOfIndices;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = CPUAccessFlags;
		ibd.MiscFlags = MiscFlags;
		ibd.StructureByteStride = StructureByteStride;


		D3D11_SUBRESOURCE_DATA iinitData;
		iinitData.pSysMem = indices;

		HR(device->CreateBuffer(&ibd, &iinitData, &m_IndexBuffer));
	}
}



#endif
