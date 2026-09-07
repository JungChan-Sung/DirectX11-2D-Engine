#pragma once

#include "Engine/Graphics/Factory/Buffer/Constant/ConstantBufferFactory.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 폰트 색상 상수 버퍼 팩토리 클래스 : 상수 버퍼 팩토리 상속
			 * @date 2024-10-18
			 * @class
			 * 폰트 색상 상수 버퍼를 생성하는 팩토리 클래스
			 */
			class FontColorConstantBufferFactory : public ConstantBufferFactory
			{
			public: // ==== [폰트 색상 상수 버퍼 생성(상수 버퍼 팩토리) 재정의] ====
				/**
				 * @brief 상수 버퍼 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 생성된 상수 버퍼 포인터
				 */
				virtual Resource::BaseConstantBuffer* Create(ID3D11Device* device) override;
			};
		} // Factory
	} // Graphics
} // Engine
