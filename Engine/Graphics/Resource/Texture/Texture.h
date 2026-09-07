#pragma once
#pragma comment(lib, "DirectXTex.lib")

#include <DirectXTex.h>
#include <wrl/client.h>
#include <string>
#include "Engine/Graphics/Common/Color.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			/**
			 * @brief 텍스처 클래스
			 * @date 2024-10-15
			 * @class
			 * DirectXTex 라이브러리를 사용하여 이미지 파일을 불러옴
			 * 텍스처는 생성시에 로드가 완료되어야함
			 */
			class Texture
			{
			private: // ==== [리소스] ====
				// ID3D11ShaderResourceView는 ID3D11Resource의 뷰이며
				// 셰이더에서 리소스의 데이터를 참조할 수 있도록함
				Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;	// 텍스처 셰이더 리소스 뷰 포인터
				ID3D11SamplerState* samplerState;										// 샘플러 상태 참조용 포인터

			private: // ==== [텍스처 정보] ====
				UINT width;		// 텍스처 너비(픽셀)
				UINT height;	// 텍스처 높이(픽셀)

			public: // ==== [생성자 및 소멸자] ====
				/**
				 * @details
				 * 색상 값으로 텍스처를 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param color : 초기화할 텍스처 색상
				 * @param samplerState : 샘플러 상태 참조용 포인터
				 */
				Texture(ID3D11Device* device, const Common::Color& color, ID3D11SamplerState* samplerState);
				Texture(ID3D11Device* device, const Common::Color32Bit& color32Bit, ID3D11SamplerState* samplerState);
				/**
				 * @details
				 * 이미지 파일을 불러와서 텍스처를 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param fileName : 파일 경로
				 * @param format : 로드할 이미지 파일 형식
				 * @param samplerState : 샘플러 상태 참조용 포인터
				 */
				Texture(ID3D11Device* device, const std::string& fileName, DXGI_FORMAT format, ID3D11SamplerState* samplerState);
				Texture(ID3D11Device* device, const std::wstring& fileName, DXGI_FORMAT format, ID3D11SamplerState* samplerState);
				/** @param moveTexture : 이동 생성할 텍스처 */
				Texture(Texture&& moveTexture) noexcept;
				~Texture();

			private: // ==== [생성자] ====
				/** @deprecated */
				Texture() = delete;
				Texture(const Texture& copyTexture) = delete;

			public: // ==== [연산자 재정의] ====
				Texture& operator=(Texture&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				Texture& operator=(const Texture& rhs) = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 텍스처 자원 해제
				 * @details
				 * 생성된 리소스 자원 및 참조 해제
				 */
				void Release();

			public: // ==== [Get] ====
				/** @return 텍스처 리소스 뷰 내부 포인터(참조용) */
				ID3D11ShaderResourceView* GetTextureResourceView() const;
				/** @return 텍스처 리소스 뷰 내부 포인터 주소 값(생성 및 초기화용) */
				ID3D11ShaderResourceView* const* GetTextureResourceViewAddress() const;

				/** @return 텍스처 샘플러 상태 참조용 포인터 주소 값 */
				ID3D11SamplerState* const* GetSamplerState() const;

				/** @return 텍스처 너비(픽셀) */
				UINT GetWidth() const;
				/** @return 텍스처 높이(픽셀) */
				UINT GetHeight() const;

			private: // ==== [텍스처 리소스 생성] ====
				// ScratchImage : DirectX에서 이미지 데이터 처리 및 변환용 클래스
				// 이미지 로딩 및 텍스처 생성 작업을 해줌
				/**
				 * @brief 이미지 파일 로드
				 * @details
				 * 파일 확장자에 구분하여 ScratchImage에 이미지 파일 로드
				 * @param fileName : 로드할 파일 이름
				 * @param scratchImage : 반환용 DirectX 이미지
				 * @return 이미지 파일 로드 성공 여부
				 */
				bool LoadImageFile(const std::wstring& fileName, DirectX::ScratchImage& scratchImage);
				/**
				 * @brief 색상 값으로 텍스처 생성
				 * @details
				 * 1X1 픽셀 크기의 단일색 텍스처 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param color : 생성할 색상 값
				 */
				void CreateColorTexture(ID3D11Device* device, const Common::Color& color);
				void CreateColorTexture(ID3D11Device* device, const Common::Color32Bit& color32Bit);
				/**
				 * @brief 텍스처 리소스 뷰 생성
				 * @date 2024-10-13
				 * @param device : DirectX 장치11 참조용 포인터
				 * @param textureDescription : 텍스처 포맷
				 * @param data : 텍스처 리소스 데이터
				 */
				void CretateTextureResourceView(ID3D11Device* device, D3D11_TEXTURE2D_DESC& textureDescription, D3D11_SUBRESOURCE_DATA& data);
			};
		} // Resource
	} // Graphics
} // Engine
