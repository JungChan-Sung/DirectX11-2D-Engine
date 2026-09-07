#include "FontColorConstantBufferFactory.h"
#include "CommonUtility/Utility/SafeDelete.h"
#include "Engine/Graphics/Resource/Buffer/ConstantBuffer.h"
#include "Engine/Graphics/Common/FontColor.h"
#include "Engine/Message/Error/ResourceFactoryErrorMessage.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			Resource::BaseConstantBuffer* FontColorConstantBufferFactory::Create(ID3D11Device* device)
			{
				// 폰트 색상 상수 버퍼 생성
				Resource::ConstantBuffer<Common::FontColor>* fontColorConstantBuffer = new Resource::ConstantBuffer<Common::FontColor>;
				if (!fontColorConstantBuffer) // fontColorConstantBuffer == nullptr
				{
					CommonUtility::Log::ErrorMessage(Message::Error::CREATE_FONT_COLOR_CONSTANT_BUFFER_FAILURE);
					return nullptr;
				}

				// 폰트 색상 상수 버퍼 초기화
				HRESULT hr = fontColorConstantBuffer->Initialize(device);
				if (FAILED(hr))
				{
					CommonUtility::SafeDelete(fontColorConstantBuffer);
					CommonUtility::Log::ErrorMessage(hr, Message::Error::INITIALIZE_FONT_COLOR_CONSTANT_BUFFER_FAILURE);
					return nullptr;
				}

				return fontColorConstantBuffer;
			}
		} // Factory
	} // Graphics
} // Engine
