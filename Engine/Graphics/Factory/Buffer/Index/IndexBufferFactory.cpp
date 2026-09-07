#include "IndexBufferFactoryManager.h"
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			IndexBufferFactoryManager::IndexBufferFactoryManager()
			{
				indexBufferFactories[Type::RenderResourceBufferType::SPRITE] = new SpriteIndexBufferFactory;
			}

			IndexBufferFactoryManager::~IndexBufferFactoryManager()
			{
				Release();
			}

			void IndexBufferFactoryManager::Release()
			{
				CommonUtility::SafeMapContainerRelease(indexBufferFactories);
			}

			Resource::IndexBuffer* IndexBufferFactoryManager::CreateIndexBuffer(ID3D11Device* device, Type::RenderResourceBufferType renderResourceBufferType)
			{
				if (indexBufferFactories.find(renderResourceBufferType) != indexBufferFactories.end())
					return indexBufferFactories[renderResourceBufferType]->Create(device);

				return nullptr;
			}
		} // Factory
	} // Graphics
} // Engine
