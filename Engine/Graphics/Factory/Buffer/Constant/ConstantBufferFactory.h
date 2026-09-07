#pragma once

#include "Engine/Graphics/Resource/Buffer/BaseConstantBuffer.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 상수 버퍼 팩토리 추상화 클래스
			 * @date 2024-10-18
			 * @class
			 * 팩토리 메서드 패턴을 사용하여 상수 버퍼를 생성하는 클래스
			 */
			class ConstantBufferFactory abstract
			{
			public: // ==== [상수 버퍼 생성] ====
				/**
				 * @brief 상수 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 생성된 상수 버퍼 포인터
				 */
				virtual Resource::BaseConstantBuffer* Create(ID3D11Device* device) abstract;
			};
		} // Factory
	} // Graphics
} // Engine
