#include "Texture.h"
#include <DirectXColors.h>
#include "CommonUtility/Utility/SafeDelete.h"
#include "CommonUtility/Utility/StringUtility.h"
#include "CommonUtility/Exception/COMException.h"
#include "Engine/Message/Error/ResourceErrorMessage.h"
#include "Engine/Graphics/Constant/ImageExtensions.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			Texture::Texture(ID3D11Device* device, const Common::Color& color, ID3D11SamplerState* samplerState) : width(1u), height(1u), samplerState(samplerState)
			{
				CreateColorTexture(device, color);
			}

			Texture::Texture(ID3D11Device* device, const Common::Color32Bit& color32Bit, ID3D11SamplerState* samplerState) : width(1u), height(1u), samplerState(samplerState)
			{
				CreateColorTexture(device, color32Bit);
			}

			Texture::Texture(ID3D11Device* device, const std::string& fileName, DXGI_FORMAT format, ID3D11SamplerState* samplerState)
				: Texture(device, CommonUtility::StringUtility::ConvertToWideString(fileName), format, samplerState)
			{
			}

			Texture::Texture(ID3D11Device* device, const std::wstring& fileName, DXGI_FORMAT format, ID3D11SamplerState* samplerState) : samplerState(samplerState)
			{
				DirectX::ScratchImage image;
				if (LoadImageFile(fileName, image))
				{
					// 이미지 포맷 변경
					image.OverrideFormat(format);

					// 이미지 정보 저장
					DirectX::TexMetadata metaDate = image.GetMetadata();
					width = static_cast<UINT>(metaDate.width);
					height = static_cast<UINT>(metaDate.height);

					// 텍스처 셰이더 리소스 뷰 생성
					HRESULT hr = CreateShaderResourceView(device, image.GetImages(), image.GetImageCount(), image.GetMetadata(), textureResourceView.GetAddressOf());
					CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_SHADER_RESOURCE_VIEW_FAILURE);
					return;
				}

				CreateColorTexture(device, Common::Color(DirectX::Colors::Magenta));
			}

			Texture::Texture(Texture&& moveTexture) noexcept : textureResourceView(std::exchange(moveTexture.textureResourceView, nullptr)),
				samplerState(std::exchange(moveTexture.samplerState, nullptr)),
				width(std::exchange(moveTexture.width, 1u)), height(std::exchange(moveTexture.height, 1u))
			{
			}

			Texture::~Texture()
			{
				Release();
			}

			Texture& Texture::operator=(Texture&& rhs) noexcept
			{
				if (this != &rhs)
				{
					textureResourceView = std::exchange(rhs.textureResourceView, nullptr);
					samplerState = std::exchange(rhs.samplerState, nullptr);
					width = std::exchange(rhs.width, 1u);
					height = std::exchange(rhs.height, 1u);
				}

				return *this;
			}

			void Texture::Release()
			{
				CommonUtility::SafeComPtrRelease(textureResourceView);

				samplerState = nullptr;
			}

			ID3D11ShaderResourceView* Texture::GetTextureResourceView() const
			{
				return textureResourceView.Get();
			}

			ID3D11ShaderResourceView* const* Texture::GetTextureResourceViewAddress() const
			{
				return textureResourceView.GetAddressOf();
			}

			ID3D11SamplerState* const* Texture::GetSamplerState() const
			{
				return &samplerState;
			}

			UINT Texture::GetWidth() const
			{
				return width;
			}

			UINT Texture::GetHeight() const
			{
				return height;
			}

			bool Texture::LoadImageFile(const std::wstring& fileName, DirectX::ScratchImage& scratchImage)
			{
				HRESULT hr = E_FAIL;
				std::string extension = CommonUtility::StringUtility::GetFileExtension(fileName);

				if (extension == Constant::DDS_EXTENSION)
					hr = DirectX::LoadFromDDSFile(fileName.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, nullptr, scratchImage);
				else if (extension == Constant::TGA_EXTENSION)
					hr = DirectX::LoadFromTGAFile(fileName.c_str(), nullptr, scratchImage);
				else
					hr = DirectX::LoadFromWICFile(fileName.c_str(), DirectX::WIC_FLAGS::WIC_FLAGS_NONE, nullptr, scratchImage);

				if (FAILED(hr))
					return false;

				return true;
			}

			void Texture::CreateColorTexture(ID3D11Device* device, const Common::Color& color)
			{
				// 색상 텍스처 포맷 설정(1 x 1 크기)
				CD3D11_TEXTURE2D_DESC textureDescription(DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, 1u, 1u);

				// 색상 리소스 준비
				D3D11_SUBRESOURCE_DATA data;
				std::memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
				data.pSysMem = &color;
				data.SysMemPitch = sizeof(Common::Color);

				// 텍스처 생성
				CretateTextureResourceView(device, textureDescription, data);
			}

			void Texture::CreateColorTexture(ID3D11Device* device, const Common::Color32Bit& color32Bit)
			{
				// 색상 텍스처 포맷 설정(1 x 1 크기)
				CD3D11_TEXTURE2D_DESC textureDescription(DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, 1u, 1u);

				// 색상 리소스 준비
				D3D11_SUBRESOURCE_DATA data;
				std::memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));
				data.pSysMem = &color32Bit;
				data.SysMemPitch = sizeof(Common::Color32Bit);

				// 텍스처 생성
				CretateTextureResourceView(device, textureDescription, data);
			}

			void Texture::CretateTextureResourceView(ID3D11Device* device, D3D11_TEXTURE2D_DESC& textureDescription, D3D11_SUBRESOURCE_DATA& data)
			{
				// 텍스처 생성
				ID3D11Texture2D* texture2D = nullptr;
				HRESULT hr = device->CreateTexture2D(&textureDescription, &data, &texture2D);
				CommonUtility::ComErrorFailed(hr, Message::Error::INITIALIZE_TEXTURE_COLOR_DATA_FAILURE);

				if (texture2D) // texture2D != nullptr
				{
					// ID3D11Resource는 GPU의 리소스를 나타내는 기본 인터페이스이며
					// 텍스처, 버퍼와 같은 데이터를 저장하는 개체
					ID3D11Resource* texture = static_cast<ID3D11Resource*>(texture2D);
					CD3D11_SHADER_RESOURCE_VIEW_DESC shaderRsourceViewDesc(D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D, textureDescription.Format);
					// 셰이더 리소스 뷰 생성
					hr = device->CreateShaderResourceView(texture, &shaderRsourceViewDesc, textureResourceView.GetAddressOf());
					CommonUtility::ComErrorFailed(hr, Message::Error::CREATE_SHADER_RESOURCE_VIEW_TEXTURE_COLOR_DATA_FAILURE);
				}
			}
		} // Resource
	} // Graphics
} // Engine
