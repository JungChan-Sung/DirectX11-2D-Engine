#include "Camera.h"
#include "Engine/Config/EngineConfig.h"
#include "Engine/GameObject/GameObject.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			Camera::Camera(GameObject* owner) : Component(owner)
			{
				// @todo : 최적화
				Transform* transform = owner->GetTransform();
				DirectX::XMVECTOR position = transform->GetPositionVector() * EngineConfig::Instance()->WorldRatio();
				DirectX::XMStoreFloat4x4(&viewMatrix, transform->GetRotationMatrix() * DirectX::XMMatrixTranslationFromVector(-position));

				DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixIdentity());
			}

			Camera::~Camera()
			{
			}

			void XM_CALLCONV Camera::UpdateViewMatrix()
			{
				Transform* transform = owner->GetTransform();

				// 트랜스폼이 갱신 되었으면
				if (transform->IsUpdated())
				{
					DirectX::XMVECTOR position = transform->GetPositionVector() * EngineConfig::Instance()->WorldRatio();
					DirectX::XMStoreFloat4x4(&viewMatrix, transform->GetRotationMatrix() * DirectX::XMMatrixTranslationFromVector(-position));
				}
			}

			void XM_CALLCONV Camera::SetProjection(float width, float height, float nearZ, float farZ)
			{
				float halfWidth = width * 0.5f;
				float halfHeight = height * 0.5f;
#if SCREEN_CROODINATE // 화면 좌표계
				DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixOrthographicOffCenterLH(-halfWidth, halfWidth, halfHeight, -halfHeight, nearZ, farZ));
#else // 데카르트 좌표계
				DirectX::XMStoreFloat4x4(&projectionMatrix, DirectX::XMMatrixOrthographicOffCenterLH(-halfWidth, halfWidth, -halfHeight, halfHeight, nearZ, farZ));
#endif
			}

			DirectX::XMMATRIX XM_CALLCONV Camera::GetViewMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&viewMatrix);
			}

			DirectX::XMMATRIX XM_CALLCONV Camera::GetProjectionMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&projectionMatrix);
			}

			DirectX::XMMATRIX XM_CALLCONV Camera::GetViewProjectionMatrix() const
			{
				return DirectX::XMLoadFloat4x4(&viewMatrix) * DirectX::XMLoadFloat4x4(&projectionMatrix);
			}
		} // Component
	} // GameObject
} // Game
