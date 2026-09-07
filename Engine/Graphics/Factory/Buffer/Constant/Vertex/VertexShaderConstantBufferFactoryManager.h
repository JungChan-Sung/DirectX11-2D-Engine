#pragma once

#include <unordered_map>
#include "Engine/Graphics/Type/BufferType.h"
#include "VertexShaderConstantBufferFactoryHeaders.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 정점 셰이더 상수 버퍼 팩토리 매니저 클래스
			 * @date 2024-10-18
			 * @class
			 * 정점 셰이더 상수 버퍼 팩토리들을 관리하는 클래스
			 */
			class VertexShaderConstantBufferFactoryManager
			{
			private: // ==== [선언] ====
				// <정점 셰이더 상수 버퍼 타입, 상수 버퍼 팩토리 포인터>
				using VertexShaderConstantBufferFactoryTable = std::unordered_map<Type::VertexShaderConstantBufferType, ConstantBufferFactory*>;

			private: // ==== 구성 요소] ====
				VertexShaderConstantBufferFactoryTable vertexShaderConstantBufferFactories; // 정점 셰이더 상수 버퍼 팩토리 테이블

			public: // ==== [생성자 및 소멸자] ====
				VertexShaderConstantBufferFactoryManager();
				~VertexShaderConstantBufferFactoryManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 정점 셰이더 상수 버퍼 팩토리 매니저 자원 해제
				 * @details
				 * 정점 셰이더 상수 버퍼 팩토리 테이블 자원 해제
				 */
				void Release();

			public: // ==== [상수 버퍼 생성] ====
				/**
				 * @brief 상수 버퍼 생성
				 * @details
				 * VertexShaderConstantBufferType에 맞는 상수 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param vertexShaderConstantBufferType : 정점 셰이더 상수 버퍼 타입
				 * @return 생성된 상수 버퍼 포인터
				 */
				Resource::BaseConstantBuffer* CreateConstantBuffer(ID3D11Device* device, Type::VertexShaderConstantBufferType vertexShaderConstantBufferType);
			};
		} // Factory
	} // Graphics
} // Engine
