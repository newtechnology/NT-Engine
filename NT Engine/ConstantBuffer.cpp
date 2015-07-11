#include "ConstantBuffer.h"
#include <assert.h>

namespace NTEngine
{
	ConstantBuffer::ConstantBuffer()
	{
		m_ConstantBuffer = nullptr;
	}

	ConstantBuffer::~ConstantBuffer()
	{
		ReleaseCOM(m_ConstantBuffer);
	}

	ID3D11Buffer* ConstantBuffer::GetConstantBuffer()
	{
		return m_ConstantBuffer;
	}

	UINT ConstantBuffer::GetBufferSlot()const
	{
		return m_BufferSlot;
	}

	
	void ConstantBuffer::SetConstantBuffer(ID3D11Device* device, UINT BufferSize, UINT BufferSlot,
		D3D11_USAGE Usage, UINT CPUAccessFlags, UINT MiscFlags, UINT StructureByteStride)
	{
#if defined(DEBUG) | defined(_DEBUG)
		if (BufferSize == 0)
			ShowError("BufferSize parameter in ConstantBuffer::SetConstantBuffer() cannot be zero.");
		
		assert(device);
#endif
		m_BufferSlot = BufferSlot;

		D3D11_BUFFER_DESC cbbd;

		ZeroMemory(&cbbd, sizeof(D3D11_BUFFER_DESC));

		cbbd.Usage = Usage;
		cbbd.ByteWidth = BufferSize; 
		cbbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbbd.CPUAccessFlags = CPUAccessFlags;
		cbbd.MiscFlags = MiscFlags;
		cbbd.StructureByteStride = StructureByteStride;

		
		HR(device->CreateBuffer(&cbbd, 0, &m_ConstantBuffer));

	}

}