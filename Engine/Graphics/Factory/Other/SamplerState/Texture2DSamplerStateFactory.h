#pragma once

#include "SamplerStateFactory.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 2D 텍스처 샘플러 상태 팩토리 클래스 : 샘플러 상태 팩토리 상속
			 * @date 2024-10-19
			 * @class
			 * 2D 텍스처용 샘플러 상태를 생성하는 팩토리 클래스
			 */
			class Texture2DSamplerStateFactory : public SamplerStateFactory
			{
			public: // ==== [샘플러 상태 생성(샘플러 상태 팩토리) 재정의] ====
				/**
				 * @brief 샘플러 상태 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @return 생성된 샘플러 상태 포인터
				 */
				virtual ID3D11SamplerState* Create(ID3D11Device* device) override;
			};
		} // Factory
	} // Graphics
} // Engine