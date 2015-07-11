#ifndef _CONSTANT_BUFFER_H_
#define _CONSTANT_BUFFER_H_

#include "BasicIncludes.h"
#include <d3d11.h>

namespace NTEngine
{
	class ConstantBuffer
	{
	public:
		DLLEXPORT ConstantBuffer();
		DLLEXPORT ~ConstantBuffer();

		DLLEXPORT inline ID3D11Buffer* GetConstantBuffer();
		DLLEXPORT inline UINT GetBufferSlot()const;

		template<typename BufferStructType>
		DLLEXPORT VOID inline UpdateBuffer(ID3D11DeviceContext* devcon, BufferStructType& BufferStruct);
		
		DLLEXPORT VOID inline SetConstantBuffer(ID3D11Device* device, UINT BufferSize,  UINT BufferSlot,
			D3D11_USAGE Usage = D3D11_USAGE_DYNAMIC,
			UINT CPUAccessFlags = D3D11_CPU_ACCESS_WRITE, UINT MiscFlags = 0, UINT StructureByteStride = 0);

	private:
		ID3D11Buffer* m_ConstantBuffer;
		UINT m_BufferSlot;
	};

	//NOTE: Make sure to transpose matrix before sending them via constant buffer to the shaders!

	template<typename BufferStructType>
	void ConstantBuffer::UpdateBuffer(ID3D11DeviceContext* devcon, BufferStructType& BufferStruct)
	{
		D3D11_MAPPED_SUBRESOURCE mappedRes;

		BufferStructType* dataPtr;

		HR(devcon->Map(m_ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedRes));

		dataPtr = (BufferStructType*)mappedRes.pData;
		
		*dataPtr = BufferStruct;

		devcon->Unmap(m_ConstantBuffer, 0);

	}
}


#endif