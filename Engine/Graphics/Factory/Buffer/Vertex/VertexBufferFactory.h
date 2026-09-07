#pragma once

#include "Engine/Graphics/Resource/Buffer/VertexBuffer.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 정점 버퍼 팩토리 추상화 클래스
			 * @date 2024-10-18
			 * @class
			 * 팩토리 메서드 패턴을 사용하여 정점 버퍼를 생성하는 클래스
			 */
			class VertexBufferFactory abstract
			{
			public: // ==== [정점 버퍼 생성] ====
				/**
				 * @brief 정점 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 생성된 정점 버퍼 포인터
				 */
				virtual Resource::VertexBuffer* Create(ID3D11Device* device) abstract;
			};
		} // Factory
	} // Graphics
} // Engine
