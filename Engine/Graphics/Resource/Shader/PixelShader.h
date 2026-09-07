#pragma once
#pragma comment(lib, "d3dcompiler.lib")

#include <wrl/client.h>
#include <d3d11.h>
#include <string>
#include "Engine/Graphics/Resource/Buffer/BaseConstantBuffer.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Resource
		{
			/**
			 * @brief 픽셀 셰이더 클래스
			 * @date 2024-10-16
			 * @class
			 * 픽셀 셰이더 컴파일 및 리소스 클래스
			 */
			class PixelShader
			{
			private: // ==== [리소스] ====
				Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;	// DirectX11 픽셀 셰이더 포인터
				BaseConstantBuffer* constantBuffer;						// 픽셀 셰이더 상수 버퍼 참조용 포인터

			public: // ==== [생성자 및 소멸자] ====
				PixelShader();
				/** @param movePixelShader : 이동 생성할 픽셀 셰이더 */
				PixelShader(PixelShader&& movePixelShader) noexcept;
				~PixelShader();

			private: // ==== [생성자] ====
				/** @deprecated */
				PixelShader(const PixelShader& copyPixelShader) = delete;

			public: // ==== [연산자 재정의] ====
				PixelShader& operator=(PixelShader&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				PixelShader& operator=(const PixelShader& rhs) = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 픽셀 셰이더 초기화
				 * @details
				 *
				 * @param device : DirectX11 장치 포인터
				 * @param fileName : 픽셀 셰이더 파일 이름
				 * @param pixelShaderConstantBuffer : 픽셀 셰이더 상수 버퍼 참조용 포인터
				 * @return 픽셀 셰이더 초기화 성공 여부
				 */
				bool Initialize(ID3D11Device* device, const std::wstring& fileName, BaseConstantBuffer* pixelShaderConstantBuffer);
				/**
				 * @brief 픽셀 셰이더 자원 해제
				 * @details
				 * 픽셀 셰이더 자원 해제 및 픽셀 셰이더 상수 버퍼 참조 해제
				 */
				void Release();

			public: // ==== [Get] ====
				/** @return 픽셀 셰이더 내부 포인터(참조용) */
				ID3D11PixelShader* Get() const;
				/** @return 픽셀 셰이더 내부 포인터 주소 값(생성 및 초기화용) */
				ID3D11PixelShader* const* GetAddressOf() const;

				/** @return 픽셀 셰이더 상수 버퍼 참조용 포인터 */
				BaseConstantBuffer* GetPixelShaderConstantBuffer() const;
			};
		} // Resource
	} // Graphics
} // Engine
