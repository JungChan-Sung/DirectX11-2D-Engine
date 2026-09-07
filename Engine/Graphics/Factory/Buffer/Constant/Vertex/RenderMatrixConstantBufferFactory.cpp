#include "RenderMatrixConstantBufferFactory.h"
#include "CommonUtility/Utility/SafeDelete.h"
#include "Engine/Graphics/Resource/Buffer/ConstantBuffer.h"
#include "Engine/Graphics/Common/RenderMatrix.h"
#include "Engine/Message/Error/ResourceFactoryErrorMessage.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			Resource::BaseConstantBuffer* RenderMatrixConstantBufferFactory::Create(ID3D11Device* device)
			{
				// 렌더 행렬 상수 버퍼 생성
				Resource::ConstantBuffer<Common::RenderMatrix>* renderMatrixConstantBuffer = new Resource::ConstantBuffer<Common::RenderMatrix>;
				if (!renderMatrixConstantBuffer) // renderMatrixConstantBuffer == nullptr
				{
					CommonUtility::Log::ErrorMessage(Message::Error::CREATE_RENDER_MATRIX_CONSTANT_BUFFER_FAILURE);
					return nullptr;
				}

				// 렌더 행렬 상수 버퍼 초기화
				HRESULT hr = renderMatrixConstantBuffer->Initialize(device);
				if (FAILED(hr))
				{
					CommonUtility::SafeDelete(renderMatrixConstantBuffer);
					CommonUtility::Log::ErrorMessage(hr, Message::Error::INITIALIZE_RENDER_MATRIX_CONSTANT_BUFFER_FAILURE);
					return nullptr;
				}

				return renderMatrixConstantBuffer;
			}
		} // Factory
	} // Graphics
} // Engine