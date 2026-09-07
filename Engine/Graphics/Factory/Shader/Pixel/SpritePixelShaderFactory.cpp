#include "SpritePixelShaderFactory.h"
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/ResourceFactoryErrorMessage.h"
#include "Engine/Graphics/Constant/PixelShaderConstants.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			Resource::PixelShader* SpritePixelShaderFactory::Create(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup)
			{
				// 스프라이트 픽셀 셰이더 생성
				Resource::PixelShader* spritePixelShader = new Resource::PixelShader;
				if (!spritePixelShader) // spritePixelShader == nullptr
				{
					CommonUtility::Log::ErrorMessage(Message::Error::CREATE_SPRITE_PIXEL_SHADER_FAILURE);
					return nullptr;
				}

				// 스프라이트 픽셀 셰이더 초기화
				if (!spritePixelShader->Initialize(device,
					CommonUtility::StringUtility::ConvertToWideString(shaderDataTableGroup[Constant::PIXEL_SHADER_GROUP_NAME][Constant::SPRITE_PIXEL_SHADER_NAME]->path),
					nullptr)) // spritePixelShader->Initialize == false
				{
					CommonUtility::SafeDelete(spritePixelShader);
					CommonUtility::Log::ErrorMessage(Message::Error::INITIALIZE_SPRITE_PIXEL_SHADER_FAILURE);
				}

				return spritePixelShader;
			}
		} // Factory
	} // Graphics
} // Engine