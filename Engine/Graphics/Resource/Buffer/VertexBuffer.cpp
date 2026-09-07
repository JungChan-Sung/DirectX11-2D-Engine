#include "VertexBuffer.h"
// std::move
#include <utility>
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			VertexBuffer::VertexBuffer() : buffer(nullptr), memoryStride(0u), vertexCount(0u)
			{
			}

			VertexBuffer::VertexBuffer(VertexBuffer&& moveVertexBuffer) noexcept
				: buffer(std::exchange(moveVertexBuffer.buffer, nullptr)),
				memoryStride(std::exchange(moveVertexBuffer.memoryStride, 0u)),
				vertexCount(std::exchange(moveVertexBuffer.vertexCount, 0u))
			{
			}

			VertexBuffer::~VertexBuffer()
			{
				Release();
			}

			VertexBuffer& VertexBuffer::operator=(VertexBuffer&& rhs) noexcept
			{
				if (this != &rhs)
				{
					buffer = std::exchange(rhs.buffer, nullptr);
					memoryStride = std::exchange(rhs.memoryStride, 0u);
					vertexCount = std::exchange(rhs.vertexCount, 0u);
				}

				return *this;
			}

			void VertexBuffer::Release()
			{
				CommonUtility::SafeComPtrRelease(buffer);
			}

			ID3D11Buffer* VertexBuffer::Get() const
			{
				return buffer.Get();
			}

			ID3D11Buffer* const* VertexBuffer::GetAddressOf() const
			{
				return buffer.GetAddressOf();
			}

			UINT VertexBuffer::GetVertexCount() const
			{
				return vertexCount;
			}

			UINT VertexBuffer::GetStride() const
			{
				return memoryStride;
			}

			const UINT* VertexBuffer::GetStridePtr() const
			{
				return &memoryStride;
			}
		} // Resource
	} // Graphics
} // Engine