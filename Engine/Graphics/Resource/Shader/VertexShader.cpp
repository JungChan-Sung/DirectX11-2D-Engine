#include "VertexShader.h"
#include <d3dcompiler.h>
// std::move
#include <utility>
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Utility/StringUtility.h"
#include "CommonUtility/Log/Log.h"
#include "Engine/Config/ResourceConfig.h"
#include "Engine/Message/Error/ResourceErrorMessage.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			VertexShader::VertexShader() : vertexShader(nullptr),
				inputLayout(nullptr), constantBuffer(nullptr)
			{
			}

			VertexShader::VertexShader(VertexShader&& moveVertexShader) noexcept
				: vertexShader(std::exchange(moveVertexShader.vertexShader, nullptr)),
				inputLayout(std::exchange(moveVertexShader.inputLayout, nullptr)),
				constantBuffer(std::exchange(moveVertexShader.constantBuffer, nullptr))
			{
			}

			VertexShader::~VertexShader()
			{
				Release();
			}

			VertexShader& VertexShader::operator=(VertexShader&& rhs) noexcept
			{
				if (this != &rhs)
				{
					vertexShader = std::exchange(rhs.vertexShader, nullptr);
					inputLayout = std::exchange(rhs.inputLayout, nullptr);
					constantBuffer = std::exchange(rhs.constantBuffer, nullptr);
				}

				return *this;
			}

			bool VertexShader::Initialize(ID3D11Device* device, const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC* inputLayoutDescription, UINT size, BaseConstantBuffer* vertexShaderConstantBuffer)
			{
				constantBuffer = vertexShaderConstantBuffer;

				// ID3D10Blob : Binary Large Object 약자이며 바리너리 데이터를 담을 수 있는 버퍼 역할을 하는 인터페이스
				Microsoft::WRL::ComPtr<ID3D10Blob> errorBuffer, vertexShaderBuffer;
				// 정점 셰이더 컴파일
				HRESULT hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr,
					ResourceConfig::Instance()->ShaderMainFunction().c_str(), ResourceConfig::Instance()->VertexShaderVersion().c_str(),
					D3D10_SHADER_ENABLE_STRICTNESS, 0, vertexShaderBuffer.GetAddressOf(), errorBuffer.GetAddressOf());
				if (FAILED(hr))
				{
					std::wstring message = Message::Error::VERTEX_SHADER_COMPILE_FAILURE;
					if (!errorBuffer.Get()) // errorBuffer == nullptr
						message += fileName;
					else
						message += CommonUtility::StringUtility::ConvertToWideString((char*)errorBuffer->GetBufferPointer());
					CommonUtility::Log::ErrorMessage(hr, message);
					return false;
				}

				// 정점 셰이더 생성
				hr = device->CreateVertexShader(vertexShaderBuffer.Get()->GetBufferPointer(), vertexShaderBuffer.Get()->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
				if (FAILED(hr))
				{
					std::wstring message = Message::Error::CREATE_VERTEX_SHADER_FAILURE;
					message += fileName;
					CommonUtility::Log::ErrorMessage(hr, message);
					return false;
				}

				// 입력 레이아웃 생성
				hr = device->CreateInputLayout(inputLayoutDescription, size, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), inputLayout.GetAddressOf());
				if (FAILED(hr))
				{
					CommonUtility::Log::ErrorMessage(hr, Message::Error::CREATE_INPUT_LAYOUT_FAILURE);
					return false;
				}

				return true;
			}

			void VertexShader::Release()
			{
				CommonUtility::SafeComPtrRelease(inputLayout);
				CommonUtility::SafeComPtrRelease(vertexShader);

				constantBuffer = nullptr;
			}

			ID3D11VertexShader* VertexShader::Get() const
			{
				return vertexShader.Get();
			}

			ID3D11VertexShader* const* VertexShader::GetAddressOf() const
			{
				return vertexShader.GetAddressOf();
			}

			ID3D11InputLayout* VertexShader::GetInputLayout() const
			{
				return inputLayout.Get();
			}

			BaseConstantBuffer* VertexShader::GetVertexShaderConstantBuffer() const
			{
				return constantBuffer;
			}
		} // Resource
	} // Graphics
} // Engine
