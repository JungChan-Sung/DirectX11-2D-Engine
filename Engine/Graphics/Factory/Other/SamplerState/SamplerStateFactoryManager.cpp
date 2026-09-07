#include "SamplerStateFactoryManager.h"
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{


			SamplerStateFactoryManager::SamplerStateFactoryManager()
			{
				samplerStateFactories[Type::SamplerStateType::TEXTURE_2D] = new Texture2DSamplerStateFactory;
			}

			SamplerStateFactoryManager::~SamplerStateFactoryManager()
			{
				Release();
			}

			void SamplerStateFactoryManager::Release()
			{
				CommonUtility::SafeMapContainerRelease(samplerStateFactories);
			}

			ID3D11SamplerState* SamplerStateFactoryManager::CreateSamplerState(ID3D11Device* device, Type::SamplerStateType samplerStateType)
			{
				if (samplerStateFactories.find(samplerStateType) != samplerStateFactories.end())
					return samplerStateFactories[samplerStateType]->Create(device);

				return nullptr;
			}
		} // Factory
	} // Graphics
} // Engine
