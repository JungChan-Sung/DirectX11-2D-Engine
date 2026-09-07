#include "SpriteIndexBufferFactory.h"
#include <vector>
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/ResourceFactoryErrorMessage.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			Resource::IndexBuffer* SpriteIndexBufferFactory::Create(ID3D11Device* device)
			{
				// 스프라이트 정점 버퍼 생성
				Resource::IndexBuffer* spriteIndexBuffer = new Resource::IndexBuffer;
				if (!spriteIndexBuffer) // spriteIndexBuffer == nullptr
				{
					CommonUtility::Log::ErrorMessage(Message::Error::CREATE_SPRITE_INDEX_BUFFER_FAILURE);
					return nullptr;
				}

				// 스프라이트 인덱스 배열
				std::vector<DWORD> indices =
				{
					0, 1, 2, // TL -> TR -> BL
					2, 1, 3  // BL -> TR -> BR
				};

				// 스프라이트 정점 버퍼 초기화
				HRESULT hr = spriteIndexBuffer->Initialize(device, indices.data(), static_cast<UINT>(indices.size()));
				if (FAILED(hr))
				{
					CommonUtility::SafeDelete(spriteIndexBuffer);
					CommonUtility::Log::ErrorMessage(hr, Message::Error::INITIALIZE_SPRITE_INDEX_BUFFER_FAILURE);
					return nullptr;
				}

				return spriteIndexBuffer;
			}
		} // Factory
	} // Graphics
} // Engine
