#include "BufferManager.h"
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Exception/COMException.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/ResourceManagerErrorMessage.h"
#include "Engine/Graphics/Factory/Buffer/Vertex/VertexBufferFactoryManager.h"
#include "Engine/Graphics/Factory/Buffer/Index/IndexBufferFactoryManager.h"
#include "Engine/Graphics/Factory/Buffer/Constant/Vertex/VertexShaderConstantBufferFactoryManager.h"
#include "Engine/Graphics/Factory/Buffer/Constant/Pixel/PixelShaderConstantBufferFactoryManager.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Manager
		{
			BufferManager::BufferManager()
			{
			}

			BufferManager::~BufferManager()
			{
				Release();
			}

			bool BufferManager::Initialize(ID3D11Device* device)
			{
				try
				{
					HRESULT hr = E_FAIL;

					// ==== 정점 버퍼 생성 ====
					hr = CreateVertexBuffers(device);
					CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_VERTEX_BUFFERS_FAILURE);
					// ========================

					// ==== 인덱스 버퍼 생성 ====
					hr = CreateIndexBuffers(device);
					CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_INDEX_BUFFERS_FAILURE);
					// ==========================

					// ==== 정점 셰이더 상수 버퍼 생성 ====
					hr = CreateVertexShaderConstantBuffers(device);
					CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_VERTEX_SHADER_CONSTANT_BUFFERS_FAILURE);
					// ====================================

					// ==== 픽셀 셰이더 상수 버퍼 생성 ====
					hr = CreatePixelShaderConstantBuffers(device);
					CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_PIXEL_SHADER_CONSTANT_BUFFERS_FAILURE);
					// ====================================
				}
				catch (CommonUtility::COMException& exception)
				{
					CommonUtility::Log::ErrorMessage(exception);
					return false;
				}

				return true;
			}

			void BufferManager::Release()
			{
				CommonUtility::SafeMapContainerRelease(pixelShaderConstantBuffers);
				CommonUtility::SafeMapContainerRelease(vertexShaderConstantBuffers);

				CommonUtility::SafeMapContainerRelease(indexBuffers);
				CommonUtility::SafeMapContainerRelease(vertexBuffers);
			}

			Resource::VertexBuffer* BufferManager::GetVertexBuffer(Type::RenderResourceBufferType renderResourceBufferType) const
			{
				return vertexBuffers.at(renderResourceBufferType);
			}

			Resource::IndexBuffer* BufferManager::GetIndexBuffer(Type::RenderResourceBufferType renderResourceBufferType) const
			{
				return indexBuffers.at(renderResourceBufferType);
			}

			Resource::BaseConstantBuffer* BufferManager::GetVertexShaderConstantBuffer(Type::VertexShaderConstantBufferType vertexShaderConstantBufferType) const
			{
				return vertexShaderConstantBuffers.at(vertexShaderConstantBufferType);
			}

			Resource::BaseConstantBuffer* BufferManager::GetPixelShaderConstantBuffer(Type::PixelShaderConstantBufferType pixelShaderConstantBufferType) const
			{
				return pixelShaderConstantBuffers.at(pixelShaderConstantBufferType);
			}

			HRESULT BufferManager::CreateVertexBuffers(ID3D11Device* device)
			{
				Type::RenderResourceBufferType renderResourceBufferType;
				Factory::VertexBufferFactoryManager vertexBufferFactoryManager;
				Resource::VertexBuffer* vertexBuffer;
				for (int i = 0; i < static_cast<int>(Type::RenderResourceBufferType::END); ++i)
				{
					renderResourceBufferType = static_cast<Type::RenderResourceBufferType>(i);
					vertexBuffer = vertexBufferFactoryManager.CreateVertexBuffer(device, renderResourceBufferType);
					
					if (!vertexBuffer) // vertexBuffer == nullptr
						return E_FAIL;

					vertexBuffers.insert(std::make_pair(renderResourceBufferType, vertexBuffer));
				}

				return S_OK;
			}

			HRESULT BufferManager::CreateIndexBuffers(ID3D11Device* device)
			{
				Type::RenderResourceBufferType renderResourceBufferType;
				Factory::IndexBufferFactoryManager indexBufferFactoryManager;
				Resource::IndexBuffer* indexBuffer;
				for (int i = 0; i < static_cast<int>(Type::RenderResourceBufferType::END); ++i)
				{
					renderResourceBufferType = static_cast<Type::RenderResourceBufferType>(i);
					indexBuffer = indexBufferFactoryManager.CreateIndexBuffer(device, renderResourceBufferType);

					if (!indexBuffer) // indexBuffer == nullptr
						return E_FAIL;

					indexBuffers.insert(std::make_pair(renderResourceBufferType, indexBuffer));
				}

				return S_OK;
			}

			HRESULT BufferManager::CreateVertexShaderConstantBuffers(ID3D11Device* device)
			{
				Type::VertexShaderConstantBufferType vertexShaderConstantBufferType;
				Factory::VertexShaderConstantBufferFactoryManager vertexShaderConstantBufferFactoryManager;
				Resource::BaseConstantBuffer* constantBuffer;
				for (int i = 0; i < static_cast<int>(Type::VertexShaderConstantBufferType::END); ++i)
				{
					vertexShaderConstantBufferType = static_cast<Type::VertexShaderConstantBufferType>(i);
					constantBuffer = vertexShaderConstantBufferFactoryManager.CreateConstantBuffer(device, vertexShaderConstantBufferType);

					if (!constantBuffer) // constantBuffer == nullptr
						return E_FAIL;

					vertexShaderConstantBuffers.insert(std::make_pair(vertexShaderConstantBufferType, constantBuffer));
				}

				return S_OK;
			}

			HRESULT BufferManager::CreatePixelShaderConstantBuffers(ID3D11Device* device)
			{
				Type::PixelShaderConstantBufferType pixelShaderConstantBufferType;
				Factory::PixelShaderConstantBufferFactoryManager pixelShaderConstantBufferfactoryManager;
				Resource::BaseConstantBuffer* constantBuffer;
				for (int i = 0; i < static_cast<int>(Type::PixelShaderConstantBufferType::END); ++i)
				{
					pixelShaderConstantBufferType = static_cast<Type::PixelShaderConstantBufferType>(i);
					constantBuffer = pixelShaderConstantBufferfactoryManager.CreateConstantBuffer(device, pixelShaderConstantBufferType);

					if (!constantBuffer) // constantBuffer == nullptr
						return E_FAIL;

					pixelShaderConstantBuffers.insert(std::make_pair(pixelShaderConstantBufferType, constantBuffer));
				}

				return S_OK;
			}
		} // Manager
	} // Graphics
} // Engine

