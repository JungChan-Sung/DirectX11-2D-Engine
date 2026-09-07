#include "BaseConstantBuffer.h"
// std::move
#include <utility>
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			BaseConstantBuffer::BaseConstantBuffer() : buffer(nullptr)
			{
			}

			BaseConstantBuffer::BaseConstantBuffer(BaseConstantBuffer&& moveBaseConstantBuffer) noexcept
				: buffer(std::exchange(moveBaseConstantBuffer.buffer, nullptr))
			{
			}

			BaseConstantBuffer::~BaseConstantBuffer()
			{
				Release();
			}

			BaseConstantBuffer& BaseConstantBuffer::operator=(BaseConstantBuffer&& rhs) noexcept
			{
				if (this != &rhs)
					buffer = std::exchange(rhs.buffer, nullptr);

				return *this;
			}

			void BaseConstantBuffer::Release()
			{
				CommonUtility::SafeComPtrRelease(buffer);
			}

			ID3D11Buffer* BaseConstantBuffer::Get() const
			{
				return buffer.Get();
			}

			ID3D11Buffer* const* BaseConstantBuffer::GetAddressOf() const
			{
				return buffer.GetAddressOf();
			}
		} // Resource
	} // Graphics
} // Engine