#include "TextureRenderResource.h"
// std::move
#include <utility>

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			TextureRenderResource::TextureRenderResource(VertexBuffer* vertexBuffer, IndexBuffer* indexBuffer, Texture* texture)
				: RenderResource(vertexBuffer, indexBuffer), texture(texture)
			{
			}

			TextureRenderResource::TextureRenderResource(TextureRenderResource&& moveTextureRenderResource) noexcept
				: RenderResource(std::move(moveTextureRenderResource)),
				texture(std::exchange(moveTextureRenderResource.texture, nullptr))
			{
			}

			TextureRenderResource::~TextureRenderResource()
			{
				Release();
			}

			TextureRenderResource& XM_CALLCONV TextureRenderResource::operator=(TextureRenderResource&& rhs) noexcept
			{
				if (this != &rhs)
				{
					// 렌더 리소스
					vertexBuffer = std::exchange(rhs.vertexBuffer, nullptr);
					indexBuffer = std::exchange(rhs.indexBuffer, nullptr);
					DirectX::XMFLOAT4X4 identityMatrix;
					DirectX::XMStoreFloat4x4(&identityMatrix, DirectX::XMMatrixIdentity());
					transformMatrix = std::exchange(rhs.transformMatrix, identityMatrix);

					// 텍스처 렌더 리소스
					texture = std::exchange(rhs.texture, nullptr);
				}

				return *this;
			}

			void TextureRenderResource::Release()
			{
				texture = nullptr;
			}

			Texture* TextureRenderResource::GetTexture() const
			{
				return texture;
			}
		} // Resource
	} // Graphics
} // Engine