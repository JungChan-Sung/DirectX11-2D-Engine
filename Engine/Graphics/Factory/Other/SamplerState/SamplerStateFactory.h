#pragma once

#include <d3d11.h>

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 샘플러 상태 팩토리 추상화 클래스
			 * @date 2024-10-19
			 * @class
			 * 팩토리 메서드 패턴을 사용하여 샘플러 상태를 생성하는 클래스
			 */
			class SamplerStateFactory abstract
			{
			public: // ==== [샘플러 상태 생성] ====
				/**
				 * @brief 샘플러 상태 생성
				 * @param device : DirectX11 장치 참조용 포인터
			 	 * @return 생성된 샘플러 상태 포인터
				 */
				virtual ID3D11SamplerState* Create(ID3D11Device* device) abstract;
			};
		} // Factory
	} // Graphics
} // Engine
