#include "VertexBuffer.h"

namespace NTEngine
{
	VertexBuffer::VertexBuffer()
	{
		m_VertexBuffer = nullptr;

		m_VertexCount = 0;
		m_VertexStride = 0;
	}

	VertexBuffer::~VertexBuffer()
	{
		ReleaseCOM(m_VertexBuffer);
	}

	ID3D11Buffer* VertexBuffer::GetVertexBuffer()
	{
#if defined(DEBUG) | defined(_DEBUG)
		if (m_VertexBuffer == nullptr)
			ShowError("Error: The VertexBuffer is null. You cannot call VertexBuffer::GetVertexBuffer() without initializing it.");
#endif
		return m_VertexBuffer;
	}

	UINT VertexBuffer::GetVertexStride()const
	{
#if defined(DEBUG) | defined(_DEBUG)
		if (m_VertexStride == 0)
			ShowError("Error: The VertexStride is null.");
#endif

		return m_VertexStride;
	}

	UINT VertexBuffer::GetVertexCount()const
	{
#if defined(DEBUG) | defined(_DEBUG)
		if (m_VertexCount == 0)
			ShowError("Error: The VertexCount is null.");
#endif

		return m_VertexCount;
	}
}