#include "PixelShaderConstantBufferFactoryManager.h"
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			PixelShaderConstantBufferFactoryManager::PixelShaderConstantBufferFactoryManager()
			{
				pixelShaderConstantBufferFactories[Type::PixelShaderConstantBufferType::FONT_COLOR] = new FontColorConstantBufferFactory;
			}

			PixelShaderConstantBufferFactoryManager::~PixelShaderConstantBufferFactoryManager()
			{
				Release();
			}

			void PixelShaderConstantBufferFactoryManager::Release()
			{
				CommonUtility::SafeMapContainerRelease(pixelShaderConstantBufferFactories);
			}

			Resource::BaseConstantBuffer* PixelShaderConstantBufferFactoryManager::CreateConstantBuffer(ID3D11Device* device, Type::PixelShaderConstantBufferType pixelShaderConstantBufferType)
			{
				if (pixelShaderConstantBufferFactories.find(pixelShaderConstantBufferType) != pixelShaderConstantBufferFactories.end())
					return pixelShaderConstantBufferFactories[pixelShaderConstantBufferType]->Create(device);

				return nullptr;
			}
		} // Factory
	} // Graphics
} // Engine