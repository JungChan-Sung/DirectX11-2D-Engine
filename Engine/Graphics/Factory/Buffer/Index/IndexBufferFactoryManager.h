#pragma once

#include <unordered_map>
#include "Engine/Graphics/Type/BufferType.h"
#include "IndexBufferFactoryHeaders.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 인덱스 버퍼 팩토리 매니저 클래스
			 * @date 2024-10-18
			 * @class
			 * 인덱스 버퍼 팩토리들을 관리하는 클래스
			 */
			class IndexBufferFactoryManager
			{
			private: // ==== [선언] ====
				// <렌더 리소스 버퍼 타입, 인덱스 버퍼 팩토리 포인터>
				using IndexBufferFactoryTable = std::unordered_map<Type::RenderResourceBufferType, IndexBufferFactory*>;

			private: // ==== [구성 요소] ====
				IndexBufferFactoryTable indexBufferFactories; // 인덱스 버퍼 팩토리 테이블

			public: // ==== [생성자 및 소멸자] ====
				IndexBufferFactoryManager();
				~IndexBufferFactoryManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 인덱스 버퍼 팩토리 매니저 자원 해제
				 * @details
				 * 인덱스 버퍼 팩토리 테이블 자원 해제
				 */
				void Release();

			public: // ==== [인덱스 버퍼 생성] ====
				/**
				 * @brief 인덱스 버퍼 생성
				 * @details
				 * RenderResourceBufferType에 맞는 인덱스 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param renderResourceBufferType : 렌더 리소스 버퍼 타입
				 * @return 생성된 인덱스 버퍼 포인터
				 */
				Resource::IndexBuffer* CreateIndexBuffer(ID3D11Device* device, Type::RenderResourceBufferType renderResourceBufferType);
			};
		} // Factory
	} // Graphics
} // Engine
