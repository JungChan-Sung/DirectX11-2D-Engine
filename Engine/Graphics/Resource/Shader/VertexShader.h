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
			 * @brief 정점 셰이더 클래스
			 * @date 2024-10-16
			 * @class
			 * 정점 셰이더 컴파일 및 리소스 클래스
			 */
			class VertexShader
			{
			private: // ==== [리소스] ====
				Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;	// DirectX11 정점 셰이더 포인터
				Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;		// DirectX11 입력 레이아웃 포인터
				BaseConstantBuffer* constantBuffer;							// 정점 셰이더 상수 버퍼 참조용 포인터

			public: // ==== [생성자 및 소멸자] ====
				VertexShader();
				/** @param moveVertexShader : 이동 생성할 정점 셰이더 */
				VertexShader(VertexShader&& moveVertexShader) noexcept;
				~VertexShader();

			private: // ==== [생성자] ====
				/** @deprecated */
				VertexShader(const VertexShader& copyVertexShader) = delete;

			public: // ==== [연산자 재정의] ====
				VertexShader& operator=(VertexShader&& rhs) noexcept;

			private: // ==== [연산자 재정의] ====
				/** @deprecated */
				VertexShader& operator=(const VertexShader& rhs) = delete;

			public: // ==== [Setup] ====
				/**
				 * @brief 정점 셰이더 초기화
				 * @details
				 * 정점 셰이더 파일 컴파일 후
				 * 정점 셰이더와 입력 레이아웃 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param fileName : 정점 셰이더 파일 이름
				 * @param inputLayoutDescription : 입력 레이아웃 구성 참조용 포인터
				 * @param size : 입력 레이아웃 구성 요소 개수
				 * @param vertexShaderConstantBuffer : 정점 셰이더 상수 버퍼 참조용 포인터
				 * @return 정점 셰이더 초기화 성공 여부
				 */
				bool Initialize(ID3D11Device* device, const std::wstring& fileName, D3D11_INPUT_ELEMENT_DESC* inputLayoutDescription, UINT size, BaseConstantBuffer* vertexShaderConstantBuffer);
				/**
				 * @brief 정점 셰이더 자원 해제
				 * @details
				 * 정점 셰이더 자원 해제
				 * 입력 레이아웃 자원 해제
				 * 정점 셰이더 상수 버퍼 참조 해제
				 */
				void Release();

			public: // ==== [Get] ====
				/** @return 정점 셰이더 내부 포인터(참조용) */
				ID3D11VertexShader* Get() const;
				/** @return 정점 셰이더 내부 포인터 주소 값(생성 및 초기화용) */
				ID3D11VertexShader* const* GetAddressOf() const;

				/** @return 입력 레이아웃 참조용 포인터 */
				ID3D11InputLayout* GetInputLayout() const;
				/** @return 정점 셰이더 상수 버퍼 참조용 포인터 */
				BaseConstantBuffer* GetVertexShaderConstantBuffer() const;
			};
		} // Resource
	} // Graphics
} // Engine
