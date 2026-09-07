#pragma once

#include "VertexBufferFactory.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 스프라이트 정점 버퍼 팩토리 클래스 : 정점 버퍼 팩토리 상속
			 * @date 2024-10-18
			 * @class
			 * 스프라이트용 정점 버퍼를 생성하는 팩토리 클래스
			 */
			class SpriteVertexBufferFactory : public VertexBufferFactory
			{
			public: // ==== [정점 버퍼 생성(정점 버퍼 팩토리) 재정의] ====
				/**
				 * @brief 정점 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 생성된 정점 버퍼 포인터
				 */
				virtual Resource::VertexBuffer* Create(ID3D11Device* device) override;
			};
		} // Factory
	} // Graphics
} // Engine
