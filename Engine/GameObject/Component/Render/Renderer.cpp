#include "Renderer.h"
#include "Engine/GameObject/GameObject.h"

namespace Engine
{
	namespace GameObject
	{
		namespace Component
		{
			Renderer::Renderer(GameObject* owner) : Component(owner)
			{
			}

			void Renderer::Draw(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix)
			{
				// 컴포넌트나 게임 오브젝트가 비활성화면 그리기 수행X
				if (!isActive || !owner->IsActive()) // isActive == false || owner->IsActive == false
					return;

				// 렌더러 작업 수행
				Operate(deviceContext, viewProjectionMatrix);
			}

			ComponentType Renderer::GetType() const
			{
				return ComponentType::RENDER;
			}
		} // Component
	} // GameObject
} // Engine
