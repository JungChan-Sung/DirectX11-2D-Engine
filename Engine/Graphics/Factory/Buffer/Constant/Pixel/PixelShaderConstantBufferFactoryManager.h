#pragma once

#include <unordered_map>
#include "Engine/Graphics/Type/BufferType.h"
#include "PixelShaderConstantBufferFactoryHeaders.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 픽셀 셰이더 상수 버퍼 팩토리 매니저 클래스
			 * @date 2024-10-18
			 * @class
			 * 픽셀 셰이더 상수 버퍼 팩토리들을 관리하는 클래스
			 */
			class PixelShaderConstantBufferFactoryManager
			{
			private: // ==== [픽셀 셰이더 상수 버퍼 팩토리 매니저 선언] ====
				// <픽셀 셰이더 상수 버퍼 타입, 상수 버퍼 팩토리 포인터>
				using PixelShaderConstantBufferFactoryTable = std::unordered_map<Type::PixelShaderConstantBufferType, ConstantBufferFactory*>;

			private: // ==== [픽셀 셰이더 상수 버퍼 팩토리 매니저 구성 요소] ====
				PixelShaderConstantBufferFactoryTable pixelShaderConstantBufferFactories; // 픽셀 셰이더 상수 버퍼 팩토리 테이블

			public: // ==== [생성자 및 소멸자] ====
				PixelShaderConstantBufferFactoryManager();
				~PixelShaderConstantBufferFactoryManager();

			public: // ==== [픽셀 셰이더 상수 버퍼 팩토리 매니저 기본] ====
				/**
				 * @brief 픽셀 셰이더 상수 버퍼 팩토리 매니저 자원 해제
				 * @details
				 * 픽셀 셰이더 상수 버퍼 팩토리 테이블 자원 해제
				 */
				void Release();

			public: // ==== [상수 버퍼 생성] ====
				/**
				 * @brief 상수 버퍼 생성
				 * @details
				 * VertexShaderConstantBufferType에 맞는 상수 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param pixelShaderConstantBufferType : 픽셀 셰이더 상수 버퍼 타입
				 * @return 생성된 상수 버퍼 포인터
				 */
				Resource::BaseConstantBuffer* CreateConstantBuffer(ID3D11Device* device, Type::PixelShaderConstantBufferType pixelShaderConstantBufferType);
			};
		} // Factory
	} // Graphics
} // Engine