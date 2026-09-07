#include "VertexShaderConstantBufferFactoryManager.h"
#include "CommonUtility/Utility/SafeDelete.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			VertexShaderConstantBufferFactoryManager::VertexShaderConstantBufferFactoryManager()
			{
				vertexShaderConstantBufferFactories[Type::VertexShaderConstantBufferType::RENDER_MATRIX] = new RenderMatrixConstantBufferFactory;
			}

			VertexShaderConstantBufferFactoryManager::~VertexShaderConstantBufferFactoryManager()
			{
				Release();
			}

			void VertexShaderConstantBufferFactoryManager::Release()
			{
				CommonUtility::SafeMapContainerRelease(vertexShaderConstantBufferFactories);
			}

			Resource::BaseConstantBuffer* VertexShaderConstantBufferFactoryManager::CreateConstantBuffer(ID3D11Device* device, Type::VertexShaderConstantBufferType vertexShaderConstantBufferType)
			{
				if (vertexShaderConstantBufferFactories.find(vertexShaderConstantBufferType) != vertexShaderConstantBufferFactories.end())
					return vertexShaderConstantBufferFactories[vertexShaderConstantBufferType]->Create(device);

				return nullptr;
			}
		} // Factory
	} // Graphics
} // Engine