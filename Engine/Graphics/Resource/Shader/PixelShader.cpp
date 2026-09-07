#include "PixelShader.h"
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
				PixelShader::PixelShader() : pixelShader(nullptr), constantBuffer(nullptr)
				{
				}

				PixelShader::PixelShader(PixelShader&& movePixelShader) noexcept
					: pixelShader(std::exchange(movePixelShader.pixelShader, nullptr)),
					constantBuffer(std::exchange(movePixelShader.constantBuffer, nullptr))
				{
				}

				PixelShader::~PixelShader()
				{
					Release();
				}

				PixelShader& PixelShader::operator=(PixelShader&& rhs) noexcept
				{
					if (this != &rhs)
					{
						pixelShader = std::exchange(rhs.pixelShader, nullptr);
						constantBuffer = std::exchange(rhs.constantBuffer, nullptr);
					}

					return *this;
				}

				bool PixelShader::Initialize(ID3D11Device* device, const std::wstring& fileName, BaseConstantBuffer* pixelShaderConstantBuffer)
				{
					constantBuffer = pixelShaderConstantBuffer;

					Microsoft::WRL::ComPtr<ID3D10Blob> errorBuffer, pixelShaderBuffer;
					// ÇÈ¼¿ ¼ÎÀÌ´õ ÄÄÆÄÀÏ
					HRESULT hr = D3DCompileFromFile(fileName.c_str(), nullptr, nullptr,
						ResourceConfig::Instance()->ShaderMainFunction().c_str(), ResourceConfig::Instance()->PixelShaderVersion().c_str(),
						D3D10_SHADER_ENABLE_STRICTNESS, 0u, pixelShaderBuffer.GetAddressOf(), errorBuffer.GetAddressOf());
					if (FAILED(hr))
					{
						std::wstring message = Message::Error::PIXEL_SHADER_COMPILE_FAILURE;
						if (!errorBuffer.Get()) // errorBuffer == nullptr
							message += fileName;
						else
							message += CommonUtility::StringUtility::ConvertToWideString((char*)errorBuffer->GetBufferPointer());
						CommonUtility::Log::ErrorMessage(hr, message);
						return false;
					}

					// ÇÈ¼¿ ¼ÎÀÌ´õ »ý¼º
					hr = device->CreatePixelShader(pixelShaderBuffer.Get()->GetBufferPointer(), pixelShaderBuffer.Get()->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
					if (FAILED(hr))
					{
						std::wstring message = Message::Error::CREATE_PIXEL_SHADER_FAILURE;
						message += fileName;
						CommonUtility::Log::ErrorMessage(hr, message);
						return false;
					}

					return true;
				}

				void PixelShader::Release()
				{
					CommonUtility::SafeComPtrRelease(pixelShader);

					constantBuffer = nullptr;
				}

				ID3D11PixelShader* PixelShader::Get() const
				{
					return pixelShader.Get();
				}

				ID3D11PixelShader* const* PixelShader::GetAddressOf() const
				{
					return pixelShader.GetAddressOf();
				}

				BaseConstantBuffer* PixelShader::GetPixelShaderConstantBuffer() const
				{
					return constantBuffer;
				}
		} // Resource
	} // Graphics
} // Engine
