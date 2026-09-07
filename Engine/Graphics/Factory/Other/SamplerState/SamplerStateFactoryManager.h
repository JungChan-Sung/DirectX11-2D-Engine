#pragma once

#include <unordered_map>
#include "Engine/Graphics/Type/SamplerStateType.h"
#include "SamplerStateFactoryHeaders.h"

namespace Engine
{
	namespace Graphics
	{
		namespace Factory
		{
			/**
			 * @brief 샘플러 상태 팩토리 매니저 클래스
			 * @date 2024-10-19
			 * @class
			 * 샘플러 상태 팩토리들을 관리하는 클래스
			 */
			class SamplerStateFactoryManager
			{
			private: // ==== [선언] ====
				// <렌더 리소스 버퍼 타입, 샘플러 상태 팩토리 포인터>
				using SamplerStateFactoryTable = std::unordered_map<Type::SamplerStateType, SamplerStateFactory*>;

			private: // ==== [구성 요소] ====
				SamplerStateFactoryTable samplerStateFactories; // 샘플러 상태 팩토리 테이블

			public: // ==== [생성자 및 소멸자] ====
				SamplerStateFactoryManager();
				~SamplerStateFactoryManager();

			public: // ==== [Setup] ====
				/**
				 * @brief 샘플러 상태 팩토리 매니저 자원 해제
				 * @details
				 * 샘플러 상태 팩토리 테이블 자원 해제
				 */
				void Release();

			public: // ==== [샘플러 상태 생성] ====
				/**
				 * @brief 샘플러 상태 생성
				 * @details
				 * SamplerStateType에 맞는 샘플러 상태 생성
				 * @param device : DirectX11 장치 참조용 포인터
				 * @param samplerStateType : 렌더 리소스 버퍼 타입
				 * @return 생성된 샘플러 상태 포인터
				 */
				ID3D11SamplerState* CreateSamplerState(ID3D11Device* device, Type::SamplerStateType samplerStateType);
			};
		} // Factory
	} // Graphics
} // Engine
