#include "IndexBuffer.h"
// std::move
#include <utility>
// std::memeset
#include <memory>
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			IndexBuffer::IndexBuffer() : buffer(nullptr), indexCount(0u)
			{
			}

			IndexBuffer::IndexBuffer(IndexBuffer&& moveIndexBuffer) noexcept
				: buffer(std::exchange(moveIndexBuffer.buffer, nullptr)),
				indexCount(std::exchange(moveIndexBuffer.indexCount, 0u))
			{
			}

			IndexBuffer::~IndexBuffer()
			{
				Release();
			}

			IndexBuffer& IndexBuffer::operator=(IndexBuffer&& rhs) noexcept
			{
				if (this != &rhs)
				{
					buffer = std::exchange(rhs.buffer, nullptr);
					indexCount = std::exchange(rhs.indexCount, 0u);
				}

				return *this;
			}

			HRESULT IndexBuffer::Initialize(ID3D11Device* device, DWORD* indices, UINT indexCount)
			{
				// 버퍼가 있는 경우 리셋(내부 포인터를 해제하고 nullptr로 설정)
				if (buffer.Get()) // buffer != nullptr
					buffer.Reset();

				this->indexCount = indexCount;

				D3D11_BUFFER_DESC indexBufferDescription;
				std::memset(&indexBufferDescription, 0, sizeof(D3D11_BUFFER_DESC));
				indexBufferDescription.ByteWidth = sizeof(DWORD) * indexCount; // 버퍼 크기 설정
				indexBufferDescription.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
				indexBufferDescription.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;

				D3D11_SUBRESOURCE_DATA indexBufferData;
				std::memset(&indexBufferData, 0, sizeof(D3D11_SUBRESOURCE_DATA));
				indexBufferData.pSysMem = indices; // 버퍼 데이터 초기화

				// 인덱스 버퍼 생성
				return device->CreateBuffer(&indexBufferDescription, &indexBufferData, buffer.GetAddressOf());
			}

			void IndexBuffer::Release()
			{
				CommonUtility::SafeComPtrRelease(buffer);
			}

			ID3D11Buffer* IndexBuffer::Get() const
			{
				return buffer.Get();
			}

			ID3D11Buffer* const* IndexBuffer::GetAddressOf() const
			{
				return buffer.GetAddressOf();
			}

			UINT IndexBuffer::GetIndexCount() const
			{
				return indexCount;
			}
		} // Resource
	} // Graphics
} // Engine