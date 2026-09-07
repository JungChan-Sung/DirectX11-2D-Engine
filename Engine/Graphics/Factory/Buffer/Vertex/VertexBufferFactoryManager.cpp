#include "VertexBufferFactoryManager.h"
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			VertexBufferFactoryManager::VertexBufferFactoryManager()
			{
				vertexBufferFactories[Type::RenderResourceBufferType::SPRITE] = new SpriteVertexBufferFactory;
			}

			VertexBufferFactoryManager::~VertexBufferFactoryManager()
			{
				Release();
			}

			void VertexBufferFactoryManager::Release()
			{
				CommonUtility::SafeMapContainerRelease(vertexBufferFactories);
			}

			Resource::VertexBuffer* VertexBufferFactoryManager::CreateVertexBuffer(ID3D11Device* device, Type::RenderResourceBufferType renderResourceBufferType)
			{
				if (vertexBufferFactories.find(renderResourceBufferType) != vertexBufferFactories.end())
					return vertexBufferFactories[renderResourceBufferType]->Create(device);

				return nullptr;
			}
		} // Factory
	} // Graphics
} // Engine
