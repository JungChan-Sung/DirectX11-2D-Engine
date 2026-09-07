#include "ShaderFactoryManager.h"
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			ShaderFactoryManager::ShaderFactoryManager()
			{
				vertexShaderFactories[Type::ShaderType::SPRITE] = new SpriteVertexShaderFactory;
				pixelShaderFactories[Type::ShaderType::SPRITE] = new SpritePixelShaderFactory;
			}

			ShaderFactoryManager::~ShaderFactoryManager()
			{
				Release();
			}

			void ShaderFactoryManager::Release()
			{
				CommonUtility::SafeMapContainerRelease(pixelShaderFactories);
				CommonUtility::SafeMapContainerRelease(vertexShaderFactories);
			}

			Resource::VertexShader* ShaderFactoryManager::CreateVertexShader(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup, Type::ShaderType shaderType)
			{
				if (vertexShaderFactories.find(shaderType) != vertexShaderFactories.end())
					return vertexShaderFactories[shaderType]->Create(device, bufferManager, shaderDataTableGroup);

				return nullptr;
			}

			Resource::PixelShader* ShaderFactoryManager::CreatePixelShader(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup, Type::ShaderType shaderType)
			{
				if (pixelShaderFactories.find(shaderType) != pixelShaderFactories.end())
					return pixelShaderFactories[shaderType]->Create(device, bufferManager, shaderDataTableGroup);

				return nullptr;
			}
		} // Factory
	} // Graphics
} // Engine
