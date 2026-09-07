#include "SpriteRendererManager.h"
#include <algorithm>
#include "Engine/GameObject/GameObject.h"

namespace Engine
{
	namespace Manager
	{
		namespace Component
		{
			SpriteRendererManager::SpriteRendererManager()
			{
			}

			SpriteRendererManager::~SpriteRendererManager()
			{
				Release();
			}

			bool SpriteRendererManager::Initialize(Observer::ICreateGameObjectSubject* createGameObjectSubject)
			{
				createGameObjectSubject->AttachCreateGameObjectObserver(this);

				return true;
			}

			void SpriteRendererManager::Release()
			{
				spriteRenderers.clear();
			}

			void SpriteRendererManager::Render(ID3D11DeviceContext* deviceContext, DirectX::FXMMATRIX viewProjectionMatrix)
			{
				std::sort(spriteRenderers.begin(), spriteRenderers.end(), SpriteCompareTo);

				for (GameObject::Component::SpriteRenderer* spriteRenderer : spriteRenderers)
					spriteRenderer->Draw(deviceContext, viewProjectionMatrix);
			}

			void SpriteRendererManager::CreateGameObjectUpdate(GameObject::GameObject* createGameObject)
			{
				createGameObject->AttachComponentObserver(this);
			}

			void SpriteRendererManager::AddComponentUpdate(GameObject::Component::Component* addComponent)
			{
				GameObject::Component::SpriteRenderer* spriteRenderer = dynamic_cast<GameObject::Component::SpriteRenderer*>(addComponent);
				if (!spriteRenderer) // spriteRenderer == nullptr
					return;

				if (std::find(spriteRenderers.begin(), spriteRenderers.end(), spriteRenderer) == spriteRenderers.end())
					spriteRenderers.push_back(spriteRenderer);
			}

			void SpriteRendererManager::RemoveComponentUpdate(GameObject::Component::Component* removeComponent)
			{
				GameObject::Component::SpriteRenderer* spriteRenderer = dynamic_cast<GameObject::Component::SpriteRenderer*>(removeComponent);
				if (!spriteRenderer) // spriteRenderer == nullptr
					return;

				SpriteRenderers::const_iterator constIterator = std::find(spriteRenderers.cbegin(), spriteRenderers.cend(), spriteRenderer);
				if (constIterator != spriteRenderers.end())
					spriteRenderers.erase(constIterator);
			}

			bool SpriteRendererManager::SpriteCompareTo(GameObject::Component::SpriteRenderer* lhs, GameObject::Component::SpriteRenderer* rhs)
			{
				return lhs->GetSprite()->GetOrder() < rhs->GetSprite()->GetOrder();
			}
		} // Component
	} // Manager
} // Engine
