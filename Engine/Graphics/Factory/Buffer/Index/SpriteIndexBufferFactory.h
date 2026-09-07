#pragma once

#include "IndexBufferFactory.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 스프라이트 인덱스 버퍼 팩토리 클래스 : 인덱스 버퍼 팩토리 상속
			 * @date 2024-10-18
			 * @class
			 * 스프라이트용 인덱스 버퍼를 생성하는 팩토리 클래스
			 */
			class SpriteIndexBufferFactory : public IndexBufferFactory
			{
			public: // ==== [인덱스 버퍼 생성(인덱스 버퍼 팩토리) 재정의] ====
				/**
				 * @brief 인덱스 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 생성된 인덱스 버퍼 포인터
				 */
				virtual Resource::IndexBuffer* Create(ID3D11Device* device) override;
			};
		} // Factory
	} // Graphics
} // Engine