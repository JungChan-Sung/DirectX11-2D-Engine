#include "SpriteVertexShaderFactory.h"
#include <vector>
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Message/Error/ResourceFactoryErrorMessage.h"
#include "Engine/Graphics/Constant/VertexShaderConstants.h"
#include "Engine/Graphics/Constant/InputLayoutConstants.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			Resource::VertexShader* SpriteVertexShaderFactory::Create(ID3D11Device* device, Manager::BufferManager* bufferManager, Data::ShaderDataGroupTable& shaderDataTableGroup)
			{
				// 입력 레이아웃은 정점을 구성하는 구조체(Vertex struct)와 같은 구조여야하며
				// 정점 셰이더의 입력구조와 일치 시켜야함
				// VertexTexture 구조체의 경우 위치, 색상, 텍스쳐 위치 순으로 배치되므로 다음과 같음
				std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayout =
				{
					{ Constant::POSITION, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ Constant::COLOR, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
					{ Constant::TEXCOORD, 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0 },
				};

				// 스프라이트 정점 셰이더 생성
				Resource::VertexShader* spriteVertexShader = new Resource::VertexShader;
				if (!spriteVertexShader) // spriteVertexShader == nullptr
				{
					CommonUtility::Log::ErrorMessage(Message::Error::CREATE_SPRITE_VERTEX_SHADER_FAILURE);
					return nullptr;
				}

				// 스프라이트 정점 셰이더 초기화
				if (!spriteVertexShader->Initialize(device,
					CommonUtility::StringUtility::ConvertToWideString(shaderDataTableGroup[Constant::VERTEX_SHADER_GROUP_NAME][Constant::SPRITE_VERTEX_SHADER_NAME]->path),
					inputLayout.data(), static_cast<UINT>(inputLayout.size()),
					bufferManager->GetVertexShaderConstantBuffer(Type::VertexShaderConstantBufferType::RENDER_MATRIX))) // spriteVertexShader->Initialize == false
				{
					CommonUtility::SafeDelete(spriteVertexShader);
					CommonUtility::Log::ErrorMessage(Message::Error::INITIALIZE_SPRITE_VERTEX_SHADER_FAILURE);
				}

				return spriteVertexShader;
			}
		} // Factory
	} // Graphics
} // Engine