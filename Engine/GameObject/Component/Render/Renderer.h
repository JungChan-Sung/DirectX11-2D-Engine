#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include "Engine/GameObject/Component/Component.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			/**
			 * @brief 렌더러 추상화 클래스 : 컴포넌트 상속
			 * @date 2024-10-14
			 * @class
			 * 렌더러 컴포넌트들의 기반 클래스
			 */
			class Renderer abstract : public Component
			{
			public: // ==== [생성자] ====
				/** @param owner : 컴포넌트 소유자의 게임 오브젝트 참조용 포인터 */
				Renderer(GameObject* owner);

			public: // ==== [렌더러 동작] ====
				/**
				 * @brief 렌더러 그리기 수행
				 * @param deviceContext : DirectX11 장치 내용 참조용 포인터
				 * @param viewProjectionMatrix : 렌더링될 뷰 * 투영 행렬
				 */
				void Draw(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix);

			public: // ==== [Get] ====
				/** @return 컴포넌트 종류 */
				virtual ComponentType GetType() const override;

			protected: // ==== [렌더러 작업] ====
				/**
				 * @brief 렌더러 작업 수행
				 * @param deviceContext : DirectX11 장치 내용 참조용 포인터
				 * @param viewProjectionMatrix : 렌더링될 뷰 * 투영 행렬
				 */
				virtual void XM_CALLCONV Operate(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix) abstract;
			};
		} // Component
	} // GameObject
} // Engine
