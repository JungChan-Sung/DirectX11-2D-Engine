#include "RenderResource.h"
// std::move
#include <utility>

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			RenderResource::RenderResource(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer)
				: vertexBuffer(vertexBuffer), indexBuffer(indexBuffer)
			{
				DirectX::XMStoreFloat4x4(&transformMatrix, DirectX::XMMatrixIdentity());
			}

			RenderResource::RenderResource(RenderResource&& moveRenderResource) noexcept
				: vertexBuffer(std::exchange(moveRenderResource.vertexBuffer, nullptr)),
				indexBuffer(std::exchange(moveRenderResource.indexBuffer, nullptr)),
				transformMatrix(std::move(moveRenderResource.transformMatrix))
			{
				DirectX::XMStoreFloat4x4(&moveRenderResource.transformMatrix, DirectX::XMMatrixIdentity());
			}

			RenderResource::~RenderResource()
			{
				Release();
			}

			RenderResource& XM_CALLCONV RenderResource::operator=(RenderResource&& rhs) noexcept
			{
				if (this != &rhs)
				{
					vertexBuffer = std::exchange(rhs.vertexBuffer, nullptr);
					indexBuffer = std::exchange(rhs.indexBuffer, nullptr);
					DirectX::XMFLOAT4X4 identityMatrix;
					DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());
					transformMatrix = std::exchange(rhs.transformMatrix, identityMatrix);
				}

				return *this;
			}

			void RenderResource::Release()
			{
				indexBuffer = nullptr;
				vertexBuffer = nullptr;
			}

			void XM_CALLCONV RenderResource::SetTransformMatrix(DirectX::FXMMATRIX transformMatrix)
			{
				DirectX::XMStoreFloat4x4(&this->transformMatrix, transformMatrix);
			}

			VertexBuffer* RenderResource::GetVertexBuffer() const
			{
				return vertexBuffer;
			}

			IndexBuffer* RenderResource::GetIndexBuffer() const
			{
				return indexBuffer;
			}

			DirectX::XMMATRIX XM_CALLCONV RenderResource::GetTransformMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&transformMatrix);
			}
		} // Resource
	} // Graphics
} // Engine
