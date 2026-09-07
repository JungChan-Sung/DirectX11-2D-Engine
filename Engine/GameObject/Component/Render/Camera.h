#pragma once

#include <DirectXMath.h>
#include "Engine/GameObject/Component/Component.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			/**
			 * @brief 카메라 컴포넌트 클래스 : 컴포넌트 상속
			 * @date 2024-10-21
			 * @class
			 * 카메라가 보는 곳(View)을 모니터에 투영(Projection)할 행렬 계산 컴포넌트
			 * 2D 엔진이므로 직교(Orthographic) 방식만 사용
			 */
			class Camera : public Component
			{
			private: // ==== [카메라 행렬] ====
				DirectX::XMFLOAT4X4 viewMatrix;			// 뷰 행렬
				DirectX::XMFLOAT4X4 projectionMatrix;	// 투영 행렬

			public: // ==== [생성자 및 소멸자] ====
				Camera(GameObject* owner);
				virtual ~Camera();

			public: // ==== [카메라 동작] ====
				/**
				 * @brief 카메라 행렬 갱신
				 * @date 2024-10-21
				 * @details
				 * 카메라의 뷰 행렬 갱신
				 */
				void XM_CALLCONV UpdateViewMatrix();

			public: // ==== [Set] ====
				/**
				 * @brief 투영 행렬 설정
				 * @param width : 카메라 투영 너비
				 * @param height : 카메라 투영 높이
				 * @param nearZ : 카메라 투영 최소 거리
				 * @param farZ : 카메라 투영 최대 거리
				 */
				void XM_CALLCONV SetProjection(float width, float height, float nearZ, float farZ);

			public: // ==== [Get] ====
				/** @return 뷰(View) 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetViewMatrix() const;
				/** @return 투영(Projection) 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetProjectionMatrix() const;
				/** @return 뷰 * 투영(V * P) 행렬 */
				DirectX::XMMATRIX XM_CALLCONV GetViewProjectionMatrix() const;
			};
		} // Component
	} // GameObject
} // Engine
