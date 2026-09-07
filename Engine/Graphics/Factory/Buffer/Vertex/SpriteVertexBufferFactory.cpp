#include "SpriteVertexBufferFactory.h"
#include <vector>
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/ResourceFactoryErrorMessage.h"
#include "Engine/Graphics/Common/Vertex.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			Resource::VertexBuffer* SpriteVertexBufferFactory::Create(ID3D11Device* device)
			{
				// 스프라이트 정점 버퍼 생성
				Resource::VertexBuffer* spriteVertexBuffer = new Resource::VertexBuffer;
				if (!spriteVertexBuffer) // spriteVertexBuffer == nullptr
				{
					CommonUtility::Log::ErrorMessage(Message::Error::CREATE_SPRITE_VERTEX_BUFFER_FAILURE);
					return nullptr;
				}

				// 스프라이트 정점 배열
				std::vector<Common::VertexTexture> vertices =
				{
					// VertexTexture(x, y, u, v)
					Common::VertexTexture(0.0f, 0.0f, 0.0f, 0.0f), // Top Left
					Common::VertexTexture(1.0f, 0.0f, 1.0f, 0.0f), // Top Right
					Common::VertexTexture(0.0f, 1.0f, 0.0f, 1.0f), // Bottom Left
					Common::VertexTexture(1.0f, 1.0f, 1.0f, 1.0f)  // Bottom Right
				};

				// 스프라이트 정점 버퍼 초기화
				HRESULT hr = spriteVertexBuffer->Initialize(device, vertices.data(), static_cast<UINT>(vertices.size()));
				if (FAILED(hr))
				{
					CommonUtility::SafeDelete(spriteVertexBuffer);
					CommonUtility::Log::ErrorMessage(hr, Message::Error::INITIALIZE_SPRITE_VERTEX_BUFFER_FAILURE);
					return nullptr;
				}

				return spriteVertexBuffer;
			}
		} // Factory
	} // Graphics
} // Engine
