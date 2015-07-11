#ifndef _VERTEX_BUFFER_H_
#define _VERTEX_BUFFER_H_

#include "BasicIncludes.h"
#include <d3d11.h>
#include <assert.h>

namespace NTEngine
{
	class VertexBuffer
	{
	public:
		DLLEXPORT VertexBuffer();
		DLLEXPORT ~VertexBuffer();

		DLLEXPORT inline ID3D11Buffer* GetVertexBuffer();
		DLLEXPORT inline UINT GetVertexStride()const;
		DLLEXPORT inline UINT GetVertexCount()const;

		template <typename VertexType>
		VOID inline SetVertices(ID3D11Device* device,
			const VertexType* vertices, UINT VertexCount,
			D3D11_USAGE Usage = D3D11_USAGE_IMMUTABLE,
			UINT CPUAccessFlags = 0, UINT MiscFlags = 0, UINT StructureByteStride = 0);
	private:
		ID3D11Buffer* m_VertexBuffer;
		UINT m_VertexStride;
		UINT m_VertexCount;
	};


	template <typename VertexType>
	void VertexBuffer::SetVertices(ID3D11Device* device, const VertexType* vertices, UINT VertexCount,
		D3D11_USAGE Usage, UINT CPUAccessFlags, UINT MiscFlags, UINT StructureByteStride)
	{
		//Release the old vertex buffer in case we're re-using it
		ReleaseCOM(m_VertexBuffer);
		
		assert(device != nullptr);

#if defined(DEBUG) | defined(_DEBUG)
		if (VertexCount == 0)
			ShowError("The VertexCount parameter in VertexBuffer::SetVertices() cannot be zero.");
#endif


		//cache this for later needs
		m_VertexCount = VertexCount;
		m_VertexStride = sizeof(VertexType);

		D3D11_BUFFER_DESC vbd;

		vbd.Usage = Usage;
		vbd.ByteWidth = sizeof(VertexType) * VertexCount;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = CPUAccessFlags;
		vbd.MiscFlags = MiscFlags;
		vbd.StructureByteStride = StructureByteStride;

		D3D11_SUBRESOURCE_DATA vinitData;
		vinitData.pSysMem = vertices;

		HR(device->CreateBuffer(&vbd, &vinitData, &m_VertexBuffer));
	}
}


#endif //_VERTEX_BUFFER_H_