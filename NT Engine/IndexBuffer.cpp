#include "IndexBuffer.h"

namespace NTEngine
{
	IndexBuffer::IndexBuffer()
	{
		m_IndexBuffer = nullptr;
		m_IndexCount = 0;

	}

	IndexBuffer::~IndexBuffer()
	{
		ReleaseCOM(m_IndexBuffer);
	}

	UINT IndexBuffer::GetIndexCount()const
	{
#if defined(DEBUG) | defined(_DEBUG)
		if (m_IndexCount == 0)
			ShowError("Error: The IndexCount is null.");
#endif
		return m_IndexCount;
	}

	ID3D11Buffer* IndexBuffer::GetIndexBuffer()
	{
#if defined(DEBUG) | defined(_DEBUG)
		if (m_IndexBuffer == nullptr)
			ShowError("Error: The index buffer is null. You cannot call IndexBuffer::GetIndexBuffer() without initializing it.");
#endif

		return m_IndexBuffer;
	}

}